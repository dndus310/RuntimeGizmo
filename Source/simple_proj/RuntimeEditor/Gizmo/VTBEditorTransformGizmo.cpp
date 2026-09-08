#include "VTBEditorTransformGizmo.h"

#include "BaseGizmos/AxisAngleGizmo.h"
#include "BaseGizmos/GizmoViewContext.h"
#include "BaseGizmos/GizmoRenderingUtil.h"
#include "BaseGizmos/TransformProxy.h"
#include "BaseGizmos/TransformSubGizmoUtil.h"
#include "BaseGizmos/ViewAdjustedStaticMeshGizmoComponent.h"
#include "BaseGizmos/ViewBasedTransformAdjusters.h"
#include "ContextObjectStore.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "RuntimeEditor/Context/VTBEditorInteractiveToolsContext.h"
#include "UObject/SoftObjectPath.h"

namespace
{
	constexpr TCHAR RuntimeGizmoMaterialPath[] = TEXT("/Script/Engine.MaterialInstanceConstant'/Engine/InteractiveToolsFramework/Materials/GizmoComponentMaterial_NotDimmed.GizmoComponentMaterial_NotDimmed'");
	constexpr TCHAR RuntimeGizmoMaterialFallbackPath[] = TEXT("/Engine/InteractiveToolsFramework/Materials/GizmoComponentMaterial_NotDimmed");
	constexpr float CombinedScalePlaneOffset = 120.0f;
	constexpr float ScaleOnlyPlaneOffset = 75.0f;

	ETransformGizmoSubElements GetElementsForSelection(int32 ActorCount)
	{
		ETransformGizmoSubElements Elements = ETransformGizmoSubElements::TranslateAllAxes
			| ETransformGizmoSubElements::TranslateAllPlanes
			| ETransformGizmoSubElements::RotateAllAxes
			| ETransformGizmoSubElements::ScaleUniform;
		const bool bSingleSelection = ActorCount == 1;
		if (bSingleSelection)
		{
			Elements |= ETransformGizmoSubElements::ScaleAllAxes;
			Elements |= ETransformGizmoSubElements::ScaleAllPlanes;
		}
		return Elements;
	}

	bool IsSelectableActor(const AActor* Actor, const UWorld* TargetWorld)
	{
		if (!IsValid(TargetWorld) || !IsValid(Actor))
		{
			return false;
		}

		return !Actor->IsActorBeingDestroyed() && Actor->GetWorld() == TargetWorld;
	}

	UMaterialInterface* LoadRuntimeGizmoMaterial()
	{
		UMaterialInterface* Material = Cast<UMaterialInterface>(FSoftObjectPath(RuntimeGizmoMaterialPath).TryLoad());
		if (!IsValid(Material))
		{
			Material = LoadObject<UMaterialInterface>(nullptr, RuntimeGizmoMaterialFallbackPath);
		}

		return Material;
	}

	void ApplyRuntimeGizmoMaterial(ACombinedTransformGizmoActor* GizmoActor)
	{
		if (!IsValid(GizmoActor))
		{
			return;
		}

		UMaterialInterface* BaseMaterial = LoadRuntimeGizmoMaterial();
		if (!ensureMsgf(IsValid(BaseMaterial), TEXT("Runtime gizmo material could not be loaded.")))
		{
			return;
		}

		TArray<UViewAdjustedStaticMeshGizmoComponent*> Components;
		GizmoActor->GetComponents(Components);
		for (UViewAdjustedStaticMeshGizmoComponent* Component : Components)
		{
			if (!IsValid(Component) || Component->GetNumMaterials() == 0)
			{
				continue;
			}

			FLinearColor GizmoColor = FLinearColor::White;
			UMaterialInstanceDynamic* ExistingMaterial = Cast<UMaterialInstanceDynamic>(Component->GetMaterial(0));
			if (ExistingMaterial)
			{
				GizmoColor = ExistingMaterial->K2_GetVectorParameterValue(TEXT("GizmoColor"));
			}

			UMaterialInstanceDynamic* RuntimeMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, Component);
			if (!ensure(RuntimeMaterial))
			{
				continue;
			}
			RuntimeMaterial->SetVectorParameterValue(TEXT("GizmoColor"), GizmoColor);
			Component->SetAllMaterials(RuntimeMaterial);
		}
	}

	void ApplyStockScalePlaneLayout(ACombinedTransformGizmoActor* GizmoActor, bool bCombinedMode)
	{
		if (!IsValid(GizmoActor) || !IsValid(GizmoActor->GetRootComponent()))
		{
			return;
		}

		const float PlaneOffset = bCombinedMode ? CombinedScalePlaneOffset : ScaleOnlyPlaneOffset;
		const FVector CornerPosition(0.0f, PlaneOffset, PlaneOffset);
		const FVector CornerScale(0.5f);
		const FTransform BaseTransform(FQuat::Identity, CornerPosition, CornerScale);

		const TPair<UPrimitiveComponent*, EAxis::Type> PlaneEntries[] =
		{
			{ GizmoActor->PlaneScaleXY, EAxis::Z },
			{ GizmoActor->PlaneScaleXZ, EAxis::Y },
			{ GizmoActor->PlaneScaleYZ, EAxis::X }
		};
		for (const TPair<UPrimitiveComponent*, EAxis::Type>& Entry : PlaneEntries)
		{
			UViewAdjustedStaticMeshGizmoComponent* Component = Cast<UViewAdjustedStaticMeshGizmoComponent>(Entry.Key);
			if (!IsValid(Component))
			{
				continue;
			}

			const FTransform DesiredTransform = UE::GizmoUtil::GetRotatedBasisTransform(BaseTransform, Entry.Value);
			const bool bTransformChanged = !Component->GetRelativeTransform().Equals(DesiredTransform);
			if (bTransformChanged)
			{
				Component->SetRelativeTransform(DesiredTransform);
			}
			if (bTransformChanged || !Component->GetTransformAdjuster().IsValid())
			{
				UE::GizmoRenderingUtil::FSubGizmoTransformAdjuster::AddTransformAdjuster(
					Component, GizmoActor->GetRootComponent(), bCombinedMode);
			}
		}
	}
}

