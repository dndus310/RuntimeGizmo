#pragma once

#include "CoreMinimal.h"
#include "ToolContextInterfaces.h"

class VTBOWTEDITOR_API IVTBOWTEditorSceneState
{
public:
	virtual ~IVTBOWTEditorSceneState() = default;

	virtual void SetActorSelection(const TArray<AActor*>& Actors) = 0;
	virtual void SetSelection(const TArray<AActor*>& Actors, const TArray<UActorComponent*>& Components) = 0;
	virtual void SetCoordinateSystem(EToolContextCoordinateSystem CoordinateSystem) = 0;
	virtual void SetGizmoMode(EToolContextTransformGizmoMode Mode) = 0;
	virtual EToolContextTransformGizmoMode GetGizmoMode() const = 0;
};
