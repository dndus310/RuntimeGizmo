#include "Duplication/PCG/OWTPCGDuplicationAdapter.h"

#include "Duplication/PCG/OWTPCGAuthoredProperties.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Grid/PCGPartitionActor.h"
#include "PCGComponent.h"
#include "PCGGraph.h"
#include "PCGManagedResource.h"
#include "RuntimeGen/SchedulingPolicies/PCGSchedulingPolicyBase.h"
#include "Subsystems/PCGSubsystem.h"

FOWTPCGDuplicationAdapter::FOWTPCGDuplicationAdapter()
    : WorldComponents(), Configurations(), GraphCloner(), ReferenceRemapper([this](const UObject& Object)
      {
	      return IsManagedObject(Object);
      }),
      GenerationTracker(OnProceduralChanged)
{
}

FOWTPCGDuplicationAdapter::~FOWTPCGDuplicationAdapter()
{
	Shutdown();
}

bool FOWTPCGDuplicationAdapter::Initialize(UWorld& World, FString& Error)
{
	bool bAccessible = true;
	for (TActorIterator<AActor> It(&World); It && bAccessible; ++It)
	{
		TInlineComponentArray<UPCGComponent*> Components(*It);
		for (UPCGComponent* Component : Components)
		{
			bAccessible = bAccessible && Component->AreManagedResourcesAccessible();
			WorldComponents.Add(Component);
		}
	}
	if (!bAccessible)
	{
		Error = TEXT("SourceBusy: PCG resource ownership is being changed in this world.");
	}
	return bAccessible;
}

bool FOWTPCGDuplicationAdapter::ValidateObject(const UObject& Object, FString& Error) const
{
	UPCGComponent* Component = const_cast<UPCGComponent*>(CastChecked<UPCGComponent>(&Object));
	const bool bNeedsWorldActor = Component->IsPartitioned() || Component->IsManagedByRuntimeGenSystem();
	UPCGSubsystem* Subsystem = Component->GetSubsystem();
	const bool bHasSubsystem = !bNeedsWorldActor || Subsystem;
	const bool bHasWorldActor = !bNeedsWorldActor || (Subsystem && Subsystem->FindPCGWorldActor());
	const bool bIdle = bHasWorldActor && !IsBusyWithLocals(*Component);
	const UPCGGraph* Graph = bIdle ? Component->GetGraph() : nullptr;
	const bool bRuntimeGraph = Graph && !Graph->IsEditorOnly();

	if (!bHasSubsystem)
	{
		Error = TEXT("MissingPCGSubsystem: the source world has no runtime PCG subsystem.");
	}
	else if (!bHasWorldActor)
	{
		Error = TEXT("MissingPCGWorldActor: partitioned and runtime-scheduled generation require a "
		             "configured PCG world actor.");
	}
	else if (!bIdle)
	{
		Error = FString::Printf(TEXT("SourceBusy: %s or one of its local PCG components is generating or cleaning up."),
		                        *Component->GetPathName());
	}
	else if (!Graph)
	{
		Error = FString::Printf(TEXT("%s has no available PCG graph."), *Component->GetPathName());
	}
	else if (!bRuntimeGraph)
	{
		Error = FString::Printf(TEXT("%s uses an editor-only PCG graph."), *Component->GetPathName());
	}
	return bRuntimeGraph;
}

bool FOWTPCGDuplicationAdapter::CaptureObject(UObject& Object, FString& Error)
{
	UPCGComponent* Component = CastChecked<UPCGComponent>(&Object);
	bool bCaptured = true;
	if (!IsManagedObject(*Component))
	{
		TUniquePtr<FOWTPCGConfigurationSnapshot> Captured = MakeUnique<FOWTPCGConfigurationSnapshot>(*Component);
		bCaptured = CaptureGraphInstance(*Component, *Captured, Error) &&
		            ReferenceRemapper.Validate(*Captured->Configuration, TMap<UObject*, UObject*>(), Error);
		if (bCaptured)
		{
			Configurations.Add(MoveTemp(Captured));
		}
	}
	return bCaptured;
}

