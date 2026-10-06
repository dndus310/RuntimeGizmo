#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OWTRuntimeActorDuplicator.generated.h"

class AActor;

/**
 * Synchronous, owner-bound duplication of standalone world Actor configurations.
 * Copies reflected user state, editable component configuration, owned instanced objects and
 * ChildActorComponent hierarchies. External references remain shared; internal references are remapped.
 * Construction runs normally. Restoration completes before the root and child Actors begin play.
 * Engine lifecycle, delegates, timers and opaque native/simulation state are not snapshots.
 * Implement IOWTRuntimeDuplicationParticipant for class-specific native configuration restoration.
 */
UCLASS(BlueprintType)
class OWTRUNTIMEDUPLICATION_API UOWTRuntimeActorDuplicator : public UObject
{
	GENERATED_BODY()

public:
	UOWTRuntimeActorDuplicator();

	virtual UWorld* GetWorld() const override;

	/** Owner must be this object's exact Outer and expose a live world. */
	UFUNCTION(BlueprintCallable, Category = "OWT|Runtime Duplication")
	bool Initialize(UObject* Owner);

	UFUNCTION(BlueprintCallable, Category = "OWT|Runtime Duplication")
	AActor* DuplicateActor(AActor* Source, const FVector& WorldOffset, FString& OutError);

	UFUNCTION(BlueprintCallable, Category = "OWT|Runtime Duplication")
	void Deinitialize();

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UObject> OwningObject;

	TMap<FString, int64> NameCounters;
	bool bDuplicating;
};
