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

#include "Modes/VTBOWTObjectEditMode.h"
#include "SceneView.h"
#include "VTBAttributeEditor.h"

UVTBOWTEditorSubsystem::UVTBOWTEditorSubsystem()
    : ActiveEditMode(nullptr), SelectedObject(), ToolsContext(nullptr), TransformGizmo(nullptr), AttributeEditor(),
      GizmoTarget(), SystemContextHandlers(), CoordinateSystem(EToolContextCoordinateSystem::World),
      TransformGizmoMode(EToolContextTransformGizmoMode::Translation), bEditingEnabled(false)
{
}

void UVTBOWTEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ActiveEditMode = NewObject<UVTBOWTObjectEditMode>(this);
	SystemContextHandlers.Add(FOWTToggleEditingContext::StaticStruct(),
	                          [this](const FInstancedStruct&)
	                          {
		                          ToggleEditing();
		                          return true;
	                          });
	SystemContextHandlers.Add(FOWTGizmoPointerContext::StaticStruct(),
	                          [this](const FInstancedStruct& Context)
	                          {
		                          if (!bEditingEnabled)
		                          {
			                          return false;
		                          }
		                          return RouteGizmoPointer(Context.Get<FOWTGizmoPointerContext>());
	                          });
	InitializeToolsContext();
}

void UVTBOWTEditorSubsystem::Deinitialize()
{
	ShutdownToolsContext();
	bEditingEnabled = false;
	SetSelectedObject(nullptr);
	NotifyEditorStateChanged();
	AttributeEditor.Reset();
	SystemContextHandlers.Empty();
	ActiveEditMode = nullptr;
	Super::Deinitialize();
}

void UVTBOWTEditorSubsystem::OnWorldEndPlay(UWorld& InWorld)
{
	ShutdownToolsContext();
	bEditingEnabled = false;
	SetSelectedObject(nullptr);
	NotifyEditorStateChanged();
	Super::OnWorldEndPlay(InWorld);
}

void UVTBOWTEditorSubsystem::Tick(float DeltaTime)
{
	if (SelectedObject.IsStale())
	{
		SetSelectedObject(nullptr);
	}
	if (AVTBAttributeEditor* Editor = AttributeEditor.Get())
	{
		Editor->RefreshSelectedTransform();
	}
	if (!ToolsContext)
	{
		return;
	}
	if (TransformGizmo)
	{
		AActor* Actor = SelectedObject.Get();
		USceneComponent* Root = Actor ? Actor->GetRootComponent() : nullptr;
		if (GizmoTarget.Get() != Root)
		{
			HideSelectionGizmo();
			ShowSelectionGizmo();
		}
		else if (!IsValid(Root))
		{
			HideSelectionGizmo();
		}
		else if (Root->Mobility != EComponentMobility::Movable)
		{
			HideSelectionGizmo();
		}
	}

	UpdateGizmoView();
	ToolsContext->ToolManager->Tick(DeltaTime);
	ToolsContext->GizmoManager->Tick(DeltaTime);
}

bool UVTBOWTEditorSubsystem::ReceiveEditContext_Implementation(const FInstancedStruct& Context)
{
	check(IsInGameThread());
	if (!Context.IsValid())
	{
		return false;
	}
	if (!IsValid(ActiveEditMode))
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
	if (Mode == this)
	{
		return false;
	}
	if (!Mode->GetClass()->ImplementsInterface(UOWTEditContextReceiver::StaticClass()))
	{
		return false;
	}

	HideSelectionGizmo();
	ActiveEditMode = Mode;
	NotifyEditorStateChanged();
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
	if (!World)
	{
		return false;
	}
	if (World->bIsTearingDown)
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
	if (!ToolsContext)
	{
		return false;
	}
	return ToolsContext->SetSnapSettings(Settings);
}

FOWTGizmoSnapSettings UVTBOWTEditorSubsystem::GetGizmoSnapSettings() const
{
	return ToolsContext ? ToolsContext->GetSnapSettings() : FOWTGizmoSnapSettings();
}

void UVTBOWTEditorSubsystem::ShowSelectionGizmo()
{
	check(IsInGameThread());
	if (!bEditingEnabled)
	{
		return;
	}
	if (!ToolsContext)
	{
		return;
	}
	if (GetWorld()->bIsTearingDown)
	{
		return;
	}
	AActor* Actor = SelectedObject.Get();
	USceneComponent* Root = Actor ? Actor->GetRootComponent() : nullptr;
	if (!IsValid(Root))
	{
		return;
	}
	if (Root->Mobility != EComponentMobility::Movable)
	{
		return;
	}
	if (TransformGizmo)
	{
		if (GizmoTarget.Get() == Root)
		{
			return;
		}
	}

	HideSelectionGizmo();
	TransformGizmo = Cast<UVTBOWTBaseTransformGizmo>(
	    ToolsContext->GizmoManager->CreateGizmo(TEXT("OWT.Transform"), FString(), this));
	if (!ensureMsgf(TransformGizmo, TEXT("OWT transform gizmo creation failed.")))
	{
		return;
	}

	TransformGizmo->SetTargetComponent(*Root);
	UTransformProxy* Proxy = TransformGizmo->GetTransformProxy();
	check(Proxy);
	Proxy->OnBeginTransformEdit.AddUObject(this, &ThisClass::OnGizmoEditStarted);
	Proxy->OnTransformChanged.AddUObject(this, &ThisClass::OnGizmoTransformChanged);
	Proxy->OnEndTransformEdit.AddUObject(this, &ThisClass::OnGizmoEditEnded);
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
	if (AVTBAttributeEditor* Editor = AttributeEditor.Get())
	{
		Editor->FinishActiveOperation();
	}
	if (!TransformGizmo)
	{
		return;
	}

	TerminateGizmoCapture();
	UTransformProxy* Proxy = TransformGizmo->GetTransformProxy();
	if (Proxy)
	{
		Proxy->OnBeginTransformEdit.RemoveAll(this);
		Proxy->OnTransformChanged.RemoveAll(this);
		Proxy->OnEndTransformEdit.RemoveAll(this);
	}
	// The manager owns the gizmo actor and its axis gizmos.
	ToolsContext->GizmoManager->DestroyAllGizmosByOwner(this);
	TransformGizmo = nullptr;
	GizmoTarget.Reset();
}

