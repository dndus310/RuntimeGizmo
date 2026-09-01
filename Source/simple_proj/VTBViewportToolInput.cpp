// Copyright Epic Games, Inc. All Rights Reserved.

#include "VTBViewportToolInput.h"

#include "CoreGlobals.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "InputRouter.h"
#include "InputState.h"
#include "InteractiveToolsContext.h"

void FVTBViewportToolInput::Reset()
{
	ReleaseCameraInputBlock();
	ReleaseMoveInputBlock();
	MouseState.Reset();
	KeyboardState.Reset();
}

EVTBViewportToolCommand FVTBViewportToolInput::Tick(
	UInteractiveToolsContext* ToolsContext,
	APlayerController* PlayerController,
	bool bTransformEditInProgress,
	bool bEnableEditorHotkeys)
{
	if (!ToolsContext || !ToolsContext->InputRouter)
	{
		ReleaseCameraInputBlock();
		ReleaseMoveInputBlock();
		KeyboardState.Reset();
		return EVTBViewportToolCommand::None;
	}

	const bool bRightMouseDown = PlayerController && PlayerController->IsInputKeyDown(EKeys::RightMouseButton);

	if (MouseState.LastUpdateFrame == GFrameCounter)
	{
		const bool bShouldBlockCamera = ShouldBlockCameraInput(ToolsContext, bTransformEditInProgress);
		SetCameraInputBlocked(PlayerController, bShouldBlockCamera);
		SetMoveInputBlocked(PlayerController, bShouldBlockCamera || !bRightMouseDown);
		return EVTBViewportToolCommand::None;
	}

	MouseState.LastUpdateFrame = GFrameCounter;

	bool bShouldBlockCamera = ShouldBlockCameraInput(ToolsContext, bTransformEditInProgress);
	const EVTBViewportToolCommand Command = UpdateHotkeys(
		PlayerController,
		ToolsContext,
		bTransformEditInProgress,
		bEnableEditorHotkeys);

	FInputDeviceState InputState;
	if (BuildMouseInputState(PlayerController, InputState))
	{
		ToolsContext->InputRouter->PostHoverInputEvent(InputState);
		bShouldBlockCamera |= ToolsContext->InputRouter->PostInputEvent(InputState);
		bShouldBlockCamera |= ShouldBlockCameraInput(ToolsContext, bTransformEditInProgress);
	}

	SetCameraInputBlocked(PlayerController, bShouldBlockCamera);
	SetMoveInputBlocked(PlayerController, bShouldBlockCamera || !bRightMouseDown);
	return Command;
}

bool FVTBViewportToolInput::HasMouseCapture(const UInteractiveToolsContext* ToolsContext) const
{
	return ToolsContext && ToolsContext->InputRouter && ToolsContext->InputRouter->HasActiveMouseCapture();
}

bool FVTBViewportToolInput::ShouldBlockCameraInput(
	const UInteractiveToolsContext* ToolsContext,
	bool bTransformEditInProgress) const
{
	return bTransformEditInProgress || HasMouseCapture(ToolsContext);
}

void FVTBViewportToolInput::SetCameraInputBlocked(APlayerController* PlayerController, bool bShouldBlock)
{
	if (!PlayerController)
	{
		ReleaseCameraInputBlock();
		return;
	}

	if (APlayerController* PreviousController = CameraInputBlock.PlayerController.Get())
	{
		if (PreviousController != PlayerController)
		{
			ReleaseCameraInputBlock();
		}
	}

	if (bShouldBlock == CameraInputBlock.bApplied)
	{
		if (bShouldBlock)
		{
			PlayerController->SetControlRotation(CameraInputBlock.LockedControlRotation);
		}
		return;
	}

	CameraInputBlock.bApplied = bShouldBlock;
	CameraInputBlock.PlayerController = bShouldBlock ? PlayerController : nullptr;
	if (bShouldBlock)
	{
		CameraInputBlock.LockedControlRotation = PlayerController->GetControlRotation();
	}

	PlayerController->SetIgnoreLookInput(bShouldBlock);
	if (bShouldBlock)
	{
		PlayerController->SetControlRotation(CameraInputBlock.LockedControlRotation);
	}
}

