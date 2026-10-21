#pragma once

#include "CoreMinimal.h"
#include "UObject/StrongObjectPtr.h"

class UPCGComponent;
class UPCGSchedulingPolicyBase;

/** Authored configuration of one source PCG component, held on a transient component that never registers. */
struct FOWTPCGConfigurationSnapshot
{
	explicit FOWTPCGConfigurationSnapshot(UPCGComponent& Component);
	/** Duplicates a scheduling policy and its subobjects into Owner, reporting every created object in Copies. */
	static UPCGSchedulingPolicyBase* CopySchedulingPolicy(UPCGSchedulingPolicyBase& Policy, UPCGComponent& Owner, TMap<UObject*, UObject*>& Copies);
	/** The policy followed by every object nested inside it. Empty for a null policy. */
	static TArray<UObject*> GetSchedulingPolicyObjects(UPCGSchedulingPolicyBase* Policy);

	TWeakObjectPtr<UPCGComponent> Source;
	TStrongObjectPtr<UPCGComponent> Configuration;
	/** Source scheduling policy objects mapped to their copies inside Configuration. */
	TMap<UObject*, UObject*> PolicyCopies;
	/** Generated on the source itself or on any of its partition-local components. */
	bool bWasGenerated;
	bool bActivated;
};
