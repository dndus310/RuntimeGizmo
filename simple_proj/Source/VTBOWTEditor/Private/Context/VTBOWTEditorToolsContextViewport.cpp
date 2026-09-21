#include "VTBOWTEditorToolsContextViewport.h"

#include "Context/VTBOWTEditorToolsContext.h"
#include "BaseGizmos/GizmoViewContext.h"
#include "ContextObjectStore.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolManager.h"
#include "Misc/ScopeExit.h"
#include "SceneView.h"
#include "UnrealClient.h"

bool FVTBOWTEditorToolsContextViewport::UpdateView(APlayerController* PlayerController)
{
	bool bHasView = false;
	Owner.RunContextUpdate([&]
	{
		UWorld* World = Owner.GetContextQueriesAPI()->GetCurrentEditingWorld();
		if (!IsValid(PlayerController) || !IsValid(World) || PlayerController->GetWorld() != World || !World->Scene)
		{
			return;
		}
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		UGameViewportClient* ViewportClient = LocalPlayer ? LocalPlayer->ViewportClient : nullptr;
		FViewport* Viewport = ViewportClient ? ViewportClient->Viewport : nullptr;
		if (!Viewport || Viewport->GetSizeXY().GetMin() <= 0)
		{
			return;
		}
		FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(Viewport, World->Scene,
			ViewportClient->EngineShowFlags)
				.SetRealtimeUpdate(true));
		FVector ViewLocation;
		FRotator ViewRotation;
		FSceneView* View = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, Viewport);
		if (!View || View->UnscaledViewRect.Width() <= 0 || View->UnscaledViewRect.Height() <= 0)
		{
			return;
		}
		Owner.UpdateRenderView(View, nullptr);
		ON_SCOPE_EXIT
		{
			Owner.ResetRenderView();
		};
		if (UGizmoViewContext* ViewContext = Owner.ContextObjectStore->FindContext<UGizmoViewContext>())
		{
			ViewContext->ResetFromSceneView(*View);
			ViewContext->SetDPIScale(Owner.GetContextRenderAPI()->GetCameraState().DPIScale);
		}
		bHasView = true;
	});
	return bHasView && Owner.IsRuntimeReady();
}

void FVTBOWTEditorToolsContextViewport::Render(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (!View || !PDI)
	{
		return;
	}
	Owner.RunContextUpdate([&]
	{
		Owner.UpdateRenderView(View, PDI);
		const FViewCameraState Camera = Owner.GetContextRenderAPI()->GetCameraState();
		ON_SCOPE_EXIT
		{
			Owner.ResetRenderView();
		};
		if (UGizmoViewContext* ViewContext = Owner.ContextObjectStore->FindContext<UGizmoViewContext>())
		{
			ViewContext->ResetFromSceneView(*View);
			ViewContext->SetDPIScale(Camera.DPIScale);
		}
		Owner.ToolManager->Render(Owner.GetContextRenderAPI());
		if (Owner.IsRuntimeReady())
		{
			Owner.GizmoManager->Render(Owner.GetContextRenderAPI());
			++RenderCallCount;
		}
	});
}
