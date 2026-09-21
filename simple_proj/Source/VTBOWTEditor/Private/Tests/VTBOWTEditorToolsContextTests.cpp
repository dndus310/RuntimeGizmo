#if WITH_DEV_AUTOMATION_TESTS

#include "Context/VTBOWTEditorToolsContext.h"
#include "Context/IVTBOWTEditorInput.h"
#include "Context/IVTBOWTEditorSceneState.h"
#include "Context/IVTBOWTEditorUndoRedo.h"
#include "Context/IVTBOWTEditorViewport.h"

#include "BaseGizmos/AxisPositionGizmo.h"
#include "BaseGizmos/GizmoViewContext.h"
#include "BaseGizmos/HitTargets.h"
#include "BaseGizmos/TransformProxy.h"
#include "BaseGizmos/ViewAdjustedStaticMeshGizmoComponent.h"
#include "BaseBehaviors/ClickDragBehavior.h"
#include "Components/SceneComponent.h"
#include "Components/DynamicMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ContextObjectStore.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/Actor.h"
#include "InputState.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolChange.h"
#include "InteractiveToolManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "Gizmo/VTBOWTEditorRepositionalGizmo.h"
#include "Gizmo/VTBOWTEditorGizmoVisualComponent.h"
#include "VTBOWTEditorGizmoManager.h"
#include "VTBOWTEditorModeSubsystem.h"
#include "Selection/VTBOWTEditorSelectionSource.h"
#include "Engine/Engine.h"
#include "UObject/UObjectGlobals.h"

namespace VTBOWTEditorToolsContextTests
{

class FScopedTestWorld
{
public:
	FScopedTestWorld()
	{
		const UWorld::InitializationValues Values = UWorld::InitializationValues()
			.InitializeScenes(false).AllowAudioPlayback(false).CreatePhysicsScene(false)
			.CreateNavigation(false).CreateAISystem(false).CreateFXSystem(false);
		World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true,
			ERHIFeatureLevel::Num, &Values);
		if (World && GEngine)
		{
			GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
		}
	}

	~FScopedTestWorld()
	{
		if (World)
		{
			World->DestroyWorld(false);
			if (GEngine)
			{
				GEngine->DestroyWorldContext(World);
			}
		}
	}

	UWorld* World = nullptr;
};

class FTrackedChange final : public FToolCommandChange
{
public:
	FTrackedChange(int32& InApplyCount, int32& InDestroyCount)
		: ApplyCount(InApplyCount), DestroyCount(InDestroyCount)
	{
	}
	virtual ~FTrackedChange() override
	{
		++DestroyCount;
	}
	virtual void Apply(UObject*) override
	{
		++ApplyCount;
	}
	virtual void Revert(UObject*) override
	{
	}

private:
	int32& ApplyCount;
	int32& DestroyCount;
};

class FValueChange final : public FToolCommandChange
{
public:
	FValueChange(int32& InValue, int32 InBefore, int32 InAfter, const bool& InExpired)
		: Value(InValue), Before(InBefore), After(InAfter), bExpired(InExpired)
	{
	}
	virtual void Apply(UObject*) override
	{
		Value = After;
	}
	virtual void Revert(UObject*) override
	{
		Value = Before;
	}
	virtual bool HasExpired(UObject*) const override
	{
		return bExpired;
	}

private:
	int32& Value;
	int32 Before;
	int32 After;
	const bool& bExpired;
};

class FRecordingGizmoActorFactory final : public FCombinedTransformGizmoActorFactory
{
public:
	FRecordingGizmoActorFactory(UGizmoViewContext* ViewContext, UStaticMesh* InHandleMesh)
		: FCombinedTransformGizmoActorFactory(ViewContext), HandleMesh(InHandleMesh)
	{
	}

	virtual ACombinedTransformGizmoActor* CreateNewGizmoActor(UWorld* World) const override
	{
		++CreateCount;
		ACombinedTransformGizmoActor* Actor = FCombinedTransformGizmoActorFactory::CreateNewGizmoActor(World);
		LastActor = Actor;
		UStaticMeshComponent* Handle = Actor ? Cast<UStaticMeshComponent>(Actor->TranslateX) : nullptr;
		LastHandle = Handle;
		if (Handle)
		{
			Handle->SetStaticMesh(HandleMesh.Get());
		}
		return Actor;
	}

	mutable int32 CreateCount = 0;
	mutable TWeakObjectPtr<ACombinedTransformGizmoActor> LastActor;
	mutable TWeakObjectPtr<UStaticMeshComponent> LastHandle;

private:
	TWeakObjectPtr<UStaticMesh> HandleMesh;
};

class FTestQueries final : public IToolsContextQueriesAPI
{
public:
	explicit FTestQueries(UWorld* InWorld) : World(InWorld)
	{
	}
	virtual UWorld* GetCurrentEditingWorld() const override
	{
		return World;
	}
	virtual void GetCurrentSelectionState(FToolBuilderState& StateOut) const override
	{
		StateOut = FToolBuilderState();
		StateOut.World = World;
	}
	virtual void GetCurrentViewState(FViewCameraState& StateOut) const override
	{
		StateOut = FViewCameraState();
		StateOut.Position = FVector(123, 456, 789);
	}
	virtual UMaterialInterface* GetStandardMaterial(EStandardToolContextMaterials) const override
	{
		return nullptr;
	}
	virtual FViewport* GetHoveredViewport() const override
	{
		return nullptr;
	}
	virtual FViewport* GetFocusedViewport() const override
	{
		return nullptr;
	}

private:
	UWorld* World;
};

class FRecordingTransactions final : public IToolsContextTransactionsAPI
{
public:
	virtual void DisplayMessage(const FText& Message, EToolMessageLevel Level) override
	{
		LastMessage = Message;
		LastLevel = Level;
	}
	virtual void PostInvalidation() override
	{
		++InvalidationCount;
	}
	virtual void BeginUndoTransaction(const FText& Description) override
	{
		++BeginCount;
		LastDescription = Description;
	}
	virtual void EndUndoTransaction() override
	{
		++EndCount;
	}
	virtual void AppendChange(UObject* TargetObject, TUniquePtr<FToolCommandChange> Change,
		const FText& Description) override
	{
		LastTarget = TargetObject;
		LastChange = MoveTemp(Change);
		LastDescription = Description;
	}
	virtual bool RequestSelectionChange(const FSelectedObjectsChangeList& SelectionChange) override
	{
		LastSelection = SelectionChange;
		return bAcceptSelection;
	}

	FText LastMessage;
	FText LastDescription;
	EToolMessageLevel LastLevel = EToolMessageLevel::Internal;
	int32 InvalidationCount = 0;
	int32 BeginCount = 0;
	int32 EndCount = 0;
	UObject* LastTarget = nullptr;
	TUniquePtr<FToolCommandChange> LastChange;
	FSelectedObjectsChangeList LastSelection{};
	bool bAcceptSelection = false;
};

AActor* SpawnMovableActor(UWorld* World, const FVector& Location)
{
	AActor* Actor = World->SpawnActor<AActor>();
	if (!Actor)
	{
		return nullptr;
	}
	USceneComponent* Root = NewObject<USceneComponent>(Actor);
	Root->SetMobility(EComponentMobility::Movable);
	Actor->SetRootComponent(Root);
	Actor->AddInstanceComponent(Root);
	Root->RegisterComponent();
	Actor->SetActorLocation(Location);
	return Actor;
}

void ConfigureSelectionManager(UVTBOWTEditorToolsContext* Context)
{
	Context->SetCreateGizmoManagerFunc([](const UInteractiveToolsContext::FContextInitInfo& Info)
	{
		UVTBOWTEditorGizmoManager* Manager = NewObject<UVTBOWTEditorGizmoManager>(Info.ToolsContext);
		Manager->Initialize(Info.QueriesAPI, Info.TransactionsAPI, Info.InputRouter);
		Manager->RegisterDefaultGizmos();
		return Manager;
	});
}

bool SynchronizeSelection(UVTBOWTEditorToolsContext* Context,
	const TOptional<TArray<TWeakObjectPtr<AActor>>>& Selection, USceneComponent* FrameComponent = nullptr)
{
	UVTBOWTEditorGizmoManager* Manager = Cast<UVTBOWTEditorGizmoManager>(Context->GizmoManager);
	if (!Manager)
	{
		return false;
	}
	bool bApplied = false;
	TOptional<FVTBOWTTransformSelection> Request;
	if (Selection.IsSet())
	{
		FVTBOWTTransformSelection TransformSelection;
		TransformSelection.Actors = Selection.GetValue();
		TransformSelection.FrameComponent = FrameComponent;
		Request = MoveTemp(TransformSelection);
	}
	Context->RunContextUpdate([&]
	{
		bApplied = Manager->SynchronizeSelection(Request, [Context]
		{
			Context->GetInput().CancelActiveInteraction();
			return Context->IsRuntimeReady();
		});
		if (bApplied && Context->IsRuntimeReady())
		{
			TArray<AActor*> Actors;
			Manager->GetSelection(Actors);
			UVTBOWTEditorRepositionalGizmo* Gizmo = Manager->GetSelectionGizmo();
			if (Gizmo && Gizmo->GetSelectionFrame())
			{
				Context->GetSceneState().SetSelection(Actors, {Gizmo->GetSelectionFrame()});
			}
			else
			{
				Context->GetSceneState().SetActorSelection(Actors);
			}
		}
	});
	return bApplied;
}

template<typename ComponentType>
ComponentType* AddHierarchyComponent(AActor* Actor, USceneComponent* Parent, const FTransform& RelativeTransform)
{
	ComponentType* Component = NewObject<ComponentType>(Actor);
	Component->SetMobility(EComponentMobility::Movable);
	Component->SetupAttachment(Parent);
	Component->SetRelativeTransform(RelativeTransform);
	Actor->AddInstanceComponent(Component);
	Component->RegisterComponent();
	return Component;
}

TArray<FTransform> CaptureTransforms(const TArray<USceneComponent*>& Components)
{
	TArray<FTransform> Transforms;
	for (USceneComponent* Component : Components)
	{
		Transforms.Add(Component->GetComponentTransform());
	}
	return Transforms;
}

UVTBOWTEditorRepositionalGizmo* FindRuntimeGizmo(UVTBOWTEditorToolsContext* Context)
{
	return Context->GizmoManager
		? Cast<UVTBOWTEditorRepositionalGizmo>(Context->GizmoManager->FindGizmoByInstanceIdentifier(TEXT("VTBOWT.SelectionTransform")))
		: nullptr;
}

UAxisPositionGizmo* FindTranslateX(UVTBOWTEditorToolsContext* Context, UVTBOWTEditorRepositionalGizmo* Gizmo)
{
	FInputDeviceState LeftPress;
	LeftPress.InputDevice = EInputDevices::Mouse;
	LeftPress.Mouse.Left.SetStates(true, true, false);
	const TArray<UInteractiveGizmo*> ActiveGizmos = Context->GizmoManager->FindAllGizmosOfType(
		UInteractiveGizmoManager::DefaultAxisPositionBuilderIdentifier);
	for (UInteractiveGizmo* Object : ActiveGizmos)
	{
		UAxisPositionGizmo* Axis = Cast<UAxisPositionGizmo>(Object);
		UGizmoComponentHitTarget* Target = Axis ? Cast<UGizmoComponentHitTarget>(Axis->HitTarget.GetObject()) : nullptr;
		if (Target && Target->Component == Gizmo->GetGizmoActor()->TranslateX
			&& Axis->MouseBehavior && Axis->MouseBehavior->IsPressed(LeftPress))
		{
			return Axis;
		}
	}
	return nullptr;
}

