#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorSubsystem.h"
#include "BaseGizmos/TransformProxy.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTAttributeRuntimeTest, "OWT.Runtime.Attributes",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTAttributeRuntimeTest::RunTest(const FString& Parameters)
{
	const UWorld::InitializationValues InitValues = UWorld::InitializationValues()
	                                                    .AllowAudioPlayback(false)
	                                                    .CreatePhysicsScene(true)
	                                                    .CreateNavigation(false)
	                                                    .CreateAISystem(false)
	                                                    .ShouldSimulatePhysics(false);
	UWorld* World =
	    UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &InitValues);
	if (!TestNotNull(TEXT("Test world"), World))
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	UVTBOWTEditorSubsystem* Hub = World->GetSubsystem<UVTBOWTEditorSubsystem>();
	AVTBAttributeEditor* Editor = World->SpawnActor<AVTBAttributeEditor>();
	if (!TestNotNull(TEXT("AttributeEditor"), Editor))
	{
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
		return false;
	}
	if (!TestNotNull(TEXT("Editing subsystem"), Hub))
	{
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
		return false;
	}
	Editor->BindSubsystem(Hub);
	TestFalse(TEXT("Editing starts disabled"), Editor->GetSnapshot().bEditingEnabled);

	int32 StateEvents = 0;
	FOWTAttributeSnapshot Received;
	const FGuid Subscription =
	    Editor->Subscribe(Editor, FOWTAttributeEventNative::CreateLambda(
	                                  [&StateEvents, &Received](FName Event, const FString& Json)
	                                  {
		                                  FOWTAttributeSnapshot Parsed;
		                                  if (AVTBAttributeEditor::ParseSnapshotJson(Json, Parsed))
		                                  {
			                                  ++StateEvents;
			                                  Received = Parsed;
		                                  }
	                                  }));
	TestTrue(TEXT("Subscription returns a handle"), Subscription.IsValid());
	TestEqual(TEXT("Late subscriber immediately receives snapshot"), StateEvents, 1);
	Hub->ToggleEditing();
	TestTrue(TEXT("Editing mode notification reaches subscriber"), Received.bEditingEnabled);

	AStaticMeshActor* Target = World->SpawnActor<AStaticMeshActor>();
	Target->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	Target->SetActorTransform(
	    FTransform(FRotator(10.0, 20.0, 30.0), FVector(15.0, 25.0, 35.0), FVector(2.0, 3.0, 4.0)));
	Hub->SetSelectedObject(Target);
	const FOWTAttributeSnapshot Initial = Editor->GetSnapshot();
	TestTrue(TEXT("Selected movable actor is editable"), Initial.bCanEditTransform);
	TestFalse(TEXT("Initial selection matches its baseline"), Initial.bHasChanges);
	TestTrue(TEXT("Selection includes its actual world transform"),
	         Received.Transform.Equals(Target->GetActorTransform()));

	TestTrue(TEXT("Single-field numeric edit accepted"),
	         Editor->RequestTransformField(Initial, EOWTTransformField::LocationX, 120.0,
	                                       EOWTTransformEditPhase::Commit, FGuid::NewGuid()));
	TestEqual(TEXT("Actor X changed"), Target->GetActorLocation().X, 120.0);
	TestEqual(TEXT("Unedited Y preserved"), Target->GetActorLocation().Y, 25.0);
	TestTrue(TEXT("Applied change marks selection modified"), Editor->GetSnapshot().bHasChanges);
	TestEqual(TEXT("Subscriber sees authoritative value"), Received.Transform.GetLocation().X, 120.0);
	UTransformProxy* Proxy = Hub->GetTransformProxy();
	if (TestNotNull(TEXT("Selection has transform proxy"), Proxy))
	{
		TestEqual(TEXT("Sidebar edit resynchronizes proxy"), Proxy->GetTransform().GetLocation().X, 120.0);
	}

	const FOWTAttributeSnapshot BeforeDrag = Editor->GetSnapshot();
	const FGuid Operation = FGuid::NewGuid();
	TestTrue(TEXT("Slider begins"), Editor->RequestTransformField(BeforeDrag, EOWTTransformField::LocationX, 120.0,
	                                                              EOWTTransformEditPhase::Begin, Operation));
	TestTrue(TEXT("Slider updates"), Editor->RequestTransformField(BeforeDrag, EOWTTransformField::LocationX, 180.0,
	                                                               EOWTTransformEditPhase::Update, Operation));
	TestTrue(TEXT("Update remains valid after state revision changes"),
	         Editor->RequestTransformField(BeforeDrag, EOWTTransformField::LocationX, 190.0,
	                                       EOWTTransformEditPhase::Update, Operation));
	TestTrue(TEXT("Slider reports modifying"), Editor->GetSnapshot().bIsModifying);
	TestFalse(TEXT("Duplicate cannot interrupt active slider"), Editor->RequestDuplicate(Editor->GetSnapshot()));
	TestTrue(TEXT("Slider cancellation accepted"),
	         Editor->RequestTransformField(BeforeDrag, EOWTTransformField::LocationX, 190.0,
	                                       EOWTTransformEditPhase::Cancel, Operation));
	TestEqual(TEXT("Cancel restores interaction start"), Target->GetActorLocation().X, 120.0);
	TestFalse(TEXT("Cancel ends modifying state"), Editor->GetSnapshot().bIsModifying);

	Target->SetActorLocation(FVector(250.0, 70.0, 90.0));
	Editor->RefreshSelectedTransform();
	TestEqual(TEXT("External Actor edit reaches JSON subscriber"), Received.Transform.GetLocation().Y, 70.0);
	if (Proxy)
	{
		TestEqual(TEXT("External Actor edit reaches proxy"), Proxy->GetTransform().GetLocation().Y, 70.0);
		Proxy->BeginTransformEditSequence();
		FTransform GizmoTransform = Proxy->GetTransform();
		GizmoTransform.SetLocation(FVector(300.0, 80.0, 95.0));
		Proxy->SetTransform(GizmoTransform);
		TestTrue(TEXT("Gizmo begins modifying state"), Editor->GetSnapshot().bIsModifying);
		TestEqual(TEXT("Gizmo change reaches JSON subscriber"), Received.Transform.GetLocation().X, 300.0);
		Proxy->EndTransformEditSequence();
		TestFalse(TEXT("Gizmo ends modifying state"), Editor->GetSnapshot().bIsModifying);
	}
	Editor->MarkSelectionBaseline();
	TestFalse(TEXT("Explicit baseline acceptance clears modified flag"), Editor->GetSnapshot().bHasChanges);

	const FTransform BeforeInvalid = Target->GetActorTransform();
	TestFalse(TEXT("Malformed JSON rejected"),
	          Editor->PublishRequest(TEXT("TransformEditRequested"), TEXT("{bad json")));
	TestTrue(TEXT("Malformed JSON does not mutate Actor"), Target->GetActorTransform().Equals(BeforeInvalid));
	const FOWTAttributeSnapshot OldSelection = Editor->GetSnapshot();
	AStaticMeshActor* Other = World->SpawnActor<AStaticMeshActor>();
	Other->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	Hub->SetSelectedObject(Other);
	TestFalse(TEXT("Old selection request rejected"),
	          Editor->RequestTransformField(OldSelection, EOWTTransformField::LocationX, 999.0,
	                                        EOWTTransformEditPhase::Commit, FGuid::NewGuid()));
	TestEqual(TEXT("Old request leaves new selection unchanged"), Other->GetActorLocation().X, 0.0);
	Hub->HideSelectionGizmo();
	TestTrue(TEXT("Hiding Gizmo retains selected details"), Editor->GetSnapshot().bHasSelection);
	Hub->SetSelectedObject(Other);
	Other->Destroy();
	Hub->Tick(0.0f);
	TestFalse(TEXT("Destroyed target clears details"), Editor->GetSnapshot().bHasSelection);

	Hub->SetSelectedObject(Target);
	const FTransform OriginalTransform = Target->GetActorTransform();
	TestTrue(TEXT("Native Actor duplication accepted"), Editor->RequestDuplicate(Editor->GetSnapshot()));
	TestEqual(TEXT("Acceptance leaves the source selected until the tool ticks"), Hub->SelectedObject.Get(),
	          static_cast<AActor*>(Target));
	Hub->Tick(0.016f);
	AActor* Duplicate = Hub->SelectedObject.Get();
	if (TestNotNull(TEXT("Duplicate selected"), Duplicate))
	{
		TestTrue(TEXT("Duplicate is a different actor"), Duplicate != Target);
		TestEqual(TEXT("Duplicate preserves native class"), Duplicate->GetClass(), Target->GetClass());
		TestTrue(TEXT("Duplicate preserves nonuniform scale"),
		         Duplicate->GetActorScale3D().Equals(Target->GetActorScale3D()));
		TestTrue(TEXT("Duplicate offset applied once"),
		         Duplicate->GetActorLocation().Equals(Target->GetActorLocation() + FVector(100, 0, 0)));
		TestEqual(TEXT("Duplicate world X after offset"), Duplicate->GetActorLocation().X,
		          Target->GetActorLocation().X + 100.0);
		TestEqual(TEXT("Duplicate world Y after offset"), Duplicate->GetActorLocation().Y,
		          Target->GetActorLocation().Y);
		TestEqual(TEXT("Duplicate world Z after offset"), Duplicate->GetActorLocation().Z,
		          Target->GetActorLocation().Z);
		TestTrue(TEXT("Original transform preserved"), Target->GetActorTransform().Equals(OriginalTransform));
		TestTrue(TEXT("New duplicate reports changes"), Editor->GetSnapshot().bHasChanges);
	}

	Hub->Tick(0.016f);
	UClass* CubeClass =
	    LoadClass<AActor>(nullptr, TEXT("/Game/VTBOWT/Blueprints/BP_OWTEditableCube.BP_OWTEditableCube_C"));
	if (TestNotNull(TEXT("Cooked BP sample class"), CubeClass))
	{
		AActor* BlueprintActor = World->SpawnActor<AActor>(CubeClass);
		if (TestNotNull(TEXT("BP instance"), BlueprintActor))
		{
			BlueprintActor->SetActorTransform(
			    FTransform(FRotator(0, 35, 0), FVector(45, 55, 65), FVector(1.5, 2.0, 2.5)));
			BlueprintActor->Tags.Add(TEXT("RuntimeInstanceValue"));
			Hub->SetSelectedObject(BlueprintActor);
			TestTrue(TEXT("BP instance duplication accepted"), Editor->RequestDuplicate(Editor->GetSnapshot()));
			Hub->Tick(0.016f);
			AActor* BPCopy = Hub->SelectedObject.Get();
			if (TestNotNull(TEXT("BP copy selected"), BPCopy))
			{
				TestTrue(TEXT("BP copy is independent"), BPCopy != BlueprintActor);
				TestEqual(TEXT("BP generated class preserved"), BPCopy->GetClass(), CubeClass);
				TestTrue(TEXT("BP scale preserved"),
				         BPCopy->GetActorScale3D().Equals(BlueprintActor->GetActorScale3D()));
				TestTrue(TEXT("BP runtime tag preserved"), BPCopy->Tags.Contains(TEXT("RuntimeInstanceValue")));
				TestTrue(TEXT("BP root component independent"),
				         BPCopy->GetRootComponent() != BlueprintActor->GetRootComponent());
			}
		}
	}

	Hub->ToggleEditing();
	TestFalse(TEXT("F2 off clears details"), Received.bHasSelection);
	TestFalse(TEXT("F2 off disables editing"), Received.bEditingEnabled);
	Editor->Unsubscribe(Subscription);
	const int32 EventsBeforeUnsubscribe = StateEvents;
	Editor->NotifyEditorStateChanged();
	TestEqual(TEXT("Unsubscribe prevents callback"), StateEvents, EventsBeforeUnsubscribe);
	Hub->ShutdownToolsContext();
	World->DestroyWorld(false);
	GEngine->DestroyWorldContext(World);
	return !HasAnyErrors();
}

#endif
