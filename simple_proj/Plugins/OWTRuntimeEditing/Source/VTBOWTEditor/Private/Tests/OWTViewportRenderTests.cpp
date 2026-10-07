#if WITH_DEV_AUTOMATION_TESTS

#include "Context/VTBOWTEditorToolsContext.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformTime.h"
#include "InteractiveToolManager.h"
#include "Misc/AutomationTest.h"
#include "Modes/OWTAttributeEditMode.h"
#include "Rendering/OWTRuntimeToolsHUD.h"
#include "RHIGlobals.h"
#include "Tests/OWTModeTestTypes.h"
#include "UnrealClient.h"
#include "VTBOWTEditorSubsystem.h"

namespace
{
class FOWTViewportRenderCommand final : public IAutomationLatentCommand
{
public:
	explicit FOWTViewportRenderCommand(FAutomationTestBase& InTest) : Test(InTest)
	{
	}

	virtual ~FOWTViewportRenderCommand() override
	{
		Cleanup(false);
	}

	virtual bool Update() override
	{
		if (Deadline == 0)
		{
			Deadline = FPlatformTime::Seconds() + 5.0;
		}
		if (!bStarted)
		{
			if (!TryStart())
			{
				if (bFailed)
				{
					Cleanup(true);
					return true;
				}
				if (FPlatformTime::Seconds() >= Deadline)
				{
					Test.AddError(
					    TEXT("No ready local game viewport/PlayerController was available within five seconds. Run "
					         "this test in a loaded -game or packaged map with the runtime tools HUD."));
					Cleanup(true);
					return true;
				}
				return false;
			}
			return false;
		}

		UOWTModeTestTool* ActiveTool = Tool.Get();
		if (!ActiveTool)
		{
			Test.AddError(TEXT("The viewport test tool was destroyed before a real HUD frame reached it."));
			Cleanup(true);
			return true;
		}
		if (ActiveTool->RenderCount > 0)
		{
			if (ActiveTool->HUDCount > 0)
			{
				Test.TestTrue(TEXT("The live game viewport dispatches tool Render"), ActiveTool->RenderCount > 0);
				Test.TestTrue(TEXT("The live game HUD dispatches tool DrawHUD"), ActiveTool->HUDCount > 0);
				Test.AddInfo(FString::Printf(TEXT("Observed real viewport callbacks: Render=%d, DrawHUD=%d. The test "
				                                  "never calls DrawHUD or RenderTools directly."),
				                             ActiveTool->RenderCount, ActiveTool->HUDCount));
				Cleanup(true);
				return true;
			}
		}
		if (FPlatformTime::Seconds() >= Deadline)
		{
			Test.AddError(FString::Printf(TEXT("The active RHI viewport did not dispatch both tool callbacks within "
			                                   "five seconds (Render=%d, DrawHUD=%d)."),
			                              ActiveTool->RenderCount, ActiveTool->HUDCount));
			Cleanup(true);
			return true;
		}
		return false;
	}

private:
	bool TryStart()
	{
		if (!GEngine)
		{
			return false;
		}
		UGameViewportClient* ViewportClient = GEngine->GameViewport;
		if (!ViewportClient)
		{
			return false;
		}
		if (!ViewportClient->Viewport)
		{
			return false;
		}
		const FIntPoint Size = ViewportClient->Viewport->GetSizeXY();
		if (Size.X <= 0)
		{
			return false;
		}
		if (Size.Y <= 0)
		{
			return false;
		}
		UWorld* World = ViewportClient->GetWorld();
		if (!World)
		{
			return false;
		}
		if (!World->IsGameWorld())
		{
			return false;
		}
		if (!World->HasBegunPlay())
		{
			return false;
		}
		APlayerController* Controller = World->GetFirstPlayerController();
		if (!Controller)
		{
			return false;
		}
		if (!Controller->IsLocalController())
		{
			return false;
		}
		if (!Controller->GetHUD())
		{
			return false;
		}
		if (!Controller->GetHUD()->IsA<AOWTRuntimeToolsHUD>())
		{
			return Fail(TEXT("The loaded map does not use AOWTRuntimeToolsHUD. This integration test requires the "
			                 "installed runtime HUD bridge; it does not replace the player's HUD."));
		}
		UVTBOWTEditorSubsystem* Hub = World->GetSubsystem<UVTBOWTEditorSubsystem>();
		if (!Hub)
		{
			return Fail(TEXT("The live game world has no AttributeEditMode host subsystem."));
		}
		UOWTAttributeEditMode* CurrentMode = Hub->GetAttributeEditMode();
		if (!CurrentMode)
		{
			return Fail(TEXT("The live game world has no runtime AttributeEditMode."));
		}
		Mode = CurrentMode;
		bPreviouslyEntered = CurrentMode->IsEntered();
		PreviousToolId = CurrentMode->GetSnapshot().ActiveToolId;
		PreviousSelection = CurrentMode->GetSelectedObject();
		if (bPreviouslyEntered)
		{
			if (!PreviousToolId.IsNone())
			{
				if (PreviousToolId != CurrentMode->DefaultToolId)
				{
					Mode.Reset();
					return Fail(TEXT("Viewport rendering validation requires an idle/default editing tool. An existing "
					                 "custom tool is active and was left untouched."));
				}
			}
		}
		if (!CurrentMode->Enter())
		{
			return Fail(TEXT("The live AttributeEditMode could not enter for viewport validation."));
		}
		FOWTToolDescriptor Descriptor;
		Descriptor.ToolId = TEXT("Contract.ViewportRendering");
		Descriptor.Label = FText::FromString(TEXT("Viewport rendering contract"));
		Descriptor.BuilderClass = UOWTModeTestBuilder::StaticClass();
		FGuid Registration;
		FString Error;
		if (!CurrentMode->RegisterTool(Descriptor, ProviderId, Registration, Error))
		{
			return Fail(TEXT("Could not register the viewport test tool: ") + Error);
		}
		bRegistered = true;
		if (!CurrentMode->RequestToolStart(Descriptor.ToolId, Error))
		{
			return Fail(TEXT("Could not activate the viewport test tool: ") + Error);
		}
		UVTBOWTEditorToolsContext* Context = CurrentMode->GetToolsContext();
		if (!Context)
		{
			return Fail(TEXT("The tools context ended during viewport test activation."));
		}
		Tool = Cast<UOWTModeTestTool>(Context->ToolManager->GetActiveTool(EToolSide::Left));
		if (!Tool.IsValid())
		{
			return Fail(TEXT("The live ToolManager did not retain the viewport test tool."));
		}
		bStarted = true;
		return true;
	}