FInputDeviceState MakePointer(float X, bool bPressed, bool bDown, bool bReleased)
{
	FInputDeviceState Input;
	Input.InputDevice = EInputDevices::Mouse;
	Input.Mouse.Position2D = FVector2D(X, 100);
	Input.Mouse.WorldRay = FRay(FVector(X, -100, 0), FVector::YAxisVector);
	Input.Mouse.Left.SetStates(bPressed, bDown, bReleased);
	return Input;
}

}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorToolsContextLifecycleTest,
	"VTBOWTEditor.ToolsContext.LifecycleAndQueries",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorToolsContextLifecycleTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Test world exists"), TestWorld.World))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	Context->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		Context->RemoveFromRoot();
	};

	bool bUpdateCalled = false;
	TestFalse(TEXT("A new context is not ready"), Context->IsRuntimeReady());
	TestFalse(TEXT("Updates before initialization are rejected"), Context->RunContextUpdate([&]
	{
		bUpdateCalled = true;
	}));
	TestFalse(TEXT("Rejected updates do not invoke callbacks"), bUpdateCalled);
	TestFalse(TEXT("Input before initialization is rejected"), Context->GetInput().PostPointerInput(MakePointer(0, true, true, false), false));
	Context->GetInput().CancelActiveInteraction();
	Context->Shutdown();
	Context->Initialize();
	if (!TestNotNull(TEXT("Default initialization creates the tool manager"), Context->ToolManager.Get())
		|| !TestNotNull(TEXT("Default initialization creates the gizmo manager"), Context->GizmoManager.Get()))
	{
		return false;
	}
	UInteractiveToolManager* OriginalManager = Context->ToolManager;
	IToolsContextQueriesAPI* Queries = Context->ToolManager->GetContextQueriesAPI();
	IToolsContextTransactionsAPI* Transactions = Context->ToolManager->GetContextTransactionsAPI();
	if (!TestNotNull(TEXT("Null query argument uses the owned queries"), Queries)
		|| !TestNotNull(TEXT("Null transaction argument uses the owned transactions"), Transactions))
	{
		return false;
	}
	Context->Initialize();
	TestTrue(TEXT("Repeated initialization preserves the manager"), Context->ToolManager == OriginalManager);
	TestTrue(TEXT("Repeated initialization preserves the active APIs"),
		Context->ToolManager->GetContextQueriesAPI() == Queries
		&& Context->ToolManager->GetContextTransactionsAPI() == Transactions);
	TestTrue(TEXT("Both managers receive the same default queries"),
		Context->GizmoManager->GetContextQueriesAPI() == Queries);
	TestTrue(TEXT("Queries obtain the world from the context outer"), Queries->GetCurrentEditingWorld() == TestWorld.World);
	TestNotNull(TEXT("The render implementation is owned by the context"), Context->GetContextRenderAPI());
	TestNotNull(TEXT("The query implementation is owned by the context"), Context->GetContextQueriesAPI());
	TestNotNull(TEXT("The transaction implementation is owned by the context"), Context->GetContextTransactionAPI());

	FToolBuilderState Selection;
	Queries->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Builder receives all context managers"), Selection.World == TestWorld.World
		&& Selection.ToolManager == Context->ToolManager
		&& Selection.TargetManager == Context->TargetManager
		&& Selection.GizmoManager == Context->GizmoManager);

	const FViewCameraState DefaultCamera;
	FViewCameraState QueriedCamera;
	QueriedCamera.Position = FVector(10, 20, 30);
	Queries->GetCurrentViewState(QueriedCamera);
	TestTrue(TEXT("Queries provide the default camera before the first render"),
		QueriedCamera.Position.Equals(DefaultCamera.Position)
		&& QueriedCamera.Orientation.Equals(DefaultCamera.Orientation)
		&& QueriedCamera.HorizontalFOVDegrees == DefaultCamera.HorizontalFOVDegrees
		&& QueriedCamera.DPIScale == DefaultCamera.DPIScale);
	TestNull(TEXT("No viewport is reported without a viewport client"), Queries->GetHoveredViewport());
	TestNull(TEXT("No focus viewport is reported without a viewport client"), Queries->GetFocusedViewport());
	TestFalse(TEXT("A null controller cannot produce a view"), Context->GetViewport().UpdateView(nullptr));
	TestTrue(TEXT("Missing a view does not shut down the context"), Context->IsRuntimeReady());
	Context->RunContextUpdate([&]
	{
		TestFalse(TEXT("Nested updates are deferred by rejecting their callback"), Context->RunContextUpdate([&]
		{
			bUpdateCalled = true;
		}));
	});
	TestFalse(TEXT("A nested update does not invoke its callback"), bUpdateCalled);

	Context->Shutdown();
	Context->Shutdown();
	TestNull(TEXT("Shutdown clears the manager"), Context->ToolManager.Get());
	TestNull(TEXT("Shutdown releases the render implementation"), Context->GetContextRenderAPI());
	TestNull(TEXT("Shutdown releases the query implementation"), Context->GetContextQueriesAPI());
	TestNull(TEXT("Shutdown releases the transaction implementation"), Context->GetContextTransactionAPI());
	TestFalse(TEXT("A shut-down context cannot accept pointer input"), Context->GetInput().PostPointerInput(MakePointer(0, true, true, false), false));
	TestFalse(TEXT("A shut-down context cannot undo"), Context->GetUndoRedo().Undo());
	TestFalse(TEXT("A shut-down context cannot redo"), Context->GetUndoRedo().Redo());
	Context->TickRuntime(0.0f);
	Context->GetInput().CancelActiveInteraction();
	Context->Initialize();
	if (!TestNotNull(TEXT("Context can initialize again after shutdown"), Context->ToolManager.Get()))
	{
		return false;
	}
	TestTrue(TEXT("Reinitialized queries still use the context world"),
		Context->ToolManager->GetContextQueriesAPI()->GetCurrentEditingWorld() == TestWorld.World);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorToolsContextTransactionsTest,
	"VTBOWTEditor.ToolsContext.TransactionBackend",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorToolsContextTransactionsTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	int32 ApplyCount = 0;
	int32 DestroyCount = 0;
	FRecordingTransactions Backend;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Test world exists"), TestWorld.World))
	{
		return false;
	}
	FTestQueries SuppliedQueries(TestWorld.World);
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	Context->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		Context->RemoveFromRoot();
	};
	Context->Initialize(&SuppliedQueries, &Backend);
	if (!TestNotNull(TEXT("Context initializes with supplied APIs"), Context->ToolManager.Get())
		|| !TestNotNull(TEXT("Context creates the gizmo manager with supplied APIs"), Context->GizmoManager.Get()))
	{
		return false;
	}
	IToolsContextQueriesAPI* Queries = Context->ToolManager->GetContextQueriesAPI();
	TestTrue(TEXT("Both managers share the owned queries adapter"),
		Queries == Context->GetContextQueriesAPI() && Context->GizmoManager->GetContextQueriesAPI() == Queries);
	FViewCameraState DelegatedCamera;
	Queries->GetCurrentViewState(DelegatedCamera);
	TestTrue(TEXT("Queries forward the supplied camera and world"),
		DelegatedCamera.Position.Equals(FVector(123, 456, 789))
		&& Queries->GetCurrentEditingWorld() == TestWorld.World);
	IToolsContextTransactionsAPI* Transactions = Context->ToolManager->GetContextTransactionsAPI();
	if (!TestNotNull(TEXT("Tool manager receives the transactions adapter"), Transactions))
	{
		return false;
	}
	const FText Description = FText::FromString(TEXT("Test change"));
	Transactions->DisplayMessage(Description, EToolMessageLevel::UserNotification);
	TestTrue(TEXT("Message and severity reach the backend"), Backend.LastMessage.EqualTo(Description)
		&& Backend.LastLevel == EToolMessageLevel::UserNotification);
	Transactions->PostInvalidation();
	TestEqual(TEXT("Invalidation reaches the backend"), Backend.InvalidationCount, 1);
	Transactions->BeginUndoTransaction(Description);
	TUniquePtr<FToolCommandChange> Change = MakeUnique<FTrackedChange>(ApplyCount, DestroyCount);
	FToolCommandChange* OriginalChange = Change.Get();
	Transactions->AppendChange(Context, MoveTemp(Change), Description);
	Transactions->EndUndoTransaction();
	TestEqual(TEXT("Transaction begins once"), Backend.BeginCount, 1);
	TestEqual(TEXT("Transaction ends once"), Backend.EndCount, 1);
	TestTrue(TEXT("Backend receives ownership, target and description"),
		Backend.LastChange.Get() == OriginalChange && Backend.LastTarget == Context
		&& Backend.LastDescription.EqualTo(Description));
	TestEqual(TEXT("Appending an already-applied change does not apply it again"), ApplyCount, 0);
	TestFalse(TEXT("A borrowed transaction backend does not advertise a second undo history"), Context->GetUndoRedo().CanUndo());
	TestFalse(TEXT("A borrowed transaction backend does not advertise a second redo history"), Context->GetUndoRedo().CanRedo());
	TestFalse(TEXT("Runtime undo does not consume a backend-owned change"), Context->GetUndoRedo().Undo());
	TestFalse(TEXT("Runtime redo does not replay a backend-owned change"), Context->GetUndoRedo().Redo());

	FSelectedObjectsChangeList SelectionRequest{};
	SelectionRequest.ModificationType = ESelectedObjectsModificationType::Clear;
	TestFalse(TEXT("Backend rejection is returned"), Transactions->RequestSelectionChange(SelectionRequest));
	Backend.bAcceptSelection = true;
	TestTrue(TEXT("Backend acceptance is returned"), Transactions->RequestSelectionChange(SelectionRequest));
	TestTrue(TEXT("Selection request reaches the backend"),
		Backend.LastSelection.ModificationType == ESelectedObjectsModificationType::Clear);

	Context->Shutdown();
	TestTrue(TEXT("Shutdown leaves the backend-owned change alive"), Backend.LastChange.Get() == OriginalChange);
	TestTrue(TEXT("Shutdown leaves the borrowed queries usable"),
		SuppliedQueries.GetCurrentEditingWorld() == TestWorld.World);
	Context->Initialize(nullptr, &Backend);
	if (!TestNotNull(TEXT("Context initializes with only supplied transactions"), Context->ToolManager.Get()))
	{
		return false;
	}
	Queries = Context->ToolManager->GetContextQueriesAPI();
	TestTrue(TEXT("Missing queries fall back independently of supplied transactions"),
		Queries && Queries->GetCurrentEditingWorld() == TestWorld.World);
	Context->ToolManager->GetContextTransactionsAPI()->PostInvalidation();
	TestEqual(TEXT("Supplied transactions still receive calls with default queries"), Backend.InvalidationCount, 2);
	Context->Shutdown();
	Context->Initialize(&SuppliedQueries, nullptr);
	if (!TestNotNull(TEXT("Context initializes with only supplied queries"), Context->ToolManager.Get()))
	{
		return false;
	}
	Transactions = Context->ToolManager->GetContextTransactionsAPI();
	if (!TestNotNull(TEXT("Missing transactions use the default implementation"), Transactions))
	{
		return false;
	}
	Context->ToolManager->GetContextQueriesAPI()->GetCurrentViewState(DelegatedCamera);
	TestTrue(TEXT("Missing transactions preserve the supplied queries"),
		DelegatedCamera.Position.Equals(FVector(123, 456, 789)));
	TestFalse(TEXT("Selection is rejected without a host backend"), Transactions->RequestSelectionChange(SelectionRequest));
	Transactions->AppendChange(Context, MakeUnique<FTrackedChange>(ApplyCount, DestroyCount), Description);
	TestEqual(TEXT("Default transactions retain undo history"), DestroyCount, 0);
	TestTrue(TEXT("Default transactions support undo without a plugin backend"), Context->GetUndoRedo().CanUndo());
	TestEqual(TEXT("Appending a default transaction does not reapply it"), ApplyCount, 0);
	TestTrue(TEXT("Default history can undo"), Context->GetUndoRedo().Undo());
	TestTrue(TEXT("Default history can redo"), Context->GetUndoRedo().Redo());
	TestEqual(TEXT("Redo applies the stored change once"), ApplyCount, 1);
	Context->Shutdown();
	TestEqual(TEXT("Shutdown releases owned history"), DestroyCount, 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorRuntimeHistoryTest,
	"VTBOWTEditor.ToolsContext.EmptyExpiredAndBranchedHistory",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorRuntimeHistoryTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Game world exists"), TestWorld.World))
	{
		return false;
	}
	int32 Value = 0;
	bool bLiveExpired = false;
	bool bOtherExpired = false;
	const bool bNestedExpired = false;
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	Context->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		Context->RemoveFromRoot();
	};
	Context->Initialize();
	IToolsContextTransactionsAPI* Transactions = Context->GetContextTransactionAPI();
	if (!TestNotNull(TEXT("Runtime transactions exist"), Transactions))
	{
		return false;
	}
	const FText Description = FText::FromString(TEXT("History regression"));
	Transactions->BeginUndoTransaction(Description);
	Transactions->EndUndoTransaction();
	TestFalse(TEXT("Empty transactions do not create undo history"), Context->GetUndoRedo().CanUndo());
	TestFalse(TEXT("Empty history cannot be undone"), Context->GetUndoRedo().Undo());
	TestFalse(TEXT("Empty history cannot be redone"), Context->GetUndoRedo().Redo());

	Value = 1;
	Transactions->AppendChange(Context, MakeUnique<FValueChange>(Value, 0, 1, bLiveExpired), Description);
	Value = 2;
	Transactions->AppendChange(Context, MakeUnique<FValueChange>(Value, 1, 2, bOtherExpired), Description);
	bOtherExpired = true;
	TestTrue(TEXT("Undo availability skips an expired newest change"), Context->GetUndoRedo().CanUndo());
	TestTrue(TEXT("Undo skips the expired change and applies the older one"), Context->GetUndoRedo().Undo());
	TestEqual(TEXT("Undo restores the live change's original value"), Value, 0);
	TestTrue(TEXT("The live change remains redoable"), Context->GetUndoRedo().Redo());
	TestEqual(TEXT("Redo restores the live change's edited value"), Value, 1);
	TestFalse(TEXT("Only expired redo entries are not advertised"), Context->GetUndoRedo().CanRedo());
	TestFalse(TEXT("Redo skips expired entries without applying them"), Context->GetUndoRedo().Redo());
	TestEqual(TEXT("Expired redo leaves the edited value unchanged"), Value, 1);

	TestTrue(TEXT("The live edit can be undone before creating a branch"), Context->GetUndoRedo().Undo());
	Transactions->BeginUndoTransaction(Description);
	Transactions->EndUndoTransaction();
	TestTrue(TEXT("An empty transaction preserves the redo branch"), Context->GetUndoRedo().CanRedo());
	Value = 3;
	Transactions->AppendChange(Context, MakeUnique<FValueChange>(Value, 0, 3, bLiveExpired), Description);
	TestFalse(TEXT("A new edit discards the previous redo branch"), Context->GetUndoRedo().CanRedo());
	TestTrue(TEXT("The replacement branch can be undone"), Context->GetUndoRedo().Undo());
	TestEqual(TEXT("Branch undo restores its starting value"), Value, 0);
	TestTrue(TEXT("The replacement branch can be redone"), Context->GetUndoRedo().Redo());
	TestEqual(TEXT("Branch redo restores only the replacement edit"), Value, 3);
	bLiveExpired = true;
	TestFalse(TEXT("Fully expired history has no applicable undo"), Context->GetUndoRedo().CanUndo());
	TestFalse(TEXT("Fully expired undo returns false"), Context->GetUndoRedo().Undo());
	TestFalse(TEXT("Fully expired history has no applicable redo"), Context->GetUndoRedo().CanRedo());
	TestFalse(TEXT("Fully expired redo returns false"), Context->GetUndoRedo().Redo());
	TestEqual(TEXT("Expired history never mutates the value"), Value, 3);

	IVTBOWTEditorUndoRedo& UndoRedo = Context->GetUndoRedo();
	Transactions->BeginUndoTransaction(Description);
	Value = 4;
	Transactions->AppendChange(Context, MakeUnique<FValueChange>(Value, 3, 4, bNestedExpired), Description);
	Transactions->BeginUndoTransaction(Description);
	Value = 5;
	Transactions->AppendChange(Context, MakeUnique<FValueChange>(Value, 4, 5, bNestedExpired), Description);
	Transactions->EndUndoTransaction();
	TestFalse(TEXT("Ending an inner transaction does not expose an incomplete undo group"), UndoRedo.CanUndo());
	TestFalse(TEXT("An open outer transaction cannot be undone"), UndoRedo.Undo());
	TestFalse(TEXT("An open outer transaction cannot be redone"), UndoRedo.Redo());
	TestEqual(TEXT("Rejected replay leaves the in-progress group unchanged"), Value, 5);
	Value = 6;
	Transactions->AppendChange(Context, MakeUnique<FValueChange>(Value, 5, 6, bNestedExpired), Description);
	Transactions->EndUndoTransaction();
	TestTrue(TEXT("Ending the outer transaction publishes one undo group"), UndoRedo.CanUndo());
	TestTrue(TEXT("One undo reverses every nested change"), UndoRedo.Undo());
	TestEqual(TEXT("Nested undo replays changes in reverse order"), Value, 3);
	TestFalse(TEXT("Inner transactions do not become separate undo entries"), UndoRedo.CanUndo());
	TestTrue(TEXT("One redo restores the complete nested group"), UndoRedo.Redo());
	TestEqual(TEXT("Nested redo replays changes in their original order"), Value, 6);
	TestFalse(TEXT("The completed nested redo leaves no extra redo entries"), UndoRedo.CanRedo());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorRuntimeSelectionRefreshTest,
	"VTBOWTEditor.ToolsContext.SelectionRefreshAfterTargetRemoval",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorRuntimeSelectionRefreshTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Game world exists"), TestWorld.World))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	ConfigureSelectionManager(Context);
	Context->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		Context->RemoveFromRoot();
	};
	Context->InitializeContext(TestWorld.World);
	AActor* First = SpawnMovableActor(TestWorld.World, FVector::ZeroVector);
	AActor* Second = SpawnMovableActor(TestWorld.World, FVector(100, 0, 0));
	if (!TestNotNull(TEXT("First actor exists"), First) || !TestNotNull(TEXT("Second actor exists"), Second))
	{
		return false;
	}
	TArray<TWeakObjectPtr<AActor>> Actors = {Second, First, Second, nullptr};
	FToolBuilderState Selection;
	Context->GetSceneState().SetSelection({Second, First, Second, nullptr},
		{Second->GetRootComponent(), First->GetRootComponent(), Second->GetRootComponent(), nullptr});
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Queries normalize explicit component selection while preserving order"),
		Selection.SelectedComponents == TArray<UActorComponent*>({Second->GetRootComponent(), First->GetRootComponent()}));
	Context->GetSceneState().SetSelection({First}, {});
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Explicit selection preserves an empty component list without deriving actor roots"),
		Selection.SelectedActors == TArray<AActor*>({First}) && Selection.SelectedComponents.IsEmpty());
	Context->GetSceneState().SetActorSelection({Second, First, Second, nullptr});
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Actor-only selection normalizes duplicate and null root components"),
		Selection.SelectedComponents == TArray<UActorComponent*>({Second->GetRootComponent(), First->GetRootComponent()}));
	TestTrue(TEXT("Selection accepts ordered actors with duplicates"),
		SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(Actors)));
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Selection preserves first occurrence order and removes duplicates"),
		Selection.SelectedActors == TArray<AActor*>({Second, First}));
	UVTBOWTEditorRepositionalGizmo* Gizmo = FindRuntimeGizmo(Context);
	if (!TestNotNull(TEXT("Selection gizmo exists"), Gizmo))
	{
		return false;
	}
	TestFalse(TEXT("The initial selection target state is current"), Gizmo->IsSelectionStateStale());
	const FVector ExternalLocation(20, 30, 40);
	First->SetActorLocation(ExternalLocation);
	TestTrue(TEXT("An external transform change makes the selection target state stale"), Gizmo->IsSelectionStateStale());
	if (!TestTrue(TEXT("An idle refresh rebuilds an externally moved target"), SynchronizeSelection(Context, {}))
		|| !TestNotNull(TEXT("The refreshed selection has a transform proxy"), Gizmo->GetTransformProxy()))
	{
		return false;
	}
	TestFalse(TEXT("Refreshing makes the selection target state current"), Gizmo->IsSelectionStateStale());
	const FVector SharedDelta(15, -5, 10);
	FTransform RefreshedTransform = Gizmo->GetTransformProxy()->GetTransform();
	RefreshedTransform.AddToTranslation(SharedDelta);
	Gizmo->SetNewGizmoTransform(RefreshedTransform);
	TestTrue(TEXT("The rebuilt proxy transforms the externally moved actor from its new location"),
		First->GetActorLocation().Equals(ExternalLocation + SharedDelta));
	TestTrue(TEXT("The rebuilt proxy applies the same delta to the other selected actor"),
		Second->GetActorLocation().Equals(FVector(100, 0, 0) + SharedDelta));
	TestFalse(TEXT("Gizmo edits keep the refreshed target state current"), Gizmo->IsSelectionStateStale());
	First->SetActorLocation(FVector::ZeroVector);
	Second->SetActorLocation(FVector(100, 0, 0));
	TestTrue(TEXT("Selection can refresh after restoring the original actor transforms"), SynchronizeSelection(Context, {}));
	TestFalse(TEXT("Restored actor transforms become the current target state"), Gizmo->IsSelectionStateStale());

	Second->Destroy();
	TestTrue(TEXT("An idle refresh removes a destroyed actor"), SynchronizeSelection(Context, {}));
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Destroyed actor is removed from queries"), Selection.SelectedActors == TArray<AActor*>({First}));
	if (!TestNotNull(TEXT("The surviving actor still has a transform target"), Gizmo->GetTransformProxy()))
	{
		return false;
	}
	FTransform Transform = Gizmo->GetTransformProxy()->GetTransform();
	Transform.AddToTranslation(FVector(25, 0, 0));
	Gizmo->SetNewGizmoTransform(Transform);
	TestTrue(TEXT("The rebuilt target still transforms the surviving actor"), First->GetActorLocation().Equals(FVector(25, 0, 0)));

	First->GetRootComponent()->DestroyComponent();
	TestTrue(TEXT("An idle refresh handles a destroyed root component"), SynchronizeSelection(Context, {}));
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("An actor without a root remains selected without a component target"),
		Selection.SelectedActors == TArray<AActor*>({First}) && Selection.SelectedComponents.IsEmpty());
	TestNull(TEXT("Removing the last transformable component clears the gizmo target"), Gizmo->ActiveTarget.Get());
	TestNull(TEXT("Removing the last transformable component releases the proxy"), Gizmo->GetTransformProxy());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorRuntimeTransformTest,
	"VTBOWTEditor.ToolsContext.RuntimeSelectionTransformUndo",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorRuntimeTransformTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Game world exists"), TestWorld.World))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	ConfigureSelectionManager(Context);
	Context->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		Context->RemoveFromRoot();
	};
	Context->Initialize();
	UInteractiveToolManager* BareManager = Context->ToolManager;
	if (!TestNotNull(TEXT("Bare initialization creates managers before host attachment"), BareManager)
		|| !TestTrue(TEXT("Runtime initialization succeeds after bare initialization"), Context->InitializeContext(TestWorld.World)))
	{
		return false;
	}
	TestTrue(TEXT("Runtime initialization preserves the same-world session"),
		Context->ToolManager == BareManager && Context->IsRuntimeReady()
		&& Context->GetContextQueriesAPI()->GetCurrentEditingWorld() == TestWorld.World);
	UInteractiveToolManager* RuntimeManager = Context->ToolManager;
	TestTrue(TEXT("Runtime initialization is idempotent"), Context->InitializeContext(TestWorld.World));
	TestTrue(TEXT("Repeated runtime initialization preserves managers"), Context->ToolManager == RuntimeManager);
	const FVector OriginalLocation(10, 20, 30);
	AActor* Actor = SpawnMovableActor(TestWorld.World, OriginalLocation);
	if (!TestNotNull(TEXT("Movable actor exists"), Actor))
	{
		return false;
	}
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Actor);
	if (!TestTrue(TEXT("Selection creates a runtime transform gizmo"),
		SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(Actors))))
	{
		return false;
	}
	FToolBuilderState Selection;
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Owned queries expose runtime selection and component"),
		Selection.SelectedActors.Num() == 1 && Selection.SelectedActors[0] == Actor
		&& Selection.SelectedComponents.Num() == 1 && Selection.SelectedComponents[0] == Actor->GetRootComponent());
	UVTBOWTEditorRepositionalGizmo* Gizmo = FindRuntimeGizmo(Context);
	if (!TestNotNull(TEXT("Runtime gizmo is registered"), Gizmo)
		|| !TestNotNull(TEXT("Selection is backed by a transform proxy"), Gizmo->GetTransformProxy()))
	{
		return false;
	}
	const FVector Translation(75, -25, 10);
	FTransform ChangedTransform = Gizmo->GetTransformProxy()->GetTransform();
	ChangedTransform.AddToTranslation(Translation);
	Gizmo->SetNewGizmoTransform(ChangedTransform);
	TestTrue(TEXT("Gizmo transform moves the selected actor"), Actor->GetActorLocation().Equals(OriginalLocation + Translation));
	TestTrue(TEXT("Gizmo edit enters runtime history"), Context->GetUndoRedo().CanUndo());
	TestTrue(TEXT("Runtime undo succeeds"), Context->GetUndoRedo().Undo());
	TestTrue(TEXT("Undo restores the actor"), Actor->GetActorLocation().Equals(OriginalLocation));
	TestTrue(TEXT("Runtime redo succeeds"), Context->GetUndoRedo().Redo());
	TestTrue(TEXT("Redo restores the edited actor"), Actor->GetActorLocation().Equals(OriginalLocation + Translation));

	const TWeakObjectPtr<UTransformProxy> HistoryProxy = Gizmo->GetTransformProxy();
	Actors.Reset();
	TestTrue(TEXT("Selection can be cleared"),
		SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(Actors)));
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestTrue(TEXT("Selection clear reaches queries and gizmo target"),
		Selection.SelectedActors.IsEmpty() && Selection.SelectedComponents.IsEmpty() && Gizmo->ActiveTarget == nullptr);
	CollectGarbage(RF_NoFlags);
	TestTrue(TEXT("Undo history retains the old proxy after selection clear and GC"), HistoryProxy.IsValid());
	TestTrue(TEXT("An unselected transform remains undoable after GC"), Context->GetUndoRedo().Undo());
	TestTrue(TEXT("History-only proxy undo restores the actor"), Actor->GetActorLocation().Equals(OriginalLocation));
	TestTrue(TEXT("An unselected transform remains redoable after GC"), Context->GetUndoRedo().Redo());
	TestTrue(TEXT("History-only proxy redo restores the edited actor"), Actor->GetActorLocation().Equals(OriginalLocation + Translation));
	Context->Shutdown();
	CollectGarbage(RF_NoFlags);
	TestFalse(TEXT("Shutdown releases the proxy retained only by history"), HistoryProxy.IsValid());
	TestFalse(TEXT("Shutdown leaves the runtime unavailable"), Context->IsRuntimeReady());
	TestNull(TEXT("Shutdown releases the owned adapters"), Context->GetContextQueriesAPI());
	TestTrue(TEXT("Runtime can initialize again after shutdown"), Context->InitializeContext(TestWorld.World));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorRuntimeReplayShutdownTest,
	"VTBOWTEditor.ToolsContext.RuntimeUndoRedoCallbackShutdown",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorRuntimeReplayShutdownTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Game world exists"), TestWorld.World))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	ConfigureSelectionManager(Context);
	Context->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		Context->RemoveFromRoot();
	};
	IVTBOWTEditorUndoRedo& UndoRedo = Context->GetUndoRedo();
	TestFalse(TEXT("The owned undo service rejects replay before initialization"), UndoRedo.Undo());
	TestFalse(TEXT("The owned redo service rejects replay before initialization"), UndoRedo.Redo());
	const FVector OriginalLocation(10, 20, 30);
	const FVector Translation(40, 0, 0);
	AActor* Actor = SpawnMovableActor(TestWorld.World, OriginalLocation);
	if (!TestNotNull(TEXT("Movable actor exists"), Actor))
	{
		return false;
	}
	const TArray<TWeakObjectPtr<AActor>> Actors = {Actor};
	for (int32 Pass = 0; Pass < 2; ++Pass)
	{
		const bool bReplayRedo = Pass == 1;
		Actor->SetActorLocation(OriginalLocation);
		if (!TestTrue(TEXT("The context initializes for another replay session"), Context->InitializeContext(TestWorld.World)))
		{
			return false;
		}
		TestTrue(TEXT("The same undo service remains available across sessions"), &Context->GetUndoRedo() == &UndoRedo);
		TestFalse(TEXT("A new session has no leftover undo history"), UndoRedo.CanUndo());
		TestFalse(TEXT("A new session has no leftover redo history"), UndoRedo.CanRedo());
		if (!TestTrue(TEXT("The session creates a selection target"),
			SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(Actors))))
		{
			return false;
		}
		UVTBOWTEditorRepositionalGizmo* Gizmo = FindRuntimeGizmo(Context);
		UTransformProxy* Proxy = Gizmo ? Gizmo->GetTransformProxy() : nullptr;
		if (!TestNotNull(TEXT("The selected actor has a transform proxy"), Proxy))
		{
			return false;
		}
		FTransform ChangedTransform = Proxy->GetTransform();
		ChangedTransform.AddToTranslation(Translation);
		Gizmo->SetNewGizmoTransform(ChangedTransform);
		if (bReplayRedo && !TestTrue(TEXT("Undo prepares the change for a redo callback"), UndoRedo.Undo()))
		{
			return false;
		}
		int32 ReplayCallbackCount = 0;
		int32 ReplayAppendApplyCount = 0;
		int32 ReplayAppendDestroyCount = 0;
		const FDelegateHandle ReplayHandle = Proxy->OnTransformChangedUndoRedo.AddLambda(
			[this, Context, &UndoRedo, &ReplayCallbackCount, &ReplayAppendApplyCount, &ReplayAppendDestroyCount](UTransformProxy*, FTransform)
			{
				++ReplayCallbackCount;
				TestFalse(TEXT("A replay callback cannot advertise another undo"), UndoRedo.CanUndo());
				TestFalse(TEXT("A replay callback cannot advertise another redo"), UndoRedo.CanRedo());
				TestFalse(TEXT("A replay callback cannot recursively undo"), UndoRedo.Undo());
				TestFalse(TEXT("A replay callback cannot recursively redo"), UndoRedo.Redo());
				Context->GetInput().CancelActiveInteraction();
				IToolsContextTransactionsAPI* Transactions = Context->GetContextTransactionAPI();
				const FText Description = FText::FromString(TEXT("Change requested during replay"));
				Transactions->BeginUndoTransaction(Description);
				Transactions->AppendChange(Context,
					MakeUnique<FTrackedChange>(ReplayAppendApplyCount, ReplayAppendDestroyCount), Description);
				Transactions->EndUndoTransaction();
				TestEqual(TEXT("Replay discards changes emitted recursively through ITF"), ReplayAppendDestroyCount, 1);
				Context->Shutdown();
				TestFalse(TEXT("Shutdown requested during replay immediately withdraws readiness"), Context->IsRuntimeReady());
				TestNotNull(TEXT("Replay keeps the manager alive until its callback returns"), Context->ToolManager.Get());
				TestNotNull(TEXT("Replay keeps transaction adapters alive until its callback returns"), Context->GetContextTransactionAPI());
			});
		const bool bReplayed = bReplayRedo ? UndoRedo.Redo() : UndoRedo.Undo();
		Proxy->OnTransformChangedUndoRedo.Remove(ReplayHandle);
		TestTrue(bReplayRedo ? TEXT("Redo completes despite callback shutdown") : TEXT("Undo completes despite callback shutdown"), bReplayed);
		TestEqual(TEXT("Replay shutdown invokes the transform callback exactly once"), ReplayCallbackCount, 1);
		TestEqual(TEXT("A change emitted during replay is never applied"), ReplayAppendApplyCount, 0);
		TestTrue(TEXT("Replay reaches the requested actor transform before shutdown completes"),
			Actor->GetActorLocation().Equals(bReplayRedo ? OriginalLocation + Translation : OriginalLocation));
		TestNull(TEXT("Deferred replay shutdown releases managers before returning"), Context->ToolManager.Get());
		TestNull(TEXT("Deferred replay shutdown releases transaction adapters before returning"), Context->GetContextTransactionAPI());
		TestFalse(TEXT("The surviving service has no undo history after shutdown"), UndoRedo.CanUndo());
		TestFalse(TEXT("The surviving service has no redo history after shutdown"), UndoRedo.CanRedo());
		TestFalse(TEXT("The surviving service rejects undo while shut down"), UndoRedo.Undo());
		TestFalse(TEXT("The surviving service rejects redo while shut down"), UndoRedo.Redo());
	}
	TestTrue(TEXT("The context can initialize again after both replay shutdown paths"), Context->InitializeContext(TestWorld.World));
	TestTrue(TEXT("Reinitialization preserves the previously acquired service interface"), &Context->GetUndoRedo() == &UndoRedo);
	TestFalse(TEXT("Reinitialization starts with empty undo history"), UndoRedo.CanUndo());
	TestFalse(TEXT("Reinitialization starts with empty redo history"), UndoRedo.CanRedo());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorGizmoSelectionOwnershipTest,
	"VTBOWTEditor.ToolsContext.GizmoSelectionOwnershipAndAttachment",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorGizmoSelectionOwnershipTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Game world exists"), TestWorld.World))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	UVTBOWTEditorToolsContext* OtherContext = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	ConfigureSelectionManager(Context);
	ConfigureSelectionManager(OtherContext);
	Context->AddToRoot();
	OtherContext->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		OtherContext->Shutdown();
		Context->RemoveFromRoot();
		OtherContext->RemoveFromRoot();
	};
	if (!TestTrue(TEXT("First context initializes"), Context->InitializeContext(TestWorld.World))
		|| !TestTrue(TEXT("Independent context initializes"), OtherContext->InitializeContext(TestWorld.World)))
	{
		return false;
	}
	AActor* Parent = SpawnMovableActor(TestWorld.World, FVector::ZeroVector);
	AActor* Child = SpawnMovableActor(TestWorld.World, FVector(25, 10, 0));
	AActor* OtherActor = SpawnMovableActor(TestWorld.World, FVector(500, 0, 0));
	if (!TestNotNull(TEXT("Parent actor exists"), Parent)
		|| !TestNotNull(TEXT("Child actor exists"), Child)
		|| !TestNotNull(TEXT("Independent actor exists"), OtherActor))
	{
		return false;
	}
	if (!TestTrue(TEXT("Child attaches without moving"),
		Child->AttachToActor(Parent, FAttachmentTransformRules::KeepWorldTransform)))
	{
		return false;
	}
	const FVector ChildLocation = Child->GetActorLocation();
	const FTransform ChildRelativeTransform = Child->GetRootComponent()->GetRelativeTransform();
	const TArray<TWeakObjectPtr<AActor>> AttachedActors = {Parent, Child};
	const TArray<TWeakObjectPtr<AActor>> OtherActors = {OtherActor};
	if (!TestTrue(TEXT("Parent and child are selected together"),
		SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(AttachedActors)))
		|| !TestTrue(TEXT("The other context selects its own actor"),
			SynchronizeSelection(OtherContext, TOptional<TArray<TWeakObjectPtr<AActor>>>(OtherActors))))
	{
		return false;
	}
	TWeakObjectPtr<UVTBOWTEditorRepositionalGizmo> WeakGizmo = FindRuntimeGizmo(Context);
	TWeakObjectPtr<UVTBOWTEditorRepositionalGizmo> WeakOtherGizmo = FindRuntimeGizmo(OtherContext);
	if (!TestNotNull(TEXT("First selection creates a gizmo"), WeakGizmo.Get())
		|| !TestNotNull(TEXT("Independent selection creates a gizmo"), WeakOtherGizmo.Get()))
	{
		return false;
	}
	TWeakObjectPtr<UTransformProxy> WeakProxy = WeakGizmo->GetTransformProxy();
	TWeakObjectPtr<UTransformProxy> WeakOtherProxy = WeakOtherGizmo->GetTransformProxy();
	CollectGarbage(RF_NoFlags);
	if (!TestNotNull(TEXT("Rooted context retains its gizmo through GC"), WeakGizmo.Get())
		|| !TestNotNull(TEXT("The other context retains its gizmo through GC"), WeakOtherGizmo.Get())
		|| !TestNotNull(TEXT("First selection retains its proxy through GC"), WeakProxy.Get())
		|| !TestNotNull(TEXT("Independent selection retains its proxy through GC"), WeakOtherProxy.Get()))
	{
		return false;
	}
	UVTBOWTEditorRepositionalGizmo* Gizmo = WeakGizmo.Get();
	UVTBOWTEditorRepositionalGizmo* OtherGizmo = WeakOtherGizmo.Get();
	TestTrue(TEXT("Each proxy keeps its owning gizmo as its outer"),
		WeakProxy->GetOuter() == Gizmo && WeakOtherProxy->GetOuter() == OtherGizmo);
	TestTrue(TEXT("Independent contexts own different selection proxies"), WeakProxy != WeakOtherProxy);
	TArray<AActor*> SelectedActors;
	Gizmo->GetSelectedActors(SelectedActors);
	TestTrue(TEXT("Selection contents survive GC"), SelectedActors == TArray<AActor*>({Parent, Child}));
	TestTrue(TEXT("The gizmo still exposes the retained proxy"), Gizmo->GetTransformProxy() == WeakProxy.Get());
	FTransform Transform = Gizmo->GetTransformProxy()->GetTransform();
	Transform.AddToTranslation(FVector(40, 0, 0));
	Gizmo->SetNewGizmoTransform(Transform);
	TestTrue(TEXT("The selected parent moves once"), Parent->GetActorLocation().Equals(FVector(40, 0, 0)));
	TestTrue(TEXT("The selected attached child moves once with its parent"),
		Child->GetActorLocation().Equals(ChildLocation + FVector(40, 0, 0))
		&& Child->GetRootComponent()->GetRelativeTransform().Equals(ChildRelativeTransform));
	TestFalse(TEXT("Proxy notifications keep the selection transform cache current after GC"), Gizmo->IsSelectionStateStale());
	TestTrue(TEXT("Editing the first selection leaves the other actor unchanged"),
		OtherActor->GetActorLocation().Equals(FVector(500, 0, 0)));

	const TArray<TWeakObjectPtr<AActor>> NoActors;
	TestTrue(TEXT("The first selection can be cleared"),
		SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(NoActors)));
	Gizmo->GetSelectedActors(SelectedActors);
	TestTrue(TEXT("Clearing releases the selection and active target"),
		SelectedActors.IsEmpty() && Gizmo->GetTransformProxy() == nullptr && Gizmo->ActiveTarget == nullptr);
	OtherGizmo->GetSelectedActors(SelectedActors);
	TestTrue(TEXT("Clearing the first context preserves the other selection"),
		SelectedActors == TArray<AActor*>({OtherActor}) && OtherGizmo->GetTransformProxy() == WeakOtherProxy.Get());
	const TArray<TWeakObjectPtr<AActor>> ReorderedActors = {Child, Parent};
	if (!TestTrue(TEXT("Parent and child can be reselected in the opposite order"),
		SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(ReorderedActors))))
	{
		return false;
	}
	UTransformProxy* BeforeRebuild = Gizmo->GetTransformProxy();
	Gizmo->RebuildFromCurrentTransforms();
	if (!TestNotNull(TEXT("Explicit rebuild creates a selection proxy"), Gizmo->GetTransformProxy()))
	{
		return false;
	}
	TestTrue(TEXT("Explicit rebuild replaces the proxy while preserving its outer"),
		Gizmo->GetTransformProxy() != BeforeRebuild && Gizmo->GetTransformProxy()->GetOuter() == Gizmo);
	TestTrue(TEXT("The manager can activate the rebuilt proxy"), SynchronizeSelection(Context, {}));
	Transform = Gizmo->GetTransformProxy()->GetTransform();
	Transform.AddToTranslation(FVector(10, 0, 0));
	Gizmo->SetNewGizmoTransform(Transform);
	TestTrue(TEXT("Reselection and rebuild preserve parent-child transform behavior"),
		Parent->GetActorLocation().Equals(FVector(50, 0, 0))
		&& Child->GetActorLocation().Equals(ChildLocation + FVector(50, 0, 0))
		&& Child->GetRootComponent()->GetRelativeTransform().Equals(ChildRelativeTransform));

	Context->Shutdown();
	Gizmo->GetSelectedActors(SelectedActors);
	TestTrue(TEXT("Shutdown clears the first gizmo's selection and proxy"),
		SelectedActors.IsEmpty() && Gizmo->GetTransformProxy() == nullptr);
	TestTrue(TEXT("Shutting down the first context leaves the other ready"), OtherContext->IsRuntimeReady());
	OtherGizmo->GetSelectedActors(SelectedActors);
	TestTrue(TEXT("The other context retains its independent selection after shutdown"),
		SelectedActors == TArray<AActor*>({OtherActor}) && OtherGizmo->GetTransformProxy() == WeakOtherProxy.Get());
	Transform = OtherGizmo->GetTransformProxy()->GetTransform();
	Transform.AddToTranslation(FVector(15, 0, 0));
	OtherGizmo->SetNewGizmoTransform(Transform);
	TestTrue(TEXT("The other context remains editable after the first shuts down"),
		OtherActor->GetActorLocation().Equals(FVector(515, 0, 0)));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorGizmoVisualActorTest,
	"VTBOWTEditor.ToolsContext.GizmoVisualActorOwnershipAndFactory",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorGizmoVisualActorTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Game world exists"), TestWorld.World))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	ConfigureSelectionManager(Context);
	Context->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		Context->RemoveFromRoot();
	};
	if (!TestTrue(TEXT("Runtime context initializes"), Context->InitializeContext(TestWorld.World)))
	{
		return false;
	}
	AActor* Actor = SpawnMovableActor(TestWorld.World, FVector::ZeroVector);
	if (!TestNotNull(TEXT("Movable actor exists"), Actor))
	{
		return false;
	}
	const TArray<TWeakObjectPtr<AActor>> Actors = {Actor};
	if (!TestTrue(TEXT("Selection creates the default factory's gizmo"),
		SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(Actors))))
	{
		return false;
	}
	UVTBOWTEditorRepositionalGizmo* DefaultGizmo = FindRuntimeGizmo(Context);
	ACombinedTransformGizmoActor* DefaultActor = DefaultGizmo ? DefaultGizmo->GetGizmoActor() : nullptr;
	if (!TestNotNull(TEXT("The default factory creates an actual gizmo actor"), DefaultActor))
	{
		return false;
	}
	const auto TestOwnedVisualComponent = [this](ACombinedTransformGizmoActor* GizmoActor)
	{
		TArray<UVTBOWTEditorGizmoVisualComponent*> Components;
		GizmoActor->GetComponents(Components);
		if (!TestEqual(TEXT("Each gizmo actor has exactly one visual component"), Components.Num(), 1))
		{
			return false;
		}
		UVTBOWTEditorGizmoVisualComponent* Visuals = Components[0];
		TestTrue(TEXT("The visual component is owned and registered on the actual gizmo actor"),
			Visuals->GetOwner() == GizmoActor && Visuals->GetOuter() == GizmoActor && Visuals->IsRegistered());
		TestTrue(TEXT("Repeated component attachment reuses the actor's existing visual component"),
			UVTBOWTEditorGizmoVisualComponent::FindOrAddTo(GizmoActor) == Visuals);
		GizmoActor->GetComponents(Components);
		return TestEqual(TEXT("Repeated attachment does not add a second visual component"), Components.Num(), 1);
	};
	if (!TestOwnedVisualComponent(DefaultActor)
		|| !TestNotNull(TEXT("Default actor has a nonuniform scale handle"), DefaultActor->PlaneScaleXY.Get())
		|| !TestNotNull(TEXT("Default actor has a uniform scale handle"), DefaultActor->UniformScale.Get())
		|| !TestNotNull(TEXT("Default actor has a rotation handle"), DefaultActor->RotateX.Get()))
	{
		return false;
	}
	TArray<UViewAdjustedStaticMeshGizmoComponent*> ViewComponents;
	DefaultActor->GetComponents(ViewComponents);
	if (!TestTrue(TEXT("The gizmo has multiple view-adjusted components"), ViewComponents.Num() > 1))
	{
		return false;
	}
	TArray<TObjectPtr<UPrimitiveComponent>> ActiveComponents = {ViewComponents[1]};
	TArray<UPrimitiveComponent*> UpdatedComponents;
	UVTBOWTEditorGizmoVisualComponent* DefaultVisuals = DefaultActor->FindComponentByClass<UVTBOWTEditorGizmoVisualComponent>();
	DefaultVisuals->UpdateCoordinateSystem(ActiveComponents, [&ActiveComponents, &UpdatedComponents](UPrimitiveComponent* Component)
	{
		UpdatedComponents.Add(Component);
		if (UpdatedComponents.Num() == 1)
		{
			ActiveComponents.Reset();
		}
	});
	TestTrue(TEXT("Coordinate updates observe a component removed from the active list by an earlier callback"),
		UpdatedComponents.Contains(ViewComponents[1]));
	TestEqual(TEXT("Callback changes to the active list are visible for all remaining components"),
		UpdatedComponents.Num(), ViewComponents.Num());
	DefaultGizmo->SetVisibility(true);
	Context->GetSceneState().SetGizmoMode(EToolContextTransformGizmoMode::Combined);
	Context->GetSceneState().SetCoordinateSystem(EToolContextCoordinateSystem::World);
	Context->TickRuntime(0.0f);
	TestTrue(TEXT("Combined world mode keeps uniform scale and rotation visible"),
		DefaultActor->UniformScale->IsVisible() && DefaultActor->RotateX->IsVisible());
	TestFalse(TEXT("Combined world mode hides nonuniform scale"), DefaultActor->PlaneScaleXY->IsVisible());
	Context->GetSceneState().SetCoordinateSystem(EToolContextCoordinateSystem::Local);
	Context->TickRuntime(0.0f);
	TestTrue(TEXT("Combined local mode shows nonuniform scale for one selected actor"), DefaultActor->PlaneScaleXY->IsVisible());
	Context->GetSceneState().SetCoordinateSystem(EToolContextCoordinateSystem::World);
	Context->GetSceneState().SetGizmoMode(EToolContextTransformGizmoMode::Scale);
	Context->TickRuntime(0.0f);
	TestTrue(TEXT("Scale mode uses local coordinates and keeps nonuniform scale visible"),
		DefaultGizmo->CurrentCoordinateSystem == EToolContextCoordinateSystem::Local && DefaultActor->PlaneScaleXY->IsVisible());
	TestFalse(TEXT("Scale mode hides rotation handles"), DefaultActor->RotateX->IsVisible());
	Context->GetSceneState().SetGizmoMode(EToolContextTransformGizmoMode::Translation);
	Context->TickRuntime(0.0f);
	TestFalse(TEXT("Translation mode hides nonuniform scale handles"), DefaultActor->PlaneScaleXY->IsVisible());
	TestFalse(TEXT("Translation mode hides rotation handles"), DefaultActor->RotateX->IsVisible());

	UGizmoViewContext* ViewContext = Context->ContextObjectStore->FindContext<UGizmoViewContext>();
	UStaticMesh* CustomMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (!TestNotNull(TEXT("The runtime supplies a gizmo view context"), ViewContext)
		|| !TestNotNull(TEXT("The injected factory has a distinctive handle mesh"), CustomMesh))
	{
		return false;
	}
	TSharedPtr<FRecordingGizmoActorFactory> Factory = MakeShared<FRecordingGizmoActorFactory>(ViewContext, CustomMesh);
	UVTBOWTEditorRepositionalGizmoBuilder* Builder = NewObject<UVTBOWTEditorRepositionalGizmoBuilder>(Context->GizmoManager);
	Builder->GizmoActorBuilder = Factory;
	const FString BuilderIdentifier(TEXT("VTBOWT.Test.InjectedVisualFactory"));
	Context->GizmoManager->RegisterGizmoType(BuilderIdentifier, Builder);
	UVTBOWTEditorRepositionalGizmo* CustomGizmo = Cast<UVTBOWTEditorRepositionalGizmo>(
		Context->GizmoManager->CreateGizmo(BuilderIdentifier, TEXT("VTBOWT.Test.InjectedVisualGizmo"), Context));
	if (!TestNotNull(TEXT("The injected builder creates a runtime gizmo"), CustomGizmo)
		|| !TestNotNull(TEXT("The injected factory produces a gizmo actor"), CustomGizmo->GetGizmoActor())
		|| !TestNotNull(TEXT("The injected factory produces a mesh handle"), Factory->LastHandle.Get()))
	{
		return false;
	}
	ACombinedTransformGizmoActor* CustomActor = CustomGizmo->GetGizmoActor();
	TestEqual(TEXT("Gizmo setup calls the injected factory once"), Factory->CreateCount, 1);
	TestTrue(TEXT("Adding visual behavior preserves the actor returned by the injected factory"), CustomActor == Factory->LastActor.Get());
	TestTrue(TEXT("Adding visual behavior preserves the injected handle and its mesh"),
		CustomActor->TranslateX == Factory->LastHandle.Get() && Factory->LastHandle->GetStaticMesh() == CustomMesh);
	if (!TestOwnedVisualComponent(CustomActor))
	{
		return false;
	}
	CustomGizmo->SetSelection(TestWorld.World, Actors);
	CustomGizmo->SetActiveTarget(CustomGizmo->GetTransformProxy(), Context->GizmoManager);
	const ETransformGizmoSubElements ReducedElements = ETransformGizmoSubElements::TranslateAllAxes
		| ETransformGizmoSubElements::ScaleUniform;
	TestTrue(TEXT("Changing enabled elements recreates the custom actor"), CustomGizmo->SetEnabledElements(ReducedElements));
	ACombinedTransformGizmoActor* RecreatedActor = CustomGizmo->GetGizmoActor();
	if (!TestNotNull(TEXT("Element recreation produces another gizmo actor"), RecreatedActor)
		|| !TestNotNull(TEXT("Element recreation produces another injected mesh handle"), Factory->LastHandle.Get()))
	{
		return false;
	}
	TestEqual(TEXT("Element recreation uses the same injected factory"), Factory->CreateCount, 2);
	TestTrue(TEXT("Element recreation replaces the old actor with the factory's next actor"),
		RecreatedActor != CustomActor && RecreatedActor == Factory->LastActor.Get() && CustomActor->IsActorBeingDestroyed());
	TestTrue(TEXT("Element recreation preserves the injected handle mesh customization"),
		RecreatedActor->TranslateX == Factory->LastHandle.Get() && Factory->LastHandle->GetStaticMesh() == CustomMesh);
	TestTrue(TEXT("The recreated actor contains only the requested handle types"),
		RecreatedActor->TranslateX != nullptr && RecreatedActor->UniformScale != nullptr
		&& RecreatedActor->RotateX == nullptr && RecreatedActor->PlaneScaleXY == nullptr);
	TestNull(TEXT("Element recreation clears the old active target before replacing handles"), CustomGizmo->ActiveTarget.Get());
	if (!TestOwnedVisualComponent(RecreatedActor))
	{
		return false;
	}
	CustomGizmo->SetActiveTarget(CustomGizmo->GetTransformProxy(), Context->GizmoManager);
	TestTrue(TEXT("The recreated actor can reconnect the existing selection proxy"), CustomGizmo->ActiveTarget == CustomGizmo->GetTransformProxy());
	TestFalse(TEXT("Reapplying the same elements keeps the custom actor"), CustomGizmo->SetEnabledElements(ReducedElements));
	TestEqual(TEXT("Unchanged elements do not call the injected factory again"), Factory->CreateCount, 2);
	TestTrue(TEXT("Unchanged elements preserve actor identity"), CustomGizmo->GetGizmoActor() == RecreatedActor);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorGizmoHoverInteractionTest,
	"VTBOWTEditor.ToolsContext.GizmoHoverInteractionState",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorGizmoHoverInteractionTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Game world exists"), TestWorld.World))
	{
		return false;
	}
	TArray<TPair<UPrimitiveComponent*, bool>> HoverUpdates;
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	ConfigureSelectionManager(Context);
	Context->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		Context->RemoveFromRoot();
	};
	if (!TestTrue(TEXT("Runtime context initializes"), Context->InitializeContext(TestWorld.World)))
	{
		return false;
	}
	AActor* Actor = SpawnMovableActor(TestWorld.World, FVector::ZeroVector);
	if (!TestNotNull(TEXT("Movable actor exists"), Actor))
	{
		return false;
	}
	const TArray<TWeakObjectPtr<AActor>> Actors = {Actor};
	if (!TestTrue(TEXT("Selection initializes runtime gizmos"),
		SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(Actors))))
	{
		return false;
	}
	UVTBOWTEditorRepositionalGizmo* Gizmo = FindRuntimeGizmo(Context);
	if (!TestNotNull(TEXT("Runtime gizmo exists"), Gizmo)
		|| !TestNotNull(TEXT("Gizmo actor exists"), Gizmo->GetGizmoActor()))
	{
		return false;
	}
	UAxisPositionGizmo* TranslateX = FindTranslateX(Context, Gizmo);
	UGizmoComponentHitTarget* HitTarget = TranslateX
		? Cast<UGizmoComponentHitTarget>(TranslateX->HitTarget.GetObject()) : nullptr;
	if (!TestNotNull(TEXT("The active translation handle retains its component hit target"), HitTarget))
	{
		return false;
	}
	UPrimitiveComponent* TranslateXComponent = Gizmo->GetGizmoActor()->TranslateX;
	Gizmo->SetUpdateHoverFunction([&HoverUpdates](UPrimitiveComponent* Component, bool bHovering)
	{
		HoverUpdates.Emplace(Component, bHovering);
	});
	const auto TestLatestHover = [this, &HoverUpdates, TranslateXComponent](const TCHAR* Description, bool bExpected)
	{
		return TestTrue(Description, !HoverUpdates.IsEmpty()
			&& HoverUpdates.Last().Key == TranslateXComponent && HoverUpdates.Last().Value == bExpected);
	};
	HitTarget->UpdateHoverState(true);
	TestLatestHover(TEXT("Pointer entry highlights the translation component"), true);
	HitTarget->UpdateInteractingState(true);
	TestLatestHover(TEXT("Beginning interaction keeps the component highlighted"), true);
	HitTarget->UpdateHoverState(false);
	TestLatestHover(TEXT("Hover exit during a drag keeps the active component highlighted"), true);
	HitTarget->UpdateInteractingState(false);
	TestLatestHover(TEXT("Release immediately restores the component's base color"), false);
	HitTarget->UpdateHoverState(true);
	TestLatestHover(TEXT("Hover remains suppressed while the released pointer stays on the handle"), false);
	HitTarget->UpdateHoverState(false);
	TestLatestHover(TEXT("Leaving the released handle keeps its base color"), false);
	HitTarget->UpdateHoverState(true);
	TestLatestHover(TEXT("Reentering after exit highlights the component again"), true);
	TestEqual(TEXT("Each installed callback forwards one hover update"), HoverUpdates.Num(), 7);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorRuntimePointerTest,
	"VTBOWTEditor.ToolsContext.RuntimePointerDragAndDeferredShutdown",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorRuntimePointerTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	if (!TestTrue(TEXT("Slate is available for application focus events"), FSlateApplication::IsInitialized()))
	{
		return false;
	}
	FSlateApplication& SlateApplication = FSlateApplication::Get();
	const bool bWasApplicationActive = SlateApplication.IsActive();
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Game world exists"), TestWorld.World))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	ConfigureSelectionManager(Context);
	Context->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		Context->RemoveFromRoot();
		SlateApplication.ProcessApplicationActivationEvent(bWasApplicationActive);
	};
	if (!TestTrue(TEXT("Runtime initialization succeeds"), Context->InitializeContext(TestWorld.World)))
	{
		return false;
	}
	AActor* Actor = SpawnMovableActor(TestWorld.World, FVector::ZeroVector);
	if (!TestNotNull(TEXT("Movable actor exists"), Actor))
	{
		return false;
	}
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Actor);
	if (!TestTrue(TEXT("Selection initializes runtime gizmos"),
		SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(Actors))))
	{
		return false;
	}
	UVTBOWTEditorRepositionalGizmo* Gizmo = FindRuntimeGizmo(Context);
	if (!TestNotNull(TEXT("Runtime gizmo exists"), Gizmo) || !TestNotNull(TEXT("Gizmo actor exists"), Gizmo->GetGizmoActor()))
	{
		return false;
	}
	UAxisPositionGizmo* TranslateX = FindTranslateX(Context, Gizmo);
	if (!TestNotNull(TEXT("Translation X sub-gizmo exists"), TranslateX))
	{
		return false;
	}
	UGizmoLambdaHitTarget* HitTarget = NewObject<UGizmoLambdaHitTarget>(TranslateX);
	HitTarget->IsHitFunction = [](const FInputDeviceRay&)
	{
		return FInputRayHit(1.0);
	};
	TranslateX->HitTarget = HitTarget;
	FInputDeviceState AltPress = MakePointer(50, true, true, false);
	AltPress.bAltKeyDown = true;
	Context->GetInput().PostPointerInput(AltPress, false);
	TestFalse(TEXT("Alt-modified pointer press does not capture the translation gizmo"), Context->GetInput().HasActiveMouseCapture());
	FInputDeviceState AltDrag = MakePointer(90, false, true, false);
	AltDrag.bAltKeyDown = true;
	Context->GetInput().PostPointerInput(AltDrag, false);
	FInputDeviceState AltRelease = MakePointer(90, false, false, true);
	AltRelease.bAltKeyDown = true;
	Context->GetInput().PostPointerInput(AltRelease, false);
	TestFalse(TEXT("Alt-modified pointer release leaves no capture"), Context->GetInput().HasActiveMouseCapture());
	TestTrue(TEXT("Alt-modified dragging leaves the selected actor unchanged"), Actor->GetActorLocation().IsNearlyZero(0.01));
	TestFalse(TEXT("Alt-modified dragging creates no undo history"), Context->GetUndoRedo().CanUndo());
	TestFalse(TEXT("Alt-modified dragging creates no redo history"), Context->GetUndoRedo().CanRedo());

	Context->GetInput().PostPointerInput(MakePointer(50, true, true, false), false);
	if (!TestTrue(TEXT("Pointer press captures the translation gizmo"), Context->GetInput().HasActiveMouseCapture()))
	{
		return false;
	}
	Context->GetInput().PostPointerInput(MakePointer(90, false, true, false), false);
	TestTrue(TEXT("Pointer drag translates the selected actor on X"), Actor->GetActorLocation().Equals(FVector(40, 0, 0), 0.01));
	Context->GetInput().PostPointerInput(MakePointer(90, false, false, true), false);
	TestFalse(TEXT("Pointer release ends capture"), Context->GetInput().HasActiveMouseCapture());
	TestTrue(TEXT("Pointer drag records an undoable change"), Context->GetUndoRedo().Undo());
	TestTrue(TEXT("Pointer drag undo restores the actor"), Actor->GetActorLocation().IsNearlyZero(0.01));
	TestTrue(TEXT("Pointer drag redo succeeds"), Context->GetUndoRedo().Redo());
	TestTrue(TEXT("Pointer drag redo restores translation"), Actor->GetActorLocation().Equals(FVector(40, 0, 0), 0.01));
	Context->GetUndoRedo().Undo();

	UTransformProxy* Proxy = Gizmo->GetTransformProxy();
	const auto VerifyFocusCancellation = [this, Context, Actor, &SlateApplication](UTransformProxy* FocusProxy, bool bExpectedRedo)
	{
		int32 EndCount = 0;
		const FDelegateHandle EndHandle = FocusProxy->OnEndTransformEdit.AddLambda(
			[&EndCount](UTransformProxy*)
			{
				++EndCount;
			});
		ON_SCOPE_EXIT
		{
			FocusProxy->OnEndTransformEdit.Remove(EndHandle);
		};
		SlateApplication.ProcessApplicationActivationEvent(true);
		Context->GetInput().PostPointerInput(MakePointer(50, true, true, false), false);
		Context->GetInput().PostPointerInput(MakePointer(90, false, true, false), false);
		TestTrue(TEXT("A drag is in progress before application focus is lost"),
			Context->GetInput().HasActiveMouseCapture() && Actor->GetActorLocation().Equals(FVector(40, 0, 0), 0.01));
		SlateApplication.ProcessApplicationActivationEvent(false);
		TestEqual(TEXT("Application deactivation ends the active drag once"), EndCount, 1);
		TestFalse(TEXT("Application deactivation releases pointer capture"), Context->GetInput().HasActiveMouseCapture());
		TestTrue(TEXT("Application deactivation rolls back the unfinished transform"),
			Actor->GetActorLocation().IsNearlyZero(0.01));
		TestFalse(TEXT("Focus cancellation does not create an undo entry"), Context->GetUndoRedo().CanUndo());
		TestEqual(TEXT("Focus cancellation preserves the existing redo branch"), Context->GetUndoRedo().CanRedo(), bExpectedRedo);
		TestTrue(TEXT("Focus cancellation keeps the context ready"), Context->IsRuntimeReady());
		SlateApplication.ProcessApplicationActivationEvent(true);
		TestEqual(TEXT("Regaining focus does not end the drag again"), EndCount, 1);
	};
	VerifyFocusCancellation(Proxy, true);

	bool bShutdownRequestedInCallback = false;
	const FDelegateHandle ShutdownHandle = Proxy->OnTransformChanged.AddLambda(
		[Context, &bShutdownRequestedInCallback](UTransformProxy*, FTransform)
		{
			if (!bShutdownRequestedInCallback)
			{
				bShutdownRequestedInCallback = true;
				Context->Shutdown();
			}
		});
	Context->GetInput().PostPointerInput(MakePointer(50, true, true, false), false);
	Context->GetInput().PostPointerInput(MakePointer(90, false, true, false), false);
	Proxy->OnTransformChanged.Remove(ShutdownHandle);
	TestTrue(TEXT("Transform callback requested shutdown"), bShutdownRequestedInCallback);
	TestFalse(TEXT("Deferred shutdown completes after input dispatch"), Context->IsRuntimeReady());
	TestNull(TEXT("Deferred shutdown releases managers"), Context->ToolManager.Get());
	TestNull(TEXT("Deferred shutdown releases owned adapters after manager callbacks"), Context->GetContextQueriesAPI());
	TestTrue(TEXT("Shutdown cancels the uncommitted drag"), Actor->GetActorLocation().IsNearlyZero(0.01));
	TestFalse(TEXT("Shutdown removes the context's application focus subscription"),
		SlateApplication.OnApplicationActivationStateChanged().IsBoundToObject(Context));
	if (!TestTrue(TEXT("The same context can initialize after deferred shutdown"), Context->InitializeContext(TestWorld.World))
		|| !TestTrue(TEXT("Repeated initialization preserves the new session"), Context->InitializeContext(TestWorld.World))
		|| !TestTrue(TEXT("The reinitialized context can restore selection"),
			SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(Actors))))
	{
		return false;
	}
	Gizmo = FindRuntimeGizmo(Context);
	if (!TestNotNull(TEXT("Reinitialization creates a new runtime gizmo"), Gizmo)
		|| !TestNotNull(TEXT("The reinitialized gizmo actor exists"), Gizmo->GetGizmoActor()))
	{
		return false;
	}
	TranslateX = FindTranslateX(Context, Gizmo);
	if (!TestNotNull(TEXT("Reinitialization restores the translation handle"), TranslateX)
		|| !TestNotNull(TEXT("Reinitialization restores the selection proxy"), Gizmo->GetTransformProxy()))
	{
		return false;
	}
	HitTarget = NewObject<UGizmoLambdaHitTarget>(TranslateX);
	HitTarget->IsHitFunction = [](const FInputDeviceRay&)
	{
		return FInputRayHit(1.0);
	};
	TranslateX->HitTarget = HitTarget;
	VerifyFocusCancellation(Gizmo->GetTransformProxy(), false);
	Context->Shutdown();
	TestFalse(TEXT("The reinitialized session also removes its focus subscription at shutdown"),
		SlateApplication.OnApplicationActivationStateChanged().IsBoundToObject(Context));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorRuntimeReentrantCancellationTest,
	"VTBOWTEditor.ToolsContext.RuntimeReentrantCancellation",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorRuntimeReentrantCancellationTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Game world exists"), TestWorld.World))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	ConfigureSelectionManager(Context);
	Context->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		Context->RemoveFromRoot();
	};
	if (!TestTrue(TEXT("Runtime initialization succeeds"), Context->InitializeContext(TestWorld.World)))
	{
		return false;
	}
	AActor* Actor = SpawnMovableActor(TestWorld.World, FVector::ZeroVector);
	if (!TestNotNull(TEXT("Movable actor exists"), Actor))
	{
		return false;
	}
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Actor);
	if (!TestTrue(TEXT("Selection initializes runtime gizmos"),
		SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(Actors))))
	{
		return false;
	}
	UVTBOWTEditorRepositionalGizmo* Gizmo = FindRuntimeGizmo(Context);
	if (!TestNotNull(TEXT("Runtime gizmo exists"), Gizmo)
		|| !TestNotNull(TEXT("Gizmo actor exists"), Gizmo->GetGizmoActor()))
	{
		return false;
	}
	UAxisPositionGizmo* TranslateX = FindTranslateX(Context, Gizmo);
	UTransformProxy* Proxy = Gizmo->GetTransformProxy();
	if (!TestNotNull(TEXT("Translation X sub-gizmo exists"), TranslateX)
		|| !TestNotNull(TEXT("Selection proxy exists"), Proxy))
	{
		return false;
	}
	UGizmoLambdaHitTarget* HitTarget = NewObject<UGizmoLambdaHitTarget>(TranslateX);
	HitTarget->IsHitFunction = [](const FInputDeviceRay&)
	{
		return FInputRayHit(1.0);
	};
	TranslateX->HitTarget = HitTarget;

	int32 BeginCount = 0;
	int32 EndCount = 0;
	const FDelegateHandle BeginHandle = Proxy->OnBeginTransformEdit.AddLambda(
		[Context, &BeginCount](UTransformProxy*)
		{
			++BeginCount;
			Context->GetInput().CancelActiveInteraction();
		});
	const FDelegateHandle CountEndHandle = Proxy->OnEndTransformEdit.AddLambda(
		[&EndCount](UTransformProxy*)
		{
			++EndCount;
		});
	Context->GetInput().PostPointerInput(MakePointer(50, true, true, false), false);
	Proxy->OnBeginTransformEdit.Remove(BeginHandle);
	Proxy->OnEndTransformEdit.Remove(CountEndHandle);
	TestEqual(TEXT("The begin-edit callback requested cancellation"), BeginCount, 1);
	TestEqual(TEXT("Deferred cancellation closes the new capture exactly once"), EndCount, 1);
	TestFalse(TEXT("Begin-edit cancellation leaves no mouse capture"), Context->GetInput().HasActiveMouseCapture());
	TestTrue(TEXT("Begin-edit cancellation leaves the actor unchanged"), Actor->GetActorLocation().IsNearlyZero(0.01));
	TestFalse(TEXT("Begin-edit cancellation leaves no undo entry"), Context->GetUndoRedo().CanUndo());
	TestFalse(TEXT("Begin-edit cancellation leaves no redo entry"), Context->GetUndoRedo().CanRedo());

	Context->GetInput().PostPointerInput(MakePointer(50, true, true, false), false);
	Context->GetInput().PostPointerInput(MakePointer(90, false, true, false), false);
	TestTrue(TEXT("A new drag works after begin-edit cancellation"),
		Context->GetInput().HasActiveMouseCapture() && Actor->GetActorLocation().Equals(FVector(40, 0, 0), 0.01));
	EndCount = 0;
	const FDelegateHandle CancelEndHandle = Proxy->OnEndTransformEdit.AddLambda(
		[Context, &EndCount](UTransformProxy*)
		{
			++EndCount;
			Context->GetInput().CancelActiveInteraction();
		});
	Context->GetInput().PostPointerInput(MakePointer(90, false, false, true), false);
	Proxy->OnEndTransformEdit.Remove(CancelEndHandle);
	TestEqual(TEXT("End-edit cancellation does not reenter the change source"), EndCount, 1);
	TestFalse(TEXT("End-edit cancellation leaves no mouse capture"), Context->GetInput().HasActiveMouseCapture());
	TestTrue(TEXT("End-edit cancellation rolls back the drag"), Actor->GetActorLocation().IsNearlyZero(0.01));
	TestFalse(TEXT("End-edit cancellation does not commit history"), Context->GetUndoRedo().CanUndo());
	TestFalse(TEXT("End-edit cancellation creates no redo entry"), Context->GetUndoRedo().CanRedo());
	TestTrue(TEXT("Cancellation keeps the runtime session ready"), Context->IsRuntimeReady());

	Context->GetInput().PostPointerInput(MakePointer(50, true, true, false), false);
	Context->GetInput().PostPointerInput(MakePointer(90, false, true, false), false);
	Context->GetInput().PostPointerInput(MakePointer(90, false, false, true), false);
	TestTrue(TEXT("A subsequent completed drag remains undoable"), Context->GetUndoRedo().Undo());
	TestTrue(TEXT("Subsequent undo restores the actor"), Actor->GetActorLocation().IsNearlyZero(0.01));
	TestTrue(TEXT("A subsequent completed drag remains redoable"), Context->GetUndoRedo().Redo());
	TestTrue(TEXT("Subsequent redo restores the translation"),
		Actor->GetActorLocation().Equals(FVector(40, 0, 0), 0.01));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorSelectionDuringCancellationTest,
	"VTBOWTEditor.ToolsContext.SelectionDuringCancellationAndShutdown",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorSelectionDuringCancellationTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Game world exists"), TestWorld.World))
	{
		return false;
	}
	UVTBOWTEditorModeSubsystem* Runtime = TestWorld.World->GetSubsystem<UVTBOWTEditorModeSubsystem>();
	if (!TestNotNull(TEXT("World owns the runtime subsystem"), Runtime))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = Runtime->GetToolsContext();
	if (!TestTrue(TEXT("Subsystem context is ready"), Context && Context->IsRuntimeReady()))
	{
		return false;
	}
	AActor* First = SpawnMovableActor(TestWorld.World, FVector::ZeroVector);
	AActor* Second = SpawnMovableActor(TestWorld.World, FVector(100, 0, 0));
	AActor* Latest = SpawnMovableActor(TestWorld.World, FVector(200, 0, 0));
	if (!TestNotNull(TEXT("First actor exists"), First)
		|| !TestNotNull(TEXT("Second actor exists"), Second)
		|| !TestNotNull(TEXT("Latest actor exists"), Latest))
	{
		return false;
	}
	const auto BeginDrag = [&]() -> UTransformProxy*
	{
		Runtime->ReceiveSelection({First});
		UVTBOWTEditorRepositionalGizmo* Gizmo = FindRuntimeGizmo(Context);
		if (!Gizmo || !Gizmo->GetGizmoActor())
		{
			AddError(TEXT("Selection must create the transform gizmo"));
			return nullptr;
		}
		UAxisPositionGizmo* TranslateX = FindTranslateX(Context, Gizmo);
		if (!TranslateX)
		{
			AddError(TEXT("Selection must create the translation handle"));
			return nullptr;
		}
		UGizmoLambdaHitTarget* HitTarget = NewObject<UGizmoLambdaHitTarget>(TranslateX);
		HitTarget->IsHitFunction = [](const FInputDeviceRay&)
		{
			return FInputRayHit(1.0);
		};
		TranslateX->HitTarget = HitTarget;
		Context->GetInput().PostPointerInput(MakePointer(50, true, true, false), false);
		Context->GetInput().PostPointerInput(MakePointer(90, false, true, false), false);
		const bool bCaptured = TestTrue(TEXT("The selected translation handle captures pointer input"), Context->GetInput().HasActiveMouseCapture());
		const bool bTranslated = TestTrue(FString::Printf(TEXT("The active drag translates the actor to X=40 (actual %s)"),
			*First->GetActorLocation().ToString()), First->GetActorLocation().Equals(FVector(40, 0, 0), 0.01));
		if (!bCaptured || !bTranslated)
		{
			return nullptr;
		}
		return Gizmo->GetTransformProxy();
	};
	UTransformProxy* Proxy = BeginDrag();
	if (!TestNotNull(TEXT("First drag has a proxy"), Proxy))
	{
		return false;
	}
	int32 EndCount = 0;
	const FDelegateHandle SelectionHandle = Proxy->OnEndTransformEdit.AddLambda(
		[Runtime, Latest, &EndCount](UTransformProxy*)
		{
			++EndCount;
			Runtime->ReceiveSelection({Latest});
		});
	Runtime->ReceiveSelection({Second});
	Proxy->OnEndTransformEdit.Remove(SelectionHandle);
	Runtime->Tick(0.0f);
	FToolBuilderState Selection;
	Context->GetContextQueriesAPI()->GetCurrentSelectionState(Selection);
	TestEqual(TEXT("Replacing selection ends the drag once"), EndCount, 1);
	TestTrue(TEXT("The reentrant selection takes precedence on the next tick"),
		Selection.SelectedActors == TArray<AActor*>({Latest}));
	TestFalse(TEXT("Selection replacement releases input capture"), Context->GetInput().HasActiveMouseCapture());
	TestTrue(TEXT("Selection replacement rolls back the interrupted drag"), First->GetActorLocation().IsNearlyZero(0.01));
	TestFalse(TEXT("Selection replacement does not commit the interrupted drag"), Context->GetUndoRedo().CanUndo());

	Proxy = BeginDrag();
	if (!TestNotNull(TEXT("A subsequent drag has a proxy"), Proxy))
	{
		return false;
	}
	EndCount = 0;
	const FDelegateHandle ShutdownHandle = Proxy->OnEndTransformEdit.AddLambda(
		[Runtime, Context, Latest, &EndCount](UTransformProxy*)
		{
			++EndCount;
			Runtime->ReceiveSelection({Latest});
			Context->Shutdown();
		});
	Runtime->ReceiveSelection({Second});
	Proxy->OnEndTransformEdit.Remove(ShutdownHandle);
	Runtime->Tick(0.0f);
	TestEqual(TEXT("Shutdown from selection cancellation ends the drag once"), EndCount, 1);
	TestFalse(TEXT("Shutdown during cancellation leaves the context unavailable"), Context->IsRuntimeReady());
	TestNull(TEXT("Queued selection cannot access managers after shutdown"), Runtime->GetGizmoManager());
	TestNull(TEXT("Shutdown releases query adapters despite the pending selection"), Context->GetContextQueriesAPI());
	TestTrue(TEXT("Shutdown during selection replacement rolls back the drag"), First->GetActorLocation().IsNearlyZero(0.01));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorShutdownDuringInitializationTest,
	"VTBOWTEditor.ToolsContext.ShutdownDuringInitialization",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorShutdownDuringInitializationTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Game world exists"), TestWorld.World))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	Context->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		Context->RemoveFromRoot();
	};
	bool bRequestShutdown = true;
	int32 FactoryCount = 0;
	Context->SetCreateGizmoManagerFunc(
		[this, Context, &bRequestShutdown, &FactoryCount](const UInteractiveToolsContext::FContextInitInfo& Info)
		{
			++FactoryCount;
			if (bRequestShutdown)
			{
				bRequestShutdown = false;
				Context->Shutdown();
				Context->Shutdown();
				Context->GetInput().CancelActiveInteraction();
				TestFalse(TEXT("Pending shutdown cannot publish readiness inside a factory"), Context->IsRuntimeReady());
				TestTrue(TEXT("Repeated shutdown preserves the router until initialization finishes"),
					Info.InputRouter != nullptr && Context->InputRouter.Get() == Info.InputRouter);
				TestTrue(TEXT("Repeated shutdown preserves the APIs borrowed by the factory"),
					Context->GetContextQueriesAPI() == Info.QueriesAPI
					&& Context->GetContextTransactionAPI() == Info.TransactionsAPI);
				TestNull(TEXT("Render API is published only after manager initialization"), Context->GetContextRenderAPI());
			}
			UVTBOWTEditorGizmoManager* Manager = NewObject<UVTBOWTEditorGizmoManager>(Info.ToolsContext);
			Manager->Initialize(Info.QueriesAPI, Info.TransactionsAPI, Info.InputRouter);
			Manager->RegisterDefaultGizmos();
			return Manager;
		});

	Context->Initialize();
	TestEqual(TEXT("The interrupted initialization still completes its factory once"), FactoryCount, 1);
	TestFalse(TEXT("Initialization does not overwrite a pending shutdown with readiness"), Context->IsRuntimeReady());
	TestTrue(TEXT("Deferred shutdown releases every initialized manager"),
		Context->InputRouter == nullptr && Context->ToolManager == nullptr && Context->GizmoManager == nullptr
		&& Context->TargetManager == nullptr && Context->ContextObjectStore == nullptr);
	TestNull(TEXT("Deferred shutdown releases the render API"), Context->GetContextRenderAPI());
	TestNull(TEXT("Deferred shutdown releases the queries API"), Context->GetContextQueriesAPI());
	TestNull(TEXT("Deferred shutdown releases the transactions API"), Context->GetContextTransactionAPI());

	Context->Initialize();
	TestEqual(TEXT("The same context can create its managers again"), FactoryCount, 2);
	TestTrue(TEXT("Reinitialization becomes ready after the one-shot shutdown request"), Context->IsRuntimeReady());
	TestNotNull(TEXT("Reinitialization restores the owned queries"), Context->GetContextQueriesAPI());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorComponentFrameTransformTest,
	"VTBOWTEditor.ToolsContext.ComponentFrameActorTransform",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorComponentFrameTransformTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Game world exists"), TestWorld.World))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	ConfigureSelectionManager(Context);
	Context->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		Context->RemoveFromRoot();
	};
	if (!TestTrue(TEXT("Context initializes"), Context->InitializeContext(TestWorld.World)))
	{
		return false;
	}
	Context->GetSceneState().SetCoordinateSystem(EToolContextCoordinateSystem::Local);
	AActor* Actor = SpawnMovableActor(TestWorld.World, FVector(100, -75, 40));
	if (!TestNotNull(TEXT("Hierarchy actor exists"), Actor))
	{
		return false;
	}
	Actor->SetActorTransform(FTransform(FRotator(12, 24, -8), FVector(100, -75, 40), FVector(1.1, 0.8, 1.2)));
	USceneComponent* Root = Actor->GetRootComponent();
	USplineComponent* Spline = AddHierarchyComponent<USplineComponent>(Actor, Root,
		FTransform(FRotator(20, 30, 10), FVector(50, -30, 20), FVector(0.8, 1.3, 0.9)));
	UStaticMeshComponent* StaticMesh = AddHierarchyComponent<UStaticMeshComponent>(Actor, Spline,
		FTransform(FRotator(-15, 35, 5), FVector(60, 25, -10), FVector(0.7, 1.4, 1.1)));
	UDynamicMeshComponent* DynamicMesh = AddHierarchyComponent<UDynamicMeshComponent>(Actor, Spline,
		FTransform(FRotator(10, -25, 20), FVector(-40, 35, 15), FVector(1.2, 0.9, 0.8)));
	UInstancedStaticMeshComponent* Instances = AddHierarchyComponent<UInstancedStaticMeshComponent>(Actor, Spline,
		FTransform(FRotator(5, 40, -12), FVector(35, -65, 20), FVector(0.9, 1.1, 1.3)));
	UHierarchicalInstancedStaticMeshComponent* HierarchicalInstances = AddHierarchyComponent<UHierarchicalInstancedStaticMeshComponent>(
		Actor, Spline, FTransform(FRotator(-10, -20, 15), FVector(-30, -45, 55), FVector(1.4, 0.8, 1.2)));
	USceneComponent* Descendant = AddHierarchyComponent<USceneComponent>(Actor, StaticMesh,
		FTransform(FRotator(15, 10, 5), FVector(20, -5, 12), FVector(1.2, 0.7, 1.1)));
	const TArray<USceneComponent*> Components = {Root, Spline, StaticMesh, DynamicMesh, Instances, HierarchicalInstances, Descendant};
	TArray<FTransform> RelativeTransforms;
	for (USceneComponent* Component : Components)
	{
		RelativeTransforms.Add(Component->GetRelativeTransform());
	}
	const TArray<FTransform> InstanceTransforms = {
		FTransform(FRotator(5, 20, 10), FVector(20, 5, 0), FVector(1.2, 0.8, 1.1)),
		FTransform(FRotator(-10, 45, 5), FVector(-10, 30, 5), FVector(0.7, 1.3, 0.9))};
	for (UInstancedStaticMeshComponent* Component : {Instances, static_cast<UInstancedStaticMeshComponent*>(HierarchicalInstances)})
	{
		for (const FTransform& Transform : InstanceTransforms)
		{
			Component->AddInstance(Transform);
		}
	}
	const TArray<USceneComponent*> Frames = {StaticMesh, DynamicMesh, Instances, HierarchicalInstances};
	for (USceneComponent* Frame : Frames)
	{
		const FString FrameName = Frame->GetClass()->GetName();
		const TArray<TWeakObjectPtr<AActor>> Actors = {Actor};
		if (!TestTrue(FrameName + TEXT(" selects its component frame"),
			SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(Actors), Frame)))
		{
			return false;
		}
		UVTBOWTEditorRepositionalGizmo* Gizmo = FindRuntimeGizmo(Context);
		if (!TestTrue(TEXT("Component selection creates a targeted gizmo"), Gizmo && Gizmo->GetTransformProxy()))
		{
			return false;
		}
		TestTrue(FrameName + TEXT(" supplies gizmo position, rotation and scale"),
			Gizmo->GetTransformProxy()->GetTransform().Equals(Frame->GetComponentTransform(), 0.001));
		for (int32 Step = 0; Step < 5; ++Step)
		{
			const FString Label = FString::Printf(TEXT("%s step %d: "), *FrameName, Step);
			const TArray<FTransform> Before = CaptureTransforms(Components);
			FTransform Desired = Frame->GetComponentTransform();
			if (Step == 0)
			{
				Desired.AddToTranslation(FVector(35, -20, 15));
			}
			else if (Step == 1)
			{
				Desired.SetRotation(FQuat(FVector::UpVector, FMath::DegreesToRadians(65.0)) * Desired.GetRotation());
			}
			else if (Step == 2)
			{
				Desired.SetScale3D(Desired.GetScale3D() * FVector(1.5, 0.7, 1.2));
			}
			else if (Step == 3)
			{
				Desired.AddToTranslation(FVector(-10, 25, 5));
				Desired.SetRotation(FQuat(FRotator(10, 15, 20)) * Desired.GetRotation());
				Desired.SetScale3D(Desired.GetScale3D() * FVector(0.9, 1.3, 0.8));
			}
			else
			{
				Desired.SetScale3D(FVector(0, 1.5, 0.75));
			}
			Gizmo->SetNewGizmoTransform(Desired);
			TestTrue(Label + TEXT("the selected child reaches the requested world TRS"),
				Frame->GetComponentTransform().Equals(Desired, 0.001));
			TestTrue(Label + TEXT("the proxy matches the actual child transform"),
				Gizmo->GetTransformProxy()->GetTransform().Equals(Frame->GetComponentTransform(), 0.001));
			TestFalse(Label + TEXT("the actor root changes with the child frame"), Root->GetComponentTransform().Equals(Before[0], 0.001));
			for (int32 Index = 1; Index < Components.Num(); ++Index)
			{
				TestTrue(Label + Components[Index]->GetName() + TEXT(" keeps its relative transform"),
					Components[Index]->GetRelativeTransform().Equals(RelativeTransforms[Index], 0.001));
			}
			for (UInstancedStaticMeshComponent* Component : {Instances, static_cast<UInstancedStaticMeshComponent*>(HierarchicalInstances)})
			{
				TestEqual(Label + TEXT("instance count is preserved"), Component->GetInstanceCount(), InstanceTransforms.Num());
				for (int32 Index = 0; Index < InstanceTransforms.Num(); ++Index)
				{
					FTransform Local;
					FTransform World;
					TestTrue(Label + TEXT("instance local transform remains available"), Component->GetInstanceTransform(Index, Local));
					TestTrue(Label + TEXT("individual instance local transforms are preserved"), Local.Equals(InstanceTransforms[Index], 0.001));
					TestTrue(Label + TEXT("instance world transform remains available"), Component->GetInstanceTransform(Index, World, true));
					TestTrue(Label + TEXT("instances follow their owning component"),
						World.Equals(InstanceTransforms[Index] * Component->GetComponentTransform(), 0.001));
				}
			}
			TestFalse(Label + TEXT("gizmo edits keep the selection current"), Gizmo->IsSelectionStateStale());
			const TArray<FTransform> After = CaptureTransforms(Components);
			TestTrue(Label + TEXT("undo succeeds"), Context->GetUndoRedo().Undo());
			for (int32 Index = 0; Index < Components.Num(); ++Index)
			{
				TestTrue(Label + Components[Index]->GetName() + TEXT(" is restored by undo"),
					Components[Index]->GetComponentTransform().Equals(Before[Index], 0.001));
			}
			TestTrue(Label + TEXT("undo keeps the proxy aligned with its frame"),
				Gizmo->GetTransformProxy()->GetTransform().Equals(Frame->GetComponentTransform(), 0.001));
			TestTrue(Label + TEXT("redo succeeds"), Context->GetUndoRedo().Redo());
			for (int32 Index = 0; Index < Components.Num(); ++Index)
			{
				TestTrue(Label + Components[Index]->GetName() + TEXT(" is restored by redo"),
					Components[Index]->GetComponentTransform().Equals(After[Index], 0.001));
			}
			if (Step == 4)
			{
				TestTrue(Label + TEXT("zero scale can be undone before changing frames"), Context->GetUndoRedo().Undo());
			}
		}
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorComponentFrameRefreshTest,
	"VTBOWTEditor.ToolsContext.ComponentFrameRefreshAndHistory",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorComponentFrameRefreshTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Game world exists"), TestWorld.World))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	ConfigureSelectionManager(Context);
	Context->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		Context->RemoveFromRoot();
	};
	if (!TestTrue(TEXT("Context initializes"), Context->InitializeContext(TestWorld.World)))
	{
		return false;
	}
	AActor* Actor = SpawnMovableActor(TestWorld.World, FVector(50, 10, 25));
	if (!TestNotNull(TEXT("The frame owner exists"), Actor))
	{
		return false;
	}
	USceneComponent* First = AddHierarchyComponent<UStaticMeshComponent>(Actor, Actor->GetRootComponent(),
		FTransform(FRotator(10, 30, 5), FVector(60, 25, 10)));
	USceneComponent* Second = AddHierarchyComponent<UDynamicMeshComponent>(Actor, Actor->GetRootComponent(),
		FTransform(FRotator(-15, 50, 20), FVector(-35, 40, 25)));
	const TArray<TWeakObjectPtr<AActor>> Actors = {Actor};
	TestTrue(TEXT("The first child selects its owner"), SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(Actors), First));
	UVTBOWTEditorRepositionalGizmo* Gizmo = FindRuntimeGizmo(Context);
	if (!TestTrue(TEXT("Selection creates a proxy"), Gizmo && Gizmo->GetTransformProxy()))
	{
		return false;
	}
	UTransformProxy* FirstProxy = Gizmo->GetTransformProxy();
	TestTrue(TEXT("The same actor accepts a different component frame"),
		SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(Actors), Second));
	TestTrue(TEXT("Changing only the frame rebuilds the proxy and repositions the gizmo"),
		Gizmo->GetTransformProxy() != FirstProxy && Gizmo->GetSelectionFrame() == Second
		&& Gizmo->GetTransformProxy()->GetTransform().Equals(Second->GetComponentTransform()));
	Second->SetRelativeTransform(FTransform(FRotator(25, -10, 35), FVector(25, -20, 45), FVector(1.2, 0.8, 1.3)));
	TestTrue(TEXT("An external component transform marks the frame stale"), Gizmo->IsSelectionStateStale());
	TestTrue(TEXT("Idle refresh observes the external component transform"), SynchronizeSelection(Context, {}));
	TestTrue(TEXT("The refreshed gizmo matches the component without moving the actor"),
		Gizmo->GetTransformProxy()->GetTransform().Equals(Second->GetComponentTransform())
		&& Actor->GetActorLocation().Equals(FVector(50, 10, 25)));
	const FTransform BeforeRoot = Actor->GetActorTransform();
	const FTransform BeforeFrame = Second->GetComponentTransform();
	FTransform Changed = BeforeFrame;
	Changed.AddToTranslation(FVector(30, 15, -10));
	Changed.SetRotation(FQuat(FRotator(15, 20, -10)) * Changed.GetRotation());
	Gizmo->SetNewGizmoTransform(Changed);
	const FTransform AfterRoot = Actor->GetActorTransform();
	const TWeakObjectPtr<UTransformProxy> HistoryProxy = Gizmo->GetTransformProxy();
	const TArray<TWeakObjectPtr<AActor>> NoActors;
	TestTrue(TEXT("Component selection can be cleared"), SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(NoActors)));
	CollectGarbage(RF_NoFlags);
	TestTrue(TEXT("History retains the component-frame proxy through GC"), HistoryProxy.IsValid());
	TestTrue(TEXT("An unselected component-frame edit can be undone"), Context->GetUndoRedo().Undo());
	TestTrue(TEXT("Undo restores both the actor root and component frame"),
		Actor->GetActorTransform().Equals(BeforeRoot, 0.001) && Second->GetComponentTransform().Equals(BeforeFrame, 0.001));
	TestTrue(TEXT("An unselected component-frame edit can be redone"), Context->GetUndoRedo().Redo());
	TestTrue(TEXT("Redo restores the actor and selected component"),
		Actor->GetActorTransform().Equals(AfterRoot, 0.001) && Second->GetComponentTransform().Equals(Changed, 0.001));
	TestTrue(TEXT("The component can be selected again"), SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(Actors), Second));
	const FTransform ExternalRelative(FRotator(-20, 65, 15), FVector(-40, 55, 30), FVector(0.7, 1.4, 0.9));
	Second->SetRelativeTransform(ExternalRelative);
	TestTrue(TEXT("A later external frame change refreshes the active proxy"), SynchronizeSelection(Context, {}));
	TestTrue(TEXT("An old edit remains undoable after the frame's relative transform changes"), Context->GetUndoRedo().Undo());
	TestTrue(TEXT("Old history restores the recorded actor root without overwriting the external component edit"),
		Actor->GetActorTransform().Equals(BeforeRoot, 0.001) && Second->GetRelativeTransform().Equals(ExternalRelative, 0.001));
	TestTrue(TEXT("The current selection refreshes after an older proxy replays undo"), SynchronizeSelection(Context, {}));
	TestTrue(TEXT("The refreshed proxy follows the externally edited component after undo"),
		Gizmo->GetTransformProxy()->GetTransform().Equals(Second->GetComponentTransform(), 0.001));
	TestTrue(TEXT("An old edit remains redoable after the frame's relative transform changes"), Context->GetUndoRedo().Redo());
	TestTrue(TEXT("Old redo restores the recorded actor root and preserves the external component edit"),
		Actor->GetActorTransform().Equals(AfterRoot, 0.001) && Second->GetRelativeTransform().Equals(ExternalRelative, 0.001));
	TestTrue(TEXT("The current selection refreshes after an older proxy replays redo"), SynchronizeSelection(Context, {}));
	Second->DestroyComponent();
	TestTrue(TEXT("Destroying the frame requires a refresh"), Gizmo->IsSelectionStateStale());
	TestTrue(TEXT("A destroyed frame refreshes safely"), SynchronizeSelection(Context, {}));
	TArray<AActor*> SelectedActors;
	Gizmo->GetSelectedActors(SelectedActors);
	TestTrue(TEXT("Destroying a frame preserves its actor selection"), SelectedActors == TArray<AActor*>({Actor}));
	TestNull(TEXT("A destroyed frame falls back to actor selection"), Gizmo->GetSelectionFrame());
	if (!TestNotNull(TEXT("The fallback actor retains a transform proxy"), Gizmo->GetTransformProxy()))
	{
		return false;
	}
	TestTrue(TEXT("The fallback proxy uses the actor root"), Gizmo->GetTransformProxy()->GetTransform().Equals(Actor->GetActorTransform(), 0.001));
	TestTrue(TEXT("An old edit remains undoable after its original frame is destroyed"), Context->GetUndoRedo().Undo());
	TestTrue(TEXT("Undo through a destroyed frame binding restores the surviving actor root"),
		Actor->GetActorTransform().Equals(BeforeRoot, 0.001));
	TestTrue(TEXT("The fallback selection refreshes after the old edit is undone"), SynchronizeSelection(Context, {}));
	TestTrue(TEXT("An old edit remains redoable after its original frame is destroyed"), Context->GetUndoRedo().Redo());
	TestTrue(TEXT("Redo through a destroyed frame binding restores the surviving actor root"),
		Actor->GetActorTransform().Equals(AfterRoot, 0.001));
	TestTrue(TEXT("The fallback selection refreshes after the old edit is redone"), SynchronizeSelection(Context, {}));
	Changed = Gizmo->GetTransformProxy()->GetTransform();
	Changed.AddToTranslation(FVector(15, 0, 0));
	Gizmo->SetNewGizmoTransform(Changed);
	TestTrue(TEXT("The surviving actor remains editable"), Actor->GetActorTransform().Equals(Changed, 0.001));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBOWTEditorComponentFrameAttachedActorsTest,
	"VTBOWTEditor.ToolsContext.ComponentFrameAttachedActors",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBOWTEditorComponentFrameAttachedActorsTest::RunTest(const FString& Parameters)
{
	using namespace VTBOWTEditorToolsContextTests;
	FScopedTestWorld TestWorld;
	if (!TestNotNull(TEXT("Game world exists"), TestWorld.World))
	{
		return false;
	}
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(TestWorld.World);
	ConfigureSelectionManager(Context);
	Context->AddToRoot();
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		Context->RemoveFromRoot();
	};
	if (!TestTrue(TEXT("Context initializes"), Context->InitializeContext(TestWorld.World)))
	{
		return false;
	}
	AActor* Parent = SpawnMovableActor(TestWorld.World, FVector(25, -10, 20));
	AActor* Child = SpawnMovableActor(TestWorld.World, FVector(65, 30, 35));
	AActor* Other = SpawnMovableActor(TestWorld.World, FVector(-80, 100, 45));
	if (!TestNotNull(TEXT("Parent actor exists"), Parent)
		|| !TestNotNull(TEXT("Child actor exists"), Child)
		|| !TestNotNull(TEXT("Independent group actor exists"), Other))
	{
		return false;
	}
	if (!TestTrue(TEXT("The child attaches to its selected parent"), Child->AttachToActor(Parent, FAttachmentTransformRules::KeepWorldTransform)))
	{
		return false;
	}
	USplineComponent* Spline = AddHierarchyComponent<USplineComponent>(Child, Child->GetRootComponent(),
		FTransform(FRotator(15, 30, 10), FVector(10, -15, 20)));
	UStaticMeshComponent* Frame = AddHierarchyComponent<UStaticMeshComponent>(Child, Spline,
		FTransform(FRotator(10, -25, 20), FVector(35, 20, 15)));
	const TArray<USceneComponent*> Components = {Parent->GetRootComponent(), Child->GetRootComponent(), Spline, Frame, Other->GetRootComponent()};
	const TArray<FTransform> Before = CaptureTransforms(Components);
	const FTransform ChildRelative = Child->GetRootComponent()->GetRelativeTransform();
	const TArray<TWeakObjectPtr<AActor>> Actors = {Child, Parent, Child, Other};
	TestTrue(TEXT("A selected child can supply the group frame"),
		SynchronizeSelection(Context, TOptional<TArray<TWeakObjectPtr<AActor>>>(Actors), Frame));
	UVTBOWTEditorRepositionalGizmo* Gizmo = FindRuntimeGizmo(Context);
	if (!TestTrue(TEXT("An attached selection creates a proxy"), Gizmo && Gizmo->GetTransformProxy()))
	{
		return false;
	}
	const FVector Delta(30, -15, 25);
	FTransform Desired = Frame->GetComponentTransform();
	Desired.AddToTranslation(Delta);
	Gizmo->SetNewGizmoTransform(Desired);
	for (int32 Index = 0; Index < Components.Num(); ++Index)
	{
		TestTrue(Components[Index]->GetName() + TEXT(" follows the group translation exactly once"),
			Components[Index]->GetComponentLocation().Equals(Before[Index].GetLocation() + Delta, 0.001));
	}
	TestTrue(TEXT("The attached actor keeps its parent-relative transform"), Child->GetRootComponent()->GetRelativeTransform().Equals(ChildRelative));
	TestTrue(TEXT("The group's frame remains the selected descendant"), Gizmo->GetTransformProxy()->GetTransform().Equals(Frame->GetComponentTransform(), 0.001));
	const FVector OtherPositionInFrame = Desired.InverseTransformPosition(Other->GetActorLocation());
	Desired.SetRotation(FQuat(FVector::UpVector, FMath::DegreesToRadians(75.0)) * Desired.GetRotation());
	Gizmo->SetNewGizmoTransform(Desired);
	TestTrue(TEXT("Rotation around a selected descendant reaches its requested frame"), Frame->GetComponentTransform().Equals(Desired, 0.001));
	TestTrue(TEXT("Group rotation does not edit the attached actor's relative transform"), Child->GetRootComponent()->GetRelativeTransform().Equals(ChildRelative));
	TestTrue(TEXT("The independent actor rotates around the same component frame"),
		Other->GetActorLocation().Equals(Desired.TransformPosition(OtherPositionInFrame), 0.001));
	TestTrue(TEXT("Attached group rotation can be undone"), Context->GetUndoRedo().Undo());
	TestTrue(TEXT("Attached group translation can be undone"), Context->GetUndoRedo().Undo());
	for (int32 Index = 0; Index < Components.Num(); ++Index)
	{
		TestTrue(Components[Index]->GetName() + TEXT(" returns to its original transform"),
			Components[Index]->GetComponentTransform().Equals(Before[Index], 0.001));
	}
	return true;
}

#endif
