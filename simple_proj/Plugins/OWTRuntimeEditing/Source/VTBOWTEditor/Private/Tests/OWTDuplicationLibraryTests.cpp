#if WITH_DEV_AUTOMATION_TESTS

#include "Duplication/OWTDuplicationLibrary.h"
#include "Duplication/OWTDuplicationSubsystem.h"
#include "Duplication/OWTRuntimeActorDuplicator.h"
#include "Engine/Engine.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/DefaultPawn.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"

static int32 CountLibraryTestActors(UWorld& World)
{
	int32 Count = 0;
	for (TActorIterator<AActor> It(&World); It; ++It)
	{
		if (!It->IsActorBeingDestroyed())
		{
			++Count;
		}
	}
	return Count;
}

static FOWTSourceDecision RejectInLibraryTest(const UObject& Object)
{
	return FOWTSourceDecision::Reject(TEXT("LibraryTestRejected"));
}

static FOWTSourceDecision AllowInLibraryTest(const UObject& Object)
{
	return FOWTSourceDecision::Allow();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTDuplicationLibraryTest, "OWT.Runtime.DuplicationLibrary",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTDuplicationLibraryTest::RunTest(const FString& Parameters)
{
	const UWorld::InitializationValues Init = UWorld::InitializationValues()
	                                              .AllowAudioPlayback(false)
	                                              .CreatePhysicsScene(true)
	                                              .CreateNavigation(false)
	                                              .CreateAISystem(false)
	                                              .ShouldSimulatePhysics(false);
	UWorld* World =
	    UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &Init);
	if (!TestNotNull(TEXT("Library test world"), World))
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	ON_SCOPE_EXIT
	{
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
	};

	AStaticMeshActor* Source = World->SpawnActor<AStaticMeshActor>(FVector(10.0, 20.0, 30.0), FRotator::ZeroRotator);
	if (!TestNotNull(TEXT("Library source"), Source))
	{
		return false;
	}
	Source->Tags.Add(TEXT("LibraryAuthoredTag"));
	UOWTDuplicationSubsystem* Subsystem = UOWTDuplicationLibrary::GetSubsystem(Source);
	if (!TestNotNull(TEXT("Game worlds host the duplication subsystem"), Subsystem))
	{
		return false;
	}

	FString Error;
	const int32 ActorsBeforePreflight = CountLibraryTestActors(*World);
	TestTrue(TEXT("Preflight accepts a plain static mesh Actor"),
	         UOWTDuplicationLibrary::CanDuplicateActor(Source, FOWTDuplicationOptions(), Error));
	TestEqual(TEXT("Preflight spawns nothing"), CountLibraryTestActors(*World), ActorsBeforePreflight);

	const FVector Offset(200.0, 0.0, 0.0);
	const FOWTDuplicationResult Result = UOWTDuplicationLibrary::DuplicateActorAtOffset(Source, Offset);
	if (!TestTrue(*FString::Printf(TEXT("One-call duplication succeeds: %s"), *Result.Error), Result.bSucceeded))
	{
		return false;
	}
	TestNotNull(TEXT("Result carries the duplicate"), Result.Duplicate.Get());
	TestTrue(TEXT("Result carries an operation id"), Result.OperationId.IsValid());
	TestEqual(TEXT("A single Actor hierarchy creates one Actor"), Result.CreatedActors.Num(), 1);
	TestTrue(TEXT("CreatedActors starts with the duplicate"),
	         Result.CreatedActors.Num() > 0 && Result.CreatedActors[0] == Result.Duplicate);
	TestTrue(TEXT("Offset is applied once"),
	         Result.Duplicate->GetActorLocation().Equals(Source->GetActorLocation() + Offset));
	TestTrue(TEXT("Authored tags are copied"), Result.Duplicate->Tags.Contains(TEXT("LibraryAuthoredTag")));

	FOWTDuplicationSourceRuleSet& Rules = Subsystem->GetDuplicator()->GetAdapterRegistry().GetSourceRules();
	TestTrue(TEXT("An application rule registers for a concrete class"),
	         Rules.Register(AStaticMeshActor::StaticClass(), RejectInLibraryTest));
	const int32 ActorsBeforeRejection = CountLibraryTestActors(*World);
	TestFalse(TEXT("Preflight reports the application rejection"),
	          UOWTDuplicationLibrary::CanDuplicateActor(Source, FOWTDuplicationOptions(), Error));
	TestEqual(TEXT("Rejection reason is the rule's reason"), Error, FString(TEXT("LibraryTestRejected")));
	const FOWTDuplicationResult Rejected = UOWTDuplicationLibrary::DuplicateActorAtOffset(Source, Offset);
	TestFalse(TEXT("Duplication honors the application rejection"), Rejected.bSucceeded);
	TestNull(TEXT("A rejected request has no duplicate"), Rejected.Duplicate.Get());
	TestEqual(TEXT("A rejected request leaves no Actor"), CountLibraryTestActors(*World), ActorsBeforeRejection);
	Rules.Unregister(AStaticMeshActor::StaticClass());
	TestTrue(TEXT("Removing the rule restores the default decision"),
	         UOWTDuplicationLibrary::CanDuplicateActor(Source, FOWTDuplicationOptions(), Error));

	ADefaultPawn* Pawn = World->SpawnActor<ADefaultPawn>();
	if (!TestNotNull(TEXT("Pawn source"), Pawn))
	{
		return false;
	}
	Pawn->AutoPossessAI = EAutoPossessAI::PlacedInWorld;
	TestFalse(TEXT("Built-in rule rejects automatic AI possession"),
	          UOWTDuplicationLibrary::CanDuplicateActor(Pawn, FOWTDuplicationOptions(), Error));
	TestTrue(TEXT("Built-in rejection explains the possession requirement"), Error.Contains(TEXT("possession")));
	TestTrue(TEXT("A subclass rule can override a built-in rejection"),
	         Rules.Register(ADefaultPawn::StaticClass(), AllowInLibraryTest));
	TestTrue(*FString::Printf(TEXT("The overriding rule admits the Pawn: %s"), *Error),
	         UOWTDuplicationLibrary::CanDuplicateActor(Pawn, FOWTDuplicationOptions(), Error));
	Rules.Unregister(ADefaultPawn::StaticClass());

	TestFalse(TEXT("A null source is rejected without a world lookup"),
	          UOWTDuplicationLibrary::DuplicateActor(nullptr, FOWTDuplicationOptions()).bSucceeded);
	return true;
}

#endif