void UVTBOWTEditorSubsystem::SetSelectedObject(AActor* Actor)
{
	check(IsInGameThread());
	if (Actor)
	{
		if (!IsValid(Actor))
		{
			return;
		}
		if (Actor->GetWorld() != GetWorld())
		{
			return;
		}
	}
	if (SelectedObject.Get() == Actor)
	{
		if (!SelectedObject.IsStale())
		{
			ShowSelectionGizmo();
			return;
		}
	}

	HideSelectionGizmo();
	SelectedObject = Actor;
	ShowSelectionGizmo();
	if (AVTBAttributeEditor* Editor = AttributeEditor.Get())
	{
		Editor->NotifySelectionChanged();
	}
}

void UVTBOWTEditorSubsystem::SetCoordinateSystem(EToolContextCoordinateSystem System)
{
	TerminateGizmoCapture();
	if (AVTBAttributeEditor* Editor = AttributeEditor.Get())
	{
		Editor->FinishActiveOperation();
	}
	CoordinateSystem = System;
	NotifyEditorStateChanged();
}

void UVTBOWTEditorSubsystem::SetTransformGizmoMode(EToolContextTransformGizmoMode Mode)
{
	TerminateGizmoCapture();
	if (AVTBAttributeEditor* Editor = AttributeEditor.Get())
	{
		Editor->FinishActiveOperation();
	}
	TransformGizmoMode = Mode;
	if (TransformGizmo)
	{
		TransformGizmo->Tick(0.f);
	}
	NotifyEditorStateChanged();
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
		SetSelectedObject(nullptr);
		HideSelectionGizmo();
		NotifyEditorStateChanged();
		return;
	}
	TransformGizmoMode = EToolContextTransformGizmoMode::Translation;
	NotifyEditorStateChanged();
}

bool UVTBOWTEditorSubsystem::IsEditingEnabled() const
{
	return bEditingEnabled;
}

bool UVTBOWTEditorSubsystem::HasGizmoCapture() const
{
	if (!ToolsContext)
	{
		return false;
	}
	return ToolsContext->InputRouter->HasActiveMouseCapture();
}

void UVTBOWTEditorSubsystem::RegisterAttributeEditor(AVTBAttributeEditor* Editor)
{
	check(IsInGameThread());
	if (Editor)
	{
		checkf(Editor->GetWorld() == GetWorld(), TEXT("AttributeEditor must belong to this world."));
	}
	AttributeEditor = Editor;
}

AVTBAttributeEditor* UVTBOWTEditorSubsystem::GetAttributeEditor() const
{
	return AttributeEditor.Get();
}

void UVTBOWTEditorSubsystem::TerminateGizmoCapture()
{
	if (HasGizmoCapture())
	{
		ToolsContext->InputRouter->ForceTerminateAll();
	}
}

void UVTBOWTEditorSubsystem::SynchronizeSelectionGizmo()
{
	if (!TransformGizmo)
	{
		return;
	}
	if (HasGizmoCapture())
	{
		return;
	}
	AActor* Actor = SelectedObject.Get();
	if (!Actor)
	{
		return;
	}
	USceneComponent* Root = Actor->GetRootComponent();
	if (!IsValid(Root))
	{
		return;
	}
	if (GizmoTarget.Get() != Root)
	{
		return;
	}
	TransformGizmo->ReinitializeGizmoTransform(Root->GetComponentTransform());
}

void UVTBOWTEditorSubsystem::NotifyEditorStateChanged()
{
	if (AVTBAttributeEditor* Editor = AttributeEditor.Get())
	{
		Editor->NotifyEditorStateChanged();
	}
}

void UVTBOWTEditorSubsystem::OnGizmoEditStarted(UTransformProxy* Proxy)
{
	if (AVTBAttributeEditor* Editor = AttributeEditor.Get())
	{
		Editor->BeginGizmoEdit();
	}
}

void UVTBOWTEditorSubsystem::OnGizmoTransformChanged(UTransformProxy* Proxy, FTransform Transform)
{
	if (AVTBAttributeEditor* Editor = AttributeEditor.Get())
	{
		Editor->UpdateGizmoEdit();
	}
}

void UVTBOWTEditorSubsystem::OnGizmoEditEnded(UTransformProxy* Proxy)
{
	if (AVTBAttributeEditor* Editor = AttributeEditor.Get())
	{
		Editor->EndGizmoEdit();
	}
}

bool UVTBOWTEditorSubsystem::RouteGizmoPointer(const FOWTGizmoPointerContext& Pointer)
{
	if (!ToolsContext)
	{
		return false;
	}
	if (Pointer.bPressed)
	{
		if (!HasGizmoCapture())
		{
			if (AVTBAttributeEditor* Editor = AttributeEditor.Get())
			{
				if (Editor->GetSnapshot().bIsModifying)
				{
					return false;
				}
			}
		}
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
