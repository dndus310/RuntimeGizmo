#include "Gizmos/Base/VTBOWTAxisPositionGizmo.h"

#include "Gizmos/Base/VTBOWTTransformGizmoBehavior.h"
#include "InputBehaviorSet.h"
#include "InteractiveGizmoManager.h"

void UVTBOWTAxisPositionGizmo::Setup()
{
	Super::Setup();

	InputBehaviors->Remove(MouseBehavior);
	MouseBehavior = NewObject<UVTBOWTTransformGizmoBehavior>(this);
	MouseBehavior->Initialize(this);
	AddInputBehavior(MouseBehavior);
}

UInteractiveGizmo* UVTBOWTAxisPositionGizmoBuilder::BuildGizmo(const FToolBuilderState& State) const
{
	return NewObject<UVTBOWTAxisPositionGizmo>(State.GizmoManager);
}
