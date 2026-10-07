#pragma once

#include "CoreMinimal.h"
#include "Events/OWTEventTypes.h"
#include "UObject/Object.h"
#include "OWTNotificationCenter.generated.h"

struct FOWTEventSubscription
{
	TWeakObjectPtr<UObject> Receiver;
	FOWTAttributeEventNative Native;
	FOWTAttributeEventDynamic Dynamic;
};

struct FOWTQueuedEvent
{
	FName Event;
	FString Json;
	FGuid Recipient;
};

/**
 * Owner-scoped, game-thread JSON transport. Receivers and the owner are held weakly.
 * The owner calls Shutdown when its session ends (for Actors, from EndPlay).
 */
UCLASS(BlueprintType, NotBlueprintable)
class OWTEVENTCORE_API UOWTNotificationCenter : public UObject
{
	GENERATED_BODY()

public:
	UOWTNotificationCenter();

	/** The center must have been created with Owner as its direct Outer. */
	UFUNCTION(BlueprintCallable, Category = "OWT|Events")
	bool Initialize(UObject* Owner);

	UFUNCTION(BlueprintCallable, Category = "OWT|Events")
	FGuid SubscribeDynamic(UObject* Receiver, const FOWTAttributeEventDynamic& Callback);

	/** Valid JSON only; a non-empty Recipient targets one subscription. Maximum payload: 65536 characters. */
	UFUNCTION(BlueprintCallable, Category = "OWT|Events")
	bool Publish(FName Event, const FString& Json, FGuid Recipient = FGuid());

	/** Journal without delivering. Malformed and truncated input remain visible for request diagnostics. */
	UFUNCTION(BlueprintCallable, Category = "OWT|Events")
	bool RecordEvent(FName Event, const FString& Json, EOWTEventDirection Direction = EOWTEventDirection::Inbound,
	                 FGuid Recipient = FGuid());

	UFUNCTION(BlueprintCallable, Category = "OWT|Events")
	void Shutdown();

	UFUNCTION(BlueprintCallable, Category = "OWT|Events")
	bool Unsubscribe(FGuid Handle);

	UFUNCTION(BlueprintCallable, Category = "OWT|Events")
	void ClearEventHistory();

	UFUNCTION(BlueprintCallable, Category = "OWT|Events")
	void SetHistoryCapacity(int32 Capacity);

	UFUNCTION(BlueprintPure, Category = "OWT|Events")
	bool IsReady() const;

	/** Latest MaximumCount records, returned oldest to newest. */
	UFUNCTION(BlueprintPure, Category = "OWT|Events")
	TArray<FOWTEventRecord> GetRecentEvents(int32 MaximumCount = 256) const;

	UFUNCTION(BlueprintPure, Category = "OWT|Events")
	int64 GetLatestSequence() const;

	/** Changes on append, clear and capacity changes, including when no events are retained. */
	UFUNCTION(BlueprintPure, Category = "OWT|Events")
	int64 GetHistoryRevision() const;

	UFUNCTION(BlueprintPure, Category = "OWT|Events")
	int32 GetHistoryCapacity() const;

	/** Receiver gates the callback lifetime, including for native lambda delegates. */
	FGuid Subscribe(UObject* Receiver, const FOWTAttributeEventNative& Callback);

private:
	void DrainEvents();
	void Deliver(const FOWTQueuedEvent& Queued);
	void AppendRecord(FName Event, const FString& Json, EOWTEventDirection Direction, FGuid Recipient, bool bValidJson);
	void TrimHistory();
	bool CanSubscribe(UObject* Receiver) const;
	static bool IsValidJson(const FString& Json);

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UObject> OwnerObject;

	TMap<FGuid, FOWTEventSubscription> Subscriptions;
	TArray<FOWTQueuedEvent> PendingEvents;
	TArray<FOWTEventRecord> EventHistory;
	int64 LatestSequence;
	int64 HistoryRevision;
	int32 HistoryCapacity;
	bool bDispatching;
};
