#include "Duplication/Pipeline/OWTDuplicationPipeline.h"

#include "Duplication/Capture/OWTDuplicationSnapshots.h"
#include "Duplication/Capture/OWTSnapshotBuilder.h"
#include "Duplication/OWTDuplicationRequest.h"
#include "Duplication/Restore/OWTActorNameAllocator.h"
#include "Duplication/Restore/OWTSnapshotRestorer.h"
#include "GameFramework/Actor.h"

FOWTDuplicationPipeline::FOWTDuplicationPipeline(UWorld& World, AActor& InSource, const FOWTDuplicationOptions& Options,
                                                 const FOWTDuplicationAdapterRegistry& InRegistry,
                                                 TMap<FString, int64>& InNameCounters, FString& Error)
    : Registry(InRegistry), Adapters(Registry.CreateAdapters()), UnsupportedStructs(),
      Context(World, Options, Registry.GetPropertyPolicies(), Registry.GetSourceRules(), Adapters, UnsupportedStructs,
              Error),
      Collector(InSource, Context), Source(InSource), NameCounters(InNameCounters), Snapshots(), Restorers(),
      CreatedRoots(), ReservedNames(), CompleteMapping()
{
}

FOWTDuplicationPipeline::~FOWTDuplicationPipeline() = default;

bool FOWTDuplicationPipeline::Prepare()
{
	return Adapters.Initialize(Context.World, Context.Error) && Collector.Collect() &&
	       Collector.CaptureAdapterSubjects() && CaptureSnapshots() &&
	       Adapters.ValidateReferences(CompleteMapping, Context.Error);
}

bool FOWTDuplicationPipeline::Execute(TFunctionRef<bool()> IsOwnerAlive)
{
	const bool bNamesReserved = ReserveNames();
	const bool bReadyToCommit = bNamesReserved && SpawnRoots(IsOwnerAlive) && FinalizeRoots() && CheckBeforeCommit();
	// Nothing exists yet when name reservation fails, so only later failures roll back.
	if (bNamesReserved && !bReadyToCommit)
	{
		Rollback();
	}
	return bReadyToCommit;
}

AActor* FOWTDuplicationPipeline::GetResult() const
{
	return Cast<AActor>(CompleteMapping.FindRef(&Source));
}

TArray<AActor*> FOWTDuplicationPipeline::GetCreatedActors() const
{
	TArray<AActor*> Created;
	for (AActor* Original : Collector.GetActors())
	{
		AActor* Duplicate = Cast<AActor>(CompleteMapping.FindRef(Original));
		if (IsValid(Duplicate))
		{
			Created.Add(Duplicate);
		}
	}
	return Created;
}

const FOWTObjectMap& FOWTDuplicationPipeline::GetCompleteMapping() const
{
	return CompleteMapping;
}

FOWTAdapterDispatcher& FOWTDuplicationPipeline::GetAdapters()
{
	return Adapters;
}

bool FOWTDuplicationPipeline::CaptureSnapshots()
{
	const TArray<AActor*> Roots = Collector.GetRootActors();
	bool bCaptured = true;
	for (int32 Index = 0; bCaptured && Index < Roots.Num(); ++Index)
	{
		TUniquePtr<FOWTRootSnapshot> Snapshot = MakeUnique<FOWTRootSnapshot>(*Roots[Index]);
		FOWTSnapshotBuilder Builder(Context, *Snapshot);
		bCaptured = Builder.Capture();
		CompleteMapping.Append(Snapshot->Mapping);
		Snapshots.Add(MoveTemp(Snapshot));
	}
	return bCaptured;
}

bool FOWTDuplicationPipeline::ReserveNames()
{
	FOWTActorNameAllocator Allocator(NameCounters);
	bool bReserved = true;
	for (int32 Index = 0; bReserved && Index < Snapshots.Num(); ++Index)
	{
		const FName Name = Allocator.Allocate(Snapshots[Index]->Root, Context.Error);
		bReserved = !Name.IsNone();
		ReservedNames.Add(Name);
	}
	return bReserved;
}

