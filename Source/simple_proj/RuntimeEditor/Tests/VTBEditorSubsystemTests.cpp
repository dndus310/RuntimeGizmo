#if WITH_DEV_AUTOMATION_TESTS

#include "RuntimeEditor/VTBEditorSubsystem.h"
#include "RuntimeEditor/Context/VTBEditorInteractiveToolsContext.h"
#include "RuntimeEditor/Gizmo/VTBEditorTransformGizmo.h"
#include "VTBEditorGameMode.h"

#include "BaseGizmos/AxisPositionGizmo.h"
#include "BaseGizmos/HitTargets.h"
#include "BaseGizmos/TransformProxy.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
#include "InputRouter.h"
#include "InteractiveGizmoManager.h"
#include "Misc/AutomationTest.h"
#include "ToolContextInterfaces.h"
#include "UObject/StrongObjectPtr.h"
#include "UObject/UObjectHash.h"

namespace VTBEditorSubsystemTests
{
	struct FRuntimeWorld
	{
		UWorld* World;
		UVTBEditorSubsystem* Subsystem;

		FRuntimeWorld()
			: World(UWorld::CreateWorld(EWorldType::Game, false))
			, Subsystem(World ? World->GetSubsystem<UVTBEditorSubsystem>() : nullptr)
		{
		}

		~FRuntimeWorld()
		{
			if (World)
			{
				World->DestroyWorld(false);
			}
		}

		AActor* SpawnTarget(const FVector& Location) const
		{
			AActor* Actor = World ? World->SpawnActor<AActor>() : nullptr;
			if (Actor)
			{
				USceneComponent* Root = NewObject<USceneComponent>(Actor);
				Actor->AddInstanceComponent(Root);
				Actor->SetRootComponent(Root);
				Root->SetMobility(EComponentMobility::Movable);
				Root->SetWorldLocation(Location);
				Root->RegisterComponentWithWorld(World);
			}
			return Actor;
		}
	};

	UVTBEditorInteractiveToolsContext* FindContext(UVTBEditorSubsystem* Subsystem)
	{
		TArray<UObject*> Children;
		GetObjectsWithOuter(Subsystem, Children);
		for (UObject* Child : Children)
		{
			if (UVTBEditorInteractiveToolsContext* Context = Cast<UVTBEditorInteractiveToolsContext>(Child))
			{
				return Context;
			}
		}
		return nullptr;
	}

