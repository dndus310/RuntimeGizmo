// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeTransformGizmoComponent.h"

#include "BaseGizmos/CombinedTransformGizmo.h"
#include "BaseGizmos/GizmoViewContext.h"
#include "BaseGizmos/TransformGizmoUtil.h"
#include "BaseGizmos/TransformProxy.h"
#include "BaseGizmos/ViewAdjustedStaticMeshGizmoComponent.h"
#include "Components/PrimitiveComponent.h"
#include "ContextObjectStore.h"
#include "DrawDebugHelpers.h"
#include "Engine/GameViewportClient.h"
#include "Engine/EngineTypes.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/WorldSettings.h"
#include "InputRouter.h"
#include "InputState.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolManager.h"
#include "InteractiveToolsContext.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialParameters.h"
#include "RuntimeTransformTarget.h"
#include "RuntimeTransformableSphereActor.h"
#include "SceneView.h"

namespace
{
	const FName RuntimeAlwaysOnTopGizmoTag(TEXT("RuntimeAlwaysOnTopGizmoMaterial"));
	const FName GizmoColorParameterName(TEXT("GizmoColor"));
	const TCHAR* AlwaysOnTopGizmoMaterialPath = TEXT("/Engine/InteractiveToolsFramework/Materials/GizmoComponentMaterial_NotDimmed");
}

URuntimeTransformGizmoComponent::URuntimeTransformGizmoComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void URuntimeTransformGizmoComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeToolsContext();
}

void URuntimeTransformGizmoComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ShutdownToolsContext();
	Super::EndPlay(EndPlayReason);
}

void URuntimeTransformGizmoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!ToolsContext)
	{
		return;
	}

	UpdateGizmoViewContext();

	FInputDeviceState InputState;
	if (BuildMouseInputState(InputState) && ToolsContext->InputRouter)
	{
		ToolsContext->InputRouter->PostHoverInputEvent(InputState);
		ToolsContext->InputRouter->PostInputEvent(InputState);
	}

	if (ToolsContext->ToolManager)
	{
		ToolsContext->ToolManager->Tick(DeltaTime);
	}

	if (ToolsContext->GizmoManager)
	{
		ToolsContext->GizmoManager->Tick(DeltaTime);
	}

}

void URuntimeTransformGizmoComponent::InitializeToolsContext()
{
	if (ToolsContext)
	{
		return;
	}

	ToolsContext = NewObject<UInteractiveToolsContext>(this, TEXT("RuntimeInteractiveToolsContext"));
	ToolsContext->Initialize(this, this);

	if (ToolsContext->InputRouter)
	{
		ToolsContext->InputRouter->bAutoInvalidateOnCapture = true;
		ToolsContext->InputRouter->bAutoInvalidateOnHover = true;
	}

	UE::TransformGizmoUtil::RegisterTransformGizmoContextObject(ToolsContext);
	UpdateGizmoViewContext();
}

void URuntimeTransformGizmoComponent::ShutdownToolsContext()
{
	ClearSelection();

	if (ToolsContext)
	{
		UE::TransformGizmoUtil::DeregisterTransformGizmoContextObject(ToolsContext);
		ToolsContext->Shutdown();
		ToolsContext = nullptr;
	}
}

void URuntimeTransformGizmoComponent::SelectActor(AActor* Actor)
{
	if (!Actor || Actor == SelectedActor.Get())
	{
		return;
	}

	if (!ToolsContext)
	{
		InitializeToolsContext();
	}

	USceneComponent* TransformComponent = ResolveTransformComponent(Actor);
	if (!TransformComponent)
	{
		return;
	}

	ClearSelectedOutline();
	SetSelectedActorVisual(SelectedActor.Get(), false);

	if (ActiveGizmo && ActiveGizmo->ActiveTarget)
	{
		ActiveGizmo->ClearActiveTarget();
	}

	SelectedActor = Actor;
	SelectedComponent = TransformComponent;

	if (UPrimitiveComponent* SelectedPrimitive = Cast<UPrimitiveComponent>(SelectedComponent.Get()))
	{
		ApplySelectedOutline(SelectedPrimitive);
	}

	SetSelectedActorVisual(SelectedActor.Get(), true);

	ActiveProxy = NewObject<UTransformProxy>(this, TEXT("RuntimeTransformProxy"));
	ActiveProxy->AddComponent(SelectedComponent.Get(), false);
	ActiveProxy->OnTransformChanged.AddUObject(this, &URuntimeTransformGizmoComponent::HandleProxyTransformChanged);

	EnsureTransformGizmo();
	if (ActiveGizmo)
	{
		SyncActiveGizmoSettings();
		ActiveGizmo->SetActiveTarget(ActiveProxy.Get(), ToolsContext->GizmoManager);
		ActiveGizmo->SetVisibility(true);
		ConfigureGizmoRenderPriority();
	}
}

