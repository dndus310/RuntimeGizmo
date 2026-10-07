#pragma once

#include "Commandlets/Commandlet.h"
#include "CreateOWTPlaygroundCommandlet.generated.h"

/** Creates the opt-in OWT showroom and stress assets without changing existing sample levels. */
UCLASS()
class UCreateOWTPlaygroundCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	UCreateOWTPlaygroundCommandlet();
	virtual int32 Main(const FString& Params) override;
};
