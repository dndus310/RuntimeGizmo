#pragma once

#include "CoreMinimal.h"
#include "InteractiveToolChange.h"
#include "ToolContextInterfaces.h"
#include "UObject/GCObject.h"

class FVTBEditorTransactionsAPI final : public IToolsContextTransactionsAPI, public FGCObject
{
public:
	FVTBEditorTransactionsAPI();
	virtual ~FVTBEditorTransactionsAPI() override = default;

	virtual void DisplayMessage(const FText& Message, EToolMessageLevel Level) override;
	virtual void PostInvalidation() override;
	virtual void BeginUndoTransaction(const FText&) override;
	virtual void EndUndoTransaction() override;
	virtual void AppendChange(UObject* TargetObject, TUniquePtr<FToolCommandChange> Change, const FText&) override;
	virtual bool RequestSelectionChange(const FSelectedObjectsChangeList& SelectionChange) override;

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override;

	void BeginCancellation();
	void EndCancellation();
	bool Undo();
	bool Redo();
	bool CanUndo() const;
	bool CanRedo() const;

private:
	struct FObjectChange
	{
		TWeakObjectPtr<UObject> Target;
		TObjectPtr<UObject> RetainedTransformProxy;
		TUniquePtr<FToolCommandChange> Change;
		bool IsApplicable() const;
	};

	struct FTransaction
	{
		TArray<FObjectChange> Changes;
		bool HasApplicableChanges() const;
	};

	bool IsHistoryAvailable() const;
	bool ApplyTransaction(FTransaction& Transaction, bool bRevert);

	TArray<FTransaction> History;
	FTransaction PendingTransaction;
	int32 HistoryCursor;
	int32 TransactionDepth;
	bool bCancelling;
	bool bReplaying;
	static constexpr int32 MaxTransactionCount = 128;
};
