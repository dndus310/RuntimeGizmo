#pragma once

#include "CoreMinimal.h"

class VTBOWTEDITOR_API IVTBOWTEditorContextExecution
{
public:
	virtual ~IVTBOWTEditorContextExecution() = default;

	virtual bool IsRuntimeReady() const = 0;
	virtual bool RunContextUpdate(TFunctionRef<void()> Action) = 0;
};