void URuntimeTransformGizmoComponent::ClearSelection()
{
	if (ActiveGizmo && ActiveGizmo->ActiveTarget)
	{
		ActiveGizmo->ClearActiveTarget();
		ActiveGizmo->SetVisibility(false);
	}

	ClearSelectedOutline();
	SetSelectedActorVisual(SelectedActor.Get(), false);
	SelectedActor = nullptr;
	SelectedComponent = nullptr;
	ActiveProxy = nullptr;
}

void URuntimeTransformGizmoComponent::SetTransformMode(EToolContextTransformGizmoMode NewMode)
{
	CurrentGizmoMode = NewMode;
	SyncActiveGizmoSettings();

	if (ToolsContext && ToolsContext->GizmoManager)
	{
		ToolsContext->GizmoManager->PostInvalidation();
	}
}

void URuntimeTransformGizmoComponent::SetTranslationMode()
{
	SetTransformMode(EToolContextTransformGizmoMode::Translation);
	DisplayMessage(NSLOCTEXT("RuntimeTransformGizmo", "TranslateMode", "Runtime gizmo mode: Translate"), EToolMessageLevel::UserNotification);
}

void URuntimeTransformGizmoComponent::SetRotationMode()
{
	SetTransformMode(EToolContextTransformGizmoMode::Rotation);
	DisplayMessage(NSLOCTEXT("RuntimeTransformGizmo", "RotateMode", "Runtime gizmo mode: Rotate"), EToolMessageLevel::UserNotification);
}

void URuntimeTransformGizmoComponent::SetScaleMode()
{
	SetTransformMode(EToolContextTransformGizmoMode::Scale);
	DisplayMessage(NSLOCTEXT("RuntimeTransformGizmo", "ScaleMode", "Runtime gizmo mode: Scale"), EToolMessageLevel::UserNotification);
}

void URuntimeTransformGizmoComponent::SetCoordinateSystem(EToolContextCoordinateSystem NewCoordinateSystem)
{
	if (NewCoordinateSystem != EToolContextCoordinateSystem::World && NewCoordinateSystem != EToolContextCoordinateSystem::Local)
	{
		return;
	}

	CoordinateSystem = NewCoordinateSystem;
	SyncActiveGizmoSettings();

	const FText ModeText = CoordinateSystem == EToolContextCoordinateSystem::World
		? NSLOCTEXT("RuntimeTransformGizmo", "WorldCoordinateSystem", "Runtime gizmo coordinate system: World")
		: NSLOCTEXT("RuntimeTransformGizmo", "LocalCoordinateSystem", "Runtime gizmo coordinate system: Local");
	DisplayMessage(ModeText, EToolMessageLevel::UserNotification);

	if (ToolsContext && ToolsContext->GizmoManager)
	{
		ToolsContext->GizmoManager->PostInvalidation();
	}
}

void URuntimeTransformGizmoComponent::ToggleCoordinateSystem()
{
	SetCoordinateSystem(CoordinateSystem == EToolContextCoordinateSystem::World
		? EToolContextCoordinateSystem::Local
		: EToolContextCoordinateSystem::World);
}

bool URuntimeTransformGizmoComponent::IsGizmoInteracting() const
{
	return ToolsContext && ToolsContext->InputRouter && ToolsContext->InputRouter->HasActiveMouseCapture();
}

void URuntimeTransformGizmoComponent::EnsureTransformGizmo()
{
	if (ActiveGizmo || !ToolsContext || !ToolsContext->GizmoManager)
	{
		return;
	}

	UpdateGizmoViewContext();

	ActiveGizmo = UE::TransformGizmoUtil::Create3AxisTransformGizmo(
		ToolsContext->GizmoManager,
		this,
		TEXT("RuntimeTransformGizmo"));

	if (ActiveGizmo)
	{
		ActiveGizmo->bUseContextGizmoMode = true;
		ActiveGizmo->bUseContextCoordinateSystem = true;
		ActiveGizmo->bSnapToWorldGrid = true;
		ActiveGizmo->bSnapToWorldRotGrid = true;
		ActiveGizmo->bSnapToScaleGrid = true;
		ActiveGizmo->SetDisallowNegativeScaling(true);
		ActiveGizmo->SetIsNonUniformScaleAllowedFunction([]() { return true; });
		ActiveGizmo->SetVisibility(false);
		SyncActiveGizmoSettings();
		ConfigureGizmoRenderPriority();
	}
}

