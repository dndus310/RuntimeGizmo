#pragma once

#include "CoreMinimal.h"

class APlayerController;
class FPrimitiveDrawInterface;
class FSceneView;

class VTBOWTEDITOR_API IVTBOWTEditorViewport
{
public:
	virtual ~IVTBOWTEditorViewport() = default;

	virtual bool UpdateView(APlayerController* PlayerController) = 0;
	virtual void Render(const FSceneView* View, FPrimitiveDrawInterface* PDI) = 0;
	virtual uint64 GetRenderCallCount() const = 0;
};
