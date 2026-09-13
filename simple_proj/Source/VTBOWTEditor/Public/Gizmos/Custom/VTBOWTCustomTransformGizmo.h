#pragma once

#include "CoreMinimal.h"
#include "Gizmos/Base/VTBOWTBaseTransformGizmo.h"
#include "VTBOWTCustomTransformGizmo.generated.h"

UCLASS()
class VTBOWTEDITOR_API UVTBOWTCustomTransformGizmoBuilder : public UInteractiveGizmoBuilder
{
	GENERATED_BODY()

public:
	virtual UInteractiveGizmo* BuildGizmo(const FToolBuilderState& SceneState) const override;
};

// Shares target binding and runtime interactions; its builder selects the custom handle actor.
UCLASS()
class VTBOWTEDITOR_API UVTBOWTCustomTransformGizmo : public UVTBOWTBaseTransformGizmo
{
	GENERATED_BODY()
};
