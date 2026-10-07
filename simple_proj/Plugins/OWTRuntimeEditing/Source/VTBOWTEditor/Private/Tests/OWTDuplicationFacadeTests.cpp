#if WITH_DEV_AUTOMATION_TESTS

#include "Components/StaticMeshComponent.h"
#include "Dom/JsonObject.h"
#include "Engine/Engine.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "Modes/OWTAttributeEditMode.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Tests/OWTModeTestTypes.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorSubsystem.h"

#include <limits>

namespace
{
struct FFacadeEvent
{
	FName Topic;
	TSharedPtr<FJsonObject> Payload;
};

TSharedRef<FJsonObject> MakeDuplicateRequest(const AVTBAttributeEditor& Editor, int32 Version)
{
	const FOWTAttributeSnapshot State = Editor.GetSnapshot();
	TSharedRef<FJsonObject> Request = MakeShared<FJsonObject>();
	Request->SetNumberField(TEXT("schemaVersion"), Version);
	Request->SetStringField(TEXT("editorId"), State.EditorId);
	Request->SetStringField(TEXT("objectId"), State.ObjectId);
	Request->SetNumberField(TEXT("selectionRevision"), State.SelectionRevision);
	Request->SetStringField(TEXT("requestId"), FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens));
	Request->SetStringField(TEXT("source"), TEXT("FacadeContract"));
	return Request;
}

FString SerializeRequest(const TSharedRef<FJsonObject>& Request)
{
	FString Json;
	FJsonSerializer::Serialize(Request, TJsonWriterFactory<>::Create(&Json));
	return Json;
}

bool FindOperation(const AVTBAttributeEditor& Editor, FGuid Id, FOWTDuplicationOperationSnapshot& OutOperation)
{
	for (const FOWTDuplicationOperationSnapshot& Operation : Editor.GetDuplicationOperations())
	{
		if (Operation.OperationId == Id)
		{
			OutOperation = Operation;
			return true;
		}
	}
	return false;
}

int32 CountStaticMeshActors(UWorld& World)
{
	int32 Count = 0;
	for (TActorIterator<AStaticMeshActor> It(&World); It; ++It)
	{
		if (!It->IsActorBeingDestroyed())
		{
			++Count;
		}
	}
	return Count;
}

int32 CountResultEvents(const TArray<FFacadeEvent>& Events, FGuid OperationId)
{
	int32 Count = 0;
	for (const FFacadeEvent& Event : Events)
	{
		if (Event.Topic != TEXT("ObjectDuplicated"))
		{
			continue;
		}
		if (!Event.Payload.IsValid())
		{
			continue;
		}
		FString Id;
		Event.Payload->TryGetStringField(TEXT("operationId"), Id);
		if (Id == OperationId.ToString(EGuidFormats::DigitsWithHyphens))
		{
			++Count;
		}
	}
	return Count;
}

FString FindLastRejection(const TArray<FFacadeEvent>& Events)
{
	for (int32 Index = Events.Num() - 1; Index >= 0; --Index)
	{
		const FFacadeEvent& Event = Events[Index];
		if (Event.Topic != TEXT("RequestRejected"))
		{
			continue;
		}
		if (!Event.Payload.IsValid())
		{
			continue;
		}
		FString Code;
		Event.Payload->TryGetStringField(TEXT("code"), Code);
		return Code;
	}
	return {};
}

bool IsUncommittedTerminal(EOWTDuplicationPhase Phase)
{
	if (Phase == EOWTDuplicationPhase::Failed)
	{
		return true;
	}
	return Phase == EOWTDuplicationPhase::Cancelled;
}

enum class EModeCallbackAction : uint8
{
	Exit,
	Shutdown,
	Replace
};

