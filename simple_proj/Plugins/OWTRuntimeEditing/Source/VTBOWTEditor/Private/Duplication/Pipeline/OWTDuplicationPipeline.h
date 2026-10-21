#pragma once

#include "CoreMinimal.h"
#include "Duplication/Capture/OWTHierarchyCollector.h"
#include "Duplication/OWTDuplicationAdapter.h"
#include "Duplication/Pipeline/OWTAdapterDispatcher.h"
#include "Duplication/Pipeline/OWTDuplicationContext.h"
#include "Duplication/Properties/OWTUnsupportedStructCache.h"

class AActor;
class FOWTSnapshotRestorer;
class UWorld;
struct FOWTDuplicationOptions;
struct FOWTRootSnapshot;

/**
 * One duplication operation from validation to the point of authored commit.
 *
 *   Prepare : adapters -> hierarchy collection -> adapter capture -> root snapshots -> reference validation
 *   Execute : names -> spawn and restore each root -> cross-root finalize -> final checks
 *
 * Prepare never changes the world, so it doubles as a preflight. Execute destroys everything it created when any
 * step fails. Commit stays with the duplicator because committed adapters outlive the operation.
 */
class FOWTDuplicationPipeline
{
public:
	FOWTDuplicationPipeline(UWorld& World, AActor& InSource, const FOWTDuplicationOptions& Options,
	                        const FOWTDuplicationAdapterRegistry& InRegistry, TMap<FString, int64>& InNameCounters,
	                        FString& Error);
	~FOWTDuplicationPipeline();
	bool Prepare();
	/** IsOwnerAlive is checked after each root because construction callbacks may release the duplicator owner. */
	bool Execute(TFunctionRef<bool()> IsOwnerAlive);
	AActor* GetResult() const;
	/** Every duplicate Actor in authored order, including ChildActorComponent children and attached Actors. */
	TArray<AActor*> GetCreatedActors() const;
	const FOWTObjectMap& GetCompleteMapping() const;
	FOWTAdapterDispatcher& GetAdapters();

private:
	bool CaptureSnapshots();
	/** Names are reserved only when duplication proceeds, so a preflight never consumes a number. */
	bool ReserveNames();
	bool SpawnRoots(TFunctionRef<bool()> IsOwnerAlive);
	bool FinalizeRoots();
	/** Construction and restoration run user code; the sources, the request and the owner must have survived it. */
	bool CheckAfterCallbacks(TFunctionRef<bool()> IsOwnerAlive);
	bool CheckBeforeCommit();
	bool IsCancellationRequested() const;
	void Rollback();

private:
	/** A copy, so factories and callbacks that edit the owner's registry only affect later operations. */
	const FOWTDuplicationAdapterRegistry Registry;
	FOWTAdapterDispatcher Adapters;
	FOWTUnsupportedStructCache UnsupportedStructs;
	FOWTDuplicationContext Context;
	FOWTHierarchyCollector Collector;
	AActor& Source;
	TMap<FString, int64>& NameCounters;
	TArray<TUniquePtr<FOWTRootSnapshot>> Snapshots;
	TArray<TUniquePtr<FOWTSnapshotRestorer>> Restorers;
	TArray<TWeakObjectPtr<AActor>> CreatedRoots;
	TArray<FName> ReservedNames;
	FOWTObjectMap CompleteMapping;
};
