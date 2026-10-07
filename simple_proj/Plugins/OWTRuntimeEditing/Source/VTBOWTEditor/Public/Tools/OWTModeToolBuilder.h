#pragma once

#include "CoreMinimal.h"
#include "InteractiveToolBuilder.h"
#include "OWTModeToolBuilder.generated.h"

class UOWTAttributeEditMode;

UCLASS(Abstract, Transient)
class VTBOWTEDITOR_API UOWTModeToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool RequestStart(UOWTAttributeEditMode& Mode, FName ToolId, FString& OutError);
};
