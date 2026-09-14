#include "Input/VTBOWTModifierKeyTrigger.h"
#include "EnhancedPlayerInput.h"
#include "InputCoreTypes.h"

UVTBOWTModifierKeyTrigger::UVTBOWTModifierKeyTrigger() : bRequireControl(false), bRequireShift(false), bDisallowShift(false)
{
}

ETriggerState UVTBOWTModifierKeyTrigger::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                                 FInputActionValue ModifiedValue, float DeltaTime)
{
	if (!PlayerInput || !IsActuated(ModifiedValue))
	{
		return ETriggerState::None;
	}

	const bool bControl = PlayerInput->IsPressed(EKeys::LeftControl) || PlayerInput->IsPressed(EKeys::RightControl);
	const bool bShift = PlayerInput->IsPressed(EKeys::LeftShift) || PlayerInput->IsPressed(EKeys::RightShift);

	if (bRequireControl && !bControl)
	{
		return ETriggerState::None;
	}

	if (bRequireShift && !bShift)
	{
		return ETriggerState::None;
	}

	if (bDisallowShift && bShift)
	{
		return ETriggerState::None;
	}

	return ETriggerState::Triggered;
}
