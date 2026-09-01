// Copyright Epic Games, Inc. All Rights Reserved.

#include "VTBGizmoPlayerController.h"

#include "VTBInteractiveToolsSubsystem.h"
#include "VTBTransformGizmoComponent.h"

AVTBGizmoPlayerController::AVTBGizmoPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
	DefaultClickTraceChannel = ECC_Visibility;

	TransformGizmoComponent = CreateDefaultSubobject<UVTBTransformGizmoComponent>(TEXT("TransformGizmoComponent"));
}

void AVTBGizmoPlayerController::BeginPlay()
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

void AVTBGizmoPlayerController::PlayerTick(float DeltaTime)
{
	if (UWorld* World = GetWorld())
	{
		if (UVTBInteractiveToolsSubsystem* ToolsSubsystem = World->GetSubsystem<UVTBInteractiveToolsSubsystem>())
		{
			ToolsSubsystem->RefreshInputStateAndCameraLock();
		}
	}

	Super::PlayerTick(DeltaTime);
}

bool AVTBGizmoPlayerController::SelectTransformTarget(AActor* TargetActor)
{
	return TransformGizmoComponent
		? TransformGizmoComponent->SelectTransformTarget(TargetActor)
		: false;
}
