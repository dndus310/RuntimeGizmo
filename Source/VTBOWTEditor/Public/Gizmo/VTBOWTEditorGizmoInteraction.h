#pragma once

#include "CoreMinimal.h"

class UInteractiveGizmo;
class UPrimitiveComponent;
class UVTBOWTEditorRepositionalGizmo;

class VTBOWTEDITOR_API FVTBOWTEditorGizmoInteraction
{
public:
	void Configure(UVTBOWTEditorRepositionalGizmo& InGizmo);
	void Reset();

private:
	static void ConfigureSubGizmo(UInteractiveGizmo* SubGizmo);

	TWeakObjectPtr<UPrimitiveComponent> InteractingPrimitive;
	TWeakObjectPtr<UPrimitiveComponent> ReleasedPrimitive;
};
