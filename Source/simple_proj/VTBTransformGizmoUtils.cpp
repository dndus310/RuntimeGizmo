// Copyright Epic Games, Inc. All Rights Reserved.

#include "VTBTransformGizmoUtils.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "VTBInteractiveToolsSubsystem.h"

bool UVTBTransformGizmoUtils::SelectTransformTarget(UObject* WorldContextObject, AActor* TargetActor)
{
	UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem(WorldContextObject);
	if (!ToolsSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("VTB runtime tools subsystem is unavailable for SelectTransformTarget."));
		return false;
	}

	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull) : nullptr;
	ToolsSubsystem->InitializeToolsContext(World ? World->GetFirstPlayerController() : nullptr);
	return ToolsSubsystem->SelectTransformTarget(TargetActor);
}

void UVTBTransformGizmoUtils::ClearSelection(UObject* WorldContextObject)
{
	if (UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem(WorldContextObject))
	{
		ToolsSubsystem->ClearSelection();
	}
}

bool UVTBTransformGizmoUtils::ShouldBlockViewportCameraInput(UObject* WorldContextObject)
{
	if (const UVTBInteractiveToolsSubsystem* ToolsSubsystem = GetSubsystem(WorldContextObject))
	{
		return ToolsSubsystem->ShouldBlockViewportCameraInput();
	}

	return false;
}

UVTBInteractiveToolsSubsystem* UVTBTransformGizmoUtils::GetSubsystem(UObject* WorldContextObject)
{
	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull) : nullptr;
	return World ? World->GetSubsystem<UVTBInteractiveToolsSubsystem>() : nullptr;
}
