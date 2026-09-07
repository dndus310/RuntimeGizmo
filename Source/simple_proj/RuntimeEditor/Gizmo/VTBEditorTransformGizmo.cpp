#include "VTBEditorTransformGizmo.h"

#include "BaseBehaviors/ClickDragBehavior.h"
#include "BaseGizmos/AxisAngleGizmo.h"
#include "BaseGizmos/AxisPositionGizmo.h"
#include "BaseGizmos/GizmoViewContext.h"
#include "BaseGizmos/PlanePositionGizmo.h"
#include "BaseGizmos/ViewAdjustedStaticMeshGizmoComponent.h"
#include "ContextObjectStore.h"
#include "Components/PrimitiveComponent.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolManager.h"

FVTBEditorGizmoBehaviorSettings::FVTBEditorGizmoBehaviorSettings()
	: bRejectAltDrag(true)
	, bRejectCtrlDrag(false)
	, bRejectShiftDrag(false)
{
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
		static const FName FullScaleAxisMeshName(TEXT("GizmoBoxArrowHandle"));
		TArray<UViewAdjustedStaticMeshGizmoComponent*> ViewAdjustedComponents;
		GizmoActor->GetComponents(ViewAdjustedComponents);
		for (UViewAdjustedStaticMeshGizmoComponent* Component : ViewAdjustedComponents)
		{
			const UStaticMesh* Mesh = Component ? Component->GetStaticMesh() : nullptr;
			if (Mesh && Mesh->GetFName() == FullScaleAxisMeshName)
			{
				UpdateCoordSystemFunction(Component, CurrentCoordinateSystem);
			}
		}
	}

	UPrimitiveComponent* ActiveRotationComponent = nullptr;
	for (const FSubGizmoInfo& GizmoInfo : RotationSubGizmos)
	{
		const UAxisAngleGizmo* RotationGizmo = Cast<UAxisAngleGizmo>(GizmoInfo.Gizmo.Get());
		if (RotationGizmo && RotationGizmo->bInInteraction)
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
		if (Component)
		{
			Component->SetVisibility(bRotationMode && (!ActiveRotationComponent || Component == ActiveRotationComponent));
		}
	}
	if (GizmoActor->RotationSphere)
	{
		GizmoActor->RotationSphere->SetVisibility(bRotationMode && !ActiveRotationComponent);
	}
}

void UVTBEditorTransformGizmo::SetActiveTarget(UTransformProxy* Target, IToolContextTransactionProvider* TransactionProvider)
{
	Super::SetActiveTarget(Target, TransactionProvider);
	// All six handle kinds use these three stock sub-gizmo/behavior types. Configure them
	// after ITF builds the complete set rather than overriding each Add*Gizmo function.
	for (UInteractiveGizmo* SubGizmo : ActiveGizmos)
	{
		UClickDragInputBehavior* Behavior = nullptr;
		if (const UAxisPositionGizmo* Axis = Cast<UAxisPositionGizmo>(SubGizmo))
		{
			Behavior = Axis->MouseBehavior;
		}
		else if (const UPlanePositionGizmo* Plane = Cast<UPlanePositionGizmo>(SubGizmo))
		{
			Behavior = Plane->MouseBehavior;
		}
		else if (const UAxisAngleGizmo* Rotation = Cast<UAxisAngleGizmo>(SubGizmo))
		{
			Behavior = Rotation->MouseBehavior;
		}

		if (Behavior)
		{
			Behavior->SetUseLeftMouseButton();
			const TFunction<bool(const FInputDeviceState&)> ExistingCheck = Behavior->ModifierCheckFunc;
			Behavior->ModifierCheckFunc = [Settings = BehaviorSettings, ExistingCheck](const FInputDeviceState& Input)
			{
				return !(Settings.bRejectAltDrag && Input.bAltKeyDown)
					&& !(Settings.bRejectCtrlDrag && Input.bCtrlKeyDown)
					&& !(Settings.bRejectShiftDrag && Input.bShiftKeyDown)
					&& (!ExistingCheck || ExistingCheck(Input));
			};
		}
	}
}

void UVTBEditorTransformGizmo::UpdateBehavior(const FVTBEditorGizmoBehaviorSettings& Settings)
{
	BehaviorSettings = Settings;
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
	if (!ensure(IsValid(SceneState.World) && IsValid(SceneState.GizmoManager) && IsValid(ViewContext)))
	{
		return nullptr;
	}

	UVTBEditorTransformGizmo* Gizmo = NewObject<UVTBEditorTransformGizmo>(SceneState.GizmoManager);
	Gizmo->SetWorld(SceneState.World);
	TSharedPtr<FCombinedTransformGizmoActorFactory> Factory = GizmoActorBuilder;
	if (!Factory)
	{
		Factory = MakeShared<FCombinedTransformGizmoActorFactory>(ViewContext);
		Factory->EnableElements = EnabledElements;
	}
	Gizmo->SetGizmoActorBuilder(Factory);
	Gizmo->SetSubGizmoBuilderIdentifiers(AxisPositionBuilderIdentifier, PlanePositionBuilderIdentifier, AxisAngleBuilderIdentifier);
	Gizmo->UpdateBehavior(BehaviorSettings);
	Gizmo->SetIsNonUniformScaleAllowedFunction([]()
	{
		return true;
	});
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
