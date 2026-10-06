#include "Gizmos/Base/VTBOWTTransformGizmoBehavior.h"
#include "InputBehaviorSet.h"
#include "InteractiveGizmoManager.h"
#include "BaseGizmos/HitTargets.h"
#include "BaseGizmos/CombinedTransformGizmo.h"
#include "Components/PrimitiveComponent.h"

UVTBOWTTransformGizmoBehavior::UVTBOWTTransformGizmoBehavior()
{
	SetDefaultPriority(FInputCapturePriority(FInputCapturePriority::DEFAULT_GIZMO_PRIORITY));
	SetUseLeftMouseButton();
}

void UVTBOWTAxisPositionGizmo::Setup()
{
	Super::Setup();
	InputBehaviors->Remove(MouseBehavior);
	MouseBehavior = NewObject<UVTBOWTTransformGizmoBehavior>(this);
	MouseBehavior->Initialize(this);
	AddInputBehavior(MouseBehavior);
}

UVTBOWTAxisAngleGizmo::UVTBOWTAxisAngleGizmo() : SuppressedRotationHandles()
{
}

void UVTBOWTAxisAngleGizmo::Setup()
{
	Super::Setup();
	InputBehaviors->Remove(MouseBehavior);
	MouseBehavior = NewObject<UVTBOWTTransformGizmoBehavior>(this);
	MouseBehavior->Initialize(this);
	AddInputBehavior(MouseBehavior);
}

void UVTBOWTAxisAngleGizmo::OnClickPress(const FInputDeviceRay& PressPos)
{
	// Both engine and custom actors expose their rotation handles through this base type.
	UGizmoComponentHitTarget* ComponentTarget = CastChecked<UGizmoComponentHitTarget>(HitTarget.GetObject());
	UPrimitiveComponent* Handle = ComponentTarget->Component;
	check(Handle);
	ACombinedTransformGizmoActor* Actor = CastChecked<ACombinedTransformGizmoActor>(Handle->GetOwner());

	Super::OnClickPress(PressPos);
	FocusRotationHandle(*Actor, *Handle);
}

void UVTBOWTAxisAngleGizmo::OnClickRelease(const FInputDeviceRay& ReleasePos)
{
	Super::OnClickRelease(ReleasePos);
	ResetRotationFocus();
}

void UVTBOWTAxisAngleGizmo::OnTerminateDragSequence()
{
	Super::OnTerminateDragSequence();
	ResetRotationFocus();
}

void UVTBOWTAxisAngleGizmo::FocusRotationHandle(ACombinedTransformGizmoActor& Actor, UPrimitiveComponent& Handle)
{
	check(SuppressedRotationHandles.IsEmpty());
	check(&Handle == Actor.RotateX || &Handle == Actor.RotateY || &Handle == Actor.RotateZ);

	for (UPrimitiveComponent* OtherHandle : {Actor.RotateX.Get(), Actor.RotateY.Get(), Actor.RotateZ.Get()})
	{
		if (!OtherHandle || OtherHandle == &Handle || !OtherHandle->IsVisible())
		{
			continue;
		}

		SuppressedRotationHandles.Add(OtherHandle);
		// Invisible handles are excluded by the engine's click and hover hit targets.
		OtherHandle->SetVisibility(false);
	}
}

void UVTBOWTAxisAngleGizmo::ResetRotationFocus()
{
	for (const TWeakObjectPtr<UPrimitiveComponent>& SuppressedHandle : SuppressedRotationHandles)
	{
		UPrimitiveComponent* Handle = SuppressedHandle.Get();
		if (!Handle)
		{
			continue;
		}

		Handle->SetVisibility(true);
	}
	SuppressedRotationHandles.Reset();
}

UInteractiveGizmo* UVTBOWTAxisPositionGizmoBuilder::BuildGizmo(const FToolBuilderState& State) const
{
	return NewObject<UVTBOWTAxisPositionGizmo>(State.GizmoManager);
}

UInteractiveGizmo* UVTBOWTAxisAngleGizmoBuilder::BuildGizmo(const FToolBuilderState& State) const
{
	return NewObject<UVTBOWTAxisAngleGizmo>(State.GizmoManager);
}
