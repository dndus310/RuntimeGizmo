#pragma once

#include "Commandlets/Commandlet.h"
#include "ValidateOWTAttributesCommandlet.generated.h"

UCLASS()
class UValidateOWTAttributesCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	UValidateOWTAttributesCommandlet();
	virtual int32 Main(const FString& Params) override;
};
