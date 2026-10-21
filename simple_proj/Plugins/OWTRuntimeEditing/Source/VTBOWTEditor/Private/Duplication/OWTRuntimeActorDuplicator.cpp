#include "Duplication/OWTRuntimeActorDuplicator.h"

#include "Duplication/Pipeline/OWTDuplicationPipeline.h"
#include "Duplication/OWTDuplicationAdapterProvider.h"
#include "Engine/World.h"
#include "Features/IModularFeatures.h"
#include "GameFramework/Actor.h"
#include "Misc/ScopeExit.h"

UOWTRuntimeActorDuplicator::UOWTRuntimeActorDuplicator() :
	OwningObject(),
	NameCounters(),
	AdapterRegistry(),
	CommittedAdapters(),
	bDuplicating(false),
	bDispatching(false)
{
}

UWorld* UOWTRuntimeActorDuplicator::GetWorld() const
{
	UObject* Owner = OwningObject.Get();
	return Owner ? Owner->GetWorld() : nullptr;
}

void UOWTRuntimeActorDuplicator::BeginDestroy()
{
	Deinitialize();
	Super::BeginDestroy();
}

bool UOWTRuntimeActorDuplicator::Initialize(UObject* Owner)
{
	check(IsInGameThread());
	const bool bIdle = !bDuplicating && !bDispatching;
	const bool bOwnedByOuter = IsValid(Owner) && Owner == GetOuter();
	UWorld* World = bOwnedByOuter ? Owner->GetWorld() : nullptr;
	const bool bCanInitialize = bIdle && World && !World->bIsTearingDown;

	if (bCanInitialize)
	{
		OwningObject = Owner;
		TGuardValue<bool> InstallingProviders(bDuplicating, true);
		RegisterAdapterProviders();
	}
	// Provider callbacks are external code and may release the owner or its world.
	const bool bInitialized = bCanInitialize && OwningObject.IsValid() && GetWorld() == World && IsValid(World) && !World->bIsTearingDown;
	if (bCanInitialize && !bInitialized)
	{
		Deinitialize();
	}
	return bInitialized;
}

AActor* UOWTRuntimeActorDuplicator::DuplicateActor(AActor* Source, const FVector& WorldOffset, FString& OutError)
{
	FOWTDuplicationOptions Options;
	Options.WorldOffset = WorldOffset;
	Options.HierarchyScope = EOWTDuplicationHierarchyScope::ActorAndManagedChildren;
	return DuplicateActorWithOptions(Source, Options, FGuid::NewGuid(), OutError);
}

void UOWTRuntimeActorDuplicator::Deinitialize()
{
	OwningObject.Reset();
	// An operation or notification on the stack still uses the adapters; it releases them when it unwinds.
	const bool bCanRelease = !bDuplicating && !bDispatching;
	if (bCanRelease)
	{
		for (const auto& Adapter : CommittedAdapters)
		{
			Adapter->Shutdown();
		}
		CommittedAdapters.Reset();
		NameCounters.Reset();
	}
}

FOWTDuplicationResult UOWTRuntimeActorDuplicator::Duplicate(AActor* Source, const FOWTDuplicationOptions& Options, const FGuid& OperationId)
{
	check(IsInGameThread());
	FOWTDuplicationResult Result;
	Result.OperationId = OperationId;
	const bool bAccepted = ValidateRequest(Source, Options, Result.Error);

	if (bAccepted)
	{
		UWorld* World = GetWorld();
		TGuardValue<bool> Guard(bDuplicating, true);
		ON_SCOPE_EXIT{
			if (!OwningObject.IsValid())
			{
				bDuplicating = false;
				Deinitialize();
			}
		};
		FOWTDuplicationPipeline Pipeline(*World, *Source, Options, AdapterRegistry, NameCounters, Result.Error);
		const bool bReadyToCommit = Pipeline.Prepare() && Pipeline.Execute([this, World](){
			return GetWorld() == World;
		});
		if (bReadyToCommit)
		{
			AActor* Duplicate = Pipeline.GetResult();
			CommitAdapters(Pipeline.GetAdapters().GetAdapters(), Pipeline.GetCompleteMapping(), *Duplicate, Options, OperationId);
			Result.Duplicate = Duplicate;
			Result.CreatedActors.Append(Pipeline.GetCreatedActors());
			Result.bSucceeded = true;
		}
	}
	return Result;
}

bool UOWTRuntimeActorDuplicator::CanDuplicate(AActor* Source, const FOWTDuplicationOptions& Options, FString& OutError)
{
	check(IsInGameThread());
	OutError.Reset();
	bool bCanDuplicate = ValidateRequest(Source, Options, OutError);

	if (bCanDuplicate)
	{
		TGuardValue<bool> Guard(bDuplicating, true);
		ON_SCOPE_EXIT{
			if (!OwningObject.IsValid())
			{
				bDuplicating = false;
				Deinitialize();
			}
		};
		FOWTDuplicationPipeline Pipeline(*GetWorld(), *Source, Options, AdapterRegistry, NameCounters, OutError);
		bCanDuplicate = Pipeline.Prepare();
	}
	return bCanDuplicate;
}

AActor* UOWTRuntimeActorDuplicator::DuplicateActorWithOptions(AActor* Source, const FOWTDuplicationOptions& Options, const FGuid& OperationId, FString& OutError)
{
	OutError.Reset();
	FOWTDuplicationResult Result = Duplicate(Source, Options, OperationId);
	OutError = MoveTemp(Result.Error);
	return Result.Duplicate;
}

