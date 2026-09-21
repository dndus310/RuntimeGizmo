#include "Gizmo/VTBOWTEditorRepositionalGizmo.h"

#include "Gizmo/VTBOWTEditorGizmoSelection.h"
#include "Gizmo/VTBOWTEditorGizmoVisualComponent.h"
#include "BaseGizmos/AxisAngleGizmo.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "InteractiveGizmoManager.h"

UVTBOWTEditorRepositionalGizmo::UVTBOWTEditorRepositionalGizmo()
{
	Selection = CreateDefaultSubobject<UVTBOWTEditorGizmoSelection>(TEXT("Selection"));
}

void UVTBOWTEditorRepositionalGizmo::Setup()
{
	Super::Setup();
	if (UVTBOWTEditorGizmoVisualComponent* Visuals = UVTBOWTEditorGizmoVisualComponent::FindOrAddTo(GizmoActor))
	{
		Visuals->ApplyMaterials();
	}
}

void UVTBOWTEditorRepositionalGizmo::Tick(float DeltaTime)
{
	const EToolContextTransformGizmoMode Mode = bUseContextGizmoMode
		? GetGizmoManager()->GetContextQueriesAPI()->GetCurrentTransformGizmoMode() : ActiveGizmoMode;
	{
		TGuardValue<bool> CoordinatePolicy(bUseContextCoordinateSystem,
			bUseContextCoordinateSystem && Mode != EToolContextTransformGizmoMode::Scale);
		if (Mode == EToolContextTransformGizmoMode::Scale)
		{
			CurrentCoordinateSystem = EToolContextCoordinateSystem::Local;
		}
		Super::Tick(DeltaTime);
	}
	if (!IsValid(GizmoActor))
	{
		return;
	}
	UVTBOWTEditorGizmoVisualComponent* Visuals = GizmoActor->FindComponentByClass<UVTBOWTEditorGizmoVisualComponent>();
	if (!Visuals)
	{
		return;
	}
	if (UpdateCoordSystemFunction)
	{
		Visuals->UpdateCoordinateSystem(ActiveComponents, [this](UPrimitiveComponent* Component)
		{
			UpdateCoordSystemFunction(Component, CurrentCoordinateSystem);
		});
	}

	const bool bNonUniformScaleAllowed = CurrentCoordinateSystem == EToolContextCoordinateSystem::Local
		&& IsNonUniformScaleAllowed();
	TArray<TWeakObjectPtr<UPrimitiveComponent>, TInlineAllocator<6>> NonUniformScaleComponents;
	for (const FSubGizmoInfo& GizmoInfo : NonUniformScaleSubGizmos)
	{
		NonUniformScaleComponents.Add(GizmoInfo.Component);
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
	Visuals->Update(ActiveGizmoMode, bNonUniformScaleAllowed,
		ActiveRotationComponent, NonUniformScaleComponents);
}

void UVTBOWTEditorRepositionalGizmo::SetActiveTarget(UTransformProxy* Target, IToolContextTransactionProvider* TransactionProvider)
{
	Super::SetActiveTarget(Target, TransactionProvider);
	Interaction.Configure(*this);
}

void UVTBOWTEditorRepositionalGizmo::Shutdown()
{
	Super::Shutdown();
	Selection->Reset();
}

void UVTBOWTEditorRepositionalGizmo::ClearActiveTarget()
{
	PivotAlignmentGizmos.Reset();
	RepositionStateTarget = nullptr;
	Super::ClearActiveTarget();
	Interaction.Reset();
}

bool UVTBOWTEditorRepositionalGizmo::SetSelection(UWorld* InWorld, const TArray<TWeakObjectPtr<AActor>>& Actors,
	USceneComponent* FrameComponent)
{
	return Selection->SetSelection(InWorld, Actors, FrameComponent);
}

USceneComponent* UVTBOWTEditorRepositionalGizmo::GetSelectionFrame() const
{
	return Selection->GetSelectionFrame();
}

bool UVTBOWTEditorRepositionalGizmo::IsSelectionStateStale() const
{
	return Selection->IsSelectionStateStale();
}

bool UVTBOWTEditorRepositionalGizmo::RefreshSelection()
{
	return Selection->RefreshSelection();
}

void UVTBOWTEditorRepositionalGizmo::RebuildFromCurrentTransforms()
{
	Selection->RebuildFromCurrentTransforms();
}

void UVTBOWTEditorRepositionalGizmo::GetSelectedActors(TArray<AActor*>& OutActors) const
{
	Selection->GetSelectedActors(OutActors);
}

UTransformProxy* UVTBOWTEditorRepositionalGizmo::GetTransformProxy() const
{
	return Selection->GetTransformProxy();
}

bool UVTBOWTEditorRepositionalGizmo::SetEnabledElements(ETransformGizmoSubElements Elements)
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
		UVTBOWTEditorGizmoVisualComponent::FindOrAddTo(GizmoActor)->ApplyMaterials();
		SetVisibility(false);
	}
	return true;
}
