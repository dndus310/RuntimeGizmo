#pragma once

#include "CoreMinimal.h"
#include "Events/OWTAttributeTypes.h"
#include "UObject/Object.h"
#include "OWTAttributeStateStore.generated.h"

class AActor;
class AVTBAttributeEditor;

struct FOWTRegisteredAttributeActor
{
	FOWTRegisteredAttributeActor() : Actor(), Baseline(FTransform::Identity), bNewObject(false)
	{
	}

	TWeakObjectPtr<AActor> Actor;
	FTransform Baseline;
	bool bNewObject;
};

// Stores observed Actor state. Only the owning editor can update it; JSON never enters this store.
UCLASS(BlueprintType, NotBlueprintable)
class VTBOWTEDITOR_API UOWTAttributeStateStore final : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "OWT|Attributes")
	FOWTAttributeSnapshot GetSnapshot() const;

private:
	friend class AVTBAttributeEditor;
	FGuid RegisterActor(AActor& Actor, bool bNewObject);
	AActor* ResolveActor(FGuid Id) const;
	bool ContainsActor(FGuid Id) const;
	void Observe(AActor* Actor, FGuid Id, FOWTAttributeSnapshot Context);
	bool MarkBaseline(FGuid Id);
	void Reset();

	TMap<FGuid, FOWTRegisteredAttributeActor> Actors;
	FOWTAttributeSnapshot Snapshot;
};
