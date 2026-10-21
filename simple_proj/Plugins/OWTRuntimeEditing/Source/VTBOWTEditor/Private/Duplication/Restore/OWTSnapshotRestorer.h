#pragma once

#include "CoreMinimal.h"
#include "Duplication/Pipeline/OWTDuplicationContext.h"

class AActor;
class UActorComponent;
class UMaterialInstanceDynamic;
class UOWTRestoreHookComponent;
class USceneComponent;
struct FOWTActorSnapshot;
struct FOWTComponentSnapshot;
struct FOWTRootSnapshot;

/**
 * Spawns one root snapshot and restores it before any duplicate Actor begins play.
 *
 * Construction runs normally inside FinishSpawning. A temporary restore hook component initializes right after it,
 * which is the last point before BeginPlay, and restores captured state there. When several roots belong to one
 * hierarchy, participants are deferred to FinalizeHierarchy so cross-root references are mapped first.
 */
class FOWTSnapshotRestorer
{
public:
	FOWTSnapshotRestorer(const FOWTDuplicationContext& InContext, const FOWTRootSnapshot& InSnapshot,
	                     FName InReservedName, bool bInDeferParticipants);
	/** Returns the restored root, or nullptr with Context.Error set after destroying any partial result. */
	AActor* Spawn();
	/** Re-applies state with the mapping of every root, then runs deferred participants. */
	bool FinalizeHierarchy(const FOWTObjectMap& CompleteMapping);
	const FOWTObjectMap& GetMapping() const;

private:
	enum class EResolvePhase : uint8
	{
		/** Only Instance components are created; construction recreates every other component. */
		BeforeConstruction,
		/** Anything construction did not recreate is created now. */
		AfterConstruction
	};

	AActor* SpawnDeferred(const FTransform& Transform);
	void FinishSpawningWithRestoreHook(const FTransform& Transform);
	bool ResolveObjects(EResolvePhase Phase);
	bool ResolveObject(UObject& Original, UObject& NewOuter, EResolvePhase Phase, UObject*& OutDestination);
	UObject* ResolveMaterial(UMaterialInstanceDynamic& Original, UObject& NewOuter) const;
	UObject* CreateObject(UObject& Original, UObject& NewOuter) const;
	/** Copies captured reflected values into every resolved destination, translating references through Mapping. */
	void ApplyCapturedState();
	/** Detects objects destroyed by a user callback since the mapping was built. */
	bool ValidateMapping();
	void RestoreAfterConstruction();
	bool RestoreActor(const FOWTActorSnapshot& State);
	bool ResolveChildActor(const FOWTActorSnapshot& State);
	bool ReplaceConstructedComponents(const FOWTActorSnapshot& State, AActor& Destination);
	bool RestoreComponents(const FOWTActorSnapshot& ActorState, AActor& Destination);
	bool ApplyComponentConfiguration(const FOWTComponentSnapshot& State, UActorComponent& Component);
	bool ActivateComponent(const FOWTComponentSnapshot& State, UActorComponent& Component, AActor& Destination);
	void RestoreActorFlags(const FOWTActorSnapshot& State, AActor& Destination);
	bool RestoreSceneTransform(const FOWTComponentSnapshot& State, USceneComponent& Scene);
	bool RestoreSceneTransforms();
	/** Runs IOWTRuntimeDuplicationParticipant on every destination. A rejection destroys the root. */
	bool RestoreParticipants();
	bool IsOwnedBy(const FOWTComponentSnapshot& State, const FOWTActorSnapshot& ActorState) const;

private:
	const FOWTDuplicationContext& Context;
	const FOWTRootSnapshot& Snapshot;
	AActor* Result;
	UOWTRestoreHookComponent* RestoreHook;
	FOWTObjectMap Mapping;
	FName ReservedName;
	bool bDeferParticipants;
	bool bRestored;
	bool bSucceeded;
};
