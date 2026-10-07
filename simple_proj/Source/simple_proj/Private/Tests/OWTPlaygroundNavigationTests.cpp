#if WITH_DEV_AUTOMATION_TESTS

#include "Components/InputComponent.h"
#include "DynamicRHI.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerInput.h"
#include "HAL/PlatformTime.h"
#include "InputKeyEventArgs.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/AutomationTest.h"
#include "Playground/OWTPlaygroundGameMode.h"
#include "Playground/OWTPlaygroundHUD.h"
#include "RHIGlobals.h"
#include "UnrealClient.h"
#include "VTBOWTEditorPlayerController.h"
#include "VTBOWTEditorSubsystem.h"

namespace
{
enum class EPlaygroundNavigationStage : uint8
{
	WaitingForShowroom,
	ShowroomInput,
	HelpPressed,
	HelpHeld,
	HelpReleased,
	HelpRestored,
	WaitingForStress,
	StressInput,
	WaitingForReturn,
	ReturnedInput
};

int32 CountEditableProps(UWorld& World)
{
	int32 Count = 0;
	for (TActorIterator<AActor> It(&World); It; ++It)
	{
		if (It->IsActorBeingDestroyed())
		{
			continue;
		}
		if (It->GetParentActor())
		{
			continue;
		}
		if (It->ActorHasTag(TEXT("OWT.Playground.Editable")))
		{
			++Count;
		}
	}
	return Count;
}

class FOWTPlaygroundNavigationCommand final : public IAutomationLatentCommand
{
public:
	explicit FOWTPlaygroundNavigationCommand(FAutomationTestBase& InTest)
	    : Test(InTest), Stage(EPlaygroundNavigationStage::WaitingForShowroom), World(), PreviousWorld(), Controller(),
	      HUD(), Deadline(0), ReadyFrame(0), bInitialHelpVisible(false), bHelpKeyDown(false), bFailed(false)
	{
	}

	virtual ~FOWTPlaygroundNavigationCommand() override
	{
		ReleaseHelpKey();
	}