bool FVTBViewportToolInput::BuildMouseInputState(APlayerController* PlayerController, FInputDeviceState& InputState)
{
	if (!PlayerController)
	{
		return false;
	}

	if (PlayerController->IsInputKeyDown(EKeys::RightMouseButton))
	{
		MouseState.ResetButtons();
		return false;
	}

	float MouseX = 0.0f;
	float MouseY = 0.0f;
	if (!PlayerController->GetMousePosition(MouseX, MouseY))
	{
		return false;
	}

	FVector RayOrigin = FVector::ZeroVector;
	FVector RayDirection = FVector::ForwardVector;
	if (!PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, RayOrigin, RayDirection))
	{
		return false;
	}

	const FVector2D MousePosition(MouseX, MouseY);
	const bool bLeftMouseDown = PlayerController->IsInputKeyDown(EKeys::LeftMouseButton);
	const bool bMiddleMouseDown = PlayerController->IsInputKeyDown(EKeys::MiddleMouseButton);
	const bool bRightMouseDown = PlayerController->IsInputKeyDown(EKeys::RightMouseButton);

	InputState.InputDevice = EInputDevices::Mouse;
	InputState.Mouse.Position2D = MousePosition;
	InputState.Mouse.Delta2D = MouseState.bHasLastPosition ? MousePosition - MouseState.LastPosition : FVector2D::ZeroVector;
	InputState.Mouse.WorldRay = FRay(RayOrigin, RayDirection.GetSafeNormal(), true);
	InputState.Mouse.Left.SetStates(!MouseState.bWasLeftDown && bLeftMouseDown, bLeftMouseDown, MouseState.bWasLeftDown && !bLeftMouseDown);
	InputState.Mouse.Middle.SetStates(!MouseState.bWasMiddleDown && bMiddleMouseDown, bMiddleMouseDown, MouseState.bWasMiddleDown && !bMiddleMouseDown);
	InputState.Mouse.Right.SetStates(!MouseState.bWasRightDown && bRightMouseDown, bRightMouseDown, MouseState.bWasRightDown && !bRightMouseDown);
	InputState.SetModifierKeyStates(
		PlayerController->IsInputKeyDown(EKeys::LeftShift) || PlayerController->IsInputKeyDown(EKeys::RightShift),
		PlayerController->IsInputKeyDown(EKeys::LeftAlt) || PlayerController->IsInputKeyDown(EKeys::RightAlt),
		PlayerController->IsInputKeyDown(EKeys::LeftControl) || PlayerController->IsInputKeyDown(EKeys::RightControl),
		PlayerController->IsInputKeyDown(EKeys::LeftCommand) || PlayerController->IsInputKeyDown(EKeys::RightCommand));

	MouseState.LastPosition = MousePosition;
	MouseState.bHasLastPosition = true;
	MouseState.bWasLeftDown = bLeftMouseDown;
	MouseState.bWasMiddleDown = bMiddleMouseDown;
	MouseState.bWasRightDown = bRightMouseDown;
	return true;
}

