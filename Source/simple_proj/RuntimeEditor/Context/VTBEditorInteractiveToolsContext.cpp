// Fill out your copyright notice in the Description page of Project Settings.


#include "VTBEditorInteractiveToolsContext.h"

#include "Private/VTBEditorTransactionsAPI.h"
#include "BaseGizmos/CombinedTransformGizmo.h"
#include "BaseGizmos/GizmoViewContext.h"
#include "Components/SceneComponent.h"
#include "ContextObjectStore.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/PlayerController.h"
#include "RuntimeEditor/Gizmo/VTBEditorTransformGizmo.h"
#include "InputRouter.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolManager.h"
#include "Materials/Material.h"
#include "SceneView.h"
#include "UnrealClient.h"
#include "UObject/UObjectHash.h"

/** Cached host state for ITF queries. This implementation is private to its runtime context. */
class FVTBEditorQueriesAPI final : public IToolsContextQueriesAPI
{
public:
	FVTBEditorQueriesAPI();

	TWeakObjectPtr<UInteractiveToolsContext> ToolsContext;
	TWeakObjectPtr<UWorld> EditingWorld;
	TWeakObjectPtr<UGameViewportClient> ActiveViewportClient;
	TArray<TWeakObjectPtr<AActor>> SelectedActors;
	FViewCameraState ViewState;
	FToolContextSnappingConfiguration SnappingSettings;
	EToolContextCoordinateSystem CoordinateSystem;
	EToolContextTransformGizmoMode GizmoMode;

	virtual UWorld* GetCurrentEditingWorld() const override { return EditingWorld.Get(); }
	virtual void GetCurrentViewState(FViewCameraState& StateOut) const override { StateOut = ViewState; }
	virtual EToolContextCoordinateSystem GetCurrentCoordinateSystem() const override
	{
		return GizmoMode == EToolContextTransformGizmoMode::Scale
			? EToolContextCoordinateSystem::Local : CoordinateSystem;
	}
	virtual EToolContextTransformGizmoMode GetCurrentTransformGizmoMode() const override { return GizmoMode; }
	virtual FToolContextSnappingConfiguration GetCurrentSnappingSettings() const override { return SnappingSettings; }
	virtual UMaterialInterface* GetStandardMaterial(EStandardToolContextMaterials MaterialType) const override
	{
		(void)MaterialType;
		return UMaterial::GetDefaultMaterial(MD_Surface);
	}
	virtual FViewport* GetHoveredViewport() const override
	{
		const UGameViewportClient* Client = ActiveViewportClient.Get();
		return Client ? Client->Viewport : nullptr;
	}
	virtual FViewport* GetFocusedViewport() const override { return GetHoveredViewport(); }
	virtual void GetCurrentSelectionState(FToolBuilderState& StateOut) const override
	{
		StateOut = FToolBuilderState();
		StateOut.World = EditingWorld.Get();
		const UInteractiveToolsContext* Context = ToolsContext.Get();
		if (Context)
		{
			StateOut.ToolManager = Context->ToolManager;
			StateOut.TargetManager = Context->TargetManager;
			StateOut.GizmoManager = Context->GizmoManager;
		}
		for (const TWeakObjectPtr<AActor>& WeakActor : SelectedActors)
		{
			AActor* Actor = WeakActor.Get();
			if (!IsValid(Actor) || Actor->GetWorld() != StateOut.World)
			{
				continue;
			}

			StateOut.SelectedActors.Add(Actor);
			if (USceneComponent* Root = Actor->GetRootComponent(); IsValid(Root))
			{
				StateOut.SelectedComponents.Add(Root);
			}
		}
	}
};

FVTBEditorQueriesAPI::FVTBEditorQueriesAPI()
	: ToolsContext(nullptr)
	, EditingWorld(nullptr)
	, ActiveViewportClient(nullptr)
	, CoordinateSystem(EToolContextCoordinateSystem::World)
	, GizmoMode(EToolContextTransformGizmoMode::Translation)
{
}

UVTBEditorInteractiveToolsContext::UVTBEditorInteractiveToolsContext()
	: RuntimePhase(EVTBEditorRuntimePhase::Uninitialized)
	, bCancellingInteraction(false)
	, bUpdating(false)
{
}

