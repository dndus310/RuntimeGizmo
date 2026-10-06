#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "StructUtils/InstancedStruct.h"
#include "Duplication/OWTRuntimeDuplicationParticipant.h"
#include "OWTDuplicationValidation.generated.h"

class UStaticMeshComponent;

UCLASS()
class UOWTDuplicationFixtureSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	virtual void OnRegister() override;
};

UCLASS(DefaultToInstanced, EditInlineNew)
class UOWTDuplicationFixtureData : public UObject
{
	GENERATED_BODY()

public:
	UOWTDuplicationFixtureData();

	UPROPERTY()
	TObjectPtr<AActor> ActorReference;
	UPROPERTY()
	TObjectPtr<USceneComponent> ComponentReference;
	UPROPERTY(Instanced)
	TObjectPtr<UOWTDuplicationFixtureData> Nested;
	UPROPERTY(EditAnywhere)
	int32 Number;
};

UCLASS(Blueprintable)
class AOWTDuplicationFixtureActor : public AActor, public IOWTRuntimeDuplicationParticipant
{
	GENERATED_BODY()

public:
	AOWTDuplicationFixtureActor();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual bool RestoreRuntimeDuplicateState_Implementation(UObject* SourceObject,
	                                                         const TMap<UObject*, UObject*>& DuplicatedObjects,
	                                                         FString& OutError) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SceneRoot;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(Instanced, EditAnywhere)
	TObjectPtr<UOWTDuplicationFixtureData> Data;
	UPROPERTY()
	TObjectPtr<AActor> SelfReference;
	UPROPERTY()
	TObjectPtr<AActor> ExternalReference;
	UPROPERTY()
	TArray<TObjectPtr<UObject>> References;
	UPROPERTY()
	TMap<TObjectPtr<UObject>, TObjectPtr<UObject>> ReferenceMap;
	UPROPERTY()
	TSoftObjectPtr<UObject> UnloadedAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InstanceNumber;
	UPROPERTY()
	bool bDestroyOnConstruction;
	UPROPERTY()
	bool bRejectDuplicateRestore;
	UPROPERTY(Transient)
	int32 BeginPlayCount;
	UPROPERTY(Transient)
	int32 NumberAtBeginPlay;
	UPROPERTY(Transient)
	int32 BlueprintNumberAtBeginPlay;
	UPROPERTY(Transient)
	int32 DataNumberAtBeginPlay;
	UPROPERTY(Transient)
	bool bReferencesValidAtBeginPlay;
	UPROPERTY(Transient)
	FTransform TransformAtBeginPlay;
	UPROPERTY(Transient)
	FVector RegisteredComponentLocationAtBeginPlay;
	UPROPERTY(Transient)
	int32 NativeNumberAtBeginPlay;

	int32 NativeNumber;
};

UCLASS()
class AOWTDuplicationChildFixtureActor : public AOWTDuplicationFixtureActor
{
	GENERATED_BODY()
};

UCLASS()
class AOWTDuplicationGrandchildFixtureActor : public AOWTDuplicationFixtureActor
{
	GENERATED_BODY()
};

UCLASS()
class AOWTDuplicationOpaqueFixtureActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FInstancedStruct Payload;
};

bool ValidateOWTRuntimeDuplication();
