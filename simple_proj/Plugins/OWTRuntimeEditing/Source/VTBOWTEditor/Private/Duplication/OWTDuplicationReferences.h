#pragma once

#include "CoreMinimal.h"

namespace OWTDuplication
{
void VisitReferences(FProperty& Property, void* Value, TFunctionRef<void(UObject*&, bool)> Visitor);
const UScriptStruct* FindUnsupportedStruct(const FProperty& Property, TSet<const UScriptStruct*>& Visited);
} // namespace OWTDuplication