bool UVTBEditorInteractiveToolsContext::InitializeRuntime(UWorld* World)
{
	if (RuntimePhase == EVTBEditorRuntimePhase::Ready)
	{
		if (!ensureMsgf(QueriesAPI.IsValid(), TEXT("Runtime context is initialized without a queries API.")))
		{
			return false;
		}
		return QueriesAPI->GetCurrentEditingWorld() == World;
	}
	if (RuntimePhase != EVTBEditorRuntimePhase::Uninitialized)
	{
		return false;
	}
	if (!IsValid(World) || !World->IsGameWorld() || World->GetNetMode() == NM_DedicatedServer)
	{
		return false;
	}
	QueriesAPI = MakeShared<FVTBEditorQueriesAPI>();
	QueriesAPI->ToolsContext = this;
	QueriesAPI->EditingWorld = World;
	TransactionsAPI = MakeShared<FVTBEditorTransactionsAPI>();
	Super::Initialize(QueriesAPI.Get(), TransactionsAPI.Get());
	RuntimePhase = EVTBEditorRuntimePhase::Ready;
	if (FSlateApplication::IsInitialized())
	{
		auto& ActivationChanged = FSlateApplication::Get().OnApplicationActivationStateChanged();
		// UInputRouter registers a Slate focus callback in its constructor. Replace only that callback
		// so focus loss enters our cancellation/rollback path, and keep the guard for commandlets.
		ActivationChanged.RemoveAll(InputRouter);
		ApplicationFocusHandle = ActivationChanged.AddWeakLambda(this, [this](bool bFocused)
		{
			if (!bFocused)
			{
				CancelActiveInteraction();
			}
		});
	}
	// The stock gizmo manager registers its default builders and creates this view context.
	if (!ensureMsgf(IsValid(ContextObjectStore), TEXT("Runtime context has no context object store.")))
	{
		Shutdown();
		return false;
	}
	GizmoViewContext = ContextObjectStore->FindContext<UGizmoViewContext>();
	if (!ensureMsgf(IsValid(GizmoViewContext), TEXT("Runtime context failed to create a gizmo view context.")))
	{
		Shutdown();
		return false;
	}
	return true;
}

bool UVTBEditorInteractiveToolsContext::IsRuntimeReady() const
{
	if (RuntimePhase != EVTBEditorRuntimePhase::Ready)
	{
		return false;
	}
	return IsValid(GizmoManager) && IsValid(InputRouter);
}

UWorld* UVTBEditorInteractiveToolsContext::GetEditingWorld() const
{
	if (!QueriesAPI.IsValid())
	{
		return nullptr;
	}
	return QueriesAPI->GetCurrentEditingWorld();
}

EToolContextTransformGizmoMode UVTBEditorInteractiveToolsContext::GetGizmoMode() const
{
	if (!ensureMsgf(QueriesAPI.IsValid(), TEXT("Runtime gizmo mode requires a queries API.")))
	{
		return EToolContextTransformGizmoMode::NoGizmo;
	}
	return QueriesAPI->GetCurrentTransformGizmoMode();
}

