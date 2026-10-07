#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Components/ChildActorComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/LightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Dom/JsonObject.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformMemory.h"
#include "Misc/App.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "PCGComponent.h"
#include "PCGGraph.h"
#include "Playground/OWTPlaygroundGameMode.h"
#include "Playground/OWTPlaygroundHUD.h"
#include "RHI.h"
#include "Serialization/JsonSerializer.h"
#include "UnrealClient.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/UnrealType.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorSubsystem.h"

namespace OWTPlaygroundTests
{
const TCHAR* FixtureNames[] = {TEXT("ColorBlock"),    TEXT("StackedSculpture"),  TEXT("LightRig"),
                               TEXT("InstanceArray"), TEXT("HierarchicalArray"), TEXT("SplineDisplay"),
                               TEXT("ChildAssembly"), TEXT("PhysicsProp"),       TEXT("PCGDisplay")};

const FName TemporaryTag(TEXT("OWT.Playground.AutomationTemporary"));

enum class EPhase
{
	Startup,
	Baseline,
	Prepare,
	SourceGeneration,
	Duplicate,
	Commit,
	CopyGeneration,
	Cleanup,
	Settle,
	PostGC,
	Screenshot,
	Complete
};

struct FSourcePCGState
{
	FSourcePCGState(UPCGComponent& InComponent)
	    : Component(&InComponent), Graph(InComponent.GetGraph()), Trigger(InComponent.GenerationTrigger),
	      Seed(InComponent.Seed), bActivated(InComponent.bActivated),
	      bExpectedGenerated(InComponent.bGenerated || InComponent.IsGenerating())
	{
	}
	TWeakObjectPtr<UPCGComponent> Component;
	TWeakObjectPtr<UPCGGraph> Graph;
	EPCGComponentGenerationTrigger Trigger;
	int32 Seed;
	bool bActivated;
	bool bExpectedGenerated;
};

void GetAuthoredActors(AActor& Root, TArray<AActor*>& Actors)
{
	Actors.Add(&Root);
	TInlineComponentArray<UChildActorComponent*> Children(&Root);
	for (UChildActorComponent* Child : Children)
	{
		if (AActor* Actor = Child->GetChildActor())
		{
			GetAuthoredActors(*Actor, Actors);
		}
	}
}

void GetPCGComponents(AActor& Root, TArray<UPCGComponent*>& Components)
{
	TArray<AActor*> Actors;
	GetAuthoredActors(Root, Actors);
	for (AActor* Actor : Actors)
	{
		TInlineComponentArray<UPCGComponent*> Owned(Actor);
		Components.Append(Owned);
	}
}

bool IsManagedComponent(AActor& Owner, const UActorComponent& Component)
{
	TInlineComponentArray<UPCGComponent*> Procedural(&Owner);
	const UObject* Object = &Component;
	for (const UPCGComponent* PCG : Procedural)
	{
		if (PCG->IsAnyObjectManagedByResource(MakeArrayView(&Object, 1)))
		{
			return true;
		}
	}
	return false;
}

int32 CountInstances(AActor& Root)
{
	TArray<AActor*> Actors;
	GetAuthoredActors(Root, Actors);
	int32 Count = 0;
	for (AActor* Actor : Actors)
	{
		TInlineComponentArray<UInstancedStaticMeshComponent*> Components(Actor);
		for (const UInstancedStaticMeshComponent* Component : Components)
		{
			Count += Component->GetInstanceCount();
		}
	}
	return Count;
}

int32 CountTaggedActors(UWorld& World, FName Tag)
{
	int32 Count = 0;
	for (TActorIterator<AActor> It(&World); It; ++It)
	{
		if (It->IsActorBeingDestroyed())
		{
			continue;
		}
		if (It->ActorHasTag(Tag))
		{
			++Count;
		}
	}
	return Count;
}

template <typename ObjectType>
int32 CountTrackedObjects(const TArray<TWeakObjectPtr<ObjectType>>& References, bool bIncludeGarbage)
{
	int32 Count = 0;
	for (const TWeakObjectPtr<ObjectType>& Reference : References)
	{
		if (bIncludeGarbage)
		{
			// Diagnostic only: never dereference a destroyed or unreachable object.
			if (Reference.GetEvenIfUnreachable())
			{
				++Count;
			}
		}
		else if (Reference.IsValid())
		{
			++Count;
		}
	}
	return Count;
}

double Percentile95(TArray<double> Values)
{
	if (Values.IsEmpty())
	{
		return 0.0;
	}
	Values.Sort();
	return Values[FMath::Clamp(FMath::CeilToInt(Values.Num() * 0.95) - 1, 0, Values.Num() - 1)];
}

double Average(const TArray<double>& Values)
{
	double Sum = 0.0;
	for (double Value : Values)
	{
		Sum += Value;
	}
	return Values.IsEmpty() ? 0.0 : Sum / Values.Num();
}

bool ValidateFixture(FAutomationTestBase& Test, AActor& Actor, const FString& Name)
{
	bool bValid = Test.TestTrue(TEXT("Placed BP has begun play"), Actor.HasActorBegunPlay());
	TInlineComponentArray<UStaticMeshComponent*> Meshes(&Actor);
	if (Name == TEXT("InstanceArray"))
	{
		const UInstancedStaticMeshComponent* Instances = Actor.FindComponentByClass<UInstancedStaticMeshComponent>();
		if (!Test.TestNotNull(TEXT("Actual ISM fixture component"), Instances))
		{
			return false;
		}
		bValid &= Test.TestEqual(TEXT("Actual ISM authored instances"), Instances->GetInstanceCount(), 6);
	}
	else if (Name == TEXT("HierarchicalArray"))
	{
		const UHierarchicalInstancedStaticMeshComponent* Instances =
		    Actor.FindComponentByClass<UHierarchicalInstancedStaticMeshComponent>();
		if (!Test.TestNotNull(TEXT("Actual HISM fixture component"), Instances))
		{
			return false;
		}
		bValid &= Test.TestEqual(TEXT("Actual HISM authored instances"), Instances->GetInstanceCount(), 9);
	}
	else if (Name == TEXT("SplineDisplay"))
	{
		const USplineComponent* Spline = Actor.FindComponentByClass<USplineComponent>();
		if (!Test.TestNotNull(TEXT("Actual spline fixture component"), Spline))
		{
			return false;
		}
		bValid &= Test.TestEqual(TEXT("Actual spline authored points"), Spline->GetNumberOfSplinePoints(), 4);
	}
	else if (Name == TEXT("LightRig"))
	{
		bValid &= Test.TestNotNull(TEXT("Actual point light"), Actor.FindComponentByClass<UPointLightComponent>());
		bValid &= Test.TestNotNull(TEXT("Actual spot light"), Actor.FindComponentByClass<USpotLightComponent>());
	}
	else if (Name == TEXT("ChildAssembly"))
	{
		const UChildActorComponent* Child = Actor.FindComponentByClass<UChildActorComponent>();
		if (!Test.TestNotNull(TEXT("Actual CAC fixture"), Child))
		{
			return false;
		}
		bValid &= Test.TestNotNull(TEXT("Actual live CAC child"), Child->GetChildActor());
	}
	else if (Name == TEXT("PCGDisplay"))
	{
		const UPCGComponent* PCG = Actor.FindComponentByClass<UPCGComponent>();
		if (!Test.TestNotNull(TEXT("Actual PCG fixture"), PCG))
		{
			return false;
		}
		bValid &= Test.TestNotNull(TEXT("Actual cooked PCG graph"), PCG->GetGraph());
		const EPCGComponentGenerationTrigger ExpectedTrigger =
		    Actor.GetWorld()->GetOutermost()->GetName().EndsWith(TEXT("L_OWTStress"))
		        ? EPCGComponentGenerationTrigger::GenerateOnDemand
		        : EPCGComponentGenerationTrigger::GenerateOnLoad;
		bValid &= Test.TestEqual(TEXT("Playground PCG uses the map generation policy"), PCG->GenerationTrigger,
		                         ExpectedTrigger);
	}
	else if (Name == TEXT("StackedSculpture"))
	{
		bValid &= Test.TestTrue(TEXT("Sculpture has multiple authored mesh components"), Meshes.Num() >= 3);
		bool bHasNestedParent = false;
		for (const UStaticMeshComponent* Mesh : Meshes)
		{
			const USceneComponent* Parent = Mesh->GetAttachParent();
			if (Parent)
			{
				bHasNestedParent |= Parent != Actor.GetRootComponent();
			}
		}
		bValid &= Test.TestTrue(TEXT("Sculpture exercises nested SCS attachment"), bHasNestedParent);
	}
	else if (Name == TEXT("PhysicsProp"))
	{
		const UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Actor.GetRootComponent());
		if (!Test.TestNotNull(TEXT("Physics fixture primitive root"), Primitive))
		{
			return false;
		}
		bValid &= Test.TestEqual(TEXT("Physics fixture preserves query/physics collision configuration"),
		                         Primitive->GetCollisionEnabled(), ECollisionEnabled::QueryAndPhysics);
	}
	for (const UStaticMeshComponent* Mesh : Meshes)
	{
		bValid &= Test.TestNotNull(TEXT("Placed mesh asset resolves in current build"), Mesh->GetStaticMesh().Get());
		bValid &= Test.TestNotNull(TEXT("Placed material resolves in current build"), Mesh->GetMaterial(0));
	}
	const FStrProperty* Label = FindFProperty<FStrProperty>(Actor.GetClass(), TEXT("FixtureLabel"));
	const FIntProperty* Revision = FindFProperty<FIntProperty>(Actor.GetClass(), TEXT("Revision"));
	const FStructProperty* Accent = FindFProperty<FStructProperty>(Actor.GetClass(), TEXT("Accent"));
	bValid &= Test.TestNotNull(TEXT("Actual Blueprint string variable"), Label);
	bValid &= Test.TestNotNull(TEXT("Actual Blueprint integer variable"), Revision);
	bValid &= Test.TestNotNull(TEXT("Actual Blueprint color variable"), Accent);
	if (Accent)
	{
		bValid &= Test.TestTrue(TEXT("Blueprint Accent variable is a color"),
		                        Accent->Struct == TBaseStructure<FLinearColor>::Get());
		if (Name == TEXT("LightRig"))
		{
			if (Accent->Struct == TBaseStructure<FLinearColor>::Get())
			{
				const UPointLightComponent* Light = Actor.FindComponentByClass<UPointLightComponent>();
				if (Light)
				{
					bValid &= Test.TestTrue(
					    TEXT("Cooked Blueprint Construction Script applied Accent to point light"),
					    Light->GetLightColor().Equals(*Accent->ContainerPtrToValuePtr<FLinearColor>(&Actor), 0.01f));
				}
			}
		}
	}
	return bValid;
}

