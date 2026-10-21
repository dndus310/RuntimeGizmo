#pragma once

#include "CoreMinimal.h"

class FArrayProperty;
class FInterfaceProperty;
class FMapProperty;
class FObjectPropertyBase;
class FProperty;
class FSetProperty;
class FStructProperty;

/**
 * Visits every object reference stored in a property value, including nested containers and structs.
 * The visitor may replace a reference in place; its second argument reports instanced ownership.
 * Delegates, timer handles and excluded struct fields are cleared because they alias source runtime state.
 */
class FOWTReferenceVisitor
{
public:
	static void Visit(FProperty& Property, void* Value, TFunctionRef<void(UObject*&, bool)> Visitor);

private:
	static void VisitElement(FProperty& Property, void* Element, TFunctionRef<void(UObject*&, bool)> Visitor);
	static void VisitObject(FObjectPropertyBase& Property, void* Element, bool bInstanced, TFunctionRef<void(UObject*&, bool)> Visitor);
	static void VisitInterface(FInterfaceProperty& Property, void* Element, TFunctionRef<void(UObject*&, bool)> Visitor);
	static void VisitStruct(FStructProperty& Property, void* Element, TFunctionRef<void(UObject*&, bool)> Visitor);
	static void VisitArray(FArrayProperty& Property, void* Element, TFunctionRef<void(UObject*&, bool)> Visitor);
	static void VisitSet(FSetProperty& Property, void* Element, TFunctionRef<void(UObject*&, bool)> Visitor);
	static void VisitMap(FMapProperty& Property, void* Element, TFunctionRef<void(UObject*&, bool)> Visitor);
};