void UVTBEditorTransformGizmo::Setup()
{
	Behavior = NewObject<UVTBEditorTransformGizmoBehavior>(this);
	if (!ensureMsgf(IsValid(Behavior), TEXT("Transform gizmo behavior could not be created.")))
	{
		return;
	}
	Super::Setup();
	ApplyRuntimeGizmoMaterial(GizmoActor);
}

void UVTBEditorTransformGizmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsValid(GizmoActor))
	{
		return;
	}

	if (UpdateCoordSystemFunction)
	{
		TArray<UViewAdjustedStaticMeshGizmoComponent*> ViewAdjustedComponents;
		GizmoActor->GetComponents(ViewAdjustedComponents);
		for (UViewAdjustedStaticMeshGizmoComponent* Component : ViewAdjustedComponents)
		{
			if (IsValid(Component) && !ActiveComponents.Contains(Component))
			{
				UpdateCoordSystemFunction(Component, CurrentCoordinateSystem);
			}
		}
	}

	const bool bScaleMode = ActiveGizmoMode == EToolContextTransformGizmoMode::Scale;
	const bool bCombinedMode = ActiveGizmoMode == EToolContextTransformGizmoMode::Combined;
	const bool bScaleVisible = bScaleMode || bCombinedMode;
	if (bScaleVisible)
	{
		ApplyStockScalePlaneLayout(GizmoActor, bCombinedMode);
	}

	const bool bLocalCoordinateSystem = CurrentCoordinateSystem == EToolContextCoordinateSystem::Local;
	const bool bCanShowNonUniformScale = bLocalCoordinateSystem && IsNonUniformScaleAllowed();
	for (const FSubGizmoInfo& GizmoInfo : NonUniformScaleSubGizmos)
	{
		UPrimitiveComponent* Component = GizmoInfo.Component.Get();
		if (IsValid(Component))
		{
			Component->SetVisibility(bScaleVisible && bCanShowNonUniformScale);
		}
	}

	UPrimitiveComponent* ActiveRotationComponent = nullptr;
	for (const FSubGizmoInfo& GizmoInfo : RotationSubGizmos)
	{
		const UAxisAngleGizmo* RotationGizmo = Cast<UAxisAngleGizmo>(GizmoInfo.Gizmo.Get());
		if (IsValid(RotationGizmo) && RotationGizmo->bInInteraction)
		{
			ActiveRotationComponent = GizmoInfo.Component.Get();
			break;
		}
	}

	const bool bRotationMode = ActiveGizmoMode == EToolContextTransformGizmoMode::Rotation
		|| ActiveGizmoMode == EToolContextTransformGizmoMode::Combined;
	UPrimitiveComponent* RotationComponents[] = { GizmoActor->RotateX, GizmoActor->RotateY, GizmoActor->RotateZ };
	for (UPrimitiveComponent* Component : RotationComponents)
	{
		if (IsValid(Component))
		{
			Component->SetVisibility(bRotationMode
				&& (ActiveRotationComponent == nullptr || Component == ActiveRotationComponent));
		}
	}
	if (IsValid(GizmoActor->RotationSphere))
	{
		GizmoActor->RotationSphere->SetVisibility(bRotationMode && ActiveRotationComponent == nullptr);
	}
}

