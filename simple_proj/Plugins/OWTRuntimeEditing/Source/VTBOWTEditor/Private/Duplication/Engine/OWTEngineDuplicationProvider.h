#pragma once

#include "CoreMinimal.h"
#include "Duplication/OWTDuplicationAdapterProvider.h"

/** Installs the engine property policies and source rules into every duplication session. */
class FOWTEngineDuplicationProvider final : public IOWTDuplicationAdapterProvider
{
public:
	virtual FName GetProviderId() const override;
	virtual void RegisterAdapters(FOWTDuplicationAdapterRegistry& Registry) const override;
};
