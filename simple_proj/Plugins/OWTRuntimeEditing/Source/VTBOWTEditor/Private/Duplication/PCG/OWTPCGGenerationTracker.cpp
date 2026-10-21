#include "Duplication/PCG/OWTPCGGenerationTracker.h"

#include "Duplication/OWTDuplicationRequest.h"
#include "Duplication/PCG/OWTPCGConfigurationSnapshot.h"
#include "Helpers/PCGHelpers.h"
#include "PCGComponent.h"
#include "PCGGraph.h"
#include "Subsystems/PCGSubsystem.h"

FOWTPCGGenerationTracker::FLocalObservation::FLocalObservation()
    : Started(), Generated(), Cancelled(), Cleaned(), LastGenerationTask(InvalidPCGTaskId), bObservedCompletion(false),
      bCancelled(false)
{
}

FOWTPCGGenerationTracker::FObservation::FObservation()
    : Snapshot(), Started(), Generated(), Cancelled(), Cleaned(), Locals(), bActivated(false),
      bRequestGeneration(false), bOriginalCancelled(false)
{
}

FOWTPCGGenerationTracker::FOWTPCGGenerationTracker(FOWTProceduralChanged& InOnChanged)
    : OnChanged(InOnChanged), Observations()
{
}

FOWTPCGGenerationTracker::~FOWTPCGGenerationTracker()
{
	Shutdown();
}

void FOWTPCGGenerationTracker::Observe(UPCGComponent& Target, const FOWTPCGConfigurationSnapshot& Captured,
                                       const FGuid& OperationId, const FOWTDuplicationOptions& Options)
{
	TUniquePtr<FObservation> Observation = MakeUnique<FObservation>();
	Observation->Snapshot.Component = &Target;
	Observation->Snapshot.OperationId = OperationId;
	Observation->Snapshot.ComponentId = FGuid::NewGuid();
	Observation->Snapshot.ComponentName = Target.GetName();
	Observation->Snapshot.GraphPath = Target.GetGraph()->GetPathName();
	Observation->Snapshot.Trigger =
	    StaticEnum<EPCGComponentGenerationTrigger>()->GetNameStringByValue(static_cast<int64>(Target.GenerationTrigger));
	const int32 Index = Observations.Add(MoveTemp(Observation));
	BindObservation(Index, Target);
	ConfigureGeneration(*Observations[Index], Target, Captured, Options);
}

void FOWTPCGGenerationTracker::RequestGenerations()
{
	for (int32 Index = 0; Index < Observations.Num(); ++Index)
	{
		RequestGeneration(Index);
	}
}

void FOWTPCGGenerationTracker::Tick()
{
	static const FString DestroyedReason = TEXT("The observed component was destroyed.");
	for (int32 Index = 0; Index < Observations.Num(); ++Index)
	{
		FObservation& Observation = *Observations[Index];
		UPCGComponent* Component = Cast<UPCGComponent>(Observation.Snapshot.Component.Get());
		const bool bDestroyedReported = Observation.Snapshot.Reason == DestroyedReason;
		const bool bCleaningUp = Component && (Component->IsCleaningUp() || !Component->AreManagedResourcesAccessible());
		if (!Component && !bDestroyedReported)
		{
			SetState(Index, EOWTProceduralState::Cancelled, DestroyedReason);
		}
		else if (bCleaningUp && Observation.Snapshot.State != EOWTProceduralState::CleaningUp)
		{
			SetState(Index, EOWTProceduralState::CleaningUp);
		}
		if (Component)
		{
			RefreshLocalObservations(Index);
		}
	}
}

void FOWTPCGGenerationTracker::Shutdown()
{
	for (const TUniquePtr<FObservation>& Observation : Observations)
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
}

TArray<FOWTProceduralComponentSnapshot> FOWTPCGGenerationTracker::GetSnapshots() const
{
	TArray<FOWTProceduralComponentSnapshot> Result;
	for (const TUniquePtr<FObservation>& Observation : Observations)
	{
		Result.Add(Observation->Snapshot);
	}
	return Result;
}

void FOWTPCGGenerationTracker::ConfigureGeneration(FObservation& Observation, const UPCGComponent& Target,
                                                   const FOWTPCGConfigurationSnapshot& Captured,
                                                   const FOWTDuplicationOptions& Options)
{
	// GenerateOnLoad always regenerates; GenerateOnDemand mirrors the source unless the caller keeps it empty.
	const bool bOnLoad = Target.GenerationTrigger == EPCGComponentGenerationTrigger::GenerateOnLoad;
	const bool bOnDemand = Target.GenerationTrigger == EPCGComponentGenerationTrigger::GenerateOnDemand;
	const bool bKeepUnGenerated = Options.GenerationPolicy == EOWTDuplicationGenerationPolicy::KeepUnGenerated;
	Observation.bActivated = Captured.bActivated;
	Observation.bRequestGeneration = bOnLoad || (bOnDemand && Captured.bWasGenerated && !bKeepUnGenerated);

	if (Observation.bActivated && Target.IsManagedByRuntimeGenSystem())
	{
		Observation.Snapshot.State = EOWTProceduralState::WaitingForGenerationSource;
	}
	else if (Observation.bActivated && Observation.bRequestGeneration)
	{
		Observation.Snapshot.State = EOWTProceduralState::Scheduled;
	}
}

