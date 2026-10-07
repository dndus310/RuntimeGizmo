#if WITH_DEV_AUTOMATION_TESTS

#include "Duplication/OWTDuplicationAdapterProvider.h"
#include "Duplication/OWTDuplicationPropertyPolicy.h"
#include "Duplication/OWTRuntimeActorDuplicator.h"
#include "Engine/Engine.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "Features/IModularFeatures.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "UObject/UnrealType.h"

namespace OWTDuplicationProviderTests
{
struct FProviderProbe
{
	TWeakObjectPtr<UOWTRuntimeActorDuplicator> Duplicator;
	int32 Registrations = 0;
	int32 Factories = 0;
	int32 Captures = 0;
	int32 Commits = 0;
	bool bAdapterRegistered = false;
	bool bPolicyRegistered = false;
	bool bMutateOwnerOnCreate = false;
	bool bOwnerPolicyReplaced = false;
};

class FProbeAdapter : public IOWTDuplicationAdapter
{
public:
	explicit FProbeAdapter(TSharedRef<FProviderProbe> InProbe);

	virtual bool CaptureActor(AActor& Actor, FString& Error) override;

	virtual void Commit(const FGuid& OperationId, const FOWTDuplicationOptions& Options,
	                    const TMap<UObject*, UObject*>& Mapping) override;

private:
	TSharedRef<FProviderProbe> Probe;
};

class FProbeProvider : public IOWTDuplicationAdapterProvider
{
public:
	explicit FProbeProvider(TSharedRef<FProviderProbe> InProbe);

	virtual FName GetProviderId() const override;

