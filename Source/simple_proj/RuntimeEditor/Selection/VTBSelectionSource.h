#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VTBSelectionSource.generated.h"

DECLARE_MULTICAST_DELEGATE(FVTBSelectionChanged);

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class SIMPLE_PROJ_API UVTBSelectionSource : public UInterface
{
	GENERATED_BODY()
};

/** Supplies a weak selection snapshot and notifies listeners after it changes. */
class SIMPLE_PROJ_API IVTBSelectionSource
{
	GENERATED_BODY()

public:
	virtual void GetSelectionSnapshot(TArray<TWeakObjectPtr<AActor>>& OutActors) const = 0;
	virtual FVTBSelectionChanged& OnSelectionChanged() = 0;
};
