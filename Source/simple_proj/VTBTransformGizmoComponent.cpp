// Copyright Epic Games, Inc. All Rights Reserved.

#include "VTBTransformGizmoComponent.h"

#include "GameFramework/PlayerController.h"
#include "VTBInteractiveToolsSubsystem.h"

UVTBTransformGizmoComponent::UVTBTransformGizmoComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UVTBTransformGizmoComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem())
	{
		ToolsSubsystem->ApplySettings(Settings);
		ToolsSubsystem->InitializeToolsContext(Cast<APlayerController>(GetOwner()));
	}
}

void UVTBTransformGizmoComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem())
	{
		ToolsSubsystem->ShutdownToolsContextForPlayer(Cast<APlayerController>(GetOwner()));
	}

	Super::EndPlay(EndPlayReason);
}

bool UVTBTransformGizmoComponent::SelectTransformTarget(AActor* TargetActor)
{
	if (UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem())
	{
		return ToolsSubsystem->SelectTransformTarget(TargetActor);
	}

	return false;
}

void UVTBTransformGizmoComponent::ClearSelection()
{
	if (UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem())
	{
		ToolsSubsystem->ClearSelection();
	}
}

void UVTBTransformGizmoComponent::ApplySettings(const FVTBTransformGizmoSettings& InSettings)
{
	Settings = InSettings;
	if (UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem())
	{
		ToolsSubsystem->ApplySettings(Settings);
	}
}

void UVTBTransformGizmoComponent::SetTransformMode(EToolContextTransformGizmoMode NewMode)
{
	Settings.TransformMode = NewMode;
	if (UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem())
	{
		ToolsSubsystem->SetTransformMode(NewMode);
	}
}

void UVTBTransformGizmoComponent::SetTranslationMode()
{
	Settings.TransformMode = EToolContextTransformGizmoMode::Translation;
	if (UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem())
	{
		ToolsSubsystem->SetTranslationMode();
	}
}

void UVTBTransformGizmoComponent::SetRotationMode()
{
	Settings.TransformMode = EToolContextTransformGizmoMode::Rotation;
	if (UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem())
	{
		ToolsSubsystem->SetRotationMode();
	}
}

void UVTBTransformGizmoComponent::SetScaleMode()
{
	Settings.TransformMode = EToolContextTransformGizmoMode::Scale;
	if (UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem())
	{
		ToolsSubsystem->SetScaleMode();
	}
}

void UVTBTransformGizmoComponent::SetCoordinateSystem(EToolContextCoordinateSystem NewCoordinateSystem)
{
	Settings.CoordinateSystem = NewCoordinateSystem;
	if (UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem())
	{
		ToolsSubsystem->SetCoordinateSystem(NewCoordinateSystem);
	}
}

void UVTBTransformGizmoComponent::ToggleCoordinateSystem()
{
	const UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem();
	const EToolContextCoordinateSystem CurrentCoordinateSystem = ToolsSubsystem
		? ToolsSubsystem->GetSettings().CoordinateSystem
		: Settings.CoordinateSystem;

	SetCoordinateSystem(CurrentCoordinateSystem == EToolContextCoordinateSystem::World
		? EToolContextCoordinateSystem::Local
		: EToolContextCoordinateSystem::World);
}

bool UVTBTransformGizmoComponent::IsGizmoInteracting() const
{
	if (const UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem())
	{
		return ToolsSubsystem->IsGizmoInteracting();
	}

	return false;
}

bool UVTBTransformGizmoComponent::ShouldBlockViewportCameraInput() const
{
	if (const UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem())
	{
		return ToolsSubsystem->ShouldBlockViewportCameraInput();
	}

	return false;
}

AActor* UVTBTransformGizmoComponent::GetSelectedActor() const
{
	if (const UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem())
	{
		return ToolsSubsystem->GetSelectedActor();
	}

	return nullptr;
}

USceneComponent* UVTBTransformGizmoComponent::GetSelectedComponent() const
{
	if (const UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem())
	{
		return ToolsSubsystem->GetSelectedComponent();
	}

	return nullptr;
}

UVTBInteractiveToolsSubsystem* UVTBTransformGizmoComponent::GetSubsystem() const
{
	UWorld* World = GetWorld();
	return World ? World->GetSubsystem<UVTBInteractiveToolsSubsystem>() : nullptr;
}
