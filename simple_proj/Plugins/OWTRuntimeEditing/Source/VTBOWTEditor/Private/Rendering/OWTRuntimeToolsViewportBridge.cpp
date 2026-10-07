#include "Rendering/OWTRuntimeToolsViewportBridge.h"
#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "Context/VTBOWTEditorToolsContext.h"
#include "Engine/Canvas.h"
#include "GameFramework/PlayerController.h"
#include "InteractiveToolManager.h"
#include "InteractiveGizmoManager.h"
#include "PrimitiveDrawInterface.h"
#include "RenderingThread.h"
#include "SceneManagement.h"
#include "SceneView.h"

namespace
{
// Runtime tools draw overlay primitives on the game thread. Material mesh tools must
// declare their stronger capability requirement and are rejected by the registry.
class FOWTCanvasPDI final : public FPrimitiveDrawInterface
{
public:
	FOWTCanvasPDI(const FSceneView* InView, FCanvas& InCanvas)
	    : FPrimitiveDrawInterface(InView), Canvas(InCanvas), Resources()
	{
	}
	virtual ~FOWTCanvasPDI() override
	{
		if (Resources.IsEmpty())
		{
			return;
		}
		ENQUEUE_RENDER_COMMAND(OWTReleaseUnsupportedMeshResources)(
		    [Owned = MoveTemp(Resources)](FRHICommandListImmediate& RHICmdList)
		    {
			    for (FDynamicPrimitiveResource* Resource : Owned)
			    {
				    Resource->InitPrimitiveResource(RHICmdList);
			    }
			    // ReleasePrimitiveResource owns destruction, like FSimpleElementCollector.
			    for (FDynamicPrimitiveResource* Resource : Owned)
			    {
				    Resource->ReleasePrimitiveResource();
			    }
		    });
	}
	virtual bool IsHitTesting() override
	{
		return false;
	}
	virtual void SetHitProxy(HHitProxy* HitProxy) override
	{
	}
	virtual void AddReserveLines(uint8 Group, int32 Count, bool Biased, bool Thick) override
	{
	}
	virtual void RegisterDynamicResource(FDynamicPrimitiveResource* Resource) override
	{
		// Even a misdeclared mesh tool must not leak resources handed to the PDI.
		Resources.Add(Resource);
	}
	virtual int32 DrawMesh(const FMeshBatch& Mesh) override
	{
		UE_LOG(LogTemp, Warning, TEXT("OWT: material mesh drawing requires a mesh-capable viewport provider."));
		return 0;
	}
	virtual void DrawLine(const FVector& Start, const FVector& End, const FLinearColor& Color, uint8 Group,
	                      float Thickness, float DepthBias, bool bScreenSpace) override
	{
		FVector2D A, B;
		if (!View->WorldToPixel(Start, A))
		{
			return;
		}
		if (!View->WorldToPixel(End, B))
		{
			return;
		}
		FCanvasLineItem Line(A, B);
		Line.SetColor(Color);
		Line.LineThickness = FMath::Max(Thickness, 1.f);
		Canvas.DrawItem(Line);
	}
	virtual void DrawTranslucentLine(const FVector& Start, const FVector& End, const FLinearColor& Color, uint8 Group,
	                                 float Thickness, float DepthBias, bool bScreenSpace) override
	{
		DrawLine(Start, End, Color, Group, Thickness, DepthBias, bScreenSpace);
	}
	virtual void DrawPoint(const FVector& Position, const FLinearColor& Color, float PointSize, uint8 Group) override
	{
		FVector2D Pixel;
		if (!View->WorldToPixel(Position, Pixel))
		{
			return;
		}
		const FVector2D Size(FMath::Max(PointSize, 1.f));
		FCanvasTileItem Point(Pixel - Size * .5, Size, Color);
		Point.BlendMode = SE_BLEND_Translucent;
		Canvas.DrawItem(Point);
	}
	virtual void DrawSprite(const FVector& Position, float SizeX, float SizeY, const FTexture* Sprite,
	                        const FLinearColor& Color, uint8 Group, float U, float UL, float V, float VL,
	                        uint8 BlendMode, float OpacityMaskRefVal) override
	{
		FVector2D Pixel;
		if (!View->WorldToPixel(Position, Pixel))
		{
			return;
		}
		const FVector2D Size(SizeX, SizeY);
		FCanvasTileItem Item(Pixel - Size * .5, Sprite, Size, FVector2D(U, V), FVector2D(U + UL, V + VL), Color);
		Item.BlendMode = static_cast<ESimpleElementBlendMode>(BlendMode);
		Canvas.DrawItem(Item);
	}

private:
	FCanvas& Canvas;
	TArray<FDynamicPrimitiveResource*> Resources;
};

class FOWTCanvasRenderAPI final : public IToolsContextRenderAPI
{
public:
	FOWTCanvasRenderAPI(FOWTCanvasPDI& InPDI, UVTBOWTEditorToolsContext& InContext) : PDI(InPDI), Context(InContext)
	{
	}
	virtual FPrimitiveDrawInterface* GetPrimitiveDrawInterface() override
	{
		return &PDI;
	}
	virtual const FSceneView* GetSceneView() override
	{
		return PDI.View;
	}
	virtual FViewCameraState GetCameraState() override
	{
		FViewCameraState State;
		Context.ToolManager->GetContextQueriesAPI()->GetCurrentViewState(State);
		return State;
	}
	virtual EViewInteractionState GetViewInteractionState() override
	{
		return EViewInteractionState::Focused;
	}

private:
	FOWTCanvasPDI& PDI;
	UVTBOWTEditorToolsContext& Context;
};
} // namespace

void OWTRuntimeToolsViewportBridge::Draw(UVTBOWTEditorToolsContext& Context, UCanvas* Canvas,
                                         APlayerController* Controller, UWorld* World)
{
	check(IsInGameThread());
	if (!Canvas)
	{
		return;
	}
	if (!Canvas->Canvas)
	{
		return;
	}
	if (!Canvas->SceneView)
	{
		return;
	}
	if (!Controller)
	{
		return;
	}
	if (Controller->GetWorld() != World)
	{
		return;
	}
	FOWTCanvasPDI PDI(Canvas->SceneView, *Canvas->Canvas);
	FOWTCanvasRenderAPI RenderAPI(PDI, Context);
	Context.ToolManager->Render(&RenderAPI);
	if (!Context.IsInitialized())
	{
		return;
	}
	Context.GizmoManager->Render(&RenderAPI);
	Context.ToolManager->DrawHUD(Canvas->Canvas, &RenderAPI);
	if (!Context.IsInitialized())
	{
		return;
	}
	Context.GizmoManager->DrawHUD(Canvas->Canvas, &RenderAPI);
}