void UOWTRuntimeActorDuplicator::Tick(float DeltaTime)
{
	const bool bIdle = !bDuplicating && !bDispatching;
	const bool bOwnerAlive = OwningObject.IsValid();
	if (bIdle && !bOwnerAlive)
	{
		Deinitialize();
	}

	if (bIdle && bOwnerAlive)
	{
		TGuardValue<bool> Guard(bDispatching, true);
		// An adapter's listeners may release the owner; the remaining adapters are then shut down by Deinitialize.
		for (int32 Index = 0; Index < CommittedAdapters.Num() && OwningObject.IsValid(); ++Index)
		{
			CommittedAdapters[Index]->Tick(DeltaTime);
		}
		// Adapters are released once every component they observe is gone.
		for (int32 Index = CommittedAdapters.Num() - 1; Index >= 0; --Index)
		{
			const bool bHasLiveComponents = CommittedAdapters[Index]->GetProceduralComponents().ContainsByPredicate([](const FOWTProceduralComponentSnapshot& Snapshot){
				return Snapshot.Component.IsValid();
			});
			if (!bHasLiveComponents)
			{
				CommittedAdapters.RemoveAt(Index);
			}
		}
	}
}

TArray<FOWTProceduralComponentSnapshot> UOWTRuntimeActorDuplicator::GetProceduralComponents() const
{
	TArray<FOWTProceduralComponentSnapshot> Result;
	for (const auto& Adapter : CommittedAdapters)
	{
		Result.Append(Adapter->GetProceduralComponents());
	}
	return Result;
}

FOWTDuplicationAdapterRegistry& UOWTRuntimeActorDuplicator::GetAdapterRegistry()
{
	return AdapterRegistry;
}

void UOWTRuntimeActorDuplicator::RegisterAdapterProviders()
{
	// Sorted by id so the registry content never depends on module load order.
	TArray<IOWTDuplicationAdapterProvider*> Providers = IModularFeatures::Get().GetModularFeatureImplementations<IOWTDuplicationAdapterProvider>(IOWTDuplicationAdapterProvider::GetFeatureName());
	Providers.Sort([](const IOWTDuplicationAdapterProvider& Left, const IOWTDuplicationAdapterProvider& Right){
		return Left.GetProviderId().LexicalLess(Right.GetProviderId());
	});
	for (const IOWTDuplicationAdapterProvider* Provider : Providers)
	{
		Provider->RegisterAdapters(AdapterRegistry);
	}
}

bool UOWTRuntimeActorDuplicator::ValidateRequest(AActor* Source, const FOWTDuplicationOptions& Options, FString& OutError) const
{
	const UWorld* World = GetWorld();
	const bool bHasWorld = World != nullptr;
	const bool bWorldRunning = bHasWorld && !World->bIsTearingDown;
	const bool bStandalone = bWorldRunning && World->GetNetMode() == NM_Standalone;
	const bool bFiniteOffset = !Options.WorldOffset.ContainsNaN();
	// The authored hierarchy validates the source in full; only what dereferencing it requires is checked here.
	const bool bSourceValid = IsValid(Source);
	const bool bAccepted = bStandalone && !bDuplicating && !bDispatching && bFiniteOffset && bSourceValid;

	if (!bHasWorld)
	{
		OutError = TEXT("Runtime duplication requires an initialized owner with a live world.");
	}
	else if (!bWorldRunning)
	{
		OutError = TEXT("The duplication world is shutting down.");
	}
	else if (bDuplicating)
	{
		OutError = TEXT("A duplication callback cannot reenter the same duplicator.");
	}
	else if (!bStandalone)
	{
		OutError = TEXT("Runtime duplication currently supports standalone worlds.");
	}
	else if (bDispatching)
	{
		OutError = TEXT("A procedural notification cannot reenter duplication synchronously.");
	}
	else if (!bFiniteOffset)
	{
		OutError = TEXT("The duplication offset must be finite.");
	}
	else if (!bSourceValid)
	{
		OutError = TEXT("The source Actor is no longer valid.");
	}
	return bAccepted;
}

void UOWTRuntimeActorDuplicator::CommitAdapters(TArray<TUniquePtr<IOWTDuplicationAdapter>>& Adapters, const TMap<UObject*, UObject*>& CompleteMapping, AActor& Result,
	const FOWTDuplicationOptions& Options, const FGuid& OperationId)
{
	// Install observations before announcing authored commit; activation can now report subsequent changes.
	const int32 FirstCommittedAdapter = CommittedAdapters.Num();
	for (auto& Adapter : Adapters)
	{
		if (!Adapter->HasCapturedSubjects())
		{
			continue;
		}
		Adapter->OnProceduralChanged.AddUObject(this, &UOWTRuntimeActorDuplicator::ForwardProceduralChanged);
		IOWTDuplicationAdapter* Preparing = Adapter.Get();
		CommittedAdapters.Add(MoveTemp(Adapter));
		Preparing->PrepareCommit(OperationId, Options, CompleteMapping);
	}
	if (Options.OnAuthoredCommitted)
	{
		Options.OnAuthoredCommitted(&Result);
	}
	for (int32 Index = FirstCommittedAdapter; Index < CommittedAdapters.Num(); ++Index)
	{
		CommittedAdapters[Index]->Commit(OperationId, Options, CompleteMapping);
	}
	for (int32 Index = CommittedAdapters.Num() - 1; Index >= FirstCommittedAdapter; --Index)
	{
		if (CommittedAdapters[Index]->GetProceduralComponents().IsEmpty())
		{
			CommittedAdapters.RemoveAt(Index);
		}
	}
}

void UOWTRuntimeActorDuplicator::ForwardProceduralChanged(const FOWTProceduralComponentSnapshot& Snapshot)
{
	TGuardValue<bool> Guard(bDispatching, true);
	OnProceduralChanged.Broadcast(Snapshot);
}
