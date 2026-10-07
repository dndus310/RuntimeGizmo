#include "Duplication/OWTPCGDuplicationAdapter.h"

#include "Duplication/OWTDuplicationReferences.h"
#include "Duplication/OWTPCGComponentConfiguration.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Grid/PCGPartitionActor.h"
#include "Helpers/PCGHelpers.h"
#include "PCGComponent.h"
#include "PCGGraph.h"
#include "PCGManagedResource.h"
#include "RuntimeGen/SchedulingPolicies/PCGSchedulingPolicyBase.h"
#include "Subsystems/PCGSubsystem.h"
#include "UObject/StrongObjectPtr.h"
#include "UObject/UObjectHash.h"
#include "UObject/UnrealType.h"

namespace
{
constexpr EPropertyFlags ExcludedConfigurationProperties = CPF_Transient | CPF_DuplicateTransient |
                                                           CPF_NonPIEDuplicateTransient | CPF_Deprecated |
                                                           CPF_EditorOnly | CPF_SkipSerialization;

bool IsReflectedConfigurationProperty(const FProperty& Property)
{
	if (!OWTPCGConfiguration::IsAuthoredProperty(Property))
	{
		return false;
	}
	const FStructProperty* StructProperty = CastField<FStructProperty>(&Property);
	if (StructProperty)
	{
		// The tool container exposes opaque FInstancedStruct values through its own runtime API below.
		if (StructProperty->Struct == FPCGInteractiveToolDataContainer::StaticStruct())
		{
			return false;
		}
	}
	return true;
}

UPCGSchedulingPolicyBase* CopyPolicy(UPCGSchedulingPolicyBase& Source, UPCGComponent& Owner,
                                     TMap<UObject*, UObject*>& Copies)
{
	FObjectDuplicationParameters Parameters(&Source, &Owner);
	Parameters.DestName = MakeUniqueObjectName(&Owner, Source.GetClass());
	Parameters.CreatedObjects = &Copies;
	Parameters.FlagMask &= ~RF_Standalone;
	return CastChecked<UPCGSchedulingPolicyBase>(StaticDuplicateObjectEx(Parameters));
}

TArray<UObject*> GetPolicyObjects(UPCGSchedulingPolicyBase* Policy)
{
	TArray<UObject*> Objects;
	if (Policy)
	{
		Objects.Add(Policy);
		GetObjectsWithOuter(Policy, Objects, true);
	}
	return Objects;
}

bool IsBusy(const UPCGComponent& Component)
{
	if (Component.IsGenerating())
	{
		return true;
	}
	if (Component.IsCleaningUp())
	{
		return true;
	}
	return !Component.AreManagedResourcesAccessible();
}

struct FPCGConfiguration
{
	explicit FPCGConfiguration(UPCGComponent& Component);

	TWeakObjectPtr<UPCGComponent> Source;
	TStrongObjectPtr<UPCGComponent> Configuration;
	TMap<UObject*, UObject*> PolicyCopies;
	bool bWasGenerated;
	bool bActivated;
};

FPCGConfiguration::FPCGConfiguration(UPCGComponent& Component)
    : Source(&Component), Configuration(NewObject<UPCGComponent>(GetTransientPackage())), PolicyCopies(),
      bWasGenerated(Component.bGenerated), bActivated(Component.bActivated)
{
	// A runtime trigger may have been changed from native code before the source creates its policy.
	// Capture the public configuration without the engine's source-policy ensure or any source mutation.
	// The engine restore API also overwrites inherited tags, so capture them alongside the base schema.
	Configuration->ComponentTags = Component.ComponentTags;
	OWTPCGConfiguration::CopyAuthoredProperties(*UPCGComponent::StaticClass(), &Component, Configuration.Get());

	// Instanced graph and scheduling policy have ownership and refresh contracts beyond reflected values.
	Configuration->GetGraphInstance()->SetGraph(Component.GetGraphInstance()->Graph);
	Configuration->GetGraphInstance()->CopyParameterOverrides(Component.GetGraphInstance());
	Configuration->SetSchedulingPolicyClass(Component.SchedulingPolicyClass);
	if (Component.SchedulingPolicy)
	{
		Configuration->SchedulingPolicy = CopyPolicy(*Component.SchedulingPolicy, *Configuration, PolicyCopies);
	}

	// Commit restores activation only after authored objects and references are ready.
	Configuration->bActivated = false;
	if (UPCGSubsystem* Subsystem = Component.GetSubsystem())
	{
		Subsystem->ForAllRegisteredLocalComponents(&Component,
		                                           [this](UPCGComponent* Local)
		                                           {
			                                           if (Local)
			                                           {
				                                           bWasGenerated |= Local->bGenerated;
			                                           }
		                                           });
	}
}

struct FPCGLocalObservation
{
	FPCGLocalObservation() : LastGenerationTask(InvalidPCGTaskId), bObservedCompletion(false), bCancelled(false)
	{
	}

	FDelegateHandle Started;
	FDelegateHandle Generated;
	FDelegateHandle Cancelled;
	FDelegateHandle Cleaned;
	FPCGTaskId LastGenerationTask;
	bool bObservedCompletion;
	bool bCancelled;
};

struct FPCGObservation
{
	FPCGObservation() : bActivated(false), bRequestGeneration(false), bOriginalCancelled(false)
	{
	}

