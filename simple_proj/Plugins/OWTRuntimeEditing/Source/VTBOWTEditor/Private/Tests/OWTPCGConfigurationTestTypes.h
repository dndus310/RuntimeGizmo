#pragma once

#include "CoreMinimal.h"
#include "OWTPCGConfigurationTestTypes.generated.h"

/** Fields introduced here require no additions to the configuration transfer implementation. */
USTRUCT()
struct FOWTPCGConfigurationTestSchema
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString FutureSetting;

	UPROPERTY(EditAnywhere)
	TArray<int32> FutureWeights;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UObject> FutureReference;

	UPROPERTY(VisibleAnywhere)
	int32 ObservedValue = 0;

	UPROPERTY(EditAnywhere, Transient)
	int32 WorkingValue = 0;

	UPROPERTY()
	int32 InternalValue = 0;
};
