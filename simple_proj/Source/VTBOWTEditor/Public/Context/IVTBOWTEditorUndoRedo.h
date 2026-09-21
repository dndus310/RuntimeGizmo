#pragma once

#include "CoreMinimal.h"

class VTBOWTEDITOR_API IVTBOWTEditorUndoRedo
{
public:
	virtual ~IVTBOWTEditorUndoRedo() = default;

	virtual bool Undo() = 0;
	virtual bool Redo() = 0;
	virtual bool CanUndo() const = 0;
	virtual bool CanRedo() const = 0;
};
