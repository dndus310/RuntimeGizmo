#pragma once

class UVTBOWTEditorToolsContext;
class UCanvas;
class APlayerController;
class UWorld;

namespace OWTRuntimeToolsViewportBridge
{
void Draw(UVTBOWTEditorToolsContext& Context, UCanvas* Canvas, APlayerController* Controller, UWorld* World);
}
