#include "Context/VTBOWTEditorTransactionHistory.h"

#include "BaseGizmos/TransformProxy.h"
#include "Context/IVTBOWTEditorContextExecution.h"
#include "InteractiveToolChange.h"
#include "UObject/GCObject.h"

class FVTBOWTEditorTransactionHistory final : public IVTBOWTEditorTransactionHistory, public FGCObject
{
public:
	explicit FVTBOWTEditorTransactionHistory(IVTBOWTEditorContextExecution& InExecution)
		: Execution(InExecution)
	{
	}

	virtual void BeginUndoTransaction(const FText& Description) override
	{
		if (!bReplaying && TransactionDepth++ == 0 && !bCancelling)
		{
			PendingTransaction = FTransaction();
		}
	}

	virtual void EndUndoTransaction() override
	{
		if (bReplaying || TransactionDepth == 0 || --TransactionDepth != 0)
		{
			return;
		}
		if (bCancelling)
		{
			return;
		}
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
		PendingTransaction = FTransaction();
	}

	virtual void AppendChange(UObject* TargetObject, TUniquePtr<FToolCommandChange> Change,
		const FText& Description) override
	{
		if (bReplaying || !IsValid(TargetObject) || !Change)
		{
			return;
		}
		const bool bStandalone = TransactionDepth == 0;
		if (bStandalone)
		{
			BeginUndoTransaction(Description);
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

	virtual void BeginCancellation() override
	{
		bCancelling = true;
	}

	virtual void EndCancellation() override
	{
		ApplyTransaction(PendingTransaction, true);
		PendingTransaction = FTransaction();
		TransactionDepth = 0;
		bCancelling = false;
	}

	virtual bool IsCancelling() const override
	{
		return bCancelling;
	}

	virtual bool IsReplaying() const override
	{
		return bReplaying;
	}

	virtual bool Undo() override
	{
		bool bUndone = false;
		Execution.RunContextUpdate([&]
		{
			if (!IsHistoryAvailable())
			{
				return;
			}
			while (HistoryCursor > 0)
			{
				if (ApplyTransaction(History[--HistoryCursor], true))
				{
					bUndone = true;
					return;
				}
			}
		});
		return bUndone;
	}

	virtual bool Redo() override
	{
		bool bRedone = false;
		Execution.RunContextUpdate([&]
		{
			if (!IsHistoryAvailable())
			{
				return;
			}
			while (HistoryCursor < History.Num())
			{
				if (ApplyTransaction(History[HistoryCursor++], false))
				{
					bRedone = true;
					return;
				}
			}
		});
		return bRedone;
	}

	virtual bool CanUndo() const override
	{
		if (!Execution.IsRuntimeReady() || !IsHistoryAvailable())
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

	virtual bool CanRedo() const override
	{
		if (!Execution.IsRuntimeReady() || !IsHistoryAvailable())
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

	virtual void Reset() override
	{
		History.Reset();
		PendingTransaction = FTransaction();
		HistoryCursor = 0;
		TransactionDepth = 0;
		bCancelling = false;
		bReplaying = false;
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		auto CollectTransaction = [&Collector](FTransaction& Transaction)
		{
			for (FObjectChange& Entry : Transaction.Changes)
			{
				Collector.AddReferencedObject(Entry.RetainedTransformProxy);
				Entry.Change->AddReferencedObjects(Collector);
			}
		};
		CollectTransaction(PendingTransaction);
		for (FTransaction& Transaction : History)
		{
			CollectTransaction(Transaction);
		}
	}

	virtual FString GetReferencerName() const override
	{
		return TEXT("FVTBOWTEditorTransactionHistory");
	}

private:
	struct FObjectChange
	{
		TWeakObjectPtr<UObject> Target;
		TObjectPtr<UObject> RetainedTransformProxy = nullptr;
		TUniquePtr<FToolCommandChange> Change;

		bool IsApplicable() const
		{
			UObject* Object = Target.Get();
			return Object && !Change->HasExpired(Object);
		}
	};

	struct FTransaction
	{
		TArray<FObjectChange> Changes;

		bool HasApplicableChanges() const
		{
			return Changes.ContainsByPredicate([](const FObjectChange& Entry)
			{
				return Entry.IsApplicable();
			});
		}
	};

	bool IsHistoryAvailable() const
	{
		return TransactionDepth == 0 && !bReplaying && !bCancelling;
	}

	bool ApplyTransaction(FTransaction& Transaction, bool bRevert)
	{
		TGuardValue<bool> ReplayGuard(bReplaying, true);
		bool bApplied = false;
		const int32 Num = Transaction.Changes.Num();
		for (int32 Step = 0; Step < Num; ++Step)
		{
			FObjectChange& Entry = Transaction.Changes[bRevert ? Num - Step - 1 : Step];
			if (!Entry.IsApplicable())
			{
				continue;
			}
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
		return bApplied;
	}

	IVTBOWTEditorContextExecution& Execution;
	TArray<FTransaction> History;
	FTransaction PendingTransaction;
	int32 HistoryCursor = 0;
	int32 TransactionDepth = 0;
	bool bCancelling = false;
	bool bReplaying = false;
	static constexpr int32 MaxTransactionCount = 128;
};

TUniquePtr<IVTBOWTEditorTransactionHistory> CreateVTBOWTEditorTransactionHistory(
	IVTBOWTEditorContextExecution& Execution)
{
	return MakeUnique<FVTBOWTEditorTransactionHistory>(Execution);
}
