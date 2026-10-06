#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "UI/OWTAttributeDetailsWidget.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorSubsystem.h"
#include "Events/OWTEventTypes.h"
#include "InputCoreTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "UObject/StrongObjectPtr.h"
#include "Widgets/SWidget.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTAttributeDetailsContractTest, "OWT.Runtime.AttributeDetailsContracts",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTAttributeDetailsContractTest::RunTest(const FString& Parameters)
{
	const UWorld::InitializationValues InitValues = UWorld::InitializationValues()
	                                                    .AllowAudioPlayback(false)
	                                                    .CreatePhysicsScene(true)
	                                                    .CreateNavigation(false)
	                                                    .CreateAISystem(false)
	                                                    .ShouldSimulatePhysics(false);
	UWorld* World =
	    UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &InitValues);
	if (!TestNotNull(TEXT("Details contract world"), World))
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	ON_SCOPE_EXIT
	{
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
	};

	UVTBOWTEditorSubsystem* Hub = World->GetSubsystem<UVTBOWTEditorSubsystem>();
	if (!TestNotNull(TEXT("Editing subsystem"), Hub))
	{
		return false;
	}
	AVTBAttributeEditor* Editor = World->SpawnActor<AVTBAttributeEditor>();
	if (!TestNotNull(TEXT("AttributeEditor"), Editor))
	{
		return false;
	}
	AStaticMeshActor* Target = World->SpawnActor<AStaticMeshActor>();
	if (!TestNotNull(TEXT("Selected actor"), Target))
	{
		return false;
	}
	Target->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	Editor->BindSubsystem(Hub);
	Hub->ToggleEditing();
	Hub->SetSelectedObject(Target);

	TStrongObjectPtr<UOWTAttributeDetailsWidget> Widget(NewObject<UOWTAttributeDetailsWidget>(World));
	Widget->SetAttributeEditor(Editor);
	const FOWTAttributeSnapshot BeforeDrag = Widget->Snapshot;
	TestEqual(TEXT("UI starts with the selected actor value"), BeforeDrag.Transform.GetLocation().X, 0.0);

	// No Tick/Refresh here: Begin must tolerate the pre-request External snapshot.
	Target->SetActorLocation(FVector(35.0, 45.0, 55.0));
	Widget->OnSliderBegin(EOWTTransformField::LocationX);
	TestTrue(TEXT("Pre-Begin external refresh preserves the UI operation"), Widget->bSliderActive);
	TestTrue(TEXT("UI operation retains a valid id"), Widget->OperationId.IsValid());
	TestTrue(TEXT("Core operation began"), Editor->GetSnapshot().bIsModifying);
	TestEqual(TEXT("Begin reads the actual external transform"), Widget->Snapshot.Transform.GetLocation().X, 35.0);
	TestEqual(TEXT("Selection revision remains unchanged"), Widget->Snapshot.SelectionRevision,
	          BeforeDrag.SelectionRevision);

	Widget->OnSliderValueChanged(90.0, EOWTTransformField::LocationX);
	TestEqual(TEXT("Slider Update reaches Actor after external refresh"), Target->GetActorLocation().X, 90.0);
	TestEqual(TEXT("Slider changes only its field"), Target->GetActorLocation().Y, 45.0);
	Widget->OnSliderEnd(100.0, EOWTTransformField::LocationX);
	TestEqual(TEXT("Slider Commit reaches Actor"), Target->GetActorLocation().X, 100.0);
	TestTrue(TEXT("Slider keeps the selected Actor"), Hub->SelectedObject.Get() == Target);
	TestFalse(TEXT("UI operation ends"), Widget->bSliderActive);
	TestFalse(TEXT("No core operation is stranded"), Editor->GetSnapshot().bIsModifying);

	// IsVisible queries the cached Slate widget, so exercise the constructed UI rather than only its UObject flags.
	const TSharedRef<SWidget> SlateWidget = Widget->TakeWidget();
	TestTrue(TEXT("Native sidebar is visible while editing"), SlateWidget->GetVisibility().IsVisible());
	TestTrue(TEXT("Native sidebar panel is constructed"), Widget->Panel.IsValid());
	Widget->SetMonitorVisible(true);
	TestTrue(TEXT("Monitor opens through the public widget API"), Widget->IsMonitorVisible());
	TestTrue(TEXT("Monitor loads existing event journal entries"), Widget->MonitorEntries.Num() > 0);
	TestEqual(TEXT("Monitor catches up to the facade sequence"), Widget->LastMonitorSequence,
	          Editor->GetLatestEventSequence());
	const int64 BeforePausedSequence = Widget->LastMonitorSequence;
	const int32 BeforePausedCount = Widget->MonitorEntries.Num();
	Widget->OnPauseMonitorClicked();
	TestTrue(TEXT("Pause freezes event display"), Widget->bMonitorPaused);
	TestTrue(TEXT("Editor remains usable while monitor is paused"),
	         Editor->RequestTransformField(Editor->GetSnapshot(), EOWTTransformField::LocationX, 125.0,
	                                       EOWTTransformEditPhase::Commit, FGuid::NewGuid()));
	Widget->RefreshMonitorHistory();
	TestEqual(TEXT("Paused history keeps its sequence"), Widget->LastMonitorSequence, BeforePausedSequence);
	TestEqual(TEXT("Paused history keeps its entries"), Widget->MonitorEntries.Num(), BeforePausedCount);
	TestEqual(TEXT("Live state advances even while event display is paused"),
	          Widget->Snapshot.Transform.GetLocation().X, 125.0);
	Widget->OnPauseMonitorClicked();
	TestFalse(TEXT("Resume enables live history"), Widget->bMonitorPaused);
	TestTrue(TEXT("Resume catches events emitted while paused"), Widget->LastMonitorSequence > BeforePausedSequence);
	Widget->OnMonitorFilterChanged(FText::FromString(TEXT("NoSuchMonitorEvent_ContractTest")));
	TestEqual(TEXT("Filter removes unmatched entries"), Widget->FilteredEntries.Num(), 0);
	Widget->OnMonitorFilterChanged(FText::GetEmpty());
	TestEqual(TEXT("Removing the filter restores displayed entries"), Widget->FilteredEntries.Num(),
	          Widget->MonitorEntries.Num());
	const int32 JournalCount = Editor->GetMonitorEntries().Num();
	Widget->OnClearMonitorClicked();
	TestEqual(TEXT("Clear view removes displayed entries"), Widget->MonitorEntries.Num(), 0);
	TestEqual(TEXT("Clear view retains the underlying journal"), Editor->GetMonitorEntries().Num(), JournalCount);
	Widget->RefreshMonitorHistory(true);
	TestEqual(TEXT("Cleared entries do not reappear on refresh"), Widget->MonitorEntries.Num(), 0);

	Hub->ToggleEditing();
	TestFalse(TEXT("Editing is disabled independently of monitor"), Widget->Snapshot.bEditingEnabled);
	TestTrue(TEXT("Monitor stays visible with editing disabled"), Widget->IsVisible());
	TestFalse(TEXT("Monitor cannot enable transform edits while editing is disabled"), Widget->CanEditFields());
	TestFalse(TEXT("Monitor cannot duplicate while editing is disabled"), Widget->CanDuplicateSelection());
	Widget->RefreshMonitorHistory();
	TestTrue(TEXT("Editor-off event appears after clear view"), Widget->MonitorEntries.Num() > 0);
	Widget->ToggleMonitor();
	TestFalse(TEXT("F3-equivalent toggle closes the monitor"), Widget->IsMonitorVisible());
	TestFalse(TEXT("Panel closes when neither edit nor monitor is enabled"), Widget->IsVisible());
	Widget->ToggleMonitor();
	TestTrue(TEXT("Monitor can be reopened while editing is disabled"), Widget->IsVisible());
	TestFalse(TEXT("Opening the monitor does not enable editing"), Editor->GetSnapshot().bEditingEnabled);

	// Focused descendants route their keys through this preview handler; repeats must not act like new presses.
	const FGeometry KeyGeometry;
	const FKeyEvent F2Pressed(EKeys::F2, FModifierKeysState(), 0, false, 0, 0);
	const FKeyEvent F2Repeated(EKeys::F2, FModifierKeysState(), 0, true, 0, 0);
	const FKeyEvent F3Pressed(EKeys::F3, FModifierKeysState(), 0, false, 0, 0);
	const FKeyEvent F3Repeated(EKeys::F3, FModifierKeysState(), 0, true, 0, 0);
	TestTrue(TEXT("Focused monitor consumes an F2 press"),
	         Widget->NativeOnPreviewKeyDown(KeyGeometry, F2Pressed).IsEventHandled());
	TestTrue(TEXT("One F2 press enables editing"), Editor->GetSnapshot().bEditingEnabled);
	TestTrue(TEXT("Focused monitor consumes repeated F2"),
	         Widget->NativeOnPreviewKeyDown(KeyGeometry, F2Repeated).IsEventHandled());
	TestTrue(TEXT("Repeated F2 does not toggle editing off"), Editor->GetSnapshot().bEditingEnabled);
	TestTrue(TEXT("Focused monitor consumes repeated F3"),
	         Widget->NativeOnPreviewKeyDown(KeyGeometry, F3Repeated).IsEventHandled());
	TestTrue(TEXT("Repeated F3 does not close the monitor"), Widget->IsMonitorVisible());
	TestTrue(TEXT("Focused monitor consumes an F3 press"),
	         Widget->NativeOnPreviewKeyDown(KeyGeometry, F3Pressed).IsEventHandled());
	TestFalse(TEXT("One F3 press returns to Details"), Widget->IsMonitorVisible());
	TestTrue(TEXT("Focused Details consumes repeated F3"),
	         Widget->NativeOnPreviewKeyDown(KeyGeometry, F3Repeated).IsEventHandled());
	TestFalse(TEXT("Repeated F3 does not reopen the monitor"), Widget->IsMonitorVisible());
	TestTrue(TEXT("Monitor shortcuts preserve editing state"), Editor->GetSnapshot().bEditingEnabled);

	Widget->SetAttributeEditor(nullptr);
	Hub->SetSelectedObject(Target);
	bool bDetachedDuringSubscribe = false;
	const FGuid DetachObserver =
	    Editor->Subscribe(Target,
	                      FOWTAttributeEventNative::CreateLambda(
	                          [&](FName Event, const FString&)
	                          {
		                          if (Event == TEXT("TransformChanged") && !bDetachedDuringSubscribe)
		                          {
			                          bDetachedDuringSubscribe = true;
			                          Widget->SetAttributeEditor(nullptr);
		                          }
	                          }),
	                      false);
	Target->SetActorLocation(FVector(150.0, 45.0, 55.0));
	Widget->SetAttributeEditor(Editor);
	TestTrue(TEXT("Initial subscription refresh exercises an external detach callback"), bDetachedDuringSubscribe);
	TestFalse(TEXT("Detach during initial callback preserves detached editor binding"),
	          Widget->AttributeEditor.IsValid());
	TestFalse(TEXT("Detach during initial callback cannot install a stale handle"), Widget->Subscription.IsValid());
	Editor->Unsubscribe(DetachObserver);
	Editor->PublishRequest(TEXT("TransformEditRequested"), TEXT("{bad json"));
	TestTrue(TEXT("Detached view does not receive later rejection callbacks"), Widget->LastError.IsEmpty());

	bool bReboundDuringSubscribe = false;
	const FGuid RebindObserver =
	    Editor->Subscribe(Target,
	                      FOWTAttributeEventNative::CreateLambda(
	                          [&](FName Event, const FString&)
	                          {
		                          if (Event == TEXT("TransformChanged") && !bReboundDuringSubscribe)
		                          {
			                          bReboundDuringSubscribe = true;
			                          Widget->SetAttributeEditor(nullptr);
			                          Widget->SetAttributeEditor(Editor);
		                          }
	                          }),
	                      false);
	Target->SetActorLocation(FVector(175.0, 45.0, 55.0));
	Widget->SetAttributeEditor(Editor);
	TestTrue(TEXT("Initial subscription refresh exercises a nested rebind"), bReboundDuringSubscribe);
	TestTrue(TEXT("Nested rebind preserves the current editor"), Widget->AttributeEditor.Get() == Editor);
	TestTrue(TEXT("Nested rebind retains its own subscription"), Widget->Subscription.IsValid());
	Editor->Unsubscribe(RebindObserver);
	Target->SetActorLocation(FVector(200.0, 45.0, 55.0));
	Editor->RefreshSelectedTransform();
	TestEqual(TEXT("Nested binding continues receiving typed state updates"),
	          Widget->Snapshot.Transform.GetLocation().X, 200.0);
	Widget->SetAttributeEditor(nullptr);
	Editor->PublishRequest(TEXT("TransformEditRequested"), TEXT("{bad json"));
	TestTrue(TEXT("Unsubscribing a nested binding removes all view callbacks"), Widget->LastError.IsEmpty());

	Hub->ShutdownToolsContext();
	return !HasAnyErrors();
}

#endif
