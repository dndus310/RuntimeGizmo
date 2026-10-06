#pragma once

#include "Commandlets/Commandlet.h"
#include "CreateOWTLevelCommandlet.generated.h"

UCLASS()
class UCreateOWTLevelCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	UCreateOWTLevelCommandlet();
	virtual int32 Main(const FString& Params) override;
};