void UVTBEditorTransformGizmo::SetActiveTarget(UTransformProxy* Target, IToolContextTransactionProvider* TransactionProvider)
{
	Super::SetActiveTarget(Target, TransactionProvider);
	if (IsValid(Behavior))
	{
		for (UInteractiveGizmo* SubGizmo : ActiveGizmos)
		{
			Behavior->ConfigureSubGizmo(SubGizmo);
		}
	}
}

void UVTBEditorTransformGizmo::Shutdown()
{
	Super::Shutdown();
	if (TransformProxy)
	{
		TransformProxy->OnTransformChanged.RemoveAll(this);
		TransformProxy->OnPivotChanged.RemoveAll(this);
	}
	TransformProxy = nullptr;
	TargetWorld.Reset();
	RequestedActors.Reset();
	TargetComponents.Reset();
	TargetTransforms.Reset();
	Behavior = nullptr;
}

bool UVTBEditorTransformGizmo::SetSelection(UWorld* InWorld, const TArray<TWeakObjectPtr<AActor>>& Actors)
{
	TargetWorld = InWorld;
	RequestedActors.Reset();
	TSet<AActor*> UniqueActors;
	for (const TWeakObjectPtr<AActor>& WeakActor : Actors)
	{
		AActor* Actor = WeakActor.Get();
		if (!IsSelectableActor(Actor, InWorld) || UniqueActors.Contains(Actor))
		{
			continue;
		}
		UniqueActors.Add(Actor);
		RequestedActors.Add(Actor);
	}

	return RebuildTargetIfNeeded();
}

bool UVTBEditorTransformGizmo::RefreshSelection()
{
	RequestedActors.RemoveAll([this](const TWeakObjectPtr<AActor>& Actor)
	{
		return !IsSelectableActor(Actor.Get(), TargetWorld.Get());
	});
	return RebuildTargetIfNeeded();
}

void UVTBEditorTransformGizmo::RebuildFromCurrentTransforms()
{
	RebuildTargetIfNeeded(true);
}

bool UVTBEditorTransformGizmo::ApplyRuntimeState(
	UVTBEditorInteractiveToolsContext* Context,
	EToolContextTransformGizmoMode GizmoMode,
	const TOptional<TArray<TWeakObjectPtr<AActor>>>& SelectionRequest)
{
	if (!ensureMsgf(IsValid(Context), TEXT("Runtime gizmo state requires a valid tools context.")))
	{
		return false;
	}
	if (!ensureMsgf(Context->IsRuntimeReady(), TEXT("Runtime gizmo state requires an initialized tools context.")))
	{
		return false;
	}
	if (!ensureMsgf(IsValid(Context->GizmoManager), TEXT("Runtime gizmo state requires a gizmo manager.")))
	{
		return false;
	}

	const bool bHasSelectionRequest = SelectionRequest.IsSet();
	bool bTargetsChanged = false;
	bool bInteractionCancelled = false;
	if (bHasSelectionRequest)
	{
		Context->CancelActiveInteraction();
		if (!Context->IsRuntimeReady())
		{
			return false;
		}
		SetSelection(Context->GetEditingWorld(), SelectionRequest.GetValue());
		bInteractionCancelled = true;
	}
	else
	{
		bTargetsChanged = RefreshSelection();
	}

	TArray<AActor*> Actors;
	GetSelectedActors(Actors);
	const bool bGizmoEnabled = GizmoMode != EToolContextTransformGizmoMode::NoGizmo;
	const ETransformGizmoSubElements DesiredElements = GetElementsForSelection(Actors.Num());
	UTransformProxy* DesiredTarget = bGizmoEnabled ? TransformProxy : nullptr;
	bool bNeedsApply = bHasSelectionRequest || bTargetsChanged;
	if (bGizmoEnabled && (!IsValid(GizmoActor) || GetGizmoElements() != DesiredElements))
	{
		bNeedsApply = true;
	}
	if (ActiveTarget != DesiredTarget)
	{
		bNeedsApply = true;
	}
	if (!bNeedsApply)
	{
		Context->SetSelection(Actors);
		return true;
	}

	if (!bInteractionCancelled)
	{
		Context->CancelActiveInteraction();
		if (!Context->IsRuntimeReady())
		{
			return false;
		}
	}
	if (bTargetsChanged)
	{
		RebuildFromCurrentTransforms();
	}

	GetSelectedActors(Actors);
	Context->SetSelection(Actors);
	UTransformProxy* Proxy = TransformProxy;
	if (!bGizmoEnabled || Proxy == nullptr)
	{
		if (ActiveTarget != nullptr)
		{
			ClearActiveTarget();
		}
		SetVisibility(false);
		return true;
	}

	const bool bSingleActor = Actors.Num() == 1;
	const ETransformGizmoSubElements Elements = GetElementsForSelection(Actors.Num());
	SetEnabledElements(Elements);
	SetIsNonUniformScaleAllowedFunction([this, bSingleActor]()
	{
		return bSingleActor && CurrentCoordinateSystem == EToolContextCoordinateSystem::Local;
	});
	if (ActiveTarget != Proxy)
	{
		SetActiveTarget(Proxy, Context->GizmoManager);
	}
	return true;
}

