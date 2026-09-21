#if WITH_DEV_AUTOMATION_TESTS

#include "VTBEditorGameMode.h"
#include "Context/VTBOWTEditorToolsContext.h"
#include "Context/IVTBOWTEditorSceneState.h"
#include "BaseGizmos/TransformProxy.h"
#include "Gizmo/VTBOWTEditorRepositionalGizmo.h"
#include "VTBOWTEditorGizmoManager.h"
#include "VTBOWTEditorModeSubsystem.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTRuntimeHostSelectionTest,
	"VTBOWTEditor.ToolsContext.HostSelectionIntegration",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTRuntimeHostSelectionTest::RunTest(const FString& Parameters)
{
	const UWorld::InitializationValues Values = UWorld::InitializationValues()
		.InitializeScenes(false).AllowAudioPlayback(false).CreatePhysicsScene(false)
		.CreateNavigation(false).CreateAISystem(false).CreateFXSystem(false);
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true,
		ERHIFeatureLevel::Num, &Values);
	if (!TestNotNull(TEXT("Game world exists"), World))
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	ON_SCOPE_EXIT
	{
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
	};
	UVTBOWTEditorModeSubsystem* Runtime = World->GetSubsystem<UVTBOWTEditorModeSubsystem>();
	if (!TestNotNull(TEXT("World creates the module-owned subsystem"), Runtime))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = Runtime->GetToolsContext();
	if (!TestTrue(TEXT("Subsystem owns a ready tools context"), Context && Context->IsRuntimeReady()))
	{
		return false;
	}
	AVTBEditorGameMode* Host = World->SpawnActor<AVTBEditorGameMode>();
	if (!TestNotNull(TEXT("Sample selection host exists"), Host)
		|| !TestTrue(TEXT("Host binds through the selection interface"), Runtime->BindSelectionSource(Host)))
	{
		return false;
	}
	AActor* Actor = World->SpawnActor<AActor>();
	if (!TestNotNull(TEXT("Selectable actor exists"), Actor))
	{
		return false;
	}
	USceneComponent* Root = NewObject<USceneComponent>(Actor);
	Root->SetMobility(EComponentMobility::Movable);
	Actor->SetRootComponent(Root);
	Actor->AddInstanceComponent(Root);
	Root->RegisterComponent();

	FSelectedObjectsChangeList Request{};
	Request.ModificationType = ESelectedObjectsModificationType::Replace;
	Request.Components.Add(Root);
	TestTrue(TEXT("ITF selection request reaches the host"),
		Context->GetContextTransactionAPI()->RequestSelectionChange(Request));
	FToolBuilderState Selection;
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Host event updates the context's actor selection"),
		Selection.SelectedActors.Num() == 1 && Selection.SelectedActors[0] == Actor);
	UVTBOWTEditorRepositionalGizmo* Gizmo = Runtime->GetGizmoManager()->GetSelectionGizmo();
	if (!TestTrue(TEXT("Host selection creates a targeted gizmo"), Gizmo && Gizmo->ActiveTarget))
	{
		return false;
	}
	TestTrue(TEXT("Binding the same selection source again succeeds"), Runtime->BindSelectionSource(Host));
	TestFalse(TEXT("An object without the selection interface is rejected"), Runtime->BindSelectionSource(Actor));
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Rejected binding preserves the current source and selection"),
		Selection.SelectedActors == TArray<AActor*>({Actor}));

	AActor* Second = World->SpawnActor<AActor>();
	AActor* Destroyed = World->SpawnActor<AActor>();
	if (!TestNotNull(TEXT("Second selectable actor exists"), Second)
		|| !TestNotNull(TEXT("Actor to be destroyed exists"), Destroyed))
	{
		return false;
	}
	Destroyed->Destroy();
	int32 SelectionEventCount = 0;
	const FDelegateHandle SelectionHandle = Host->OnSelectionChanged().AddLambda([&SelectionEventCount]
	{
		++SelectionEventCount;
	});
	ON_SCOPE_EXIT
	{
		Host->OnSelectionChanged().Remove(SelectionHandle);
	};
	Request.ModificationType = ESelectedObjectsModificationType::Replace;
	Request.Actors = {Second, Actor, Second, nullptr, Destroyed};
	Request.Components = {Root, Root};
	TestTrue(TEXT("Replace accepts mixed actor and component requests"),
		Context->GetContextTransactionAPI()->RequestSelectionChange(Request));
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Replace preserves first occurrence order and excludes invalid actors"),
		Selection.SelectedActors == TArray<AActor*>({Second, Actor}));
	TestEqual(TEXT("A normalized replacement emits one host selection event"), SelectionEventCount, 1);
	TestTrue(TEXT("Repeating the same normalized selection succeeds"),
		Context->GetContextTransactionAPI()->RequestSelectionChange(Request));
	TestEqual(TEXT("Duplicate selection does not emit another event"), SelectionEventCount, 1);

	Request.ModificationType = ESelectedObjectsModificationType::Add;
	Request.Actors = {Actor, Second, nullptr, Destroyed};
	TestTrue(TEXT("Add accepts duplicate and invalid actor requests"),
		Context->GetContextTransactionAPI()->RequestSelectionChange(Request));
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Adding existing actors preserves order without duplicates"),
		Selection.SelectedActors == TArray<AActor*>({Second, Actor}));
	TestEqual(TEXT("A no-op add does not emit another event"), SelectionEventCount, 1);

	Request.ModificationType = ESelectedObjectsModificationType::Remove;
	Request.Actors = {Second, Second, nullptr, Destroyed};
	Request.Components.Reset();
	TestTrue(TEXT("Remove accepts duplicate and invalid actor requests"),
		Context->GetContextTransactionAPI()->RequestSelectionChange(Request));
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Remove retains the unrequested actor"), Selection.SelectedActors == TArray<AActor*>({Actor}));
	TestEqual(TEXT("Removal emits one additional event"), SelectionEventCount, 2);
	Request.ModificationType = ESelectedObjectsModificationType::Add;
	Request.Actors = {Second, nullptr, Second, Destroyed};
	TestTrue(TEXT("Add appends new actors once"), Context->GetContextTransactionAPI()->RequestSelectionChange(Request));
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("New actors are appended after the existing selection"),
		Selection.SelectedActors == TArray<AActor*>({Actor, Second}));
	TestEqual(TEXT("Adding a new actor emits one additional event"), SelectionEventCount, 3);
	Request.ModificationType = ESelectedObjectsModificationType::Remove;
	Request.Actors.Reset();
	Request.Components = {Root, Root};
	TestTrue(TEXT("Component removal applies to its owning actor"),
		Context->GetContextTransactionAPI()->RequestSelectionChange(Request));
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Component removal preserves the other selected actor"), Selection.SelectedActors == TArray<AActor*>({Second}));
	TestEqual(TEXT("Component removal emits one additional event"), SelectionEventCount, 4);
	Request.ModificationType = ESelectedObjectsModificationType::Clear;
	Request.Actors = {Actor};
	Request.Components = {Root};
	TestTrue(TEXT("Clear ignores supplied actor and component payloads"),
		Context->GetContextTransactionAPI()->RequestSelectionChange(Request));
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Clear removes all selected actors"), Selection.SelectedActors.IsEmpty());
	TestEqual(TEXT("Clear emits one additional event"), SelectionEventCount, 5);
	TestTrue(TEXT("Clearing an empty selection still succeeds"),
		Context->GetContextTransactionAPI()->RequestSelectionChange(Request));
	TestEqual(TEXT("Repeated clear does not emit another event"), SelectionEventCount, 5);
	Host->SetSelectedActors({Actor});

	Context->RunContextUpdate([&]
	{
		Host->ClearSelection();
		Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
		TestEqual(TEXT("Selection waits until the active ITF callback ends"), Selection.SelectedActors.Num(), 1);
	});
	Runtime->Tick(0.0f);
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Deferred empty selection clears queries and gizmo target"),
		Selection.SelectedActors.IsEmpty() && Gizmo->ActiveTarget == nullptr);
	Host->SetSelectedActors({Actor});
	TestNotNull(TEXT("Host can select again"), Gizmo->ActiveTarget.Get());
	TestTrue(TEXT("Selection source can detach"), Runtime->BindSelectionSource(nullptr));
	Host->ClearSelection();
	Host->SetSelectedActors({Actor});
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Detached host no longer controls the context"), Selection.SelectedActors.IsEmpty());
	TestTrue(TEXT("Detaching twice is harmless"), Runtime->BindSelectionSource(nullptr));
	TestFalse(TEXT("A detached context rejects selection requests"),
		Context->GetContextTransactionAPI()->RequestSelectionChange(Request));
	TestTrue(TEXT("The host can rebind after detaching"), Runtime->BindSelectionSource(Host));
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Rebinding imports the host's current selection"), Selection.SelectedActors == TArray<AActor*>({Actor}));

	AVTBEditorGameMode* Replacement = World->SpawnActor<AVTBEditorGameMode>();
	if (!TestNotNull(TEXT("Replacement selection source exists"), Replacement))
	{
		return false;
	}
	Replacement->SetSelectedActors({Second});
	TestTrue(TEXT("A different host can replace the selection source"), Runtime->BindSelectionSource(Replacement));
	Host->ClearSelection();
	Host->SetSelectedActors({Actor});
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Events from the previous source cannot overwrite the replacement"),
		Selection.SelectedActors == TArray<AActor*>({Second}));
	Replacement->Destroy();
	Runtime->Tick(0.0f);
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Destroying the source clears selection on the next tick"), Selection.SelectedActors.IsEmpty());
	TestTrue(TEXT("A live host can bind after the previous source is destroyed"), Runtime->BindSelectionSource(Host));
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Binding after source destruction restores selection"), Selection.SelectedActors == TArray<AActor*>({Actor}));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTRuntimeHostComponentFrameTest,
	"VTBOWTEditor.ToolsContext.HostComponentFrameIntegration",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTRuntimeHostComponentFrameTest::RunTest(const FString& Parameters)
{
	const UWorld::InitializationValues Values = UWorld::InitializationValues()
		.InitializeScenes(false).AllowAudioPlayback(false).CreatePhysicsScene(false)
		.CreateNavigation(false).CreateAISystem(false).CreateFXSystem(false);
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true,
		ERHIFeatureLevel::Num, &Values);
	if (!TestNotNull(TEXT("Game world exists"), World))
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	ON_SCOPE_EXIT
	{
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
	};
	UVTBOWTEditorModeSubsystem* Runtime = World->GetSubsystem<UVTBOWTEditorModeSubsystem>();
	AVTBEditorGameMode* Host = World->SpawnActor<AVTBEditorGameMode>();
	if (!TestNotNull(TEXT("The runtime subsystem exists"), Runtime)
		|| !TestNotNull(TEXT("The host exists"), Host)
		|| !TestTrue(TEXT("The host binds through its selection interface"), Runtime->BindSelectionSource(Host)))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = Runtime->GetToolsContext();
	AActor* Actor = World->SpawnActor<AActor>();
	if (!TestNotNull(TEXT("The actor exists"), Actor))
	{
		return false;
	}
	USplineComponent* Spline = NewObject<USplineComponent>(Actor);
	Spline->SetMobility(EComponentMobility::Movable);
	Actor->SetRootComponent(Spline);
	Actor->AddInstanceComponent(Spline);
	Spline->RegisterComponent();
	UStaticMeshComponent* First = NewObject<UStaticMeshComponent>(Actor);
	First->SetMobility(EComponentMobility::Movable);
	First->SetupAttachment(Spline);
	First->SetRelativeTransform(FTransform(FRotator(15, 40, -10), FVector(75, 20, 15)));
	Actor->AddInstanceComponent(First);
	First->RegisterComponent();
	UStaticMeshComponent* Second = NewObject<UStaticMeshComponent>(Actor);
	Second->SetMobility(EComponentMobility::Movable);
	Second->SetupAttachment(Spline);
	Second->SetRelativeTransform(FTransform(FRotator(-10, -35, 25), FVector(-45, 30, 20)));
	Actor->AddInstanceComponent(Second);
	Second->RegisterComponent();
	int32 SelectionEvents = 0;
	const FDelegateHandle SelectionHandle = Host->OnSelectionChanged().AddLambda([&SelectionEvents]
	{
		++SelectionEvents;
	});
	ON_SCOPE_EXIT
	{
		Host->OnSelectionChanged().Remove(SelectionHandle);
	};
	Host->SetSelectedComponent(First);
	UVTBOWTEditorRepositionalGizmo* Gizmo = Runtime->GetGizmoManager()->GetSelectionGizmo();
	if (!TestTrue(TEXT("Component selection creates a runtime target"), Gizmo && Gizmo->GetTransformProxy()))
	{
		return false;
	}
	FToolBuilderState Selection;
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Queries retain both the owning actor and selected component"),
		Selection.SelectedActors == TArray<AActor*>({Actor}) && Selection.SelectedComponents == TArray<UActorComponent*>({First}));
	TestTrue(TEXT("The host frame reaches the gizmo unchanged"), Host->GetSelectionFrame() == First && Gizmo->GetSelectionFrame() == First);
	TestTrue(TEXT("A new component frame selects local axes"),
		Context->GetContextQueriesAPI()->GetCurrentCoordinateSystem() == EToolContextCoordinateSystem::Local);
	TestTrue(TEXT("The initial gizmo uses the selected component's world frame"),
		Gizmo->GetTransformProxy()->GetTransform().Equals(First->GetComponentTransform()));
	TestEqual(TEXT("Selecting the component emits one event"), SelectionEvents, 1);
	Context->GetSceneState().SetCoordinateSystem(EToolContextCoordinateSystem::World);
	Host->SetSelectedComponent(First);
	Runtime->Tick(0.0f);
	TestEqual(TEXT("Selecting the same component is a no-op"), SelectionEvents, 1);
	TestTrue(TEXT("The user's world-axis choice survives repeated selection and ticking"),
		Context->GetContextQueriesAPI()->GetCurrentCoordinateSystem() == EToolContextCoordinateSystem::World);
	Host->SetSelectedComponent(Second);
	TestEqual(TEXT("Changing components within the same actor emits an event"), SelectionEvents, 2);
	TestTrue(TEXT("Changing components activates the new local frame"),
		Gizmo->GetSelectionFrame() == Second
		&& Gizmo->GetTransformProxy()->GetTransform().Equals(Second->GetComponentTransform())
		&& Context->GetContextQueriesAPI()->GetCurrentCoordinateSystem() == EToolContextCoordinateSystem::Local);
	FSelectedObjectsChangeList Request{};
	Request.ModificationType = ESelectedObjectsModificationType::Replace;
	Request.Components = {First};
	TestTrue(TEXT("ITF component selection is accepted"), Context->GetContextTransactionAPI()->RequestSelectionChange(Request));
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("ITF requests preserve the frame component through the host and subsystem"),
		Host->GetSelectionFrame() == First && Gizmo->GetSelectionFrame() == First
		&& Selection.SelectedComponents == TArray<UActorComponent*>({First}));
	const FTransform Relative = First->GetRelativeTransform();
	const FVector RootLocation = Actor->GetActorLocation();
	FTransform Changed = Gizmo->GetTransformProxy()->GetTransform();
	Changed.AddToTranslation(FVector(20, -15, 10));
	Gizmo->SetNewGizmoTransform(Changed);
	TestTrue(TEXT("Host component selection transforms the entire actor"),
		Actor->GetActorLocation().Equals(RootLocation + FVector(20, -15, 10))
		&& First->GetRelativeTransform().Equals(Relative));
	Host->SetSelectedActors({Actor});
	TestNull(TEXT("Explicit actor selection clears the host component frame"), Host->GetSelectionFrame());
	TestNull(TEXT("Explicit actor selection clears the gizmo component frame"), Gizmo->GetSelectionFrame());
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Actor selection restores the actor root in queries"), Selection.SelectedComponents == TArray<UActorComponent*>({Spline}));
	Host->SetSelectedComponent(Second);
	Second->DestroyComponent();
	Runtime->Tick(0.0f);
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Destroying the frame keeps the owner selected with its root frame"),
		Selection.SelectedActors == TArray<AActor*>({Actor}) && Selection.SelectedComponents == TArray<UActorComponent*>({Spline})
		&& Gizmo->GetSelectionFrame() == nullptr && Gizmo->GetTransformProxy()->GetTransform().Equals(Actor->GetActorTransform()));
	return true;
}

#endif
