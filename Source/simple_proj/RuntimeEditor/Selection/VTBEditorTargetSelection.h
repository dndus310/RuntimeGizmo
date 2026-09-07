#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "VTBEditorTargetSelection.generated.h"

class AActor;
class USceneComponent;
class UTransformProxy;
class UWorld;

UCLASS(Transient)
class SIMPLE_PROJ_API UVTBEditorTargetSelection : public UObject
{
	GENERATED_BODY()

public:
	bool SetSelection(UWorld* World, const TArray<TWeakObjectPtr<AActor>>& Actors);
	bool RefreshSelection();
	void RebuildFromCurrentTransforms();

	UTransformProxy* GetTransformProxy() const { return TransformProxy; }
	void GetSelectedActors(TArray<AActor*>& OutActors) const;

private:
	bool RebuildTargetIfNeeded(bool bForceRebuild = false);

private:
	UPROPERTY(Transient)
	TObjectPtr<UTransformProxy> TransformProxy;

	UPROPERTY(Transient)
	TWeakObjectPtr<UWorld> TargetWorld;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> RequestedActors;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<USceneComponent>> TargetComponents;
};
