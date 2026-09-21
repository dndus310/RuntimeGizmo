#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "VTBOWTEditorLocalPlayer.generated.h"

UCLASS()
class VTBOWTEDITOR_API UVTBOWTEditorLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()

public:
	virtual bool CalcSceneViewInitOptions(
		FSceneViewInitOptions& OutInitOptions,
		FViewport* Viewport,
		FViewElementDrawer* ViewDrawer = nullptr,
		int32 StereoViewIndex = INDEX_NONE) override;
};
