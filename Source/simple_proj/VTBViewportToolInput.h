// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class APlayerController;
class UInteractiveToolsContext;
struct FInputDeviceState;

enum class EVTBViewportToolCommand : uint8
{
	None,
	Translate,
	Rotate,
	Scale,
	ToggleCoordinateSystem
};

class FVTBViewportToolInput
{
public:
	void Reset();
	EVTBViewportToolCommand Tick(
		UInteractiveToolsContext* ToolsContext,
		APlayerController* PlayerController,
		bool bTransformEditInProgress,
		bool bEnableEditorHotkeys);

	bool HasMouseCapture(const UInteractiveToolsContext* ToolsContext) const;
	bool ShouldBlockCameraInput(const UInteractiveToolsContext* ToolsContext, bool bTransformEditInProgress) const;
	void SetCameraInputBlocked(APlayerController* PlayerController, bool bShouldBlock);

private:
	struct FMouseState
	{
		FVector2D LastPosition = FVector2D::ZeroVector;
		uint64 LastUpdateFrame = MAX_uint64;
		bool bHasLastPosition = false;
		bool bWasLeftDown = false;
		bool bWasMiddleDown = false;
		bool bWasRightDown = false;

		void Reset();
		void ResetButtons();
	};

	struct FKeyboardState
	{
		bool bWasTranslateDown = false;
		bool bWasRotateDown = false;
		bool bWasScaleDown = false;
		bool bWasToggleCoordinateSystemDown = false;

		void Reset();
	};

	struct FCameraInputBlock
	{
		bool bApplied = false;
		TWeakObjectPtr<APlayerController> PlayerController;
		FRotator LockedControlRotation = FRotator::ZeroRotator;
	};

	struct FMoveInputBlock
	{
		bool bApplied = false;
		TWeakObjectPtr<APlayerController> PlayerController;
	};

	bool BuildMouseInputState(APlayerController* PlayerController, FInputDeviceState& InputState);
	EVTBViewportToolCommand UpdateHotkeys(
		APlayerController* PlayerController,
		const UInteractiveToolsContext* ToolsContext,
		bool bTransformEditInProgress,
		bool bEnableEditorHotkeys);
	void SetMoveInputBlocked(APlayerController* PlayerController, bool bShouldBlock);
	void ReleaseMoveInputBlock();
	void ReleaseCameraInputBlock();

	FMouseState MouseState;
	FKeyboardState KeyboardState;
	FCameraInputBlock CameraInputBlock;
	FMoveInputBlock MoveInputBlock;
};
