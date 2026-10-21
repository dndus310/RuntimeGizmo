#include "Duplication/Properties/OWTPropertyFilter.h"

#include "Duplication/OWTDuplicationPropertyPolicy.h"
#include "UObject/UnrealType.h"

bool FOWTPropertyFilter::IsDelegate(const FProperty& Property)
{
	return CastField<FDelegateProperty>(&Property) || CastField<FMulticastDelegateProperty>(&Property);
}

bool FOWTPropertyFilter::IsCopied(const FProperty& Property, const UObject& Object,
                                  const FOWTDuplicationPropertyPolicySet& Policies)
{
	const bool bRuntimeState = Property.HasAnyPropertyFlags(ExcludedStateFlags) || IsDelegate(Property);
	return !bRuntimeState && Policies.Resolve(Object, Property).Get(true);
}