void URuntimeTransformGizmoComponent::SyncActiveGizmoSettings()
{
	if (!ActiveGizmo)
	{
		return;
	}

	ActiveGizmo->CurrentCoordinateSystem = CoordinateSystem;
	ActiveGizmo->ActiveGizmoMode = CurrentGizmoMode;
	ActiveGizmo->Tick(0.0f);
	ConfigureGizmoRenderPriority();
}

void URuntimeTransformGizmoComponent::ConfigureGizmoRenderPriority() const
{
	if (!IsValid(ActiveGizmo) || !IsValid(ActiveGizmo->GetGizmoActor()))
	{
		return;
	}

	TArray<UPrimitiveComponent*> GizmoPrimitives;
	ActiveGizmo->GetGizmoActor()->GetComponents<UPrimitiveComponent>(GizmoPrimitives, true);

	for (UPrimitiveComponent* Primitive : GizmoPrimitives)
	{
		if (!IsValid(Primitive))
		{
			continue;
		}

		bool bRenderStateChanged = false;
		if (Primitive->DepthPriorityGroup != SDPG_Foreground)
		{
			Primitive->DepthPriorityGroup = SDPG_Foreground;
			bRenderStateChanged = true;
		}

		if (!Primitive->bUseEditorCompositing)
		{
			Primitive->bUseEditorCompositing = true;
			bRenderStateChanged = true;
		}

		if (!Primitive->bRenderCustomDepth)
		{
			Primitive->bRenderCustomDepth = true;
			bRenderStateChanged = true;
		}

		if (Primitive->TranslucencySortPriority < GizmoTranslucentSortPriority)
		{
			Primitive->TranslucencySortPriority = GizmoTranslucentSortPriority;
			bRenderStateChanged = true;
		}

		ApplyAlwaysOnTopGizmoMaterial(Primitive);

		if (bRenderStateChanged && Primitive->IsRegistered())
		{
			Primitive->MarkRenderStateDirty();
		}
	}
}

void URuntimeTransformGizmoComponent::ApplyAlwaysOnTopGizmoMaterial(UPrimitiveComponent* Primitive) const
{
	if (!Primitive || Primitive->ComponentHasTag(RuntimeAlwaysOnTopGizmoTag))
	{
		return;
	}

	UMaterialInterface* AlwaysOnTopBaseMaterial = LoadObject<UMaterialInterface>(nullptr, AlwaysOnTopGizmoMaterialPath);
	if (!AlwaysOnTopBaseMaterial)
	{
		return;
	}

	const FLinearColor GizmoColor = ResolveGizmoColor(Primitive);
	UMaterialInstanceDynamic* AlwaysOnTopMaterial = UMaterialInstanceDynamic::Create(AlwaysOnTopBaseMaterial, Primitive);
	if (!AlwaysOnTopMaterial)
	{
		return;
	}

	AlwaysOnTopMaterial->SetVectorParameterValue(GizmoColorParameterName, GizmoColor);

	if (UViewAdjustedStaticMeshGizmoComponent* MeshGizmoComponent = Cast<UViewAdjustedStaticMeshGizmoComponent>(Primitive))
	{
		MeshGizmoComponent->SetAllMaterials(AlwaysOnTopMaterial);

		if (UMaterialInstanceDynamic* HoverMaterial = UMaterialInstanceDynamic::Create(AlwaysOnTopBaseMaterial, Primitive))
		{
			HoverMaterial->SetVectorParameterValue(GizmoColorParameterName, FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
			MeshGizmoComponent->SetHoverOverrideMaterial(HoverMaterial);
		}
	}
	else
	{
		for (int32 MaterialIndex = 0; MaterialIndex < Primitive->GetNumMaterials(); ++MaterialIndex)
		{
			Primitive->SetMaterial(MaterialIndex, AlwaysOnTopMaterial);
		}
	}

	Primitive->ComponentTags.Add(RuntimeAlwaysOnTopGizmoTag);
}

FLinearColor URuntimeTransformGizmoComponent::ResolveGizmoColor(UPrimitiveComponent* Primitive) const
{
	if (Primitive)
	{
		if (UMaterialInterface* ExistingMaterial = Primitive->GetMaterial(0))
		{
			FLinearColor ExistingColor = FLinearColor::White;
			if (ExistingMaterial->GetVectorParameterValue(FMaterialParameterInfo(GizmoColorParameterName), ExistingColor))
			{
				return ExistingColor;
			}
		}
	}

	return FLinearColor::White;
}

void URuntimeTransformGizmoComponent::UpdateGizmoViewContext() const
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

	APlayerController* PlayerController = GetOwningPlayerController();
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

bool URuntimeTransformGizmoComponent::BuildMouseInputState(FInputDeviceState& InputState)
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController)
	{
		return false;
	}

	if (PlayerController->IsInputKeyDown(EKeys::RightMouseButton))
	{
		bHasLastMousePosition = false;
		bWasLeftMouseDown = false;
		bWasMiddleMouseDown = false;
		bWasRightMouseDown = false;
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
	InputState.Mouse.Delta2D = bHasLastMousePosition ? MousePosition - LastMousePosition : FVector2D::ZeroVector;
	InputState.Mouse.WorldRay = FRay(RayOrigin, RayDirection.GetSafeNormal(), true);
	InputState.Mouse.Left.SetStates(!bWasLeftMouseDown && bLeftMouseDown, bLeftMouseDown, bWasLeftMouseDown && !bLeftMouseDown);
	InputState.Mouse.Middle.SetStates(!bWasMiddleMouseDown && bMiddleMouseDown, bMiddleMouseDown, bWasMiddleMouseDown && !bMiddleMouseDown);
	InputState.Mouse.Right.SetStates(!bWasRightMouseDown && bRightMouseDown, bRightMouseDown, bWasRightMouseDown && !bRightMouseDown);
	InputState.SetModifierKeyStates(
		PlayerController->IsInputKeyDown(EKeys::LeftShift) || PlayerController->IsInputKeyDown(EKeys::RightShift),
		PlayerController->IsInputKeyDown(EKeys::LeftAlt) || PlayerController->IsInputKeyDown(EKeys::RightAlt),
		PlayerController->IsInputKeyDown(EKeys::LeftControl) || PlayerController->IsInputKeyDown(EKeys::RightControl),
		PlayerController->IsInputKeyDown(EKeys::LeftCommand) || PlayerController->IsInputKeyDown(EKeys::RightCommand));

	LastMousePosition = MousePosition;
	bHasLastMousePosition = true;
	bWasLeftMouseDown = bLeftMouseDown;
	bWasMiddleMouseDown = bMiddleMouseDown;
	bWasRightMouseDown = bRightMouseDown;

	return true;
}

