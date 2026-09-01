// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseBehaviors/BehaviorTargetInterfaces.h"
#include "Engine/EngineTypes.h"
#include "InputBehaviorSet.h"
#include "Templates/Function.h"
#include "UObject/Object.h"
#include "VTBTransformSelection.generated.h"

class APlayerController;
class AActor;
class USingleClickInputBehavior;

UCLASS(Transient)
class SIMPLE_PROJ_API UVTBTransformSelection final : public UObject, public IInputBehaviorSource, public IClickBehaviorTarget
{
	GENERATED_BODY()

public:
	void Initialize(
		APlayerController* InPlayerController,
		TUniqueFunction<bool()> CanChangeSelectionCallbackIn,
		TUniqueFunction<void(AActor*)> SelectTargetCallbackIn,
		ECollisionChannel InTraceChannel,
		double InTraceDistance,
		bool bInRequireTransformTargetInterface,
		bool bInClearSelectionOnBackgroundClick);

	void Shutdown();

	virtual const UInputBehaviorSet* GetInputBehaviors() const override { return BehaviorSet; }
	virtual FInputRayHit IsHitByClick(const FInputDeviceRay& ClickPos) override;
	virtual void OnClicked(const FInputDeviceRay& ClickPos) override;

private:
	AActor* FindTransformTarget(const FInputDeviceRay& ClickPos, FHitResult& HitOut) const;
	bool IsSelectableTransformTarget(AActor* Actor) const;

	UPROPERTY(Transient)
	TObjectPtr<USingleClickInputBehavior> ClickBehavior;

	UPROPERTY(Transient)
	TObjectPtr<UInputBehaviorSet> BehaviorSet;

	TWeakObjectPtr<APlayerController> PlayerController;
	TUniqueFunction<bool()> CanChangeSelectionCallback = []() { return true; };
	TUniqueFunction<void(AActor*)> SelectTargetCallback = [](AActor*) {};
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
	double TraceDistance = 100000.0;
	bool bRequireTransformTargetInterface = false;
	bool bClearSelectionOnBackgroundClick = true;
};
