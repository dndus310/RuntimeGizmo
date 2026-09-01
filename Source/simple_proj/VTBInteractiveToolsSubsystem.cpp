// Copyright Epic Games, Inc. All Rights Reserved.

#include "VTBInteractiveToolsSubsystem.h"

#include "BaseGizmos/GizmoViewContext.h"
#include "BaseGizmos/TransformGizmoUtil.h"
#include "ContextObjectStore.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/WorldSettings.h"
#include "InputRouter.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolManager.h"
#include "InteractiveToolsContext.h"
#include "Materials/Material.h"
#include "SceneView.h"
#include "VTBTransformGizmoInteraction.h"
#include "VTBTransformSelection.h"

void UVTBInteractiveToolsSubsystem::Deinitialize()
{
	ShutdownToolsContext();
	Super::Deinitialize();
}

void UVTBInteractiveToolsSubsystem::Tick(float DeltaTime)
{
	if (!ToolsContext)
	{
		return;
	}

	UpdateGizmoViewContext();
	RefreshInputStateAndCameraLock();

	if (ToolsContext->ToolManager)
	{
		ToolsContext->ToolManager->Tick(DeltaTime);
	}

	if (ToolsContext->GizmoManager)
	{
		ToolsContext->GizmoManager->Tick(DeltaTime);
	}
}

TStatId UVTBInteractiveToolsSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UVTBRuntimeToolsSubsystem, STATGROUP_Tickables);
}

void UVTBInteractiveToolsSubsystem::InitializeToolsContext(APlayerController* InPlayerController)
{
	ActivePlayerController = InPlayerController;

	if (ToolsContext)
	{
		UpdateGizmoViewContext();
		return;
	}

	ToolsContext = NewObject<UInteractiveToolsContext>(this, TEXT("VTBInteractiveToolsContext"));
	ToolsContext->Initialize(this, this);

	if (ToolsContext->InputRouter)
	{
		ToolsContext->InputRouter->bAutoInvalidateOnCapture = true;
		ToolsContext->InputRouter->bAutoInvalidateOnHover = true;
	}

	UE::TransformGizmoUtil::RegisterTransformGizmoContextObject(ToolsContext);

	TransformInteraction = NewObject<UVTBTransformGizmoInteraction>(this, TEXT("VTBTransformGizmoInteraction"));
	TransformInteraction->Initialize(
		ToolsContext,
		[this](bool bTransformEditInProgress)
		{
			ViewportInput.SetCameraInputBlocked(GetPlayerController(), bTransformEditInProgress || ViewportInput.HasMouseCapture(ToolsContext));
		});

	RegisterSelectionInteraction();
	UpdateGizmoViewContext();
}

void UVTBInteractiveToolsSubsystem::ShutdownToolsContext()
{
	ViewportInput.Reset();
	DeregisterSelectionInteraction();

	if (TransformInteraction)
	{
		TransformInteraction->Shutdown();
		TransformInteraction = nullptr;
	}

	if (ToolsContext)
	{
		UE::TransformGizmoUtil::DeregisterTransformGizmoContextObject(ToolsContext);
		ToolsContext->Shutdown();
		ToolsContext = nullptr;
	}

	ActivePlayerController.Reset();
	bHasOpenTransaction = false;
}

void UVTBInteractiveToolsSubsystem::ShutdownToolsContextForPlayer(APlayerController* InPlayerController)
{
	if (!InPlayerController || ActivePlayerController.Get() == InPlayerController)
	{
		ShutdownToolsContext();
	}
}

void UVTBInteractiveToolsSubsystem::ApplySettings(const FVTBTransformGizmoSettings& InSettings)
{
	Settings = InSettings;
	if (Settings.CoordinateSystem != EToolContextCoordinateSystem::World &&
		Settings.CoordinateSystem != EToolContextCoordinateSystem::Local)
	{
		Settings.CoordinateSystem = EToolContextCoordinateSystem::World;
	}

	if (ToolsContext)
	{
		DeregisterSelectionInteraction();
		RegisterSelectionInteraction();
	}

	SyncActiveGizmoSettings();
}

