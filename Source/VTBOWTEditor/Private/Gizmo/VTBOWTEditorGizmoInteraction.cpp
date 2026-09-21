#include "Gizmo/VTBOWTEditorGizmoInteraction.h"

#include "Gizmo/VTBOWTEditorRepositionalGizmo.h"
#include "BaseBehaviors/ClickDragBehavior.h"
#include "BaseGizmos/AxisAngleGizmo.h"
#include "BaseGizmos/AxisPositionGizmo.h"
#include "BaseGizmos/PlanePositionGizmo.h"
#include "BaseGizmos/HitTargets.h"
#include "Components/PrimitiveComponent.h"

void FVTBOWTEditorGizmoInteraction::Configure(UVTBOWTEditorRepositionalGizmo& InGizmo)
{
	Reset();
	for (UInteractiveGizmo* SubGizmo : InGizmo.ActiveGizmos)
	{
		ConfigureSubGizmo(SubGizmo);
		if (InGizmo.PivotAlignmentGizmos.Contains(SubGizmo))
		{
			continue;
		}
		UObject* HitObject = nullptr;
		if (const auto* Axis = Cast<UAxisPositionGizmo>(SubGizmo))
		{
			HitObject = Axis->HitTarget.GetObject();
		}
		else if (const auto* Plane = Cast<UPlanePositionGizmo>(SubGizmo))
		{
			HitObject = Plane->HitTarget.GetObject();
		}
		else if (const auto* Angle = Cast<UAxisAngleGizmo>(SubGizmo))
		{
			HitObject = Angle->HitTarget.GetObject();
		}
		UGizmoComponentHitTarget* Hit = Cast<UGizmoComponentHitTarget>(HitObject);
		if (!Hit)
		{
			continue;
		}
		const TWeakObjectPtr<UVTBOWTEditorRepositionalGizmo> WeakThis(&InGizmo);
		const TWeakObjectPtr<UGizmoComponentHitTarget> WeakHit(Hit);
		const auto Hover = [](const UVTBOWTEditorRepositionalGizmo* Gizmo,
			const UGizmoComponentHitTarget* TargetHit, bool bHovering)
		{
			if (Gizmo->UpdateHoverFunction)
			{
				Gizmo->UpdateHoverFunction(TargetHit->Component, bHovering);
			}
		};
		const auto Interaction = Hit->UpdateInteractingFunction;
		Hit->UpdateHoverFunction = [WeakThis, WeakHit, Hover](bool bHovering)
		{
			const auto* TargetHit = WeakHit.Get();
			auto* Gizmo = WeakThis.Get();
			if (!Gizmo || !TargetHit)
			{
				return;
			}
			UPrimitiveComponent* Primitive = TargetHit->Component;
			if (Gizmo->Interaction.InteractingPrimitive.Get() == Primitive)
			{
				Hover(Gizmo, TargetHit, true);
				return;
			}
			if (Gizmo->Interaction.ReleasedPrimitive.Get() == Primitive)
			{
				if (!bHovering)
				{
					Gizmo->Interaction.ReleasedPrimitive.Reset();
				}
				Hover(Gizmo, TargetHit, false);
				return;
			}
			Hover(Gizmo, TargetHit, bHovering);
		};
		Hit->UpdateInteractingFunction = [WeakThis, WeakHit, Hover, Interaction](bool bInteracting)
		{
			if (Interaction)
			{
				Interaction(bInteracting);
			}
			auto* Gizmo = WeakThis.Get();
			const auto* TargetHit = WeakHit.Get();
			if (!Gizmo || !TargetHit)
			{
				return;
			}
			if (bInteracting)
			{
				Gizmo->Interaction.InteractingPrimitive = TargetHit->Component;
				Gizmo->Interaction.ReleasedPrimitive.Reset();
			}
			else
			{
				Gizmo->Interaction.InteractingPrimitive.Reset();
				Gizmo->Interaction.ReleasedPrimitive = TargetHit->Component;
			}
			Hover(Gizmo, TargetHit, bInteracting);
		};
	}
}

void FVTBOWTEditorGizmoInteraction::Reset()
{
	InteractingPrimitive.Reset();
	ReleasedPrimitive.Reset();
}

void FVTBOWTEditorGizmoInteraction::ConfigureSubGizmo(UInteractiveGizmo* SubGizmo)
{
	UClickDragInputBehavior* MouseBehavior = nullptr;
	if (UAxisPositionGizmo* Axis = Cast<UAxisPositionGizmo>(SubGizmo))
	{
		MouseBehavior = Axis->MouseBehavior;
	}
	else if (UPlanePositionGizmo* Plane = Cast<UPlanePositionGizmo>(SubGizmo))
	{
		MouseBehavior = Plane->MouseBehavior;
	}
	else if (UAxisAngleGizmo* Rotation = Cast<UAxisAngleGizmo>(SubGizmo))
	{
		MouseBehavior = Rotation->MouseBehavior;
	}
	if (MouseBehavior)
	{
		const auto OriginalCheck = MouseBehavior->ModifierCheckFunc;
		MouseBehavior->ModifierCheckFunc = [OriginalCheck](const FInputDeviceState& Input)
		{
			return !Input.bAltKeyDown && (!OriginalCheck || OriginalCheck(Input));
		};
	}
}