	virtual bool Update() override
	{
		if (Deadline == 0)
		{
			Deadline = FPlatformTime::Seconds() + 120.0;
		}
		if (FPlatformTime::Seconds() > Deadline)
		{
			return Fail(
			    FString::Printf(TEXT("Playground navigation exceeded 120 seconds at stage %d. Current map: %s."),
			                    static_cast<int32>(Stage), *GetCurrentMapName()));
		}
		if (Stage == EPlaygroundNavigationStage::WaitingForShowroom)
		{
			return WaitForMap(false, EPlaygroundNavigationStage::ShowroomInput);
		}
		if (Stage == EPlaygroundNavigationStage::WaitingForStress)
		{
			return WaitForMap(true, EPlaygroundNavigationStage::StressInput);
		}
		if (Stage == EPlaygroundNavigationStage::WaitingForReturn)
		{
			return WaitForMap(false, EPlaygroundNavigationStage::ReturnedInput);
		}
		if (GFrameCounter < ReadyFrame)
		{
			return false;
		}
		if (!HUD.IsValid())
		{
			return Fail(TEXT("The playground HUD ended outside a requested map transition."));
		}
		if (!Controller.IsValid())
		{
			return Fail(TEXT("The playground controller ended outside a requested map transition."));
		}
		if (Stage == EPlaygroundNavigationStage::ShowroomInput)
		{
			if (!CheckMapReady(54))
			{
				return true;
			}
			bInitialHelpVisible = HUD->IsHelpVisible();
			InjectHelpKey(IE_Pressed);
			Stage = EPlaygroundNavigationStage::HelpPressed;
			ReadyFrame = GFrameCounter + 3;
			return false;
		}
		if (Stage == EPlaygroundNavigationStage::HelpPressed)
		{
			if (!Test.TestEqual(TEXT("F8 input toggles help once"), HUD->IsHelpVisible(), !bInitialHelpVisible))
			{
				return Fail(TEXT("The native player input stack did not dispatch the F8 pressed binding."));
			}
			Stage = EPlaygroundNavigationStage::HelpHeld;
			ReadyFrame = GFrameCounter + 5;
			return false;
		}
		if (Stage == EPlaygroundNavigationStage::HelpHeld)
		{
			if (!Test.TestEqual(TEXT("Holding F8 does not repeat the help action"), HUD->IsHelpVisible(),
			                    !bInitialHelpVisible))
			{
				return Fail(TEXT("The playground shortcut repeated while the key was held."));
			}
			ReleaseHelpKey();
			Stage = EPlaygroundNavigationStage::HelpReleased;
			ReadyFrame = GFrameCounter + 2;
			return false;
		}
		if (Stage == EPlaygroundNavigationStage::HelpReleased)
		{
			if (!Test.TestEqual(TEXT("F8 release does not toggle help"), HUD->IsHelpVisible(), !bInitialHelpVisible))
			{
				return Fail(TEXT("The playground shortcut dispatched on release."));
			}
			InjectHelpKey(IE_Pressed);
			Stage = EPlaygroundNavigationStage::HelpRestored;
			ReadyFrame = GFrameCounter + 3;
			return false;
		}
		if (Stage == EPlaygroundNavigationStage::HelpRestored)
		{
			ReleaseHelpKey();
			if (!Test.TestEqual(TEXT("A second F8 press restores help"), HUD->IsHelpVisible(), bInitialHelpVisible))
			{
				return Fail(TEXT("The second F8 press did not dispatch exactly once."));
			}
			return RequestTransition(true, EPlaygroundNavigationStage::WaitingForStress);
		}
		if (Stage == EPlaygroundNavigationStage::StressInput)
		{
			if (!CheckMapReady(600))
			{
				return true;
			}
			return RequestTransition(false, EPlaygroundNavigationStage::WaitingForReturn);
		}
		if (!CheckMapReady(54))
		{
			return true;
		}
		Test.AddInfo(
		    TEXT("Navigation completed: Showroom (54) -> Stress (600) -> new Showroom (54). F8 was "
		         "injected through native player input; map travel used the same public HUD request as F6/F7."));
		return true;
	}

private:
	bool WaitForMap(bool bStress, EPlaygroundNavigationStage NextStage)
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
		UWorld* CurrentWorld = ViewportClient->GetWorld();
		if (!CurrentWorld)
		{
			return false;
		}
		if (!CurrentWorld->HasBegunPlay())
		{
			return false;
		}
		if (CurrentWorld->bIsTearingDown)
		{
			return false;
		}
		const FString ExpectedName = bStress ? TEXT("L_OWTStress") : TEXT("L_OWTPlayground");
		if (UGameplayStatics::GetCurrentLevelName(CurrentWorld, true) != ExpectedName)
		{
			if (Stage == EPlaygroundNavigationStage::WaitingForShowroom)
			{
				return Fail(TEXT("Navigation must start in /Game/OWTPlayground/Maps/L_OWTPlayground."));
			}
			return false;
		}
		if (CurrentWorld == PreviousWorld.Get())
		{
			return false;
		}
		AOWTPlaygroundGameMode* GameMode = Cast<AOWTPlaygroundGameMode>(CurrentWorld->GetAuthGameMode());
		if (!GameMode)
		{
			return Fail(TEXT("The loaded playground map did not install AOWTPlaygroundGameMode."));
		}
		if (!GameMode->IsPlaygroundReady())
		{
			return false;
		}
		AVTBOWTEditorPlayerController* CurrentController =
		    Cast<AVTBOWTEditorPlayerController>(CurrentWorld->GetFirstPlayerController());
		if (!CurrentController)
		{
			return false;
		}
		if (!CurrentController->IsLocalController())
		{
			return Fail(TEXT("Navigation requires the local runtime editor player controller."));
		}
		AOWTPlaygroundHUD* CurrentHUD = Cast<AOWTPlaygroundHUD>(CurrentController->GetHUD());
		if (!CurrentHUD)
		{
			return false;
		}
		if (!CurrentHUD->InputComponent)
		{
			return false;
		}
		if (!CurrentController->PlayerInput)
		{
			return false;
		}
		World = CurrentWorld;
		Controller = CurrentController;
		HUD = CurrentHUD;
		const FIntPoint Size = ViewportClient->Viewport->GetSizeXY();
		// Give the native viewport focus, then release permanent capture for normal sidebar interaction.
		CurrentController->SetInputMode(FInputModeGameOnly());
		FInputModeGameAndUI GameAndUI;
		GameAndUI.SetHideCursorDuringCapture(false);
		GameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		CurrentController->SetInputMode(GameAndUI);
		CurrentController->SetMouseLocation(24, FMath::Max(24, Size.Y - 24));
		Stage = NextStage;
		ReadyFrame = GFrameCounter + 3;
		return false;
	}

