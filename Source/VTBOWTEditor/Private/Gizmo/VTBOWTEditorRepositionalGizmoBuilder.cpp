#include "Gizmo/VTBOWTEditorRepositionalGizmo.h"
#include "Gizmo/VTBOWTEditorGizmoVisualComponent.h"

#include "BaseGizmos/GizmoViewContext.h"
#include "ContextObjectStore.h"
#include "Engine/World.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolManager.h"

namespace
{
	class FVTBOWTEditorGizmoActorFactory final : public FCombinedTransformGizmoActorFactory
	{
	public:
		explicit FVTBOWTEditorGizmoActorFactory(UGizmoViewContext* ViewContext)
			: FCombinedTransformGizmoActorFactory(ViewContext)
		{
		}

		virtual ACombinedTransformGizmoActor* CreateNewGizmoActor(UWorld* World) const override
		{
			ACombinedTransformGizmoActor* GizmoActor = FCombinedTransformGizmoActorFactory::CreateNewGizmoActor(World);
			UVTBOWTEditorGizmoVisualComponent::FindOrAddTo(GizmoActor);
			return GizmoActor;
		}
	};
}

const FString UVTBOWTEditorRepositionalGizmoBuilder::BuilderIdentifier(TEXT("VTBOWT.Transform"));

UVTBOWTEditorRepositionalGizmoBuilder::UVTBOWTEditorRepositionalGizmoBuilder()
{
	AxisPositionBuilderIdentifier = UInteractiveGizmoManager::DefaultAxisPositionBuilderIdentifier;
	PlanePositionBuilderIdentifier = UInteractiveGizmoManager::DefaultPlanePositionBuilderIdentifier;
	AxisAngleBuilderIdentifier = UInteractiveGizmoManager::DefaultAxisAngleBuilderIdentifier;
	EnabledElements = ETransformGizmoSubElements::TranslateAllAxes
		| ETransformGizmoSubElements::TranslateAllPlanes | ETransformGizmoSubElements::RotateAllAxes
		| ETransformGizmoSubElements::ScaleAllAxes | ETransformGizmoSubElements::ScaleAllPlanes
		| ETransformGizmoSubElements::ScaleUniform;
}

UInteractiveGizmo* UVTBOWTEditorRepositionalGizmoBuilder::BuildGizmo(const FToolBuilderState& SceneState) const
{
	UContextObjectStore* ContextStore = SceneState.ToolManager ? SceneState.ToolManager->GetContextObjectStore() : nullptr;
	UGizmoViewContext* ViewContext = ContextStore ? ContextStore->FindContext<UGizmoViewContext>() : nullptr;
	if (!ensureMsgf(IsValid(SceneState.World), TEXT("Runtime transform gizmo builder is missing a world."))
		|| !ensureMsgf(IsValid(SceneState.GizmoManager), TEXT("Runtime transform gizmo builder is missing a gizmo manager."))
		|| !ensureMsgf(IsValid(ViewContext), TEXT("Runtime transform gizmo builder is missing a view context.")))
	{
		return nullptr;
	}

	UVTBOWTEditorRepositionalGizmo* Gizmo = NewObject<UVTBOWTEditorRepositionalGizmo>(SceneState.GizmoManager);
	Gizmo->SetWorld(SceneState.World);
	TSharedPtr<FCombinedTransformGizmoActorFactory> Factory = GizmoActorBuilder;
	if (!Factory)
	{
		Factory = MakeShared<FVTBOWTEditorGizmoActorFactory>(ViewContext);
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
