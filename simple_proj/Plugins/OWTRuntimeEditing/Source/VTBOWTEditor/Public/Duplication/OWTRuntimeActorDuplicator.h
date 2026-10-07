#pragma once

#include "CoreMinimal.h"
#include "Duplication/OWTDuplicationAdapter.h"
#include "UObject/Object.h"
#include "OWTRuntimeActorDuplicator.generated.h"

class AActor;

/**
 * Synchronous, owner-bound duplication of standalone world Actor configurations.
 * Copies reflected user state, editable component configuration, owned instanced objects and
 * Authored attachments and ChildActorComponent hierarchies. External references remain shared.
 * Root and ChildActorComponent configuration is restored before BeginPlay. Ordinary attached Actors
 * can begin play at different times; their shared reference map is restored at authored commit.
 * Procedural execution remains suppressed until that commit. Construction runs normally.
 * Engine lifecycle, delegates, timers and opaque native/simulation state are not
 * snapshots. Implement IOWTRuntimeDuplicationParticipant for class-specific native configuration restoration.
 */
UCLASS(BlueprintType)
class VTBOWTEDITOR_API UOWTRuntimeActorDuplicator : public UObject
{
	GENERATED_BODY()

public:
	UOWTRuntimeActorDuplicator();

	virtual UWorld* GetWorld() const override;
	virtual void BeginDestroy() override;

	/** Owner must be this object's exact Outer and expose a live world. */
	UFUNCTION(BlueprintCallable, Category = "OWT|Runtime Duplication")
	bool Initialize(UObject* Owner);

	UFUNCTION(BlueprintCallable, Category = "OWT|Runtime Duplication")
	AActor* DuplicateActor(AActor* Source, const FVector& WorldOffset, FString& OutError);

	UFUNCTION(BlueprintCallable, Category = "OWT|Runtime Duplication")
	void Deinitialize();

	AActor* DuplicateActorWithOptions(AActor* Source, const FOWTDuplicationOptions& Options, const FGuid& OperationId,
	                                  FString& OutError);
	void Tick(float DeltaTime);

	TArray<FOWTProceduralComponentSnapshot> GetProceduralComponents() const;
	FOWTDuplicationAdapterRegistry& GetAdapterRegistry();

private:
	void RegisterAdapterProviders();

	void CommitAdapters(TArray<TUniquePtr<IOWTDuplicationAdapter>>& Adapters,
	                    const TMap<UObject*, UObject*>& CompleteMapping, AActor& Result,
	                    const FOWTDuplicationOptions& Options, const FGuid& OperationId);
	void ForwardProceduralChanged(const FOWTProceduralComponentSnapshot& Snapshot);

public:
	FOWTProceduralChanged OnProceduralChanged;

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UObject> OwningObject;

	TMap<FString, int64> NameCounters;
	FOWTDuplicationAdapterRegistry AdapterRegistry;
	TArray<TUniquePtr<IOWTDuplicationAdapter>> CommittedAdapters;
	bool bDuplicating;
	bool bDispatching;
};