	bool CheckMapReady(int32 ExpectedProps)
	{
		UWorld* CurrentWorld = World.Get();
		if (!CurrentWorld)
		{
			Fail(TEXT("The destination world expired before its readiness checks."));
			return false;
		}
		UVTBOWTEditorSubsystem* Hub = CurrentWorld->GetSubsystem<UVTBOWTEditorSubsystem>();
		if (!Hub)
		{
			Fail(TEXT("The destination map has no runtime editing subsystem."));
			return false;
		}
		bool bValid = Test.TestEqual(TEXT("Actual top-level editable Actor count"), CountEditableProps(*CurrentWorld),
		                             ExpectedProps);
		bValid &= Test.TestTrue(TEXT("The destination starts with editing enabled"), Hub->IsEditingEnabled());
		AActor* Selected = Hub->SelectedObject.Get();
		bValid &= Test.TestNotNull(TEXT("The destination has an initial selection"), Selected);
		if (Selected)
		{
			bValid &= Test.TestTrue(TEXT("The initial selection uses the authored marker"),
			                        Selected->ActorHasTag(TEXT("OWT.Playground.InitialSelection")));
		}
		bValid &= Test.TestFalse(TEXT("The fresh HUD has no pending navigation"), HUD->IsMapChangePending());
		const FKey ShortcutKeys[] = {EKeys::F6, EKeys::F7, EKeys::F8};
		for (const FKey& Key : ShortcutKeys)
		{
			int32 BindingCount = 0;
			for (const FInputKeyBinding& Binding : HUD->InputComponent->KeyBindings)
			{
				if (Binding.Chord.Key != Key)
				{
					continue;
				}
				++BindingCount;
				bValid &= Test.TestEqual(TEXT("Playground shortcuts dispatch on pressed only"),
				                         static_cast<int32>(Binding.KeyEvent), static_cast<int32>(IE_Pressed));
				bValid &=
				    Test.TestTrue(TEXT("The playground shortcut delegate is bound"), Binding.KeyDelegate.IsBound());
			}
			bValid &= Test.TestEqual(FString::Printf(TEXT("Exactly one %s binding in the new HUD"), *Key.ToString()),
			                         BindingCount, 1);
			for (const FKeyBind& DebugBinding : Controller->PlayerInput->DebugExecBindings)
			{
				if (DebugBinding.Key == Key)
				{
					bValid &= Test.TestTrue(TEXT("Matching engine debug shortcuts are disabled"),
					                        DebugBinding.bDisabled != 0);
				}
			}
		}
		FString Reason;
		if (!HUD->CanChangePlaygroundMap(Reason))
		{
			Fail(TEXT("The destination cannot accept normal map navigation: ") + Reason);
			return false;
		}
		Test.AddInfo(FString::Printf(
		    TEXT("Ready map %s: %d top-level editable props; input bindings and native editor verified."),
		    *GetCurrentMapName(), ExpectedProps));
		if (!bValid)
		{
			Fail(TEXT("The loaded playground map failed its runtime readiness contract."));
		}
		return bValid;
	}

