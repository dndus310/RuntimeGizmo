#pragma once

#include "CoreMinimal.h"

class FProperty;
class UStruct;

namespace OWTPCGConfiguration
{
/** VisibleAnywhere values are observations, even though their flags also include CPF_Edit. */
bool IsAuthoredProperty(const FProperty& Property);

/** Copies only the declaring schema's writable settings; derived fields belong to the core duplicator. */
void CopyAuthoredProperties(const UStruct& DeclaringType, const void* Source, void* Destination);
} // namespace OWTPCGConfiguration
