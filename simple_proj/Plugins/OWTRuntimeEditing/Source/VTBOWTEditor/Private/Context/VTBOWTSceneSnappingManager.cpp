#include "Context/VTBOWTSceneSnappingManager.h"
#include "Context/VTBOWTEditorToolsContext.h"

bool UVTBOWTSceneSnappingManager::ExecuteSceneSnapQuery(const FSceneSnapQueryRequest& Request,
                                                    TArray<FSceneSnapQueryResult>& ResultsOut) const
{
	if (Request.RequestType != ESceneSnapQueryType::Position ||
	    !EnumHasAnyFlags(Request.TargetTypes, ESceneSnapQueryTargetType::Grid))
	{
		return false;
	}
	const UVTBOWTEditorToolsContext* Context = GetTypedOuter<UVTBOWTEditorToolsContext>();
	check(Context);
	const FOWTGizmoSnapSettings Settings = Context->GetSnapSettings();
	if (!Settings.bTranslationEnabled)
	{
		return false;
	}
	const FVector Grid = Request.GridSize.Get(FVector(Settings.TranslationStep));
	if (Grid.ContainsNaN() || Grid.GetMin() <= UE_SMALL_NUMBER || Request.Position.ContainsNaN())
	{
		return false;
	}

	FSceneSnapQueryResult Result;
	Result.TargetType = ESceneSnapQueryTargetType::Grid;
	Result.Position = FVector(FMath::GridSnap(Request.Position.X, Grid.X), FMath::GridSnap(Request.Position.Y, Grid.Y),
	                          FMath::GridSnap(Request.Position.Z, Grid.Z));
	ResultsOut.Add(Result);
	return true;
}
