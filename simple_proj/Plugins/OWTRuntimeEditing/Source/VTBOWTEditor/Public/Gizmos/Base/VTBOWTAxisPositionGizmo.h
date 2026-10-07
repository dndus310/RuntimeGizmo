#pragma once

#include "CoreMinimal.h"
#include "BaseGizmos/AxisPositionGizmo.h"
#include "VTBOWTAxisPositionGizmo.generated.h"

UCLASS()
class VTBOWTEDITOR_API UVTBOWTAxisPositionGizmo : public UAxisPositionGizmo
{
	GENERATED_BODY()

public:
	virtual void Setup() override;
};

UCLASS()
class VTBOWTEDITOR_API UVTBOWTAxisPositionGizmoBuilder : public UInteractiveGizmoBuilder
{
	GENERATED_BODY()

public:
	virtual UInteractiveGizmo* BuildGizmo(const FToolBuilderState& State) const override;
};
