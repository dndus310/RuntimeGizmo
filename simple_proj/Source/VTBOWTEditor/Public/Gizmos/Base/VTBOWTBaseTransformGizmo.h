#pragma once

#include "CoreMinimal.h"
#include "BaseGizmos/CombinedTransformGizmo.h"
#include "InteractiveGizmoBuilder.h"
#include "VTBOWTBaseTransformGizmo.generated.h"

class USceneComponent;
class UTransformProxy;

// Registered and owned by the runtime GizmoManager. No LightGizmos/UnrealEd dependency.
UCLASS()
class VTBOWTEDITOR_API UVTBOWTBaseTransformGizmoBuilder : public UInteractiveGizmoBuilder
{
	GENERATED_BODY()

public:
	virtual UInteractiveGizmo* BuildGizmo(const FToolBuilderState& SceneState) const override;
};

// CombinedTransformGizmo supplies runtime handles, hit testing and drag behaviors.
// Its Setup/Shutdown lifecycle is driven exclusively by GizmoManager.
UCLASS()
class VTBOWTEDITOR_API UVTBOWTBaseTransformGizmo : public UCombinedTransformGizmo
{
	GENERATED_BODY()

public:
	void SetTargetComponent(USceneComponent& Component);
	UTransformProxy* GetTransformProxy() const;
};
