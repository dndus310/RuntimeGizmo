#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "VTBOWTModifierKeyTrigger.generated.h"

class UEnhancedPlayerInput;

// Used alongside Pressed on individual IMC mappings, never by the editing system.
UCLASS(meta = (DisplayName = "OWT Modifier Keys"))
class VTBOWTEDITOR_API UVTBOWTModifierKeyTrigger : public UInputTrigger
{
	GENERATED_BODY()

public:
	UVTBOWTModifierKeyTrigger();

	virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput,
	                                                 FInputActionValue ModifiedValue, float DeltaTime) override;
	virtual ETriggerType GetTriggerType_Implementation() const override
	{
		return ETriggerType::Implicit;
	}

public:
	UPROPERTY(EditAnywhere, Category = "Modifiers")
	bool bRequireControl;

	UPROPERTY(EditAnywhere, Category = "Modifiers")
	bool bRequireShift;

	UPROPERTY(EditAnywhere, Category = "Modifiers")
	bool bDisallowShift;
};
