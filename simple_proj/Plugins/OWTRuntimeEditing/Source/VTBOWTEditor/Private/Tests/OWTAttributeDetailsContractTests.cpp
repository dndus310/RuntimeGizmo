#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "UI/OWTAttributeDetailsWidget.h"
#include "UI/OWTAttributeMonitorTypes.h"
#include "OWTStateMonitorModel.h"
#include "SOWTStateMonitor.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorSubsystem.h"
#include "Modes/OWTAttributeEditMode.h"
#include "Tools/OWTAttributeEditTool.h"
#include "Events/OWTEventTypes.h"
#include "InputCoreTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "UObject/StrongObjectPtr.h"
#include "Widgets/SWidget.h"
#include "Widgets/SBoxPanel.h"

namespace OWTDetailsContract
{
TSharedPtr<FOWTStateMonitorNode> FindNode(const TSharedPtr<FOWTStateMonitorNode>& Node, const FString& Path)
{
	if (!Node.IsValid())
	{
		return nullptr;
	}
	if (Node->Path == Path)
	{
		return Node;
	}
	for (const TSharedPtr<FOWTStateMonitorNode>& Child : Node->Children)
	{
		TSharedPtr<FOWTStateMonitorNode> Found = FindNode(Child, Path);
		if (Found.IsValid())
		{
			return Found;
		}
	}
	return nullptr;
}
}

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
	UOWTAttributeEditMode* Mode = Hub->GetAttributeEditMode();
	if (!TestNotNull(TEXT("Attribute edit mode"), Mode))
	{
		return false;
	}
	Widget->RefreshSessionState();
	TestTrue(TEXT("Tools are supplied by the mode registry"), Widget->AvailableTools.Num() > 0);
	TestTrue(TEXT("Tool rows are constructed"), Widget->ToolRows.IsValid());
	TestEqual(TEXT("Each registry entry has one UI row"), Widget->ToolRows->NumSlots(), Widget->AvailableTools.Num());
	const int32 BuiltInToolCount = Widget->AvailableTools.Num();
	FOWTToolDescriptor ExtraTool;
	ExtraTool.ToolId = TEXT("Contract.ExtraTool");
	ExtraTool.Label = FText::FromString(TEXT("Contract extra tool"));
	ExtraTool.Category = FText::FromString(TEXT("Contract tools"));
	ExtraTool.BuilderClass = UOWTAttributeEditToolBuilder::StaticClass();
	FGuid ToolRegistration;
	FString RegistrationError;
	TestTrue(TEXT("A provider can add a tool without changing the widget"),
	         Mode->RegisterTool(ExtraTool, TEXT("Contract.UI"), ToolRegistration, RegistrationError));
	Widget->RefreshSessionState();
	TestEqual(TEXT("Provider tool appears in registry-driven UI"), Widget->ToolRows->NumSlots(), BuiltInToolCount + 1);
	TestTrue(TEXT("Custom tool id uses the same availability path"), Widget->CanStartTool(ExtraTool.ToolId));
	TestTrue(TEXT("Provider registration is removable"),
	         Mode->UnregisterProvider(TEXT("Contract.UI"), RegistrationError));
	Widget->RefreshSessionState();
	TestEqual(TEXT("Removing a provider removes its UI row"), Widget->ToolRows->NumSlots(), BuiltInToolCount);
	TestFalse(TEXT("Removed tools cannot be started from a stale button"), Widget->CanStartTool(ExtraTool.ToolId));
	Widget->SetMonitorVisible(true);
	TestTrue(TEXT("Monitor opens through the public widget API"), Widget->IsMonitorVisible());
	TestTrue(TEXT("Monitor constructs the reusable viewer"), Widget->MonitorView.IsValid());
	TSharedPtr<FOWTStateMonitorSource> CurrentSource = Widget->MonitorModel->FindSource(TEXT("Current"));
	TSharedPtr<FOWTStateMonitorSource> EventSource = Widget->MonitorModel->FindSource(TEXT("Events"));
	if (!TestTrue(TEXT("Authoritative state and transport events are separate sources"),
		CurrentSource.IsValid() && EventSource.IsValid()))
	{
		return false;
	}
	TestTrue(TEXT("Monitor loads the existing event journal"), EventSource->CurrentJson.Contains(TEXT("Journal")));
	TestTrue(TEXT("Journal snapshots do not duplicate event history"), EventSource->History.IsEmpty());
	TestEqual(TEXT("Monitor catches up to the facade sequence"), Widget->LastMonitorSequence,
		Editor->GetLatestEventSequence());
	const uint64 BeforePausedSequence = CurrentSource->Sequence;
	Widget->MonitorView->SetHistoryPaused(true);
	TestTrue(TEXT("History display can be paused independently"), Widget->MonitorView->IsHistoryPaused());
	TestTrue(TEXT("Editor remains usable while history is paused"),
		Editor->RequestTransformField(Editor->GetSnapshot(), EOWTTransformField::LocationX, 125.0,
			EOWTTransformEditPhase::Commit, FGuid::NewGuid()));
	Widget->RefreshMonitorState(true);
	CurrentSource = Widget->MonitorModel->FindSource(TEXT("Current"));
	TestTrue(TEXT("Current monitor snapshot advances while history is paused"), CurrentSource->Sequence > BeforePausedSequence);
	TestEqual(TEXT("Live details state advances while history is paused"), Widget->Snapshot.Transform.GetLocation().X, 125.0);
	TestTrue(TEXT("Reflected current state contains the new transform"), CurrentSource->CurrentJson.Contains(TEXT("125")));
	TestEqual(TEXT("Journal continues receiving transport evidence while history is paused"),
		Widget->LastMonitorSequence, Editor->GetLatestEventSequence());
	Widget->MonitorView->SetHistoryPaused(false);
	TestFalse(TEXT("Resume enables live history display"), Widget->MonitorView->IsHistoryPaused());
	const int32 JournalCount = Editor->GetMonitorEntries().Num();
	const FString CurrentBeforeClear = CurrentSource->CurrentJson;
	Widget->MonitorModel->ClearHistory(TEXT("Current"));
	CurrentSource = Widget->MonitorModel->FindSource(TEXT("Current"));
	TestTrue(TEXT("Clear history removes retained changes"), CurrentSource->History.IsEmpty());
	TestEqual(TEXT("Clear history preserves the authoritative current snapshot"), CurrentSource->CurrentJson, CurrentBeforeClear);
	TestEqual(TEXT("Clear history retains the underlying event journal"), Editor->GetMonitorEntries().Num(), JournalCount);
	Widget->RefreshMonitorState(true);
	TestTrue(TEXT("Unchanged snapshot does not recreate cleared history"),
		Widget->MonitorModel->FindSource(TEXT("Current"))->History.IsEmpty());

	Hub->ToggleEditing();
	TestFalse(TEXT("Editing is disabled independently of monitor"), Widget->Snapshot.bEditingEnabled);
	TestTrue(TEXT("Monitor stays visible with editing disabled"), Widget->IsVisible());
	TestFalse(TEXT("Monitor cannot enable transform edits while editing is disabled"), Widget->CanEditFields());
	TestFalse(TEXT("Monitor cannot duplicate while editing is disabled"), Widget->CanDuplicateSelection());
	Widget->RefreshSessionState();
	TestFalse(TEXT("Mode monitor observes editing disabled"), Widget->ModeSnapshot.bEditingEnabled);
	for (const FOWTToolAvailability& Tool : Widget->AvailableTools)
	{
		TestFalse(TEXT("Registry tools stay disabled in the off-mode monitor"), Widget->CanStartTool(Tool.ToolId));
	}
	Widget->RefreshMonitorState(true);
	TestEqual(TEXT("Editor-off transport evidence remains available"), Widget->LastMonitorSequence, Editor->GetLatestEventSequence());
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

	// Exiting the mode cleared its selection. Reopening it must not silently restore an old target.
	TestFalse(TEXT("Reopening edit mode keeps the old selection cleared"), Editor->GetSnapshot().bHasSelection);
	Hub->SetSelectedObject(Target);
	Widget->RefreshSessionState();
	TestTrue(TEXT("A fresh selection enables the registry duplicate entry"),
	         Widget->CanStartTool(OWTToolIds::Duplicate()));
	Widget->SetMonitorVisible(true);
	Widget->MonitorView->SetHistoryPaused(true);
	const uint64 BeforeDuplicateSequence = Widget->MonitorModel->FindSource(TEXT("Current"))->Sequence;
	TestTrue(TEXT("Duplicate tool accepts a UI request"), Editor->RequestStartTool(OWTToolIds::Duplicate()));
	Widget->RefreshMonitorState(true);
	TestTrue(TEXT("Accepted duplication is observed without a JSON state parser"),
		Editor->GetDuplicationOperations().Num() > 0);
	Hub->Tick(0.016f);
	Widget->RefreshSessionState();
	Widget->RefreshMonitorState(true);
	CurrentSource = Widget->MonitorModel->FindSource(TEXT("Current"));
	TestTrue(TEXT("Current state advances during duplication while history is paused"), CurrentSource->Sequence > BeforeDuplicateSequence);
	const TArray<FOWTDuplicationOperationSnapshot> Duplications = Editor->GetDuplicationOperations();
	if (!Duplications.IsEmpty())
	{
		const FOWTDuplicationOperationSnapshot& Duplicate = Duplications.Last();
		TestEqual(TEXT("Live typed operation reaches authored commit while history is paused"), Duplicate.Phase,
			EOWTDuplicationPhase::Committed);
		TestTrue(TEXT("Live operation names its resulting actor"), Duplicate.DuplicateActor.IsValid());
		TestTrue(TEXT("Generic monitor includes the duplication operation fields"),
			OWTDetailsContract::FindNode(CurrentSource->Root, TEXT("$/DuplicationOperations/0/OperationId")).IsValid());
		TestTrue(TEXT("Generic monitor includes the resulting object id"), CurrentSource->CurrentJson.Contains(Duplicate.DuplicateObjectId));
	}
	Widget->MonitorView->SetHistoryPaused(false);
	Hub->SetSelectedObject(Target);

	// Presentation of component state must not collapse a waiting component into another component's success.
	FOWTProceduralComponentSnapshot Waiting;
	Waiting.ComponentId = FGuid::NewGuid();
	Waiting.OperationId = FGuid::NewGuid();
	Waiting.ComponentName = TEXT("WaitingPCG");
	Waiting.GraphPath = TEXT("/Contract/GraphA");
	Waiting.Trigger = TEXT("GenerateAtRuntime");
	Waiting.State = EOWTProceduralState::WaitingForGenerationSource;
	Waiting.GenerationAttempt = 2;
	Waiting.Reason = TEXT("No generation source is in range.");
	FOWTProceduralComponentSnapshot Ready = Waiting;
	Ready.ComponentId = FGuid::NewGuid();
	Ready.ComponentName = TEXT("ReadyPCG");
	Ready.State = EOWTProceduralState::Ready;
	Ready.Reason.Reset();
	FOWTAttributeMonitorSnapshot ComponentFixture;
	ComponentFixture.ProceduralComponents = {Waiting, Ready};
	Widget->MonitorModel->SubmitSnapshot(TEXT("Contract.Components"), FText::FromString(TEXT("Component contract")),
		FOWTAttributeMonitorSnapshot::StaticStruct(), &ComponentFixture, false);
	const TSharedPtr<FOWTStateMonitorSource> Components = Widget->MonitorModel->FindSource(TEXT("Contract.Components"));
	const FString ComponentStatus = Components->CurrentJson;
	TestTrue(TEXT("Component monitor retains the waiting reason"), ComponentStatus.Contains(Waiting.Reason));
	TestTrue(TEXT("Component monitor identifies the waiting component"), ComponentStatus.Contains(Waiting.ComponentName));
	TestTrue(TEXT("Component monitor identifies the ready component independently"), ComponentStatus.Contains(Ready.ComponentName));
	TestTrue(TEXT("Component monitor includes graph"), ComponentStatus.Contains(Waiting.GraphPath));
	TestTrue(TEXT("Component monitor includes trigger"), ComponentStatus.Contains(Waiting.Trigger));
	const TSharedPtr<FOWTStateMonitorNode> Attempt = OWTDetailsContract::FindNode(Components->Root,
		TEXT("$/ProceduralComponents/0/GenerationAttempt"));
	TestTrue(TEXT("Generic reflection retains the generation attempt"), Attempt.IsValid());
	if (Attempt.IsValid())
	{
		TestEqual(TEXT("Reflected generation attempt is unchanged"), Attempt->Value, FString(TEXT("2")));
	}
	Widget->MonitorModel->RemoveSource(TEXT("Contract.Components"));
	Widget->RefreshSessionState();

	Widget->SetAttributeEditor(nullptr);
	TestTrue(TEXT("Detaching the facade clears its reflected monitoring sources"), Widget->MonitorModel->GetSources().IsEmpty());
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
