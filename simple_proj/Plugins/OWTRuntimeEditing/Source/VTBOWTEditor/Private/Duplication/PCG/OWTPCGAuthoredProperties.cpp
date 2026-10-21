#include "Duplication/PCG/OWTPCGAuthoredProperties.h"

#include "Duplication/Properties/OWTPropertyFilter.h"
#include "UObject/UnrealType.h"

bool FOWTPCGAuthoredProperties::IsAuthored(const FProperty& Property)
{
	const bool bEditable = Property.HasAnyPropertyFlags(CPF_Edit);
	const bool bReadOnlyOrState = Property.HasAnyPropertyFlags(CPF_EditConst | FOWTPropertyFilter::ExcludedStateFlags);
	return bEditable && !bReadOnlyOrState && !FOWTPropertyFilter::IsDelegate(Property);
}

void FOWTPCGAuthoredProperties::Copy(const UStruct& DeclaringType, const void* Source, void* Destination)
{
	check(Source);
	check(Destination);
	for (TFieldIterator<FProperty> Property(&DeclaringType, EFieldIteratorFlags::ExcludeSuper); Property; ++Property)
	{
		if (IsAuthored(**Property))
		{
			Property->CopyCompleteValue(Property->ContainerPtrToValuePtr<void>(Destination),
			                            Property->ContainerPtrToValuePtr<void>(Source));
		}
	}
}
