#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Duplication/OWTRuntimeActorDuplicator.h"
#include "Duplication/OWTPCGComponentConfiguration.h"
#include "Elements/PCGCreatePoints.h"
#include "Elements/PCGStaticMeshSpawner.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "MeshSelectors/PCGMeshSelectorWeighted.h"
#include "PCGComponent.h"
#include "PCGGraph.h"
#include "PCGManagedResource.h"
#include "PCGNode.h"
#include "PCGWorldActor.h"
#include "RuntimeGen/PCGGenSourceManager.h"
#include "RuntimeGen/GenSources/PCGGenSourceComponent.h"
#include "Tests/OWTPCGTestGraph.h"
#include "Tests/OWTPCGConfigurationTestTypes.h"
#include "Subsystems/PCGSubsystem.h"
#include "UObject/StrongObjectPtr.h"
#include "UObject/Package.h"
#include "UObject/UnrealType.h"

namespace
{
struct FPCGTestWorld
{
	FPCGTestWorld()
	{
		const UWorld::InitializationValues Init = UWorld::InitializationValues()
		                                              .AllowAudioPlayback(false)
		                                              .CreatePhysicsScene(true)
		                                              .CreateNavigation(false)
		                                              .CreateAISystem(false)
		                                              .ShouldSimulatePhysics(false);
		World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &Init);
		GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
		UPCGSubsystem* Subsystem = World->GetSubsystem<UPCGSubsystem>();
		if (!Subsystem->FindPCGWorldActor())
		{
			Subsystem->RegisterPCGWorldActor(World->SpawnActor<APCGWorldActor>());
		}
	}
	~FPCGTestWorld()
	{
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
	}
	AActor* SpawnActor()
	{
		AActor* Actor = World->SpawnActor<AActor>();
		UBoxComponent* Root = NewObject<UBoxComponent>(Actor);
		Root->SetBoxExtent(FVector(100));
		Actor->AddInstanceComponent(Root);
		Actor->SetRootComponent(Root);
		Root->RegisterComponent();
		return Actor;
	}
	UPCGComponent* AddPCG(AActor* Actor, UPCGGraph* Graph)
	{
		UPCGComponent* Component = NewObject<UPCGComponent>(Actor);
		Actor->AddInstanceComponent(Component);
		Component->bActivated = false;
		Component->GenerationTrigger = EPCGComponentGenerationTrigger::GenerateOnDemand;
		Component->SetGraphLocal(Graph);
		Component->RegisterComponent();
		return Component;
	}
	bool RunGeneration(UPCGComponent* Component)
	{
		Component->bActivated = true;
		if (Component->GenerateLocalGetTaskId(true) == InvalidPCGTaskId)
		{
			return false;
		}
		UPCGSubsystem* Subsystem = World->GetSubsystem<UPCGSubsystem>();
		const double Deadline = FPlatformTime::Seconds() + 15.0;
		while (Component->IsGenerating())
		{
			Subsystem->Tick(1.0f / 60.0f);
			FPlatformProcess::Sleep(0.001f);
			if (FPlatformTime::Seconds() > Deadline)
			{
				Component->CancelGeneration();
				return false;
			}
		}
		return Component->bGenerated;
	}
	UWorld* World;
};

