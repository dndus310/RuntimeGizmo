#pragma once

#include "CoreMinimal.h"
#include "OWTGizmoSnapSettings.generated.h"

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTGizmoSnapSettings
{
	GENERATED_BODY()

public:
	FOWTGizmoSnapSettings()
	    : TranslationStep(10.f), RotationStepDegrees(15.f), ScaleStep(0.1f), bTranslationEnabled(true),
	      bRotationEnabled(true), bScaleEnabled(true)
	{
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OWT|Snapping", meta = (ClampMin = "0.001", Units = "cm"))
	float TranslationStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OWT|Snapping", meta = (ClampMin = "0.001", Units = "deg"))
	float RotationStepDegrees;

	// Additive scale increment: 0.1 changes a scale of 1.0 to 1.1.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OWT|Snapping", meta = (ClampMin = "0.001"))
	float ScaleStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OWT|Snapping")
	bool bTranslationEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OWT|Snapping")
	bool bRotationEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OWT|Snapping")
	bool bScaleEnabled;
};
