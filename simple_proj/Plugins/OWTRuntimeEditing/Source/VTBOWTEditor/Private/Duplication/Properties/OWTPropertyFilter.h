#pragma once

#include "CoreMinimal.h"

class FOWTDuplicationPropertyPolicySet;
class FProperty;

/** Decides which reflected properties hold authored configuration. */
class FOWTPropertyFilter
{
public:
	/** Properties with these flags describe runtime or editor state, never authored configuration. */
	static constexpr EPropertyFlags ExcludedStateFlags = CPF_Transient | CPF_DuplicateTransient |
	                                                     CPF_NonPIEDuplicateTransient | CPF_Deprecated |
	                                                     CPF_EditorOnly | CPF_SkipSerialization;

	/** Bound delegates point at the source's listeners and are never part of a snapshot. */
	static bool IsDelegate(const FProperty& Property);
	/** Reflected state the core copies for Object, after class-specific policies were consulted. */
	static bool IsCopied(const FProperty& Property, const UObject& Object, const FOWTDuplicationPropertyPolicySet& Policies);
};
