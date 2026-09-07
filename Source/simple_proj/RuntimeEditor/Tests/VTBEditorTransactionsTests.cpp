#if WITH_DEV_AUTOMATION_TESTS

#include "RuntimeEditor/Context/VTBEditorInteractiveToolsContext.h"
#include "RuntimeEditor/Selection/VTBEditorTargetSelection.h"

#include "BaseGizmos/TransformProxy.h"
#include "Changes/TransformChange.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/Actor.h"
#include "InputRouter.h"
#include "InteractiveGizmoManager.h"
#include "Misc/AutomationTest.h"
#include "UObject/GarbageCollection.h"
#include "UObject/StrongObjectPtr.h"

namespace VTBEditorTransactionsTests
{
	struct FTestScene
	{
		// CreateWorld roots the world; strong pointers retain the services during explicit GC tests.
		UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
		TStrongObjectPtr<UVTBEditorInteractiveToolsContext> Context{NewObject<UVTBEditorInteractiveToolsContext>()};
		TStrongObjectPtr<UVTBEditorTargetSelection> Adapter{NewObject<UVTBEditorTargetSelection>()};
		AActor* Actor = nullptr;
		bool bInitialized = false;

		FTestScene()
		{
			if (!World || !Context->InitializeRuntime(World))
			{
				return;
			}
			Actor = World->SpawnActor<AActor>();
			if (!Actor)
			{
				return;
			}
			USceneComponent* Root = NewObject<USceneComponent>(Actor);
			Actor->AddInstanceComponent(Root);
			Actor->SetRootComponent(Root);
			Root->SetMobility(EComponentMobility::Movable);
			Root->RegisterComponentWithWorld(World);
			Adapter->SetSelection(World, {Actor});
			Context->SetSelection({Actor});
			bInitialized = Adapter->GetTransformProxy() != nullptr;
		}

		~FTestScene()
		{
			Context->Shutdown();
			Adapter->SetSelection(World, {});
			Adapter.Reset();
			Context.Reset();
			if (World)
			{
				World->DestroyWorld(false);
			}
		}

