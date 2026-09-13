#include "VTBOWTEditorSubsystem.h"
#include "Context/OWTEditContexts.h"
#include "Gizmos/Base/VTBOWTTransformGizmoBehavior.h"

#include "Gizmos/Base/VTBOWTBaseTransformGizmo.h"
#include "Gizmos/Custom/VTBOWTCustomTransformGizmo.h"
#include "BaseGizmos/GizmoViewContext.h"
#include "BaseGizmos/TransformProxy.h"

#include "Components/SceneComponent.h"
#include "Context/VTBOWTEditorToolsContext.h"
#include "ContextObjectStore.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "InputRouter.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolManager.h"

#include "Modes/OWTObjectEditMode.h"
#include "SceneView.h"

UVTBOWTEditorSubsystem::UVTBOWTEditorSubsystem()
    : ActiveEditMode(nullptr), SelectedObject(), ToolsContext(nullptr), TransformGizmo(nullptr), GizmoTarget(),
      SystemContextHandlers(), CoordinateSystem(EToolContextCoordinateSystem::World),
      TransformGizmoMode(EToolContextTransformGizmoMode::Translation), bEditingEnabled(false)
{
}

void UVTBOWTEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ActiveEditMode = NewObject<UOWTObjectEditMode>(this);
	SystemContextHandlers.Add(FOWTToggleEditingContext::StaticStruct(),
	                          [this](const FInstancedStruct&)
	                          {
		                          ToggleEditing();
		                          return true;
	                          });
	SystemContextHandlers.Add(FOWTGizmoPointerContext::StaticStruct(),
	                          [this](const FInstancedStruct& Context)
	                          {
		                          return bEditingEnabled && RouteGizmoPointer(Context.Get<FOWTGizmoPointerContext>());
	                          });
	InitializeToolsContext();
}

void UVTBOWTEditorSubsystem::Deinitialize()
{
	ShutdownToolsContext();
	SystemContextHandlers.Empty();
	ActiveEditMode = nullptr;
	SelectedObject.Reset();
	Super::Deinitialize();
}

void UVTBOWTEditorSubsystem::OnWorldEndPlay(UWorld& InWorld)
{
	ShutdownToolsContext();
	Super::OnWorldEndPlay(InWorld);
}

void UVTBOWTEditorSubsystem::Tick(float DeltaTime)
{
	if (!ToolsContext)
	{
		return;
	}
	if (TransformGizmo && !GizmoTarget.IsValid())
	{
		HideSelectionGizmo();
	}

	UpdateGizmoView();
	ToolsContext->ToolManager->Tick(DeltaTime);
	ToolsContext->GizmoManager->Tick(DeltaTime);
}

bool UVTBOWTEditorSubsystem::ReceiveEditContext_Implementation(const FInstancedStruct& Context)
{
	check(IsInGameThread());
	if (!Context.IsValid() || !IsValid(ActiveEditMode))
	{
		return false;
	}

	if (const auto* Handler = SystemContextHandlers.Find(Context.GetScriptStruct()))
	{
		return (*Handler)(Context);
	}
	if (!bEditingEnabled)
	{
		return false;
	}
	return IOWTEditContextReceiver::Execute_ReceiveEditContext(ActiveEditMode, Context);
}

TStatId UVTBOWTEditorSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UVTBOWTEditorSubsystem, STATGROUP_Tickables);
}

bool UVTBOWTEditorSubsystem::SetActiveEditMode(UObject* Mode)
{
	check(IsInGameThread());
	if (!IsValid(Mode))
	{
		return false;
	}
	if (!ensureMsgf(Mode != this && Mode->GetClass()->ImplementsInterface(UOWTEditContextReceiver::StaticClass()),
	                TEXT("OWT active mode must be a separate edit-context receiver.")))
	{
		return false;
	}

	HideSelectionGizmo();
	ActiveEditMode = Mode;
	return true;
}

