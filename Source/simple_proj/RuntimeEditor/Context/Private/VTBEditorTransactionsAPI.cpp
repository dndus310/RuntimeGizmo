#include "VTBEditorTransactionsAPI.h"

#include "BaseGizmos/TransformProxy.h"

FVTBEditorTransactionsAPI::FVTBEditorTransactionsAPI()
	: HistoryCursor(0)
	, TransactionDepth(0)
	, bCancelling(false)
	, bReplaying(false)
{
}

void FVTBEditorTransactionsAPI::DisplayMessage(const FText&, EToolMessageLevel)
{
}

void FVTBEditorTransactionsAPI::PostInvalidation()
{
}

void FVTBEditorTransactionsAPI::BeginUndoTransaction(const FText&)
{
	if (bReplaying)
	{
		return;
	}
	if (TransactionDepth++ == 0)
	{
		PendingTransaction = FTransaction();
	}
}

void FVTBEditorTransactionsAPI::EndUndoTransaction()
{
	if (bReplaying || TransactionDepth == 0)
	{
		return;
	}

	if (--TransactionDepth != 0)
	{
		return;
	}

	if (bCancelling)
	{
		ApplyTransaction(PendingTransaction, true);
	}
	else
	{
		if (!PendingTransaction.Changes.IsEmpty())
		{
			History.SetNum(HistoryCursor);
			History.Add(MoveTemp(PendingTransaction));
			if (History.Num() > MaxTransactionCount)
			{
				History.RemoveAt(0, History.Num() - MaxTransactionCount);
			}
			HistoryCursor = History.Num();
		}
	}
	PendingTransaction = FTransaction();
}

void FVTBEditorTransactionsAPI::AppendChange(UObject* TargetObject, TUniquePtr<FToolCommandChange> Change, const FText&)
{
	if (bReplaying || !IsValid(TargetObject) || !Change)
	{
		return;
	}
	const bool bStandalone = TransactionDepth == 0;
	if (bStandalone)
	{
		BeginUndoTransaction(FText::GetEmpty());
	}
	FObjectChange& Entry = PendingTransaction.Changes.AddDefaulted_GetRef();
	Entry.Target = TargetObject;
	Entry.RetainedTransformProxy = Cast<UTransformProxy>(TargetObject);
	Entry.Change = MoveTemp(Change);
	if (bStandalone)
	{
		EndUndoTransaction();
	}
}

bool FVTBEditorTransactionsAPI::RequestSelectionChange(const FSelectedObjectsChangeList&)
{
	return false;
}

void FVTBEditorTransactionsAPI::AddReferencedObjects(FReferenceCollector& Collector)
{
	auto CollectTransaction = [&Collector](FTransaction& Transaction)
	{
		for (FObjectChange& Entry : Transaction.Changes)
		{
			Collector.AddReferencedObject(Entry.RetainedTransformProxy);
			if (Entry.Change)
			{
				Entry.Change->AddReferencedObjects(Collector);
			}
		}
	};
	CollectTransaction(PendingTransaction);
	for (FTransaction& Transaction : History)
	{
		CollectTransaction(Transaction);
	}
}

FString FVTBEditorTransactionsAPI::GetReferencerName() const
{
	return TEXT("FVTBEditorTransactionsAPI");
}

void FVTBEditorTransactionsAPI::BeginCancellation()
{
	if (!bReplaying)
	{
		bCancelling = true;
	}
}

void FVTBEditorTransactionsAPI::EndCancellation()
{
	if (bReplaying || !bCancelling)
	{
		return;
	}
	ApplyTransaction(PendingTransaction, true);
	PendingTransaction = FTransaction();
	TransactionDepth = 0;
	bCancelling = false;
}

bool FVTBEditorTransactionsAPI::Undo()
{
	if (!IsHistoryAvailable())
	{
		return false;
	}
	while (HistoryCursor > 0)
	{
		if (ApplyTransaction(History[--HistoryCursor], true))
		{
			return true;
		}
	}
	return false;
}

bool FVTBEditorTransactionsAPI::Redo()
{
	if (!IsHistoryAvailable())
	{
		return false;
	}
	while (HistoryCursor < History.Num())
	{
		if (ApplyTransaction(History[HistoryCursor++], false))
		{
			return true;
		}
	}
	return false;
}

bool FVTBEditorTransactionsAPI::CanUndo() const
{
	if (!IsHistoryAvailable())
	{
		return false;
	}
	for (int32 Index = HistoryCursor - 1; Index >= 0; --Index)
	{
		if (History[Index].HasApplicableChanges())
		{
			return true;
		}
	}
	return false;
}

bool FVTBEditorTransactionsAPI::CanRedo() const
{
	if (!IsHistoryAvailable())
	{
		return false;
	}
	for (int32 Index = HistoryCursor; Index < History.Num(); ++Index)
	{
		if (History[Index].HasApplicableChanges())
		{
			return true;
		}
	}
	return false;
}

bool FVTBEditorTransactionsAPI::IsHistoryAvailable() const
{
	return TransactionDepth == 0 && !bReplaying && !bCancelling;
}

bool FVTBEditorTransactionsAPI::FObjectChange::IsApplicable() const
{
	UObject* Object = Target.Get();
	if (!IsValid(Object) || !Change)
	{
		return false;
	}
	return !Change->HasExpired(Object);
}

bool FVTBEditorTransactionsAPI::FTransaction::HasApplicableChanges() const
{
	return Changes.ContainsByPredicate([](const FObjectChange& Entry) { return Entry.IsApplicable(); });
}

bool FVTBEditorTransactionsAPI::ApplyTransaction(FTransaction& Transaction, bool bRevert)
{
	TGuardValue<bool> ReplayGuard(bReplaying, true);
	bool bApplied = false;
	const int32 Num = Transaction.Changes.Num();
	for (int32 Step = 0; Step < Num; ++Step)
	{
		FObjectChange& Entry = Transaction.Changes[bRevert ? Num - Step - 1 : Step];
		if (Entry.IsApplicable())
		{
			if (bRevert)
			{
				Entry.Change->Revert(Entry.Target.Get());
			}
			else
			{
				Entry.Change->Apply(Entry.Target.Get());
			}
			bApplied = true;
		}
	}
	return bApplied;
}
