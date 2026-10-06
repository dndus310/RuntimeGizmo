#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OWTEventTestObject.generated.h"

/** Concrete CoreUObject-only owner/receiver fixture; UObject itself is abstract in UE 5.7. */
UCLASS(Transient, NotBlueprintable)
class UOWTEventTestObject : public UObject
{
	GENERATED_BODY()

public:
	UOWTEventTestObject() : LastEvent(NAME_None), LastJson(), InvocationCount(0)
	{
	}

	UFUNCTION()
	void OnEvent(FName Event, const FString& Json)
	{
		LastEvent = Event;
		LastJson = Json;
		++InvocationCount;
	}

	FName LastEvent;
	FString LastJson;
	int32 InvocationCount;
};