bool UVTBEditorInteractiveToolsContext::UpdateView(APlayerController* PlayerController)
{
	if (!IsRuntimeReady())
	{
		return false;
	}
	if (!ensureMsgf(QueriesAPI.IsValid(), TEXT("Runtime view update requires a queries API.")))
	{
		return false;
	}
	check(GizmoViewContext);
	QueriesAPI->ActiveViewportClient.Reset();
	UWorld* World = QueriesAPI->GetCurrentEditingWorld();
	if (!IsValid(PlayerController) || !IsValid(World))
	{
		return false;
	}
	if (PlayerController->GetWorld() != World || World->Scene == nullptr)
	{
		return false;
	}
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	UGameViewportClient* ViewportClient = LocalPlayer ? LocalPlayer->ViewportClient : nullptr;
	FViewport* Viewport = ViewportClient ? ViewportClient->Viewport : nullptr;
	if (Viewport == nullptr || Viewport->GetSizeXY().GetMin() <= 0)
	{
		return false;
	}

	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(Viewport, World->Scene,
		ViewportClient->EngineShowFlags).SetRealtimeUpdate(true));
	FVector ViewLocation;
	FRotator ViewRotation;
	FSceneView* View = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, Viewport);
	if (View == nullptr || View->UnscaledViewRect.Width() <= 0 || View->UnscaledViewRect.Height() <= 0)
	{
		return false;
	}

	FViewCameraState CameraState;
	CameraState.Position = View->ViewLocation;
	CameraState.Orientation = ViewRotation.Quaternion();
	CameraState.bIsOrthographic = !View->IsPerspectiveProjection();
	CameraState.AspectRatio = static_cast<float>(View->UnscaledViewRect.Width()) / View->UnscaledViewRect.Height();
	CameraState.DPIScale = ViewportClient->GetDPIScale();
	const double ProjectionScaleX = FMath::Abs(View->ViewMatrices.GetProjectionMatrix().M[0][0]);
	if (ProjectionScaleX > UE_SMALL_NUMBER)
	{
		CameraState.HorizontalFOVDegrees = FMath::RadiansToDegrees(2.0 * FMath::Atan(1.0 / ProjectionScaleX));
		CameraState.OrthoWorldCoordinateWidth = 2.0 / ProjectionScaleX;
	}
	QueriesAPI->ViewState = CameraState;
	QueriesAPI->ActiveViewportClient = ViewportClient;
	GizmoViewContext->ResetFromSceneView(*View);
	GizmoViewContext->SetDPIScale(CameraState.DPIScale);
	return true;
}

bool UVTBEditorInteractiveToolsContext::UpdateView()
{
	if (!IsRuntimeReady())
	{
		return false;
	}

	UWorld* World = QueriesAPI->GetCurrentEditingWorld();
	if (!IsValid(World))
	{
		return false;
	}

	APlayerController* LocalController = nullptr;
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* Candidate = It->Get();
		if (!IsValid(Candidate) || !Candidate->IsLocalController())
		{
			continue;
		}
		LocalController = Candidate;
		break;
	}
	return UpdateView(LocalController);
}

bool UVTBEditorInteractiveToolsContext::PostPointerInput(const FInputDeviceState& Input, bool bHover)
{
	if (!BeginRuntimeUpdate())
	{
		return false;
	}

	ON_SCOPE_EXIT
	{
		EndRuntimeUpdate();
	};
	if (bHover)
	{
		InputRouter->PostHoverInputEvent(Input);
	}
	else
	{
		InputRouter->PostInputEvent(Input);
	}
	return true;
}

UVTBEditorTransformGizmo* UVTBEditorInteractiveToolsContext::CreateTransformGizmo(UObject* Owner)
{
	if (IsValid(TransformGizmo))
	{
		return TransformGizmo;
	}

	if (!ensureMsgf(IsValid(Owner), TEXT("Runtime transform gizmo creation requires a valid owner.")))
	{
		return nullptr;
	}
	if (!ensureMsgf(IsValid(GizmoManager), TEXT("Runtime transform gizmo creation requires an initialized gizmo manager.")))
	{
		return nullptr;
	}

	GizmoBuilder = NewObject<UVTBEditorTransformGizmoBuilder>(GizmoManager);
	if (!ensureMsgf(IsValid(GizmoBuilder), TEXT("Failed to create the runtime transform gizmo builder.")))
	{
		GizmoBuilder = nullptr;
		return nullptr;
	}
	GizmoManager->RegisterGizmoType(UVTBEditorTransformGizmoBuilder::BuilderIdentifier, GizmoBuilder);
	TransformGizmo = Cast<UVTBEditorTransformGizmo>(GizmoManager->CreateGizmo(
		UVTBEditorTransformGizmoBuilder::BuilderIdentifier, TEXT("VTB.RuntimeTransform"), Owner));
	if (!ensureMsgf(IsValid(TransformGizmo), TEXT("Failed to create the runtime transform gizmo.")))
	{
		TransformGizmo = nullptr;
		return nullptr;
	}

	TransformGizmo->SetVisibility(false);
	return TransformGizmo;
}

