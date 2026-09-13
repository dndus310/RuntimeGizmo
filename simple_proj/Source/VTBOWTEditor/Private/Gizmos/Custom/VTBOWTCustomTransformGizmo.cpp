#include "Gizmos/Custom/VTBOWTCustomTransformGizmo.h"
#include "Gizmos/Custom/VTBOWTTransformGizmoActor.h"

#include "BaseGizmos/GizmoViewContext.h"
#include "ContextObjectStore.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolManager.h"

UInteractiveGizmo* UVTBOWTCustomTransformGizmoBuilder::BuildGizmo(const FToolBuilderState& SceneState) const
{
	check(SceneState.World && SceneState.ToolManager && SceneState.GizmoManager);
	UGizmoViewContext* ViewContext = SceneState.ToolManager->GetContextObjectStore()->FindContext<UGizmoViewContext>();
	check(ViewContext);

	UVTBOWTCustomTransformGizmo* Gizmo = NewObject<UVTBOWTCustomTransformGizmo>(SceneState.GizmoManager);
	Gizmo->SetWorld(SceneState.World);
	Gizmo->SetGizmoActorBuilder(MakeShared<FVTBOWTTransformGizmoActorFactory>(*ViewContext));
	Gizmo->SetSubGizmoBuilderIdentifiers(TEXT("OWT.AxisPosition"),
	                                     UInteractiveGizmoManager::DefaultPlanePositionBuilderIdentifier,
	                                     TEXT("OWT.AxisAngle"));
	return Gizmo;
}
