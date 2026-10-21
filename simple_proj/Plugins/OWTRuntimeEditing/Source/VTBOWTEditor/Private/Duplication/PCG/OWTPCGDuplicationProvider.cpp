#include "Duplication/PCG/OWTPCGDuplicationProvider.h"

#include "Duplication/PCG/OWTPCGDuplicationAdapter.h"
#include "PCGComponent.h"

FName FOWTPCGDuplicationProvider::GetProviderId() const
{
	return TEXT("OWT.PCG");
}

void FOWTPCGDuplicationProvider::RegisterAdapters(FOWTDuplicationAdapterRegistry& Registry) const
{
	Registry.Register(UPCGComponent::StaticClass(), []()
	{
		return MakeUnique<FOWTPCGDuplicationAdapter>();
	});
}
