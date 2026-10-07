#pragma once

#include "CoreMinimal.h"
#include "Features/IModularFeature.h"

class FOWTDuplicationAdapterRegistry;

/** Modules publish factories; each duplicator captures registrations into its own session registry. */
class VTBOWTEDITOR_API IOWTDuplicationAdapterProvider : public IModularFeature
{
public:
	virtual ~IOWTDuplicationAdapterProvider();

	virtual FName GetProviderId() const = 0;

	virtual void RegisterAdapters(FOWTDuplicationAdapterRegistry& Registry) const = 0;

	static FName GetFeatureName();
};