void FOWTPCGDuplicationAdapter::VisitCapturedReferences(TFunctionRef<void(UObject*&, bool)> Visitor)
{
	for (const TUniquePtr<FOWTPCGConfigurationSnapshot>& Captured : Configurations)
	{
		ReferenceRemapper.VisitCapturedReferences(*Captured->Configuration, Visitor);
	}
}

void FOWTPCGDuplicationAdapter::PrepareDestination(UObject& Destination)
{
	if (UPCGComponent* Component = Cast<UPCGComponent>(&Destination))
	{
		Component->bActivated = false;
	}
}

bool FOWTPCGDuplicationAdapter::Restore(TMap<UObject*, UObject*>& Mapping, FString& Error)
{
	bool bRestored = true;
	for (const TUniquePtr<FOWTPCGConfigurationSnapshot>& Captured : Configurations)
	{
		UPCGComponent* Target = FindTarget(*Captured, Mapping);
		bRestored = bRestored && (!Target || RestoreConfiguration(*Captured, *Target, Mapping, Error));
	}
	// All owned graph/policy objects must be mapped before cross-component references are restored.
	for (const TUniquePtr<FOWTPCGConfigurationSnapshot>& Captured : Configurations)
	{
		UPCGComponent* Target = FindTarget(*Captured, Mapping);
		bRestored = bRestored && (!Target || ReferenceRemapper.Remap(*Target, Mapping, Error));
	}
	return bRestored;
}

bool FOWTPCGDuplicationAdapter::ValidateReferences(const TMap<UObject*, UObject*>& Mapping, FString& Error)
{
	bool bValid = true;
	for (const TUniquePtr<FOWTPCGConfigurationSnapshot>& Captured : Configurations)
	{
		bValid = bValid && ReferenceRemapper.Validate(*Captured->Configuration, Mapping, Error);
	}
	return bValid;
}

void FOWTPCGDuplicationAdapter::PrepareCommit(const FGuid& OperationId, const FOWTDuplicationOptions& Options,
                                              const TMap<UObject*, UObject*>& Mapping)
{
	for (const TUniquePtr<FOWTPCGConfigurationSnapshot>& Captured : Configurations)
	{
		UPCGComponent* Target = FindTarget(*Captured, Mapping);
		if (IsValid(Target))
		{
			GenerationTracker.Observe(*Target, *Captured, OperationId, Options);
		}
	}
}

void FOWTPCGDuplicationAdapter::Commit(const FGuid& OperationId, const FOWTDuplicationOptions& Options,
                                       const TMap<UObject*, UObject*>& Mapping)
{
	GenerationTracker.RequestGenerations();
	// Capture state is no longer needed; only generation observation outlives the operation.
	Configurations.Reset();
	GraphCloner.Reset();
	WorldComponents.Reset();
}

void FOWTPCGDuplicationAdapter::Tick(float DeltaTime)
{
	GenerationTracker.Tick();
}

void FOWTPCGDuplicationAdapter::Rollback(const TMap<UObject*, UObject*>& Mapping)
{
	for (const TUniquePtr<FOWTPCGConfigurationSnapshot>& Captured : Configurations)
	{
		UPCGComponent* Target = FindTarget(*Captured, Mapping);
		UPCGSubsystem* Subsystem = Target ? Target->GetSubsystem() : nullptr;
		if (Target)
		{
			Target->bActivated = false;
			Target->CancelGeneration();
		}
		if (Target && Target->AreManagedResourcesAccessible())
		{
			Target->CleanupLocalImmediate(true, true);
		}
		if (Subsystem)
		{
			Subsystem->UnregisterPCGComponent(Target, true);
		}
	}
}

void FOWTPCGDuplicationAdapter::Shutdown()
{
	GenerationTracker.Shutdown();
	OnProceduralChanged.Clear();
}

