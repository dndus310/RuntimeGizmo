#if WITH_DEV_AUTOMATION_TESTS

#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Level.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformTime.h"
#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"
#include "Modes/OWTAttributeEditMode.h"
#include "RHIGlobals.h"
#include "UI/OWTAttributeDetailsWidget.h"
#include "UnrealClient.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorPlayerController.h"
#include "VTBOWTEditorSubsystem.h"

namespace
{
enum class ECaptureStage : uint8
{
	WaitingForViewport,
	DetailsLayout,
	DetailsFile,
	MonitorLayout,
	MonitorFile
};

class FOWTViewportMonitorCaptureCommand final : public IAutomationLatentCommand
{
public:
	explicit FOWTViewportMonitorCaptureCommand(FAutomationTestBase& InTest) : Test(InTest)
	{
	}
	virtual ~FOWTViewportMonitorCaptureCommand() override
	{
		Cleanup();
	}

	virtual bool Update() override
	{
		if (Deadline == 0)
		{
			EnterStage(ECaptureStage::WaitingForViewport);
		}
		if (FPlatformTime::Seconds() > Deadline)
		{
			return FinishWithError(FString::Printf(
			    TEXT("Viewport monitor capture exceeded 45 seconds at stage %d. Run in a loaded active-RHI game "
			         "with the native sidebar and a renderable window."),
			    static_cast<int32>(Stage)));
		}
		if (Stage == ECaptureStage::WaitingForViewport)
		{
			return PrepareViewport();
		}
		if (!Editor.IsValid())
		{
			return FinishWithError(TEXT("The capture editor ended before screenshot completion."));
		}
		if (!Widget.IsValid())
		{
			return FinishWithError(TEXT("The native sidebar ended before screenshot completion."));
		}
		if (Stage == ECaptureStage::DetailsLayout)
		{
			if (GFrameCounter < ReadyFrame)
			{
				return false;
			}
			RequestCapture(TEXT("Details"));
			EnterStage(ECaptureStage::DetailsFile);
			return false;
		}
		if (Stage == ECaptureStage::DetailsFile)
		{
			if (!CaptureFileExists())
			{
				return false;
			}
			Test.AddInfo(TEXT("Details screenshot: ") + ScreenshotPath);
			FOWTDuplicationOptions Options;
			Options.WorldOffset = FVector(0, 100, 0);
			OperationId = Editor->BeginDuplicateOperation(Editor->GetSnapshot(), Options);
			if (!OperationId.IsValid())
			{
				return FinishWithError(TEXT("The native fixture duplication was not accepted for monitor capture."));
			}
			Widget->SetMonitorVisible(true);
			ReadyFrame = GFrameCounter + 4;
			EnterStage(ECaptureStage::MonitorLayout);
			return false;
		}
		if (Stage == ECaptureStage::MonitorLayout)
		{
			if (GFrameCounter < ReadyFrame)
			{
				return false;
			}
			for (const FOWTDuplicationOperationSnapshot& Operation : Editor->GetDuplicationOperations())
			{
				if (Operation.OperationId != OperationId)
				{
					continue;
				}
				if (Operation.Phase == EOWTDuplicationPhase::Committed)
				{
					Duplicate = Operation.DuplicateActor;
					RequestCapture(TEXT("Monitor"));
					EnterStage(ECaptureStage::MonitorFile);
				}
				else if (Operation.Phase == EOWTDuplicationPhase::Failed)
				{
					return FinishWithError(TEXT("Monitor fixture duplication failed: ") + Operation.Error);
				}
				else if (Operation.Phase == EOWTDuplicationPhase::Cancelled)
				{
					return FinishWithError(TEXT("Monitor fixture duplication was cancelled: ") + Operation.Error);
				}
			}
			return false;
		}
		if (!CaptureFileExists())
		{
			return false;
		}
		Test.AddInfo(TEXT("Monitor screenshot: ") + ScreenshotPath);
		Test.AddInfo(TEXT("Both screenshots include the actual Slate UI. Inspect the PNG files for clipping; file "
		                  "creation alone does not verify visual quality."));
		Cleanup();
		return true;
	}

private:
	void EnterStage(ECaptureStage NewStage)
	{
		Stage = NewStage;
		Deadline = FPlatformTime::Seconds() + 45.0;
	}