		void ApplyAndRecord(UTransformProxy* Proxy, const FVector& Position, bool bCloseTransaction = true)
		{
			UInteractiveGizmoManager* Manager = Context->GizmoManager;
			Manager->BeginUndoTransaction(FText::FromString(TEXT("Transform test")));
			TUniquePtr<FTransformProxyChange> Change = MakeUnique<FTransformProxyChange>();
			Change->From = Proxy->GetTransform();
			FTransform After = Change->From;
			After.SetLocation(Position);
			Proxy->SetTransform(After);
			Change->To = Proxy->GetTransform();
			Manager->EmitObjectChange(Proxy, MoveTemp(Change), FText::FromString(TEXT("Move actor")));
			if (bCloseTransaction)
			{
				Manager->EndUndoTransaction();
			}
		}
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBTransactionsGroupingTest,
	"VTB.RuntimeGizmo.Transactions.GroupsAndBranches",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FVTBTransactionsGroupingTest::RunTest(const FString& Parameters)
{
	using namespace VTBEditorTransactionsTests;
	FTestScene Scene;
	if (!TestTrue(TEXT("Runtime context and actor initialize"), Scene.bInitialized))
	{
		return false;
	}
	TestEqual(TEXT("Context uses the stock ITF input router"), Scene.Context->InputRouter->GetClass(), UInputRouter::StaticClass());
	TestEqual(TEXT("Context uses the stock ITF gizmo manager"), Scene.Context->GizmoManager->GetClass(), UInteractiveGizmoManager::StaticClass());
	UTransformProxy* Proxy = Scene.Adapter->GetTransformProxy();
	Scene.Context->GizmoManager->BeginUndoTransaction(FText::FromString(TEXT("Grouped edit")));
	Scene.ApplyAndRecord(Proxy, FVector(10, 0, 0));
	Scene.ApplyAndRecord(Proxy, FVector(30, 0, 0));
	TestFalse(TEXT("Undo is disabled inside an open transaction"), Scene.Context->CanUndo());
	Scene.Context->GizmoManager->EndUndoTransaction();
	TestTrue(TEXT("Nested edits form one undo step"), Scene.Context->Undo());
	TestTrue(TEXT("Reversal runs changes in reverse order"), Scene.Actor->GetActorLocation().Equals(FVector::ZeroVector));
	TestFalse(TEXT("The grouped edit has no extra undo step"), Scene.Context->CanUndo());
	TestTrue(TEXT("Grouped edit redoes"), Scene.Context->Redo());
	TestTrue(TEXT("Redo runs changes in original order"), Scene.Actor->GetActorLocation().Equals(FVector(30, 0, 0)));

	Scene.ApplyAndRecord(Proxy, FVector(40, 0, 0));
	TestTrue(TEXT("Separate edit undoes independently"), Scene.Context->Undo());
	TestTrue(TEXT("Undo returns to preceding edit"), Scene.Actor->GetActorLocation().Equals(FVector(30, 0, 0)));
	TestTrue(TEXT("Undone edit can redo"), Scene.Context->CanRedo());
	Scene.ApplyAndRecord(Proxy, FVector(60, 0, 0));
	TestFalse(TEXT("New edit replaces the redo branch"), Scene.Context->CanRedo());
	TestTrue(TEXT("New branch can undo"), Scene.Context->Undo());
	TestTrue(TEXT("Branch reversal keeps previous group"), Scene.Actor->GetActorLocation().Equals(FVector(30, 0, 0)));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBTransactionsCancellationTest,
	"VTB.RuntimeGizmo.Transactions.ForcedTerminationRollsBackAndKeepsRedo",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FVTBTransactionsCancellationTest::RunTest(const FString& Parameters)
{
	using namespace VTBEditorTransactionsTests;
	FTestScene Scene;
	if (!TestTrue(TEXT("Runtime context and actor initialize"), Scene.bInitialized))
	{
		return false;
	}
	UTransformProxy* Proxy = Scene.Adapter->GetTransformProxy();
	Scene.ApplyAndRecord(Proxy, FVector(20, 0, 0));
	TestTrue(TEXT("Completed edit can undo"), Scene.Context->Undo());
	Scene.ApplyAndRecord(Proxy, FVector(100, 0, 0), false);
	TestTrue(TEXT("Pending gesture immediately moves actor"), Scene.Actor->GetActorLocation().Equals(FVector(100, 0, 0)));
	if (FSlateApplication::IsInitialized())
	{
		// Exercise the registered focus-loss callback, including its rollback policy.
		FSlateApplication::Get().OnApplicationActivationStateChanged().Broadcast(false);
		FSlateApplication::Get().OnApplicationActivationStateChanged().Broadcast(true);
	}
	else
	{
		AddInfo(TEXT("Slate is unavailable; checking explicit context cancellation instead of application focus loss."));
		Scene.Context->CancelActiveInteraction();
	}
	TestTrue(TEXT("Context focus cancellation restores gesture start"), Scene.Actor->GetActorLocation().Equals(FVector::ZeroVector));
	TestFalse(TEXT("Cancelled gesture adds no undo entry"), Scene.Context->CanUndo());
	TestTrue(TEXT("Cancellation preserves the previous redo branch"), Scene.Context->CanRedo());
	TestTrue(TEXT("Earlier completed edit still redoes"), Scene.Context->Redo());
	TestTrue(TEXT("Redo restores completed edit, not cancelled gesture"), Scene.Actor->GetActorLocation().Equals(FVector(20, 0, 0)));

	Scene.ApplyAndRecord(Proxy, FVector(200, 0, 0), false);
	Scene.Context->CancelActiveInteraction();
	TestTrue(TEXT("Explicit context cancellation also rolls back"), Scene.Actor->GetActorLocation().Equals(FVector(20, 0, 0)));
	Scene.Context->CancelActiveInteraction();
	TestTrue(TEXT("Cancellation is safe when no gesture exists"), Scene.Actor->GetActorLocation().Equals(FVector(20, 0, 0)));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBTransactionsLifetimeTest,
	"VTB.RuntimeGizmo.Transactions.ProxySurvivesSelectionAndGarbageCollection",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FVTBTransactionsLifetimeTest::RunTest(const FString& Parameters)
{
	using namespace VTBEditorTransactionsTests;
	FTestScene Scene;
	if (!TestTrue(TEXT("Runtime context and actor initialize"), Scene.bInitialized))
	{
		return false;
	}
	UTransformProxy* Proxy = Scene.Adapter->GetTransformProxy();
	const TWeakObjectPtr<UTransformProxy> HistoryProxy(Proxy);
	Scene.ApplyAndRecord(Proxy, FVector(25, 0, 0));
	Scene.Adapter->SetSelection(Scene.World, {});
	Scene.Context->SetSelection({});
	Proxy = nullptr;
	CollectGarbage(RF_NoFlags);
	TestTrue(TEXT("History retains the retired proxy across garbage collection"), HistoryProxy.IsValid());
	TestTrue(TEXT("Retired selection can undo"), Scene.Context->Undo());
	TestTrue(TEXT("Undo still updates its scene component"), Scene.Actor->GetActorLocation().Equals(FVector::ZeroVector));
	TestTrue(TEXT("Retired selection can redo"), Scene.Context->Redo());
	TestTrue(TEXT("Redo still updates its scene component"), Scene.Actor->GetActorLocation().Equals(FVector(25, 0, 0)));

	// Non-proxy targets are weak: invalid entries are skipped without blocking older history.
	USceneComponent* EphemeralComponent = NewObject<USceneComponent>();
	const TWeakObjectPtr<USceneComponent> WeakEphemeral(EphemeralComponent);
	const FTransform Before = EphemeralComponent->GetComponentTransform();
	const FTransform After(FVector(10, 20, 30));
	EphemeralComponent->SetWorldTransform(After);
	Scene.Context->GizmoManager->EmitObjectChange(EphemeralComponent,
		MakeUnique<FComponentWorldTransformChange>(Before, After), FText::FromString(TEXT("Ephemeral component")));
	EphemeralComponent = nullptr;
	CollectGarbage(RF_NoFlags);
	TestFalse(TEXT("History does not retain ordinary scene objects"), WeakEphemeral.IsValid());
	TestTrue(TEXT("Undo skips the expired top entry"), Scene.Context->Undo());
	TestTrue(TEXT("Undo reaches the earlier live proxy entry"), Scene.Actor->GetActorLocation().Equals(FVector::ZeroVector));
	TestTrue(TEXT("Live entry remains redoable"), Scene.Context->Redo());

	Scene.Context->Shutdown();
	CollectGarbage(RF_NoFlags);
	TestFalse(TEXT("Context shutdown releases the retired proxy"), HistoryProxy.IsValid());
	TestFalse(TEXT("Shutdown context cannot undo"), Scene.Context->CanUndo());
	TestFalse(TEXT("Shutdown context cannot redo"), Scene.Context->CanRedo());
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