USceneComponent* URuntimeTransformGizmoComponent::ResolveTransformComponent(AActor* Actor) const
{
	if (!Actor)
	{
		return nullptr;
	}

	if (Actor->GetClass()->ImplementsInterface(URuntimeTransformTarget::StaticClass()))
	{
		if (USceneComponent* InterfaceComponent = IRuntimeTransformTarget::Execute_GetRuntimeTransformComponent(Actor))
		{
			return InterfaceComponent;
		}
	}

	return Actor->GetRootComponent();
}

APlayerController* URuntimeTransformGizmoComponent::GetOwningPlayerController() const
{
	return Cast<APlayerController>(GetOwner());
}

void URuntimeTransformGizmoComponent::SetSelectedActorVisual(AActor* Actor, bool bSelected) const
{
	if (ARuntimeTransformableSphereActor* RuntimeSphere = Cast<ARuntimeTransformableSphereActor>(Actor))
	{
		RuntimeSphere->SetSelectedByRuntimeGizmo(bSelected);
	}
}

void URuntimeTransformGizmoComponent::ApplySelectedOutline(UPrimitiveComponent* PrimitiveComponent)
{
	ClearSelectedOutline();

	if (!PrimitiveComponent)
	{
		return;
	}

	SelectedOutlineComponent = PrimitiveComponent;
	bPreviousRenderCustomDepth = PrimitiveComponent->bRenderCustomDepth;
	PreviousCustomDepthStencilValue = PrimitiveComponent->CustomDepthStencilValue;

	PrimitiveComponent->SetRenderCustomDepth(true);
	PrimitiveComponent->SetCustomDepthStencilValue(SelectedOutlineStencilValue);
}

void URuntimeTransformGizmoComponent::ClearSelectedOutline()
{
	if (UPrimitiveComponent* PrimitiveComponent = SelectedOutlineComponent.Get())
	{
		PrimitiveComponent->SetRenderCustomDepth(bPreviousRenderCustomDepth);
		PrimitiveComponent->SetCustomDepthStencilValue(PreviousCustomDepthStencilValue);
	}

	SelectedOutlineComponent.Reset();
	bPreviousRenderCustomDepth = false;
	PreviousCustomDepthStencilValue = 0;
}