bool FOWTDuplicationPipeline::SpawnRoots(TFunctionRef<bool()> IsOwnerAlive)
{
	// Separately spawned roots can reference each other, so participants wait until every root exists.
	const bool bDeferParticipants = Snapshots.Num() > 1;
	bool bSpawned = true;
	for (int32 Index = 0; bSpawned && Index < Snapshots.Num(); ++Index)
	{
		TUniquePtr<FOWTSnapshotRestorer> Restorer =
		    MakeUnique<FOWTSnapshotRestorer>(Context, *Snapshots[Index], ReservedNames[Index], bDeferParticipants);
		AActor* Duplicate = Restorer->Spawn();
		// Rollback needs partial mappings to find every object an adapter may have started.
		CompleteMapping.Append(Restorer->GetMapping());
		Restorers.Add(MoveTemp(Restorer));
		if (Duplicate)
		{
			CreatedRoots.Add(Duplicate);
		}
		bSpawned = Duplicate && CheckAfterCallbacks(IsOwnerAlive);
	}
	return bSpawned;
}

bool FOWTDuplicationPipeline::FinalizeRoots()
{
	bool bFinalized = true;
	if (Restorers.Num() > 1)
	{
		bFinalized = Adapters.Restore(CompleteMapping, Context.Error);
		for (const TUniquePtr<FOWTSnapshotRestorer>& Restorer : Restorers)
		{
			bFinalized = bFinalized && Restorer->FinalizeHierarchy(CompleteMapping);
			if (bFinalized)
			{
				CompleteMapping.Append(Restorer->GetMapping());
			}
		}
	}
	return bFinalized;
}

bool FOWTDuplicationPipeline::CheckAfterCallbacks(TFunctionRef<bool()> IsOwnerAlive)
{
	AActor* const* FailedSource = Collector.GetActors().FindByPredicate([](const AActor* Original)
	{
		return !IsValid(Original) || Original->IsActorBeingDestroyed();
	});
	const bool bSourcesAlive = FailedSource == nullptr;
	const bool bCancelled = bSourcesAlive && IsCancellationRequested();
	const bool bOwnerAlive = bSourcesAlive && !bCancelled && IsOwnerAlive();

	if (FailedSource && !IsValid(*FailedSource))
	{
		Context.Error = TEXT("A source Actor was destroyed by a construction or restoration callback.");
	}
	else if (FailedSource)
	{
		Context.Error = TEXT("A source Actor is being destroyed after a user callback.");
	}
	else if (bCancelled)
	{
		Context.Error = TEXT("Cancelled: the editing operation ended during construction or restoration.");
	}
	else if (!bOwnerAlive)
	{
		Context.Error = TEXT("The duplicator owner was released during duplication.");
	}
	return bOwnerAlive;
}

bool FOWTDuplicationPipeline::CheckBeforeCommit()
{
	const bool bCancelled = IsCancellationRequested();
	const bool bResultValid = !bCancelled && IsValid(GetResult());

	if (bCancelled)
	{
		Context.Error = TEXT("Cancelled: the editing operation ended before authored commit.");
	}
	else if (!bResultValid)
	{
		Context.Error = TEXT("The final authored duplicate is no longer valid.");
	}
	return bResultValid;
}

bool FOWTDuplicationPipeline::IsCancellationRequested() const
{
	const TFunction<bool()>& IsCancellationRequested = Context.Options.IsCancellationRequested;
	return IsCancellationRequested && IsCancellationRequested();
}

void FOWTDuplicationPipeline::Rollback()
{
	Adapters.Rollback(CompleteMapping);
	for (const TWeakObjectPtr<AActor>& Root : CreatedRoots)
	{
		if (AActor* Actor = Root.Get())
		{
			Actor->Destroy();
		}
	}
}