void SplitName(const FString& Name, FString& Family, int64& Number)
{
	Family = Name;
	Number = 0;
	int32 Separator = INDEX_NONE;
	if (!Name.FindLastChar(TEXT('_'), Separator))
	{
		return;
	}
	const FString Suffix = Name.Mid(Separator + 1);
	if (Suffix.IsEmpty())
	{
		return;
	}
	for (TCHAR Character : Suffix)
	{
		if (!FChar::IsDigit(Character))
		{
			return;
		}
	}
	Family = Name.Left(Separator);
	Number = FCString::Atoi64(*Suffix);
}

bool CompareActor(FAutomationTestBase& Test, AActor& Source, AActor& Copy)
{
	bool bValid = Test.TestEqual(TEXT("Blueprint generated class preserved"), Copy.GetClass(), Source.GetClass());
	const FStrProperty* Label = FindFProperty<FStrProperty>(Source.GetClass(), TEXT("FixtureLabel"));
	const FIntProperty* Revision = FindFProperty<FIntProperty>(Source.GetClass(), TEXT("Revision"));
	const FStructProperty* Accent = FindFProperty<FStructProperty>(Source.GetClass(), TEXT("Accent"));
	if (Label)
	{
		bValid &=
		    Test.TestEqual(TEXT("Blueprint instance string value preserved"),
		                   Label->GetPropertyValue_InContainer(&Copy), Label->GetPropertyValue_InContainer(&Source));
	}
	if (Revision)
	{
		bValid &= Test.TestEqual(TEXT("Blueprint instance integer value preserved"),
		                         Revision->GetPropertyValue_InContainer(&Copy),
		                         Revision->GetPropertyValue_InContainer(&Source));
	}
	if (Accent)
	{
		bValid &= Test.TestTrue(TEXT("Blueprint color value preserved"), Accent->Identical_InContainer(&Source, &Copy));
	}
	TInlineComponentArray<UActorComponent*> Components(&Source);
	for (UActorComponent* Original : Components)
	{
		if (IsManagedComponent(Source, *Original))
		{
			continue;
		}
		UActorComponent* Target = FindObjectFast<UActorComponent>(&Copy, Original->GetFName());
		if (!Test.TestNotNull(FString::Printf(TEXT("Authored component %s exists"), *Original->GetName()), Target))
		{
			bValid = false;
			continue;
		}
		bValid &= Test.TestTrue(TEXT("Authored component is independent"), Original != Target);
		bValid &= Test.TestEqual(TEXT("Component class preserved"), Target->GetClass(), Original->GetClass());
		if (Target->GetClass() != Original->GetClass())
		{
			continue;
		}
		if (const USceneComponent* Scene = Cast<USceneComponent>(Original))
		{
			const USceneComponent* TargetScene = CastChecked<USceneComponent>(Target);
			if (Scene != Source.GetRootComponent())
			{
				bValid &=
				    Test.TestTrue(TEXT("Nested component local transform preserved"),
				                  Scene->GetRelativeTransform().Equals(TargetScene->GetRelativeTransform(), 0.001));
			}
			const USceneComponent* Parent = Scene->GetAttachParent();
			if (Parent)
			{
				bValid &= Test.TestNotNull(TEXT("Nested component parent exists"), TargetScene->GetAttachParent());
				if (TargetScene->GetAttachParent())
				{
					bValid &= Test.TestEqual(TEXT("Nested component parent preserved"),
					                         TargetScene->GetAttachParent()->GetFName(), Parent->GetFName());
				}
			}
			bValid &= Test.TestEqual(TEXT("Attachment socket preserved"), TargetScene->GetAttachSocketName(),
			                         Scene->GetAttachSocketName());
		}
		if (const UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Original))
		{
			const UStaticMeshComponent* TargetMesh = CastChecked<UStaticMeshComponent>(Target);
			bValid &=
			    Test.TestEqual(TEXT("Static mesh asset preserved"), TargetMesh->GetStaticMesh(), Mesh->GetStaticMesh());
			bValid &= Test.TestEqual(TEXT("Collision mode preserved"), TargetMesh->GetCollisionEnabled(),
			                         Mesh->GetCollisionEnabled());
			bValid &= Test.TestEqual(TEXT("Physics simulation flag preserved"), TargetMesh->IsSimulatingPhysics(),
			                         Mesh->IsSimulatingPhysics());
			bValid &= Test.TestEqual(TEXT("Material slot count preserved"), TargetMesh->GetNumMaterials(),
			                         Mesh->GetNumMaterials());
			for (int32 Index = 0; Index < Mesh->GetNumMaterials(); ++Index)
			{
				bValid &= Test.TestEqual(TEXT("Authored material preserved"), TargetMesh->GetMaterial(Index),
				                         Mesh->GetMaterial(Index));
			}
		}
		if (const UInstancedStaticMeshComponent* Instances = Cast<UInstancedStaticMeshComponent>(Original))
		{
			const UInstancedStaticMeshComponent* TargetInstances = CastChecked<UInstancedStaticMeshComponent>(Target);
			bValid &= Test.TestEqual(TEXT("Authored ISM/HISM instance count preserved"),
			                         TargetInstances->GetInstanceCount(), Instances->GetInstanceCount());
			for (int32 Index = 0; Index < Instances->GetInstanceCount(); ++Index)
			{
				FTransform OriginalTransform;
				FTransform TargetTransform;
				Instances->GetInstanceTransform(Index, OriginalTransform);
				TargetInstances->GetInstanceTransform(Index, TargetTransform);
				bValid &= Test.TestTrue(TEXT("Authored instance transform preserved"),
				                        OriginalTransform.Equals(TargetTransform));
			}
		}
		if (const USplineComponent* Spline = Cast<USplineComponent>(Original))
		{
			const USplineComponent* TargetSpline = CastChecked<USplineComponent>(Target);
			bValid &= Test.TestEqual(TEXT("Spline point count preserved"), TargetSpline->GetNumberOfSplinePoints(),
			                         Spline->GetNumberOfSplinePoints());
			bValid &= Test.TestEqual(TEXT("Spline loop flag preserved"), TargetSpline->IsClosedLoop(),
			                         Spline->IsClosedLoop());
			for (int32 Index = 0; Index < Spline->GetNumberOfSplinePoints(); ++Index)
			{
				bValid &=
				    Test.TestTrue(TEXT("Spline authored point preserved"),
				                  TargetSpline->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::Local)
				                      .Equals(Spline->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::Local)));
			}
		}
		if (const ULightComponent* Light = Cast<ULightComponent>(Original))
		{
			const ULightComponent* TargetLight = CastChecked<ULightComponent>(Target);
			bValid &= Test.TestEqual(TEXT("Light intensity preserved"), TargetLight->Intensity, Light->Intensity);
			bValid &= Test.TestTrue(TEXT("Light color preserved"),
			                        TargetLight->GetLightColor().Equals(Light->GetLightColor()));
		}
		if (const UChildActorComponent* Child = Cast<UChildActorComponent>(Original))
		{
			const UChildActorComponent* TargetChild = CastChecked<UChildActorComponent>(Target);
			bValid &= Test.TestEqual(TEXT("CAC child class preserved"), TargetChild->GetChildActorClass().Get(),
			                         Child->GetChildActorClass().Get());
			if (Test.TestNotNull(TEXT("CAC spawned child preserved"), TargetChild->GetChildActor()))
			{
				if (Test.TestNotNull(TEXT("CAC source child exists"), Child->GetChildActor()))
				{
					bValid &= Test.TestTrue(TEXT("CAC child independent"),
					                        TargetChild->GetChildActor() != Child->GetChildActor());
					bValid &= CompareActor(Test, *Child->GetChildActor(), *TargetChild->GetChildActor());
				}
			}
			else
			{
				bValid = false;
			}
		}
		if (const UPCGComponent* PCG = Cast<UPCGComponent>(Original))
		{
			const UPCGComponent* TargetPCG = CastChecked<UPCGComponent>(Target);
			bValid &= Test.TestEqual(TEXT("PCG graph asset preserved"), TargetPCG->GetGraph(), PCG->GetGraph());
			bValid &= Test.TestTrue(TEXT("PCG graph instance independent"),
			                        TargetPCG->GetGraphInstance() != PCG->GetGraphInstance());
			bValid &= Test.TestEqual(TEXT("PCG seed preserved"), TargetPCG->Seed, PCG->Seed);
		}
	}
	return bValid;
}

