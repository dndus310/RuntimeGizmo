#pragma once

#include "CoreMinimal.h"

struct FInputDeviceState;

class VTBOWTEDITOR_API IVTBOWTEditorInput
{
public:
	virtual ~IVTBOWTEditorInput() = default;

	virtual bool PostPointerInput(const FInputDeviceState& Input, bool bHover) = 0;
	virtual void CancelActiveInteraction() = 0;
	virtual bool HasActiveMouseCapture() const = 0;
};
