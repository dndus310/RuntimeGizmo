#pragma once

#include "CoreMinimal.h"

class FOWTDuplicationPropertyPolicySet;
class FProperty;

/** Separates authored engine-class properties from lifecycle state the core restores through engine APIs. */
class FOWTEnginePropertyPolicies
{
public:
	static void Register(FOWTDuplicationPropertyPolicySet& Policies);

private:
	static TOptional<bool> ResolveObjectProperty(const UObject& Object, const FProperty& Property);
	static TOptional<bool> ResolveActorProperty(const UObject& Object, const FProperty& Property);
	static TOptional<bool> ResolveComponentProperty(const UObject& Object, const FProperty& Property);
	static TOptional<bool> ResolveSceneProperty(const UObject& Object, const FProperty& Property);
	static TOptional<bool> ResolveMaterialProperty(const UObject& Object, const FProperty& Property);
};
