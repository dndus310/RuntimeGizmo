#pragma once

#include "CoreMinimal.h"
#include "ToolContextInterfaces.h"
#include "UObject/Interface.h"
#include "VTBOWTEditorSelectionSource.generated.h"

using FVTBOWTActorSelection = TArray<TWeakObjectPtr<AActor>>;
class USceneComponent;

struct FVTBOWTTransformSelection
{
	FVTBOWTActorSelection Actors;
	TWeakObjectPtr<USceneComponent> FrameComponent;
};
DECLARE_MULTICAST_DELEGATE(FVTBOWTSelectionChanged);

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class VTBOWTEDITOR_API UVTBOWTEditorSelectionSource : public UInterface
{
	GENERATED_BODY()
};

class VTBOWTEDITOR_API IVTBOWTEditorSelectionSource
{
	GENERATED_BODY()
public:
	virtual void GetSelection(FVTBOWTActorSelection& OutActors) const = 0;
	virtual USceneComponent* GetSelectionFrame() const
	{
		return nullptr;
	}
	virtual FVTBOWTSelectionChanged& OnSelectionChanged() = 0;
	virtual bool ApplySelectionChange(const FSelectedObjectsChangeList& Change)
	{
		return false;
	}
};