class FRunPlayground final : public IAutomationLatentCommand
{
public:
	FRunPlayground(FAutomationTestBase& InTest, bool bInStress)
	    : Test(InTest), World(), Hub(), Editor(), OriginalSelection(), Source(), Copy(), Sources(), RetiredActors(),
	      RetiredComponents(), GeneratedSources(), InitialPCGStates(), LastSuffixes(), Coverage(), BaselineFrames(),
	      WorkFrames(), CommitTimes(), ReadyTimes(), Phase(EPhase::Startup), OperationId(), Subscription(),
	      SourceTransform(FTransform::Identity), RunStarted(FPlatformTime::Seconds()), PhaseStarted(RunStarted),
	      RequestStarted(0.0), MemoryStart(0), MemoryEnd(0), MemoryAfterGC(0), GCMilliseconds(0.0), Cycle(0),
	      CycleCount(bInStress ? 288 : 18), BaselineActorCount(0), SourceInstanceCount(0), EventCount(0),
	      ResultEventCount(0), FrameCount(0), PostGCFrames(0), AllocatedActorsBeforeGC(0),
	      AllocatedComponentsBeforeGC(0), bStress(bInStress), bAborting(false), bWasEditing(false),
	      bSourceTransformCaptured(false), OriginalLabel(), OriginalRevision(0), MetricsPath(), ScreenshotPath()
	{
	}

	virtual ~FRunPlayground() override
	{
		if (Editor.IsValid())
		{
			Editor->Unsubscribe(Subscription);
		}
	}

