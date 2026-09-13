#pragma once

#include "SceneQueries/SceneSnappingManager.h"
#include "OWTGridSnappingManager.generated.h"

// Runtime grid queries used by CombinedTransformGizmo's translation parameter sources.
UCLASS()
class VTBOWTEDITOR_API UOWTGridSnappingManager : public USceneSnappingManager
{
	GENERATED_BODY()

public:
	virtual bool ExecuteSceneSnapQuery(const FSceneSnapQueryRequest& Request,
	                                   TArray<FSceneSnapQueryResult>& ResultsOut) const override;
};
