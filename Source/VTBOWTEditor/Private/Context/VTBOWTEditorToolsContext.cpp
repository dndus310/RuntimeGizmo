#include "Context/VTBOWTEditorToolsContext.h"

#include "Context/VTBOWTEditorToolsContextInput.h"
#include "Context/VTBOWTEditorToolsContextViewport.h"
#include "Context/VTBOWTEditorTransactionHistory.h"
#include "Context/IVTBOWTEditorSceneState.h"

#include "BaseGizmos/CombinedTransformGizmo.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "InputRouter.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolManager.h"
#include "InteractiveToolChange.h"
#include "Materials/Material.h"
#include "MaterialDomain.h"
#include "SceneView.h"
#include "UnrealClient.h"
#include "UObject/UObjectHash.h"

DEFINE_LOG_CATEGORY_STATIC(LogVTBOWTToolsContext, Log, All);

class FVTBOWTEditorToolsContextRenderImpl final : public IToolsContextRenderAPI
{
public:
	FVTBOWTEditorToolsContextRenderImpl(UVTBOWTEditorToolsContext& InOwner)
		: View(nullptr), PDI(nullptr), Owner(InOwner)
	{
	}

	virtual FPrimitiveDrawInterface* GetPrimitiveDrawInterface() override
	{
		return PDI;
	}
	virtual const FSceneView* GetSceneView() override
	{
		return View;
	}
	virtual FViewCameraState GetCameraState() override
	{
		return CameraState;
	}
	virtual EViewInteractionState GetViewInteractionState() override
	{
		return InteractionState;
	}

	void UpdateCameraState(const FSceneView* InView, FPrimitiveDrawInterface* InPDI)
	{
		View = InView;
		PDI = InPDI;
		CameraState = FViewCameraState();
		CameraState.bIsOrthographic = !View->IsPerspectiveProjection();
		CameraState.bIsVR = false;
		CameraState.Position = View->ViewLocation;
		CameraState.Orientation = View->ViewRotation.Quaternion();
		CameraState.AspectRatio = View->UnscaledViewRect.Height() > 0
			? float(View->UnscaledViewRect.Width()) / View->UnscaledViewRect.Height()
			: 1.0f;
		const double ProjectionScale = FMath::Abs(View->ViewMatrices.GetProjectionMatrix().M[0][0]);
		if (ProjectionScale > UE_SMALL_NUMBER)
		{
			CameraState.HorizontalFOVDegrees = FMath::RadiansToDegrees(2.0 * FMath::Atan(1.0 / ProjectionScale));
			CameraState.OrthoWorldCoordinateWidth = float(2.0 / ProjectionScale);
		}
		UWorld* World = Owner.GetEditingWorld();
		UGameViewportClient* GameViewport = World ? World->GetGameViewport() : nullptr;
		CameraState.DPIScale = GameViewport ? GameViewport->GetDPIScale() : 1.0;
	}
	void ResetView()
	{
		View = nullptr;
		PDI = nullptr;
	}

private:
	const FSceneView* View;
	FPrimitiveDrawInterface* PDI;
	UVTBOWTEditorToolsContext& Owner;
	FViewCameraState CameraState;
	EViewInteractionState InteractionState = EViewInteractionState::None;
};

class FVTBOWTEditorToolsContextQueriesImpl final : public IToolsContextQueriesAPI
{
public:
	explicit FVTBOWTEditorToolsContextQueriesImpl(UVTBOWTEditorToolsContext& InOwner)
		: Owner(InOwner)
	{
	}

	void Initialize(UWorld* InWorld, IToolsContextQueriesAPI* InBackend)
	{
		EditingWorld = InWorld;
		Backend = InBackend;
	}