void FOWTPCGGenerationTracker::RequestGeneration(int32 Index)
{
	FObservation& Bound = *Observations[Index];
	UPCGComponent* Target = Cast<UPCGComponent>(Bound.Snapshot.Component.Get());
	if (Target)
	{
		Target->bActivated = Bound.bActivated;
	}
	UPCGSubsystem* Subsystem = Target && Target->bActivated ? Target->GetSubsystem() : nullptr;
	if (Subsystem)
	{
		Subsystem->RegisterOrUpdatePCGComponent(Target);
	}
	const bool bRuntimeManaged = Subsystem && Target->IsManagedByRuntimeGenSystem();
	const bool bGenerateNow = Subsystem && !bRuntimeManaged && Bound.bRequestGeneration;

	// Listeners run inside SetState and may destroy the component before the follow-up request.
	if (Target && !Target->bActivated)
	{
		SetState(Index, EOWTProceduralState::NotRequested, TEXT("The source component was inactive."));
	}
	else if (Target && !Subsystem)
	{
		SetState(Index, EOWTProceduralState::Failed, TEXT("The destination world has no PCG subsystem."));
	}
	else if (bRuntimeManaged)
	{
		SetState(Index, EOWTProceduralState::WaitingForGenerationSource);
		if (IsValid(Target))
		{
			Subsystem->RefreshRuntimeGenComponent(Target);
		}
	}
	else if (Subsystem && !bGenerateNow)
	{
		SetState(Index, EOWTProceduralState::NotRequested);
	}
	else if (bGenerateNow)
	{
		SetState(Index, EOWTProceduralState::Scheduled);
		const bool bAccepted = !IsValid(Target) || Target->GenerateLocalGetTaskId(true) != InvalidPCGTaskId;
		if (!bAccepted)
		{
			SetState(Index, EOWTProceduralState::Failed, TEXT("PCG did not accept the generation request."));
		}
	}
}

void FOWTPCGGenerationTracker::BindObservation(int32 Index, UPCGComponent& Target)
{
	FObservation& Bound = *Observations[Index];
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
		    // Partitioned generation is complete only when every local component has generated.
		    if (Component->IsPartitioned())
		    {
			    RefreshLocalObservations(Index);
		    }
		    else
		    {
			    SetState(Index, EOWTProceduralState::Ready);
		    }
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

void FOWTPCGGenerationTracker::BindLocal(int32 Index, UPCGComponent& Component)
{
	FLocalObservation& Local = Observations[Index]->Locals.Add(&Component);
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
		    if (FLocalObservation* Entry = FindObservedLocal(Index, Changed))
		    {
			    Entry->bCancelled = true;
			    UpdateLocalAggregate(Index);
		    }
	    });
	Local.Cleaned = Component.OnPCGGraphCleanedDelegate.AddLambda(
	    [this, Index](UPCGComponent* Changed)
	    {
		    if (FLocalObservation* Entry = FindObservedLocal(Index, Changed))
		    {
			    Entry->bObservedCompletion = false;
			    Entry->bCancelled = false;
			    UpdateLocalAggregate(Index);
		    }
	    });
}

void FOWTPCGGenerationTracker::UnbindLocal(UPCGComponent* Component, const FLocalObservation& Observation)
{
	if (Component)
	{
		Component->OnPCGGraphStartGeneratingDelegate.Remove(Observation.Started);
		Component->OnPCGGraphGeneratedDelegate.Remove(Observation.Generated);
		Component->OnPCGGraphCancelledDelegate.Remove(Observation.Cancelled);
		Component->OnPCGGraphCleanedDelegate.Remove(Observation.Cleaned);
	}
}

void FOWTPCGGenerationTracker::RefreshLocalObservations(int32 Index)
{
	FObservation& Observation = *Observations[Index];
	UPCGComponent* Original = Cast<UPCGComponent>(Observation.Snapshot.Component.Get());
	UPCGSubsystem* Subsystem = Original && Original->IsPartitioned() ? Original->GetSubsystem() : nullptr;
	if (Subsystem)
	{
		TSet<TWeakObjectPtr<UPCGComponent>> CurrentLocals;
		Subsystem->ForAllRegisteredLocalComponents(Original, [&CurrentLocals](UPCGComponent* Local)
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
			if (Local && !Observation.Locals.Contains(LocalKey))
			{
				BindLocal(Index, *Local);
			}
			if (Local)
			{
				ObserveLocalStart(Index, Local);
			}
			// A local may have finished between ticks without this tracker seeing its completion delegate.
			const FLocalObservation* State = IsValid(Local) ? Observation.Locals.Find(LocalKey) : nullptr;
			const bool bMissedCompletion = State && !State->bObservedCompletion && !State->bCancelled &&
			                               !Local->IsGenerating() && Local->bGenerated;
			if (bMissedCompletion)
			{
				ObserveLocalCompletion(Index, Local);
			}
		}
		UpdateLocalAggregate(Index);
	}
}

