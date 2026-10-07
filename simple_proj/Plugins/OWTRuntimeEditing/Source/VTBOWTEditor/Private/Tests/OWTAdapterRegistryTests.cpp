#if WITH_DEV_AUTOMATION_TESTS

#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Duplication/OWTDuplicationAdapter.h"
#include "Duplication/OWTRuntimeActorDuplicator.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "PCGComponent.h"
#include "UObject/UnrealType.h"

namespace
{
struct FAdapterProbe
{
	FAdapterProbe()
	    : ObjectValidations(0), ObjectCaptures(0), ActorValidations(0), ActorCaptures(0), Commits(0), bOwnsTags(false),
	      ValidationFailure()
	{
	}
	int32 ObjectValidations;
	int32 ObjectCaptures;
	int32 ActorValidations;
	int32 ActorCaptures;
	int32 Commits;
	bool bOwnsTags;
	FString ValidationFailure;
};

class FProbeAdapter : public IOWTDuplicationAdapter
{
public:
	explicit FProbeAdapter(TSharedRef<FAdapterProbe> InProbe) : Probe(MoveTemp(InProbe))
	{
	}
	virtual bool ValidateObject(const UObject& Object, FString& Error) const override
	{
		++Probe->ObjectValidations;
		if (!Probe->ValidationFailure.IsEmpty())
		{
			Error = Probe->ValidationFailure;
			return false;
		}
		return IOWTDuplicationAdapter::ValidateObject(Object, Error);
	}
	virtual bool CaptureObject(UObject& Object, FString& Error) override
	{
		++Probe->ObjectCaptures;
		return IOWTDuplicationAdapter::CaptureObject(Object, Error);
	}
	virtual bool ValidateActor(const AActor& Actor, FString& Error) const override
	{
		++Probe->ActorValidations;
		return true;
	}
	virtual bool CaptureActor(AActor& Actor, FString& Error) override
	{
		++Probe->ActorCaptures;
		return true;
	}
	virtual bool OwnsProperty(const UObject& Object, const FProperty& Property) const override
	{
		if (!Probe->bOwnsTags)
		{
			return false;
		}
		return Property.GetFName() == GET_MEMBER_NAME_CHECKED(AActor, Tags);
	}
	virtual void Commit(const FGuid& OperationId, const FOWTDuplicationOptions& Options,
	                    const TMap<UObject*, UObject*>& Mapping) override
	{
		++Probe->Commits;
	}

private:
	TSharedRef<FAdapterProbe> Probe;
};

FOWTDuplicationAdapterRegistry::FFactory ProbeFactory(TSharedRef<FAdapterProbe> Probe)
{
	return [Probe]() -> TUniquePtr<IOWTDuplicationAdapter>
	{
		return MakeUnique<FProbeAdapter>(Probe);
	};
}

int32 CountApplicable(const TArray<TUniquePtr<IOWTDuplicationAdapter>>& Adapters, const UObject& Object,
                      EOWTDuplicationAdapterRole Role)
{
	int32 Count = 0;
	for (const auto& Adapter : Adapters)
	{
		if (Adapter->IsAuxiliary() != (Role == EOWTDuplicationAdapterRole::Auxiliary))
		{
			continue;
		}
		if (Adapter->AcceptsObject(Object))
		{
			++Count;
		}
	}
	return Count;
}

int32 CountLiveActors(UWorld& World)
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
} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTAdapterRegistryResolutionTest, "OWT.Runtime.AdapterRegistryResolution",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTAdapterRegistryResolutionTest::RunTest(const FString& Parameters)
{
	const UActorComponent* BaseObject = GetDefault<UActorComponent>();
	USceneComponent* DerivedObject = NewObject<USceneComponent>();
	UBoxComponent* GrandchildObject = NewObject<UBoxComponent>();
	const UObject* UnrelatedObject = GetDefault<UObject>();
	for (int32 Order = 0; Order < 2; ++Order)
	{
		FOWTDuplicationAdapterRegistry Registry;
		const TSharedRef<FAdapterProbe> BaseProbe = MakeShared<FAdapterProbe>();
		const TSharedRef<FAdapterProbe> DerivedProbe = MakeShared<FAdapterProbe>();
		auto RegisterBase = [&]()
		{
			TestTrue(TEXT("Base primary registers"),
			         Registry.Register(UActorComponent::StaticClass(), ProbeFactory(BaseProbe)));
		};
		auto RegisterDerived = [&]()
		{
			TestTrue(TEXT("Derived primary registers"),
			         Registry.Register(USceneComponent::StaticClass(), ProbeFactory(DerivedProbe)));
		};
		if (Order == 0)
		{
			RegisterBase();
			RegisterDerived();
		}
		else
		{
			RegisterDerived();
			RegisterBase();
		}
		TestFalse(TEXT("Same class and primary role cannot overwrite an existing registration"),
		          Registry.Register(USceneComponent::StaticClass(), ProbeFactory(BaseProbe)));
		TestTrue(TEXT("A primary and auxiliary may share a class"),
		         Registry.Register(USceneComponent::StaticClass(), ProbeFactory(BaseProbe),
		                           EOWTDuplicationAdapterRole::Auxiliary));
		TestTrue(TEXT("Base auxiliary registers independently"),
		         Registry.Register(UActorComponent::StaticClass(), ProbeFactory(BaseProbe),
		                           EOWTDuplicationAdapterRole::Auxiliary));
		TestFalse(TEXT("Same class and auxiliary role cannot overwrite an existing registration"),
		          Registry.Register(USceneComponent::StaticClass(), ProbeFactory(DerivedProbe),
		                            EOWTDuplicationAdapterRole::Auxiliary));
		const auto Adapters = Registry.CreateAdapters();
		TestEqual(TEXT("Base object has one primary"),
		          CountApplicable(Adapters, *BaseObject, EOWTDuplicationAdapterRole::Primary), 1);
		TestEqual(TEXT("Derived object has one primary"),
		          CountApplicable(Adapters, *DerivedObject, EOWTDuplicationAdapterRole::Primary), 1);
		TestEqual(TEXT("Grandchild inherits one most-specific primary"),
		          CountApplicable(Adapters, *GrandchildObject, EOWTDuplicationAdapterRole::Primary), 1);
		TestEqual(TEXT("All matching auxiliary policies remain applicable"),
		          CountApplicable(Adapters, *GrandchildObject, EOWTDuplicationAdapterRole::Auxiliary), 2);
		TestEqual(TEXT("Unrelated object has no primary"),
		          CountApplicable(Adapters, *UnrelatedObject, EOWTDuplicationAdapterRole::Primary), 0);
		FString Error;
		for (const auto& Adapter : Adapters)
		{
			if (Adapter->IsAuxiliary())
			{
				continue;
			}
			if (Adapter->AcceptsObject(*GrandchildObject))
			{
				Adapter->ValidateObject(*GrandchildObject, Error);
			}
		}
		TestEqual(TEXT("Registration order never dispatches the shadowed base primary"), BaseProbe->ObjectValidations,
		          0);
		TestEqual(TEXT("Most-specific primary receives grandchild dispatch"), DerivedProbe->ObjectValidations, 1);
		Registry.Unregister(USceneComponent::StaticClass());
		const auto AfterUnregister = Registry.CreateAdapters();
		TestEqual(TEXT("Unregistering a primary retains the same-class auxiliary"),
		          CountApplicable(AfterUnregister, *GrandchildObject, EOWTDuplicationAdapterRole::Auxiliary), 2);
		TestEqual(TEXT("The base primary becomes applicable after derived unregister"),
		          CountApplicable(AfterUnregister, *GrandchildObject, EOWTDuplicationAdapterRole::Primary), 1);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTAdapterRegistryDispatchTest, "OWT.Runtime.AdapterRegistryDispatch",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTAdapterRegistryDispatchTest::RunTest(const FString& Parameters)
{
	const UWorld::InitializationValues Init = UWorld::InitializationValues()
	                                              .AllowAudioPlayback(false)
	                                              .CreatePhysicsScene(true)
	                                              .CreateNavigation(false)
	                                              .CreateAISystem(false)
	                                              .ShouldSimulatePhysics(false);
	UWorld* World =
	    UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &Init);
	if (!TestNotNull(TEXT("Adapter test world"), World))
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	ON_SCOPE_EXIT
	{
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
	};
	AActor* Source = World->SpawnActor<AActor>();
	Source->Tags.Add(TEXT("AuthoredTag"));
	UOWTRuntimeActorDuplicator* Duplicator = NewObject<UOWTRuntimeActorDuplicator>(Source);
	if (!TestTrue(TEXT("Owner-bound duplicator initializes"), Duplicator->Initialize(Source)))
	{
		return false;
	}
	ON_SCOPE_EXIT
	{
		Duplicator->Deinitialize();
	};
	FOWTDuplicationAdapterRegistry& Registry = Duplicator->GetAdapterRegistry();
	const TSharedRef<FAdapterProbe> Primary = MakeShared<FAdapterProbe>();
	const TSharedRef<FAdapterProbe> Auxiliary = MakeShared<FAdapterProbe>();
	TestTrue(TEXT("Actor primary registers with built-in component policies"),
	         Registry.Register(AActor::StaticClass(), ProbeFactory(Primary)));
	TestTrue(TEXT("Actor auxiliary registers alongside its primary"),
	         Registry.Register(AActor::StaticClass(), ProbeFactory(Auxiliary), EOWTDuplicationAdapterRole::Auxiliary));
	FString Error;
	AActor* Duplicate = Duplicator->DuplicateActor(Source, FVector::ZeroVector, Error);
	TestNotNull(*FString::Printf(TEXT("Primary and auxiliary jointly allow duplication: %s"), *Error), Duplicate);
	TestEqual(TEXT("Primary receives Actor validation through the object bridge"), Primary->ActorValidations, 1);
	TestEqual(TEXT("Primary receives Actor capture through the object bridge"), Primary->ActorCaptures, 1);
	TestEqual(TEXT("Auxiliary validates the same Actor"), Auxiliary->ActorValidations, 1);
	TestEqual(TEXT("Auxiliary captures the same Actor"), Auxiliary->ActorCaptures, 1);
	TestEqual(TEXT("Primary commit executes once"), Primary->Commits, 1);
	TestEqual(TEXT("Auxiliary commit executes once"), Auxiliary->Commits, 1);
	Registry.Unregister(AActor::StaticClass(), EOWTDuplicationAdapterRole::Auxiliary);
	const TSharedRef<FAdapterProbe> ConflictingAuxiliary = MakeShared<FAdapterProbe>();
	ConflictingAuxiliary->bOwnsTags = true;
	Registry.Register(AActor::StaticClass(), ProbeFactory(ConflictingAuxiliary), EOWTDuplicationAdapterRole::Auxiliary);
	const int32 ActorsBeforeFailure = CountLiveActors(*World);
	TestNull(TEXT("An auxiliary cannot take ownership of an authored property"),
	         Duplicator->DuplicateActor(Source, FVector::ZeroVector, Error));
	TestTrue(TEXT("Ownership rejection is explicit"), Error.Contains(TEXT("AdapterOwnershipConflict")));
	TestTrue(TEXT("Ownership rejection identifies the auxiliary policy"), Error.Contains(TEXT("auxiliary")));
	TestEqual(TEXT("Rejected ownership leaves no provisional Actor"), CountLiveActors(*World), ActorsBeforeFailure);
	TestTrue(TEXT("Rejected ownership preserves source configuration"), Source->Tags.Contains(TEXT("AuthoredTag")));
	Registry.Unregister(AActor::StaticClass(), EOWTDuplicationAdapterRole::Auxiliary);
	AActor* PCGActor = World->SpawnActor<AActor>();
	UPCGComponent* Component = NewObject<UPCGComponent>(PCGActor);
	PCGActor->AddInstanceComponent(Component);
	Component->bActivated = false;
	Component->GenerationTrigger = EPCGComponentGenerationTrigger::GenerateOnDemand;
	Component->bIsComponentPartitioned = false;
	Component->RegisterComponent();
	const TSharedRef<FAdapterProbe> SpecificPCG = MakeShared<FAdapterProbe>();
	SpecificPCG->ValidationFailure = TEXT("CustomPCGPolicyRejected");
	const auto BuiltInAdapters = Registry.CreateAdapters();
	TestEqual(TEXT("Built-in PCG primary serves the component before replacement"),
	          CountApplicable(BuiltInAdapters, *Component, EOWTDuplicationAdapterRole::Primary), 1);
	TestFalse(TEXT("Replacing the built-in policy requires explicit unregister"),
	          Registry.Register(UPCGComponent::StaticClass(), ProbeFactory(SpecificPCG)));
	Registry.Unregister(UPCGComponent::StaticClass());
	TestTrue(TEXT("A custom PCG primary registers after explicit built-in removal"),
	         Registry.Register(UPCGComponent::StaticClass(), ProbeFactory(SpecificPCG)));
	const auto PCGAdapters = Registry.CreateAdapters();
	TestEqual(TEXT("The component has exactly one replacement PCG primary"),
	          CountApplicable(PCGAdapters, *Component, EOWTDuplicationAdapterRole::Primary), 1);
	TestNull(TEXT("Custom PCG validation governs actual duplication"),
	         Duplicator->DuplicateActor(PCGActor, FVector::ZeroVector, Error));
	TestEqual(TEXT("Custom PCG rejection survives dispatch"), Error, FString(TEXT("CustomPCGPolicyRejected")));
	TestEqual(TEXT("Custom PCG primary receives exactly one validation"), SpecificPCG->ObjectValidations, 1);
	return true;
}

#endif