int32 CountInstances(AActor* Actor)
{
	int32 Result = 0;
	TInlineComponentArray<UInstancedStaticMeshComponent*> Components(Actor);
	for (UInstancedStaticMeshComponent* Component : Components)
	{
		Result += Component->GetInstanceCount();
	}
	return Result;
}
} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTPCGReflectedConfigurationTest, "OWT.Runtime.PCGReflectedConfiguration",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTPCGReflectedConfigurationTest::RunTest(const FString& Parameters)
{
	FOWTPCGConfigurationTestSchema Source;
	Source.FutureSetting = TEXT("An authoring field added without a transfer branch");
	Source.FutureWeights = {3, 8, 21};
	Source.FutureReference = GetTransientPackage();
	Source.ObservedValue = 90;
	Source.WorkingValue = 91;
	Source.InternalValue = 93;
	FOWTPCGConfigurationTestSchema Copy;
	OWTPCGConfiguration::CopyAuthoredProperties(*FOWTPCGConfigurationTestSchema::StaticStruct(), &Source, &Copy);
	TestEqual(TEXT("A future authored field is copied from the schema"), Copy.FutureSetting, Source.FutureSetting);
	TestTrue(TEXT("Future authored containers are copied"), Copy.FutureWeights == Source.FutureWeights);
	TestTrue(TEXT("Future reference values remain available for the remapping phase"), Copy.FutureReference == Source.FutureReference);
	TestEqual(TEXT("VisibleAnywhere values are observations, not authored settings"), Copy.ObservedValue, 0);
	TestEqual(TEXT("Transient working values are excluded"), Copy.WorkingValue, 0);
	TestEqual(TEXT("Unexposed internal values are excluded"), Copy.InternalValue, 0);
	Source.FutureWeights[0] = 999;
	TestEqual(TEXT("Configuration container storage is detached"), Copy.FutureWeights[0], 3);

	TStrongObjectPtr<UPCGComponent> SourcePCG(NewObject<UPCGComponent>(GetTransientPackage()));
	TStrongObjectPtr<UPCGComponent> CopiedPCG(NewObject<UPCGComponent>(GetTransientPackage()));
	SourcePCG->Seed = 441;
	SourcePCG->bGenerated = true;
	UPCGGraphInstance* DestinationGraph = CopiedPCG->GetGraphInstance();
	OWTPCGConfiguration::CopyAuthoredProperties(*UPCGComponent::StaticClass(), SourcePCG.Get(), CopiedPCG.Get());
	TestEqual(TEXT("Actual PCG authoring field follows the same transfer"), CopiedPCG->Seed, 441);
	TestFalse(TEXT("Actual PCG generated state is never restored from source"), CopiedPCG->bGenerated);
	TestTrue(TEXT("Owned GraphInstance stays under the engine lifecycle contract"), CopiedPCG->GetGraphInstance() == DestinationGraph);
	return !HasAnyErrors();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTPCGConfigurationTest, "OWT.Runtime.PCGConfiguration",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTPCGConfigurationTest::RunTest(const FString& Parameters)
{
	FPCGTestWorld Fixture;
	TStrongObjectPtr<UPCGGraph> Graph(NewObject<UPCGGraph>(GetTransientPackage()));
	Graph->UpdateUserParametersStruct(
	    [](FInstancedPropertyBag& Bag)
	    {
		    Bag.AddProperty(TEXT("SelectedActor"), EPropertyBagPropertyType::Object, AActor::StaticClass());
	    });
	AActor* Source = Fixture.SpawnActor();
	UPCGComponent* PCG = Fixture.AddPCG(Source, Graph.Get());
	UPCGGraphInstance* OwnedParent = NewObject<UPCGGraphInstance>(PCG);
	OwnedParent->SetGraph(Graph.Get());
	PCG->GetGraphInstance()->SetGraph(OwnedParent);
	AActor* AuthoredChild = Fixture.SpawnActor();
	AuthoredChild->AttachToActor(Source, FAttachmentTransformRules::KeepWorldTransform);
	UPCGComponent* ChildPCG = Fixture.AddPCG(AuthoredChild, Graph.Get());
	FInstancedPropertyBag& ChildBag = ChildPCG->GetGraphInstance()->ParametersOverrides.Parameters;
	ChildBag.SetValueObject(TEXT("SelectedActor"), Source);
	ChildPCG->GetGraphInstance()->UpdatePropertyOverride(
	    ChildBag.GetPropertyBagStruct()->FindPropertyByName(TEXT("SelectedActor")), true);
	PCG->Seed = 823;
	PCG->bParseActorComponents = false;
	PCG->bGenerateOnDropWhenTriggerOnDemand = true;
	PCG->ComponentTags = {TEXT("AuthoredPCG"), TEXT("ReflectedConfiguration")};
	PCG->PostGenerateFunctionNames = {TEXT("AuthoredCompletion")};
	FInstancedPropertyBag& ParametersBag = PCG->GetGraphInstance()->ParametersOverrides.Parameters;
	ParametersBag.SetValueObject(TEXT("SelectedActor"), Source);
	const FProperty* ActorParameter = ParametersBag.GetPropertyBagStruct()->FindPropertyByName(TEXT("SelectedActor"));
	PCG->GetGraphInstance()->UpdatePropertyOverride(ActorParameter, true);
	UInstancedStaticMeshComponent* Generated = NewObject<UInstancedStaticMeshComponent>(Source);
	Source->AddInstanceComponent(Generated);
	Generated->SetupAttachment(Source->GetRootComponent());
	Generated->RegisterComponent();
	PCG->AddComponentsToManagedResources({Generated});
	UOWTRuntimeActorDuplicator* Duplicator = NewObject<UOWTRuntimeActorDuplicator>(Source);
	Duplicator->Initialize(Source);
	FString Error;
	FOWTDuplicationOptions Options;
	AActor* Copy = Duplicator->DuplicateActorWithOptions(Source, Options, FGuid::NewGuid(), Error);
	if (!TestNotNull(*Error, Copy))
	{
		return false;
	}
	UPCGComponent* CopiedPCG = Copy->FindComponentByClass<UPCGComponent>();
	if (!TestNotNull(TEXT("PCG component copied"), CopiedPCG))
	{
		return false;
	}
	TestEqual(TEXT("Seed retained"), CopiedPCG->Seed, 823);
	TestFalse(TEXT("Actor component parsing setting retained"), CopiedPCG->bParseActorComponents);
	TestTrue(TEXT("On-demand placement setting retained"), CopiedPCG->bGenerateOnDropWhenTriggerOnDemand);
	TestTrue(TEXT("Engine restore preserves inherited component tags"), CopiedPCG->ComponentTags == PCG->ComponentTags);
	TestTrue(TEXT("Reflected authoring transfer preserves completion function configuration"),
		CopiedPCG->PostGenerateFunctionNames == PCG->PostGenerateFunctionNames);
	TestTrue(TEXT("External graph shared"), CopiedPCG->GetGraph() == Graph.Get());
	TestTrue(TEXT("GraphInstance independent"), CopiedPCG->GetGraphInstance() != PCG->GetGraphInstance());
	TestTrue(TEXT("Owned parent graph instance independent"), CopiedPCG->GetGraphInstance()->Graph != OwnedParent);
	TestTrue(TEXT("Parent graph instance belongs to destination"),
	         CopiedPCG->GetGraphInstance()->Graph->IsIn(CopiedPCG));
	const auto ActorValue =
	    CopiedPCG->GetGraphInstance()->ParametersOverrides.Parameters.GetValueObject(TEXT("SelectedActor"));
	TestTrue(TEXT("Parameter reference remapped"), ActorValue.HasValue());
	if (ActorValue.HasValue())
	{
		TestTrue(TEXT("Parameter points to duplicate"), ActorValue.GetValue() == Copy);
	}
	TestNull(TEXT("Managed output excluded from authored component copying"),
	         Copy->FindComponentByClass<UInstancedStaticMeshComponent>());
	const auto Observations = Duplicator->GetProceduralComponents();
	TestEqual(TEXT("Both authored PCG components observed"), Observations.Num(), 2);
	if (!Observations.IsEmpty())
	{
		TestEqual(TEXT("Inactive policy retained"), Observations[0].State, EOWTProceduralState::NotRequested);
	}
	CopiedPCG->CleanupLocalImmediate(true, true);
	TestTrue(TEXT("Target cleanup leaves source managed component alive"), IsValid(Generated));
	TArray<AActor*> CopiedChildren;
	Copy->GetAttachedActors(CopiedChildren, true, false);
	TestEqual(TEXT("Authored PCG child copied"), CopiedChildren.Num(), 1);
	if (CopiedChildren.Num() == 1)
	{
		UPCGComponent* CopiedChildPCG = CopiedChildren[0]->FindComponentByClass<UPCGComponent>();
		TestNotNull(TEXT("Child PCG restored"), CopiedChildPCG);
		if (CopiedChildPCG)
		{
			const auto ChildActorValue =
			    CopiedChildPCG->GetGraphInstance()->ParametersOverrides.Parameters.GetValueObject(
			        TEXT("SelectedActor"));
			TestTrue(TEXT("Cross-actor parameter targets copied root"),
			         ChildActorValue.HasValue() && ChildActorValue.GetValue() == Copy);
		}
	}
	bool bCommitted = false;
	Options.OnAuthoredCommitted = [&bCommitted](AActor*)
	{
		bCommitted = true;
	};
	Duplicator->OnProceduralChanged.AddLambda(
	    [&](const FOWTProceduralComponentSnapshot&)
	    {
		    TestTrue(TEXT("Authored commit precedes procedural notification"), bCommitted);
		    Duplicator->Deinitialize();
	    });
	AActor* ShutdownCopy = Duplicator->DuplicateActorWithOptions(Source, Options, FGuid::NewGuid(), Error);
	TestNotNull(TEXT("Observer shutdown cannot roll back authored commit"), ShutdownCopy);
	Duplicator->Tick(0.0f);
	TestEqual(TEXT("Reentrant shutdown detaches observations"), Duplicator->GetProceduralComponents().Num(), 0);
	Duplicator->Deinitialize();
	PCG->CleanupLocalImmediate(true, true);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTPCGRegenerationTest, "OWT.Runtime.PCGRegeneration",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTPCGRegenerationTest::RunTest(const FString& Parameters)
{
	FPCGTestWorld Fixture;
	TStrongObjectPtr<UPCGGraph> Graph(NewObject<UPCGGraph>(GetTransientPackage()));
	UPCGCreatePointsSettings* Points = nullptr;
	UPCGNode* PointNode = Graph->AddNodeOfType<UPCGCreatePointsSettings>(Points);
	Points->CoordinateSpace = EPCGCoordinateSpace::OriginalComponent;
	Points->PointsToCreate.SetNum(2);
	Points->PointsToCreate[1].Transform.SetLocation(FVector(50, 0, 0));
	UPCGStaticMeshSpawnerSettings* Spawner = nullptr;
	UPCGNode* SpawnerNode = Graph->AddNodeOfType<UPCGStaticMeshSpawnerSettings>(Spawner);
	Spawner->SetMeshSelectorType(UPCGMeshSelectorWeighted::StaticClass());
	Spawner->bSynchronousLoad = true;
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (!TestNotNull(TEXT("CPU PCG fixture mesh"), Mesh))
	{
		return false;
	}
	CastChecked<UPCGMeshSelectorWeighted>(Spawner->MeshSelectorParameters)
	    ->MeshEntries.Add(FPCGMeshSelectorWeightedEntry(Mesh, 1));
	Graph->AddEdge(PointNode, PCGPinConstants::DefaultOutputLabel, SpawnerNode, PCGPinConstants::DefaultInputLabel);
	Graph->AddEdge(SpawnerNode, PCGPinConstants::DefaultOutputLabel, Graph->GetOutputNode(),
	               PCGPinConstants::DefaultOutputLabel);
	AActor* Source = Fixture.SpawnActor();
	UPCGComponent* PCG = Fixture.AddPCG(Source, Graph.Get());
	if (!TestTrue(TEXT("Source CPU graph generated"), Fixture.RunGeneration(PCG)))
	{
		return false;
	}
	TestEqual(TEXT("Source generated two instances"), CountInstances(Source), 2);
	UOWTRuntimeActorDuplicator* Duplicator = NewObject<UOWTRuntimeActorDuplicator>(Source);
	Duplicator->Initialize(Source);
	FOWTDuplicationOptions Options;
	Options.WorldOffset = FVector(500, 0, 0);
	FString Error;
	FOWTDuplicationOptions NoGenerateOptions;
	NoGenerateOptions.GenerationPolicy = EOWTDuplicationGenerationPolicy::KeepUnGenerated;
	AActor* UnGeneratedCopy = Duplicator->DuplicateActorWithOptions(Source, NoGenerateOptions, FGuid::NewGuid(), Error);
	if (!TestNotNull(TEXT("OnDemand configuration-only copy"), UnGeneratedCopy))
	{
		AddError(Error);
		return false;
	}
	TestEqual(TEXT("KeepUnGenerated does not reuse source output"), CountInstances(UnGeneratedCopy), 0);
	TestFalse(TEXT("OnDemand target remains ungenerated"),
	          UnGeneratedCopy->FindComponentByClass<UPCGComponent>()->bGenerated);
	AActor* Copy = Duplicator->DuplicateActorWithOptions(Source, Options, FGuid::NewGuid(), Error);
	if (!TestNotNull(*Error, Copy))
	{
		return false;
	}
	UPCGComponent* CopiedPCG = Copy->FindComponentByClass<UPCGComponent>();
	const double Deadline = FPlatformTime::Seconds() + 15.0;
	while (CopiedPCG->IsGenerating())
	{
		Fixture.World->GetSubsystem<UPCGSubsystem>()->Tick(1.0f / 60.0f);
		FPlatformProcess::Sleep(0.001f);
		if (FPlatformTime::Seconds() > Deadline)
		{
			AddError(TEXT("Duplicate generation timed out."));
			break;
		}
	}
	TestTrue(TEXT("Duplicate generated independently"), CopiedPCG->bGenerated);
	TestEqual(TEXT("Duplicate has exactly two fresh instances"), CountInstances(Copy), 2);
	PCG->CleanupLocalImmediate(true, true);
	TestEqual(TEXT("Source cleanup leaves duplicate instances"), CountInstances(Copy), 2);
	TestEqual(TEXT("Source cleanup removes original instances"), CountInstances(Source), 0);
	CopiedPCG->CleanupLocalImmediate(true, true);
	TestEqual(TEXT("Target cleanup removes its instances"), CountInstances(Copy), 0);
	Duplicator->Deinitialize();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTPCGRuntimePolicyTest, "OWT.Runtime.PCGRuntimePolicy",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTPCGRuntimePolicyTest::RunTest(const FString& Parameters)
{
	FPCGTestWorld Fixture;
	TStrongObjectPtr<UPCGGraph> Graph(NewObject<UPCGGraph>(GetTransientPackage()));
	UPCGCreatePointsSettings* Points = nullptr;
	UPCGNode* PointNode = Graph->AddNodeOfType<UPCGCreatePointsSettings>(Points);
	Points->PointsToCreate.SetNum(1);
	Graph->AddEdge(PointNode, PCGPinConstants::DefaultOutputLabel, Graph->GetOutputNode(),
	               PCGPinConstants::DefaultOutputLabel);
	AActor* Source = Fixture.SpawnActor();
	UPCGComponent* PCG = Fixture.AddPCG(Source, Graph.Get());
	PCG->bActivated = true;
	PCG->GenerationTrigger = EPCGComponentGenerationTrigger::GenerateAtRuntime;
	UOWTRuntimeActorDuplicator* Duplicator = NewObject<UOWTRuntimeActorDuplicator>(Source);
	Duplicator->Initialize(Source);
	FString Error;
	FOWTDuplicationOptions Options;
	UPCGSubsystem* Subsystem = Fixture.World->GetSubsystem<UPCGSubsystem>();
	APCGWorldActor* WorldActor = Subsystem->FindPCGWorldActor();
	Subsystem->UnregisterPCGWorldActor(WorldActor);
	TestNull(TEXT("Runtime scheduling requires world configuration"),
	         Duplicator->DuplicateActorWithOptions(Source, Options, FGuid::NewGuid(), Error));
	TestTrue(TEXT("Missing world actor is distinguished from waiting for a source"),
	         Error.Contains(TEXT("MissingPCGWorldActor")));
	Subsystem->RegisterPCGWorldActor(WorldActor);
	AActor* Copy = Duplicator->DuplicateActorWithOptions(Source, Options, FGuid::NewGuid(), Error);
	if (!TestNotNull(*Error, Copy))
	{
		return false;
	}
	const auto Observations = Duplicator->GetProceduralComponents();
	TestEqual(TEXT("Runtime component observed"), Observations.Num(), 1);
	if (!Observations.IsEmpty())
	{
		TestEqual(TEXT("No generation source is a normal waiting state"), Observations[0].State,
		          EOWTProceduralState::WaitingForGenerationSource);
	}
	PCG->GenerationTrigger = EPCGComponentGenerationTrigger::GenerateOnDemand;
	TestTrue(TEXT("Source starts a pending generation task"), PCG->GenerateLocalGetTaskId(true) != InvalidPCGTaskId);
	AActor* BusyCopy = Duplicator->DuplicateActorWithOptions(Source, Options, FGuid::NewGuid(), Error);
	TestNull(TEXT("Generating source rejects duplication"), BusyCopy);
	TestTrue(TEXT("Busy cause is diagnostic"), Error.Contains(TEXT("SourceBusy")));
	TestTrue(TEXT("Source generation was not cancelled"), PCG->IsGenerating());
	PCG->CancelGeneration();
	PCG->CleanupLocalImmediate(true, true);
	Copy->FindComponentByClass<UPCGComponent>()->bActivated = false;
	Copy->FindComponentByClass<UPCGComponent>()->CleanupLocalImmediate(true, true);
	Duplicator->Deinitialize();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTPCGSchedulerTest, "OWT.Runtime.PCGSchedulerHierarchy",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTPCGSchedulerTest::RunTest(const FString& Parameters)
{
	for (int32 Case = 0; Case < 3; ++Case)
	{
		FPCGTestWorld Fixture;
		UPCGSubsystem* Subsystem = Fixture.World->GetSubsystem<UPCGSubsystem>();
		TStrongObjectPtr<UOWTPCGTestGraph> Graph(NewObject<UOWTPCGTestGraph>(GetTransientPackage()));
		if (Case == 2)
		{
			Graph->ConfigureHierarchicalGeneration();
		}
		UPCGCreatePointsSettings* Points = nullptr;
		UPCGNode* PointNode = Graph->AddNodeOfType<UPCGCreatePointsSettings>(Points);
		Points->CoordinateSpace = EPCGCoordinateSpace::OriginalComponent;
		Points->PointsToCreate.SetNum(2);
		Points->PointsToCreate[1].Transform.SetLocation(FVector(40, 0, 0));
		Points->bCullPointsOutsideVolume = true;
		UPCGStaticMeshSpawnerSettings* Spawner = nullptr;
		UPCGNode* SpawnerNode = Graph->AddNodeOfType<UPCGStaticMeshSpawnerSettings>(Spawner);
		Spawner->SetMeshSelectorType(UPCGMeshSelectorWeighted::StaticClass());
		Spawner->bSynchronousLoad = true;
		UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
		CastChecked<UPCGMeshSelectorWeighted>(Spawner->MeshSelectorParameters)
		    ->MeshEntries.Add(FPCGMeshSelectorWeightedEntry(Mesh, 1));
		Graph->AddEdge(PointNode, PCGPinConstants::DefaultOutputLabel, SpawnerNode, PCGPinConstants::DefaultInputLabel);
		Graph->AddEdge(SpawnerNode, PCGPinConstants::DefaultOutputLabel, Graph->GetOutputNode(),
		               PCGPinConstants::DefaultOutputLabel);
		AActor* Source = Fixture.SpawnActor();
		Source->SetActorLocation(FVector(1600, 1600, 0));
		UPCGComponent* PCG = Fixture.AddPCG(Source, Graph.Get());
		Subsystem->UnregisterPCGComponent(PCG, true);
		PCG->GenerationTrigger = EPCGComponentGenerationTrigger::GenerateAtRuntime;
		PCG->SetSchedulingPolicyClass(PCG->SchedulingPolicyClass);
		PCG->bOverrideGenerationRadii = true;
		PCG->GenerationRadii.GenerationRadius = 1000.0;
		PCG->bIsComponentPartitioned = Case > 0;
		PCG->bActivated = true;
		Subsystem->RegisterOrUpdatePCGComponent(PCG);
		UOWTRuntimeActorDuplicator* Duplicator = NewObject<UOWTRuntimeActorDuplicator>(Source);
		Duplicator->Initialize(Source);
		FOWTDuplicationOptions Options;
		Options.WorldOffset = FVector(400, 0, 0);
		FString Error;
		AActor* Copy = Duplicator->DuplicateActorWithOptions(Source, Options, FGuid::NewGuid(), Error);
		if (!TestNotNull(*FString::Printf(TEXT("Runtime scheduler case %d: %s"), Case, *Error), Copy))
		{
			return false;
		}
		UPCGComponent* CopiedPCG = Copy->FindComponentByClass<UPCGComponent>();
		AActor* Generator = Fixture.SpawnActor();
		Generator->SetActorLocation(Source->GetActorLocation());
		UPCGGenSourceComponent* GenerationSource = NewObject<UPCGGenSourceComponent>(Generator);
		Generator->AddInstanceComponent(GenerationSource);
		GenerationSource->RegisterComponent();
		Subsystem->GetGenSourceManager()->RegisterGenSource(GenerationSource);
		auto CountManaged = [Subsystem](UPCGComponent* Original)
		{
			int32 Count = 0;
			TSet<const UInstancedStaticMeshComponent*> Counted;
			auto CountLocal = [&Count, &Counted](UPCGComponent* Component)
			{
				Component->ForEachConstManagedResource(
				    [&Count, &Counted](const UPCGManagedResource* Resource)
				    {
					    if (const UPCGManagedISMComponent* ISM = Cast<UPCGManagedISMComponent>(Resource))
					    {
						    const UInstancedStaticMeshComponent* Instances = ISM->GetComponent();
						    if (Instances)
						    {
							    if (!Counted.Contains(Instances))
							    {
								    Counted.Add(Instances);
								    Count += Instances->GetInstanceCount();
							    }
						    }
					    }
				    });
			};
			CountLocal(Original);
			Subsystem->ForAllRegisteredLocalComponents(Original, CountLocal);
			return Count;
		};
		auto HasState = [Duplicator](EOWTProceduralState State)
		{
			const auto Snapshots = Duplicator->GetProceduralComponents();
			if (Snapshots.Num() != 1)
			{
				return false;
			}
			return Snapshots[0].State == State;
		};
		auto PumpUntil = [&](TFunctionRef<bool()> Complete)
		{
			const double Deadline = FPlatformTime::Seconds() + 15.0;
			while (!Complete())
			{
				Subsystem->Tick(1.0f / 60.0f);
				Duplicator->Tick(1.0f / 60.0f);
				FPlatformProcess::Sleep(0.001f);
				if (FPlatformTime::Seconds() > Deadline)
				{
					AddError(FString::Printf(TEXT("Runtime scheduler case %d timed out: source=%d target=%d."), Case,
					                         CountManaged(PCG), CountManaged(CopiedPCG)));
					return false;
				}
			}
			return true;
		};
		PumpUntil(
		    [&]()
		    {
			    if (CountManaged(PCG) == 0)
			    {
				    return false;
			    }
			    if (CountManaged(CopiedPCG) == 0)
			    {
				    return false;
			    }
			    return HasState(EOWTProceduralState::Ready);
		    });
		TestEqual(*FString::Printf(TEXT("Runtime case %d source resources"), Case), CountManaged(PCG), 2);
		TestEqual(*FString::Printf(TEXT("Runtime case %d independent target resources"), Case), CountManaged(CopiedPCG),
		          2);
		const auto Observations = Duplicator->GetProceduralComponents();
		TestEqual(TEXT("Runtime scheduler target observed"), Observations.Num(), 1);
		if (Observations.Num() == 1)
		{
			TestEqual(*FString::Printf(TEXT("Runtime case %d reports ready after generation"), Case),
			          Observations[0].State, EOWTProceduralState::Ready);
			TestTrue(TEXT("Runtime generation attempts were observed"), Observations[0].GenerationAttempt > 0);
		}
		const int32 FirstAttemptCount = Observations.IsEmpty() ? 0 : Observations[0].GenerationAttempt;
		Generator->SetActorLocation(FVector(10000000));
		PumpUntil(
		    [&]()
		    {
			    if (CountManaged(PCG) != 0)
			    {
				    return false;
			    }
			    if (CountManaged(CopiedPCG) != 0)
			    {
				    return false;
			    }
			    return HasState(EOWTProceduralState::Cleaned);
		    });
		TestTrue(TEXT("Moving the generation source reports runtime cleanup"), HasState(EOWTProceduralState::Cleaned));
		Generator->SetActorLocation(Source->GetActorLocation());
		PumpUntil(
		    [&]()
		    {
			    if (CountManaged(PCG) != 2)
			    {
				    return false;
			    }
			    if (CountManaged(CopiedPCG) != 2)
			    {
				    return false;
			    }
			    return HasState(EOWTProceduralState::Ready);
		    });
		const auto Regenerated = Duplicator->GetProceduralComponents();
		if (Regenerated.Num() == 1)
		{
			TestTrue(TEXT("A returning generation source starts a new observed attempt"),
			         Regenerated[0].GenerationAttempt > FirstAttemptCount);
		}
		CopiedPCG->bActivated = false;
		CopiedPCG->CleanupLocalImmediate(true, true);
		TestEqual(TEXT("Runtime target cleanup does not remove source resources"), CountManaged(PCG), 2);
		PCG->bActivated = false;
		PCG->CleanupLocalImmediate(true, true);
		Subsystem->GetGenSourceManager()->UnregisterGenSource(GenerationSource);
		Duplicator->Deinitialize();
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTPCGPolicyReferencesTest, "OWT.Runtime.PCGPolicyReferences",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTPCGPolicyReferencesTest::RunTest(const FString& Parameters)
{
	FPCGTestWorld Fixture;
	TStrongObjectPtr<UPCGGraph> Graph(NewObject<UPCGGraph>(GetTransientPackage()));
	AActor* Source = Fixture.SpawnActor();
	AActor* Child = Fixture.SpawnActor();
	Child->AttachToActor(Source, FAttachmentTransformRules::KeepWorldTransform);
	UPCGComponent* PCG = Fixture.AddPCG(Source, Graph.Get());
	UPCGComponent* ChildPCG = Fixture.AddPCG(Child, Graph.Get());
	PCG->GenerationTrigger = EPCGComponentGenerationTrigger::GenerateAtRuntime;
	PCG->SetSchedulingPolicyClass(UOWTPCGTestPolicy::StaticClass());
	UOWTPCGTestPolicy* Policy = CastChecked<UOWTPCGTestPolicy>(PCG->SchedulingPolicy);
	Policy->Actor = Child;
	Policy->Data = NewObject<UOWTPCGTestPolicyData>(Policy);
	Policy->Data->Actor = Source;
	Policy->Data->Component = ChildPCG;
	UOWTRuntimeActorDuplicator* Duplicator = NewObject<UOWTRuntimeActorDuplicator>(Source);
	Duplicator->Initialize(Source);
	FString Error;
	AActor* Copy = Duplicator->DuplicateActorWithOptions(Source, FOWTDuplicationOptions(), FGuid::NewGuid(), Error);
	if (!TestNotNull(*Error, Copy))
	{
		return false;
	}
	UPCGComponent* CopiedPCG = Copy->FindComponentByClass<UPCGComponent>();
	UOWTPCGTestPolicy* CopiedPolicy = Cast<UOWTPCGTestPolicy>(CopiedPCG->SchedulingPolicy);
	if (!TestNotNull(TEXT("Derived scheduling policy retained"), CopiedPolicy))
	{
		return false;
	}
	TArray<AActor*> Children;
	Copy->GetAttachedActors(Children, true, false);
	TestEqual(TEXT("Policy's authored child copied"), Children.Num(), 1);
	TestTrue(TEXT("Policy belongs to target component"), CopiedPolicy->IsIn(CopiedPCG));
	TestTrue(TEXT("Instanced policy data independently owned"), CopiedPolicy->Data != Policy->Data);
	TestTrue(TEXT("Instanced data belongs to target policy"), CopiedPolicy->Data->IsIn(CopiedPolicy));
	TestTrue(TEXT("Nested policy Actor reference remapped"), CopiedPolicy->Data->Actor == Copy);
	if (Children.Num() == 1)
	{
		TestTrue(TEXT("Policy Actor reference remapped across hierarchy"), CopiedPolicy->Actor == Children[0]);
		TestTrue(TEXT("Nested policy component reference remapped"),
		         CopiedPolicy->Data->Component == Children[0]->FindComponentByClass<UPCGComponent>());
	}
	TestTrue(TEXT("Source policy Actor unchanged"), Policy->Actor == Child);
	TestTrue(TEXT("Source nested reference unchanged"), Policy->Data->Actor == Source);
	PCG->SetSchedulingPolicyClass(UOWTPCGTestOpaquePolicy::StaticClass());
	TestNull(TEXT("Opaque custom policy requires an explicit adapter"),
	         Duplicator->DuplicateActorWithOptions(Source, FOWTDuplicationOptions(), FGuid::NewGuid(), Error));
	TestTrue(TEXT("Unsupported policy payload reports its property"), Error.Contains(TEXT("Opaque")));
	Duplicator->Deinitialize();
	return true;
}
#endif
