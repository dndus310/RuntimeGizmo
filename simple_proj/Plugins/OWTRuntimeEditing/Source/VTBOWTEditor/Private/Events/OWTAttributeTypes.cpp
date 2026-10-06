#include "Events/OWTAttributeTypes.h"

FOWTAttributeSnapshot::FOWTAttributeSnapshot()
    : EditorId(), ObjectId(), ObjectName(), ObjectClass(), DisabledReason(), ActiveMode(), GizmoMode(),
      GizmoCoordinateSystem(), SelectionRevision(0), StateRevision(0), bEditingEnabled(false), bHasSelection(false),
      bCanEditTransform(false), bIsModifying(false), bHasChanges(false), Transform(FTransform::Identity)
{
}
