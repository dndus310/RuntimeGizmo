#if WITH_DEV_AUTOMATION_TESTS

#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Duplication/OWTRuntimeActorDuplicator.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "PCGComponent.h"
#include "Subsystems/PCGSubsystem.h"

namespace
{
int32 CountChildPCGInstances(const AActor& Actor)
{
	int32 Count = 0;
	TInlineComponentArray<UInstancedStaticMeshComponent*> Components(&Actor);
	for (const UInstancedStaticMeshComponent* Component : Components)
	{
		Count += Component->GetInstanceCount();
	}
	return Count;
}

bool FinishChildPCGGeneration(UWorld& World, UPCGComponent& Component, UOWTRuntimeActorDuplicator* Observer)
{
	UPCGSubsystem* Subsystem = World.GetSubsystem<UPCGSubsystem>();
	check(Subsystem);
	const double Deadline = FPlatformTime::Seconds() + 15.0;
	while (Component.IsGenerating())
	{
		Subsystem->Tick(1.0f / 60.0f);
		if (Observer)
		{
			Observer->Tick(1.0f / 60.0f);
		}
		FPlatformProcess::Sleep(0.001f);
		if (FPlatformTime::Seconds() > Deadline)
		{
			Component.CancelGeneration();
			return false;
		}
	}
	return Component.bGenerated;
}
} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTPCGChildActorOnLoadTest, "OWT.Runtime.PCGChildActorOnLoad",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTPCGChildActorOnLoadTest::RunTest(const FString& Parameters)
{
	UClass* ChildClass =
	    LoadClass<AActor>(nullptr, TEXT("/OWTRuntimeEditing/Tests/BP_OWTPCGFixture.BP_OWTPCGFixture_C"));
	if (!TestNotNull(TEXT("Cooked ChildActor Blueprint fixture"), ChildClass))
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
	if (!TestNotNull(TEXT("ChildActor runtime test world"), World))
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	World->InitializeActorsForPlay(FURL());
	World->SetBegunPlay(true);
	ON_SCOPE_EXIT
	{
		World->EndPlay(EEndPlayReason::Quit);
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
	};
	AActor* Source = World->SpawnActor<AActor>();
	UBoxComponent* Root = NewObject<UBoxComponent>(Source);
	Source->AddInstanceComponent(Root);
	Source->SetRootComponent(Root);
	Root->RegisterComponent();
	UChildActorComponent* ChildComponent = NewObject<UChildActorComponent>(Source);
	Source->AddInstanceComponent(ChildComponent);
	ChildComponent->SetupAttachment(Root);
	ChildComponent->SetRelativeLocation(FVector(150, 0, 0));
	ChildComponent->SetChildActorClass(ChildClass);
	ChildComponent->RegisterComponent();
	AActor* SourceChild = ChildComponent->GetChildActor();
	if (!TestNotNull(TEXT("Source ChildActor constructed"), SourceChild))
	{
		return false;
	}
	TestTrue(TEXT("Source child has begun play"), SourceChild->HasActorBegunPlay());
	UPCGComponent* SourcePCG = SourceChild->FindComponentByClass<UPCGComponent>();
	if (!TestNotNull(TEXT("Source child has authored PCG"), SourcePCG))
	{
		return false;
	}
	SourcePCG->GenerationTrigger = EPCGComponentGenerationTrigger::GenerateOnLoad;
	SourcePCG->bActivated = true;
	SourcePCG->GenerateLocal(true);
	if (!TestTrue(TEXT("Source child CPU graph completes"), FinishChildPCGGeneration(*World, *SourcePCG, nullptr)))
	{
		return false;
	}
	TestEqual(TEXT("Source child owns two generated instances"), CountChildPCGInstances(*SourceChild), 2);
	UOWTRuntimeActorDuplicator* Duplicator = NewObject<UOWTRuntimeActorDuplicator>(Source);
	Duplicator->Initialize(Source);
	ON_SCOPE_EXIT
	{
		Duplicator->Deinitialize();
	};
	bool bCommitted = false;
	bool bSawGenerating = false;
	FOWTDuplicationOptions Options;
	Options.WorldOffset = FVector(500, 0, 0);
	Options.OnAuthoredCommitted = [&](AActor* CommittedRoot)
	{
		UChildActorComponent* CommittedCAC = CommittedRoot->FindComponentByClass<UChildActorComponent>();
		if (!TestNotNull(TEXT("CAC is restored before authored commit"), CommittedCAC))
		{
			return;
		}
		AActor* CommittedChild = CommittedCAC->GetChildActor();
		if (!TestNotNull(TEXT("ChildActor exists before authored commit"), CommittedChild))
		{
			return;
		}
		UPCGComponent* CommittedPCG = CommittedChild->FindComponentByClass<UPCGComponent>();
		if (!TestNotNull(TEXT("Child PCG exists before authored commit"), CommittedPCG))
		{
			return;
		}
		TestTrue(TEXT("Child has completed BeginPlay before commit"), CommittedChild->HasActorBegunPlay());
		TestEqual(TEXT("OnLoad trigger restored before BeginPlay returns"), CommittedPCG->GenerationTrigger,
		          EPCGComponentGenerationTrigger::GenerateOnLoad);
		TestFalse(TEXT("Child PCG remains suppressed through BeginPlay"), CommittedPCG->bActivated);
		TestFalse(TEXT("No PCG task starts before authored commit"), CommittedPCG->IsGenerating());
		TestFalse(TEXT("No generated state copied from source child"), CommittedPCG->bGenerated);
		TestEqual(TEXT("Source resources were not copied into the child"), CountChildPCGInstances(*CommittedChild), 0);
		bCommitted = true;
	};
	Duplicator->OnProceduralChanged.AddLambda(
	    [&](const FOWTProceduralComponentSnapshot& Snapshot)
	    {
		    if (Snapshot.State == EOWTProceduralState::Generating)
		    {
			    TestTrue(TEXT("OnLoad generation starts after authored commit"), bCommitted);
			    bSawGenerating = true;
		    }
	    });
	FString Error;
	AActor* Copy = Duplicator->DuplicateActorWithOptions(Source, Options, FGuid::NewGuid(), Error);
	if (!TestNotNull(*Error, Copy))
	{
		return false;
	}
	UChildActorComponent* CopiedCAC = Copy->FindComponentByClass<UChildActorComponent>();
	if (!TestNotNull(TEXT("Copied CAC"), CopiedCAC))
	{
		return false;
	}
	AActor* CopiedChild = CopiedCAC->GetChildActor();
	if (!TestNotNull(TEXT("Copied child"), CopiedChild))
	{
		return false;
	}
	TestEqual(TEXT("Cooked child Blueprint class retained"), CopiedChild->GetClass(), ChildClass);
	TestTrue(TEXT("Copied child's CAC reference targets copied component"),
	         CopiedChild->GetParentComponent() == CopiedCAC);
	TestTrue(TEXT("Copied CAC belongs to copied root"), CopiedCAC->GetOwner() == Copy);
	TestTrue(TEXT("Source child ownership unchanged"), SourceChild->GetParentComponent() == ChildComponent);
	TestTrue(TEXT("Child world offset applied exactly once"),
	         CopiedChild->GetActorLocation().Equals(SourceChild->GetActorLocation() + Options.WorldOffset));
	UPCGComponent* CopiedPCG = CopiedChild->FindComponentByClass<UPCGComponent>();
	if (!TestNotNull(TEXT("Copied child PCG"), CopiedPCG))
	{
		return false;
	}
	TestTrue(TEXT("OnLoad target activates after commit"), CopiedPCG->bActivated);
	TestTrue(TEXT("Target graph instance is independent"),
	         CopiedPCG->GetGraphInstance() != SourcePCG->GetGraphInstance());
	TestTrue(TEXT("Child OnLoad generation completes"), FinishChildPCGGeneration(*World, *CopiedPCG, Duplicator));
	TestTrue(TEXT("OnLoad generation was observed"), bSawGenerating);
	TestEqual(TEXT("Copied child owns exactly two fresh instances"), CountChildPCGInstances(*CopiedChild), 2);
	const auto Snapshots = Duplicator->GetProceduralComponents();
	TestEqual(TEXT("One authored child PCG observation"), Snapshots.Num(), 1);
	if (Snapshots.Num() == 1)
	{
		TestEqual(TEXT("Copied child reaches Ready"), Snapshots[0].State, EOWTProceduralState::Ready);
	}
	CopiedPCG->CleanupLocalImmediate(true, true);
	TestEqual(TEXT("Copied child cleanup preserves source output"), CountChildPCGInstances(*SourceChild), 2);
	TestEqual(TEXT("Copied child cleanup removes only copied output"), CountChildPCGInstances(*CopiedChild), 0);
	SourcePCG->CleanupLocalImmediate(true, true);
	return true;
}
#endif
