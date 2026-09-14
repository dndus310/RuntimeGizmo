#pragma once

#include "Commandlets/Commandlet.h"
#include "Modes/VTBOWTObjectEditMode.h"
#include "EnhancedPlayerInput.h"
#include "CreateOWTInputCommandlet.generated.h"

UCLASS()
class UCreateOWTInputCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	UCreateOWTInputCommandlet();
	virtual int32 Main(const FString& Params) override;
};

// A context unknown to the runtime router: tests extension by registration only.
USTRUCT()
struct FOWTValidationContext
{
	GENERATED_BODY()

public:
	FOWTValidationContext() : Message(), Value(0)
	{
	}

public:
	UPROPERTY()
	FString Message;

	UPROPERTY()
	int32 Value;
};

UCLASS(Transient)
class UOWTInputValidationMode : public UVTBOWTObjectEditMode
{
	GENERATED_BODY()

public:
	UOWTInputValidationMode()
	    : LastRequest(), ValidationMessage(), ReceivedCount(0), ValidationCount(0), ValidationValue(0)
	{
	}

	virtual bool ReceiveEditContext_Implementation(const FInstancedStruct& Context) override
	{
		const bool bHandled = Super::ReceiveEditContext_Implementation(Context);
		if (bHandled)
		{
			++ReceivedCount;
			LastRequest = Context;
		}
		return bHandled;
	}

	UFUNCTION()
	void HandleValidation(const FOWTValidationContext& Context)
	{
		++ValidationCount;
		ValidationMessage = Context.Message;
		ValidationValue = Context.Value;
	}

public:
	FInstancedStruct LastRequest;
	FString ValidationMessage;

	int32 ReceivedCount;
	int32 ValidationCount;
	int32 ValidationValue;
};

UCLASS(Transient)
class UOWTValidationPlayerInput : public UEnhancedPlayerInput
{
	GENERATED_BODY()

public:
	void SetHeld(FKey Key, bool bHeld);
};