bool UVTBOWTEditorSubsystem::InitializeToolsContext()
{
	check(IsInGameThread());
	if (ToolsContext)
	{
		return true;
	}
	UWorld* World = GetWorld();
	if (!World || World->bIsTearingDown)
	{
		return false;
	}

	ToolsContext = NewObject<UVTBOWTEditorToolsContext>(this);
	ToolsContext->InitializeContext(*this);
	ToolsContext->GizmoManager->RegisterGizmoType(TEXT("OWT.AxisPosition"),
	                                              NewObject<UVTBOWTAxisPositionGizmoBuilder>(ToolsContext));
	ToolsContext->GizmoManager->RegisterGizmoType(TEXT("OWT.AxisAngle"),
	                                              NewObject<UVTBOWTAxisAngleGizmoBuilder>(ToolsContext));
	ToolsContext->GizmoManager->RegisterGizmoType(TEXT("OWT.Transform"),
	                                              NewObject<UVTBOWTBaseTransformGizmoBuilder>(ToolsContext));
	ToolsContext->GizmoManager->RegisterGizmoType(TEXT("OWT.CustomTransform"),
	                                              NewObject<UVTBOWTCustomTransformGizmoBuilder>(ToolsContext));
	return true;
}

bool UVTBOWTEditorSubsystem::SetGizmoSnapSettings(const FOWTGizmoSnapSettings& Settings)
{
	return ToolsContext && ToolsContext->SetSnapSettings(Settings);
}

FOWTGizmoSnapSettings UVTBOWTEditorSubsystem::GetGizmoSnapSettings() const
{
	return ToolsContext ? ToolsContext->GetSnapSettings() : FOWTGizmoSnapSettings();
}

void UVTBOWTEditorSubsystem::ShowSelectionGizmo()
{
	check(IsInGameThread());
	AActor* Actor = SelectedObject.Get();
	USceneComponent* Root = Actor ? Actor->GetRootComponent() : nullptr;
	if (!bEditingEnabled || !ToolsContext || GetWorld()->bIsTearingDown || !IsValid(Root) ||
	    Root->Mobility != EComponentMobility::Movable)
	{
		return;
	}
	if (TransformGizmo && GizmoTarget.Get() == Root)
	{
		return;
	}

	HideSelectionGizmo();
	TransformGizmo = Cast<UVTBOWTBaseTransformGizmo>(
	    ToolsContext->GizmoManager->CreateGizmo(TEXT("OWT.Transform"), FString(), this));
	if (!ensureMsgf(TransformGizmo, TEXT("OWT transform gizmo creation failed.")))
	{
		return;
	}

	TransformGizmo->SetTargetComponent(*Root);
	TransformGizmo->Tick(0.f);
	GizmoTarget = Root;
}

void UVTBOWTEditorSubsystem::ShutdownToolsContext()
{
	check(IsInGameThread());
	if (!ToolsContext)
	{
		return;
	}

	ToolsContext->InputRouter->ForceTerminateAll();
	HideSelectionGizmo();
	ToolsContext->Shutdown();
	ToolsContext = nullptr;
}

void UVTBOWTEditorSubsystem::HideSelectionGizmo()
{
	check(IsInGameThread());
	if (!TransformGizmo)
	{
		return;
	}

	// DestroyGizmo terminates capture, clears the active target and destroys its actor/sub-gizmos.
	ToolsContext->GizmoManager->DestroyAllGizmosByOwner(this);
	TransformGizmo = nullptr;
	GizmoTarget.Reset();
}

void UVTBOWTEditorSubsystem::SetSelectedObject(AActor* Actor)
{
	check(IsInGameThread());
	if (!ensureMsgf(!Actor || (IsValid(Actor) && Actor->GetWorld() == GetWorld()),
	                TEXT("OWT selection must be a live actor in this world.")))
	{
		return;
	}

	HideSelectionGizmo();
	SelectedObject = Actor;
	ShowSelectionGizmo();
}