bool FOWTPCGDuplicationAdapter::IsManagedObject(const UObject& Object) const
{
	const UPCGComponent* PCGComponent = Cast<UPCGComponent>(&Object);
	const bool bGeneratedOutput = Object.IsA<UPCGManagedResource>() || Object.IsA<APCGPartitionActor>() ||
	                              (PCGComponent && PCGComponent->IsLocalComponent());
	const bool bManagedByResource = !bGeneratedOutput && WorldComponents.ContainsByPredicate([&Object](const TWeakObjectPtr<UPCGComponent>& Entry)
	{
		const UPCGComponent* Component = Entry.Get();
		const UObject* Candidate = &Object;
		return Component && Component->AreManagedResourcesAccessible() &&
		       Component->IsAnyObjectManagedByResource(MakeArrayView(&Candidate, 1));
	});
	return bGeneratedOutput || bManagedByResource;
}

bool FOWTPCGDuplicationAdapter::OwnsProperty(const UObject& Object, const FProperty& Property) const
{
	// Subclass properties remain reflected configuration copied by the core.
	const bool bPCGComponent = Object.IsA<UPCGComponent>();
	const bool bBaseSchema = AcceptsObject(Object) && Property.GetOwner<UClass>() == UPCGComponent::StaticClass();
	return bPCGComponent ? bBaseSchema : OwnsObject(Object);
}

bool FOWTPCGDuplicationAdapter::OwnsObject(const UObject& Object) const
{
	return GraphCloner.IsOwnedSource(Object);
}

TArray<FOWTProceduralComponentSnapshot> FOWTPCGDuplicationAdapter::GetProceduralComponents() const
{
	return GenerationTracker.GetSnapshots();
}

bool FOWTPCGDuplicationAdapter::IsBusy(const UPCGComponent& Component)
{
	return Component.IsGenerating() || Component.IsCleaningUp() || !Component.AreManagedResourcesAccessible();
}

bool FOWTPCGDuplicationAdapter::IsBusyWithLocals(UPCGComponent& Component)
{
	bool bBusy = IsBusy(Component);
	if (UPCGSubsystem* Subsystem = Component.GetSubsystem())
	{
		Subsystem->ForAllRegisteredLocalComponents(&Component, [&bBusy](UPCGComponent* Local)
		{
			bBusy |= Local && IsBusy(*Local);
		});
	}
	return bBusy;
}

bool FOWTPCGDuplicationAdapter::ValidateDestinationLifecycle(const UPCGComponent& Target, FString& Error)
{
	const bool bCleaningUp = Target.IsCleaningUp();
	const bool bGenerating = Target.IsGenerating();
	const bool bGenerated = Target.bGenerated;

	if (bCleaningUp)
	{
		Error = TEXT("DestinationPCGLifecycleViolation: a construction callback started PCG cleanup before "
		             "authored commit. The callback must cooperate with duplication suppression; cleanup-only "
		             "engine tasks cannot be cancelled through the public runtime API.");
	}
	else if (bGenerating)
	{
		Error = TEXT("A custom construction callback started PCG before configuration restoration. A "
		             "cooperative duplication participant is required.");
	}
	else if (bGenerated)
	{
		Error = TEXT("A custom construction callback generated PCG before authored commit; a cooperative "
		             "adapter is required.");
	}
	return !bCleaningUp && !bGenerating && !bGenerated;
}

bool FOWTPCGDuplicationAdapter::CaptureGraphInstance(UPCGComponent& Component, FOWTPCGConfigurationSnapshot& Captured,
                                                     FString& Error)
{
	UPCGGraphInstance* SourceGraph = Component.GetGraphInstance();
	UPCGGraphInstance* SnapshotGraph = Captured.Configuration->GetGraphInstance();
	UPCGGraphInterface* Parent = GraphCloner.Clone(SourceGraph->Graph, *Component.GetOwner(), *Captured.Configuration, Error);
	const bool bCloned = Error.IsEmpty();
	if (bCloned)
	{
		SnapshotGraph->SetGraph(Parent);
		SnapshotGraph->CopyParameterOverrides(SourceGraph);
		GraphCloner.RecordCopy(SourceGraph, SnapshotGraph);
		GraphCloner.AddOwnedSource(SourceGraph);
		for (const TPair<UObject*, UObject*>& PolicyCopy : Captured.PolicyCopies)
		{
			GraphCloner.AddOwnedSource(PolicyCopy.Key);
		}
	}
	return bCloned;
}

