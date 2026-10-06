#pragma once

#include "Commandlets/Commandlet.h"
#include "CreateVTBSampleCommandlet.generated.h"

UCLASS()
class UCreateVTBSampleCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	UCreateVTBSampleCommandlet();
	virtual int32 Main(const FString& Params) override;
};
