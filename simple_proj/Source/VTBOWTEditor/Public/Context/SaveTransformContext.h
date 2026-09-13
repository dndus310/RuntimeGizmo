#pragma once

#include "CoreMinimal.h"
#include "SaveTransformContext.generated.h"

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FSaveTransformContext
{
	GENERATED_BODY()

public:
	FSaveTransformContext() : SaveTransform(FTransform::Identity), ObjectID(), ModeName(TEXT("Transform"))
	{
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History")
	FTransform SaveTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History")
	FString ObjectID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History")
	FString ModeName;
};
