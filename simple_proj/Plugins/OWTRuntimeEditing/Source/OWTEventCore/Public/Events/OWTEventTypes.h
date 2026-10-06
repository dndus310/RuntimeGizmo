#pragma once

#include "CoreMinimal.h"
#include "OWTEventTypes.generated.h"

// The existing names remain source compatible for Attribute Editor clients.
DECLARE_DELEGATE_TwoParams(FOWTAttributeEventNative, FName, const FString&);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOWTAttributeEventDynamic, FName, Event, const FString&, Json);

UENUM(BlueprintType)
enum class EOWTEventDirection : uint8
{
	Inbound,
	Outbound
};

/** A diagnostic copy of an event. The original payload is never retained beyond the journal limit. */
USTRUCT(BlueprintType)
struct OWTEVENTCORE_API FOWTEventRecord
{
	GENERATED_BODY()

	FOWTEventRecord();

	UPROPERTY(BlueprintReadOnly, Category = "OWT|Events")
	FDateTime TimestampUtc;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Events")
	FName Event;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Events")
	FString Json;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Events")
	FGuid Recipient;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Events")
	EOWTEventDirection Direction;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Events")
	int64 Sequence;
	/** Whether the original payload was valid JSON, before journal truncation. */
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Events")
	bool bValidJson;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Events")
	bool bPayloadTruncated;
};