	virtual UWorld* GetCurrentEditingWorld() const override
	{
		return Backend ? Backend->GetCurrentEditingWorld() : EditingWorld.Get();
	}
	virtual void GetCurrentSelectionState(FToolBuilderState& StateOut) const override
	{
		if (Backend)
		{
			Backend->GetCurrentSelectionState(StateOut);
		}
		else
		{
			StateOut = FToolBuilderState();
			StateOut.World = EditingWorld.Get();
			for (const TWeakObjectPtr<AActor>& Actor : SelectedActors)
			{
				if (Actor.IsValid() && !Actor->IsActorBeingDestroyed() && Actor->GetWorld() == StateOut.World)
				{
					StateOut.SelectedActors.Add(Actor.Get());
				}
			}
			for (const TWeakObjectPtr<UActorComponent>& Component : SelectedComponents)
			{
				if (Component.IsValid() && Component->GetWorld() == StateOut.World)
				{
					StateOut.SelectedComponents.Add(Component.Get());
				}
			}
		}
		StateOut.ToolManager = Owner.ToolManager;
		StateOut.TargetManager = Owner.TargetManager;
		StateOut.GizmoManager = Owner.GizmoManager;
	}
	virtual void GetCurrentViewState(FViewCameraState& StateOut) const override
	{
		IToolsContextRenderAPI* RenderAPI = Owner.GetContextRenderAPI();
		if (Backend && (!RenderAPI || !RenderAPI->GetSceneView()))
		{
			Backend->GetCurrentViewState(StateOut);
			return;
		}
		StateOut = RenderAPI ? RenderAPI->GetCameraState() : FViewCameraState();
	}
	virtual EToolContextCoordinateSystem GetCurrentCoordinateSystem() const override
	{
		if (Backend)
		{
			return Backend->GetCurrentCoordinateSystem();
		}
		if (GizmoMode == EToolContextTransformGizmoMode::Scale)
		{
			return EToolContextCoordinateSystem::Local;
		}
		return CoordinateSystem;
	}
	virtual EToolContextTransformGizmoMode GetCurrentTransformGizmoMode() const override
	{
		return Backend ? Backend->GetCurrentTransformGizmoMode() : GizmoMode;
	}
	virtual FToolContextSnappingConfiguration GetCurrentSnappingSettings() const override
	{
		if (Backend)
		{
			return Backend->GetCurrentSnappingSettings();
		}
		FToolContextSnappingConfiguration SnappingSettings;
		SnappingSettings.bEnablePositionGridSnapping = false;
		SnappingSettings.PositionGridDimensions = FVector::Zero();
		SnappingSettings.bEnableRotationGridSnapping = false;
		SnappingSettings.RotationGridAngles = FRotator::ZeroRotator;
		SnappingSettings.bEnableScaleGridSnapping = false;
		SnappingSettings.ScaleGridSize = 1.0f;
		SnappingSettings.bEnableAbsoluteWorldSnapping = false;
		return SnappingSettings;
	}
	virtual UMaterialInterface* GetStandardMaterial(EStandardToolContextMaterials MaterialType) const override
	{
		return Backend ? Backend->GetStandardMaterial(MaterialType) : UMaterial::GetDefaultMaterial(MD_Surface);
	}
	virtual FViewport* GetHoveredViewport() const override
	{
		if (Backend)
		{
			return Backend->GetHoveredViewport();
		}
		UWorld* World = EditingWorld.Get();
		UGameViewportClient* GameViewport = World ? World->GetGameViewport() : nullptr;
		return GameViewport ? GameViewport->Viewport : nullptr;
	}
	virtual FViewport* GetFocusedViewport() const override
	{
		return Backend ? Backend->GetFocusedViewport() : GetHoveredViewport();
	}
	void SetSelection(const TArray<AActor*>& Actors, const TArray<UActorComponent*>& Components)
	{
		SelectedActors.Reset();
		SelectedComponents.Reset();
		UWorld* World = GetCurrentEditingWorld();
		for (AActor* Actor : Actors)
		{
			if (IsValid(Actor) && !Actor->IsActorBeingDestroyed() && Actor->GetWorld() == World)
			{
				SelectedActors.AddUnique(Actor);
			}
		}
		for (UActorComponent* Component : Components)
		{
			if (IsValid(Component) && Component->GetWorld() == World)
			{
				SelectedComponents.AddUnique(Component);
			}
		}
	}

