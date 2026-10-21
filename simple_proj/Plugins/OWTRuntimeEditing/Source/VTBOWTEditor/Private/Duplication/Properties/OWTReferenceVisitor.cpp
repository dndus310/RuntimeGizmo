#include "Duplication/Properties/OWTReferenceVisitor.h"

#include "Duplication/Properties/OWTPropertyFilter.h"
#include "Engine/TimerHandle.h"
#include "UObject/UnrealType.h"

void FOWTReferenceVisitor::Visit(FProperty& Property, void* Value, TFunctionRef<void(UObject*&, bool)> Visitor)
{
	for (int32 ArrayIndex = 0; ArrayIndex < Property.ArrayDim; ++ArrayIndex)
	{
		void* Element = static_cast<uint8*>(Value) + Property.GetElementSize() * ArrayIndex;
		VisitElement(Property, Element, Visitor);
	}
}

void FOWTReferenceVisitor::VisitElement(FProperty& Property, void* Element, TFunctionRef<void(UObject*&, bool)> Visitor)
{
	FMulticastDelegateProperty* MulticastDelegate = CastField<FMulticastDelegateProperty>(&Property);
	FObjectPropertyBase* ObjectProperty = CastField<FObjectPropertyBase>(&Property);
	FInterfaceProperty* InterfaceProperty = CastField<FInterfaceProperty>(&Property);
	FStructProperty* StructProperty = CastField<FStructProperty>(&Property);
	FArrayProperty* ArrayProperty = CastField<FArrayProperty>(&Property);
	FSetProperty* SetProperty = CastField<FSetProperty>(&Property);
	FMapProperty* MapProperty = CastField<FMapProperty>(&Property);

	if (CastField<FDelegateProperty>(&Property))
	{
		static_cast<FScriptDelegate*>(Element)->Unbind();
	}
	else if (MulticastDelegate)
	{
		MulticastDelegate->ClearDelegate(nullptr, Element);
	}
	else if (ObjectProperty)
	{
		VisitObject(*ObjectProperty, Element, Property.HasAnyPropertyFlags(CPF_InstancedReference), Visitor);
	}
	else if (InterfaceProperty)
	{
		VisitInterface(*InterfaceProperty, Element, Visitor);
	}
	else if (StructProperty)
	{
		VisitStruct(*StructProperty, Element, Visitor);
	}
	else if (ArrayProperty)
	{
		VisitArray(*ArrayProperty, Element, Visitor);
	}
	else if (SetProperty)
	{
		VisitSet(*SetProperty, Element, Visitor);
	}
	else if (MapProperty)
	{
		VisitMap(*MapProperty, Element, Visitor);
	}
}

void FOWTReferenceVisitor::VisitObject(FObjectPropertyBase& Property, void* Element, bool bInstanced, TFunctionRef<void(UObject*&, bool)> Visitor)
{
	UObject* Object = Property.GetObjectPropertyValue(Element);
	UObject* OriginalObject = Object;
	Visitor(Object, bInstanced);
	if (Object != OriginalObject)
	{
		// Unloaded soft references still have a valid path despite resolving to nullptr.
		Property.SetObjectPropertyValue(Element, Object);
	}
}

void FOWTReferenceVisitor::VisitInterface(FInterfaceProperty& Property, void* Element, TFunctionRef<void(UObject*&, bool)> Visitor)
{
	FScriptInterface* Interface = static_cast<FScriptInterface*>(Element);
	UObject* Object = Interface->GetObject();
	Visitor(Object, false);
	Interface->SetObject(Object);
	Interface->SetInterface(Object ? Object->GetInterfaceAddress(Property.InterfaceClass) : nullptr);
}

void FOWTReferenceVisitor::VisitStruct(FStructProperty& Property, void* Element, TFunctionRef<void(UObject*&, bool)> Visitor)
{
	// Timer handles identify entries in a shared world manager; copying them aliases the source's timer.
	const bool bTimerHandle = Property.Struct == FTimerHandle::StaticStruct();
	if (bTimerHandle)
	{
		static_cast<FTimerHandle*>(Element)->Invalidate();
	}
	for (TFieldIterator<FProperty> Field(Property.Struct); Field && !bTimerHandle; ++Field)
	{
		void* FieldValue = Field->ContainerPtrToValuePtr<void>(Element);
		if (Field->HasAnyPropertyFlags(FOWTPropertyFilter::ExcludedStateFlags))
		{
			Field->ClearValue(FieldValue);
		}
		else
		{
			Visit(**Field, FieldValue, Visitor);
		}
	}
}

void FOWTReferenceVisitor::VisitArray(FArrayProperty& Property, void* Element, TFunctionRef<void(UObject*&, bool)> Visitor)
{
	FScriptArrayHelper Array(&Property, Element);
	for (int32 Index = 0; Index < Array.Num(); ++Index)
	{
		Visit(*Property.Inner, Array.GetRawPtr(Index), Visitor);
	}
}

void FOWTReferenceVisitor::VisitSet(FSetProperty& Property, void* Element, TFunctionRef<void(UObject*&, bool)> Visitor)
{
	FScriptSetHelper Set(&Property, Element);
	for (int32 Index = 0; Index < Set.GetMaxIndex(); ++Index)
	{
		if (Set.IsValidIndex(Index))
		{
			Visit(*Property.ElementProp, Set.GetElementPtr(Index), Visitor);
		}
	}
	// Remapped elements hash differently from their source objects.
	Set.Rehash();
}

void FOWTReferenceVisitor::VisitMap(FMapProperty& Property, void* Element, TFunctionRef<void(UObject*&, bool)> Visitor)
{
	FScriptMapHelper Map(&Property, Element);
	for (int32 Index = 0; Index < Map.GetMaxIndex(); ++Index)
	{
		if (Map.IsValidIndex(Index))
		{
			Visit(*Property.KeyProp, Map.GetKeyPtr(Index), Visitor);
			Visit(*Property.ValueProp, Map.GetValuePtr(Index), Visitor);
		}
	}
	Map.Rehash();
}
