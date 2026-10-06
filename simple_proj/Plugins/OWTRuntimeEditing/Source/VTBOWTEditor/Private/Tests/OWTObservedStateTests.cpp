#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Dom/JsonObject.h"
#include "Engine/Engine.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "Events/OWTEventTypes.h"
#include "Serialization/JsonSerializer.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTObservedStateTest, "OWT.Runtime.ObservedState",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTObservedStateTest::RunTest(const FString& Parameters)
{
	const UWorld::InitializationValues InitValues = UWorld::InitializationValues()
	                                                    .AllowAudioPlayback(false)
	                                                    .CreatePhysicsScene(true)
	                                                    .CreateNavigation(false)
	                                                    .CreateAISystem(false)
	                                                    .ShouldSimulatePhysics(false);
	UWorld* World =
	    UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &InitValues);
	if (!TestNotNull(TEXT("Observed state world"), World))
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	ON_SCOPE_EXIT
	{
		if (UVTBOWTEditorSubsystem* Editing = World->GetSubsystem<UVTBOWTEditorSubsystem>())
		{
			Editing->ShutdownToolsContext();
		}
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
	};

	UVTBOWTEditorSubsystem* Hub = World->GetSubsystem<UVTBOWTEditorSubsystem>();
	AVTBAttributeEditor* Editor = World->SpawnActor<AVTBAttributeEditor>();
	AStaticMeshActor* Target = World->SpawnActor<AStaticMeshActor>();
	if (!TestNotNull(TEXT("Editing subsystem"), Hub) || !TestNotNull(TEXT("Attribute editor"), Editor) ||
	    !TestNotNull(TEXT("Observed actor"), Target))
	{
		return false;
	}
	Target->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	Target->SetActorLocation(FVector(10.0, 20.0, 30.0));
	Editor->BindSubsystem(Hub);
	Hub->ToggleEditing();
	Hub->SetSelectedObject(Target);
	const FOWTAttributeSnapshot Initial = Editor->GetSnapshot();

	FOWTAttributeSnapshot Copy = Initial;
	Copy.Transform.SetLocation(FVector(900.0, 900.0, 900.0));
	Copy.ObjectName = TEXT("Caller-owned copy");
	Copy.bEditingEnabled = false;
	Copy.bHasChanges = true;
	Copy.StateRevision += 5000;
	const FOWTAttributeSnapshot AfterCopy = Editor->GetSnapshot();
	TestTrue(TEXT("Mutating a returned snapshot does not change stored transform"),
	         AfterCopy.Transform.Equals(Initial.Transform));
	TestEqual(TEXT("Mutating a returned snapshot does not change stored identity"), AfterCopy.ObjectName,
	          Initial.ObjectName);
	TestEqual(TEXT("Mutating a returned snapshot does not advance revision"), AfterCopy.StateRevision,
	          Initial.StateRevision);
	TestTrue(TEXT("Caller-owned flags do not change editing state"), AfterCopy.bEditingEnabled);
	TestFalse(TEXT("Caller-owned flags do not change dirty state"), AfterCopy.bHasChanges);

	const int64 BeforeMalformed = Editor->GetLatestEventSequence();
	TestFalse(TEXT("Malformed request is rejected"),
	          Editor->PublishRequest(TEXT("TransformEditRequested"), TEXT("{bad json")));
	bool bSawInvalidInbound = false;
	bool bSawInvalidRejection = false;
	int64 InboundSequence = 0;
	int64 RejectionSequence = 0;
	for (const FOWTEventRecord& Record : Editor->GetMonitorEntries())
	{
		if (Record.Sequence <= BeforeMalformed)
		{
			continue;
		}
		if (Record.Direction == EOWTEventDirection::Inbound && Record.Event == TEXT("TransformEditRequested"))
		{
			bSawInvalidInbound = !Record.bValidJson && Record.Json == TEXT("{bad json");
			InboundSequence = Record.Sequence;
		}
		if (Record.Direction == EOWTEventDirection::Outbound && Record.Event == TEXT("RequestRejected"))
		{
			bSawInvalidRejection = Record.bValidJson && Record.Json.Contains(TEXT("InvalidJson"));
			RejectionSequence = Record.Sequence;
		}
	}
	TestTrue(TEXT("Malformed inbound payload is journaled with invalid JSON metadata"), bSawInvalidInbound);
	TestTrue(TEXT("Malformed request rejection is journaled"), bSawInvalidRejection);
	TestTrue(TEXT("Inbound sequence precedes its rejection"),
	         InboundSequence > 0 && RejectionSequence > InboundSequence);
	TestEqual(TEXT("Malformed JSON does not advance observed state"), Editor->GetSnapshot().StateRevision,
	          Initial.StateRevision);
	TestTrue(TEXT("Malformed JSON does not mutate the actor"), Target->GetActorTransform().Equals(Initial.Transform));

	TSharedPtr<FJsonObject> ForgedState;
	for (const FOWTEventRecord& Record : Editor->GetMonitorEntries())
	{
		if (Record.Event == TEXT("SelectionChanged") && Record.Direction == EOWTEventDirection::Outbound)
		{
			FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Record.Json), ForgedState);
		}
	}
	if (!TestTrue(TEXT("Observed selection has a diagnostic snapshot"), ForgedState.IsValid()))
	{
		return false;
	}
	const FString ForgedRequestId = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens);
	ForgedState->SetStringField(TEXT("requestId"), ForgedRequestId);
	ForgedState->SetStringField(TEXT("source"), TEXT("ObservedStateTest"));
	ForgedState->SetStringField(TEXT("objectName"), TEXT("Forged transport state"));
	ForgedState->SetNumberField(TEXT("stateRevision"), Initial.StateRevision + 5000);
	ForgedState->SetBoolField(TEXT("editingEnabled"), false);
	ForgedState->SetBoolField(TEXT("hasChanges"), true);
	FString ForgedJson;
	FJsonSerializer::Serialize(ForgedState.ToSharedRef(), TJsonWriterFactory<>::Create(&ForgedJson));
	FOWTAttributeSnapshot ParsedCopy;
	TestTrue(TEXT("A valid transport snapshot can be parsed as caller data"),
	         AVTBAttributeEditor::ParseSnapshotJson(ForgedJson, ParsedCopy));
	TestEqual(TEXT("The caller can inspect its parsed transport data"), ParsedCopy.ObjectName,
	          FString(TEXT("Forged transport state")));
	TestFalse(TEXT("State notifications cannot be injected through the command API"),
	          Editor->PublishRequest(TEXT("EditorStateChanged"), ForgedJson));
	const FOWTAttributeSnapshot AfterRejectedState = Editor->GetSnapshot();
	TestEqual(TEXT("Rejected snapshot JSON cannot replace observed identity"), AfterRejectedState.ObjectName,
	          Initial.ObjectName);
	TestEqual(TEXT("Rejected snapshot JSON cannot replace observed revision"), AfterRejectedState.StateRevision,
	          Initial.StateRevision);
	TestTrue(TEXT("Rejected snapshot JSON cannot disable editing"), AfterRejectedState.bEditingEnabled);
	TestFalse(TEXT("Rejected snapshot JSON cannot mark the actor dirty"), AfterRejectedState.bHasChanges);
	TestTrue(TEXT("Rejected snapshot JSON preserves actual actor transform"),
	         Target->GetActorTransform().Equals(Initial.Transform));
	bool bSawCorrelatedRejection = false;
	for (const FOWTEventRecord& Record : Editor->GetMonitorEntries())
	{
		if (Record.Event == TEXT("RequestRejected") && Record.Json.Contains(ForgedRequestId))
		{
			bSawCorrelatedRejection = Record.Json.Contains(TEXT("UnknownEvent"));
		}
	}
	TestTrue(TEXT("Rejected state injection remains diagnosable by request ID"), bSawCorrelatedRejection);

	const FVector ExternalLocation(120.0, 220.0, 320.0);
	const int64 BeforeExternal = Editor->GetLatestEventSequence();
	Target->SetActorLocation(ExternalLocation);
	TestTrue(TEXT("The observed store changes only after observation"),
	         Editor->GetSnapshot().Transform.Equals(Initial.Transform));
	Hub->Tick(0.0f);
	const FOWTAttributeSnapshot Observed = Editor->GetSnapshot();
	TestTrue(TEXT("Subsystem tick observes actual external actor changes"),
	         Observed.Transform.GetLocation().Equals(ExternalLocation));
	TestTrue(TEXT("Actual external change advances the observed revision"),
	         Observed.StateRevision > Initial.StateRevision);
	TestEqual(TEXT("External transform change preserves selection revision"), Observed.SelectionRevision,
	          Initial.SelectionRevision);
	TestTrue(TEXT("Observed external change is compared against baseline"), Observed.bHasChanges);
	bool bSawExternalSnapshot = false;
	int64 PreviousSequence = 0;
	for (const FOWTEventRecord& Record : Editor->GetMonitorEntries())
	{
		TestTrue(TEXT("Facade journal is ordered by monotonic sequence"), Record.Sequence > PreviousSequence);
		PreviousSequence = Record.Sequence;
		if (Record.Sequence <= BeforeExternal || Record.Event != TEXT("TransformChanged") ||
		    Record.Direction != EOWTEventDirection::Outbound)
		{
			continue;
		}
		FOWTAttributeSnapshot EventState;
		if (AVTBAttributeEditor::ParseSnapshotJson(Record.Json, EventState))
		{
			bSawExternalSnapshot = EventState.StateRevision == Observed.StateRevision &&
			                       EventState.Transform.Equals(Observed.Transform) &&
			                       Record.Json.Contains(TEXT("External"));
		}
	}
	TestTrue(TEXT("External change journal reflects the same observed snapshot"), bSawExternalSnapshot);
	TestEqual(TEXT("Latest facade sequence matches its newest journal record"), Editor->GetLatestEventSequence(),
	          PreviousSequence);

	Editor->MarkSelectionBaseline();
	TestFalse(TEXT("Accepting actual external transform as baseline clears dirty state"),
	          Editor->GetSnapshot().bHasChanges);
	Target->SetActorTransform(Initial.Transform);
	Hub->Tick(0.0f);
	TestTrue(TEXT("Old transform differs from the newly accepted baseline"), Editor->GetSnapshot().bHasChanges);
	Target->SetActorLocation(ExternalLocation);
	Hub->Tick(0.0f);
	TestFalse(TEXT("Returning to the accepted baseline clears dirty state"), Editor->GetSnapshot().bHasChanges);

	const int32 BeforeDestroyedRevision = Editor->GetSnapshot().SelectionRevision;
	const int64 BeforeDestroyedSequence = Editor->GetLatestEventSequence();
	Target->Destroy();
	Hub->Tick(0.0f);
	const FOWTAttributeSnapshot Destroyed = Editor->GetSnapshot();
	TestFalse(TEXT("Destroyed selection is absent from observed state"), Destroyed.bHasSelection);
	TestFalse(TEXT("Destroyed selection cannot be edited"), Destroyed.bCanEditTransform);
	TestTrue(TEXT("Destroyed selection clears stable object identity"), Destroyed.ObjectId.IsEmpty());
	TestTrue(TEXT("Destroyed selection advances its lifetime revision"),
	         Destroyed.SelectionRevision > BeforeDestroyedRevision);
	bool bSawDestroyedSelection = false;
	for (const FOWTEventRecord& Record : Editor->GetMonitorEntries())
	{
		if (Record.Sequence <= BeforeDestroyedSequence || Record.Event != TEXT("SelectionChanged"))
		{
			continue;
		}
		FOWTAttributeSnapshot EventState;
		if (AVTBAttributeEditor::ParseSnapshotJson(Record.Json, EventState))
		{
			bSawDestroyedSelection = !EventState.bHasSelection && EventState.ObjectId.IsEmpty();
		}
	}
	TestTrue(TEXT("Destroyed selection cleanup is recorded in the journal"), bSawDestroyedSelection);

	AStaticMeshActor* CallbackTarget = World->SpawnActor<AStaticMeshActor>();
	if (!TestNotNull(TEXT("Callback lifetime target"), CallbackTarget))
	{
		return false;
	}
	CallbackTarget->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	Hub->SetSelectedObject(CallbackTarget);
	Editor->DispatchBeginPlay();
	bool bMovementCallbackRan = false;
	const FDelegateHandle MovementCallback = CallbackTarget->GetRootComponent()->TransformUpdated.AddLambda(
	    [WeakEditor = TWeakObjectPtr<AVTBAttributeEditor>(Editor),
	     &bMovementCallbackRan](USceneComponent*, EUpdateTransformFlags, ETeleportType)
	    {
		    if (AVTBAttributeEditor* ActiveEditor = WeakEditor.Get())
		    {
			    bMovementCallbackRan = true;
			    ActiveEditor->Destroy();
		    }
	    });
	const bool bAppliedAfterShutdown = Editor->RequestTransformField(
	    Editor->GetSnapshot(), EOWTTransformField::LocationX, 75.0, EOWTTransformEditPhase::Commit, FGuid::NewGuid());
	CallbackTarget->GetRootComponent()->TransformUpdated.Remove(MovementCallback);
	TestTrue(TEXT("Movement can synchronously destroy the editor owner"), bMovementCallbackRan);
	TestFalse(TEXT("Ended session does not continue through its subsystem after movement"), bAppliedAfterShutdown);
	TestNull(TEXT("Editor teardown unregisters the service"), Hub->GetAttributeEditor());
	return !HasAnyErrors();
}

#endif