	UVTBOWTEditorToolsContext& Owner;
	EToolContextCoordinateSystem CoordinateSystem = EToolContextCoordinateSystem::World;
	EToolContextTransformGizmoMode GizmoMode = EToolContextTransformGizmoMode::Translation;

private:
	TWeakObjectPtr<UWorld> EditingWorld;
	IToolsContextQueriesAPI* Backend = nullptr;
	TArray<TWeakObjectPtr<AActor>> SelectedActors;
	TArray<TWeakObjectPtr<UActorComponent>> SelectedComponents;
};

class FVTBOWTEditorToolsContextTransactionImpl final : public IToolsContextTransactionsAPI
{
public:
	FVTBOWTEditorToolsContextTransactionImpl(UVTBOWTEditorToolsContext& InOwner,
		IVTBOWTEditorTransactionHistory& InHistory)
		: Owner(InOwner)
		, History(InHistory)
	{
	}

	void Initialize(IToolsContextTransactionsAPI* InBackend)
	{
		Backend = InBackend;
	}

	virtual void DisplayMessage(const FText& Message, EToolMessageLevel Level) override
	{
		if (Backend)
		{
			Backend->DisplayMessage(Message, Level);
			return;
		}
		UE_LOG(LogVTBOWTToolsContext, Log, TEXT("ITF message (%d): %s"), static_cast<int32>(Level), *Message.ToString());
	}
	virtual void PostInvalidation() override
	{
		if (FViewport* Viewport = Owner.GetContextQueriesAPI()->GetFocusedViewport())
		{
			Viewport->Invalidate();
		}
		if (Backend)
		{
			Backend->PostInvalidation();
		}
	}
	virtual void BeginUndoTransaction(const FText& Description) override
	{
		if (Backend)
		{
			Backend->BeginUndoTransaction(Description);
			return;
		}
		History.BeginUndoTransaction(Description);
	}
	virtual void EndUndoTransaction() override
	{
		if (Backend)
		{
			Backend->EndUndoTransaction();
			return;
		}
		History.EndUndoTransaction();
	}
	virtual void AppendChange(UObject* TargetObject, TUniquePtr<FToolCommandChange> Change, const FText& Description) override
	{
		if (Backend)
		{
			Backend->AppendChange(TargetObject, MoveTemp(Change), Description);
			return;
		}
		History.AppendChange(TargetObject, MoveTemp(Change), Description);
	}
	virtual bool RequestSelectionChange(const FSelectedObjectsChangeList& SelectionChange) override
	{
		if (Backend)
		{
			return Backend->RequestSelectionChange(SelectionChange);
		}
		return Owner.OnSelectionChangeRequested.IsBound() && Owner.OnSelectionChangeRequested.Execute(SelectionChange);
	}

	UVTBOWTEditorToolsContext& Owner;

private:
	IVTBOWTEditorTransactionHistory& History;
	IToolsContextTransactionsAPI* Backend = nullptr;
};

class FVTBOWTEditorSceneState final : public IVTBOWTEditorSceneState
{
public:
	explicit FVTBOWTEditorSceneState(UVTBOWTEditorToolsContext& InOwner)
		: Owner(InOwner)
	{
	}

	virtual void SetActorSelection(const TArray<AActor*>& Actors) override
	{
		TArray<UActorComponent*> Components;
		for (AActor* Actor : Actors)
		{
			if (IsValid(Actor))
			{
				Components.Add(Actor->GetRootComponent());
			}
		}
		SetSelection(Actors, Components);
	}