bool UVTBEditorInteractiveToolsContext::ApplyTransformGizmoState(
	UObject* Owner,
	const TOptional<TArray<TWeakObjectPtr<AActor>>>& SelectionRequest)
{
	if (!SelectionRequest.IsSet() && !IsValid(TransformGizmo))
	{
		return false;
	}

	if (!BeginRuntimeUpdate())
	{
		return false;
	}

	ON_SCOPE_EXIT
	{
		EndRuntimeUpdate();
	};

	UVTBEditorTransformGizmo* Gizmo = TransformGizmo;
	if (!IsValid(Gizmo))
	{
		Gizmo = CreateTransformGizmo(Owner);
		if (!IsValid(Gizmo))
		{
			return false;
		}
	}

	return Gizmo->ApplyRuntimeState(this, GetGizmoMode(), SelectionRequest);
}

void UVTBEditorInteractiveToolsContext::UpdateTransformGizmoVisibility(bool bHasView)
{
	if (!IsValid(TransformGizmo))
	{
		return;
	}

	TransformGizmo->SetVisibility(bHasView && TransformGizmo->ActiveTarget != nullptr);
}

void UVTBEditorInteractiveToolsContext::TickRuntime(float DeltaTime)
{
	if (!BeginRuntimeUpdate())
	{
		return;
	}

	ON_SCOPE_EXIT
	{
		EndRuntimeUpdate();
	};

	check(ToolManager);
	check(GizmoManager);
	ToolManager->Tick(DeltaTime);
	GizmoManager->Tick(DeltaTime);
}

void UVTBEditorInteractiveToolsContext::Shutdown()
{
	if (RuntimePhase == EVTBEditorRuntimePhase::Uninitialized
		|| RuntimePhase == EVTBEditorRuntimePhase::ShuttingDown)
	{
		return;
	}
	if (bUpdating || bCancellingInteraction || IsReplayingTransaction())
	{
		// Transform callbacks can request shutdown while the router or a change is still on the stack.
		RuntimePhase = EVTBEditorRuntimePhase::ShutdownPending;
		return;
	}
	RuntimePhase = EVTBEditorRuntimePhase::ShuttingDown;
	CancelActiveInteraction();
	// Manager shutdown callbacks must not tick or cancel a partially dismantled context.
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().OnApplicationActivationStateChanged().Remove(ApplicationFocusHandle);
	}
	ApplicationFocusHandle.Reset();

	// UE 5.7's manager shuts down a snapshot of active gizmos. Clear compound children first
	// so their parent shutdown cannot remove entries that the manager will later revisit.
	TArray<UObject*> ManagedObjects;
	GetObjectsWithOuter(GizmoManager, ManagedObjects);
	for (UObject* Object : ManagedObjects)
	{
		UCombinedTransformGizmo* Gizmo = Cast<UCombinedTransformGizmo>(Object);
		if (IsValid(Gizmo) && Gizmo->ActiveTarget != nullptr)
		{
			Gizmo->ClearActiveTarget();
		}
	}
	// Dependencies stay alive until all ITF managers finish their shutdown callbacks.
	Super::Shutdown();
	GizmoViewContext = nullptr;
	TransformGizmo = nullptr;
	GizmoBuilder = nullptr;
	TransactionsAPI.Reset();
	QueriesAPI.Reset();
	bUpdating = false;
	bCancellingInteraction = false;
	RuntimePhase = EVTBEditorRuntimePhase::Uninitialized;
}

void UVTBEditorInteractiveToolsContext::BeginDestroy()
{
	Shutdown();
	Super::BeginDestroy();
}

void UVTBEditorInteractiveToolsContext::SetSelection(const TArray<AActor*>& Actors)
{
	if (!QueriesAPI.IsValid())
	{
		return;
	}

	QueriesAPI->SelectedActors.Reset(Actors.Num());
	for (AActor* Actor : Actors)
	{
		if (IsValid(Actor) && Actor->GetWorld() == QueriesAPI->EditingWorld.Get())
		{
			QueriesAPI->SelectedActors.AddUnique(Actor);
		}
	}
}

