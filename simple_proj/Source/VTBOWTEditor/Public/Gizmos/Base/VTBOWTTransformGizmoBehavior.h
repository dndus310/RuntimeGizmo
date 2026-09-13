#pragma once

#include "CoreMinimal.h"
#include "BaseBehaviors/ClickDragBehavior.h"
#include "BaseGizmos/AxisPositionGizmo.h"
#include "BaseGizmos/AxisAngleGizmo.h"
#include "VTBOWTTransformGizmoBehavior.generated.h"

class ACombinedTransformGizmoActor;
class UPrimitiveComponent;

UCLASS()
class VTBOWTEDITOR_API UVTBOWTTransformGizmoBehavior : public UClickDragInputBehavior
{
	GENERATED_BODY()

public:
	UVTBOWTTransformGizmoBehavior();
};

UCLASS()
class VTBOWTEDITOR_API UVTBOWTAxisPositionGizmo : public UAxisPositionGizmo
{
	GENERATED_BODY()

public:
	virtual void Setup() override;
};

UCLASS()
class VTBOWTEDITOR_API UVTBOWTAxisAngleGizmo : public UAxisAngleGizmo
{
	GENERATED_BODY()

public:
	UVTBOWTAxisAngleGizmo();

	virtual void Setup() override;
	virtual void OnClickPress(const FInputDeviceRay& PressPos) override;
	virtual void OnClickRelease(const FInputDeviceRay& ReleasePos) override;
	virtual void OnTerminateDragSequence() override;

private:
	void FocusRotationHandle(ACombinedTransformGizmoActor& Actor, UPrimitiveComponent& Handle);
	void ResetRotationFocus();

private:
	TArray<TWeakObjectPtr<UPrimitiveComponent>> SuppressedRotationHandles;
};

UCLASS()
class VTBOWTEDITOR_API UVTBOWTAxisPositionGizmoBuilder : public UInteractiveGizmoBuilder
{
	GENERATED_BODY()

public:
	virtual UInteractiveGizmo* BuildGizmo(const FToolBuilderState& State) const override;
};

UCLASS()
class VTBOWTEDITOR_API UVTBOWTAxisAngleGizmoBuilder : public UInteractiveGizmoBuilder
{
	GENERATED_BODY()

public:
	virtual UInteractiveGizmo* BuildGizmo(const FToolBuilderState& State) const override;
};
