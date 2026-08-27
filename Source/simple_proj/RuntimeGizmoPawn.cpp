// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeGizmoPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"

ARuntimeGizmoPawn::ARuntimeGizmoPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SceneRoot);
	CameraComponent->SetFieldOfView(90.0f);
	CameraComponent->bUsePawnControlRotation = false;
}

void ARuntimeGizmoPawn::BeginPlay()
{
	Super::BeginPlay();

	CurrentCameraSpeed = FMath::Clamp(GetEditorCameraSpeed(CameraSpeedSetting), MinCameraSpeed, MaxCameraSpeed);

	if (bUseDemoCameraPlacement)
	{
		SetActorLocation(FVector(-500.0, -500.0, 300.0));
		SetActorRotation(FRotator(-25.0, 45.0, 0.0));
	}
}

void ARuntimeGizmoPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateEditorViewportCamera(DeltaSeconds);
}

void ARuntimeGizmoPawn::UpdateEditorViewportCamera(float DeltaSeconds)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!bEnableEditorViewportCamera || !PlayerController)
	{
		return;
	}

	const bool bFreeCameraActive = PlayerController->IsInputKeyDown(EKeys::RightMouseButton);
	SetFreeCameraActive(PlayerController, bFreeCameraActive);

	const float WheelDelta = PlayerController->GetInputAnalogKeyState(EKeys::MouseWheelAxis);
	if (!FMath::IsNearlyZero(WheelDelta))
	{
		if (bFreeCameraActive)
		{
			ChangeCameraSpeedByMouseWheel(WheelDelta);
		}
		else
		{
			DollyCameraByMouseWheel(WheelDelta);
		}
	}

	if (!bFreeCameraActive)
	{
		return;
	}

	float MouseDeltaX = 0.0f;
	float MouseDeltaY = 0.0f;
	PlayerController->GetInputMouseDelta(MouseDeltaX, MouseDeltaY);

	if (!FMath::IsNearlyZero(MouseDeltaX) || !FMath::IsNearlyZero(MouseDeltaY))
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += MouseDeltaX * MouseLookSensitivity;
		NewRotation.Pitch = FMath::Clamp(
			FRotator::NormalizeAxis(NewRotation.Pitch - MouseDeltaY * MouseLookSensitivity),
			-90.0f + KINDA_SMALL_NUMBER,
			90.0f - KINDA_SMALL_NUMBER);
		NewRotation.Roll = 0.0f;
		SetActorRotation(NewRotation);
	}

	FVector MoveDirection = FVector::ZeroVector;
	MoveDirection += PlayerController->IsInputKeyDown(EKeys::W) ? CameraComponent->GetForwardVector() : FVector::ZeroVector;
	MoveDirection -= PlayerController->IsInputKeyDown(EKeys::S) ? CameraComponent->GetForwardVector() : FVector::ZeroVector;
	MoveDirection += PlayerController->IsInputKeyDown(EKeys::D) ? CameraComponent->GetRightVector() : FVector::ZeroVector;
	MoveDirection -= PlayerController->IsInputKeyDown(EKeys::A) ? CameraComponent->GetRightVector() : FVector::ZeroVector;
	MoveDirection += PlayerController->IsInputKeyDown(EKeys::E) ? FVector::UpVector : FVector::ZeroVector;
	MoveDirection -= PlayerController->IsInputKeyDown(EKeys::Q) ? FVector::UpVector : FVector::ZeroVector;

	if (!MoveDirection.IsNearlyZero())
	{
		const bool bFast = PlayerController->IsInputKeyDown(EKeys::LeftShift) || PlayerController->IsInputKeyDown(EKeys::RightShift);
		const bool bSlow = PlayerController->IsInputKeyDown(EKeys::LeftControl) || PlayerController->IsInputKeyDown(EKeys::RightControl);
		const float SpeedMultiplier = bFast ? FastMoveMultiplier : (bSlow ? SlowMoveMultiplier : 1.0f);
		AddActorWorldOffset(MoveDirection.GetSafeNormal() * GetFlightCameraSpeed() * SpeedMultiplier * DeltaSeconds, false);
	}
}

void ARuntimeGizmoPawn::SetFreeCameraActive(APlayerController* PlayerController, bool bActive)
{
	if (!PlayerController || bWasFreeCameraActive == bActive)
	{
		return;
	}

	bWasFreeCameraActive = bActive;

	if (bActive)
	{
		float MouseX = 0.0f;
		float MouseY = 0.0f;
		bHasStoredMousePosition = PlayerController->GetMousePosition(MouseX, MouseY);
		StoredMousePosition = FVector2D(MouseX, MouseY);

		if (bHideCursorWhileLooking)
		{
			PlayerController->bShowMouseCursor = false;
		}

		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(false);
		PlayerController->SetInputMode(InputMode);
	}
	else
	{
		PlayerController->bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);

		if (bHasStoredMousePosition)
		{
			PlayerController->SetMouseLocation(
				FMath::RoundToInt(StoredMousePosition.X),
				FMath::RoundToInt(StoredMousePosition.Y));
		}

		bHasStoredMousePosition = false;
	}
}

float ARuntimeGizmoPawn::GetEditorCameraSpeed(int32 SpeedSetting) const
{
	static constexpr float EditorSpeeds[] = { 0.033f, 0.1f, 0.33f, 1.0f, 3.0f, 8.0f, 16.0f, 32.0f };
	const int32 ClampedSetting = FMath::Clamp(SpeedSetting, 1, UE_ARRAY_COUNT(EditorSpeeds));
	return EditorSpeeds[ClampedSetting - 1];
}

float ARuntimeGizmoPawn::GetFlightCameraSpeed() const
{
	const float Speed = FMath::Clamp(CurrentCameraSpeed, MinCameraSpeed, MaxCameraSpeed);
	return Speed * CameraUnitsPerSecond;
}

void ARuntimeGizmoPawn::ChangeCameraSpeedByMouseWheel(float WheelDelta)
{
	const float SpeedDelta = CurrentCameraSpeed * 0.1f * WheelDelta;
	CurrentCameraSpeed = FMath::Clamp(CurrentCameraSpeed + SpeedDelta, MinCameraSpeed, MaxCameraSpeed);
}

void ARuntimeGizmoPawn::DollyCameraByMouseWheel(float WheelDelta)
{
	const FVector DollyDirection = CameraComponent ? CameraComponent->GetForwardVector() : GetActorForwardVector();
	const float DollyDistance = GetEditorCameraSpeed(MouseScrollCameraSpeedSetting) * MouseWheelDollyScale * WheelDelta;
	AddActorWorldOffset(DollyDirection * DollyDistance, false);
}
