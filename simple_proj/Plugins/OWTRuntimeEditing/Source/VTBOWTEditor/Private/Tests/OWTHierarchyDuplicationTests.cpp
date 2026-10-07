#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "Components/SceneComponent.h"
#include "Duplication/OWTRuntimeActorDuplicator.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTAuthoredHierarchyTest, "OWT.Runtime.AuthoredHierarchy",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTAuthoredHierarchyTest::RunTest(const FString& Parameters)
{
	const UWorld::InitializationValues Init = UWorld::InitializationValues()
	                                              .AllowAudioPlayback(false)
	                                              .CreatePhysicsScene(true)
	                                              .CreateNavigation(false)
	                                              .CreateAISystem(false)
	                                              .ShouldSimulatePhysics(false);
	UWorld* World =
	    UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &Init);
	if (!TestNotNull(TEXT("Hierarchy world"), World))
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	ON_SCOPE_EXIT
	{
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
	};
	auto Spawn = [World](const FVector& Position)
	{
		AActor* Actor = World->SpawnActor<AActor>();
		USceneComponent* Root = NewObject<USceneComponent>(Actor);
		Actor->AddInstanceComponent(Root);
		Actor->SetRootComponent(Root);
		Root->RegisterComponent();
		Actor->SetActorLocation(Position);
		return Actor;
	};
	AActor* Parent = Spawn(FVector(20, 30, 40));
	AActor* Child = Spawn(FVector(55, 80, 100));
	AActor* Grandchild = Spawn(FVector(100, 120, 160));
	Child->AttachToActor(Parent, FAttachmentTransformRules::KeepWorldTransform);
	Grandchild->AttachToActor(Child, FAttachmentTransformRules::KeepWorldTransform);
	const FTransform ChildRelative = Child->GetRootComponent()->GetRelativeTransform();
	const FTransform GrandchildRelative = Grandchild->GetRootComponent()->GetRelativeTransform();
	UOWTRuntimeActorDuplicator* Duplicator = NewObject<UOWTRuntimeActorDuplicator>(Parent);
	TestTrue(TEXT("Owner initializes duplicator"), Duplicator->Initialize(Parent));
	FOWTDuplicationOptions Options;
	Options.WorldOffset = FVector(200, 300, 0);
	FString Error;
	AActor* Copy = Duplicator->DuplicateActorWithOptions(Parent, Options, FGuid::NewGuid(), Error);
	if (!TestNotNull(*Error, Copy))
	{
		return false;
	}
	TArray<AActor*> Children;
	Copy->GetAttachedActors(Children, true, false);
	TestEqual(TEXT("One direct authored child"), Children.Num(), 1);
	if (Children.Num() != 1)
	{
		return false;
	}
	TestTrue(TEXT("Child belongs to the new hierarchy"), Children[0] != Child);
	TestTrue(TEXT("Child relative transform preserved"),
	         Children[0]->GetRootComponent()->GetRelativeTransform().Equals(ChildRelative));
	TestTrue(TEXT("World offset applied once"),
	         Children[0]->GetActorLocation().Equals(Child->GetActorLocation() + Options.WorldOffset));
	TArray<AActor*> Grandchildren;
	Children[0]->GetAttachedActors(Grandchildren, true, false);
	TestEqual(TEXT("One grandchild"), Grandchildren.Num(), 1);
	if (Grandchildren.Num() == 1)
	{
		TestTrue(TEXT("Grandchild relative transform preserved"),
		         Grandchildren[0]->GetRootComponent()->GetRelativeTransform().Equals(GrandchildRelative));
	}
	AActor* LegacyCopy = Duplicator->DuplicateActor(Parent, FVector::ZeroVector, Error);
	TestNotNull(TEXT("Legacy actor and managed-child wrapper"), LegacyCopy);
	if (LegacyCopy)
	{
		Children.Reset();
		LegacyCopy->GetAttachedActors(Children, true, true);
		TestEqual(TEXT("Legacy wrapper does not copy ordinary attachments"), Children.Num(), 0);
	}
	Duplicator->Deinitialize();
	return true;
}
#endif