	bool Fail(const FString& Message)
	{
		Test.AddError(Message);
		bFailed = true;
		return false;
	}

	void Cleanup(bool bReportErrors)
	{
		if (bCleaned)
		{
			return;
		}
		bCleaned = true;
		UOWTAttributeEditMode* CurrentMode = Mode.Get();
		if (!CurrentMode)
		{
			return;
		}
		if (!CurrentMode->IsEntered())
		{
			return;
		}
		FString Error;
		if (bRegistered)
		{
			if (!CurrentMode->UnregisterProvider(ProviderId, Error))
			{
				if (bReportErrors)
				{
					Test.AddError(TEXT("Viewport test provider cleanup failed: ") + Error);
				}
			}
		}
		if (!bPreviouslyEntered)
		{
			CurrentMode->Exit();
			return;
		}
		FName RestoreId = PreviousToolId;
		if (RestoreId.IsNone())
		{
			RestoreId = CurrentMode->DefaultToolId;
		}
		if (!CurrentMode->StartTool(RestoreId, Error))
		{
			if (bReportErrors)
			{
				Test.AddError(TEXT("Could not restore the previous editing tool: ") + Error);
			}
		}
		CurrentMode->SetSelectedObject(PreviousSelection.Get());
	}

	FAutomationTestBase& Test;
	TWeakObjectPtr<UOWTAttributeEditMode> Mode;
	TWeakObjectPtr<UOWTModeTestTool> Tool;
	TWeakObjectPtr<AActor> PreviousSelection;
	FName PreviousToolId;
	const FName ProviderId = TEXT("Contract.ViewportRendering.Provider");
	double Deadline = 0;
	bool bPreviouslyEntered = false;
	bool bRegistered = false;
	bool bStarted = false;
	bool bFailed = false;
	bool bCleaned = false;
};
} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTViewportRenderTest, "OWT.Runtime.ViewportRendering",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTViewportRenderTest::RunTest(const FString& Parameters)
{
	if (GUsingNullRHI)
	{
		AddInfo(TEXT("SKIPPED: NullRHI has no real viewport rendering. Run OWT.Runtime.ViewportRendering with an "
		             "active RHI in a loaded -game or packaged map using AOWTRuntimeToolsHUD."));
		return true;
	}
	FAutomationTestFramework::Get().EnqueueLatentCommand(MakeShared<FOWTViewportRenderCommand>(*this));
	return true;
}

#endif
