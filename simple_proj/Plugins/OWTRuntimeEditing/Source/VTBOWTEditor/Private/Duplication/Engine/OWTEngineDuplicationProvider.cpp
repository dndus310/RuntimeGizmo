#include "Duplication/Engine/OWTEngineDuplicationProvider.h"

#include "Duplication/Engine/OWTEnginePropertyPolicies.h"
#include "Duplication/Engine/OWTEngineSourceRules.h"
#include "Duplication/OWTDuplicationAdapter.h"

FName FOWTEngineDuplicationProvider::GetProviderId() const
{
	return TEXT("OWT.EngineConfiguration");
}

void FOWTEngineDuplicationProvider::RegisterAdapters(FOWTDuplicationAdapterRegistry& Registry) const
{
	FOWTEnginePropertyPolicies::Register(Registry.GetPropertyPolicies());
	FOWTEngineSourceRules::Register(Registry.GetSourceRules());
}
