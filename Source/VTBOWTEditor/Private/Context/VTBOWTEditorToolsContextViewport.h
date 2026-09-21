#pragma once

#include "CoreMinimal.h"
#include "Context/IVTBOWTEditorViewport.h"

class APlayerController;
class FPrimitiveDrawInterface;
class FSceneView;
class UVTBOWTEditorToolsContext;

class FVTBOWTEditorToolsContextViewport : public IVTBOWTEditorViewport
{
public:
	explicit FVTBOWTEditorToolsContextViewport(UVTBOWTEditorToolsContext& InOwner)
		: Owner(InOwner)
	{
	}

	virtual bool UpdateView(APlayerController* PlayerController) override;
	virtual void Render(const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	void ResetRenderCallCount()
	{
		RenderCallCount = 0;
	}
	virtual uint64 GetRenderCallCount() const override
	{
		return RenderCallCount;
	}

private:
	UVTBOWTEditorToolsContext& Owner;
	uint64 RenderCallCount = 0;
};