	FOWTProceduralComponentSnapshot Snapshot;
	FDelegateHandle Started;
	FDelegateHandle Generated;
	FDelegateHandle Cancelled;
	FDelegateHandle Cleaned;
	TMap<TWeakObjectPtr<UPCGComponent>, FPCGLocalObservation> Locals;
	bool bActivated;
	bool bRequestGeneration;
	bool bOriginalCancelled;
};

class FPCGDuplicationAdapter final : public IOWTDuplicationAdapter
{
public:
	virtual ~FPCGDuplicationAdapter() override;

	virtual bool Initialize(UWorld& World, FString& Error) override;
	virtual bool ValidateObject(const UObject& Object, FString& Error) const override;
	virtual bool CaptureObject(UObject& Object, FString& Error) override;
	virtual void VisitCapturedReferences(TFunctionRef<void(UObject*&, bool)> Visitor) override;
	virtual void PrepareDestination(UObject& Destination) override;
	virtual bool Restore(TMap<UObject*, UObject*>& Mapping, FString& Error) override;
	virtual bool ValidateReferences(const TMap<UObject*, UObject*>& Mapping, FString& Error) override;
	virtual void PrepareCommit(const FGuid& OperationId, const FOWTDuplicationOptions& Options,
	                           const TMap<UObject*, UObject*>& Mapping) override;
	virtual void Commit(const FGuid& OperationId, const FOWTDuplicationOptions& Options,
	                    const TMap<UObject*, UObject*>& Mapping) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Rollback(const TMap<UObject*, UObject*>& Mapping) override;
	virtual void Shutdown() override;

	virtual bool IsManagedObject(const UObject& Object) const override;
	virtual bool OwnsProperty(const UObject& Object, const FProperty& Property) const override;
	virtual bool OwnsObject(const UObject& Object) const override;
	virtual TArray<FOWTProceduralComponentSnapshot> GetProceduralComponents() const override;

private:
	bool RestoreConfiguration(const FPCGConfiguration& Captured, UPCGComponent& Target,
	                          TMap<UObject*, UObject*>& Mapping, FString& Error);
	UPCGGraphInterface* CloneOwnedGraph(UPCGGraphInterface* Graph, const UObject& SourceOwner,
	                                    UObject& DestinationOwner, FString& Error);
	bool VisitStruct(const UStruct& Struct, void* Memory, const TMap<UObject*, UObject*>& Mapping, bool bValidateOnly,
	                 FString& Error);

	bool VisitProperty(FProperty& Property, void* Value, const TMap<UObject*, UObject*>& Mapping, bool bValidateOnly,
	                   FString& Error);
	bool VisitConfiguration(UPCGComponent& Component, const TMap<UObject*, UObject*>& Mapping, bool bValidateOnly,
	                        FString& Error);

	static void ConfigureGeneration(FPCGObservation& Observation, const UPCGComponent& Target,
	                                const FPCGConfiguration& Captured, const FOWTDuplicationOptions& Options);
	void RequestGeneration(int32 Index);
	void RefreshLocalObservations(int32 Index);
	void UpdateLocalAggregate(int32 Index);
	void SetState(int32 Index, EOWTProceduralState State, const FString& Reason = FString());