	virtual void RegisterAdapters(FOWTDuplicationAdapterRegistry& Registry) const override;

private:
	TSharedRef<FProviderProbe> Probe;
};

TOptional<bool> ExcludeTags(const UObject& Object, const FProperty& Property)
{
	if (Property.GetFName() == GET_MEMBER_NAME_CHECKED(AActor, Tags))
	{
		return false;
	}
	return {};
}

TOptional<bool> IncludeTags(const UObject& Object, const FProperty& Property)
{
	if (Property.GetFName() == GET_MEMBER_NAME_CHECKED(AActor, Tags))
	{
		return true;
	}
	return {};
}

FProbeAdapter::FProbeAdapter(TSharedRef<FProviderProbe> InProbe) : Probe(MoveTemp(InProbe))
{
}

bool FProbeAdapter::CaptureActor(AActor& Actor, FString& Error)
{
	++Probe->Captures;
	return true;
}

void FProbeAdapter::Commit(const FGuid& OperationId, const FOWTDuplicationOptions& Options,
                           const TMap<UObject*, UObject*>& Mapping)
{
	++Probe->Commits;
}

FProbeProvider::FProbeProvider(TSharedRef<FProviderProbe> InProbe) : Probe(MoveTemp(InProbe))
{
}

FName FProbeProvider::GetProviderId() const
{
	return TEXT("OWT.Automation.ProviderSessionIsolation");
}

void FProbeProvider::RegisterAdapters(FOWTDuplicationAdapterRegistry& Registry) const
{
	++Probe->Registrations;
	const TSharedRef<FProviderProbe> CapturedProbe = Probe;
	Probe->bAdapterRegistered = Registry.Register(
	    AStaticMeshActor::StaticClass(),
	    [CapturedProbe]() -> TUniquePtr<IOWTDuplicationAdapter>
	    {
		    ++CapturedProbe->Factories;
		    if (CapturedProbe->bMutateOwnerOnCreate)
		    {
			    if (UOWTRuntimeActorDuplicator* Duplicator = CapturedProbe->Duplicator.Get())
			    {
				    FOWTDuplicationAdapterRegistry& OwnerRegistry = Duplicator->GetAdapterRegistry();
				    OwnerRegistry.Unregister(AStaticMeshActor::StaticClass(), EOWTDuplicationAdapterRole::Auxiliary);
				    FOWTDuplicationPropertyPolicySet& OwnerPolicies = OwnerRegistry.GetPropertyPolicies();
				    OwnerPolicies.Unregister(AStaticMeshActor::StaticClass());
				    CapturedProbe->bOwnerPolicyReplaced =
				        OwnerPolicies.Register(AStaticMeshActor::StaticClass(), IncludeTags);
			    }
		    }
		    return MakeUnique<FProbeAdapter>(CapturedProbe);
	    },
	    EOWTDuplicationAdapterRole::Auxiliary);
	Probe->bPolicyRegistered = Registry.GetPropertyPolicies().Register(AStaticMeshActor::StaticClass(), ExcludeTags);
}
} // namespace OWTDuplicationProviderTests

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTDuplicationPropertyPolicySnapshotTest,
                                 "OWT.Runtime.DuplicationPropertyPolicySnapshot",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTDuplicationPropertyPolicySnapshotTest::RunTest(const FString& Parameters)
{
	const FProperty* Tags = FindFProperty<FProperty>(AActor::StaticClass(), GET_MEMBER_NAME_CHECKED(AActor, Tags));
	const FProperty* RootComponent = FindFProperty<FProperty>(AActor::StaticClass(), TEXT("RootComponent"));
	if (!TestNotNull(TEXT("Actor Tags reflection"), Tags))
	{
		return false;
	}
	if (!TestNotNull(TEXT("Actor RootComponent reflection"), RootComponent))
	{
		return false;
	}

	const AStaticMeshActor* DerivedObject = GetDefault<AStaticMeshActor>();
	for (int32 RegistrationOrder = 0; RegistrationOrder < 2; ++RegistrationOrder)
	{
		FOWTDuplicationPropertyPolicySet Policies;
		const FOWTDuplicationPropertyPolicySet::FRule BaseRule = [](const UObject& Object,
		                                                            const FProperty& Property) -> TOptional<bool>
		{
			return Property.GetFName() != GET_MEMBER_NAME_CHECKED(AActor, Tags);
		};
		if (RegistrationOrder == 0)
		{
			TestTrue(TEXT("Base policy registers"), Policies.Register(AActor::StaticClass(), BaseRule));
			TestTrue(TEXT("Subclass policy registers"),
			         Policies.Register(AStaticMeshActor::StaticClass(), OWTDuplicationProviderTests::IncludeTags));
		}
		else
		{
			TestTrue(TEXT("Subclass policy can register first"),
			         Policies.Register(AStaticMeshActor::StaticClass(), OWTDuplicationProviderTests::IncludeTags));
			TestTrue(TEXT("Base policy can register last"), Policies.Register(AActor::StaticClass(), BaseRule));
		}

		const TOptional<bool> DerivedResult = Policies.Resolve(*DerivedObject, *Tags);
		TestTrue(TEXT("Most-specific explicit result overrides base regardless of registration order"),
		         DerivedResult.Get(false));
		const TOptional<bool> FallbackResult = Policies.Resolve(*DerivedObject, *RootComponent);
		TestTrue(TEXT("Unset subclass result falls through to base rule"), FallbackResult.Get(false));
		TestFalse(TEXT("Unrelated class has no policy decision"),
		          Policies.Resolve(*GetDefault<UObject>(), *Tags).IsSet());

		const FOWTDuplicationPropertyPolicySet Snapshot = Policies;
		Policies.Unregister(AStaticMeshActor::StaticClass());
		TestTrue(TEXT("Replacement subclass rule registers"),
		         Policies.Register(AStaticMeshActor::StaticClass(), OWTDuplicationProviderTests::ExcludeTags));
		const TOptional<bool> ReplacementResult = Policies.Resolve(*DerivedObject, *Tags);
		TestTrue(TEXT("Replacement decision remains explicit"), ReplacementResult.IsSet());
		TestFalse(TEXT("Live policy now excludes Tags"), ReplacementResult.Get(true));
		TestTrue(TEXT("Copied operation policy preserves its original rule"),
		         Snapshot.Resolve(*DerivedObject, *Tags).Get(false));
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTDuplicationProviderSessionTest, "OWT.Runtime.DuplicationProviderSessionIsolation",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTDuplicationProviderSessionTest::RunTest(const FString& Parameters)
{
	const UWorld::InitializationValues Init = UWorld::InitializationValues()
	                                              .AllowAudioPlayback(false)
	                                              .CreatePhysicsScene(true)
	                                              .CreateNavigation(false)
	                                              .CreateAISystem(false)
	                                              .ShouldSimulatePhysics(false);
	UWorld* World =
	    UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &Init);
	if (!TestNotNull(TEXT("Provider test standalone world"), World))
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	ON_SCOPE_EXIT
	{
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
	};

	AStaticMeshActor* Source = World->SpawnActor<AStaticMeshActor>();
	if (!TestNotNull(TEXT("Reflected subclass source"), Source))
	{
		return false;
	}
	Source->Tags.Add(TEXT("ProviderAuthoredTag"));
	const TSharedRef<OWTDuplicationProviderTests::FProviderProbe> Probe =
	    MakeShared<OWTDuplicationProviderTests::FProviderProbe>();
	OWTDuplicationProviderTests::FProbeProvider Provider(Probe);
	IModularFeatures& Features = IModularFeatures::Get();
	const FName FeatureName = IOWTDuplicationAdapterProvider::GetFeatureName();
	Features.RegisterModularFeature(FeatureName, &Provider);
	bool bProviderRegistered = true;
	ON_SCOPE_EXIT
	{
		if (bProviderRegistered)
		{
			Features.UnregisterModularFeature(FeatureName, &Provider);
		}
	};

	UOWTRuntimeActorDuplicator* Duplicator = NewObject<UOWTRuntimeActorDuplicator>(Source);
	Probe->Duplicator = Duplicator;
	if (!TestTrue(TEXT("Session captures modular providers on initialization"), Duplicator->Initialize(Source)))
	{
		return false;
	}
	ON_SCOPE_EXIT
	{
		Duplicator->Deinitialize();
	};
	TestEqual(TEXT("Custom provider injected once"), Probe->Registrations, 1);
	TestTrue(TEXT("Provider injected an auxiliary factory"), Probe->bAdapterRegistered);
	TestTrue(TEXT("Provider injected subclass property rules"), Probe->bPolicyRegistered);
	Features.UnregisterModularFeature(FeatureName, &Provider);
	bProviderRegistered = false;

	FString Error;
	AActor* First = Duplicator->DuplicateActor(Source, FVector(100.0, 0.0, 0.0), Error);
	if (!TestNotNull(*FString::Printf(TEXT("Configured session survives provider unregistration: %s"), *Error), First))
	{
		return false;
	}
	TestFalse(TEXT("Injected subclass policy excludes authored Tags"),
	          First->Tags.Contains(TEXT("ProviderAuthoredTag")));
	TestEqual(TEXT("Captured factory remains callable after provider unregistration"), Probe->Factories, 1);
	TestEqual(TEXT("Auxiliary adapter captures the source"), Probe->Captures, 1);
	TestEqual(TEXT("Auxiliary adapter reaches commit"), Probe->Commits, 1);

	Probe->bMutateOwnerOnCreate = true;
	AActor* DuringMutation = Duplicator->DuplicateActor(Source, FVector(200.0, 0.0, 0.0), Error);
	if (!TestNotNull(*FString::Printf(TEXT("Factory can update next-operation configuration safely: %s"), *Error),
	                 DuringMutation))
	{
		return false;
	}
	TestTrue(TEXT("Factory changed the live owner's property policy"), Probe->bOwnerPolicyReplaced);
	TestFalse(TEXT("In-flight operation retains the pre-factory property policy"),
	          DuringMutation->Tags.Contains(TEXT("ProviderAuthoredTag")));
	TestEqual(TEXT("In-flight adapter registration remains active despite owner removal"), Probe->Captures, 2);
	TestEqual(TEXT("In-flight adapter completes normally"), Probe->Commits, 2);

	AActor* Following = Duplicator->DuplicateActor(Source, FVector(300.0, 0.0, 0.0), Error);
	if (!TestNotNull(*FString::Printf(TEXT("Next operation uses edited configuration: %s"), *Error), Following))
	{
		return false;
	}
	TestTrue(TEXT("Next operation observes the new include-Tags rule"),
	         Following->Tags.Contains(TEXT("ProviderAuthoredTag")));
	TestEqual(TEXT("Removed factory is not called on the next operation"), Probe->Factories, 2);
	TestEqual(TEXT("Removed auxiliary adapter does not capture on the next operation"), Probe->Captures, 2);
	TestTrue(TEXT("Duplication never mutates the source's authored Tags"),
	         Source->Tags.Contains(TEXT("ProviderAuthoredTag")));
	return true;
}

#endif
