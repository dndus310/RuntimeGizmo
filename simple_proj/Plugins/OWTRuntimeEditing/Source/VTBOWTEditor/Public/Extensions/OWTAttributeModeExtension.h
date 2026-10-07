#pragma once

#include "CoreMinimal.h"
#include "Features/IModularFeature.h"
#include "Extensions/OWTToolDescriptor.h"

class UOWTAttributeEditMode;

// A provider owns code and immutable descriptors; each mode owns its registered UObjects.
class VTBOWTEDITOR_API IOWTAttributeModeExtension : public IModularFeature
{
public:
	virtual ~IOWTAttributeModeExtension() = default;
	static FName GetModularFeatureName()
	{
		return TEXT("OWTAttributeModeExtension");
	}
	virtual FName GetProviderId() const = 0;
	virtual void RegisterTools(UOWTAttributeEditMode& Mode) = 0;
};
