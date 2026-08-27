// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeGizmoPlayerController.h"

#include "InputCoreTypes.h"
#include "RuntimeTransformGizmoComponent.h"

ARuntimeGizmoPlayerController::ARuntimeGizmoPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
	DefaultClickTraceChannel = ECC_Visibility;

	RuntimeTransformGizmoComponent = CreateDefaultSubobject<URuntimeTransformGizmoComponent>(TEXT("RuntimeTransformGizmoComponent"));
}

void ARuntimeGizmoPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}

void ARuntimeGizmoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);
	InputComponent->BindKey(EKeys::W, IE_Pressed, this, &ARuntimeGizmoPlayerController::HandleTranslateMode);
	InputComponent->BindKey(EKeys::E, IE_Pressed, this, &ARuntimeGizmoPlayerController::HandleRotateMode);
	InputComponent->BindKey(EKeys::R, IE_Pressed, this, &ARuntimeGizmoPlayerController::HandleScaleMode);
	InputComponent->BindKey(EKeys::Tilde, IE_Pressed, this, &ARuntimeGizmoPlayerController::HandleToggleCoordinateSystem);
}

void ARuntimeGizmoPlayerController::SelectTransformableActor(AActor* Actor)
{
	if (RuntimeTransformGizmoComponent)
	{
		RuntimeTransformGizmoComponent->SelectActor(Actor);
	}
}

void ARuntimeGizmoPlayerController::HandleTranslateMode()
{
	if (IsInputKeyDown(EKeys::RightMouseButton))
	{
		return;
	}

	if (RuntimeTransformGizmoComponent)
	{
		RuntimeTransformGizmoComponent->SetTranslationMode();
	}
}

void ARuntimeGizmoPlayerController::HandleRotateMode()
{
	if (IsInputKeyDown(EKeys::RightMouseButton))
	{
		return;
	}

	if (RuntimeTransformGizmoComponent)
	{
		RuntimeTransformGizmoComponent->SetRotationMode();
	}
}

void ARuntimeGizmoPlayerController::HandleScaleMode()
{
	if (IsInputKeyDown(EKeys::RightMouseButton))
	{
		return;
	}

	if (RuntimeTransformGizmoComponent)
	{
		RuntimeTransformGizmoComponent->SetScaleMode();
	}
}

void ARuntimeGizmoPlayerController::HandleToggleCoordinateSystem()
{
	const bool bControlDown = IsInputKeyDown(EKeys::LeftControl) || IsInputKeyDown(EKeys::RightControl);
	if (!bControlDown || IsInputKeyDown(EKeys::RightMouseButton))
	{
		return;
	}

	if (RuntimeTransformGizmoComponent)
	{
		RuntimeTransformGizmoComponent->ToggleCoordinateSystem();
	}
}
