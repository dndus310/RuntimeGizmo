#include "Tools/OWTModeToolBuilder.h"
#include "Modes/OWTAttributeEditMode.h"

bool UOWTModeToolBuilder::RequestStart(UOWTAttributeEditMode& Mode, FName ToolId, FString& OutError)
{
	return Mode.StartTool(ToolId, OutError);
}