void UVTBOWTEditorSubsystem::SetCoordinateSystem(EToolContextCoordinateSystem System)
{
	CoordinateSystem = System;
}

void UVTBOWTEditorSubsystem::SetTransformGizmoMode(EToolContextTransformGizmoMode Mode)
{
	if (HasGizmoCapture())
	{
		ToolsContext->InputRouter->ForceTerminateAll();
	}
	TransformGizmoMode = Mode;
	if (TransformGizmo)
	{
		TransformGizmo->Tick(0.f);
	}
}

UVTBOWTEditorToolsContext* UVTBOWTEditorSubsystem::GetToolsContext() const
{
	return ToolsContext;
}

UCombinedTransformGizmo* UVTBOWTEditorSubsystem::GetTransformGizmo() const
{
	return TransformGizmo;
}

UTransformProxy* UVTBOWTEditorSubsystem::GetTransformProxy() const
{
	return TransformGizmo ? TransformGizmo->GetTransformProxy() : nullptr;
}

EToolContextCoordinateSystem UVTBOWTEditorSubsystem::GetCoordinateSystem() const
{
	return CoordinateSystem;
}

EToolContextTransformGizmoMode UVTBOWTEditorSubsystem::GetTransformGizmoMode() const
{
	return TransformGizmoMode;
}

void UVTBOWTEditorSubsystem::UpdateGizmoView()
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	ULocalPlayer* Player = Controller ? Controller->GetLocalPlayer() : nullptr;
	UGameViewportClient* Viewport = Player ? Player->ViewportClient.Get() : nullptr;
	if (!Viewport || !Viewport->Viewport)
	{
		return;
	}

	FSceneViewFamilyContext Family(
	    FSceneViewFamily::ConstructionValues(Viewport->Viewport, GetWorld()->Scene, Viewport->EngineShowFlags)
	        .SetRealtimeUpdate(true));
	FVector Location;
	FRotator Rotation;
	const FSceneView* View = Player->CalcSceneView(&Family, Location, Rotation, Viewport->Viewport);
	if (!View)
	{
		return;
	}

	UGizmoViewContext* ViewContext = ToolsContext->ContextObjectStore->FindContext<UGizmoViewContext>();
	ViewContext->ResetFromSceneView(*View);
}

void UVTBOWTEditorSubsystem::ToggleEditing()
{
	check(IsInGameThread());
	bEditingEnabled = !bEditingEnabled;
	if (!bEditingEnabled)
	{
		HideSelectionGizmo();
		SelectedObject.Reset();
		return;
	}
	TransformGizmoMode = EToolContextTransformGizmoMode::Translation;
}

bool UVTBOWTEditorSubsystem::IsEditingEnabled() const
{
	return bEditingEnabled;
}

bool UVTBOWTEditorSubsystem::HasGizmoCapture() const
{
	return ToolsContext && ToolsContext->InputRouter->HasActiveMouseCapture();
}

bool UVTBOWTEditorSubsystem::RouteGizmoPointer(const FOWTGizmoPointerContext& Pointer)
{
	if (!ToolsContext)
	{
		return false;
	}
	UpdateGizmoView();
	FInputDeviceState Input;
	Input.InputDevice = EInputDevices::Mouse;
	Input.Mouse.WorldRay = FRay(Pointer.RayOrigin, Pointer.RayDirection);
	Input.Mouse.Position2D = Pointer.ScreenPosition;
	Input.Mouse.Left.SetStates(Pointer.bPressed, Pointer.bDown, Pointer.bReleased);
	if (Pointer.bPressed || Pointer.bDown || Pointer.bReleased)
	{
		ToolsContext->InputRouter->PostInputEvent(Input);
	}
	else
	{
		ToolsContext->InputRouter->PostHoverInputEvent(Input);
	}
	return true;
}