	virtual void SetSelection(const TArray<AActor*>& Actors, const TArray<UActorComponent*>& Components) override
	{
		if (Owner.ContextQueriesAPI && !Owner.bShutdownRequested)
		{
			Owner.ContextQueriesAPI->SetSelection(Actors, Components);
		}
	}

	virtual void SetCoordinateSystem(EToolContextCoordinateSystem CoordinateSystem) override
	{
		if (Owner.ContextQueriesAPI)
		{
			Owner.ContextQueriesAPI->CoordinateSystem = CoordinateSystem == EToolContextCoordinateSystem::Local
				? EToolContextCoordinateSystem::Local
				: EToolContextCoordinateSystem::World;
		}
	}

	virtual void SetGizmoMode(EToolContextTransformGizmoMode Mode) override
	{
		if (Owner.ContextQueriesAPI)
		{
			Owner.ContextQueriesAPI->GizmoMode = Mode;
		}
	}

	virtual EToolContextTransformGizmoMode GetGizmoMode() const override
	{
		return Owner.ContextQueriesAPI
			? Owner.ContextQueriesAPI->GetCurrentTransformGizmoMode()
			: EToolContextTransformGizmoMode::NoGizmo;
	}

private:
	UVTBOWTEditorToolsContext& Owner;
};

UVTBOWTEditorToolsContext::UVTBOWTEditorToolsContext()
	: TransactionHistory(CreateVTBOWTEditorTransactionHistory(*this))
	, SceneState(MakeUnique<FVTBOWTEditorSceneState>(*this))
	, ContextInput(MakeUnique<FVTBOWTEditorToolsContextInput>(*this, *TransactionHistory))
	, ContextViewport(MakeUnique<FVTBOWTEditorToolsContextViewport>(*this))
{
}
UVTBOWTEditorToolsContext::UVTBOWTEditorToolsContext(FVTableHelper& Helper)
	: Super(Helper)
	, TransactionHistory(CreateVTBOWTEditorTransactionHistory(*this))
	, SceneState(MakeUnique<FVTBOWTEditorSceneState>(*this))
	, ContextInput(MakeUnique<FVTBOWTEditorToolsContextInput>(*this, *TransactionHistory))
	, ContextViewport(MakeUnique<FVTBOWTEditorToolsContextViewport>(*this))
{
}
UVTBOWTEditorToolsContext::~UVTBOWTEditorToolsContext() = default;

void UVTBOWTEditorToolsContext::Initialize(IToolsContextQueriesAPI* InQueriesAPI, IToolsContextTransactionsAPI* InTransactionsAPI)
{
	InitializeInternal(GetWorld(), InQueriesAPI, InTransactionsAPI);
}

bool UVTBOWTEditorToolsContext::InitializeContext(UWorld* InWorld)
{
	InitializeInternal(InWorld, nullptr, nullptr);

	return IsRuntimeReady();
}

void UVTBOWTEditorToolsContext::InitializeInternal(UWorld* InWorld, IToolsContextQueriesAPI* InQueriesAPI,
	IToolsContextTransactionsAPI* InTransactionsAPI)
{
	if (ContextQueriesAPI)
	{
		return;
	}
	{
		TGuardValue<bool> UpdateGuard(bUpdating, true);
		ContextQueriesAPI = MakeUnique<FVTBOWTEditorToolsContextQueriesImpl>(*this);
		ContextQueriesAPI->Initialize(InWorld, InQueriesAPI);
		ContextTransactionAPI = MakeUnique<FVTBOWTEditorToolsContextTransactionImpl>(*this, *TransactionHistory);
		ContextTransactionAPI->Initialize(InTransactionsAPI);
		Super::Initialize(ContextQueriesAPI.Get(), ContextTransactionAPI.Get());
	}
	ContextViewport->ResetRenderCallCount();
	if (bShutdownRequested)
	{
		FinishPendingShutdown();
		return;
	}
	ContextRenderAPI = MakeUnique<FVTBOWTEditorToolsContextRenderImpl>(*this);
	ContextInput->BindApplicationFocus();
}

