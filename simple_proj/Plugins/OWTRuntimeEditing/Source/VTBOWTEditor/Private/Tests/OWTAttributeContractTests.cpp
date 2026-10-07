#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorSubsystem.h"
#include "Components/SceneComponent.h"
#include "Dom/JsonObject.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Serialization/JsonSerializer.h"

namespace
{
TSharedRef<FJsonObject> MakeContractRequest(const FOWTAttributeSnapshot& Snapshot)
{
	TSharedRef<FJsonObject> Request = MakeShared<FJsonObject>();
	Request->SetNumberField(TEXT("schemaVersion"), 1);
	Request->SetStringField(TEXT("editorId"), Snapshot.EditorId);
	Request->SetStringField(TEXT("objectId"), Snapshot.ObjectId);
	Request->SetNumberField(TEXT("selectionRevision"), Snapshot.SelectionRevision);
	Request->SetStringField(TEXT("requestId"), FGuid::NewGuid().ToString());
	Request->SetStringField(TEXT("source"), TEXT("ContractTest"));
	Request->SetStringField(TEXT("operationId"), FGuid::NewGuid().ToString());
	Request->SetStringField(TEXT("phase"), TEXT("Commit"));
	Request->SetStringField(TEXT("space"), TEXT("World"));
	Request->SetStringField(TEXT("property"), TEXT("Location.X"));
	Request->SetNumberField(TEXT("value"), 10.0);
	return Request;
}

FString ContractJson(const TSharedRef<FJsonObject>& Request)
{
	FString Json;
	FJsonSerializer::Serialize(Request, TJsonWriterFactory<>::Create(&Json));
	return Json;
}
} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTAttributeContractTest, "OWT.Runtime.AttributeContracts",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTAttributeContractTest::RunTest(const FString& Parameters)
{
	const UWorld::InitializationValues InitValues = UWorld::InitializationValues()
	                                                    .AllowAudioPlayback(false)
	                                                    .CreatePhysicsScene(true)
	                                                    .CreateNavigation(false)
	                                                    .CreateAISystem(false)
	                                                    .ShouldSimulatePhysics(false);
	UWorld* World =
	    UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &InitValues);
	if (!TestNotNull(TEXT("World exists"), World))
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	UVTBOWTEditorSubsystem* Hub = World->GetSubsystem<UVTBOWTEditorSubsystem>();
	AVTBAttributeEditor* Editor = World->SpawnActor<AVTBAttributeEditor>();
	if (!TestNotNull(TEXT("Editor exists"), Editor))
	{
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
		return false;
	}
	if (!TestNotNull(TEXT("Subsystem exists"), Hub))
	{
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
		return false;
	}
	TestFalse(TEXT("Unbound editor rejects requests"),
	          Editor->PublishRequest(TEXT("TransformEditRequested"), TEXT("{}")));
	Editor->BindSubsystem(Hub);
	Hub->ToggleEditing();
	AActor* Target = World->SpawnActor<AActor>();
	USceneComponent* Root = NewObject<USceneComponent>(Target);
	Target->SetRootComponent(Root);
	Target->AddInstanceComponent(Root);
	Root->SetMobility(EComponentMobility::Movable);
	Root->RegisterComponent();
	Hub->SetSelectedObject(Target);
	const FOWTAttributeSnapshot Initial = Editor->GetSnapshot();

	TSharedRef<FJsonObject> Request = MakeContractRequest(Initial);
	const FString ReplayJson = ContractJson(Request);
	TestTrue(TEXT("Valid JSON request succeeds"), Editor->PublishRequest(TEXT("TransformEditRequested"), ReplayJson));
	TestFalse(TEXT("Same requestId cannot execute again"),
	          Editor->PublishRequest(TEXT("TransformEditRequested"), ReplayJson));
	TestEqual(TEXT("Replay leaves value unchanged"), Target->GetActorLocation().X, 10.0);

	Request = MakeContractRequest(Editor->GetSnapshot());
	Request->SetStringField(TEXT("value"), TEXT("400"));
	TestFalse(TEXT("Numeric strings are rejected"),
	          Editor->PublishRequest(TEXT("TransformEditRequested"), ContractJson(Request)));
	Request = MakeContractRequest(Editor->GetSnapshot());
	Request->SetNumberField(TEXT("selectionRevision"), Initial.SelectionRevision + 0.5);
	TestFalse(TEXT("Fractional revision is rejected"),
	          Editor->PublishRequest(TEXT("TransformEditRequested"), ContractJson(Request)));
	Request = MakeContractRequest(Editor->GetSnapshot());
	Request->SetStringField(TEXT("editorId"), FGuid::NewGuid().ToString());
	TestFalse(TEXT("Other editor session rejected"),
	          Editor->PublishRequest(TEXT("TransformEditRequested"), ContractJson(Request)));
	Request = MakeContractRequest(Editor->GetSnapshot());
	Request->SetNumberField(TEXT("schemaVersion"), 3);
	TestFalse(TEXT("Unsupported schema rejected"),
	          Editor->PublishRequest(TEXT("TransformEditRequested"), ContractJson(Request)));
	TestEqual(TEXT("Rejected requests preserve Actor"), Target->GetActorLocation().X, 10.0);

	TestTrue(TEXT("Restoring initial value succeeds"),
	         Editor->RequestTransformField(Editor->GetSnapshot(), EOWTTransformField::LocationX, 0.0,
	                                       EOWTTransformEditPhase::Commit, FGuid::NewGuid()));
	TestFalse(TEXT("Restoring baseline clears hasChanges"), Editor->GetSnapshot().bHasChanges);
	const FOWTAttributeSnapshot BeforeDrag = Editor->GetSnapshot();
	const FGuid Operation = FGuid::NewGuid();
	TestTrue(TEXT("Begin before external edit"),
	         Editor->RequestTransformField(BeforeDrag, EOWTTransformField::LocationX, 0.0,
	                                       EOWTTransformEditPhase::Begin, Operation));
	TestTrue(TEXT("Update before external edit"),
	         Editor->RequestTransformField(BeforeDrag, EOWTTransformField::LocationX, 20.0,
	                                       EOWTTransformEditPhase::Update, Operation));
	Target->SetActorLocation(FVector(90.0, 30.0, 40.0));
	Editor->RefreshSelectedTransform();
	TestFalse(TEXT("External edit ends active operation"), Editor->GetSnapshot().bIsModifying);
	TestFalse(TEXT("Late commit from ended operation rejected"),
	          Editor->RequestTransformField(BeforeDrag, EOWTTransformField::LocationX, 20.0,
	                                        EOWTTransformEditPhase::Commit, Operation));
	TestEqual(TEXT("External value survives late commit"), Target->GetActorLocation().X, 90.0);

	AActor* Observer = World->SpawnActor<AActor>();
	int32 SelfEvents = 0;
	int32 OtherEvents = 0;
	int32 AddedEvents = 0;
	FGuid SelfHandle;
	FGuid AddedHandle;
	SelfHandle = Editor->Subscribe(Observer,
	                               FOWTAttributeEventNative::CreateLambda(
	                                   [&](FName Event, const FString& Json)
	                                   {
		                                   ++SelfEvents;
		                                   Editor->Unsubscribe(SelfHandle);
		                                   AddedHandle = Editor->Subscribe(Editor,
		                                                                   FOWTAttributeEventNative::CreateLambda(
		                                                                       [&AddedEvents](FName, const FString&)
		                                                                       {
			                                                                       ++AddedEvents;
		                                                                       }),
		                                                                   false);
		                                   Editor->NotifyEditorStateChanged();
	                                   }),
	                               false);
	const FGuid OtherHandle = Editor->Subscribe(Editor,
	                                            FOWTAttributeEventNative::CreateLambda(
	                                                [&OtherEvents](FName, const FString&)
	                                                {
		                                                ++OtherEvents;
	                                                }),
	                                            false);
	Editor->NotifyEditorStateChanged();
	TestEqual(TEXT("Observer can unsubscribe itself during callback"), SelfEvents, 1);
	TestEqual(TEXT("Nested event is queued and delivered"), OtherEvents, 2);
	TestEqual(TEXT("New observer starts with queued event only"), AddedEvents, 1);
	Editor->Unsubscribe(OtherHandle);
	Editor->Unsubscribe(AddedHandle);

	int32 DestroyedObserverEvents = 0;
	Editor->Subscribe(Observer,
	                  FOWTAttributeEventNative::CreateLambda(
	                      [&DestroyedObserverEvents](FName, const FString&)
	                      {
		                      ++DestroyedObserverEvents;
	                      }),
	                  false);
	Observer->Destroy();
	Editor->NotifyEditorStateChanged();
	TestEqual(TEXT("Destroyed observer is not invoked"), DestroyedObserverEvents, 0);

	int32 LoopEvents = 0;
	const FGuid LoopHandle = Editor->Subscribe(Editor,
	                                           FOWTAttributeEventNative::CreateLambda(
	                                               [&](FName, const FString&)
	                                               {
		                                               ++LoopEvents;
		                                               Editor->NotifyEditorStateChanged();
	                                               }),
	                                           false);
	AddExpectedError(TEXT("OWT event dispatch limit exceeded"), EAutomationExpectedErrorFlags::Contains, 1);
	Editor->NotifyEditorStateChanged();
	TestEqual(TEXT("Unbounded republishing stops at dispatch budget"), LoopEvents, 256);
	Editor->Unsubscribe(LoopHandle);

	Hub->ShutdownToolsContext();
	World->DestroyWorld(false);
	GEngine->DestroyWorldContext(World);
	return !HasAnyErrors();
}

#endif
