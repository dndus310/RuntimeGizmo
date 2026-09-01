// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "ToolContextInterfaces.h"
#include "VTBTransformGizmoTypes.generated.h"

USTRUCT(BlueprintType)
struct FVTBTransformSelectionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	bool bEnableClickSelection = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	bool bRequireTransformTargetInterface = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	bool bClearSelectionOnBackgroundClick = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	bool bPreventSelectingActiveViewTarget = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selection", meta = (ClampMin = "1.0"))
	float TraceDistance = 100000.0f;
};

USTRUCT(BlueprintType)
struct FVTBTransformSnappingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping")
	bool bEnablePositionSnapping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping", meta = (EditCondition = "bEnablePositionSnapping"))
	FVector PositionGrid = FVector(10.0f, 10.0f, 10.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping")
	bool bEnableRotationSnapping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping", meta = (EditCondition = "bEnableRotationSnapping"))
	FRotator RotationGrid = FRotator(15.0f, 15.0f, 15.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping")
	bool bEnableScaleSnapping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snapping", meta = (EditCondition = "bEnableScaleSnapping", ClampMin = "0.0001"))
	float ScaleGrid = 0.1f;
};

USTRUCT(BlueprintType)
struct FVTBTransformHotkeySettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hotkeys")
	bool bEnableEditorHotkeys = true;
};

USTRUCT(BlueprintType)
struct FVTBTransformFeedbackSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
	bool bShowRotationDelta = true;
};

USTRUCT(BlueprintType)
struct FVTBTransformGizmoSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform Gizmo")
	EToolContextCoordinateSystem CoordinateSystem = EToolContextCoordinateSystem::World;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform Gizmo")
	EToolContextTransformGizmoMode TransformMode = EToolContextTransformGizmoMode::Translation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform Gizmo")
	FVTBTransformSelectionSettings Selection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform Gizmo")
	FVTBTransformSnappingSettings Snapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform Gizmo")
	FVTBTransformHotkeySettings Hotkeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform Gizmo")
	FVTBTransformFeedbackSettings Feedback;
};
