#pragma once

#include "CoreMinimal.h"

class FProperty;

/** An owned copy of one reflected property value, independent of the source container's lifetime. */
class FOWTPropertyValue
{
public:
	FOWTPropertyValue(FProperty& InProperty, const void* SourceContainer);
	~FOWTPropertyValue();
	FOWTPropertyValue(const FOWTPropertyValue&) = delete;
	FOWTPropertyValue& operator=(const FOWTPropertyValue&) = delete;
	/** Copies the value into DestinationContainer and remaps references found in Mapping. */
	void ApplyTo(void* DestinationContainer, const TMap<UObject*, UObject*>& Mapping) const;
	FProperty& GetProperty() const;
	void* GetValue() const;

private:
	FProperty& Property;
	void* Value;
};
