#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Duplication/OWTRuntimeActorDuplicator.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "PCGComponent.h"
#include "PCGGraph.h"
#include "Subsystems/PCGSubsystem.h"

namespace
{
bool FinishGeneration(UWorld& World, UPCGComponent& Component)
{
	UPCGSubsystem* Subsystem = World.GetSubsystem<UPCGSubsystem>();
	if (!Subsystem)
	{
		return false;
	}
	const double Deadline = FPlatformTime::Seconds() + 15.0;
	while (Component.IsGenerating())
	{
		Subsystem->Tick(1.0f / 60.0f);
		FPlatformProcess::Sleep(0.001f);
		if (FPlatformTime::Seconds() > Deadline)
		{
			Component.CancelGeneration();
			return false;
		}
	}
	return Component.bGenerated;
}

int32 InstanceCount(AActor& Actor)
{
	TInlineComponentArray<UInstancedStaticMeshComponent*> Components(&Actor);
	int32 Count = 0;
	for (const UInstancedStaticMeshComponent* Component : Components)
	{
		Count += Component->GetInstanceCount();
	}
	return Count;
}
} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTPCGCookedAssetTest, "OWT.Runtime.PCGCookedBlueprintHierarchy",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTPCGCookedAssetTest::RunTest(const FString& Parameters)
{
	UClass* BlueprintClass =
	    LoadClass<AActor>(nullptr, TEXT("/OWTRuntimeEditing/Tests/BP_OWTPCGFixture.BP_OWTPCGFixture_C"));
	if (!TestNotNull(TEXT("Plugin cooked Blueprint fixture"), BlueprintClass))
	{
		return false;
	}
	const UWorld::InitializationValues Init = UWorld::InitializationValues()
	                                              .AllowAudioPlayback(false)
	                                              .CreatePhysicsScene(true)
	                                              .CreateNavigation(false)
	                                              .CreateAISystem(false)
	                                              .ShouldSimulatePhysics(false);
	UWorld* World =
	    UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &Init);
	if (!TestNotNull(TEXT("Cooked PCG world"), World))
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	World->InitializeActorsForPlay(FURL());
	// This isolated test world has no GameMode; enable the same spawned-Actor BeginPlay path as a live map.
	World->SetBegunPlay(true);
	ON_SCOPE_EXIT
	{
		World->EndPlay(EEndPlayReason::Quit);
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
	};
	AActor* Source = World->SpawnActor<AActor>(BlueprintClass);
	TestTrue(TEXT("Cooked PCG source follows the live BeginPlay path"), Source->HasActorBegunPlay());
	AActor* Child = World->SpawnActor<AActor>(BlueprintClass, FVector(300, 0, 0), FRotator::ZeroRotator);
	Child->AttachToActor(Source, FAttachmentTransformRules::KeepWorldTransform);
	UPCGComponent* SourcePCG = Source->FindComponentByClass<UPCGComponent>();
	UPCGComponent* ChildPCG = Child->FindComponentByClass<UPCGComponent>();
	if (!TestNotNull(TEXT("SCS PCG component"), SourcePCG))
	{
		return false;
	}
	if (!TestNotNull(TEXT("Child SCS PCG component"), ChildPCG))
	{
		return false;
	}
	SourcePCG->GenerateLocal(true);
	ChildPCG->GenerateLocal(true);
	if (!TestTrue(TEXT("Cooked source graph produces CPU output"), FinishGeneration(*World, *SourcePCG)))
	{
		return false;
	}
	if (!TestTrue(TEXT("Cooked child graph produces CPU output"), FinishGeneration(*World, *ChildPCG)))
	{
		return false;
	}
	TestEqual(TEXT("Source starts with two generated instances"), InstanceCount(*Source), 2);
	UOWTRuntimeActorDuplicator* Duplicator = NewObject<UOWTRuntimeActorDuplicator>(Source);
	Duplicator->Initialize(Source);
	ON_SCOPE_EXIT
	{
		Duplicator->Deinitialize();
	};
	FOWTDuplicationOptions Options;
	Options.WorldOffset = FVector(500, 0, 0);
	const FGuid OperationId = FGuid::NewGuid();
	FString Error;
	AActor* Copy = Duplicator->DuplicateActorWithOptions(Source, Options, OperationId, Error);
	if (!TestNotNull(*Error, Copy))
	{
		return false;
	}
	TestEqual(TEXT("Generated Blueprint class retained"), Copy->GetClass(), BlueprintClass);
	TArray<AActor*> Children;
	Copy->GetAttachedActors(Children, true, false);
	TestEqual(TEXT("PCG authored child retained exactly once"), Children.Num(), 1);
	if (Children.Num() != 1)
	{
		return false;
	}
	TestEqual(TEXT("Attached Blueprint class retained"), Children[0]->GetClass(), BlueprintClass);
	UPCGComponent* CopyPCG = Copy->FindComponentByClass<UPCGComponent>();
	UPCGComponent* CopyChildPCG = Children[0]->FindComponentByClass<UPCGComponent>();
	if (!TestNotNull(TEXT("Duplicated PCG"), CopyPCG))
	{
		return false;
	}
	if (!TestNotNull(TEXT("Duplicated child PCG"), CopyChildPCG))
	{
		return false;
	}
	TestTrue(TEXT("Root fresh generation completed"), FinishGeneration(*World, *CopyPCG));
	TestTrue(TEXT("Child fresh generation completed"), FinishGeneration(*World, *CopyChildPCG));
	TestTrue(TEXT("Child offset once"),
	         Children[0]->GetActorLocation().Equals(Child->GetActorLocation() + Options.WorldOffset));
	TestEqual(TEXT("Root output not copied twice"), InstanceCount(*Copy), 2);
	TestEqual(TEXT("Child output not copied twice"), InstanceCount(*Children[0]), 2);
	const auto States = Duplicator->GetProceduralComponents();
	TestEqual(TEXT("Two components observed independently"), States.Num(), 2);
	for (const FOWTProceduralComponentSnapshot& State : States)
	{
		TestEqual(TEXT("Observation operation correlation"), State.OperationId, OperationId);
		TestEqual(TEXT("Each component reaches Ready"), State.State, EOWTProceduralState::Ready);
	}
	CopyPCG->CleanupLocalImmediate(true, true);
	CopyChildPCG->CleanupLocalImmediate(true, true);
	TestEqual(TEXT("Copy root cleanup leaves source output intact"), InstanceCount(*Source), 2);
	TestEqual(TEXT("Copy child cleanup leaves source child output intact"), InstanceCount(*Child), 2);
	SourcePCG->CleanupLocalImmediate(true, true);
	ChildPCG->CleanupLocalImmediate(true, true);
	return true;
}
#endif