	virtual bool Update() override
	{
		++FrameCount;
		if (Phase != EPhase::Startup)
		{
			if (!World.IsValid())
			{
				Test.AddError(TEXT("The actual playground world was unloaded during automation."));
				return true;
			}
			if (!Editor.IsValid())
			{
				Test.AddError(TEXT("The live AttributeEditor disappeared during automation."));
				return true;
			}
			if (Phase == EPhase::Baseline)
			{
				BaselineFrames.Add(FApp::GetDeltaTime() * 1000.0);
			}
			else if (Phase <= EPhase::Cleanup)
			{
				WorkFrames.Add(FApp::GetDeltaTime() * 1000.0);
			}
		}
		switch (Phase)
		{
		case EPhase::Startup:
			return Start();
		case EPhase::Baseline:
			if (BaselineFrames.Num() >= 60)
			{
				MemoryStart = FPlatformMemory::GetStats().UsedPhysical;
				SetPhase(EPhase::Prepare);
			}
			break;
		case EPhase::Prepare:
			Prepare();
			break;
		case EPhase::SourceGeneration:
			WaitForSource();
			break;
		case EPhase::Duplicate:
			RequestCopy();
			break;
		case EPhase::Commit:
			WaitForCommit();
			break;
		case EPhase::CopyGeneration:
			WaitForCopy();
			break;
		case EPhase::Cleanup:
			Cleanup();
			break;
		case EPhase::Settle:
			Settle();
			break;
		case EPhase::PostGC:
			FinishGC();
			break;
		case EPhase::Screenshot:
			FinishScreenshot();
			break;
		case EPhase::Complete:
			return true;
		}
		return Phase == EPhase::Complete;
	}

private:
	void SetPhase(EPhase NewPhase)
	{
		Phase = NewPhase;
		PhaseStarted = FPlatformTime::Seconds();
	}

	bool HasTimedOut(double Seconds = 30.0) const
	{
		return FPlatformTime::Seconds() - PhaseStarted > Seconds;
	}

	void Fail(const FString& Error)
	{
		Test.AddError(FString::Printf(TEXT("Cycle %d: %s"), Cycle, *Error));
		bAborting = true;
		if (Editor.IsValid())
		{
			if (Editor->CanCancelActiveTool())
			{
				Editor->RequestEndTool(false);
			}
		}
		SetPhase(EPhase::Cleanup);
	}

