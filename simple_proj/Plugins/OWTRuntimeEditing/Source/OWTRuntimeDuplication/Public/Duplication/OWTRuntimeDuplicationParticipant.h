#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OWTRuntimeDuplicationParticipant.generated.h"

UINTERFACE(BlueprintType)
class OWTRUNTIMEDUPLICATION_API UOWTRuntimeDuplicationParticipant : public UInterface
{
	GENERATED_BODY()
};

/** Optional extension for configuration stored outside the supported reflected properties. */
class OWTRUNTIMEDUPLICATION_API IOWTRuntimeDuplicationParticipant
{
	GENERATED_BODY()

public:
	virtual bool RestoreRuntimeDuplicateState_Implementation(UObject* SourceObject,
	                                                         const TMap<UObject*, UObject*>& DuplicatedObjects,
	                                                         FString& OutError);

	/**
	 * Called on each destination Actor, component or owned object after the complete hierarchy is mapped,
	 * before any duplicate Actor begins play. Read custom native configuration from SourceObject and
	 * use DuplicatedObjects to translate internal references. Recreate resources, never copy live handles.
	 * Returning false rolls back the newly spawned root and its ChildActorComponent hierarchy.
	 * Construction and initialization callbacks have already run and their external side effects cannot be undone.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "OWT|Runtime Duplication")
	bool RestoreRuntimeDuplicateState(UObject* SourceObject, const TMap<UObject*, UObject*>& DuplicatedObjects,
	                                  FString& OutError);
};
