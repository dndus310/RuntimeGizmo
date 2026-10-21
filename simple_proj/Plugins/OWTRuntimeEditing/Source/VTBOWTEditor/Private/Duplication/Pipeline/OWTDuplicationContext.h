#pragma once

#include "CoreMinimal.h"

class FOWTAdapterDispatcher;
class FOWTDuplicationPropertyPolicySet;
class FOWTDuplicationSourceRuleSet;
class FOWTUnsupportedStructCache;
class UWorld;
struct FOWTDuplicationOptions;

/** Source object -> duplicate object for every object an operation captured. */
using FOWTObjectMap = TMap<UObject*, UObject*>;

/** Services shared by every stage of one duplication operation. Error receives the first failure. */
struct FOWTDuplicationContext
{
	FOWTDuplicationContext(UWorld& InWorld, const FOWTDuplicationOptions& InOptions,
	                       const FOWTDuplicationPropertyPolicySet& InPropertyPolicies,
	                       const FOWTDuplicationSourceRuleSet& InSourceRules, FOWTAdapterDispatcher& InAdapters,
	                       FOWTUnsupportedStructCache& InUnsupportedStructs, FString& InError);

	UWorld& World;
	const FOWTDuplicationOptions& Options;
	const FOWTDuplicationPropertyPolicySet& PropertyPolicies;
	const FOWTDuplicationSourceRuleSet& SourceRules;
	FOWTAdapterDispatcher& Adapters;
	FOWTUnsupportedStructCache& UnsupportedStructs;
	FString& Error;
};