bool UVTBOWTEditorToolsContext::IsRuntimeReady() const
{
	return ContextRenderAPI != nullptr && !bShutdownRequested;
}

UWorld* UVTBOWTEditorToolsContext::GetEditingWorld() const
{
	return ContextQueriesAPI ? ContextQueriesAPI->GetCurrentEditingWorld() : nullptr;
}

void UVTBOWTEditorToolsContext::Shutdown()
{
	if (!ContextQueriesAPI)
	{
		return;
	}
	bShutdownRequested = true;
	FinishPendingShutdown();
}

void UVTBOWTEditorToolsContext::FinishPendingShutdown()
{
	if (!bShutdownRequested || bUpdating)
	{
		return;
	}
	TGuardValue<bool> UpdateGuard(bUpdating, true);
	ContextRenderAPI.Reset();
	ContextInput->UnbindApplicationFocus();
	TransactionHistory->BeginCancellation();
	ContextInput->FinishPendingCancellation();
	{
		TArray<UObject*> ManagedObjects;
		GetObjectsWithOuter(GizmoManager, ManagedObjects);
		for (UObject* Object : ManagedObjects)
		{
			UCombinedTransformGizmo* Gizmo = Cast<UCombinedTransformGizmo>(Object);
			if (IsValid(Gizmo) && Gizmo->ActiveTarget)
			{
				Gizmo->ClearActiveTarget();
			}
		}
	}
	Super::Shutdown();
	ContextTransactionAPI.Reset();
	TransactionHistory->Reset();
	ContextQueriesAPI.Reset();
	bShutdownRequested = false;
}

void UVTBOWTEditorToolsContext::BeginDestroy()
{
	Shutdown();
	Super::BeginDestroy();
}

bool UVTBOWTEditorToolsContext::RunContextUpdate(TFunctionRef<void()> Action)
{
	if (!IsRuntimeReady() || bUpdating)
	{
		return false;
	}
	RunGuardedContextUpdate(Action);
	return true;
}

void UVTBOWTEditorToolsContext::RunGuardedContextUpdate(TFunctionRef<void()> Action)
{
	{
		TGuardValue<bool> UpdateGuard(bUpdating, true);
		Action();
	}
	FinishPendingShutdown();
}

IToolsContextRenderAPI* UVTBOWTEditorToolsContext::GetContextRenderAPI()
{
	return ContextRenderAPI.Get();
}
IToolsContextQueriesAPI* UVTBOWTEditorToolsContext::GetContextQueriesAPI()
{
	return ContextQueriesAPI.Get();
}
IToolsContextTransactionsAPI* UVTBOWTEditorToolsContext::GetContextTransactionAPI()
{
	return ContextTransactionAPI.Get();
}

void UVTBOWTEditorToolsContext::TickRuntime(float DeltaTime)
{
	RunContextUpdate([&]
	{
		ToolManager->Tick(DeltaTime);
		if (IsRuntimeReady())
		{
			GizmoManager->Tick(DeltaTime);
		}
	});
}

IVTBOWTEditorInput& UVTBOWTEditorToolsContext::GetInput() const
{
	return *ContextInput;
}

IVTBOWTEditorViewport& UVTBOWTEditorToolsContext::GetViewport() const
{
	return *ContextViewport;
}

IVTBOWTEditorSceneState& UVTBOWTEditorToolsContext::GetSceneState() const
{
	return *SceneState;
}

IVTBOWTEditorUndoRedo& UVTBOWTEditorToolsContext::GetUndoRedo() const
{
	return *TransactionHistory;
}

void UVTBOWTEditorToolsContext::UpdateRenderView(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	ContextRenderAPI->UpdateCameraState(View, PDI);
}

void UVTBOWTEditorToolsContext::ResetRenderView()
{
	ContextRenderAPI->ResetView();
}
