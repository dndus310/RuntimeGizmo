#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "VTBOWTEditorGizmoSelection.generated.h"

class AActor;
class USceneComponent;
class UTransformProxy;
class UWorld;

UCLASS()
class UVTBOWTEditorGizmoSelection : public UObject
{
	GENERATED_BODY()

public:
	bool SetSelection(UWorld* InWorld, const TArray<TWeakObjectPtr<AActor>>& Actors, USceneComponent* FrameComponent);
	USceneComponent* GetSelectionFrame() const;
	bool IsSelectionStateStale() const;
	bool RefreshSelection();
	void RebuildFromCurrentTransforms();
	void GetSelectedActors(TArray<AActor*>& OutActors) const;
	UTransformProxy* GetTransformProxy() const;
	void Reset();

private:
	struct FTargetState
	{
		TWeakObjectPtr<USceneComponent> RootComponent;
		TWeakObjectPtr<USceneComponent> FrameComponent;
		FTransform RootTransform = FTransform::Identity;
		FTransform FrameTransform = FTransform::Identity;
	};

	void CollectTargetComponents(TArray<USceneComponent*>& OutComponents) const;
	USceneComponent* GetFrameForRoot(USceneComponent* RootComponent) const;
	bool IsTargetStateCurrent(const TArray<USceneComponent*>& Components) const;
	bool RebuildTargetIfNeeded(bool bForceRebuild = false);
	void CacheTargetTransforms(UTransformProxy* Proxy, FTransform Transform);

	TWeakObjectPtr<UWorld> TargetWorld;
	TArray<TWeakObjectPtr<AActor>> RequestedActors;
	TWeakObjectPtr<USceneComponent> RequestedFrameComponent;
	TArray<FTargetState> Targets;

	UPROPERTY(Transient)
	TObjectPtr<UTransformProxy> TransformProxy;
};