void FOWTPCGGenerationTracker::UpdateLocalAggregate(int32 Index)
{
	FObservation& Observation = *Observations[Index];
	UPCGComponent* Original = Cast<UPCGComponent>(Observation.Snapshot.Component.Get());
	const EOWTProceduralState State = Original ? CalculateAggregateState(Observation, *Original) : Observation.Snapshot.State;
	if (State != Observation.Snapshot.State)
	{
		SetState(Index, State);
	}
}

EOWTProceduralState FOWTPCGGenerationTracker::CalculateAggregateState(const FObservation& Observation, UPCGComponent& Original) const
{
	bool bGenerating = Original.IsGenerating();
	bool bCleaningUp = Original.IsCleaningUp() || !Original.AreManagedResourcesAccessible();
	bool bCancelled = Observation.bOriginalCancelled;
	int32 RegisteredGridCount = 0;
	int32 GeneratedGridCount = 0;
	if (UPCGSubsystem* Subsystem = Original.GetSubsystem())
	{
		// The unbounded grid is generated by the original component itself rather than by a local.
		PCGHiGenGrid::FSizeArray GridSizes;
		bool bHasUnbounded = false;
		PCGHelpers::GetGenerationGridSizes(Original.GetGraph(), Subsystem->FindPCGWorldActor(), GridSizes, bHasUnbounded);
		RegisteredGridCount += bHasUnbounded ? 1 : 0;
		GeneratedGridCount += bHasUnbounded && Original.bGenerated ? 1 : 0;
		Subsystem->ForAllRegisteredLocalComponents(&Original, [&](UPCGComponent* Local)
		{
			if (IsValid(Local))
			{
				++RegisteredGridCount;
				GeneratedGridCount += Local->bGenerated ? 1 : 0;
				bGenerating |= Local->IsGenerating();
				bCleaningUp |= Local->IsCleaningUp() || !Local->AreManagedResourcesAccessible();
			}
		});
	}
	for (const auto& Entry : Observation.Locals)
	{
		bCancelled |= Entry.Key.IsValid() && Entry.Value.bCancelled;
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
		else if (Original.IsManagedByRuntimeGenSystem())
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
	return State;
}

void FOWTPCGGenerationTracker::ObserveLocalStart(int32 Index, UPCGComponent* Component)
{
	FLocalObservation* Local = FindObservedLocal(Index, Component);
	const FPCGTaskId Task = Local ? Component->GetGenerationTaskId() : InvalidPCGTaskId;
	const bool bNewAttempt = Task != InvalidPCGTaskId && Task != Local->LastGenerationTask;
	if (bNewAttempt)
	{
		Local->LastGenerationTask = Task;
		Local->bCancelled = false;
		Local->bObservedCompletion = false;
		++Observations[Index]->Snapshot.GenerationAttempt;
		SetState(Index, EOWTProceduralState::Generating);
	}
}

void FOWTPCGGenerationTracker::ObserveLocalCompletion(int32 Index, UPCGComponent* Component)
{
	FLocalObservation* Local = FindObservedLocal(Index, Component);
	if (Local)
	{
		if (Local->LastGenerationTask == InvalidPCGTaskId)
		{
			// A local can finish before the owner's next Tick. Its public generated state confirms one attempt.
			++Observations[Index]->Snapshot.GenerationAttempt;
		}
		Local->bObservedCompletion = true;
		Local->bCancelled = false;
		UpdateLocalAggregate(Index);
	}
}

void FOWTPCGGenerationTracker::SetState(int32 Index, EOWTProceduralState State, const FString& Reason)
{
	FOWTProceduralComponentSnapshot& Snapshot = Observations[Index]->Snapshot;
	Snapshot.State = State;
	Snapshot.Reason = Reason;
	++Snapshot.Revision;
	// Listeners may shut the tracker down, which destroys the snapshot being broadcast.
	const FOWTProceduralComponentSnapshot Copy = Snapshot;
	OnChanged.Broadcast(Copy);
}

FOWTPCGGenerationTracker::FLocalObservation* FOWTPCGGenerationTracker::FindObservedLocal(int32 Index, UPCGComponent* Component)
{
	// Delegates fire for any local; only locals generated for this observation's component are tracked.
	const bool bObservedLocal = Component && Component->GetOriginalComponent() == Observations[Index]->Snapshot.Component.Get();
	return bObservedLocal ? Observations[Index]->Locals.Find(Component) : nullptr;
}