	void BindObservation(int32 Index, UPCGComponent& Target);
	void BindLocal(int32 Index, UPCGComponent& Component);
	static void UnbindLocal(UPCGComponent* Component, const FPCGLocalObservation& Observation);
	void ObserveLocalStart(int32 Index, UPCGComponent* Component);
	void ObserveLocalCompletion(int32 Index, UPCGComponent* Component);
	FPCGLocalObservation* FindObservedLocal(int32 Index, UPCGComponent* Component);

private:
	TArray<TWeakObjectPtr<UPCGComponent>> WorldComponents;
	TArray<TUniquePtr<FPCGConfiguration>> Configurations;
	TArray<TUniquePtr<FPCGObservation>> Observations;
	TSet<const UObject*> SourceGraphs;
	TSet<const UObject*> CloningGraphs;
	TMap<const UObject*, UObject*> GraphCopies;
};

FPCGDuplicationAdapter::~FPCGDuplicationAdapter()
{
	Shutdown();
}

bool FPCGDuplicationAdapter::Initialize(UWorld& World, FString& Error)
{
	for (TActorIterator<AActor> It(&World); It; ++It)
	{
		TInlineComponentArray<UPCGComponent*> Components(*It);
		for (UPCGComponent* Component : Components)
		{
			if (!Component->AreManagedResourcesAccessible())
			{
				Error = TEXT("SourceBusy: PCG resource ownership is being changed in this world.");
				return false;
			}
			WorldComponents.Add(Component);
		}
	}
	return true;
}

bool FPCGDuplicationAdapter::IsManagedObject(const UObject& Object) const
{
	if (Object.IsA<UPCGManagedResource>())
	{
		return true;
	}
	if (Object.IsA<APCGPartitionActor>())
	{
		return true;
	}
	if (const UPCGComponent* Component = Cast<UPCGComponent>(&Object))
	{
		if (Component->IsLocalComponent())
		{
			return true;
		}
	}
	const UObject* Candidate = &Object;
	for (const TWeakObjectPtr<UPCGComponent>& Entry : WorldComponents)
	{
		const UPCGComponent* Component = Entry.Get();
		if (!Component)
		{
			continue;
		}
		if (!Component->AreManagedResourcesAccessible())
		{
			continue;
		}
		if (Component->IsAnyObjectManagedByResource(MakeArrayView(&Candidate, 1)))
		{
			return true;
		}
	}
	return false;
}

bool FPCGDuplicationAdapter::ValidateObject(const UObject& Object, FString& Error) const
{
	UPCGComponent* Component = const_cast<UPCGComponent*>(CastChecked<UPCGComponent>(&Object));
	bool bNeedsWorldActor = Component->IsPartitioned();
	bNeedsWorldActor |= Component->IsManagedByRuntimeGenSystem();
	if (bNeedsWorldActor)
	{
		UPCGSubsystem* Subsystem = Component->GetSubsystem();
		if (!Subsystem)
		{
			Error = TEXT("MissingPCGSubsystem: the source world has no runtime PCG subsystem.");
			return false;
		}
		if (!Subsystem->FindPCGWorldActor())
		{
			Error = TEXT("MissingPCGWorldActor: partitioned and runtime-scheduled generation require a "
			             "configured PCG world actor.");
			return false;
		}
	}
	bool bBusy = IsBusy(*Component);
	if (UPCGSubsystem* Subsystem = Component->GetSubsystem())
	{
		Subsystem->ForAllRegisteredLocalComponents(Component,
		                                           [&bBusy](UPCGComponent* Local)
		                                           {
			                                           if (Local)
			                                           {
				                                           bBusy |= IsBusy(*Local);
			                                           }
		                                           });
	}
	if (bBusy)
	{
		Error = FString::Printf(TEXT("SourceBusy: %s or one of its local PCG components is generating or cleaning up."),
		                        *Component->GetPathName());
		return false;
	}
	if (!Component->GetGraph())
	{
		Error = FString::Printf(TEXT("%s has no available PCG graph."), *Component->GetPathName());
		return false;
	}
	if (Component->GetGraph()->IsEditorOnly())
	{
		Error = FString::Printf(TEXT("%s uses an editor-only PCG graph."), *Component->GetPathName());
		return false;
	}
	return true;
}

bool FPCGDuplicationAdapter::CaptureObject(UObject& Object, FString& Error)
{
	UPCGComponent* Component = CastChecked<UPCGComponent>(&Object);
	AActor& Actor = *Component->GetOwner();
	if (IsManagedObject(*Component))
	{
		return true;
	}
	TUniquePtr<FPCGConfiguration> Captured = MakeUnique<FPCGConfiguration>(*Component);
	UPCGGraphInstance* SnapshotGraph = Captured->Configuration->GetGraphInstance();
	UPCGGraphInterface* Parent =
	    CloneOwnedGraph(Component->GetGraphInstance()->Graph, Actor, *Captured->Configuration, Error);
	if (!Error.IsEmpty())
	{
		return false;
	}
	SnapshotGraph->SetGraph(Parent);
	SnapshotGraph->CopyParameterOverrides(Component->GetGraphInstance());
	GraphCopies.Add(Component->GetGraphInstance(), SnapshotGraph);
	SourceGraphs.Add(Component->GetGraphInstance());
	for (const TPair<UObject*, UObject*>& PolicyCopy : Captured->PolicyCopies)
	{
		SourceGraphs.Add(PolicyCopy.Key);
	}
	if (!VisitConfiguration(*Captured->Configuration, {}, true, Error))
	{
		return false;
	}
	Configurations.Add(MoveTemp(Captured));
	return true;
}

bool FPCGDuplicationAdapter::OwnsProperty(const UObject& Object, const FProperty& Property) const
{
	if (Object.IsA<UPCGComponent>())
	{
		if (!AcceptsObject(Object))
		{
			return false;
		}
		return Property.GetOwner<UClass>() == UPCGComponent::StaticClass();
	}
	return OwnsObject(Object);
}

bool FPCGDuplicationAdapter::OwnsObject(const UObject& Object) const
{
	return SourceGraphs.Contains(&Object);
}

void FPCGDuplicationAdapter::VisitCapturedReferences(TFunctionRef<void(UObject*&, bool)> Visitor)
{
	for (const auto& Captured : Configurations)
	{
		for (TFieldIterator<FProperty> Property(UPCGComponent::StaticClass(), EFieldIteratorFlags::ExcludeSuper); Property; ++Property)
		{
			if (!IsReflectedConfigurationProperty(**Property))
			{
				continue;
			}
			OWTDuplication::VisitReferences(**Property,
				Property->ContainerPtrToValuePtr<void>(Captured->Configuration.Get()), Visitor);
		}

		for (UObject* PolicyObject : GetPolicyObjects(Captured->Configuration->SchedulingPolicy))
		{
			for (TFieldIterator<FProperty> Property(PolicyObject->GetClass()); Property; ++Property)
			{
				if (Property->HasAnyPropertyFlags(ExcludedConfigurationProperties))
				{
					continue;
				}
				OWTDuplication::VisitReferences(**Property, Property->ContainerPtrToValuePtr<void>(PolicyObject),
				                                Visitor);
			}
		}
		for (UPCGGraphInstance* Graph = Captured->Configuration->GetGraphInstance(); Graph;
		     Graph = Cast<UPCGGraphInstance>(Graph->Graph))
		{
			if (!Graph->IsIn(Captured->Configuration.Get()))
			{
				break;
			}
			FInstancedPropertyBag& Bag = Graph->ParametersOverrides.Parameters;
			if (const UStruct* Struct = Bag.GetPropertyBagStruct())
			{
				for (TFieldIterator<FProperty> Property(Struct); Property; ++Property)
				{
					OWTDuplication::VisitReferences(
					    **Property, Property->ContainerPtrToValuePtr<void>(Bag.GetMutableValue().GetMemory()), Visitor);
				}
			}
		}
		for (auto& Data : Captured->Configuration->ToolDataContainer.ToolData)
		{
			if (!Data.IsValid())
			{
				continue;
			}
			for (TFieldIterator<FProperty> Property(Data.GetScriptStruct()); Property; ++Property)
			{
				OWTDuplication::VisitReferences(
				    **Property, Property->ContainerPtrToValuePtr<void>(Data.GetMutableMemory()), Visitor);
			}
		}
	}
}

void FPCGDuplicationAdapter::PrepareDestination(UObject& Destination)
{
	if (UPCGComponent* Component = Cast<UPCGComponent>(&Destination))
	{
		Component->bActivated = false;
	}
}

bool FPCGDuplicationAdapter::Restore(TMap<UObject*, UObject*>& Mapping, FString& Error)
{
	for (const TUniquePtr<FPCGConfiguration>& Captured : Configurations)
	{
		UPCGComponent* Target = Cast<UPCGComponent>(Mapping.FindRef(Captured->Source.Get()));
		if (!Target)
		{
			continue;
		}
		if (!RestoreConfiguration(*Captured, *Target, Mapping, Error))
		{
			return false;
		}
	}
	// All owned graph/policy objects must be mapped before cross-component references are restored.
	for (const TUniquePtr<FPCGConfiguration>& Captured : Configurations)
	{
		UPCGComponent* Target = Cast<UPCGComponent>(Mapping.FindRef(Captured->Source.Get()));
		if (!Target)
		{
			continue;
		}
		if (!VisitConfiguration(*Target, Mapping, false, Error))
		{
			return false;
		}
	}
	return true;
}

bool FPCGDuplicationAdapter::RestoreConfiguration(const FPCGConfiguration& Captured, UPCGComponent& Target,
                                                  TMap<UObject*, UObject*>& Mapping, FString& Error)
{
	Target.bActivated = false;
	if (Target.IsCleaningUp())
	{
		Error = TEXT("DestinationPCGLifecycleViolation: a construction callback started PCG cleanup before "
		             "authored commit. The callback must cooperate with duplication suppression; cleanup-only "
		             "engine tasks cannot be cancelled through the public runtime API.");
		return false;
	}
	if (Target.IsGenerating())
	{
		Error = TEXT("A custom construction callback started PCG before configuration restoration. A "
		             "cooperative duplication participant is required.");
		return false;
	}
	if (Target.bGenerated)
	{
		Error = TEXT("A custom construction callback generated PCG before authored commit; a cooperative "
		             "adapter is required.");
		return false;
	}
	Target.SetPropertiesFromOriginal(Captured.Configuration.Get());
	// Partition settings remain dormant here; SetIsPartitioned would perform cleanup/registration before commit.
	OWTPCGConfiguration::CopyAuthoredProperties(*UPCGComponent::StaticClass(), Captured.Configuration.Get(), &Target);
	if (UPCGSchedulingPolicyBase* Policy = Captured.Configuration->SchedulingPolicy)
	{
		TMap<UObject*, UObject*> DestinationPolicies;
		Target.SchedulingPolicy = CopyPolicy(*Policy, Target, DestinationPolicies);
		for (const TPair<UObject*, UObject*>& PolicyCopy : Captured.PolicyCopies)
		{
			if (UObject* DestinationPolicyObject = DestinationPolicies.FindRef(PolicyCopy.Value))
			{
				Mapping.Add(PolicyCopy.Key, DestinationPolicyObject);
			}
		}
	}
	// Each generated destination owns its parent instance chain as well as its default GraphInstance.
	UPCGGraphInterface* Parent =
	    CloneOwnedGraph(Captured.Configuration->GetGraphInstance()->Graph, *Captured.Configuration, Target, Error);
	if (!Error.IsEmpty())
	{
		return false;
	}
	Target.GetGraphInstance()->SetGraph(Parent);
	Target.GetGraphInstance()->CopyParameterOverrides(Captured.Configuration->GetGraphInstance());
	GraphCopies.Add(Captured.Configuration->GetGraphInstance(), Target.GetGraphInstance());
	if (UPCGComponent* Original = Captured.Source.Get())
	{
		Mapping.Add(Original->GetGraphInstance(), Target.GetGraphInstance());
		if (Original->SchedulingPolicy)
		{
			Mapping.Add(Original->SchedulingPolicy, Target.SchedulingPolicy);
		}
	}
	for (const TPair<const UObject*, UObject*>& Copy : GraphCopies)
	{
		if (Copy.Value->IsIn(Captured.Configuration.Get()))
		{
			if (UObject* TargetGraph = GraphCopies.FindRef(Copy.Value))
			{
				Mapping.Add(const_cast<UObject*>(Copy.Key), TargetGraph);
			}
		}
	}
	return true;
}

bool FPCGDuplicationAdapter::ValidateReferences(const TMap<UObject*, UObject*>& Mapping, FString& Error)
{
	for (const TUniquePtr<FPCGConfiguration>& Captured : Configurations)
	{
		if (!VisitConfiguration(*Captured->Configuration, Mapping, true, Error))
		{
			return false;
		}
	}
	return true;
}

void FPCGDuplicationAdapter::PrepareCommit(const FGuid& OperationId, const FOWTDuplicationOptions& Options,
                                           const TMap<UObject*, UObject*>& Mapping)
{
	for (const TUniquePtr<FPCGConfiguration>& Captured : Configurations)
	{
		UPCGComponent* Target = Cast<UPCGComponent>(Mapping.FindRef(Captured->Source.Get()));
		if (!IsValid(Target))
		{
			continue;
		}
		TUniquePtr<FPCGObservation> Observation = MakeUnique<FPCGObservation>();
		Observation->Snapshot.Component = Target;
		Observation->Snapshot.OperationId = OperationId;
		Observation->Snapshot.ComponentId = FGuid::NewGuid();
		Observation->Snapshot.ComponentName = Target->GetName();
		Observation->Snapshot.GraphPath = Target->GetGraph()->GetPathName();
		Observation->Snapshot.Trigger = StaticEnum<EPCGComponentGenerationTrigger>()->GetNameStringByValue(
		    static_cast<int64>(Target->GenerationTrigger));
		const int32 Index = Observations.Add(MoveTemp(Observation));
		BindObservation(Index, *Target);
		ConfigureGeneration(*Observations[Index], *Target, *Captured, Options);
	}
}

void FPCGDuplicationAdapter::BindObservation(int32 Index, UPCGComponent& Target)
{
	FPCGObservation& Bound = *Observations[Index];
	Bound.Started = Target.OnPCGGraphStartGeneratingDelegate.AddLambda(
	    [this, Index](UPCGComponent*)
	    {
		    Observations[Index]->bOriginalCancelled = false;
		    ++Observations[Index]->Snapshot.GenerationAttempt;
		    SetState(Index, EOWTProceduralState::Generating);
	    });
	Bound.Generated = Target.OnPCGGraphGeneratedDelegate.AddLambda(
	    [this, Index](UPCGComponent* Component)
	    {
		    Observations[Index]->bOriginalCancelled = false;
		    if (Component->IsPartitioned())
		    {
			    RefreshLocalObservations(Index);
			    return;
		    }
		    SetState(Index, EOWTProceduralState::Ready);
	    });
	Bound.Cancelled = Target.OnPCGGraphCancelledDelegate.AddLambda(
	    [this, Index](UPCGComponent*)
	    {
		    Observations[Index]->bOriginalCancelled = true;
		    SetState(Index, EOWTProceduralState::Cancelled);
	    });
	Bound.Cleaned = Target.OnPCGGraphCleanedDelegate.AddLambda(
	    [this, Index](UPCGComponent*)
	    {
		    Observations[Index]->bOriginalCancelled = false;
		    SetState(Index, EOWTProceduralState::Cleaned);
	    });
}

void FPCGDuplicationAdapter::ConfigureGeneration(FPCGObservation& Observation, const UPCGComponent& Target,
                                                 const FPCGConfiguration& Captured,
                                                 const FOWTDuplicationOptions& Options)
{
	Observation.bActivated = Captured.bActivated;
	Observation.bRequestGeneration = Target.GenerationTrigger == EPCGComponentGenerationTrigger::GenerateOnLoad;
	if (Target.GenerationTrigger == EPCGComponentGenerationTrigger::GenerateOnDemand)
	{
		Observation.bRequestGeneration = Captured.bWasGenerated;
		if (Options.GenerationPolicy == EOWTDuplicationGenerationPolicy::KeepUnGenerated)
		{
			Observation.bRequestGeneration = false;
		}
	}
	if (Observation.bActivated)
	{
		if (Target.IsManagedByRuntimeGenSystem())
		{
			Observation.Snapshot.State = EOWTProceduralState::WaitingForGenerationSource;
		}
		else if (Observation.bRequestGeneration)
		{
			Observation.Snapshot.State = EOWTProceduralState::Scheduled;
		}
	}
}

void FPCGDuplicationAdapter::Commit(const FGuid& OperationId, const FOWTDuplicationOptions& Options,
                                    const TMap<UObject*, UObject*>& Mapping)
{
	for (int32 Index = 0; Index < Observations.Num(); ++Index)
	{
		RequestGeneration(Index);
	}
	Configurations.Reset();
	GraphCopies.Reset();
	SourceGraphs.Reset();
	WorldComponents.Reset();
}

void FPCGDuplicationAdapter::RequestGeneration(int32 Index)
{
	FPCGObservation& Bound = *Observations[Index];
	UPCGComponent* Target = Cast<UPCGComponent>(Bound.Snapshot.Component.Get());
	if (!Target)
	{
		return;
	}
	Target->bActivated = Bound.bActivated;
	if (!Target->bActivated)
	{
		SetState(Index, EOWTProceduralState::NotRequested, TEXT("The source component was inactive."));
		return;
	}
	UPCGSubsystem* Subsystem = Target->GetSubsystem();
	if (!Subsystem)
	{
		SetState(Index, EOWTProceduralState::Failed, TEXT("The destination world has no PCG subsystem."));
		return;
	}
	Subsystem->RegisterOrUpdatePCGComponent(Target);
	if (Target->IsManagedByRuntimeGenSystem())
	{
		SetState(Index, EOWTProceduralState::WaitingForGenerationSource);
		if (IsValid(Target))
		{
			Subsystem->RefreshRuntimeGenComponent(Target);
		}
		return;
	}
	if (!Bound.bRequestGeneration)
	{
		SetState(Index, EOWTProceduralState::NotRequested);
		return;
	}
	SetState(Index, EOWTProceduralState::Scheduled);
	if (!IsValid(Target))
	{
		return;
	}
	if (Target->GenerateLocalGetTaskId(true) == InvalidPCGTaskId)
	{
		SetState(Index, EOWTProceduralState::Failed, TEXT("PCG did not accept the generation request."));
	}
}

void FPCGDuplicationAdapter::Rollback(const TMap<UObject*, UObject*>& Mapping)
{
	for (const TUniquePtr<FPCGConfiguration>& Captured : Configurations)
	{
		if (UPCGComponent* Target = Cast<UPCGComponent>(Mapping.FindRef(Captured->Source.Get())))
		{
			Target->bActivated = false;
			Target->CancelGeneration();
			if (Target->AreManagedResourcesAccessible())
			{
				Target->CleanupLocalImmediate(true, true);
			}
			if (UPCGSubsystem* Subsystem = Target->GetSubsystem())
			{
				Subsystem->UnregisterPCGComponent(Target, true);
			}
		}
	}
}

void FPCGDuplicationAdapter::Tick(float DeltaTime)
{
	for (int32 Index = 0; Index < Observations.Num(); ++Index)
	{
		FPCGObservation& Observation = *Observations[Index];
		if (!Observation.Snapshot.Component.IsValid())
		{
			if (Observation.Snapshot.Reason != TEXT("The observed component was destroyed."))
			{
				SetState(Index, EOWTProceduralState::Cancelled, TEXT("The observed component was destroyed."));
			}
			continue;
		}
		UPCGComponent* Component = CastChecked<UPCGComponent>(Observation.Snapshot.Component.Get());
		bool bCleaningUp = Component->IsCleaningUp();
		bCleaningUp |= !Component->AreManagedResourcesAccessible();
		if (bCleaningUp)
		{
			if (Observation.Snapshot.State != EOWTProceduralState::CleaningUp)
			{
				SetState(Index, EOWTProceduralState::CleaningUp);
			}
		}
		RefreshLocalObservations(Index);
	}
}

void FPCGDuplicationAdapter::Shutdown()
{
	for (const TUniquePtr<FPCGObservation>& Observation : Observations)
	{
		for (const auto& Entry : Observation->Locals)
		{
			UnbindLocal(Entry.Key.Get(), Entry.Value);
		}
		if (UPCGComponent* Component = Cast<UPCGComponent>(Observation->Snapshot.Component.Get()))
		{
			Component->OnPCGGraphStartGeneratingDelegate.Remove(Observation->Started);
			Component->OnPCGGraphGeneratedDelegate.Remove(Observation->Generated);
			Component->OnPCGGraphCancelledDelegate.Remove(Observation->Cancelled);
			Component->OnPCGGraphCleanedDelegate.Remove(Observation->Cleaned);
		}
	}
	Observations.Reset();
	OnProceduralChanged.Clear();
}

TArray<FOWTProceduralComponentSnapshot> FPCGDuplicationAdapter::GetProceduralComponents() const
{
	TArray<FOWTProceduralComponentSnapshot> Result;
	for (const TUniquePtr<FPCGObservation>& Observation : Observations)
	{
		Result.Add(Observation->Snapshot);
	}
	return Result;
}

void FPCGDuplicationAdapter::UnbindLocal(UPCGComponent* Component, const FPCGLocalObservation& Observation)
{
	if (!Component)
	{
		return;
	}
	Component->OnPCGGraphStartGeneratingDelegate.Remove(Observation.Started);
	Component->OnPCGGraphGeneratedDelegate.Remove(Observation.Generated);
	Component->OnPCGGraphCancelledDelegate.Remove(Observation.Cancelled);
	Component->OnPCGGraphCleanedDelegate.Remove(Observation.Cleaned);
}

FPCGLocalObservation* FPCGDuplicationAdapter::FindObservedLocal(int32 Index, UPCGComponent* Component)
{
	if (!Component)
	{
		return nullptr;
	}
	if (Component->GetOriginalComponent() != Observations[Index]->Snapshot.Component.Get())
	{
		return nullptr;
	}
	return Observations[Index]->Locals.Find(Component);
}

void FPCGDuplicationAdapter::ObserveLocalStart(int32 Index, UPCGComponent* Component)
{
	FPCGLocalObservation* Local = FindObservedLocal(Index, Component);
	if (!Local)
	{
		return;
	}
	const FPCGTaskId Task = Component->GetGenerationTaskId();
	if (Task == InvalidPCGTaskId)
	{
		return;
	}
	if (Task == Local->LastGenerationTask)
	{
		return;
	}
	Local->LastGenerationTask = Task;
	Local->bCancelled = false;
	Local->bObservedCompletion = false;
	++Observations[Index]->Snapshot.GenerationAttempt;
	SetState(Index, EOWTProceduralState::Generating);
}

void FPCGDuplicationAdapter::ObserveLocalCompletion(int32 Index, UPCGComponent* Component)
{
	FPCGLocalObservation* Local = FindObservedLocal(Index, Component);
	if (!Local)
	{
		return;
	}
	if (Local->LastGenerationTask == InvalidPCGTaskId)
	{
		// A local can finish before the owner's next Tick. Its public generated state confirms one attempt.
		++Observations[Index]->Snapshot.GenerationAttempt;
	}
	Local->bObservedCompletion = true;
	Local->bCancelled = false;
	UpdateLocalAggregate(Index);
}

void FPCGDuplicationAdapter::BindLocal(int32 Index, UPCGComponent& Component)
{
	FPCGLocalObservation& Local = Observations[Index]->Locals.Add(&Component);
	Local.Started = Component.OnPCGGraphStartGeneratingDelegate.AddLambda(
	    [this, Index](UPCGComponent* Changed)
	    {
		    ObserveLocalStart(Index, Changed);
	    });
	Local.Generated = Component.OnPCGGraphGeneratedDelegate.AddLambda(
	    [this, Index](UPCGComponent* Changed)
	    {
		    ObserveLocalCompletion(Index, Changed);
	    });
	Local.Cancelled = Component.OnPCGGraphCancelledDelegate.AddLambda(
	    [this, Index](UPCGComponent* Changed)
	    {
		    if (FPCGLocalObservation* Entry = FindObservedLocal(Index, Changed))
		    {
			    Entry->bCancelled = true;
			    UpdateLocalAggregate(Index);
		    }
	    });
	Local.Cleaned = Component.OnPCGGraphCleanedDelegate.AddLambda(
	    [this, Index](UPCGComponent* Changed)
	    {
		    if (FPCGLocalObservation* Entry = FindObservedLocal(Index, Changed))
		    {
			    Entry->bObservedCompletion = false;
			    Entry->bCancelled = false;
			    UpdateLocalAggregate(Index);
		    }
	    });
}

void FPCGDuplicationAdapter::RefreshLocalObservations(int32 Index)
{
	FPCGObservation& Observation = *Observations[Index];
	UPCGComponent* Original = Cast<UPCGComponent>(Observation.Snapshot.Component.Get());
	if (!Original)
	{
		return;
	}
	if (!Original->IsPartitioned())
	{
		return;
	}
	UPCGSubsystem* Subsystem = Original->GetSubsystem();
	if (!Subsystem)
	{
		return;
	}
	TSet<TWeakObjectPtr<UPCGComponent>> CurrentLocals;
	Subsystem->ForAllRegisteredLocalComponents(Original,
	                                           [&CurrentLocals](UPCGComponent* Local)
	                                           {
		                                           if (IsValid(Local))
		                                           {
			                                           CurrentLocals.Add(Local);
		                                           }
	                                           });
	for (auto It = Observation.Locals.CreateIterator(); It; ++It)
	{
		if (!CurrentLocals.Contains(It.Key()))
		{
			UnbindLocal(It.Key().Get(), It.Value());
			It.RemoveCurrent();
		}
	}
	for (const TWeakObjectPtr<UPCGComponent>& LocalKey : CurrentLocals)
	{
		UPCGComponent* Local = LocalKey.Get();
		if (!Local)
		{
			continue;
		}
		if (!Observation.Locals.Contains(LocalKey))
		{
			BindLocal(Index, *Local);
		}
		ObserveLocalStart(Index, Local);
		if (!IsValid(Local))
		{
			continue;
		}
		FPCGLocalObservation& State = Observation.Locals.FindChecked(LocalKey);
		if (State.bObservedCompletion)
		{
			continue;
		}
		if (Local->IsGenerating())
		{
			continue;
		}
		if (!Local->bGenerated)
		{
			continue;
		}
		if (State.bCancelled)
		{
			continue;
		}
		ObserveLocalCompletion(Index, Local);
	}
	UpdateLocalAggregate(Index);
}

void FPCGDuplicationAdapter::UpdateLocalAggregate(int32 Index)
{
	FPCGObservation& Observation = *Observations[Index];
	UPCGComponent* Original = Cast<UPCGComponent>(Observation.Snapshot.Component.Get());
	if (!Original)
	{
		return;
	}
	bool bGenerating = Original->IsGenerating();
	bool bCleaningUp = Original->IsCleaningUp();
	bCleaningUp |= !Original->AreManagedResourcesAccessible();
	bool bCancelled = Observation.bOriginalCancelled;
	int32 RegisteredGridCount = 0;
	int32 GeneratedGridCount = 0;
	if (UPCGSubsystem* Subsystem = Original->GetSubsystem())
	{
		PCGHiGenGrid::FSizeArray GridSizes;
		bool bHasUnbounded = false;
		PCGHelpers::GetGenerationGridSizes(Original->GetGraph(), Subsystem->FindPCGWorldActor(), GridSizes,
		                                   bHasUnbounded);
		if (bHasUnbounded)
		{
			++RegisteredGridCount;
			if (Original->bGenerated)
			{
				++GeneratedGridCount;
			}
		}
		Subsystem->ForAllRegisteredLocalComponents(Original,
		                                           [&](UPCGComponent* Local)
		                                           {
			                                           if (IsValid(Local))
			                                           {
				                                           ++RegisteredGridCount;
				                                           bGenerating |= Local->IsGenerating();
				                                           bCleaningUp |= Local->IsCleaningUp();
				                                           bCleaningUp |= !Local->AreManagedResourcesAccessible();
				                                           if (Local->bGenerated)
				                                           {
					                                           ++GeneratedGridCount;
				                                           }
			                                           }
		                                           });
	}
	for (const auto& Entry : Observation.Locals)
	{
		if (const UPCGComponent* Local = Entry.Key.Get())
		{
			bCancelled |= Entry.Value.bCancelled;
		}
	}
	EOWTProceduralState State = Observation.Snapshot.State;
	if (bCleaningUp)
	{
		State = EOWTProceduralState::CleaningUp;
	}
	else if (bGenerating)
	{
		State = EOWTProceduralState::Generating;
	}
	else if (bCancelled)
	{
		State = EOWTProceduralState::Cancelled;
	}
	else if (GeneratedGridCount > 0)
	{
		if (GeneratedGridCount == RegisteredGridCount)
		{
			State = EOWTProceduralState::Ready;
		}
		else if (Original->IsManagedByRuntimeGenSystem())
		{
			State = EOWTProceduralState::WaitingForGenerationSource;
		}
		else
		{
			State = EOWTProceduralState::Scheduled;
		}
	}
	else if (Observation.Snapshot.GenerationAttempt > 0)
	{
		State = EOWTProceduralState::Cleaned;
	}
	if (State != Observation.Snapshot.State)
	{
		SetState(Index, State);
	}
}

UPCGGraphInterface* FPCGDuplicationAdapter::CloneOwnedGraph(UPCGGraphInterface* Graph, const UObject& SourceOwner,
                                                            UObject& DestinationOwner, FString& Error)
{
	if (!Graph)
	{
		return nullptr;
	}
	if (!Graph->IsIn(&SourceOwner))
	{
		return Graph;
	}
	UPCGGraphInstance* Instance = Cast<UPCGGraphInstance>(Graph);
	if (!Instance)
	{
		Error = TEXT("Actor-owned PCG graph definitions require a graph construction adapter; use an external "
		             "graph asset or an owned GraphInstance.");
		return nullptr;
	}
	if (CloningGraphs.Contains(Graph))
	{
		Error = TEXT("The owned PCG graph instance parent chain contains a cycle.");
		return nullptr;
	}
	CloningGraphs.Add(Graph);
	UPCGGraphInterface* Parent = CloneOwnedGraph(Instance->Graph, SourceOwner, DestinationOwner, Error);
	CloningGraphs.Remove(Graph);
	if (!Error.IsEmpty())
	{
		return nullptr;
	}
	UPCGGraphInstance* Copy = NewObject<UPCGGraphInstance>(&DestinationOwner);
	Copy->SetGraph(Parent);
	Copy->CopyParameterOverrides(Instance);
	GraphCopies.Add(Graph, Copy);
	SourceGraphs.Add(Graph);
	return Copy;
}

bool FPCGDuplicationAdapter::VisitStruct(const UStruct& Struct, void* Memory, const TMap<UObject*, UObject*>& Mapping,
                                         bool bValidateOnly, FString& Error)
{
	for (TFieldIterator<FProperty> Property(&Struct); Property; ++Property)
	{
		if (Property->HasAnyPropertyFlags(ExcludedConfigurationProperties))
		{
			continue;
		}
		if (!VisitProperty(**Property, Property->ContainerPtrToValuePtr<void>(Memory), Mapping, bValidateOnly, Error))
		{
			return false;
		}
	}
	return true;
}

bool FPCGDuplicationAdapter::VisitProperty(FProperty& Property, void* Value,
    const TMap<UObject*, UObject*>& Mapping, bool bValidateOnly, FString& Error)
{
	TSet<const UScriptStruct*> Visited;
	if (const UScriptStruct* Unsupported = OWTDuplication::FindUnsupportedStruct(Property, Visited))
	{
		Error = FString::Printf(TEXT("PCG parameter %s contains unsupported native state %s."),
			*Property.GetPathName(), *Unsupported->GetName());
		return false;
	}
	OWTDuplication::VisitReferences(Property, Value,
		[this, &Mapping, bValidateOnly, &Error, &Property](UObject*& Reference, bool)
		{
			if (!Reference)
			{
				return;
			}
			if (IsManagedObject(*Reference))
			{
				Error = FString::Printf(TEXT("PCG parameter %s references managed output %s and needs a "
					"logical output remapping adapter."), *Property.GetPathName(), *Reference->GetPathName());
				return;
			}
			if (!bValidateOnly)
			{
				if (UObject* const* Replacement = Mapping.Find(Reference))
				{
					Reference = *Replacement;
				}
			}
		});
	return Error.IsEmpty();
}

bool FPCGDuplicationAdapter::VisitConfiguration(UPCGComponent& Component, const TMap<UObject*, UObject*>& Mapping,
                                                bool bValidateOnly, FString& Error)
{
	for (TFieldIterator<FProperty> Property(UPCGComponent::StaticClass(), EFieldIteratorFlags::ExcludeSuper); Property; ++Property)
	{
		if (!IsReflectedConfigurationProperty(**Property))
		{
			continue;
		}
		if (!VisitProperty(**Property, Property->ContainerPtrToValuePtr<void>(&Component), Mapping, bValidateOnly, Error))
		{
			return false;
		}
	}

	for (UObject* PolicyObject : GetPolicyObjects(Component.SchedulingPolicy))
	{
		if (!VisitStruct(*PolicyObject->GetClass(), PolicyObject, Mapping, bValidateOnly, Error))
		{
			return false;
		}
	}
	for (UPCGGraphInstance* Graph = Component.GetGraphInstance(); Graph; Graph = Cast<UPCGGraphInstance>(Graph->Graph))
	{
		if (!Graph->IsIn(&Component))
		{
			break;
		}
		FInstancedPropertyBag& Bag = Graph->ParametersOverrides.Parameters;
		if (const UScriptStruct* Struct = Bag.GetPropertyBagStruct())
		{
			if (!VisitStruct(*Struct, Bag.GetMutableValue().GetMemory(), Mapping, bValidateOnly, Error))
			{
				return false;
			}
		}
	}
	for (auto& Data : Component.ToolDataContainer.ToolData)
	{
		if (Data.IsValid())
		{
			if (!VisitStruct(*Data.GetScriptStruct(), Data.GetMutableMemory(), Mapping, bValidateOnly, Error))
			{
				return false;
			}
		}
	}
	return true;
}

void FPCGDuplicationAdapter::SetState(int32 Index, EOWTProceduralState State, const FString& Reason)
{
	FOWTProceduralComponentSnapshot& Snapshot = Observations[Index]->Snapshot;
	Snapshot.State = State;
	Snapshot.Reason = Reason;
	++Snapshot.Revision;
	const FOWTProceduralComponentSnapshot Copy = Snapshot;
	OnProceduralChanged.Broadcast(Copy);
}
} // namespace

void RegisterOWTPCGDuplicationAdapter(FOWTDuplicationAdapterRegistry& Registry)
{
	Registry.Register(UPCGComponent::StaticClass(),
	                  []()
	                  {
		                  return MakeUnique<FPCGDuplicationAdapter>();
	                  });
}
