#pragma once

#include "CoreMinimal.h"
#include "Duplication/OWTDuplicationAdapterProvider.h"

/** Installs the PCG component adapter into every duplication session. */
class FOWTPCGDuplicationProvider final : public IOWTDuplicationAdapterProvider
{
public:
	virtual FName GetProviderId() const override;
	virtual void RegisterAdapters(FOWTDuplicationAdapterRegistry& Registry) const override;
};
