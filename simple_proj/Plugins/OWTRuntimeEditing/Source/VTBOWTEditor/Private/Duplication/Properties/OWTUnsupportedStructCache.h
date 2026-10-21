#pragma once

#include "CoreMinimal.h"

class FProperty;
class UScriptStruct;

/** Finds structs holding native runtime handles. Each struct type is inspected once per operation. */
class FOWTUnsupportedStructCache
{
public:
	FOWTUnsupportedStructCache();
	/** Returns the first struct reachable from Property that requires an explicit adapter, or nullptr. */
	const UScriptStruct* Find(const FProperty& Property);

private:
	const UScriptStruct* FindInStruct(const UScriptStruct& Struct);
	/** Structs whose values are handles into native engine systems; copying them would alias the source. */
	static bool IsNativeHandleStruct(const UScriptStruct& Struct);

private:
	TMap<const UScriptStruct*, const UScriptStruct*> Results;
	TSet<const UScriptStruct*> Visiting;
};
