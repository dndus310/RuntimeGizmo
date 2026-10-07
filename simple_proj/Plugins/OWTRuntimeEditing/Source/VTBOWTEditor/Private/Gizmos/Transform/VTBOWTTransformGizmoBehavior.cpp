#include "Gizmos/Base/VTBOWTTransformGizmoBehavior.h"

UVTBOWTTransformGizmoBehavior::UVTBOWTTransformGizmoBehavior()
{
	SetDefaultPriority(FInputCapturePriority(FInputCapturePriority::DEFAULT_GIZMO_PRIORITY));
	SetUseLeftMouseButton();
}
