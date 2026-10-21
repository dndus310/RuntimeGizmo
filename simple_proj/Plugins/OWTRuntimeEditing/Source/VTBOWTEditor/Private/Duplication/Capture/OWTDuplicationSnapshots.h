#pragma once

#include "CoreMinimal.h"
#include "Duplication/Pipeline/OWTDuplicationContext.h"
#include "UObject/StrongObjectPtr.h"

class AActor;
class FOWTPropertyValue;
class UActorComponent;
class UChildActorComponent;
class UMaterialInstanceDynamic;
class USceneComponent;

/** Reflected values of one authored object that the core copies, minus adapter-owned properties. */
struct FOWTObjectSnapshot
{
	/** Returns nullptr with Context.Error set when a property needs an adapter or ownership is contested. */
	static TUniquePtr<FOWTObjectSnapshot> Capture(UObject& Source, const FOWTDuplicationContext& Context);
	~FOWTObjectSnapshot();
	void ApplyTo(UObject& Destination, const FOWTObjectMap& Mapping) const;

	TStrongObjectPtr<UObject> Source;
	/** MIDs restore parameters through their API, so their values are first applied to this transient copy. */
	TStrongObjectPtr<UMaterialInstanceDynamic> MaterialSnapshot;
	TArray<TUniquePtr<FOWTPropertyValue>> Properties;

private:
	explicit FOWTObjectSnapshot(UObject& InSource);
	void ApplyToMaterial(UMaterialInstanceDynamic& Destination, const FOWTObjectMap& Mapping) const;
};

/** Component lifecycle and native state restored through engine APIs instead of reflected values. */
struct FOWTComponentSnapshot
{
	explicit FOWTComponentSnapshot(UActorComponent& Component);
	~FOWTComponentSnapshot();

	TStrongObjectPtr<UActorComponent> Source;
	AActor* ChildActor;
	USceneComponent* Parent;
	FName Socket;
	FTransform RelativeTransform;
	FTransform WorldTransform;
	TArray<TUniquePtr<FOWTPropertyValue>> BodyProperties;
	TArray<FTransform> InstanceTransforms;
	TArray<float> InstanceCustomData;
	FVector LinearVelocity;
	FVector AngularVelocity;
	int32 NumCustomDataFloats;
	float TickInterval;
	bool bRegistered;
	bool bActive;
	bool bTickEnabled;
	bool bSimulating;
	bool bAwake;
};

/** Actor-level runtime flags and the destination transform including the requested offset. */
struct FOWTActorSnapshot
{
	FOWTActorSnapshot(AActor& Actor, const FVector& Offset);

	TStrongObjectPtr<AActor> Source;
	UChildActorComponent* ParentComponent;
	FTransform Transform;
	float TickInterval;
	bool bHidden;
	bool bCollision;
	bool bTickEnabled;
};

/** Everything captured for one spawned root Actor and its ChildActorComponent descendants. */
struct FOWTRootSnapshot
{
	explicit FOWTRootSnapshot(AActor& InRoot);
	~FOWTRootSnapshot();

	AActor& Root;
	/** Actors in capture order. The first entry is Root. */
	TArray<TUniquePtr<FOWTActorSnapshot>> Actors;
	TArray<TUniquePtr<FOWTObjectSnapshot>> Objects;
	TArray<TUniquePtr<FOWTComponentSnapshot>> Components;
	/** Every captured source object. Values stay null until restoration creates the destination. */
	FOWTObjectMap Mapping;
};