EVTBViewportToolCommand FVTBViewportToolInput::UpdateHotkeys(
	APlayerController* PlayerController,
	const UInteractiveToolsContext* ToolsContext,
	bool bTransformEditInProgress,
	bool bEnableEditorHotkeys)
{
	if (!PlayerController)
	{
		KeyboardState.Reset();
		return EVTBViewportToolCommand::None;
	}

	const bool bTranslateDown = PlayerController->IsInputKeyDown(EKeys::W);
	const bool bRotateDown = PlayerController->IsInputKeyDown(EKeys::E);
	const bool bScaleDown = PlayerController->IsInputKeyDown(EKeys::R);
	const bool bToggleCoordinateSystemDown = PlayerController->IsInputKeyDown(EKeys::Tilde);
	const bool bControlDown = PlayerController->IsInputKeyDown(EKeys::LeftControl) || PlayerController->IsInputKeyDown(EKeys::RightControl);

	EVTBViewportToolCommand Command = EVTBViewportToolCommand::None;
	const bool bCanUseHotkeys = bEnableEditorHotkeys
		&& !bTransformEditInProgress
		&& !HasMouseCapture(ToolsContext)
		&& !PlayerController->IsInputKeyDown(EKeys::RightMouseButton);

	if (bCanUseHotkeys)
	{
		if (!KeyboardState.bWasToggleCoordinateSystemDown && bToggleCoordinateSystemDown && bControlDown)
		{
			Command = EVTBViewportToolCommand::ToggleCoordinateSystem;
		}
		else if (!KeyboardState.bWasTranslateDown && bTranslateDown)
		{
			Command = EVTBViewportToolCommand::Translate;
		}
		else if (!KeyboardState.bWasRotateDown && bRotateDown)
		{
			Command = EVTBViewportToolCommand::Rotate;
		}
		else if (!KeyboardState.bWasScaleDown && bScaleDown)
		{
			Command = EVTBViewportToolCommand::Scale;
		}
	}

	KeyboardState.bWasTranslateDown = bTranslateDown;
	KeyboardState.bWasRotateDown = bRotateDown;
	KeyboardState.bWasScaleDown = bScaleDown;
	KeyboardState.bWasToggleCoordinateSystemDown = bToggleCoordinateSystemDown;
	return Command;
}

void FVTBViewportToolInput::SetMoveInputBlocked(APlayerController* PlayerController, bool bShouldBlock)
{
	if (!PlayerController)
	{
		ReleaseMoveInputBlock();
		return;
	}

	if (APlayerController* PreviousController = MoveInputBlock.PlayerController.Get())
	{
		if (PreviousController != PlayerController)
		{
			ReleaseMoveInputBlock();
		}
	}

	if (bShouldBlock == MoveInputBlock.bApplied)
	{
		return;
	}

	MoveInputBlock.bApplied = bShouldBlock;
	MoveInputBlock.PlayerController = bShouldBlock ? PlayerController : nullptr;
	PlayerController->SetIgnoreMoveInput(bShouldBlock);
}

void FVTBViewportToolInput::ReleaseMoveInputBlock()
{
	if (!MoveInputBlock.bApplied)
	{
		MoveInputBlock.PlayerController.Reset();
		return;
	}

	if (APlayerController* PlayerController = MoveInputBlock.PlayerController.Get())
	{
		PlayerController->SetIgnoreMoveInput(false);
	}

	MoveInputBlock.bApplied = false;
	MoveInputBlock.PlayerController.Reset();
}

void FVTBViewportToolInput::ReleaseCameraInputBlock()
{
	if (!CameraInputBlock.bApplied)
	{
		CameraInputBlock.PlayerController.Reset();
		return;
	}

	if (APlayerController* PlayerController = CameraInputBlock.PlayerController.Get())
	{
		PlayerController->SetControlRotation(CameraInputBlock.LockedControlRotation);
		PlayerController->SetIgnoreLookInput(false);
	}

	CameraInputBlock.bApplied = false;
	CameraInputBlock.PlayerController.Reset();
	CameraInputBlock.LockedControlRotation = FRotator::ZeroRotator;
}

void FVTBViewportToolInput::FMouseState::Reset()
{
	LastPosition = FVector2D::ZeroVector;
	LastUpdateFrame = MAX_uint64;
	ResetButtons();
}

void FVTBViewportToolInput::FMouseState::ResetButtons()
{
	bHasLastPosition = false;
	bWasLeftDown = false;
	bWasMiddleDown = false;
	bWasRightDown = false;
}

void FVTBViewportToolInput::FKeyboardState::Reset()
{
	bWasTranslateDown = false;
	bWasRotateDown = false;
	bWasScaleDown = false;
	bWasToggleCoordinateSystemDown = false;
}
