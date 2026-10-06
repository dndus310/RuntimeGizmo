#pragma once

#include "SceneQueries/SceneSnappingManager.h"
#include "VTBOWTSceneSnappingManager.generated.h"

// Runtime grid queries used by CombinedTransformGizmo's translation parameter sources.
UCLASS()
class VTBOWTEDITOR_API UVTBOWTSceneSnappingManager : public USceneSnappingManager
{
	GENERATED_BODY()

public:
	virtual bool ExecuteSceneSnapQuery(const FSceneSnapQueryRequest& Request,
	                                   TArray<FSceneSnapQueryResult>& ResultsOut) const override;
};
