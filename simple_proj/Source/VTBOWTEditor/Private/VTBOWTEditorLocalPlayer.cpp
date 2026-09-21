#include "VTBOWTEditorLocalPlayer.h"

#include "VTBOWTEditorViewportClient.h"

bool UVTBOWTEditorLocalPlayer::CalcSceneViewInitOptions(
	FSceneViewInitOptions& OutInitOptions,
	FViewport* Viewport,
	FViewElementDrawer* ViewDrawer,
	int32 StereoViewIndex)
{
	if (!ViewDrawer)
	{
		UVTBOWTEditorViewportClient* EditorViewport = Cast<UVTBOWTEditorViewportClient>(ViewportClient);
		if (IsValid(EditorViewport) && Viewport && EditorViewport->Viewport == Viewport)
		{
			ViewDrawer = EditorViewport;
		}
	}

	return Super::CalcSceneViewInitOptions(OutInitOptions, Viewport, ViewDrawer, StereoViewIndex);
}
