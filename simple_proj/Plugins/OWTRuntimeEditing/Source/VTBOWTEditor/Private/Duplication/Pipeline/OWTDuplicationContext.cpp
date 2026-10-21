#include "Duplication/Pipeline/OWTDuplicationContext.h"

FOWTDuplicationContext::FOWTDuplicationContext(UWorld& InWorld, const FOWTDuplicationOptions& InOptions,
                                               const FOWTDuplicationPropertyPolicySet& InPropertyPolicies,
                                               const FOWTDuplicationSourceRuleSet& InSourceRules,
                                               FOWTAdapterDispatcher& InAdapters,
                                               FOWTUnsupportedStructCache& InUnsupportedStructs, FString& InError)
    : World(InWorld), Options(InOptions), PropertyPolicies(InPropertyPolicies), SourceRules(InSourceRules),
      Adapters(InAdapters), UnsupportedStructs(InUnsupportedStructs), Error(InError)
{
}