void UVTBEditorTransformGizmo::GetSelectedActors(TArray<AActor*>& OutActors) const
{
	OutActors.Reset();
	for (const TWeakObjectPtr<AActor>& WeakActor : RequestedActors)
	{
		AActor* Actor = WeakActor.Get();
		if (IsSelectableActor(Actor, TargetWorld.Get()))
		{
			OutActors.Add(Actor);
		}
	}
}

bool UVTBEditorTransformGizmo::RebuildTargetIfNeeded(bool bForceRebuild)
{
	TArray<USceneComponent*> Candidates;
	TSet<USceneComponent*> CandidateSet;
	for (const TWeakObjectPtr<AActor>& WeakActor : RequestedActors)
	{
		AActor* Actor = WeakActor.Get();
		if (!IsSelectableActor(Actor, TargetWorld.Get()))
		{
			continue;
		}

		USceneComponent* Root = Actor->GetRootComponent();
		if (IsValid(Root) && Root->IsRegistered()
			&& Root->Mobility == EComponentMobility::Movable
			&& !CandidateSet.Contains(Root))
		{
			Candidates.Add(Root);
			CandidateSet.Add(Root);
		}
	}

	TArray<USceneComponent*> Components;
	for (USceneComponent* Candidate : Candidates)
	{
		bool bHasSelectedAncestor = false;
		for (USceneComponent* Parent = Candidate->GetAttachParent(); Parent; Parent = Parent->GetAttachParent())
		{
			if (CandidateSet.Contains(Parent))
			{
				bHasSelectedAncestor = true;
				break;
			}
		}
		// An attached child already follows its selected ancestor. Adding both transforms it twice.
		if (!bHasSelectedAncestor)
		{
			Components.Add(Candidate);
		}
	}

	bool bSameTargets = Components.Num() == TargetComponents.Num();
	for (const TWeakObjectPtr<USceneComponent>& Previous : TargetComponents)
	{
		if (!Previous.IsValid() || !Components.Contains(Previous.Get()))
		{
			bSameTargets = false;
			break;
		}
	}
	bool bSameTransforms = bSameTargets && TargetTransforms.Num() == TargetComponents.Num();
	for (int32 Index = 0; Index < TargetComponents.Num(); ++Index)
	{
		if (!bSameTransforms)
		{
			break;
		}
		bSameTransforms = TargetComponents[Index]->GetComponentTransform().Equals(TargetTransforms[Index]);
	}
	if (bSameTransforms && !bForceRebuild)
	{
		return false;
	}

	if (TransformProxy)
	{
		TransformProxy->OnTransformChanged.RemoveAll(this);
		TransformProxy->OnPivotChanged.RemoveAll(this);
	}
	TransformProxy = nullptr;
	TargetComponents.Reset(Components.Num());
	TargetTransforms.Reset(Components.Num());
	if (!Components.IsEmpty())
	{
		TransformProxy = NewObject<UTransformProxy>(this, NAME_None, RF_Transient);
		for (USceneComponent* Component : Components)
		{
			TargetComponents.Add(Component);
			// Runtime history is supplied by the context; editor UObject::Modify is unnecessary.
			TransformProxy->AddComponent(Component, false);
		}
		TransformProxy->OnTransformChanged.AddUObject(this, &ThisClass::CacheTargetTransforms);
		TransformProxy->OnPivotChanged.AddUObject(this, &ThisClass::CacheTargetTransforms);
		CacheTargetTransforms(TransformProxy, TransformProxy->GetTransform());
	}
	return true;
}