void CheckModeCallbackReentry(FAutomationTestBase& Test, UVTBOWTEditorSubsystem& Hub, AVTBAttributeEditor& Editor,
                              FName TriggerTopic, EModeCallbackAction Action)
{
	UOWTAttributeEditMode* CurrentMode = NewObject<UOWTAttributeEditMode>(&Hub);
	if (!Test.TestTrue(TEXT("Install callback contract mode"), Hub.SetActiveEditMode(CurrentMode)))
	{
		return;
	}
	if (!Test.TestTrue(TEXT("Enter callback contract mode"), CurrentMode->Enter()))
	{
		return;
	}
	FOWTToolDescriptor Descriptor;
	Descriptor.ToolId = TEXT("Contract.FacadeCallback");
	Descriptor.Label = FText::FromString(TEXT("Facade callback fixture"));
	Descriptor.BuilderClass = UOWTModeTestBuilder::StaticClass();
	FGuid Token;
	FString Error;
	if (!Test.TestTrue(TEXT("Register callback contract tool"),
	                   CurrentMode->RegisterTool(Descriptor, TEXT("Contract.Facade"), Token, Error)))
	{
		return;
	}
	if (TriggerTopic == TEXT("ToolEnded"))
	{
		if (!Test.TestTrue(TEXT("Start tool before testing its end callback"),
		                   CurrentMode->StartTool(Descriptor.ToolId, Error)))
		{
			return;
		}
	}
	UOWTAttributeEditMode* Replacement = NewObject<UOWTAttributeEditMode>(&Hub);
	bool bTriggered = false;
	TSharedPtr<FJsonObject> LastStateEvent;
	const FGuid Subscription =
	    Editor.Subscribe(&Editor,
	                     FOWTAttributeEventNative::CreateLambda(
	                         [&](FName Topic, const FString& Json)
	                         {
		                         TSharedPtr<FJsonObject> Payload;
		                         if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Json), Payload))
		                         {
			                         Test.AddError(TEXT("A mode callback event was not valid JSON."));
			                         return;
		                         }
		                         if (Topic == TEXT("EditorStateChanged"))
		                         {
			                         LastStateEvent = Payload;
		                         }
		                         if (!bTriggered)
		                         {
			                         if (Topic != TriggerTopic)
			                         {
				                         return;
			                         }
			                         FString ToolId;
			                         Payload->TryGetStringField(TEXT("toolId"), ToolId);
			                         if (ToolId != Descriptor.ToolId.ToString())
			                         {
				                         return;
			                         }
			                         bTriggered = true;
			                         switch (Action)
			                         {
			                         case EModeCallbackAction::Exit:
				                         CurrentMode->Exit();
				                         break;
			                         case EModeCallbackAction::Shutdown:
				                         CurrentMode->Shutdown();
				                         break;
			                         case EModeCallbackAction::Replace:
				                         Test.TestTrue(TEXT("Replace mode from a tool event callback"),
				                                       Hub.SetActiveEditMode(Replacement));
				                         break;
			                         }
		                         }
	                         }),
	                     false);
	ON_SCOPE_EXIT
	{
		Editor.Unsubscribe(Subscription);
	};
	if (TriggerTopic == TEXT("ToolStarted"))
	{
		CurrentMode->StartTool(Descriptor.ToolId, Error);
	}
	else
	{
		CurrentMode->EndTool(false, Error);
	}
	Test.TestTrue(
	    FString::Printf(TEXT("%s callback action %d ran"), *TriggerTopic.ToString(), static_cast<int32>(Action)),
	    bTriggered);
	if (Test.TestTrue(TEXT("Callback transition publishes an observable state"), LastStateEvent.IsValid()))
	{
		// Nested events drain in publication order. Check the final delivered state after that queue drains.
		bool bEventEditing = false;
		Test.TestTrue(TEXT("State event contains editing availability"),
		              LastStateEvent->TryGetBoolField(TEXT("editingEnabled"), bEventEditing));
		Test.TestEqual(TEXT("No stale state event overwrites callback editing availability"), bEventEditing,
		               Editor.GetSnapshot().bEditingEnabled);
		double EventRevision = 0;
		if (LastStateEvent->TryGetNumberField(TEXT("modeRevision"), EventRevision))
		{
			Test.TestEqual(TEXT("Final state event revision belongs to the current mode"),
			               static_cast<int32>(EventRevision), Editor.GetModeSnapshot().Revision);
			FString ToolId;
			LastStateEvent->TryGetStringField(TEXT("toolId"), ToolId);
			Test.TestEqual(TEXT("Final state event tool belongs to the current mode"), ToolId,
			               Editor.GetModeSnapshot().ActiveToolId.ToString());
		}
	}
	if (Action == EModeCallbackAction::Replace)
	{
		Test.TestTrue(TEXT("Callback replacement remains the active mode"), Hub.GetAttributeEditMode() == Replacement);
		Test.TestTrue(TEXT("Replacement mode keeps editing enabled"), Editor.GetModeSnapshot().bEditingEnabled);
	}
	else
	{
		Test.TestFalse(TEXT("Callback exit/shutdown keeps typed editing disabled"),
		               Editor.GetModeSnapshot().bEditingEnabled);
		Test.TestFalse(TEXT("Callback exit/shutdown keeps the observed state disabled"),
		               Editor.GetSnapshot().bEditingEnabled);
	}
	Hub.Tick(0.016f);
}
} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTDuplicationFacadeTest, "OWT.Runtime.DuplicationFacade",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTDuplicationFacadeTest::RunTest(const FString& Parameters)
{
	const UWorld::InitializationValues InitValues = UWorld::InitializationValues()
	                                                    .AllowAudioPlayback(false)
	                                                    .CreatePhysicsScene(true)
	                                                    .CreateNavigation(false)
	                                                    .CreateAISystem(false)
	                                                    .ShouldSimulatePhysics(false);
	UWorld* World =
	    UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &InitValues);
	if (!TestNotNull(TEXT("Facade contract world"), World))
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
	AStaticMeshActor* Source = World->SpawnActor<AStaticMeshActor>();
	if (!TestNotNull(TEXT("Attribute editor"), Editor))
	{
		return false;
	}
	if (!TestNotNull(TEXT("Duplication source"), Source))
	{
		return false;
	}
	Source->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	Source->SetActorLocation(FVector(10, 20, 30));
	Editor->BindSubsystem(Hub);
	Hub->ToggleEditing();
	Hub->SetSelectedObject(Source);
	UOWTAttributeEditMode* Mode = Hub->GetAttributeEditMode();
	if (!TestNotNull(TEXT("Runtime mode"), Mode))
	{
		return false;
	}

	TArray<FFacadeEvent> Events;
	const FGuid Subscription =
	    Editor->Subscribe(Editor,
	                      FOWTAttributeEventNative::CreateLambda(
	                          [&Events](FName Topic, const FString& Json)
	                          {
		                          FFacadeEvent Event;
		                          Event.Topic = Topic;
		                          FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Json), Event.Payload);
		                          Events.Add(MoveTemp(Event));
	                          }),
	                      false);
	ON_SCOPE_EXIT
	{
		Editor->Unsubscribe(Subscription);
	};

	FOWTDuplicationOptions Options;
	Options.WorldOffset = FVector(37, 18, -9);
	Options.HierarchyScope = EOWTDuplicationHierarchyScope::AuthoredHierarchy;
	const int32 BeforeCommit = CountStaticMeshActors(*World);
	const FGuid OperationId = Editor->BeginDuplicateOperation(Editor->GetSnapshot(), Options);
	TestTrue(TEXT("Schema 2 facade returns an accepted operation id"), OperationId.IsValid());
	FOWTDuplicationOperationSnapshot Accepted;
	if (TestTrue(TEXT("Accepted operation is observable before the next frame"),
	             FindOperation(*Editor, OperationId, Accepted)))
	{
		TestEqual(TEXT("Initial operation phase is Accepted"), Accepted.Phase, EOWTDuplicationPhase::Accepted);
		TestTrue(TEXT("Acceptance retains the requested source"), Accepted.SourceActor.Get() == Source);
		TestFalse(TEXT("Acceptance does not claim a ready Actor"), Accepted.DuplicateActor.IsValid());
	}
	TestEqual(TEXT("Acceptance does not synchronously spawn the duplicate"), CountStaticMeshActors(*World),
	          BeforeCommit);
	TestEqual(TEXT("No result event is sent at acceptance"), CountResultEvents(Events, OperationId), 0);
	Hub->Tick(0.016f);
	FOWTDuplicationOperationSnapshot Committed;
	if (TestTrue(TEXT("Committed operation remains observable"), FindOperation(*Editor, OperationId, Committed)))
	{
		TestEqual(TEXT("Next tool tick commits authored duplication"), Committed.Phase,
		          EOWTDuplicationPhase::Committed);
		AActor* Duplicate = Committed.DuplicateActor.Get();
		if (TestNotNull(TEXT("Commit exposes the resulting Actor"), Duplicate))
		{
			TestTrue(TEXT("Schema 2 world offset reaches the clone"),
			         Duplicate->GetActorLocation().Equals(Source->GetActorLocation() + Options.WorldOffset));
			TestTrue(TEXT("Committed duplicate becomes selected"), Hub->SelectedObject.Get() == Duplicate);
		}
	}
	TestEqual(TEXT("Commit creates one Actor"), CountStaticMeshActors(*World), BeforeCommit + 1);
	TestEqual(TEXT("Commit sends ObjectDuplicated exactly once"), CountResultEvents(Events, OperationId), 1);
	Hub->Tick(0.016f);
	TestEqual(TEXT("Following frame restores the default tool"), Editor->GetModeSnapshot().ActiveToolId,
	          Mode->DefaultToolId);
	Hub->Tick(0.016f);
	TestEqual(TEXT("Later ticks do not resend the duplication result"), CountResultEvents(Events, OperationId), 1);
	Hub->SetSelectedObject(Source);

	const int32 BeforeRejectedRequests = Editor->GetDuplicationOperations().Num();
	const TCHAR* PolicyFields[] = {TEXT("worldOffset"), TEXT("hierarchyScope"), TEXT("generationPolicy")};
	for (const TCHAR* Field : PolicyFields)
	{
		TSharedRef<FJsonObject> Request = MakeDuplicateRequest(*Editor, 1);
		if (FCString::Strcmp(Field, TEXT("worldOffset")) == 0)
		{
			TSharedRef<FJsonObject> Offset = MakeShared<FJsonObject>();
			Offset->SetNumberField(TEXT("x"), 1);
			Offset->SetNumberField(TEXT("y"), 2);
			Offset->SetNumberField(TEXT("z"), 3);
			Request->SetObjectField(Field, Offset);
		}
		else
		{
			Request->SetStringField(Field, TEXT("AuthoredHierarchy"));
		}
		TestFalse(FString::Printf(TEXT("Schema 1 rejects schema 2 field %s"), Field),
		          Editor->PublishRequest(TEXT("DuplicateRequested"), SerializeRequest(Request)));
		TestEqual(TEXT("Schema-specific fields return an explicit rejection"), FindLastRejection(Events),
		          FString(TEXT("SchemaRequired")));
	}

	FOWTDuplicationOptions Invalid = Options;
	Invalid.WorldOffset.X = std::numeric_limits<double>::quiet_NaN();
	TestFalse(TEXT("Typed NaN offset cannot enter the operation queue"),
	          Editor->BeginDuplicateOperation(Editor->GetSnapshot(), Invalid).IsValid());
	TestEqual(TEXT("NaN offset reports InvalidOffset"), FindLastRejection(Events), FString(TEXT("InvalidOffset")));
	const int64 InvalidScopeValues[] = {StaticEnum<EOWTDuplicationHierarchyScope>()->GetMaxEnumValue(), 255};
	for (const int64 Value : InvalidScopeValues)
	{
		Invalid = Options;
		Invalid.HierarchyScope = static_cast<EOWTDuplicationHierarchyScope>(Value);
		TestFalse(TEXT("Typed scope sentinel/out-of-range cannot enter the operation queue"),
		          Editor->BeginDuplicateOperation(Editor->GetSnapshot(), Invalid).IsValid());
		TestEqual(TEXT("Invalid scope reports InvalidScope"), FindLastRejection(Events), FString(TEXT("InvalidScope")));
	}
	const int64 InvalidPolicyValues[] = {StaticEnum<EOWTDuplicationGenerationPolicy>()->GetMaxEnumValue(), 255};
	for (const int64 Value : InvalidPolicyValues)
	{
		Invalid = Options;
		Invalid.GenerationPolicy = static_cast<EOWTDuplicationGenerationPolicy>(Value);
		TestFalse(TEXT("Typed generation sentinel/out-of-range cannot enter the operation queue"),
		          Editor->BeginDuplicateOperation(Editor->GetSnapshot(), Invalid).IsValid());
		TestEqual(TEXT("Invalid generation reports InvalidGenerationPolicy"), FindLastRejection(Events),
		          FString(TEXT("InvalidGenerationPolicy")));
	}
	TestEqual(TEXT("Rejected requests do not add operation records"), Editor->GetDuplicationOperations().Num(),
	          BeforeRejectedRequests);

	const int32 BeforeCancel = CountStaticMeshActors(*World);
	const FGuid CancelledId = Editor->BeginDuplicateOperation(Editor->GetSnapshot(), Options);
	TestTrue(TEXT("Pending cancellation starts with an accepted operation"), CancelledId.IsValid());
	TestTrue(TEXT("Pending duplicate advertises cancellation"), Editor->CanCancelActiveTool());
	TestTrue(TEXT("Facade can cancel a pending duplicate"), Editor->RequestEndTool(false));
	Hub->Tick(0.016f);
	FOWTDuplicationOperationSnapshot Cancelled;
	if (TestTrue(TEXT("Cancelled operation remains available"), FindOperation(*Editor, CancelledId, Cancelled)))
	{
		TestEqual(TEXT("Pending cancel is terminal"), Cancelled.Phase, EOWTDuplicationPhase::Cancelled);
		TestFalse(TEXT("Cancelled operation has no duplicate"), Cancelled.DuplicateActor.IsValid());
	}
	TestEqual(TEXT("Cancel creates no Actor"), CountStaticMeshActors(*World), BeforeCancel);
	TestEqual(TEXT("Cancel sends no successful duplication result"), CountResultEvents(Events, CancelledId), 0);
	Hub->Tick(0.016f);

	AStaticMeshActor* Other = World->SpawnActor<AStaticMeshActor>();
	if (!TestNotNull(TEXT("Alternative selection"), Other))
	{
		return false;
	}
	Other->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	const int32 BeforeSelectionChange = CountStaticMeshActors(*World);
	const FGuid SelectionChangedId = Editor->BeginDuplicateOperation(Editor->GetSnapshot(), Options);
	Hub->SetSelectedObject(Other);
	Hub->Tick(0.016f);
	FOWTDuplicationOperationSnapshot SelectionChanged;
	if (TestTrue(TEXT("Selection invalidation remains observable"),
	             FindOperation(*Editor, SelectionChangedId, SelectionChanged)))
	{
		TestTrue(TEXT("Selection changed before execution prevents commit"),
		         IsUncommittedTerminal(SelectionChanged.Phase));
		TestFalse(TEXT("Invalidated selection has no duplicate"), SelectionChanged.DuplicateActor.IsValid());
		TestFalse(TEXT("Selection invalidation explains the outcome"), SelectionChanged.Error.IsEmpty());
	}
	TestEqual(TEXT("Selection invalidation creates no Actor"), CountStaticMeshActors(*World), BeforeSelectionChange);
	TestEqual(TEXT("Selection invalidation emits no success"), CountResultEvents(Events, SelectionChangedId), 0);
	Hub->Tick(0.016f);
	Hub->SetSelectedObject(Other);

	const FGuid DestroyedId = Editor->BeginDuplicateOperation(Editor->GetSnapshot(), Options);
	TestTrue(TEXT("Destroy-before-tick starts with an accepted operation"), DestroyedId.IsValid());
	Other->Destroy();
	const int32 AfterSourceDestroy = CountStaticMeshActors(*World);
	Hub->Tick(0.016f);
	FOWTDuplicationOperationSnapshot Destroyed;
	if (TestTrue(TEXT("Destroyed source outcome is retained"), FindOperation(*Editor, DestroyedId, Destroyed)))
	{
		TestTrue(TEXT("Destroyed source cannot commit"), IsUncommittedTerminal(Destroyed.Phase));
		TestFalse(TEXT("Destroyed source has no duplicate"), Destroyed.DuplicateActor.IsValid());
		TestFalse(TEXT("Destroyed source explains the failure"), Destroyed.Error.IsEmpty());
	}
	TestEqual(TEXT("Destroyed source does not produce a clone"), CountStaticMeshActors(*World), AfterSourceDestroy);
	TestEqual(TEXT("Destroyed source emits no success"), CountResultEvents(Events, DestroyedId), 0);
	Hub->Tick(0.016f);
	Hub->SetSelectedObject(Source);

	const int32 BeforeOff = CountStaticMeshActors(*World);
	const FGuid OffId = Editor->BeginDuplicateOperation(Editor->GetSnapshot(), Options);
	TestTrue(TEXT("Off-mode cancellation starts with acceptance"), OffId.IsValid());
	Hub->ToggleEditing();
	TestFalse(TEXT("Editing is off"), Editor->GetModeSnapshot().bEditingEnabled);
	FOWTDuplicationOperationSnapshot Off;
	if (TestTrue(TEXT("Off-mode retains cancelled operation state"), FindOperation(*Editor, OffId, Off)))
	{
		TestEqual(TEXT("Editing off cancels the pending operation"), Off.Phase, EOWTDuplicationPhase::Cancelled);
	}
	const int32 OffOperationCount = Editor->GetDuplicationOperations().Num();
	const int32 OffRevision = Off.Revision;
	Hub->Tick(0.016f);
	Hub->Tick(0.016f);
	TestEqual(TEXT("Off-mode ticks preserve operation history"), Editor->GetDuplicationOperations().Num(),
	          OffOperationCount);
	FOWTDuplicationOperationSnapshot StableOff;
	if (TestTrue(TEXT("Cancelled operation is still addressable by id"), FindOperation(*Editor, OffId, StableOff)))
	{
		TestEqual(TEXT("Off-mode ticks do not repeatedly cancel the same operation"), StableOff.Revision, OffRevision);
	}
	TestEqual(TEXT("Off-mode cancellation does not create a clone"), CountStaticMeshActors(*World), BeforeOff);
	TestEqual(TEXT("Off-mode cancellation emits no success"), CountResultEvents(Events, OffId), 0);
	TestEqual(TEXT("Earlier successful result remains exactly once"), CountResultEvents(Events, OperationId), 1);

	const FName ToolTopics[] = {TEXT("ToolStarted"), TEXT("ToolEnded")};
	const EModeCallbackAction CallbackActions[] = {EModeCallbackAction::Exit, EModeCallbackAction::Shutdown,
	                                               EModeCallbackAction::Replace};
	for (FName Topic : ToolTopics)
	{
		for (EModeCallbackAction Action : CallbackActions)
		{
			CheckModeCallbackReentry(*this, *Hub, *Editor, Topic, Action);
		}
	}

	AStaticMeshActor* CallbackSelection = World->SpawnActor<AStaticMeshActor>();
	if (!TestNotNull(TEXT("Committed callback alternative selection"), CallbackSelection))
	{
		return false;
	}
	CallbackSelection->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	for (const bool bReplaceMode : {false, true})
	{
		UOWTAttributeEditMode* OperationMode = NewObject<UOWTAttributeEditMode>(Hub);
		if (!TestTrue(TEXT("Install committed callback mode"), Hub->SetActiveEditMode(OperationMode)))
		{
			return false;
		}
		if (!TestTrue(TEXT("Enter committed callback mode"), OperationMode->Enter()))
		{
			return false;
		}
		Hub->SetSelectedObject(Source);
		UOWTAttributeEditMode* Replacement = NewObject<UOWTAttributeEditMode>(Hub);
		FGuid CallbackOperationId;
		bool bCallbackRan = false;
		const FGuid CallbackSubscription =
		    Editor->Subscribe(Editor,
		                      FOWTAttributeEventNative::CreateLambda(
		                          [&](FName Topic, const FString& Json)
		                          {
			                          if (bCallbackRan)
			                          {
				                          return;
			                          }
			                          if (Topic != TEXT("DuplicateOperationChanged"))
			                          {
				                          return;
			                          }
			                          TSharedPtr<FJsonObject> Payload;
			                          if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Json), Payload))
			                          {
				                          return;
			                          }
			                          FString Phase;
			                          FString Id;
			                          Payload->TryGetStringField(TEXT("phase"), Phase);
			                          Payload->TryGetStringField(TEXT("operationId"), Id);
			                          if (Phase != TEXT("Committed"))
			                          {
				                          return;
			                          }
			                          if (Id != CallbackOperationId.ToString(EGuidFormats::DigitsWithHyphens))
			                          {
				                          return;
			                          }
			                          bCallbackRan = true;
			                          if (bReplaceMode)
			                          {
				                          TestTrue(TEXT("Committed subscriber can replace the editing mode"),
				                                   Hub->SetActiveEditMode(Replacement));
			                          }
			                          Hub->SetSelectedObject(CallbackSelection);
		                          }),
		                      false);
		ON_SCOPE_EXIT
		{
			Editor->Unsubscribe(CallbackSubscription);
		};
		CallbackOperationId = Editor->BeginDuplicateOperation(Editor->GetSnapshot(), Options);
		TestTrue(TEXT("Committed callback request is accepted"), CallbackOperationId.IsValid());
		Hub->Tick(0.016f);
		TestTrue(TEXT("Committed subscriber ran"), bCallbackRan);
		TestTrue(TEXT("Old duplicate auto-selection preserves subscriber selection"),
		         Hub->GetAttributeEditMode()->GetSelectedObject() == CallbackSelection);
		TestEqual(TEXT("Observed state follows the subscriber selection"), Editor->GetSnapshot().ObjectName,
		          CallbackSelection->GetName());
		if (bReplaceMode)
		{
			TestTrue(TEXT("Old operation does not replace the subscriber mode"),
			         Hub->GetAttributeEditMode() == Replacement);
		}
		TestEqual(TEXT("Committed callback still emits one successful result"),
		          CountResultEvents(Events, CallbackOperationId), 1);
		Hub->Tick(0.016f);
		TestTrue(TEXT("Following tick preserves the subscriber selection"),
		         Hub->GetAttributeEditMode()->GetSelectedObject() == CallbackSelection);
		TestEqual(TEXT("Following tick does not resend the callback result"),
		          CountResultEvents(Events, CallbackOperationId), 1);
	}

	return !HasAnyErrors();
}

#endif
