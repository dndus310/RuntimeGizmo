#pragma once

#include "CoreMinimal.h"
#include "Context/IVTBOWTEditorUndoRedo.h"

class FToolCommandChange;
class IVTBOWTEditorContextExecution;

class IVTBOWTEditorTransactionHistory : public IVTBOWTEditorUndoRedo
{
public:
	virtual void BeginUndoTransaction(const FText& Description) = 0;
	virtual void EndUndoTransaction() = 0;
	virtual void AppendChange(UObject* TargetObject, TUniquePtr<FToolCommandChange> Change,
		const FText& Description) = 0;

	virtual void BeginCancellation() = 0;
	virtual void EndCancellation() = 0;
	virtual bool IsCancelling() const = 0;
	virtual bool IsReplaying() const = 0;

	virtual void Reset() = 0;
};

TUniquePtr<IVTBOWTEditorTransactionHistory> CreateVTBOWTEditorTransactionHistory(
	IVTBOWTEditorContextExecution& Execution);