bool FOWTPCGDuplicationAdapter::RestoreConfiguration(const FOWTPCGConfigurationSnapshot& Captured, UPCGComponent& Target,
                                                     TMap<UObject*, UObject*>& Mapping, FString& Error)
{
	Target.bActivated = false;
	const bool bLifecycleValid = ValidateDestinationLifecycle(Target, Error);
	if (bLifecycleValid)
	{
		Target.SetPropertiesFromOriginal(Captured.Configuration.Get());
		// Partition settings remain dormant here; SetIsPartitioned would perform cleanup/registration before commit.
		FOWTPCGAuthoredProperties::Copy(*UPCGComponent::StaticClass(), Captured.Configuration.Get(), &Target);
		RestoreSchedulingPolicy(Captured, Target, Mapping);
	}
	return bLifecycleValid && RestoreGraphInstance(Captured, Target, Mapping, Error);
}

void FOWTPCGDuplicationAdapter::RestoreSchedulingPolicy(const FOWTPCGConfigurationSnapshot& Captured,
                                                        UPCGComponent& Target, TMap<UObject*, UObject*>& Mapping)
{
	UPCGSchedulingPolicyBase* Policy = Captured.Configuration->SchedulingPolicy;
	if (Policy)
	{
		TMap<UObject*, UObject*> DestinationPolicies;
		Target.SchedulingPolicy = FOWTPCGConfigurationSnapshot::CopySchedulingPolicy(*Policy, Target, DestinationPolicies);
		// Captured.PolicyCopies maps source -> snapshot; DestinationPolicies maps snapshot -> destination.
		for (const TPair<UObject*, UObject*>& PolicyCopy : Captured.PolicyCopies)
		{
			if (UObject* DestinationPolicyObject = DestinationPolicies.FindRef(PolicyCopy.Value))
			{
				Mapping.Add(PolicyCopy.Key, DestinationPolicyObject);
			}
		}
	}
}

bool FOWTPCGDuplicationAdapter::RestoreGraphInstance(const FOWTPCGConfigurationSnapshot& Captured, UPCGComponent& Target,
                                                     TMap<UObject*, UObject*>& Mapping, FString& Error)
{
	// Each generated destination owns its parent instance chain as well as its default GraphInstance.
	UPCGGraphInstance* SnapshotGraph = Captured.Configuration->GetGraphInstance();
	UPCGGraphInterface* Parent = GraphCloner.Clone(SnapshotGraph->Graph, *Captured.Configuration, Target, Error);
	const bool bCloned = Error.IsEmpty();
	if (bCloned)
	{
		Target.GetGraphInstance()->SetGraph(Parent);
		Target.GetGraphInstance()->CopyParameterOverrides(SnapshotGraph);
		GraphCloner.RecordCopy(SnapshotGraph, Target.GetGraphInstance());
		if (UPCGComponent* Original = Captured.Source.Get())
		{
			Mapping.Add(Original->GetGraphInstance(), Target.GetGraphInstance());
			if (Original->SchedulingPolicy)
			{
				Mapping.Add(Original->SchedulingPolicy, Target.SchedulingPolicy);
			}
		}
		// Source graphs map to snapshot copies and snapshot graphs to destination copies; compose the two hops.
		for (const TPair<const UObject*, UObject*>& Copy : GraphCloner.GetCopies())
		{
			UObject* TargetGraph = Copy.Value->IsIn(Captured.Configuration.Get()) ? GraphCloner.FindCopy(Copy.Value) : nullptr;
			if (TargetGraph)
			{
				Mapping.Add(const_cast<UObject*>(Copy.Key), TargetGraph);
			}
		}
	}
	return bCloned;
}

UPCGComponent* FOWTPCGDuplicationAdapter::FindTarget(const FOWTPCGConfigurationSnapshot& Captured,
                                                     const TMap<UObject*, UObject*>& Mapping) const
{
	return Cast<UPCGComponent>(Mapping.FindRef(Captured.Source.Get()));
}