void URuntimeTransformGizmoComponent::HandleProxyTransformChanged(UTransformProxy* Proxy, FTransform NewTransform)
{
	if (!SelectedActor)
	{
		return;
	}

	const FVector Location = SelectedActor->GetActorLocation();
	const FRotator Rotation = SelectedActor->GetActorRotation();
	const FVector Scale = SelectedActor->GetActorScale3D();
	UE_LOG(LogTemp, Verbose, TEXT("Runtime gizmo changed %s | Location=%s Rotation=%s Scale=%s"),
		*SelectedActor->GetName(),
		*Location.ToCompactString(),
		*Rotation.ToCompactString(),
		*Scale.ToCompactString());
}

UWorld* URuntimeTransformGizmoComponent::GetCurrentEditingWorld() const
{
	return GetWorld();
}

void URuntimeTransformGizmoComponent::GetCurrentSelectionState(FToolBuilderState& StateOut) const
{
	StateOut.World = GetWorld();
	StateOut.ToolManager = ToolsContext ? ToolsContext->ToolManager : nullptr;
	StateOut.TargetManager = ToolsContext ? ToolsContext->TargetManager : nullptr;
	StateOut.GizmoManager = ToolsContext ? ToolsContext->GizmoManager : nullptr;

	if (SelectedActor)
	{
		StateOut.SelectedActors.Add(SelectedActor.Get());
	}

	if (SelectedComponent)
	{
		StateOut.SelectedComponents.Add(SelectedComponent.Get());
	}
}

void URuntimeTransformGizmoComponent::GetCurrentViewState(FViewCameraState& StateOut) const
{
	APlayerController* PlayerController = GetOwningPlayerController();
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

FToolContextSnappingConfiguration URuntimeTransformGizmoComponent::GetCurrentSnappingSettings() const
{
	FToolContextSnappingConfiguration SnappingConfiguration;
	SnappingConfiguration.bEnablePositionGridSnapping = bEnablePositionSnapping;
	SnappingConfiguration.PositionGridDimensions = PositionSnapGrid;
	SnappingConfiguration.bEnableRotationGridSnapping = bEnableRotationSnapping;
	SnappingConfiguration.RotationGridAngles = RotationSnapGrid;
	SnappingConfiguration.bEnableScaleGridSnapping = bEnableScaleSnapping;
	SnappingConfiguration.ScaleGridSize = ScaleSnapGrid;
	SnappingConfiguration.bEnableAbsoluteWorldSnapping = false;
	return SnappingConfiguration;
}

UMaterialInterface* URuntimeTransformGizmoComponent::GetStandardMaterial(EStandardToolContextMaterials MaterialType) const
{
	return UMaterial::GetDefaultMaterial(MD_Surface);
}

FViewport* URuntimeTransformGizmoComponent::GetHoveredViewport() const
{
	return GetFocusedViewport();
}

FViewport* URuntimeTransformGizmoComponent::GetFocusedViewport() const
{
	const APlayerController* PlayerController = GetOwningPlayerController();
	const ULocalPlayer* LocalPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr;
	return LocalPlayer && LocalPlayer->ViewportClient ? LocalPlayer->ViewportClient->Viewport : nullptr;
}

void URuntimeTransformGizmoComponent::DisplayMessage(const FText& Message, EToolMessageLevel Level)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *Message.ToString());

	if (GEngine && Level >= EToolMessageLevel::UserNotification)
	{
		GEngine->AddOnScreenDebugMessage(reinterpret_cast<uint64>(this), 1.25f, FColor::Cyan, Message.ToString());
	}
}

void URuntimeTransformGizmoComponent::PostInvalidation()
{
	if (FViewport* Viewport = GetFocusedViewport())
	{
		Viewport->Invalidate();
	}
}

void URuntimeTransformGizmoComponent::BeginUndoTransaction(const FText& Description)
{
	bHasOpenTransaction = true;
	UE_LOG(LogTemp, Verbose, TEXT("Runtime gizmo transaction begin: %s"), *Description.ToString());
}

void URuntimeTransformGizmoComponent::EndUndoTransaction()
{
	bHasOpenTransaction = false;
	UE_LOG(LogTemp, Verbose, TEXT("Runtime gizmo transaction end"));
}

void URuntimeTransformGizmoComponent::AppendChange(UObject* TargetObject, TUniquePtr<FToolCommandChange> Change, const FText& Description)
{
	UE_LOG(LogTemp, Verbose, TEXT("Runtime gizmo change: %s"), *Description.ToString());
}

bool URuntimeTransformGizmoComponent::RequestSelectionChange(const FSelectedObjectsChangeList& SelectionChange)
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
		SelectActor(SelectionChange.Actors[0]);
		return true;
	}

	return false;
}
