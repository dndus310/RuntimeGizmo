#if WITH_DEV_AUTOMATION_TESTS

#include "RuntimeEditor/Selection/VTBEditorTargetSelection.h"

#include "BaseGizmos/TransformProxy.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"
#include "UObject/StrongObjectPtr.h"

namespace VTBEditorSelectionTests
{
	/** Uses only runtime Engine facilities and tears down the world even after an early failure. */
	struct FTestWorld
	{
		UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
		TStrongObjectPtr<UVTBEditorTargetSelection> Adapter{NewObject<UVTBEditorTargetSelection>()};

		~FTestWorld()
		{
			Adapter->SetSelection(World, {});
			Adapter.Reset();
			if (World)
			{
				World->DestroyWorld(false);
			}
		}

		AActor* SpawnActor(const FVector& Location, EComponentMobility::Type Mobility = EComponentMobility::Movable,
			bool bRegisterRoot = true) const
		{
			AActor* Actor = World ? World->SpawnActor<AActor>() : nullptr;
			if (!Actor)
			{
				return nullptr;
			}
			USceneComponent* Root = NewObject<USceneComponent>(Actor);
			Actor->AddInstanceComponent(Root);
			Actor->SetRootComponent(Root);
			Root->SetMobility(Mobility);
			Root->SetWorldLocation(Location);
			if (bRegisterRoot)
			{
				Root->RegisterComponentWithWorld(World);
			}
			return Actor;
		}
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBSelectionFilteringTest,
	"VTB.RuntimeGizmo.Selection.FiltersAndDeduplicates",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FVTBSelectionFilteringTest::RunTest(const FString& Parameters)
{
	using namespace VTBEditorSelectionTests;
	FTestWorld Scene;
	FTestWorld OtherScene;
	if (!TestNotNull(TEXT("Runtime world"), Scene.World) || !TestNotNull(TEXT("Foreign world"), OtherScene.World))
	{
		return false;
	}

	AActor* First = Scene.SpawnActor(FVector(10, 0, 0));
	AActor* Second = Scene.SpawnActor(FVector(30, 0, 0));
	AActor* StaticActor = Scene.SpawnActor(FVector(1000, 0, 0), EComponentMobility::Static);
	AActor* UnregisteredActor = Scene.SpawnActor(FVector(2000, 0, 0), EComponentMobility::Movable, false);
	AActor* RootlessActor = Scene.World->SpawnActor<AActor>();
	AActor* ForeignActor = OtherScene.SpawnActor(FVector(5000, 0, 0));
	if (!TestNotNull(TEXT("First actor"), First) || !TestNotNull(TEXT("Second actor"), Second)
		|| !TestNotNull(TEXT("Static actor"), StaticActor) || !TestNotNull(TEXT("Unregistered actor"), UnregisteredActor)
		|| !TestNotNull(TEXT("Rootless actor"), RootlessActor) || !TestNotNull(TEXT("Foreign actor"), ForeignActor))
	{
		return false;
	}

	const TArray<TWeakObjectPtr<AActor>> Selection{
		First, Second, First, StaticActor, UnregisteredActor, RootlessActor, ForeignActor, nullptr};
	TestTrue(TEXT("Valid roots create a target"), Scene.Adapter->SetSelection(Scene.World, Selection));
	UTransformProxy* Proxy = Scene.Adapter->GetTransformProxy();
	if (!TestNotNull(TEXT("Proxy"), Proxy))
	{
		return false;
	}
	TestTrue(TEXT("Only the two unique movable roots contribute to the pivot"),
		Proxy->GetTransform().GetLocation().Equals(FVector(20, 0, 0)));

	const FVector Delta(5, 8, -3);
	FTransform Moved = Proxy->GetTransform();
	Moved.AddToTranslation(Delta);
	Proxy->SetTransform(Moved);
	TestTrue(TEXT("First root moves once"), First->GetActorLocation().Equals(FVector(10, 0, 0) + Delta));
	TestTrue(TEXT("Second root moves once"), Second->GetActorLocation().Equals(FVector(30, 0, 0) + Delta));
	TestTrue(TEXT("Static root is untouched"), StaticActor->GetActorLocation().Equals(FVector(1000, 0, 0)));
	TestTrue(TEXT("Unregistered root is untouched"), UnregisteredActor->GetActorLocation().Equals(FVector(2000, 0, 0)));
	TestTrue(TEXT("Foreign-world root is untouched"), ForeignActor->GetActorLocation().Equals(FVector(5000, 0, 0)));

	TArray<AActor*> QuerySelection;
	Scene.Adapter->GetSelectedActors(QuerySelection);
	TestEqual(TEXT("Query selection retains five unique valid local actors"), QuerySelection.Num(), 5);
	TestFalse(TEXT("Foreign actor is excluded from query selection"), QuerySelection.Contains(ForeignActor));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBSelectionAttachmentTest,
	"VTB.RuntimeGizmo.Selection.AttachedChildTransformsOnce",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FVTBSelectionAttachmentTest::RunTest(const FString& Parameters)
{
	using namespace VTBEditorSelectionTests;
	FTestWorld Scene;
	AActor* Parent = Scene.SpawnActor(FVector(10, 0, 0));
	AActor* Child = Scene.SpawnActor(FVector(35, 0, 0));
	if (!TestNotNull(TEXT("Parent actor"), Parent) || !TestNotNull(TEXT("Child actor"), Child))
	{
		return false;
	}
	USceneComponent* ChildRoot = Child->GetRootComponent();
	TestTrue(TEXT("Child attaches to parent"),
		ChildRoot->AttachToComponent(Parent->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform));
	const FTransform ChildRelative = ChildRoot->GetRelativeTransform();

	// Put the child first: applying child and then parent is a double-transform regression.
	Scene.Adapter->SetSelection(Scene.World, {Child, Parent});
	UTransformProxy* Proxy = Scene.Adapter->GetTransformProxy();
	if (!TestNotNull(TEXT("Hierarchy proxy"), Proxy))
	{
		return false;
	}
	TestTrue(TEXT("Hierarchy pivot is the selected parent pivot"),
		Proxy->GetTransform().Equals(Parent->GetActorTransform()));

	const FTransform NewParentTransform(FRotator(0, 90, 0), FVector(110, 20, 0), FVector(2));
	Proxy->SetTransform(NewParentTransform);
	TestTrue(TEXT("Parent receives the requested translation, rotation, and scale"),
		Parent->GetActorTransform().Equals(NewParentTransform));
	TestTrue(TEXT("Child receives the inherited transform exactly once"),
		Child->GetActorTransform().Equals(ChildRelative * NewParentTransform));
	TestTrue(TEXT("The child attachment-relative transform is preserved"),
		ChildRoot->GetRelativeTransform().Equals(ChildRelative));

	TStrongObjectPtr<UTransformProxy> PreviousProxy(Proxy);
	ChildRoot->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	TestTrue(TEXT("Detaching makes the child an independent target"), Scene.Adapter->RefreshSelection());
	TestNotSamePtr(TEXT("Attachment target change replaces the proxy"),
		Scene.Adapter->GetTransformProxy(), PreviousProxy.Get());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBSelectionInvalidationTest,
	"VTB.RuntimeGizmo.Selection.RootReplacementAndDestruction",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FVTBSelectionInvalidationTest::RunTest(const FString& Parameters)
{
	using namespace VTBEditorSelectionTests;
	FTestWorld Scene;
	AActor* Actor = Scene.SpawnActor(FVector(4, 0, 0));
	if (!TestNotNull(TEXT("Selected actor"), Actor))
	{
		return false;
	}
	Scene.Adapter->SetSelection(Scene.World, {Actor});
	TStrongObjectPtr<UTransformProxy> PreviousProxy(Scene.Adapter->GetTransformProxy());
	USceneComponent* PreviousRoot = Actor->GetRootComponent();
	const FTransform PreviousRootTransform = PreviousRoot->GetComponentTransform();

	USceneComponent* ReplacementRoot = NewObject<USceneComponent>(Actor);
	Actor->AddInstanceComponent(ReplacementRoot);
	ReplacementRoot->SetMobility(EComponentMobility::Movable);
	ReplacementRoot->SetWorldLocation(FVector(80, 0, 0));
	Actor->SetRootComponent(ReplacementRoot);
	ReplacementRoot->RegisterComponentWithWorld(Scene.World);
	TestTrue(TEXT("Root replacement invalidates the target"), Scene.Adapter->RefreshSelection());
	UTransformProxy* NewProxy = Scene.Adapter->GetTransformProxy();
	if (!TestNotNull(TEXT("Replacement proxy"), NewProxy))
	{
		return false;
	}
	TestNotSamePtr(TEXT("Root replacement creates a distinct proxy"), NewProxy, PreviousProxy.Get());
	TestTrue(TEXT("Replacement proxy starts at the new root"),
		NewProxy->GetTransform().GetLocation().Equals(FVector(80, 0, 0)));
	FTransform Moved = NewProxy->GetTransform();
	Moved.AddToTranslation(FVector(12, 0, 0));
	NewProxy->SetTransform(Moved);
	TestTrue(TEXT("Replacement root receives edits"), Actor->GetActorLocation().Equals(FVector(92, 0, 0)));
	TestTrue(TEXT("Retired root receives no new-target edits"),
		PreviousRoot->GetComponentTransform().Equals(PreviousRootTransform));

	TestTrue(TEXT("Actor destruction succeeds"), Actor->Destroy());
	TestTrue(TEXT("Destruction invalidates the target"), Scene.Adapter->RefreshSelection());
	TestNull(TEXT("Destroyed selection removes the proxy"), Scene.Adapter->GetTransformProxy());
	TArray<AActor*> RemainingSelection;
	Scene.Adapter->GetSelectedActors(RemainingSelection);
	TestTrue(TEXT("Destroyed actor disappears from query selection"), RemainingSelection.IsEmpty());
	TestFalse(TEXT("Further refreshes of an empty target are stable"), Scene.Adapter->RefreshSelection());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBSelectionStableProxyTest,
	"VTB.RuntimeGizmo.Selection.UnchangedSelectionKeepsProxy",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FVTBSelectionStableProxyTest::RunTest(const FString& Parameters)
{
	using namespace VTBEditorSelectionTests;
	FTestWorld Scene;
	AActor* First = Scene.SpawnActor(FVector(10, 0, 0));
	AActor* Second = Scene.SpawnActor(FVector(30, 0, 0));
	if (!TestNotNull(TEXT("First actor"), First) || !TestNotNull(TEXT("Second actor"), Second))
	{
		return false;
	}
	Scene.Adapter->SetSelection(Scene.World, {First, Second});
	TStrongObjectPtr<UTransformProxy> OriginalProxy(Scene.Adapter->GetTransformProxy());
	if (!TestNotNull(TEXT("Original proxy"), OriginalProxy.Get()))
	{
		return false;
	}
	FTransform Edited = OriginalProxy->GetTransform();
	Edited.AddToTranslation(FVector(25, 10, 0));
	OriginalProxy->SetTransform(Edited);

	TestFalse(TEXT("Duplicate and reordered snapshots do not change the target"),
		Scene.Adapter->SetSelection(Scene.World, {Second, First, Second}));
	TestSamePtr(TEXT("Equivalent selection retains the proxy for existing history"),
		Scene.Adapter->GetTransformProxy(), OriginalProxy.Get());
	TestTrue(TEXT("Equivalent selection preserves the edited proxy transform"),
		Scene.Adapter->GetTransformProxy()->GetTransform().Equals(Edited));
	TestFalse(TEXT("Refreshing unchanged selection does not replace the proxy"), Scene.Adapter->RefreshSelection());

	Second->GetRootComponent()->UnregisterComponent();
	TestTrue(TEXT("Unregistration changes the effective targets"), Scene.Adapter->RefreshSelection());
	TestNotSamePtr(TEXT("Changed targets receive a new proxy"),
		Scene.Adapter->GetTransformProxy(), OriginalProxy.Get());
	TestTrue(TEXT("The remaining movable root determines the pivot"),
		Scene.Adapter->GetTransformProxy()->GetTransform().Equals(First->GetActorTransform()));
	Second->GetRootComponent()->RegisterComponentWithWorld(Scene.World);
	TestTrue(TEXT("Re-registering a selected root restores it to the target"), Scene.Adapter->RefreshSelection());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBSelectionHistoryRefreshTest,
	"VTB.RuntimeGizmo.Selection.RefreshesAfterRetiredProxyUndo",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FVTBSelectionHistoryRefreshTest::RunTest(const FString& Parameters)
{
	using namespace VTBEditorSelectionTests;
	FTestWorld Scene;
	AActor* Actor = Scene.SpawnActor(FVector(10, 0, 0));
	if (!TestNotNull(TEXT("Actor"), Actor))
	{
		return false;
	}
	Scene.Adapter->SetSelection(Scene.World, {Actor});
	TStrongObjectPtr<UTransformProxy> HistoryProxy(Scene.Adapter->GetTransformProxy());
	if (!TestNotNull(TEXT("History proxy"), HistoryProxy.Get()))
	{
		return false;
	}
	const FTransform BeforeEdit = HistoryProxy->GetTransform();
	FTransform AfterEdit = BeforeEdit;
	AfterEdit.AddToTranslation(FVector(50, 0, 0));
	HistoryProxy->SetTransform(AfterEdit);
	Scene.Adapter->SetSelection(Scene.World, {});
	Scene.Adapter->SetSelection(Scene.World, {Actor});
	TStrongObjectPtr<UTransformProxy> ReselectedProxy(Scene.Adapter->GetTransformProxy());
	if (!TestNotNull(TEXT("Reselected proxy"), ReselectedProxy.Get()))
	{
		return false;
	}
	TestNotSamePtr(TEXT("Reselection creates a separate proxy"), ReselectedProxy.Get(), HistoryProxy.Get());

	// FTransformProxyChange::Revert applies to its retained original proxy after reselection.
	FTransformProxyChange HistoryChange;
	HistoryChange.From = BeforeEdit;
	HistoryChange.To = AfterEdit;
	HistoryChange.Revert(HistoryProxy.Get());
	TestTrue(TEXT("Undo through retired proxy restores the actor"), Actor->GetActorTransform().Equals(BeforeEdit));
	TestTrue(TEXT("Reselected proxy still has its independent cached transform before refresh"),
		ReselectedProxy->GetTransform().Equals(AfterEdit));

	Scene.Adapter->RebuildFromCurrentTransforms();
	UTransformProxy* RefreshedProxy = Scene.Adapter->GetTransformProxy();
	if (!TestNotNull(TEXT("Refreshed proxy"), RefreshedProxy))
	{
		return false;
	}
	TestTrue(TEXT("Fresh active target adopts the restored component transform"),
		RefreshedProxy->GetTransform().Equals(BeforeEdit));
	TestNotSamePtr(TEXT("Refresh does not repurpose the reselection proxy"), RefreshedProxy, ReselectedProxy.Get());
	TestTrue(TEXT("Retired reselection proxy retains its own history state"),
		ReselectedProxy->GetTransform().Equals(AfterEdit));

	HistoryChange.Apply(HistoryProxy.Get());
	Scene.Adapter->RebuildFromCurrentTransforms();
	TestTrue(TEXT("Refresh also adopts redo through the original proxy"),
		Scene.Adapter->GetTransformProxy()->GetTransform().Equals(AfterEdit));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