void UVTBEditorInteractiveToolsContext::SetCoordinateSystem(EToolContextCoordinateSystem CoordinateSystem)
{
	if (!QueriesAPI.IsValid())
	{
		return;
	}

	// Non-uniform scale is evaluated in local space so the scale handles remain
	// aligned with the selected actor even when the requested editor space is World.
	if (QueriesAPI->GizmoMode == EToolContextTransformGizmoMode::Scale)
	{
		QueriesAPI->CoordinateSystem = EToolContextCoordinateSystem::Local;
		return;
	}

	// CombinedTransformGizmo supports World and Local, not Screen coordinates.
	QueriesAPI->CoordinateSystem = CoordinateSystem == EToolContextCoordinateSystem::Local
		? EToolContextCoordinateSystem::Local : EToolContextCoordinateSystem::World;
}

void UVTBEditorInteractiveToolsContext::SetGizmoMode(EToolContextTransformGizmoMode Mode)
{
	if (!QueriesAPI.IsValid())
	{
		return;
	}

	QueriesAPI->GizmoMode = Mode;
	if (Mode == EToolContextTransformGizmoMode::Scale)
	{
		QueriesAPI->CoordinateSystem = EToolContextCoordinateSystem::Local;
	}
}

void UVTBEditorInteractiveToolsContext::CancelActiveInteraction()
{
	if (RuntimePhase == EVTBEditorRuntimePhase::Uninitialized
		|| RuntimePhase == EVTBEditorRuntimePhase::ShutdownPending
		|| bCancellingInteraction
		|| IsReplayingTransaction())
	{
		if (RuntimePhase == EVTBEditorRuntimePhase::ShutdownPending && !bUpdating && !IsReplayingTransaction())
		{
			Shutdown();
		}
		return;
	}

	check(TransactionsAPI.IsValid());
	check(InputRouter);
	// Proxy callbacks run before ForceTerminateAll clears its active capture pointers.
	TGuardValue<bool> CancellingGuard(bCancellingInteraction, true);
	TransactionsAPI->BeginCancellation();
	InputRouter->ForceTerminateAll();
	TransactionsAPI->EndCancellation();

	if (RuntimePhase == EVTBEditorRuntimePhase::ShutdownPending)
	{
		Shutdown();
	}
}

bool UVTBEditorInteractiveToolsContext::BeginRuntimeUpdate()
{
	if (!IsRuntimeReady() || bUpdating || bCancellingInteraction || IsReplayingTransaction())
	{
		return false;
	}

	bUpdating = true;
	return true;
}

void UVTBEditorInteractiveToolsContext::EndRuntimeUpdate()
{
	bUpdating = false;
	if (RuntimePhase == EVTBEditorRuntimePhase::ShutdownPending)
	{
		Shutdown();
	}
}

bool UVTBEditorInteractiveToolsContext::HasActiveMouseCapture() const
{
	if (!IsValid(InputRouter))
	{
		return false;
	}
	return InputRouter->HasActiveMouseCapture();
}

bool UVTBEditorInteractiveToolsContext::Undo()
{
	if (RuntimePhase == EVTBEditorRuntimePhase::ShuttingDown || !TransactionsAPI.IsValid())
	{
		return false;
	}

	const bool bUndone = TransactionsAPI->Undo();
	if (RuntimePhase == EVTBEditorRuntimePhase::ShutdownPending)
	{
		Shutdown();
	}
	return bUndone;
}

bool UVTBEditorInteractiveToolsContext::Redo()
{
	if (RuntimePhase == EVTBEditorRuntimePhase::ShuttingDown || !TransactionsAPI.IsValid())
	{
		return false;
	}

	const bool bRedone = TransactionsAPI->Redo();
	if (RuntimePhase == EVTBEditorRuntimePhase::ShutdownPending)
	{
		Shutdown();
	}
	return bRedone;
}

bool UVTBEditorInteractiveToolsContext::CanUndo() const
{
	if (RuntimePhase == EVTBEditorRuntimePhase::ShuttingDown || !TransactionsAPI.IsValid())
	{
		return false;
	}
	return TransactionsAPI->CanUndo();
}

bool UVTBEditorInteractiveToolsContext::CanRedo() const
{
	if (RuntimePhase == EVTBEditorRuntimePhase::ShuttingDown || !TransactionsAPI.IsValid())
	{
		return false;
	}
	return TransactionsAPI->CanRedo();
}

bool UVTBEditorInteractiveToolsContext::IsReplayingTransaction() const
{
	return TransactionsAPI.IsValid() && TransactionsAPI->IsReplaying();
}
