#include "Duplication/OWTPCGComponentConfiguration.h"

#include "UObject/UnrealType.h"

namespace OWTPCGConfiguration
{
bool IsAuthoredProperty(const FProperty& Property)
{
	if (!Property.HasAnyPropertyFlags(CPF_Edit))
	{
		return false;
	}
	constexpr EPropertyFlags Excluded = CPF_EditConst | CPF_Transient | CPF_DuplicateTransient |
	                                    CPF_NonPIEDuplicateTransient | CPF_Deprecated | CPF_EditorOnly |
	                                    CPF_SkipSerialization;
	if (Property.HasAnyPropertyFlags(Excluded))
	{
		return false;
	}
	if (CastField<FDelegateProperty>(&Property))
	{
		return false;
	}
	if (CastField<FMulticastDelegateProperty>(&Property))
	{
		return false;
	}
	return true;
}

void CopyAuthoredProperties(const UStruct& DeclaringType, const void* Source, void* Destination)
{
	check(Source);
	check(Destination);
	for (TFieldIterator<FProperty> Property(&DeclaringType, EFieldIteratorFlags::ExcludeSuper); Property; ++Property)
	{
		if (!IsAuthoredProperty(**Property))
		{
			continue;
		}
		Property->CopyCompleteValue(Property->ContainerPtrToValuePtr<void>(Destination),
		                            Property->ContainerPtrToValuePtr<void>(Source));
	}
}
} // namespace OWTPCGConfiguration
