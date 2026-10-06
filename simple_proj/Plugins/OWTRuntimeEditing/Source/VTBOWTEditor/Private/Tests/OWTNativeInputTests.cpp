#if WITH_DEV_AUTOMATION_TESTS

#include "VTBOWTSpectator.h"
#include "VTBOWTEditorGameMode.h"
#include "VTBOWTEditorPlayerController.h"
#include "VTBOWTEditorSubsystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"
#include "EnhancedInputSubsystemInterface.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputKeyEventArgs.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"

namespace
{
/** Uses the engine's mapping rebuild and key/trigger processing without requiring a viewport. */
class FOWTInputPipeline : public IEnhancedInputSubsystemInterface
{
public:
	explicit FOWTInputPipeline(UEnhancedPlayerInput& InPlayerInput) : PlayerInput(InPlayerInput), InjectedInputs()
	{
	}

	virtual UEnhancedPlayerInput* GetPlayerInput() const override
	{
		return &PlayerInput;
	}
	virtual TMap<TObjectPtr<const UInputAction>, FInjectedInput>& GetContinuouslyInjectedInputs() override
	{
		return InjectedInputs;
	}

private:
	UEnhancedPlayerInput& PlayerInput;
	TMap<TObjectPtr<const UInputAction>, FInjectedInput> InjectedInputs;
};

int32 TriggerActionForKey(AVTBOWTSpectator& Pawn, FKey Key)
{
	TSet<const UInputAction*> Actions;
	for (const FEnhancedActionKeyMapping& Mapping : Pawn.MappingContext->GetMappings())
	{
		if (Mapping.Key == Key)
		{
			Actions.Add(Mapping.Action);
		}
	}

	int32 Count = 0;
	for (const TUniquePtr<FEnhancedInputActionEventBinding>& Binding : Pawn.EnhancedInput->GetActionEventBindings())
	{
		if (Binding->GetTriggerEvent() != ETriggerEvent::Triggered)
		{
			continue;
		}
		if (!Actions.Contains(Binding->GetAction()))
		{
			continue;
		}

		Binding->Execute(FInputActionInstance(Binding->GetAction()));
		++Count;
	}
	return Count;
}
} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTNativeInputTest, "OWT.Runtime.NativeInput",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTNativeInputTest::RunTest(const FString& Parameters)
{
	const AVTBOWTEditorGameMode* Mode = GetDefault<AVTBOWTEditorGameMode>();
	TestEqual(TEXT("Default gameplay pawn is native"), Mode->DefaultPawnClass.Get(), AVTBOWTSpectator::StaticClass());
	TestEqual(TEXT("Default spectator is native"), Mode->SpectatorClass.Get(), AVTBOWTSpectator::StaticClass());
	TestEqual(TEXT("Controller provides Enhanced Input without project config"),
	          GetDefault<AVTBOWTEditorPlayerController>()->GetOverridePlayerInputClass().Get(),
	          UEnhancedPlayerInput::StaticClass());

	const UWorld::InitializationValues InitValues = UWorld::InitializationValues()
	                                                    .AllowAudioPlayback(false)
	                                                    .CreatePhysicsScene(true)
	                                                    .CreateNavigation(false)
	                                                    .CreateAISystem(false)
	                                                    .ShouldSimulatePhysics(false);
	UWorld* World =
	    UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &InitValues);
	if (!TestNotNull(TEXT("Input test world"), World))
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	UVTBOWTEditorSubsystem* Hub = World->GetSubsystem<UVTBOWTEditorSubsystem>();
	ON_SCOPE_EXIT
	{
		if (Hub)
		{
			Hub->ShutdownToolsContext();
		}
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
	};
	if (!TestNotNull(TEXT("Editing subsystem"), Hub))
	{
		return false;
	}
	AVTBOWTSpectator* Pawn = World->SpawnActor<AVTBOWTSpectator>();
	if (!TestNotNull(TEXT("Native spectator"), Pawn))
	{
		return false;
	}
	Pawn->SetupPlayerInputComponent(Pawn->EnhancedInput);
	if (!TestNotNull(TEXT("Missing asset mapping creates a fallback"), Pawn->MappingContext.Get()))
	{
		return false;
	}
	TestEqual(TEXT("Fallback mapping belongs to this pawn"), Pawn->MappingContext->GetOuter(),
	          static_cast<UObject*>(Pawn));
	TestTrue(TEXT("Fallback is transient"), Pawn->MappingContext->HasAnyFlags(RF_Transient));
	TestEqual(TEXT("Fallback contains edit shortcuts, redo alternate and camera mappings"),
	          Pawn->MappingContext->GetMappings().Num(), 18);
	for (const FEnhancedActionKeyMapping& Mapping : Pawn->MappingContext->GetMappings())
	{
		TestEqual(TEXT("Input actions are created locally, with no project asset dependency"),
		          Mapping.Action->GetOuter(), static_cast<UObject*>(Pawn->MappingContext.Get()));
	}
	TestNotNull(TEXT("RMB navigation action exists"), Pawn->CameraNavigateAction.Get());
	TestNotNull(TEXT("Camera movement action exists"), Pawn->CameraMoveAction.Get());
	TestNotNull(TEXT("Camera look action exists"), Pawn->CameraLookAction.Get());
	TestFalse(TEXT("Editing initially disabled"), Hub->IsEditingEnabled());
	TestEqual(TEXT("F2 dispatches exactly one callback"), TriggerActionForKey(*Pawn, EKeys::F2), 1);
	TestTrue(TEXT("Generated F2 action enables editing"), Hub->IsEditingEnabled());

	UInputMappingContext* OriginalMapping = Pawn->MappingContext;
	Pawn->SetupPlayerInputComponent(Pawn->EnhancedInput);
	TestEqual(TEXT("Repeated setup reuses its mapping"), Pawn->MappingContext.Get(), OriginalMapping);
	TestEqual(TEXT("Repeated setup retains one F2 callback"), TriggerActionForKey(*Pawn, EKeys::F2), 1);
	TestFalse(TEXT("Repeated setup does not double-toggle editing"), Hub->IsEditingEnabled());
	TriggerActionForKey(*Pawn, EKeys::F2);
	TestEqual(TEXT("Generated E action dispatches once"), TriggerActionForKey(*Pawn, EKeys::E), 1);
	TestEqual(TEXT("Generated E action selects rotation"), Hub->GetTransformGizmoMode(),
	          EToolContextTransformGizmoMode::Rotation);
	TestEqual(TEXT("Generated R action dispatches once"), TriggerActionForKey(*Pawn, EKeys::R), 1);
	TestEqual(TEXT("Generated R action selects scale"), Hub->GetTransformGizmoMode(),
	          EToolContextTransformGizmoMode::Scale);

	AVTBOWTSpectator* CustomPawn = World->SpawnActor<AVTBOWTSpectator>();
	if (!TestNotNull(TEXT("Custom input spectator"), CustomPawn))
	{
		return false;
	}
	UInputMappingContext* CustomMapping = NewObject<UInputMappingContext>(CustomPawn);
	CustomPawn->MappingContext = CustomMapping;
	CustomPawn->CameraNavigateAction = NewObject<UInputAction>(CustomMapping);
	CustomPawn->CameraMoveAction = NewObject<UInputAction>(CustomMapping);
	CustomPawn->CameraLookAction = NewObject<UInputAction>(CustomMapping);
	UInputAction* CustomEdit = NewObject<UInputAction>(CustomMapping);
	CustomMapping->MapKey(CustomEdit, EKeys::F2);
	int32 CustomCount = 0;
	CustomPawn->EnhancedInput->BindActionValueLambda(CustomEdit, ETriggerEvent::Triggered,
	                                                 [&CustomCount](const FInputActionValue&)
	                                                 {
		                                                 ++CustomCount;
	                                                 });
	CustomPawn->SetupPlayerInputComponent(CustomPawn->EnhancedInput);
	CustomPawn->SetupPlayerInputComponent(CustomPawn->EnhancedInput);
	TestEqual(TEXT("Supplied mapping is preserved"), CustomPawn->MappingContext.Get(), CustomMapping);
	TestEqual(TEXT("Supplied mapping gains no fallback keys"), CustomMapping->GetMappings().Num(), 1);
	TestEqual(TEXT("Existing binding survives native setup without duplication"),
	          TriggerActionForKey(*CustomPawn, EKeys::F2), 1);
	TestEqual(TEXT("Custom input callback runs once"), CustomCount, 1);

	AVTBOWTEditorPlayerController* Controller = World->SpawnActor<AVTBOWTEditorPlayerController>();
	if (!TestNotNull(TEXT("Input pipeline controller"), Controller))
	{
		return false;
	}
	UEnhancedPlayerInput* PlayerInput = NewObject<UEnhancedPlayerInput>(Controller);
	Controller->PlayerInput = PlayerInput;
	Controller->SetupInputComponent();
	for (const FKeyBind& Binding : PlayerInput->DebugExecBindings)
	{
		if (Binding.Key == EKeys::F2 || Binding.Key == EKeys::F3)
		{
			TestTrue(TEXT("Editor shortcut disables conflicting engine viewmode binding"), Binding.bDisabled);
		}
	}
	FOWTInputPipeline Pipeline(*PlayerInput);
	FModifyContextOptions Options;
	Options.bForceImmediately = true;
	Pipeline.AddMappingContext(Pawn->MappingContext, 100, Options);
	const TArray<UInputComponent*> InputStack = {Pawn->EnhancedInput, Controller->InputComponent};
	constexpr float InputFrameSeconds = 1.0f / 60.0f;
	PlayerInput->ProcessInputStack(InputStack, InputFrameSeconds, false);
	const bool bEditingBeforePhysicalInput = Hub->IsEditingEnabled();
	PlayerInput->InputKey(FInputKeyEventArgs::CreateSimulated(EKeys::F2, IE_Pressed, 1.0f));
	PlayerInput->ProcessInputStack(InputStack, InputFrameSeconds, false);
	TestEqual(TEXT("F2 key passes through mapping, Pressed trigger and input stack"), Hub->IsEditingEnabled(),
	          !bEditingBeforePhysicalInput);
	PlayerInput->ProcessInputStack(InputStack, InputFrameSeconds, false);
	TestEqual(TEXT("Holding F2 does not repeatedly toggle editing"), Hub->IsEditingEnabled(),
	          !bEditingBeforePhysicalInput);
	PlayerInput->InputKey(FInputKeyEventArgs::CreateSimulated(EKeys::F2, IE_Released, 0.0f));
	PlayerInput->ProcessInputStack(InputStack, InputFrameSeconds, false);
	PlayerInput->InputKey(FInputKeyEventArgs::CreateSimulated(EKeys::F2, IE_Pressed, 1.0f));
	PlayerInput->ProcessInputStack(InputStack, InputFrameSeconds, false);
	TestEqual(TEXT("Repressing F2 toggles editing once more"), Hub->IsEditingEnabled(), bEditingBeforePhysicalInput);
	PlayerInput->InputKey(FInputKeyEventArgs::CreateSimulated(EKeys::F2, IE_Released, 0.0f));
	PlayerInput->ProcessInputStack(InputStack, InputFrameSeconds, false);
	const bool bBeforeSameFrameTap = Hub->IsEditingEnabled();
	PlayerInput->InputKey(FInputKeyEventArgs::CreateSimulated(EKeys::F2, IE_Pressed, 1.0f));
	PlayerInput->InputKey(FInputKeyEventArgs::CreateSimulated(EKeys::F2, IE_Released, 0.0f));
	PlayerInput->ProcessInputStack(InputStack, InputFrameSeconds, false);
	AddInfo(
	    FString::Printf(TEXT("Enhanced Input same-frame keyboard press/release observation: %s"),
	                    Hub->IsEditingEnabled() != bBeforeSameFrameTap ? TEXT("triggered") : TEXT("not triggered")));
	Pipeline.RemoveMappingContext(Pawn->MappingContext, Options);
	return !HasAnyErrors();
}

#endif
