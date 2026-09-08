#include "VTBEditorTransformGizmo.h"

#include "BaseBehaviors/ClickDragBehavior.h"
#include "BaseGizmos/AxisAngleGizmo.h"
#include "BaseGizmos/AxisPositionGizmo.h"
#include "BaseGizmos/PlanePositionGizmo.h"

void UVTBEditorTransformGizmoBehavior::UpdateSettings(
	bool bInRejectAltDrag,
	bool bInRejectCtrlDrag,
	bool bInRejectShiftDrag)
{
	bRejectAltDrag = bInRejectAltDrag;
	bRejectCtrlDrag = bInRejectCtrlDrag;
	bRejectShiftDrag = bInRejectShiftDrag;
}

void UVTBEditorTransformGizmoBehavior::ConfigureSubGizmo(UInteractiveGizmo* SubGizmo)
{
	UClickDragInputBehavior* MouseBehavior = nullptr;
	UAxisPositionGizmo* AxisGizmo = Cast<UAxisPositionGizmo>(SubGizmo);
	if (AxisGizmo)
	{
		MouseBehavior = AxisGizmo->MouseBehavior;
	}
	else
	{
		UPlanePositionGizmo* PlaneGizmo = Cast<UPlanePositionGizmo>(SubGizmo);
		if (PlaneGizmo)
		{
			MouseBehavior = PlaneGizmo->MouseBehavior;
		}
		else
		{
			UAxisAngleGizmo* RotationGizmo = Cast<UAxisAngleGizmo>(SubGizmo);
			if (RotationGizmo)
			{
				MouseBehavior = RotationGizmo->MouseBehavior;
			}
		}
	}

	if (!IsValid(MouseBehavior))
	{
		return;
	}

	if (!bRejectAltDrag && !bRejectCtrlDrag && !bRejectShiftDrag)
	{
		MouseBehavior->ModifierCheckFunc = nullptr;
		return;
	}

	MouseBehavior->SetUseLeftMouseButton();
	const TFunction<bool(const FInputDeviceState&)> ExistingCheck = MouseBehavior->ModifierCheckFunc;
	const bool RejectAltDrag = bRejectAltDrag;
	const bool RejectCtrlDrag = bRejectCtrlDrag;
	const bool RejectShiftDrag = bRejectShiftDrag;
	MouseBehavior->ModifierCheckFunc = [RejectAltDrag, RejectCtrlDrag, RejectShiftDrag, ExistingCheck](const FInputDeviceState& Input)
	{
		return (!RejectAltDrag || !Input.bAltKeyDown)
			&& (!RejectCtrlDrag || !Input.bCtrlKeyDown)
			&& (!RejectShiftDrag || !Input.bShiftKeyDown)
			&& (!ExistingCheck || ExistingCheck(Input));
	};
}
