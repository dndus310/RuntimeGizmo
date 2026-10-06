#include "Events/OWTNotificationCenter.h"

#include "Dom/JsonValue.h"
#include "Serialization/JsonSerializer.h"

namespace
{
constexpr int32 MaximumDispatchEvents = 256;
constexpr int32 MaximumTransportCharacters = 65536;
constexpr int32 MaximumJournalCharacters = 16384;
constexpr int32 MaximumHistoryCapacity = 1024;
} // namespace

UOWTNotificationCenter::UOWTNotificationCenter()
    : OwnerObject(), Subscriptions(), PendingEvents(), EventHistory(), LatestSequence(0), HistoryRevision(0),
      HistoryCapacity(256), bDispatching(false)
{
}

bool UOWTNotificationCenter::Initialize(UObject* Owner)
{
	if (!IsInGameThread())
	{
		return false;
	}
	if (!IsValid(Owner))
	{
		return false;
	}
	if (Owner->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed))
	{
		return false;
	}
	if (GetOuter() != Owner)
	{
		return false;
	}
	if (bDispatching)
	{
		return false;
	}
	if (OwnerObject.IsValid())
	{
		return OwnerObject.Get() == Owner;
	}

	OwnerObject = Owner;
	return true;
}

FGuid UOWTNotificationCenter::SubscribeDynamic(UObject* Receiver, const FOWTAttributeEventDynamic& Callback)
{
	if (!IsReady())
	{
		return {};
	}
	if (!CanSubscribe(Receiver))
	{
		return {};
	}
	if (!Callback.IsBound())
	{
		return {};
	}
	if (Callback.GetUObject() != Receiver)
	{
		return {};
	}

	const FGuid Handle = FGuid::NewGuid();
	FOWTEventSubscription Subscription;
	Subscription.Receiver = Receiver;
	Subscription.Dynamic = Callback;
	Subscriptions.Add(Handle, MoveTemp(Subscription));
	return Handle;
}

bool UOWTNotificationCenter::Publish(FName Event, const FString& Json, FGuid Recipient)
{
	if (!IsReady())
	{
		return false;
	}
	if (Event.IsNone())
	{
		return false;
	}
	if (Json.Len() > MaximumTransportCharacters)
	{
		return false;
	}
	if (!IsValidJson(Json))
	{
		return false;
	}
	if (PendingEvents.Num() >= MaximumDispatchEvents)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT event dispatch limit exceeded; nested event discarded."));
		return false;
	}

	AppendRecord(Event, Json, EOWTEventDirection::Outbound, Recipient, true);
	PendingEvents.Add({Event, Json, Recipient});
	if (!bDispatching)
	{
		DrainEvents();
	}
	return true;
}

bool UOWTNotificationCenter::RecordEvent(FName Event, const FString& Json, EOWTEventDirection Direction,
                                         FGuid Recipient)
{
	if (!IsReady())
	{
		return false;
	}

	AppendRecord(Event, Json, Direction, Recipient, IsValidJson(Json));
	return true;
}

void UOWTNotificationCenter::Shutdown()
{
	if (!IsInGameThread())
	{
		return;
	}

	OwnerObject.Reset();
	Subscriptions.Empty();
	PendingEvents.Empty();
	ClearEventHistory();
}

bool UOWTNotificationCenter::Unsubscribe(FGuid Handle)
{
	if (!IsInGameThread())
	{
		return false;
	}

	return Subscriptions.Remove(Handle) > 0;
}

void UOWTNotificationCenter::ClearEventHistory()
{
	if (!IsInGameThread())
	{
		return;
	}

	EventHistory.Empty();
	++HistoryRevision;
}

void UOWTNotificationCenter::SetHistoryCapacity(int32 Capacity)
{
	if (!IsInGameThread())
	{
		return;
	}

	HistoryCapacity = FMath::Clamp(Capacity, 0, MaximumHistoryCapacity);
	TrimHistory();
	++HistoryRevision;
}

bool UOWTNotificationCenter::IsReady() const
{
	if (!IsInGameThread())
	{
		return false;
	}
	if (!OwnerObject.IsValid())
	{
		return false;
	}

	check(GetOuter() == OwnerObject.Get());
	return !OwnerObject->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed);
}

TArray<FOWTEventRecord> UOWTNotificationCenter::GetRecentEvents(int32 MaximumCount) const
{
	if (!IsInGameThread())
	{
		return {};
	}
	if (MaximumCount <= 0)
	{
		return {};
	}

	const int32 Count = FMath::Min(MaximumCount, EventHistory.Num());
	TArray<FOWTEventRecord> Records;
	Records.Reserve(Count);
	for (int32 Index = EventHistory.Num() - Count; Index < EventHistory.Num(); ++Index)
	{
		Records.Add(EventHistory[Index]);
	}
	return Records;
}

int64 UOWTNotificationCenter::GetLatestSequence() const
{
	return IsInGameThread() ? LatestSequence : 0;
}