bool UVTBInteractiveToolsSubsystem::SelectTransformTarget(AActor* TargetActor)
{
	if (!TargetActor)
	{
		ClearSelection();
		return true;
	}

	if (Settings.Selection.bPreventSelectingActiveViewTarget && IsViewportCameraActor(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("VTB runtime tools ignored %s because it is the active viewport camera actor."),
			*GetNameSafe(TargetActor));
		return false;
	}

	if (!ToolsContext)
	{
		InitializeToolsContext(GetPlayerController());
	}

	if (!TransformInteraction)
	{
		UE_LOG(LogTemp, Warning, TEXT("VTB runtime tools could not select %s because TransformInteraction is not initialized."),
			*GetNameSafe(TargetActor));
		return false;
	}

	const bool bSelected = TransformInteraction->SelectTarget(
		TargetActor,
		false,
		MakeActiveGizmoSettings());

	if (!bSelected)
	{
		UE_LOG(LogTemp, Warning, TEXT("VTB runtime tools rejected transform target %s. Check that it has a valid root scene component."),
			*GetNameSafe(TargetActor));
	}

	return bSelected;
}

void UVTBInteractiveToolsSubsystem::ClearSelection()
{
	if (TransformInteraction)
	{
		TransformInteraction->ClearSelection();
	}
}

void UVTBInteractiveToolsSubsystem::SetTransformMode(EToolContextTransformGizmoMode NewMode)
{
	Settings.TransformMode = NewMode;
	SyncActiveGizmoSettings();

	if (ToolsContext && ToolsContext->GizmoManager)
	{
		ToolsContext->GizmoManager->PostInvalidation();
	}
}

void UVTBInteractiveToolsSubsystem::SetTranslationMode()
{
	SetTransformMode(EToolContextTransformGizmoMode::Translation);
	DisplayMessage(NSLOCTEXT("VTBTransformGizmo", "TranslateMode", "VTB gizmo mode: Translate"), EToolMessageLevel::UserNotification);
}

void UVTBInteractiveToolsSubsystem::SetRotationMode()
{
	SetTransformMode(EToolContextTransformGizmoMode::Rotation);
	DisplayMessage(NSLOCTEXT("VTBTransformGizmo", "RotateMode", "VTB gizmo mode: Rotate"), EToolMessageLevel::UserNotification);
}

void UVTBInteractiveToolsSubsystem::SetScaleMode()
{
	SetTransformMode(EToolContextTransformGizmoMode::Scale);
	DisplayMessage(NSLOCTEXT("VTBTransformGizmo", "ScaleMode", "VTB gizmo mode: Scale"), EToolMessageLevel::UserNotification);
}

void UVTBInteractiveToolsSubsystem::SetCoordinateSystem(EToolContextCoordinateSystem NewCoordinateSystem)
{
	if (NewCoordinateSystem != EToolContextCoordinateSystem::World && NewCoordinateSystem != EToolContextCoordinateSystem::Local)
	{
		return;
	}

	Settings.CoordinateSystem = NewCoordinateSystem;
	SyncActiveGizmoSettings();

	const FText ModeText = Settings.CoordinateSystem == EToolContextCoordinateSystem::World
		? NSLOCTEXT("VTBTransformGizmo", "WorldCoordinateSystem", "VTB gizmo coordinate system: World")
		: NSLOCTEXT("VTBTransformGizmo", "LocalCoordinateSystem", "VTB gizmo coordinate system: Local");
	DisplayMessage(ModeText, EToolMessageLevel::UserNotification);

	if (ToolsContext && ToolsContext->GizmoManager)
	{
		ToolsContext->GizmoManager->PostInvalidation();
	}
}

void UVTBInteractiveToolsSubsystem::ToggleCoordinateSystem()
{
	SetCoordinateSystem(Settings.CoordinateSystem == EToolContextCoordinateSystem::World
		? EToolContextCoordinateSystem::Local
		: EToolContextCoordinateSystem::World);
}

bool UVTBInteractiveToolsSubsystem::IsGizmoInteracting() const
{
	return IsTransformEditInProgress() || ViewportInput.HasMouseCapture(ToolsContext);
}

bool UVTBInteractiveToolsSubsystem::ShouldBlockViewportCameraInput() const
{
	return ViewportInput.ShouldBlockCameraInput(ToolsContext, IsTransformEditInProgress());
}

