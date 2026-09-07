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
	virtual EToolContextCoordinateSystem GetCurrentCoordinateSystem() const override { return CoordinateSystem; }
	virtual EToolContextTransformGizmoMode GetCurrentTransformGizmoMode() const override { return GizmoMode; }
	virtual FToolContextSnappingConfiguration GetCurrentSnappingSettings() const override { return SnappingSettings; }
	virtual UMaterialInterface* GetStandardMaterial(EStandardToolContextMaterials MaterialType) const override
	{
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
		if (const UInteractiveToolsContext* Context = ToolsContext.Get())
		{
			StateOut.ToolManager = Context->ToolManager;
			StateOut.TargetManager = Context->TargetManager;
			StateOut.GizmoManager = Context->GizmoManager;
		}
		for (const TWeakObjectPtr<AActor>& WeakActor : SelectedActors)
		{
			if (AActor* Actor = WeakActor.Get(); Actor && Actor->GetWorld() == StateOut.World)
			{
				StateOut.SelectedActors.Add(Actor);
				if (USceneComponent* Root = Actor->GetRootComponent())
				{
					StateOut.SelectedComponents.Add(Root);
				}
			}
		}
	}
};

FVTBEditorQueriesAPI::FVTBEditorQueriesAPI()
	: ToolsContext(nullptr)
	, EditingWorld(nullptr)
	, ActiveViewportClient(nullptr)
	, CoordinateSystem(EToolContextCoordinateSystem::World)
	, GizmoMode(EToolContextTransformGizmoMode::Combined)
{
}

UVTBEditorInteractiveToolsContext::UVTBEditorInteractiveToolsContext()
	: bRuntimeInitialized(false)
	, bCancellingInteraction(false)
{
}

bool UVTBEditorInteractiveToolsContext::InitializeRuntime(UWorld* World)
{
	if (bRuntimeInitialized)
	{
		return QueriesAPI->GetCurrentEditingWorld() == World;
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
	bRuntimeInitialized = true;
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
	GizmoViewContext = ContextObjectStore->FindContext<UGizmoViewContext>();
	if (!GizmoViewContext)
	{
		Shutdown();
		return false;
	}
	return true;
}

bool UVTBEditorInteractiveToolsContext::UpdateView(APlayerController* PlayerController)
{
	if (!bRuntimeInitialized)
	{
		return false;
	}
	QueriesAPI->ActiveViewportClient.Reset();
	UWorld* World = QueriesAPI->GetCurrentEditingWorld();
	if (!IsValid(PlayerController) || PlayerController->GetWorld() != World || !World || !World->Scene)
	{
		return false;
	}
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	UGameViewportClient* ViewportClient = LocalPlayer ? LocalPlayer->ViewportClient : nullptr;
	FViewport* Viewport = ViewportClient ? ViewportClient->Viewport : nullptr;
	if (!Viewport || Viewport->GetSizeXY().GetMin() <= 0)
	{
		return false;
	}

	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(Viewport, World->Scene,
		ViewportClient->EngineShowFlags).SetRealtimeUpdate(true));
	FVector ViewLocation;
	FRotator ViewRotation;
	FSceneView* View = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, Viewport);
	if (!View || View->UnscaledViewRect.Width() <= 0 || View->UnscaledViewRect.Height() <= 0)
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

void UVTBEditorInteractiveToolsContext::TickRuntime(float DeltaTime)
{
	if (bRuntimeInitialized)
	{
		ToolManager->Tick(DeltaTime);
		GizmoManager->Tick(DeltaTime);
	}
}

void UVTBEditorInteractiveToolsContext::Shutdown()
{
	if (!bRuntimeInitialized)
	{
		return;
	}
	CancelActiveInteraction();
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
		if (UCombinedTransformGizmo* Gizmo = Cast<UCombinedTransformGizmo>(Object);
			IsValid(Gizmo) && Gizmo->ActiveTarget)
		{
			Gizmo->ClearActiveTarget();
		}
	}
	// Dependencies stay alive until all ITF managers finish their shutdown callbacks.
	Super::Shutdown();
	GizmoViewContext = nullptr;
	bRuntimeInitialized = false;
	TransactionsAPI.Reset();
	QueriesAPI.Reset();
}

void UVTBEditorInteractiveToolsContext::BeginDestroy()
{
	Shutdown();
	Super::BeginDestroy();
}

void UVTBEditorInteractiveToolsContext::SetSelection(const TArray<AActor*>& Actors)
{
	if (QueriesAPI)
	{
		QueriesAPI->SelectedActors.Reset(Actors.Num());
		for (AActor* Actor : Actors)
		{
			if (IsValid(Actor) && Actor->GetWorld() == QueriesAPI->EditingWorld.Get())
			{
				QueriesAPI->SelectedActors.AddUnique(Actor);
			}
		}
	}
}

void UVTBEditorInteractiveToolsContext::SetCoordinateSystem(EToolContextCoordinateSystem CoordinateSystem)
{
	if (QueriesAPI)
	{
		// CombinedTransformGizmo supports World and Local, not Screen coordinates.
		QueriesAPI->CoordinateSystem = CoordinateSystem == EToolContextCoordinateSystem::Local
			? EToolContextCoordinateSystem::Local : EToolContextCoordinateSystem::World;
	}
}

void UVTBEditorInteractiveToolsContext::SetGizmoMode(EToolContextTransformGizmoMode Mode)
{
	if (QueriesAPI)
	{
		QueriesAPI->GizmoMode = Mode;
	}
}

void UVTBEditorInteractiveToolsContext::CancelActiveInteraction()
{
	if (bRuntimeInitialized && !bCancellingInteraction)
	{
		// Proxy callbacks run before ForceTerminateAll clears its active capture pointers.
		TGuardValue<bool> CancellingGuard(bCancellingInteraction, true);
		TransactionsAPI->BeginCancellation();
		InputRouter->ForceTerminateAll();
		TransactionsAPI->EndCancellation();
	}
}

bool UVTBEditorInteractiveToolsContext::Undo()
{
	return TransactionsAPI && TransactionsAPI->Undo();
}

bool UVTBEditorInteractiveToolsContext::Redo()
{
	return TransactionsAPI && TransactionsAPI->Redo();
}

bool UVTBEditorInteractiveToolsContext::CanUndo() const
{
	return TransactionsAPI && TransactionsAPI->CanUndo();
}

bool UVTBEditorInteractiveToolsContext::CanRedo() const
{
	return TransactionsAPI && TransactionsAPI->CanRedo();
}
