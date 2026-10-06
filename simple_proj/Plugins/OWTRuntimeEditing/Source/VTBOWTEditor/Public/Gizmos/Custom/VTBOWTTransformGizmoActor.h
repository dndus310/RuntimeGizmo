#pragma once

#include "CoreMinimal.h"
#include "BaseGizmos/CombinedTransformGizmo.h"
#include "VTBOWTTransformGizmoActor.generated.h"

class UGizmoViewContext;

UCLASS(Transient, NotPlaceable)
class VTBOWTEDITOR_API AVTBOWTTransformGizmoActor : public ACombinedTransformGizmoActor
{
	GENERATED_BODY()

public:
	void InitializeHandles(UGizmoViewContext& ViewContext);

private:
	void CreateTranslationHandles(UGizmoViewContext& ViewContext);
	void CreateRotationHandles(UGizmoViewContext& ViewContext);
	void CreateScaleHandles(UGizmoViewContext& ViewContext);
};

class FVTBOWTTransformGizmoActorFactory : public FCombinedTransformGizmoActorFactory
{
public:
	explicit FVTBOWTTransformGizmoActorFactory(UGizmoViewContext& ViewContext);
	virtual ACombinedTransformGizmoActor* CreateNewGizmoActor(UWorld* World) const override;
};