void UVTBInteractiveToolsSubsystem::RefreshInputStateAndCameraLock()
{
	const EVTBViewportToolCommand Command = ViewportInput.Tick(
		ToolsContext,
		GetPlayerController(),
		IsTransformEditInProgress(),
		Settings.Hotkeys.bEnableEditorHotkeys);
	ExecuteViewportToolCommand(Command);
}

AActor* UVTBInteractiveToolsSubsystem::GetSelectedActor() const
{
	return TransformInteraction ? TransformInteraction->GetSelectedActor() : nullptr;
}

USceneComponent* UVTBInteractiveToolsSubsystem::GetSelectedComponent() const
{
	return TransformInteraction ? TransformInteraction->GetSelectedComponent() : nullptr;
}

void UVTBInteractiveToolsSubsystem::RegisterSelectionInteraction()
{
	if (!ToolsContext || !ToolsContext->InputRouter || SelectionInteraction)
	{
		return;
	}

	SelectionInteraction = NewObject<UVTBTransformSelection>(this, TEXT("VTBTransformSelectionInteraction"));
	SelectionInteraction->Initialize(
		GetPlayerController(),
		[this]() { return Settings.Selection.bEnableClickSelection && CanChangeSelectionFromInput(); },
		[this](AActor* TargetActor)
		{
			if (TargetActor)
			{
				SelectTransformTarget(TargetActor);
			}
			else if (Settings.Selection.bClearSelectionOnBackgroundClick)
			{
				ClearSelection();
			}
		},
		Settings.Selection.TraceChannel,
		Settings.Selection.TraceDistance,
		Settings.Selection.bRequireTransformTargetInterface,
		Settings.Selection.bClearSelectionOnBackgroundClick);

	ToolsContext->InputRouter->RegisterSource(SelectionInteraction);
}

void UVTBInteractiveToolsSubsystem::DeregisterSelectionInteraction()
{
	if (ToolsContext && ToolsContext->InputRouter && SelectionInteraction)
	{
		ToolsContext->InputRouter->DeregisterSource(SelectionInteraction);
	}

	if (SelectionInteraction)
	{
		SelectionInteraction->Shutdown();
		SelectionInteraction = nullptr;
	}
}

bool UVTBInteractiveToolsSubsystem::CanChangeSelectionFromInput() const
{
	const bool bToolActive = ToolsContext
		&& ToolsContext->ToolManager
		&& ToolsContext->ToolManager->HasActiveTool(EToolSide::Mouse);

	return !bToolActive && !IsGizmoInteracting();
}

void UVTBInteractiveToolsSubsystem::SyncActiveGizmoSettings()
{
	if (TransformInteraction)
	{
		TransformInteraction->SyncGizmoSettings(MakeActiveGizmoSettings());
	}
}

FVTBTransformGizmoSettings UVTBInteractiveToolsSubsystem::MakeActiveGizmoSettings() const
{
	FVTBTransformGizmoSettings ActiveGizmoSettings = Settings;
	ActiveGizmoSettings.CoordinateSystem = GetEffectiveCoordinateSystem();
	return ActiveGizmoSettings;
}

EToolContextCoordinateSystem UVTBInteractiveToolsSubsystem::GetEffectiveCoordinateSystem() const
{
	// Matches the level editor widget policy: scale is always evaluated in local/component space.
	return Settings.TransformMode == EToolContextTransformGizmoMode::Scale
		? EToolContextCoordinateSystem::Local
		: Settings.CoordinateSystem;
}