	UCombinedTransformGizmo* FindGizmo(UVTBEditorSubsystem* Subsystem)
	{
		UVTBEditorInteractiveToolsContext* Context = FindContext(Subsystem);
		UInteractiveGizmoManager* Manager = Context ? Context->GizmoManager.Get() : nullptr;
		return Manager ? Cast<UCombinedTransformGizmo>(Manager->FindGizmoByInstanceIdentifier(TEXT("VTB.RuntimeTransform"))) : nullptr;
	}

}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBSubsystemSelectionSourceTest,
	"VTB.RuntimeGizmo.Subsystem.SelectionSourceSwitchUnsubscribes",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FVTBSubsystemSelectionSourceTest::RunTest(const FString& Parameters)
{
	using namespace VTBEditorSubsystemTests;
	FRuntimeWorld Scene;
	if (!TestNotNull(TEXT("Initialized runtime subsystem"), Scene.Subsystem))
	{
		return false;
	}

	AVTBEditorGameMode* FirstSource = Scene.World->SpawnActor<AVTBEditorGameMode>();
	AVTBEditorGameMode* SecondSource = Scene.World->SpawnActor<AVTBEditorGameMode>();
	AActor* First = Scene.SpawnTarget(FVector(10, 0, 0));
	AActor* Second = Scene.SpawnTarget(FVector(90, 0, 0));
	if (!TestNotNull(TEXT("First source"), FirstSource) || !TestNotNull(TEXT("Second source"), SecondSource)
		|| !TestNotNull(TEXT("First target"), First) || !TestNotNull(TEXT("Second target"), Second))
	{
		return false;
	}

	Scene.Subsystem->ReceiveSelection({First});
	UCombinedTransformGizmo* DirectGizmo = FindGizmo(Scene.Subsystem);
	TestNull(TEXT("This world has no authoritative GameMode"), Scene.World->GetAuthGameMode());
	Scene.Subsystem->OnWorldBeginPlay(*Scene.World);
	TestSamePtr(TEXT("BeginPlay without GameMode preserves directly supplied selection"),
		FindGizmo(Scene.Subsystem), DirectGizmo);

	FirstSource->SetSelectedActors({First});
	TestTrue(TEXT("Subsystem binds a GameMode through IVTBSelectionSource"), Scene.Subsystem->BindSelectionSource(FirstSource));
	UCombinedTransformGizmo* Gizmo = FindGizmo(Scene.Subsystem);
	if (!TestNotNull(TEXT("Gizmo"), Gizmo) || !TestNotNull(TEXT("Initial proxy"), Gizmo->ActiveTarget.Get()))
	{
		return false;
	}
	TestTrue(TEXT("Bind pulls the existing selection snapshot"),
		Gizmo->ActiveTarget->GetTransform().Equals(First->GetActorTransform()));

	FirstSource->SetSelectedActors({Second});
	TestTrue(TEXT("Published selection updates the target"),
		Gizmo->ActiveTarget->GetTransform().Equals(Second->GetActorTransform()));

	SecondSource->SetSelectedActors({First});
	TestTrue(TEXT("A different source can replace the subscription"), Scene.Subsystem->BindSelectionSource(SecondSource));
	TStrongObjectPtr<UTransformProxy> BoundProxy(Gizmo->ActiveTarget);
	FirstSource->ClearSelection();
	TestSamePtr(TEXT("Notifications from the old source no longer affect the target"),
		Gizmo->ActiveTarget.Get(), BoundProxy.Get());

	SecondSource->ClearSelection();
	TestNull(TEXT("Clearing selection destroys the gizmo"), FindGizmo(Scene.Subsystem));
	SecondSource->SetSelectedActors({Second});
	if (!TestNotNull(TEXT("A later selection recreates the gizmo"), FindGizmo(Scene.Subsystem)))
	{
		return false;
	}

	Scene.Subsystem->BindSelectionSource(nullptr);
	TestNull(TEXT("Binding null clears the active gizmo"), FindGizmo(Scene.Subsystem));
	SecondSource->SetSelectedActors({First});
	TestNull(TEXT("The unbound source no longer changes selection"), FindGizmo(Scene.Subsystem));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBSubsystemInvalidationRollbackTest,
	"VTB.RuntimeGizmo.Subsystem.InvalidationRefreshesAfterRollback",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FVTBSubsystemInvalidationRollbackTest::RunTest(const FString& Parameters)
{
	using namespace VTBEditorSubsystemTests;
	FRuntimeWorld Scene;
	if (!TestNotNull(TEXT("Initialized runtime subsystem"), Scene.Subsystem))
	{
		return false;
	}

	AActor* Survivor = Scene.SpawnTarget(FVector(10, 0, 0));
	AActor* Removed = Scene.SpawnTarget(FVector(30, 0, 0));
	if (!TestNotNull(TEXT("Surviving target"), Survivor) || !TestNotNull(TEXT("Removed target"), Removed))
	{
		return false;
	}

	Scene.Subsystem->ReceiveSelection({Survivor, Removed});
	UCombinedTransformGizmo* Gizmo = FindGizmo(Scene.Subsystem);
	UVTBEditorInteractiveToolsContext* Context = FindContext(Scene.Subsystem);
	if (!TestNotNull(TEXT("Gizmo"), Gizmo) || !TestNotNull(TEXT("Context"), Context))
	{
		return false;
	}

	const FTransform Before = Survivor->GetActorTransform();
	FTransform Dragged = Gizmo->ActiveTarget->GetTransform();
	Dragged.AddToTranslation(FVector(50, 0, 0));
	Context->GizmoManager->BeginUndoTransaction(FText::FromString(TEXT("Pending multi-target edit")));
	Gizmo->BeginTransformEditSequence();
	Gizmo->UpdateTransformDuringEditSequence(Dragged);
	Gizmo->EndTransformEditSequence();
	TestTrue(TEXT("The pending gizmo edit moved the surviving actor"),
		Survivor->GetActorLocation().Equals(FVector(60, 0, 0)));

	TestTrue(TEXT("The second selected actor can be destroyed mid-edit"), Removed->Destroy());
	Scene.Subsystem->Tick(0.f);
	TestTrue(TEXT("Invalidation cancels the pending edit and restores the survivor"),
		Survivor->GetActorTransform().Equals(Before));

	Gizmo = FindGizmo(Scene.Subsystem);
	if (!TestNotNull(TEXT("Surviving gizmo"), Gizmo))
	{
		return false;
	}
	UTransformProxy* RefreshedProxy = Gizmo->ActiveTarget;
	if (!TestNotNull(TEXT("Refreshed survivor proxy"), RefreshedProxy))
	{
		return false;
	}
	TestTrue(TEXT("The rebuilt proxy uses the post-rollback component transform"), RefreshedProxy->GetTransform().Equals(Before));
	TestTrue(TEXT("The visible gizmo pivot matches the restored survivor"), Gizmo->GetGizmoTransform().Equals(Before));
	TestFalse(TEXT("The cancelled pending edit does not enter undo history"), Context->CanUndo());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBSubsystemMultiSelectionScaleElementsTest,
	"VTB.RuntimeGizmo.Subsystem.MultiSelectionUsesUniformScale",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FVTBSubsystemMultiSelectionScaleElementsTest::RunTest(const FString& Parameters)
{
	using namespace VTBEditorSubsystemTests;
	FRuntimeWorld Scene;
	if (!TestNotNull(TEXT("Initialized runtime subsystem"), Scene.Subsystem))
	{
		return false;
	}

	Scene.Subsystem->SetTransformGizmoSource(EVTBEditorTransformGizmoSource::CustomVTB);

	AActor* First = Scene.SpawnTarget(FVector(10, 0, 0));
	AActor* Second = Scene.SpawnTarget(FVector(30, 0, 0));
	if (!TestNotNull(TEXT("First target"), First))
	{
		return false;
	}
	if (!TestNotNull(TEXT("Second target"), Second))
	{
		return false;
	}

	Scene.Subsystem->ReceiveSelection({First, Second});
	UCombinedTransformGizmo* Gizmo = FindGizmo(Scene.Subsystem);
	if (!TestNotNull(TEXT("Multi-selection gizmo"), Gizmo))
	{
		return false;
	}

	ETransformGizmoSubElements Elements = Gizmo->GetGizmoElements();
	TestTrue(TEXT("Multi-selection keeps uniform scale"), EnumHasAnyFlags(Elements, ETransformGizmoSubElements::ScaleUniform));
	TestFalse(TEXT("Multi-selection hides axis scale"), EnumHasAnyFlags(Elements, ETransformGizmoSubElements::ScaleAllAxes));
	TestFalse(TEXT("Multi-selection hides plane scale"), EnumHasAnyFlags(Elements, ETransformGizmoSubElements::ScaleAllPlanes));

	Scene.Subsystem->ReceiveSelection({First});
	Gizmo = FindGizmo(Scene.Subsystem);
	if (!TestNotNull(TEXT("Single-selection gizmo"), Gizmo))
	{
		return false;
	}

	Elements = Gizmo->GetGizmoElements();
	TestTrue(TEXT("Single selection keeps uniform scale"), EnumHasAnyFlags(Elements, ETransformGizmoSubElements::ScaleUniform));
	TestTrue(TEXT("Single selection restores axis scale"), EnumHasAnyFlags(Elements, ETransformGizmoSubElements::ScaleAllAxes));
	TestTrue(TEXT("Single selection restores plane scale"), EnumHasAnyFlags(Elements, ETransformGizmoSubElements::ScaleAllPlanes));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBSubsystemReselectionHistoryTest,
	"VTB.RuntimeGizmo.Subsystem.UndoRedoRefreshesReselectedPivot",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FVTBSubsystemReselectionHistoryTest::RunTest(const FString& Parameters)
{
	using namespace VTBEditorSubsystemTests;
	FRuntimeWorld Scene;
	if (!TestNotNull(TEXT("Initialized runtime subsystem"), Scene.Subsystem))
	{
		return false;
	}

	AActor* Actor = Scene.SpawnTarget(FVector(10, 0, 0));
	if (!TestNotNull(TEXT("Target"), Actor))
	{
		return false;
	}

	Scene.Subsystem->ReceiveSelection({Actor});
	UCombinedTransformGizmo* Gizmo = FindGizmo(Scene.Subsystem);
	UVTBEditorInteractiveToolsContext* Context = FindContext(Scene.Subsystem);
	if (!TestNotNull(TEXT("Original gizmo"), Gizmo) || !TestNotNull(TEXT("Context"), Context))
	{
		return false;
	}

	TStrongObjectPtr<UTransformProxy> HistoryProxy(Gizmo->ActiveTarget);
	const FTransform Before = Actor->GetActorTransform();
	FTransform After = Before;
	After.AddToTranslation(FVector(75, 20, 0));
	Gizmo->SetNewGizmoTransform(After);
	TestTrue(TEXT("The gizmo commits an undoable edit"), Context->CanUndo());

	Scene.Subsystem->ReceiveSelection({});
	Scene.Subsystem->ReceiveSelection({Actor});
	Gizmo = FindGizmo(Scene.Subsystem);
	if (!TestNotNull(TEXT("Reselected gizmo"), Gizmo))
	{
		return false;
	}
	TestNotSamePtr(TEXT("Reselection uses a new proxy"), Gizmo->ActiveTarget.Get(), HistoryProxy.Get());

	TestTrue(TEXT("Context undo replays the retained old proxy"), Context->Undo());
	TestTrue(TEXT("Undo restores the component"), Actor->GetActorTransform().Equals(Before));
	Scene.Subsystem->ReceiveSelection({});
	Scene.Subsystem->ReceiveSelection({Actor});
	Gizmo = FindGizmo(Scene.Subsystem);
	if (!TestNotNull(TEXT("Undo refreshed gizmo"), Gizmo))
	{
		return false;
	}
	TestTrue(TEXT("Undo also refreshes the currently selected proxy"), Gizmo->ActiveTarget->GetTransform().Equals(Before));
	TestTrue(TEXT("Undo refreshes the current gizmo pivot"), Gizmo->GetGizmoTransform().Equals(Before));

	TestTrue(TEXT("Context redo replays the retained old proxy"), Context->Redo());
	TestTrue(TEXT("Redo restores the component edit"), Actor->GetActorTransform().Equals(After));
	Scene.Subsystem->ReceiveSelection({});
	Scene.Subsystem->ReceiveSelection({Actor});
	Gizmo = FindGizmo(Scene.Subsystem);
	if (!TestNotNull(TEXT("Redo refreshed gizmo"), Gizmo))
	{
		return false;
	}
	TestTrue(TEXT("Redo refreshes the current gizmo pivot"), Gizmo->GetGizmoTransform().Equals(After));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBSubsystemFocusCancellationReentrancyTest,
	"VTB.RuntimeGizmo.Subsystem.FocusCancellationDefersProxyCallbacks",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FVTBSubsystemFocusCancellationReentrancyTest::RunTest(const FString& Parameters)
{
	using namespace VTBEditorSubsystemTests;
	FRuntimeWorld Scene;
	if (!TestNotNull(TEXT("Initialized runtime subsystem"), Scene.Subsystem))
	{
		return false;
	}

	AActor* First = Scene.SpawnTarget(FVector::ZeroVector);
	AActor* Next = Scene.SpawnTarget(FVector(100, 0, 0));
	AVTBEditorGameMode* Source = Scene.World->SpawnActor<AVTBEditorGameMode>();
	if (!TestNotNull(TEXT("First target"), First) || !TestNotNull(TEXT("Next target"), Next)
		|| !TestNotNull(TEXT("Selection source"), Source))
	{
		return false;
	}

	Source->SetSelectedActors({First});
	Scene.Subsystem->BindSelectionSource(Source);
	UVTBEditorInteractiveToolsContext* Context = FindContext(Scene.Subsystem);
	UCombinedTransformGizmo* Gizmo = FindGizmo(Scene.Subsystem);
	if (!TestNotNull(TEXT("Context"), Context) || !TestNotNull(TEXT("Gizmo"), Gizmo))
	{
		return false;
	}

	TStrongObjectPtr<UTransformProxy> OriginalProxy(Gizmo->ActiveTarget);
	const TArray<UInteractiveGizmo*> Axes = Context->GizmoManager->FindAllGizmosOfType(
		UInteractiveGizmoManager::DefaultAxisPositionBuilderIdentifier);
	UAxisPositionGizmo* Axis = Axes.IsEmpty() ? nullptr : Cast<UAxisPositionGizmo>(Axes[0]);
	if (!TestNotNull(TEXT("Stock translation axis"), Axis))
	{
		return false;
	}

	UGizmoLambdaHitTarget* HitTarget = NewObject<UGizmoLambdaHitTarget>(Axis);
	HitTarget->IsHitFunction = [](const FInputDeviceRay&) { return FInputRayHit(1.0); };
	Axis->HitTarget = HitTarget;
	Gizmo->bSnapToWorldGrid = false;

	FInputDeviceState Input;
	Input.InputDevice = EInputDevices::Mouse;
	Input.Mouse.WorldRay = FRay(FVector(40, 0, -100), FVector::ZAxisVector);
	Input.Mouse.Left.SetStates(true, true, false);
	Context->InputRouter->PostInputEvent(Input);
	if (!TestTrue(TEXT("Real ITF mouse capture begins"), Context->InputRouter->HasActiveMouseCapture()))
	{
		return false;
	}

	Input.Mouse.WorldRay.Origin.X += 25;
	Input.Mouse.Left.SetStates(false, true, false);
	Context->InputRouter->PostInputEvent(Input);
	TestTrue(TEXT("Captured drag moves its target"), First->GetActorLocation().Equals(FVector(25, 0, 0)));

	int32 EndCallbackCount = 0;
	bool bSelectionStayedBoundDuringCallback = false;
	const FDelegateHandle Callback = OriginalProxy->OnEndTransformEdit.AddLambda([&](UTransformProxy*)
	{
		++EndCallbackCount;
		Source->ClearSelection();
		Source->SetSelectedActors({Next});
		Context->CancelActiveInteraction();
		bSelectionStayedBoundDuringCallback = Gizmo->ActiveTarget == OriginalProxy.Get();
	});
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().OnApplicationActivationStateChanged().Broadcast(false);
		FSlateApplication::Get().OnApplicationActivationStateChanged().Broadcast(true);
	}
	else
	{
		AddInfo(TEXT("Slate unavailable; exercising the same context cancellation entry point directly."));
		Context->CancelActiveInteraction();
	}
	OriginalProxy->OnEndTransformEdit.Remove(Callback);

	TestEqual(TEXT("Termination callback runs once despite nested cancellation"), EndCallbackCount, 1);
	TestTrue(TEXT("Selection replacement waits until the cancellation stack unwinds"), bSelectionStayedBoundDuringCallback);
	TestFalse(TEXT("Focus loss releases the captured router"), Context->InputRouter->HasActiveMouseCapture());
	TestTrue(TEXT("Focus loss rolls back the edited actor"), First->GetActorLocation().IsNearlyZero());
	TestFalse(TEXT("Cancelled edit adds no undo entry"), Context->CanUndo());

	Scene.Subsystem->Tick(0.f);
	Gizmo = FindGizmo(Scene.Subsystem);
	if (!TestNotNull(TEXT("The latest deferred selection has a gizmo"), Gizmo))
	{
		return false;
	}
	TestNotSamePtr(TEXT("Deferred selection receives a new proxy"), Gizmo->ActiveTarget.Get(), OriginalProxy.Get());
	TestTrue(TEXT("Deferred proxy points to the next selected actor"),
		Gizmo->ActiveTarget->GetTransform().Equals(Next->GetActorTransform()));

	Context->SetGizmoMode(EToolContextTransformGizmoMode::NoGizmo);
	Context->SetCoordinateSystem(EToolContextCoordinateSystem::Local);
	Source->ClearSelection();
	Source->SetSelectedActors({Next});
	TestNull(TEXT("NoGizmo disables and destroys the gizmo"), FindGizmo(Scene.Subsystem));

	Context->SetGizmoMode(EToolContextTransformGizmoMode::Translation);
	Source->ClearSelection();
	Source->SetSelectedActors({Next});
	Gizmo = FindGizmo(Scene.Subsystem);
	if (!TestNotNull(TEXT("Re-enabling recreates the gizmo"), Gizmo))
	{
		return false;
	}
	TestTrue(TEXT("Re-enabling reconnects the retained selection"),
		Gizmo->ActiveTarget->GetTransform().Equals(Next->GetActorTransform()));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