int64 UOWTNotificationCenter::GetHistoryRevision() const
{
	return IsInGameThread() ? HistoryRevision : 0;
}

int32 UOWTNotificationCenter::GetHistoryCapacity() const
{
	return IsInGameThread() ? HistoryCapacity : 0;
}

FGuid UOWTNotificationCenter::Subscribe(UObject* Receiver, const FOWTAttributeEventNative& Callback)
{
	if (!IsReady())
	{
		return {};
	}
	if (!CanSubscribe(Receiver))
	{
		return {};
	}
	if (!Callback.IsBound())
	{
		return {};
	}

	const FGuid Handle = FGuid::NewGuid();
	FOWTEventSubscription Subscription;
	Subscription.Receiver = Receiver;
	Subscription.Native = Callback;
	Subscriptions.Add(Handle, MoveTemp(Subscription));
	return Handle;
}

void UOWTNotificationCenter::DrainEvents()
{
	check(IsInGameThread());
	TGuardValue<bool> DispatchGuard(bDispatching, true);
	int32 EventIndex = 0;
	while (EventIndex < PendingEvents.Num())
	{
		if (!IsReady())
		{
			break;
		}

		const FOWTQueuedEvent Queued = PendingEvents[EventIndex++];
		Deliver(Queued);
	}
	PendingEvents.Empty();
}

void UOWTNotificationCenter::Deliver(const FOWTQueuedEvent& Queued)
{
	TArray<FGuid> Handles;
	if (Queued.Recipient.IsValid())
	{
		Handles.Add(Queued.Recipient);
	}
	else
	{
		Subscriptions.GenerateKeyArray(Handles);
	}

	for (const FGuid& Handle : Handles)
	{
		if (!IsReady())
		{
			break;
		}
		const FOWTEventSubscription* Found = Subscriptions.Find(Handle);
		if (!Found)
		{
			continue;
		}
		if (!Found->Receiver.IsValid())
		{
			Subscriptions.Remove(Handle);
			continue;
		}
		if (Found->Receiver->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed))
		{
			Subscriptions.Remove(Handle);
			continue;
		}

		// User callbacks may remove themselves, subscribe, shut down or enqueue another event.
		const FOWTEventSubscription Subscription = *Found;
		if (Subscription.Native.IsBound())
		{
			Subscription.Native.Execute(Queued.Event, Queued.Json);
		}
		else if (Subscription.Dynamic.IsBound())
		{
			Subscription.Dynamic.Execute(Queued.Event, Queued.Json);
		}
	}
}

void UOWTNotificationCenter::AppendRecord(FName Event, const FString& Json, EOWTEventDirection Direction,
                                          FGuid Recipient, bool bValidJson)
{
	check(IsInGameThread());
	++LatestSequence;
	++HistoryRevision;
	if (HistoryCapacity == 0)
	{
		return;
	}

	FOWTEventRecord Record;
	Record.TimestampUtc = FDateTime::UtcNow();
	Record.Event = Event;
	Record.Json = Json.Left(MaximumJournalCharacters);
	Record.Recipient = Recipient;
	Record.Direction = Direction;
	Record.Sequence = LatestSequence;
	Record.bValidJson = bValidJson;
	Record.bPayloadTruncated = Json.Len() > MaximumJournalCharacters;
	EventHistory.Add(MoveTemp(Record));
	TrimHistory();
}

void UOWTNotificationCenter::TrimHistory()
{
	const int32 Excess = EventHistory.Num() - HistoryCapacity;
	if (Excess > 0)
	{
		EventHistory.RemoveAt(0, Excess, EAllowShrinking::No);
	}
}

bool UOWTNotificationCenter::CanSubscribe(UObject* Receiver) const
{
	if (!IsValid(Receiver))
	{
		return false;
	}
	if (Receiver->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed))
	{
		return false;
	}

	check(OwnerObject.IsValid());
#if WITH_ENGINE
	const UWorld* OwnerWorld = OwnerObject->GetWorld();
	const UWorld* ReceiverWorld = Receiver->GetWorld();
	if (!OwnerWorld)
	{
		return true;
	}
	if (!ReceiverWorld)
	{
		return true;
	}
	return OwnerWorld == ReceiverWorld;
#else
	return true;
#endif
}

bool UOWTNotificationCenter::IsValidJson(const FString& Json)
{
	// UE's serializer requires an object/array root; a single-element array also validates JSON scalars.
	FString WrappedJson;
	WrappedJson.Reserve(Json.Len() + 2);
	WrappedJson.AppendChar(TEXT('['));
	WrappedJson.Append(Json);
	WrappedJson.AppendChar(TEXT(']'));
	TArray<TSharedPtr<FJsonValue>> Values;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(WrappedJson), Values))
	{
		return false;
	}
	if (Values.Num() != 1)
	{
		return false;
	}
	return Values[0].IsValid();
}