void UVTBInteractiveToolsSubsystem::UpdateGizmoViewContext() const
{
	if (!ToolsContext || !ToolsContext->ContextObjectStore)
	{
		return;
	}

	UGizmoViewContext* GizmoViewContext = ToolsContext->ContextObjectStore->FindContext<UGizmoViewContext>();
	if (!GizmoViewContext)
	{
		return;
	}

	APlayerController* PlayerController = GetPlayerController();
	ULocalPlayer* LocalPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr;
	FViewport* Viewport = GetFocusedViewport();
	if (!LocalPlayer || !Viewport)
	{
		return;
	}

	FSceneViewProjectionData ProjectionData;
	if (!LocalPlayer->GetProjectionData(Viewport, ProjectionData))
	{
		return;
	}

	FVector ViewLocation = ProjectionData.ViewOrigin;
	FRotator ViewRotation = FRotator::ZeroRotator;
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FSceneViewInitOptions ViewInitOptions;
	ViewInitOptions.ViewOrigin = ProjectionData.ViewOrigin;
	ViewInitOptions.ViewRotationMatrix = ProjectionData.ViewRotationMatrix;
	ViewInitOptions.ProjectionMatrix = ProjectionData.ProjectionMatrix;
	ViewInitOptions.SetViewRectangle(ProjectionData.GetViewRect());
	ViewInitOptions.SetConstrainedViewRectangle(ProjectionData.GetConstrainedViewRect());
	ViewInitOptions.CameraToViewTarget = ProjectionData.CameraToViewTarget;
	ViewInitOptions.ViewLocation = ViewLocation;
	ViewInitOptions.ViewRotation = ViewRotation;
	ViewInitOptions.FOV = PlayerController->PlayerCameraManager ? PlayerController->PlayerCameraManager->GetFOVAngle() : 90.0f;
	ViewInitOptions.DesiredFOV = ViewInitOptions.FOV;
	ViewInitOptions.WorldToMetersScale = GetWorld() && GetWorld()->GetWorldSettings() ? GetWorld()->GetWorldSettings()->WorldToMeters : 100.0f;

	FSceneView SceneView(ViewInitOptions);
	GizmoViewContext->ResetFromSceneView(SceneView);
	GizmoViewContext->SetDPIScale(1.0);
}

void UVTBInteractiveToolsSubsystem::ExecuteViewportToolCommand(EVTBViewportToolCommand Command)
{
	switch (Command)
	{
	case EVTBViewportToolCommand::Translate:
		SetTranslationMode();
		break;
	case EVTBViewportToolCommand::Rotate:
		SetRotationMode();
		break;
	case EVTBViewportToolCommand::Scale:
		SetScaleMode();
		break;
	case EVTBViewportToolCommand::ToggleCoordinateSystem:
		ToggleCoordinateSystem();
		break;
	default:
		break;
	}
}

APlayerController* UVTBInteractiveToolsSubsystem::GetPlayerController() const
{
	if (ActivePlayerController.IsValid())
	{
		return ActivePlayerController.Get();
	}

	return GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
}

bool UVTBInteractiveToolsSubsystem::IsTransformEditInProgress() const
{
	return TransformInteraction && TransformInteraction->IsTransformEditInProgress();
}

bool UVTBInteractiveToolsSubsystem::IsViewportCameraActor(AActor* Actor) const
{
	if (!Actor)
	{
		return false;
	}

	APlayerController* PlayerController = GetPlayerController();
	if (!PlayerController)
	{
		return false;
	}

	return Actor == PlayerController
		|| Actor == PlayerController->GetPawn()
		|| Actor == PlayerController->GetViewTarget()
		|| Actor == PlayerController->PlayerCameraManager;
}

UWorld* UVTBInteractiveToolsSubsystem::GetCurrentEditingWorld() const
{
	return GetWorld();
}

void UVTBInteractiveToolsSubsystem::GetCurrentSelectionState(FToolBuilderState& StateOut) const
{
	StateOut.World = GetWorld();
	StateOut.ToolManager = ToolsContext ? ToolsContext->ToolManager : nullptr;
	StateOut.TargetManager = ToolsContext ? ToolsContext->TargetManager : nullptr;
	StateOut.GizmoManager = ToolsContext ? ToolsContext->GizmoManager : nullptr;

	if (AActor* SelectedActor = GetSelectedActor())
	{
		StateOut.SelectedActors.Add(SelectedActor);
	}

	if (USceneComponent* SelectedComponent = GetSelectedComponent())
	{
		StateOut.SelectedComponents.Add(SelectedComponent);
	}
}

