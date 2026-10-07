#include "Context/OWTAttributeEditSessionContext.h"
#include "Modes/OWTAttributeEditMode.h"

void UOWTAttributeEditSessionContext::Initialize(UOWTAttributeEditMode& InMode)
{
	Mode = &InMode;
}
UOWTAttributeEditMode* UOWTAttributeEditSessionContext::GetMode() const
{
	return Mode.Get();
}