	bool PrepareViewport()
	{
		if (!GEngine)
		{
			return false;
		}
		UGameViewportClient* Viewport = GEngine->GameViewport;
		if (!Viewport)
		{
			return false;
		}
		if (!Viewport->Viewport)
		{
			return false;
		}
		UWorld* World = Viewport->GetWorld();
		if (!World)
		{
			return false;
		}
		if (!World->HasBegunPlay())
		{
			return false;
		}
		AVTBOWTEditorPlayerController* Controller =
		    Cast<AVTBOWTEditorPlayerController>(World->GetFirstPlayerController());
		if (!Controller)
		{
			return false;
		}
		if (!Controller->IsLocalController())
		{
			return false;
		}
		UOWTAttributeDetailsWidget* CurrentWidget = Controller->GetAttributeDetailsWidget();
		if (!CurrentWidget)
		{
			return false;
		}
		UVTBOWTEditorSubsystem* Hub = World->GetSubsystem<UVTBOWTEditorSubsystem>();
		if (!Hub)
		{
			return false;
		}
		UOWTAttributeEditMode* CurrentMode = Hub->GetAttributeEditMode();
		AVTBAttributeEditor* CurrentEditor = Hub->GetAttributeEditor();
		if (!CurrentMode)
		{
			return false;
		}
		if (!CurrentEditor)
		{
			return false;
		}
		if (FScreenshotRequest::IsScreenshotRequested())
		{
			return false;
		}
		if (CurrentMode->IsEntered())
		{
			const FName Active = CurrentMode->GetSnapshot().ActiveToolId;
			if (!Active.IsNone())
			{
				if (Active != CurrentMode->DefaultToolId)
				{
					return FinishWithError(
					    TEXT("Capture requires an idle/default mode; an existing custom tool was left untouched."));
				}
			}
		}
		Mode = CurrentMode;
		Editor = CurrentEditor;
		Widget = CurrentWidget;
		bPreviouslyEntered = CurrentMode->IsEntered();
		bPreviousMonitor = CurrentWidget->IsMonitorVisible();
		PreviousSelection = CurrentMode->GetSelectedObject();
		if (!CurrentMode->Enter())
		{
			return FinishWithError(TEXT("The editing mode could not enter for screenshot capture."));
		}
		FActorSpawnParameters Parameters;
		Parameters.Name = MakeUniqueObjectName(World->PersistentLevel, AStaticMeshActor::StaticClass(),
		                                       TEXT("Viewport_Attribute_Sample"));
		AStaticMeshActor* Target = World->SpawnActor<AStaticMeshActor>(Parameters);
		if (!Target)
		{
			return FinishWithError(TEXT("Could not create the native screenshot fixture Actor."));
		}
		Source = Target;
		Target->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
		FVector ViewLocation;
		FRotator ViewRotation;
		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
		Target->SetActorLocation(ViewLocation + ViewRotation.Vector() * 500.0);
		Hub->SetSelectedObject(Target);
		CurrentWidget->SetMonitorVisible(false);
		CaptureId = FGuid::NewGuid().ToString(EGuidFormats::Digits);
		ReadyFrame = GFrameCounter + 3;
		EnterStage(ECaptureStage::DetailsLayout);
		return false;
	}

	void RequestCapture(const TCHAR* Label)
	{
		const FString Directory = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() / TEXT("Screenshots"));
		IFileManager::Get().MakeDirectory(*Directory, true);
		ScreenshotPath = Directory / FString::Printf(TEXT("OWT_%s_%s.png"), Label, *CaptureId);
		FScreenshotRequest::RequestScreenshot(ScreenshotPath, true, false);
		ScreenshotPath = FScreenshotRequest::GetFilename();
		bRequestedScreenshot = true;
	}

	bool CaptureFileExists() const
	{
		return IFileManager::Get().FileSize(*ScreenshotPath) > 0;
	}

	bool FinishWithError(const FString& Error)
	{
		Test.AddError(Error);
		Cleanup();
		return true;
	}

	void Cleanup()
	{
		if (bCleaned)
		{
			return;
		}
		bCleaned = true;
		if (Editor.IsValid())
		{
			for (const FOWTDuplicationOperationSnapshot& Operation : Editor->GetDuplicationOperations())
			{
				if (Operation.OperationId == OperationId)
				{
					Duplicate = Operation.DuplicateActor;
					break;
				}
			}
		}
		if (bRequestedScreenshot)
		{
			if (FScreenshotRequest::GetFilename() == ScreenshotPath)
			{
				FScreenshotRequest::Reset();
			}
		}
		if (UOWTAttributeEditMode* CurrentMode = Mode.Get())
		{
			if (CurrentMode->HasPendingDuplicate())
			{
				CurrentMode->CancelPendingDuplicate();
			}
			if (bPreviouslyEntered)
			{
				FString Error;
				CurrentMode->StartTool(CurrentMode->DefaultToolId, Error);
				CurrentMode->SetSelectedObject(PreviousSelection.Get());
			}
			else
			{
				CurrentMode->Exit();
			}
		}
		if (Widget.IsValid())
		{
			Widget->SetMonitorVisible(bPreviousMonitor);
		}
		if (Duplicate.IsValid())
		{
			Duplicate->Destroy();
		}
		if (Source.IsValid())
		{
			Source->Destroy();
		}
	}

	FAutomationTestBase& Test;
	TWeakObjectPtr<UOWTAttributeEditMode> Mode;
	TWeakObjectPtr<AVTBAttributeEditor> Editor;
	TWeakObjectPtr<UOWTAttributeDetailsWidget> Widget;
	TWeakObjectPtr<AActor> Source;
	TWeakObjectPtr<AActor> Duplicate;
	TWeakObjectPtr<AActor> PreviousSelection;
	FGuid OperationId;
	FString CaptureId;
	FString ScreenshotPath;
	ECaptureStage Stage = ECaptureStage::WaitingForViewport;
	double Deadline = 0;
	uint64 ReadyFrame = 0;
	bool bPreviouslyEntered = false;
	bool bPreviousMonitor = false;
	bool bRequestedScreenshot = false;
	bool bCleaned = false;
};
} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTViewportMonitorCaptureTest, "OWT.Runtime.ViewportMonitorCapture",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTViewportMonitorCaptureTest::RunTest(const FString& Parameters)
{
	if (GUsingNullRHI)
	{
		AddInfo(TEXT("SKIPPED: NullRHI cannot capture the real viewport and Slate sidebar. Run with an active RHI and "
		             "the native runtime editor controller."));
		return true;
	}
	FAutomationTestFramework::Get().EnqueueLatentCommand(MakeShared<FOWTViewportMonitorCaptureCommand>(*this));
	return true;
}

#endif
