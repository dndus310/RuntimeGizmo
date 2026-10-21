#include "Duplication/Properties/OWTPropertyValue.h"

#include "Duplication/Properties/OWTReferenceVisitor.h"
#include "UObject/UnrealType.h"

FOWTPropertyValue::FOWTPropertyValue(FProperty& InProperty, const void* SourceContainer)
    : Property(InProperty), Value(nullptr)
{
	Value = FMemory::Malloc(Property.GetSize(), Property.GetMinAlignment());
	Property.InitializeValue(Value);
	Property.CopyCompleteValue(Value, Property.ContainerPtrToValuePtr<void>(SourceContainer));
}

FOWTPropertyValue::~FOWTPropertyValue()
{
	Property.DestroyValue(Value);
	FMemory::Free(Value);
}

void FOWTPropertyValue::ApplyTo(void* DestinationContainer, const TMap<UObject*, UObject*>& Mapping) const
{
	void* Destination = Property.ContainerPtrToValuePtr<void>(DestinationContainer);
	Property.CopyCompleteValue(Destination, Value);
	FOWTReferenceVisitor::Visit(Property, Destination,
	                            [&Mapping](UObject*& Reference, bool)
	                            {
		                            if (UObject* const* Replacement = Mapping.Find(Reference))
		                            {
			                            Reference = *Replacement;
		                            }
	                            });
}

FProperty& FOWTPropertyValue::GetProperty() const
{
	return Property;
}

void* FOWTPropertyValue::GetValue() const
{
	return Value;
}