	bool Start()
	{
		if (!GEngine->GameViewport)
		{
			if (HasTimedOut())
			{
				Test.AddError(TEXT(
				    "Launch a playground map in -game or a packaged application; a live GameViewport is required."));
				return true;
			}
			return false;
		}
		UWorld* CurrentWorld = GEngine->GameViewport->GetWorld();
		if (!CurrentWorld)
		{
			if (HasTimedOut())
			{
				Test.AddError(TEXT("GameViewport did not acquire a world."));
				return true;
			}
			return false;
		}
		if (!CurrentWorld->HasBegunPlay())
		{
			if (HasTimedOut())
			{
				Test.AddError(TEXT("Playground world did not begin play."));
				return true;
			}
			return false;
		}
		const FString Map = CurrentWorld->GetOutermost()->GetName();
		if (!Map.Contains(TEXT("/OWTPlayground/Maps/L_OWT")))
		{
			Test.AddError(FString::Printf(TEXT("Expected an actual playground map, got %s."), *Map));
			return true;
		}
		World = CurrentWorld;
		AOWTPlaygroundGameMode* GameMode = CurrentWorld->GetAuthGameMode<AOWTPlaygroundGameMode>();
		if (!Test.TestNotNull(TEXT("Actual playground GameMode override"), GameMode))
		{
			return true;
		}
		if (!GameMode->IsPlaygroundReady())
		{
			if (HasTimedOut())
			{
				Test.AddError(TEXT("Playground startup did not enable editing and initial selection."));
				return true;
			}
			return false;
		}
		Hub = CurrentWorld->GetSubsystem<UVTBOWTEditorSubsystem>();
		if (!Test.TestNotNull(TEXT("Live editing subsystem"), Hub.Get()))
		{
			return true;
		}
		Editor = Hub->GetAttributeEditor();
		if (!Test.TestNotNull(TEXT("Live AttributeEditor created by map startup"), Editor.Get()))
		{
			return true;
		}
		bWasEditing = Hub->IsEditingEnabled();
		OriginalSelection = Hub->SelectedObject;
		if (!Test.TestTrue(TEXT("Playground starts in edit mode"), bWasEditing))
		{
			return true;
		}
		if (!Test.TestNotNull(TEXT("Playground starts with an editable selection"), OriginalSelection.Get()))
		{
			return true;
		}
		APlayerController* Controller = CurrentWorld->GetFirstPlayerController();
		if (!Test.TestNotNull(TEXT("Live playground controller"), Controller))
		{
			return true;
		}
		AOWTPlaygroundHUD* HUD = Cast<AOWTPlaygroundHUD>(Controller->GetHUD());
		if (!Test.TestNotNull(TEXT("Live playground help/stats HUD"), HUD))
		{
			return true;
		}
		const bool bHelpWasVisible = HUD->IsHelpVisible();
		HUD->ToggleHelp();
		Test.TestEqual(TEXT("Playground help visibility toggles"), HUD->IsHelpVisible(), !bHelpWasVisible);
		HUD->ToggleHelp();
		Test.TestEqual(TEXT("Playground help visibility restored"), HUD->IsHelpVisible(), bHelpWasVisible);
		BaselineActorCount = CountTaggedActors(*CurrentWorld, TEXT("OWT.Playground.Editable"));
		const int32 ExpectedCount = Map.EndsWith(TEXT("L_OWTStress")) ? 600 : 54;
		Test.TestEqual(TEXT("Authored playground fixture count"), BaselineActorCount, ExpectedCount);
		for (const TCHAR* Name : FixtureNames)
		{
			const FString Path =
			    FString::Printf(TEXT("/Game/OWTPlayground/Blueprints/BP_OWT%s.BP_OWT%s_C"), Name, Name);
			UClass* Class = LoadClass<AActor>(nullptr, *Path);
			if (!Test.TestNotNull(*FString::Printf(TEXT("Actual cooked BP %s"), Name), Class))
			{
				return true;
			}
			AActor* Found = nullptr;
			const FName Tag(*FString::Printf(TEXT("OWT.Fixture.%s"), Name));
			for (TActorIterator<AActor> It(CurrentWorld); It; ++It)
			{
				if (It->GetClass() != Class)
				{
					continue;
				}
				if (!It->ActorHasTag(Tag))
				{
					continue;
				}
				Found = *It;
				break;
			}
			if (!Test.TestNotNull(*FString::Printf(TEXT("Placed BP fixture %s"), Name), Found))
			{
				return true;
			}
			if (!ValidateFixture(Test, *Found, Name))
			{
				return true;
			}
			Sources.Add(Found);
			Coverage.Add(Name, 0);
		}
		Subscription = Editor->Subscribe(Editor.Get(),
		                                 FOWTAttributeEventNative::CreateLambda(
		                                     [this](FName Topic, const FString&)
		                                     {
			                                     ++EventCount;
			                                     if (Topic == TEXT("ObjectDuplicated"))
			                                     {
				                                     ++ResultEventCount;
			                                     }
		                                     }),
		                                 false);
		Test.TestTrue(TEXT("Live pub-sub subscription registered"), Subscription.IsValid());
		const FString Label = bStress ? TEXT("Stress") : TEXT("Smoke");
		const FString Stem = Label + TEXT("_") + FPaths::GetBaseFilename(Map) + TEXT("_") +
		                     FDateTime::UtcNow().ToString(TEXT("%Y%m%d_%H%M%S"));
		const FString Directory = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Automation/OWTPlayground"));
		IFileManager::Get().MakeDirectory(*Directory, true);
		MetricsPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(Directory, Stem + TEXT(".json")));
		ScreenshotPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(Directory, Stem + TEXT(".png")));
		SetPhase(EPhase::Baseline);
		return false;
	}

	void Prepare()
	{
		if (Cycle >= CycleCount)
		{
			SetPhase(EPhase::Settle);
			return;
		}
		if (!Editor->GetModeSnapshot().bCanStartTools)
		{
			if (HasTimedOut())
			{
				Fail(TEXT("Default tool did not become available between duplicate operations."));
			}
			return;
		}
		Source = Sources[Cycle % Sources.Num()];
		if (!Source.IsValid())
		{
			Fail(TEXT("An original placed Blueprint fixture disappeared."));
			return;
		}
		Hub->SetSelectedObject(Source.Get());
		SourceTransform = Source->GetActorTransform();
		bSourceTransformCaptured = true;
		FStrProperty* Label = FindFProperty<FStrProperty>(Source->GetClass(), TEXT("FixtureLabel"));
		FIntProperty* Revision = FindFProperty<FIntProperty>(Source->GetClass(), TEXT("Revision"));
		check(Label);
		check(Revision);
		OriginalLabel = Label->GetPropertyValue_InContainer(Source.Get());
		OriginalRevision = Revision->GetPropertyValue_InContainer(Source.Get());
		Label->SetPropertyValue_InContainer(Source.Get(), FString::Printf(TEXT("Automation cycle %d"), Cycle));
		Revision->SetPropertyValue_InContainer(Source.Get(), OriginalRevision + Cycle + 3);
		const FOWTAttributeSnapshot Before = Editor->GetSnapshot();
		if (!Test.TestTrue(TEXT("Selected fixture supports Details transform editing"), Before.bCanEditTransform))
		{
			Fail(Before.DisabledReason);
			return;
		}
		const double EditedX = SourceTransform.GetLocation().X + 17.0;
		const bool bEdited = Editor->RequestTransformField(Before, EOWTTransformField::LocationX, EditedX,
		                                                   EOWTTransformEditPhase::Commit, FGuid::NewGuid());
		Test.TestTrue(TEXT("Details numeric transform accepted"), bEdited);
		Test.TestEqual(TEXT("Details updates actual Actor"), Source->GetActorLocation().X, EditedX);
		Source->SetActorTransform(SourceTransform, false, nullptr, ETeleportType::TeleportPhysics);
		if (!bEdited)
		{
			Fail(TEXT("Details request was rejected."));
			return;
		}
		GeneratedSources.Reset();
		InitialPCGStates.Reset();
		TArray<UPCGComponent*> Components;
		GetPCGComponents(*Source, Components);
		for (UPCGComponent* PCG : Components)
		{
			InitialPCGStates.Emplace(*PCG);
			if (PCG->IsGenerating())
			{
				continue;
			}
			if (!PCG->bGenerated)
			{
				GeneratedSources.Add(PCG);
				PCG->GenerateLocal(true);
			}
		}
		SetPhase(EPhase::SourceGeneration);
	}

	bool GenerationFinished(AActor& Actor) const
	{
		TArray<UPCGComponent*> Components;
		GetPCGComponents(Actor, Components);
		for (const UPCGComponent* PCG : Components)
		{
			if (PCG->IsGenerating())
			{
				return false;
			}
			if (PCG->IsCleaningUp())
			{
				return false;
			}
			if (!PCG->AreManagedResourcesAccessible())
			{
				return false;
			}
			if (!PCG->bGenerated)
			{
				return false;
			}
		}
		return true;
	}

	void WaitForSource()
	{
		if (!Source.IsValid())
		{
			Fail(TEXT("Source destroyed while awaiting generation."));
			return;
		}
		if (!GenerationFinished(*Source))
		{
			if (HasTimedOut())
			{
				Fail(TEXT("Actual source BP PCG generation did not finish."));
			}
			return;
		}
		SourceInstanceCount = CountInstances(*Source);
		Test.TestTrue(TEXT("Live tick observes external Actor transform restoration"),
		              Editor->GetSnapshot().Transform.Equals(SourceTransform));
		if (!GeneratedSources.IsEmpty())
		{
			Test.TestTrue(TEXT("Actual PCG graph generates mesh instances"), SourceInstanceCount > 0);
		}
		SetPhase(EPhase::Duplicate);
	}

	void RequestCopy()
	{
		FOWTDuplicationOptions Options;
		Options.WorldOffset = FVector(125.0, 75.0, 15.0);
		RequestStarted = FPlatformTime::Seconds();
		OperationId = Editor->BeginDuplicateOperation(Editor->GetSnapshot(), Options);
		if (!OperationId.IsValid())
		{
			Fail(TEXT("Facade rejected the actual Blueprint duplication request."));
			return;
		}
		SetPhase(EPhase::Commit);
	}

	void WaitForCommit()
	{
		if (!Source.IsValid())
		{
			Fail(TEXT("Source destroyed while awaiting authored commit."));
			return;
		}
		for (const FOWTDuplicationOperationSnapshot& Operation : Editor->GetDuplicationOperations())
		{
			if (Operation.OperationId != OperationId)
			{
				continue;
			}
			if (Operation.Phase == EOWTDuplicationPhase::Failed)
			{
				Fail(Operation.Error);
				return;
			}
			if (Operation.Phase == EOWTDuplicationPhase::Cancelled)
			{
				Fail(TEXT("Actual Blueprint duplicate was unexpectedly cancelled."));
				return;
			}
			if (Operation.Phase != EOWTDuplicationPhase::Committed)
			{
				break;
			}
			Copy = Operation.DuplicateActor;
			if (!Copy.IsValid())
			{
				Fail(TEXT("Committed duplicate has no live Actor."));
				return;
			}
			Copy->Tags.Add(TemporaryTag);
			CommitTimes.Add((FPlatformTime::Seconds() - RequestStarted) * 1000.0);
			Test.TestTrue(TEXT("Committed duplicate automatically selected"), Hub->SelectedObject.Get() == Copy.Get());
			Test.TestTrue(TEXT("Source transform untouched by duplicate"),
			              Source->GetActorTransform().Equals(SourceTransform));
			Test.TestTrue(
			    TEXT("Duplicate world offset applied once"),
			    Copy->GetActorLocation().Equals(SourceTransform.GetLocation() + FVector(125.0, 75.0, 15.0), 0.01));
			FString SourceFamily;
			FString CopyFamily;
			int64 SourceNumber = 0;
			int64 CopyNumber = 0;
			SplitName(Source->GetName(), SourceFamily, SourceNumber);
			SplitName(Copy->GetName(), CopyFamily, CopyNumber);
			Test.TestEqual(TEXT("Duplicate retains original name family"), CopyFamily, SourceFamily);
			Test.TestTrue(TEXT("Duplicate increments numeric suffix"), CopyNumber > SourceNumber);
			Test.TestTrue(TEXT("Repeated duplicate suffix is monotonically increasing"),
			              CopyNumber > LastSuffixes.FindRef(SourceFamily));
			LastSuffixes.Add(SourceFamily, CopyNumber);
			SetPhase(EPhase::CopyGeneration);
			return;
		}
		if (HasTimedOut())
		{
			Fail(TEXT("Accepted duplication did not reach authored commit."));
		}
	}

	void WaitForCopy()
	{
		if (!Source.IsValid())
		{
			Fail(TEXT("Source destroyed while awaiting copy readiness."));
			return;
		}
		if (!Copy.IsValid())
		{
			Fail(TEXT("Copy destroyed while awaiting PCG completion."));
			return;
		}
		if (!GenerationFinished(*Copy))
		{
			if (HasTimedOut())
			{
				Fail(TEXT("Copy PCG generation did not finish."));
			}
			return;
		}
		ReadyTimes.Add((FPlatformTime::Seconds() - RequestStarted) * 1000.0);
		if (!CompareActor(Test, *Source, *Copy))
		{
			bAborting = true;
		}
		Test.TestEqual(TEXT("Independent generated/configured instance count preserved"), CountInstances(*Copy),
		               SourceInstanceCount);
		TArray<UPCGComponent*> Components;
		GetPCGComponents(*Copy, Components);
		for (UPCGComponent* PCG : Components)
		{
			TArray<UPCGComponent*> Originals;
			GetPCGComponents(*Source, Originals);
			TInlineComponentArray<UActorComponent*> CopiedComponents(Copy.Get());
			for (const UPCGComponent* Original : Originals)
			{
				for (const UActorComponent* CopiedComponent : CopiedComponents)
				{
					const UObject* Object = CopiedComponent;
					Test.TestFalse(TEXT("Source PCG does not own any copied component"),
					               Original->IsAnyObjectManagedByResource(MakeArrayView(&Object, 1)));
				}
			}
			bool bReadyObserved = false;
			for (const FOWTProceduralComponentSnapshot& State : Editor->GetProceduralComponents())
			{
				if (State.Component.Get() != PCG)
				{
					continue;
				}
				if (State.OperationId != OperationId)
				{
					continue;
				}
				bReadyObserved = State.State == EOWTProceduralState::Ready;
				Test.TestTrue(TEXT("PCG generation attempt observed"), State.GenerationAttempt > 0);
			}
			Test.TestTrue(TEXT("Facade observes copy PCG Ready"), bReadyObserved);
		}
		Coverage.FindChecked(FixtureNames[Cycle % UE_ARRAY_COUNT(FixtureNames)])++;
		SetPhase(EPhase::Cleanup);
	}

	bool CleanupPCG(AActor& Actor)
	{
		TArray<UPCGComponent*> Components;
		GetPCGComponents(Actor, Components);
		for (UPCGComponent* PCG : Components)
		{
			if (PCG->IsCleaningUp())
			{
				return false;
			}
			if (!PCG->AreManagedResourcesAccessible())
			{
				return false;
			}
		}
		for (UPCGComponent* PCG : Components)
		{
			PCG->CancelGeneration();
			PCG->CleanupLocalImmediate(true, true);
			Test.TestFalse(TEXT("Temporary copy generated resources cleaned"), PCG->bGenerated);
		}
		return true;
	}

	void Cleanup()
	{
		if (Copy.IsValid())
		{
			TArray<AActor*> Actors;
			GetAuthoredActors(*Copy, Actors);
			for (AActor* Actor : Actors)
			{
				RetiredActors.AddUnique(Actor);
				TInlineComponentArray<UActorComponent*> Components(Actor);
				for (UActorComponent* Component : Components)
				{
					RetiredComponents.AddUnique(Component);
				}
			}
			if (!CleanupPCG(*Copy))
			{
				if (HasTimedOut())
				{
					Test.AddError(
					    TEXT("Copy resources remained inaccessible; unsafe immediate destruction was avoided."));
					bAborting = true;
					SetPhase(EPhase::Settle);
				}
				return;
			}
			if (Source.IsValid())
			{
				Test.TestEqual(TEXT("Copy cleanup leaves source instances intact"), CountInstances(*Source),
				               SourceInstanceCount);
			}
			Hub->SetSelectedObject(Source.Get());
			Test.TestTrue(TEXT("Temporary duplicate destroyed"), Copy->Destroy());
			Copy.Reset();
		}
		for (const TWeakObjectPtr<UPCGComponent>& WeakPCG : GeneratedSources)
		{
			UPCGComponent* PCG = WeakPCG.Get();
			if (!PCG)
			{
				continue;
			}
			if (PCG->IsCleaningUp())
			{
				if (HasTimedOut())
				{
					Test.AddError(TEXT("Source cleanup remained busy; original authored Actor was retained."));
					bAborting = true;
					SetPhase(EPhase::Settle);
				}
				return;
			}
			if (!PCG->AreManagedResourcesAccessible())
			{
				if (HasTimedOut())
				{
					Test.AddError(TEXT("Source PCG resources stayed inaccessible during cleanup."));
					bAborting = true;
					SetPhase(EPhase::Settle);
				}
				return;
			}
			PCG->CancelGeneration();
			PCG->CleanupLocalImmediate(true, true);
		}
		GeneratedSources.Reset();
		for (const FSourcePCGState& Initial : InitialPCGStates)
		{
			UPCGComponent* PCG = Initial.Component.Get();
			if (!Test.TestNotNull(TEXT("Original PCG component retained"), PCG))
			{
				continue;
			}
			Test.TestEqual(TEXT("Original PCG graph unchanged"), PCG->GetGraph(), Initial.Graph.Get());
			Test.TestEqual(TEXT("Original PCG trigger unchanged"), PCG->GenerationTrigger, Initial.Trigger);
			Test.TestEqual(TEXT("Original PCG seed unchanged"), PCG->Seed, Initial.Seed);
			Test.TestEqual(TEXT("Original PCG activation unchanged"), PCG->bActivated, Initial.bActivated);
			Test.TestEqual(TEXT("Original PCG generation intent restored"), PCG->bGenerated,
			               Initial.bExpectedGenerated);
		}
		if (Source.IsValid())
		{
			if (bSourceTransformCaptured)
			{
				Source->SetActorTransform(SourceTransform, false, nullptr, ETeleportType::TeleportPhysics);
				FindFProperty<FStrProperty>(Source->GetClass(), TEXT("FixtureLabel"))
				    ->SetPropertyValue_InContainer(Source.Get(), OriginalLabel);
				FindFProperty<FIntProperty>(Source->GetClass(), TEXT("Revision"))
				    ->SetPropertyValue_InContainer(Source.Get(), OriginalRevision);
				Editor->RefreshSelectedTransform();
				bSourceTransformCaptured = false;
			}
		}
		++Cycle;
		const TArray<FOWTEventRecord> Events = Editor->GetMonitorEntries(10000);
		Test.TestTrue(TEXT("Event monitor storage remains bounded"), Events.Num() <= 256);
		for (int32 Index = 1; Index < Events.Num(); ++Index)
		{
			if (Events[Index].Sequence <= Events[Index - 1].Sequence)
			{
				Fail(TEXT("Event monitor sequence is not increasing."));
				return;
			}
		}
		Test.TestTrue(TEXT("Duplication operation history remains bounded"),
		              Editor->GetDuplicationOperations().Num() <= 256);
		SetPhase(bAborting ? EPhase::Settle : EPhase::Prepare);
	}

	void Settle()
	{
		if (FPlatformTime::Seconds() - PhaseStarted < 0.5)
		{
			return;
		}
		Hub->SetSelectedObject(OriginalSelection.Get());
		if (Hub->IsEditingEnabled() != bWasEditing)
		{
			Hub->ToggleEditing();
		}
		Test.TestEqual(TEXT("Original level fixture count restored"),
		               CountTaggedActors(*World, TEXT("OWT.Playground.Editable")), BaselineActorCount);
		Test.TestEqual(TEXT("No temporary root Actors remain"), CountTaggedActors(*World, TemporaryTag), 0);
		if (!bAborting)
		{
			Test.TestEqual(TEXT("One successful result event per cycle"), ResultEventCount, CycleCount);
			for (const auto& Entry : Coverage)
			{
				Test.TestTrue(*FString::Printf(TEXT("Repeated real BP coverage: %s"), *Entry.Key), Entry.Value >= 2);
			}
		}
		MemoryEnd = FPlatformMemory::GetStats().UsedPhysical;
		AllocatedActorsBeforeGC = CountTrackedObjects(RetiredActors, true);
		AllocatedComponentsBeforeGC = CountTrackedObjects(RetiredComponents, true);
		Editor->Unsubscribe(Subscription);
		Subscription.Invalidate();
		const double GCStarted = FPlatformTime::Seconds();
		CollectGarbage(RF_NoFlags, true);
		GCMilliseconds = (FPlatformTime::Seconds() - GCStarted) * 1000.0;
		PostGCFrames = 0;
		SetPhase(EPhase::PostGC);
	}

	void FinishGC()
	{
		++PostGCFrames;
		if (PostGCFrames < 8)
		{
			return;
		}
		if (FPlatformTime::Seconds() - PhaseStarted < 0.5)
		{
			return;
		}
		MemoryAfterGC = FPlatformMemory::GetStats().UsedPhysical;
		Test.TestEqual(TEXT("No live copied Actors after full GC"), CountTrackedObjects(RetiredActors, false), 0);
		Test.TestEqual(TEXT("No live copied components after full GC"), CountTrackedObjects(RetiredComponents, false),
		               0);
		Test.TestEqual(TEXT("Original level fixtures survive diagnostic GC"),
		               CountTaggedActors(*World, TEXT("OWT.Playground.Editable")), BaselineActorCount);
		Test.AddInfo(FString::Printf(
		    TEXT("Post-GC diagnostic: %.2f MiB before, %.2f MiB after; GC %.2f ms outside work timing; "
		         "remaining allocated copied Actors/components (including garbage): %d/%d."),
		    static_cast<double>(MemoryEnd) / (1024.0 * 1024.0), static_cast<double>(MemoryAfterGC) / (1024.0 * 1024.0),
		    GCMilliseconds, CountTrackedObjects(RetiredActors, true), CountTrackedObjects(RetiredComponents, true)));
		if (GUsingNullRHI)
		{
			Test.AddInfo(TEXT(
			    "Screenshot unavailable under NullRHI; gameplay assertions ran, visual output was not validated."));
			ScreenshotPath.Reset();
			WriteMetrics();
			SetPhase(EPhase::Complete);
			return;
		}
		FScreenshotRequest::RequestScreenshot(ScreenshotPath, true, false);
		SetPhase(EPhase::Screenshot);
	}

	void FinishScreenshot()
	{
		if (IFileManager::Get().FileExists(*ScreenshotPath))
		{
			Test.AddInfo(TEXT("Playground screenshot: ") + ScreenshotPath);
			WriteMetrics();
			SetPhase(EPhase::Complete);
			return;
		}
		if (HasTimedOut(15.0))
		{
			Test.AddError(TEXT("Actual RHI screenshot was requested but no PNG was written: ") + ScreenshotPath);
			WriteMetrics();
			SetPhase(EPhase::Complete);
		}
	}

	void WriteMetrics()
	{
		TSharedRef<FJsonObject> Json = MakeShared<FJsonObject>();
		Json->SetStringField(TEXT("test"), bStress ? TEXT("OWT.Playground.Stress") : TEXT("OWT.Playground.Smoke"));
		Json->SetStringField(TEXT("map"), World->GetOutermost()->GetName());
		Json->SetStringField(TEXT("screenshot"), ScreenshotPath);
		Json->SetBoolField(TEXT("nullRHI"), GUsingNullRHI);
		Json->SetBoolField(TEXT("aborted"), bAborting);
		Json->SetNumberField(TEXT("requestedCycles"), CycleCount);
		Json->SetNumberField(TEXT("committedCycles"), CommitTimes.Num());
		Json->SetNumberField(TEXT("validatedCycles"), ReadyTimes.Num());
		Json->SetNumberField(TEXT("elapsedSeconds"), FPlatformTime::Seconds() - RunStarted);
		Json->SetNumberField(TEXT("baselineMeanFrameMs"), Average(BaselineFrames));
		Json->SetNumberField(TEXT("baselineFrameSamples"), BaselineFrames.Num());
		Json->SetNumberField(TEXT("baselineP95FrameMs"), Percentile95(BaselineFrames));
		Json->SetNumberField(TEXT("workMeanFrameMs"), Average(WorkFrames));
		Json->SetNumberField(TEXT("workFrameSamples"), WorkFrames.Num());
		Json->SetNumberField(TEXT("workP95FrameMs"), Percentile95(WorkFrames));
		Json->SetNumberField(TEXT("requestToCommitMeanMs"), Average(CommitTimes));
		Json->SetNumberField(TEXT("requestToCommitP95Ms"), Percentile95(CommitTimes));
		Json->SetNumberField(TEXT("requestToReadyMeanMs"), Average(ReadyTimes));
		Json->SetNumberField(TEXT("requestToReadyP95Ms"), Percentile95(ReadyTimes));
		Json->SetNumberField(TEXT("physicalMemoryStartBytes"), static_cast<double>(MemoryStart));
		Json->SetNumberField(TEXT("physicalMemoryEndBytes"), static_cast<double>(MemoryEnd));
		Json->SetNumberField(TEXT("physicalMemoryDeltaBytes"),
		                     static_cast<double>(MemoryEnd) - static_cast<double>(MemoryStart));
		Json->SetNumberField(TEXT("physicalMemoryBeforeGCBytes"), static_cast<double>(MemoryEnd));
		Json->SetNumberField(TEXT("physicalMemoryAfterGCBytes"), static_cast<double>(MemoryAfterGC));
		Json->SetNumberField(TEXT("physicalMemoryAfterGCDeltaBytes"),
		                     static_cast<double>(MemoryAfterGC) - static_cast<double>(MemoryStart));
		Json->SetNumberField(TEXT("diagnosticGCMs"), GCMilliseconds);
		Json->SetNumberField(TEXT("postGCSettleFrames"), PostGCFrames);
		Json->SetStringField(
		    TEXT("memoryMeasurement"),
		    TEXT("Process physical memory. Legacy end/delta fields describe pre-GC memory. "
		         "Separate full-GC diagnostic waits at least eight frames and 0.5 seconds for normal render cleanup. "
		         "GC/settle are excluded from work-frame and operation timing. Renderer/cache/history allocations may "
		         "remain; no zero-delta guarantee."));
		Json->SetNumberField(TEXT("trackedCopiedActorRefs"), RetiredActors.Num());
		Json->SetNumberField(TEXT("trackedCopiedComponentRefs"), RetiredComponents.Num());
		Json->SetNumberField(TEXT("allocatedCopiedActorsBeforeGC"), AllocatedActorsBeforeGC);
		Json->SetNumberField(TEXT("allocatedCopiedComponentsBeforeGC"), AllocatedComponentsBeforeGC);
		Json->SetNumberField(TEXT("allocatedCopiedActorsAfterGC"), CountTrackedObjects(RetiredActors, true));
		Json->SetNumberField(TEXT("allocatedCopiedComponentsAfterGC"), CountTrackedObjects(RetiredComponents, true));
		Json->SetNumberField(TEXT("validCopiedActorRefsAfterGC"), CountTrackedObjects(RetiredActors, false));
		Json->SetNumberField(TEXT("validCopiedComponentRefsAfterGC"), CountTrackedObjects(RetiredComponents, false));
		Json->SetNumberField(TEXT("eventCount"), EventCount);
		Json->SetNumberField(TEXT("monitorEntries"), Editor->GetMonitorEntries(10000).Num());
		Json->SetNumberField(TEXT("operationRecords"), Editor->GetDuplicationOperations().Num());
		Json->SetNumberField(TEXT("initialEditableActors"), BaselineActorCount);
		Json->SetNumberField(TEXT("finalEditableActors"), CountTaggedActors(*World, TEXT("OWT.Playground.Editable")));
		Json->SetNumberField(TEXT("residualTemporaryActors"), CountTaggedActors(*World, TemporaryTag));
		TSharedRef<FJsonObject> FixtureCoverage = MakeShared<FJsonObject>();
		for (const auto& Entry : Coverage)
		{
			FixtureCoverage->SetNumberField(Entry.Key, Entry.Value);
		}
		Json->SetObjectField(TEXT("blueprintCoverage"), FixtureCoverage);
		TArray<TSharedPtr<FJsonValue>> Timings;
		for (int32 Index = 0; Index < CommitTimes.Num(); ++Index)
		{
			TSharedRef<FJsonObject> Timing = MakeShared<FJsonObject>();
			Timing->SetNumberField(TEXT("cycle"), Index);
			Timing->SetStringField(TEXT("fixture"), FixtureNames[Index % UE_ARRAY_COUNT(FixtureNames)]);
			Timing->SetNumberField(TEXT("requestToCommitMs"), CommitTimes[Index]);
			if (ReadyTimes.IsValidIndex(Index))
			{
				Timing->SetNumberField(TEXT("requestToReadyMs"), ReadyTimes[Index]);
			}
			Timings.Add(MakeShared<FJsonValueObject>(Timing));
		}
		Json->SetArrayField(TEXT("operationTimings"), Timings);
		FString Text;
		FJsonSerializer::Serialize(Json, TJsonWriterFactory<>::Create(&Text));
		Test.TestTrue(TEXT("Measured playground report saved"), FFileHelper::SaveStringToFile(Text, *MetricsPath));
		Test.AddInfo(TEXT("Playground metrics: ") + MetricsPath);
		Test.AddInfo(
		    FString::Printf(TEXT("%d/%d duplicate cycles, work p95 %.2f ms, request-to-commit p95 %.2f ms, physical "
		                         "memory delta %.2f MiB."),
		                    CommitTimes.Num(), CycleCount, Percentile95(WorkFrames), Percentile95(CommitTimes),
		                    (static_cast<double>(MemoryEnd) - static_cast<double>(MemoryStart)) / (1024.0 * 1024.0)));
	}

	FAutomationTestBase& Test;
	TWeakObjectPtr<UWorld> World;
	TWeakObjectPtr<UVTBOWTEditorSubsystem> Hub;
	TWeakObjectPtr<AVTBAttributeEditor> Editor;
	TWeakObjectPtr<AActor> OriginalSelection;
	TWeakObjectPtr<AActor> Source;
	TWeakObjectPtr<AActor> Copy;
	TArray<TWeakObjectPtr<AActor>> Sources;
	TArray<TWeakObjectPtr<AActor>> RetiredActors;
	TArray<TWeakObjectPtr<UActorComponent>> RetiredComponents;
	TArray<TWeakObjectPtr<UPCGComponent>> GeneratedSources;
	TArray<FSourcePCGState> InitialPCGStates;
	TMap<FString, int64> LastSuffixes;
	TMap<FString, int32> Coverage;
	TArray<double> BaselineFrames;
	TArray<double> WorkFrames;
	TArray<double> CommitTimes;
	TArray<double> ReadyTimes;
	EPhase Phase;
	FGuid OperationId;
	FGuid Subscription;
	FTransform SourceTransform;
	double RunStarted;
	double PhaseStarted;
	double RequestStarted;
	uint64 MemoryStart;
	uint64 MemoryEnd;
	uint64 MemoryAfterGC;
	double GCMilliseconds;
	int32 Cycle;
	int32 CycleCount;
	int32 BaselineActorCount;
	int32 SourceInstanceCount;
	int32 EventCount;
	int32 ResultEventCount;
	int32 FrameCount;
	int32 PostGCFrames;
	int32 AllocatedActorsBeforeGC;
	int32 AllocatedComponentsBeforeGC;
	bool bStress;
	bool bAborting;
	bool bWasEditing;
	bool bSourceTransformCaptured;
	FString OriginalLabel;
	int32 OriginalRevision;
	FString MetricsPath;
	FString ScreenshotPath;
};
} // namespace OWTPlaygroundTests

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTPlaygroundSmoke, "OWT.Playground.Smoke",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTPlaygroundSmoke::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(OWTPlaygroundTests::FRunPlayground(*this, false));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTPlaygroundStress, "OWT.Playground.Stress",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTPlaygroundStress::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(OWTPlaygroundTests::FRunPlayground(*this, true));
	return true;
}

#endif