void UVTBInteractiveToolsSubsystem::GetCurrentViewState(FViewCameraState& StateOut) const
{
	APlayerController* PlayerController = GetPlayerController();
	if (!PlayerController)
	{
		return;
	}

	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	int32 ViewportX = 1;
	int32 ViewportY = 1;
	PlayerController->GetViewportSize(ViewportX, ViewportY);

	StateOut.Position = ViewLocation;
	StateOut.Orientation = ViewRotation.Quaternion();
	StateOut.HorizontalFOVDegrees = PlayerController->PlayerCameraManager ? PlayerController->PlayerCameraManager->GetFOVAngle() : 90.0f;
	StateOut.AspectRatio = ViewportY > 0 ? static_cast<float>(ViewportX) / static_cast<float>(ViewportY) : 1.0f;
	StateOut.bIsOrthographic = false;
	StateOut.bIsVR = false;
	StateOut.DPIScale = 1.0;
}

EToolContextCoordinateSystem UVTBInteractiveToolsSubsystem::GetCurrentCoordinateSystem() const
{
	return GetEffectiveCoordinateSystem();
}

FToolContextSnappingConfiguration UVTBInteractiveToolsSubsystem::GetCurrentSnappingSettings() const
{
	FToolContextSnappingConfiguration SnappingConfiguration;
	SnappingConfiguration.bEnablePositionGridSnapping = Settings.Snapping.bEnablePositionSnapping;
	SnappingConfiguration.PositionGridDimensions = Settings.Snapping.PositionGrid;
	SnappingConfiguration.bEnableRotationGridSnapping = Settings.Snapping.bEnableRotationSnapping;
	SnappingConfiguration.RotationGridAngles = Settings.Snapping.RotationGrid;
	SnappingConfiguration.bEnableScaleGridSnapping = Settings.Snapping.bEnableScaleSnapping;
	SnappingConfiguration.ScaleGridSize = Settings.Snapping.ScaleGrid;
	SnappingConfiguration.bEnableAbsoluteWorldSnapping = false;
	return SnappingConfiguration;
}

UMaterialInterface* UVTBInteractiveToolsSubsystem::GetStandardMaterial(EStandardToolContextMaterials MaterialType) const
{
	return UMaterial::GetDefaultMaterial(MD_Surface);
}

FViewport* UVTBInteractiveToolsSubsystem::GetHoveredViewport() const
{
	return GetFocusedViewport();
}

FViewport* UVTBInteractiveToolsSubsystem::GetFocusedViewport() const
{
	const APlayerController* PlayerController = GetPlayerController();
	const ULocalPlayer* LocalPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr;
	return LocalPlayer && LocalPlayer->ViewportClient ? LocalPlayer->ViewportClient->Viewport : nullptr;
}

void UVTBInteractiveToolsSubsystem::DisplayMessage(const FText& Message, EToolMessageLevel Level)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *Message.ToString());

	if (GEngine && Level >= EToolMessageLevel::UserNotification)
	{
		GEngine->AddOnScreenDebugMessage(reinterpret_cast<uint64>(this), 1.25f, FColor::Cyan, Message.ToString());
	}
}

void UVTBInteractiveToolsSubsystem::PostInvalidation()
{
	if (FViewport* Viewport = GetFocusedViewport())
	{
		Viewport->Invalidate();
	}
}

void UVTBInteractiveToolsSubsystem::BeginUndoTransaction(const FText& Description)
{
	bHasOpenTransaction = true;
	UE_LOG(LogTemp, Verbose, TEXT("VTB gizmo transaction begin: %s"), *Description.ToString());
}

void UVTBInteractiveToolsSubsystem::EndUndoTransaction()
{
	bHasOpenTransaction = false;
	UE_LOG(LogTemp, Verbose, TEXT("VTB gizmo transaction end"));
}

void UVTBInteractiveToolsSubsystem::AppendChange(UObject* TargetObject, TUniquePtr<FToolCommandChange> Change, const FText& Description)
{
	UE_LOG(LogTemp, Verbose, TEXT("VTB gizmo change: %s"), *Description.ToString());
}

bool UVTBInteractiveToolsSubsystem::RequestSelectionChange(const FSelectedObjectsChangeList& SelectionChange)
{
	if (SelectionChange.ModificationType == ESelectedObjectsModificationType::Clear)
	{
		ClearSelection();
		return true;
	}

	if ((SelectionChange.ModificationType == ESelectedObjectsModificationType::Replace ||
		SelectionChange.ModificationType == ESelectedObjectsModificationType::Add) &&
		SelectionChange.Actors.Num() > 0)
	{
		return SelectTransformTarget(SelectionChange.Actors[0]);
	}

	return false;
}