	bool RequestTransition(bool bStress, EPlaygroundNavigationStage WaitingStage)
	{
		FString Reason;
		if (!HUD->CanChangePlaygroundMap(Reason))
		{
			return Fail(TEXT("HUD rejected map navigation before dispatch: ") + Reason);
		}
		if (!HUD->RequestPlaygroundMap(bStress))
		{
			HUD->CanChangePlaygroundMap(Reason);
			return Fail(TEXT("HUD did not accept the cooked map request. Verify both maps are packaged. ") + Reason);
		}
		if (!Test.TestTrue(TEXT("Accepted navigation sets pending immediately"), HUD->IsMapChangePending()))
		{
			return Fail(TEXT("Map navigation did not establish its pending guard."));
		}
		Test.TestFalse(TEXT("A second navigation request is rejected while pending"),
		               HUD->RequestPlaygroundMap(bStress));
		Test.TestTrue(TEXT("The duplicate request preserves pending navigation"), HUD->IsMapChangePending());
		PreviousWorld = World;
		World.Reset();
		Controller.Reset();
		HUD.Reset();
		Stage = WaitingStage;
		return false;
	}

	void InjectHelpKey(EInputEvent Event)
	{
		check(Controller.IsValid());
		check(Controller->PlayerInput);
		const float Amount = Event == IE_Released ? 0.0f : 1.0f;
		Controller->PlayerInput->InputKey(FInputKeyEventArgs::CreateSimulated(EKeys::F8, Event, Amount));
		bHelpKeyDown = Event != IE_Released;
	}

	void ReleaseHelpKey()
	{
		if (!bHelpKeyDown)
		{
			return;
		}
		if (Controller.IsValid())
		{
			if (Controller->PlayerInput)
			{
				InjectHelpKey(IE_Released);
			}
		}
		bHelpKeyDown = false;
	}

	FString GetCurrentMapName() const
	{
		if (!GEngine)
		{
			return TEXT("none");
		}
		if (!GEngine->GameViewport)
		{
			return TEXT("none");
		}
		return UGameplayStatics::GetCurrentLevelName(GEngine->GameViewport->GetWorld(), true);
	}

	bool Fail(const FString& Message)
	{
		if (!bFailed)
		{
			Test.AddError(Message);
		}
		bFailed = true;
		ReleaseHelpKey();
		if (HUD.IsValid())
		{
			if (HUD->IsHelpVisible() != bInitialHelpVisible)
			{
				HUD->ToggleHelp();
			}
		}
		return true;
	}

	FAutomationTestBase& Test;
	EPlaygroundNavigationStage Stage;
	TWeakObjectPtr<UWorld> World;
	TWeakObjectPtr<UWorld> PreviousWorld;
	TWeakObjectPtr<AVTBOWTEditorPlayerController> Controller;
	TWeakObjectPtr<AOWTPlaygroundHUD> HUD;
	double Deadline;
	uint64 ReadyFrame;
	bool bInitialHelpVisible;
	bool bHelpKeyDown;
	bool bFailed;
};
} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTPlaygroundNavigationTest, "OWT.Playground.Navigation",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTPlaygroundNavigationTest::RunTest(const FString& Parameters)
{
	AddInfo(FString::Printf(TEXT("Playground Navigation RHI=%s; NullRHI=%s. This test still checks real game-world "
	                             "map travel under NullRHI; visual output is a separate check."),
	                        GDynamicRHI ? GDynamicRHI->GetName() : TEXT("unavailable"),
	                        GUsingNullRHI ? TEXT("true") : TEXT("false")));
	FAutomationTestFramework::Get().EnqueueLatentCommand(MakeShared<FOWTPlaygroundNavigationCommand>(*this));
	return true;
}

#endif
