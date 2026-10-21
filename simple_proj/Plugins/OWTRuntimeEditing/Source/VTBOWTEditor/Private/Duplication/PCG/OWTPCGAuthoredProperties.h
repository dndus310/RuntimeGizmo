#pragma once

#include "CoreMinimal.h"

class FProperty;
class UStruct;

/** The writable settings of a PCG schema. Derived and observed fields belong to the core duplicator. */
class FOWTPCGAuthoredProperties
{
public:
	/** VisibleAnywhere values are observations, even though their flags also include CPF_Edit. */
	static bool IsAuthored(const FProperty& Property);
	/** Copies only properties DeclaringType itself declares, not those of its bases. */
	static void Copy(const UStruct& DeclaringType, const void* Source, void* Destination);
};