void UVTBEditorTransformGizmo::CacheTargetTransforms(UTransformProxy* Proxy, FTransform)
{
	if (Proxy != TransformProxy)
	{
		return;
	}

	TargetTransforms.Reset(TargetComponents.Num());
	for (const TWeakObjectPtr<USceneComponent>& Component : TargetComponents)
	{
		TargetTransforms.Add(Component.IsValid() ? Component->GetComponentTransform() : FTransform::Identity);
	}
}

bool UVTBEditorTransformGizmo::SetEnabledElements(ETransformGizmoSubElements Elements)
{
	if (IsValid(GizmoActor) && GetGizmoElements() == Elements)
	{
		return false;
	}

	if (!ensureMsgf(GizmoActorBuilder.IsValid() && IsValid(World), TEXT("Transform gizmo actor builder and world must be valid.")))
	{
		return false;
	}
	if (ActiveTarget)
	{
		ClearActiveTarget();
	}
	if (IsValid(GizmoActor))
	{
		GizmoActor->Destroy();
	}
	GizmoActorBuilder->EnableElements = Elements;
	GizmoActor = GizmoActorBuilder->CreateNewGizmoActor(World);
	if (IsValid(GizmoActor))
	{
		ApplyRuntimeGizmoMaterial(GizmoActor);
		SetVisibility(false);
	}
	return true;
}

void UVTBEditorTransformGizmo::UpdateBehavior(bool bInRejectAltDrag, bool bInRejectCtrlDrag, bool bInRejectShiftDrag)
{
	if (IsValid(Behavior))
	{
		Behavior->UpdateSettings(bInRejectAltDrag, bInRejectCtrlDrag, bInRejectShiftDrag);
		for (UInteractiveGizmo* SubGizmo : ActiveGizmos)
		{
			Behavior->ConfigureSubGizmo(SubGizmo);
		}
	}
}

const FString UVTBEditorTransformGizmoBuilder::BuilderIdentifier(TEXT("VTBEditor.TransformGizmo"));

UVTBEditorTransformGizmoBuilder::UVTBEditorTransformGizmoBuilder()
{
	AxisPositionBuilderIdentifier = UInteractiveGizmoManager::DefaultAxisPositionBuilderIdentifier;
	PlanePositionBuilderIdentifier = UInteractiveGizmoManager::DefaultPlanePositionBuilderIdentifier;
	AxisAngleBuilderIdentifier = UInteractiveGizmoManager::DefaultAxisAngleBuilderIdentifier;
	EnabledElements = ETransformGizmoSubElements::TranslateAllAxes
		| ETransformGizmoSubElements::TranslateAllPlanes | ETransformGizmoSubElements::RotateAllAxes
		| ETransformGizmoSubElements::ScaleAllAxes | ETransformGizmoSubElements::ScaleAllPlanes
		| ETransformGizmoSubElements::ScaleUniform;
}

UInteractiveGizmo* UVTBEditorTransformGizmoBuilder::BuildGizmo(const FToolBuilderState& SceneState) const
{
	UContextObjectStore* ContextStore = SceneState.ToolManager ? SceneState.ToolManager->GetContextObjectStore() : nullptr;
	UGizmoViewContext* ViewContext = ContextStore ? ContextStore->FindContext<UGizmoViewContext>() : nullptr;
	if (!ensureMsgf(IsValid(SceneState.World), TEXT("Runtime transform gizmo builder is missing a world."))
		|| !ensureMsgf(IsValid(SceneState.GizmoManager), TEXT("Runtime transform gizmo builder is missing a gizmo manager."))
		|| !ensureMsgf(IsValid(ViewContext), TEXT("Runtime transform gizmo builder is missing a view context.")))
	{
		return nullptr;
	}

	UVTBEditorTransformGizmo* Gizmo = NewObject<UVTBEditorTransformGizmo>(SceneState.GizmoManager);
	Gizmo->SetWorld(SceneState.World);
	TSharedPtr<FCombinedTransformGizmoActorFactory> Factory = GizmoActorBuilder;
	if (!Factory)
	{
		Factory = MakeShared<FCombinedTransformGizmoActorFactory>(ViewContext);
	}
	Factory->EnableElements = EnabledElements;
	Gizmo->SetGizmoActorBuilder(Factory);
	Gizmo->SetSubGizmoBuilderIdentifiers(AxisPositionBuilderIdentifier, PlanePositionBuilderIdentifier, AxisAngleBuilderIdentifier);
	if (UpdateHoverFunction)
	{
		Gizmo->SetUpdateHoverFunction(UpdateHoverFunction);
	}
	if (UpdateCoordSystemFunction)
	{
		Gizmo->SetUpdateCoordSystemFunction(UpdateCoordSystemFunction);
	}
	return Gizmo;
}
