#pragma once

#include "CoreMinimal.h"
#include "BaseBehaviors/ClickDragBehavior.h"
// Preserve source compatibility for consumers of the former combined header.
#include "Gizmos/Base/VTBOWTAxisPositionGizmo.h"
#include "Gizmos/Base/VTBOWTAxisAngleGizmo.h"
#include "VTBOWTTransformGizmoBehavior.generated.h"

// Input configuration only. Axis interactions and builders live in their own headers.
UCLASS()
class VTBOWTEDITOR_API UVTBOWTTransformGizmoBehavior : public UClickDragInputBehavior
{
	GENERATED_BODY()

public:
	UVTBOWTTransformGizmoBehavior();
};
