#include "OWTDuplicationValidation.h"

#include "Duplication/OWTRuntimeActorDuplicator.h"
#include "VTBAttributeEditor.h"
#include "Components/ChildActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EdGraph/EdGraph.h"
#include "EdGraphSchema_K2.h"
#include "Engine/Blueprint.h"
#include "Engine/Engine.h"
#include "Engine/EngineTypes.h"
#include "Engine/Level.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsConstraintTemplate.h"
#include "StaticMeshCompiler.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "K2Node_FunctionEntry.h"
#include "K2Node_VariableSet.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "KismetCompiler.h"
#include "UObject/StrongObjectPtr.h"
#include "UObject/UnrealType.h"

void UOWTDuplicationFixtureSceneComponent::OnRegister()
{
	Super::OnRegister();
	SetRelativeLocation(FVector::ZeroVector);
}

UOWTDuplicationFixtureData::UOWTDuplicationFixtureData()
    : ActorReference(nullptr), ComponentReference(nullptr), Nested(nullptr), Number(1)
{
}

AOWTDuplicationFixtureActor::AOWTDuplicationFixtureActor()
    : SceneRoot(nullptr), Mesh(nullptr), Data(nullptr), SelfReference(nullptr), ExternalReference(nullptr),
      References(), ReferenceMap(), UnloadedAsset(), InstanceNumber(3), bDestroyOnConstruction(false),
      bRejectDuplicateRestore(false), BeginPlayCount(0), NumberAtBeginPlay(0), BlueprintNumberAtBeginPlay(0),
      DataNumberAtBeginPlay(0), bReferencesValidAtBeginPlay(false), TransformAtBeginPlay(FTransform::Identity),
      RegisteredComponentLocationAtBeginPlay(FVector::ZeroVector), NativeNumberAtBeginPlay(0), NativeNumber(1)
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneRoot);
	Data = CreateDefaultSubobject<UOWTDuplicationFixtureData>(TEXT("Data"));
}

void AOWTDuplicationFixtureActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// Deliberately overwrite the source's later runtime edits on every fresh construction.
	InstanceNumber = 3;
	Mesh->SetRelativeLocation(FVector(5, 0, 0));
	if (bDestroyOnConstruction)
	{
		Destroy();
	}
}

void AOWTDuplicationFixtureActor::BeginPlay()
{
	Super::BeginPlay();
	++BeginPlayCount;
	NumberAtBeginPlay = InstanceNumber;
	NativeNumberAtBeginPlay = NativeNumber;
	DataNumberAtBeginPlay = Data ? Data->Number : -1;
	TransformAtBeginPlay = GetActorTransform();
	if (USceneComponent* Registered = FindObjectFast<USceneComponent>(this, TEXT("ResettingScene")))
	{
		RegisteredComponentLocationAtBeginPlay = Registered->GetRelativeLocation();
	}
	bReferencesValidAtBeginPlay = SelfReference == this;
	if (Data)
	{
		bReferencesValidAtBeginPlay &= Data->ActorReference == this;
		bReferencesValidAtBeginPlay &= Data->ComponentReference == Mesh;
	}
	if (const FIntProperty* Number = FindFProperty<FIntProperty>(GetClass(), TEXT("BlueprintNumber")))
	{
		BlueprintNumberAtBeginPlay = Number->GetPropertyValue_InContainer(this);
	}
}

bool AOWTDuplicationFixtureActor::RestoreRuntimeDuplicateState_Implementation(
    UObject* SourceObject, const TMap<UObject*, UObject*>& DuplicatedObjects, FString& OutError)
{
	if (bRejectDuplicateRestore)
	{
		OutError = TEXT("Fixture custom restoration rejection.");
		return false;
	}
	NativeNumber = CastChecked<AOWTDuplicationFixtureActor>(SourceObject)->NativeNumber;
	return true;
}

namespace
{
UBlueprint* CreateFixtureBlueprint()
{
	const FName Name =
	    MakeUniqueObjectName(GetTransientPackage(), UBlueprint::StaticClass(), TEXT("BP_OWTDuplicateFixture"));
	UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(AOWTDuplicationFixtureActor::StaticClass(),
	                                                                GetTransientPackage(), Name, BPTYPE_Normal);
	if (!Blueprint)
	{
		return nullptr;
	}
	USCS_Node* Node =
	    Blueprint->SimpleConstructionScript->CreateNode(UStaticMeshComponent::StaticClass(), TEXT("BPMesh"));
	Blueprint->SimpleConstructionScript->AddNode(Node);
	FEdGraphPinType NumberType;
	NumberType.PinCategory = UEdGraphSchema_K2::PC_Int;
	if (!FBlueprintEditorUtils::AddMemberVariable(Blueprint, TEXT("BlueprintNumber"), NumberType, TEXT("11")))
	{
		return nullptr;
	}
	UEdGraph* Graph = FBlueprintEditorUtils::FindUserConstructionScript(Blueprint);
	if (!Graph)
	{
		return nullptr;
	}
	UK2Node_FunctionEntry* Entry = nullptr;
	for (UEdGraphNode* GraphNode : Graph->Nodes)
	{
		if (UK2Node_FunctionEntry* Candidate = Cast<UK2Node_FunctionEntry>(GraphNode))
		{
			Entry = Candidate;
			break;
		}
	}
	if (!Entry)
	{
		return nullptr;
	}
	UK2Node_VariableSet* SetNumber = NewObject<UK2Node_VariableSet>(Graph);
	SetNumber->VariableReference.SetSelfMember(TEXT("BlueprintNumber"));
	Graph->AddNode(SetNumber, false, false);
	SetNumber->CreateNewGuid();
	SetNumber->PostPlacedNewNode();
	SetNumber->AllocateDefaultPins();
	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();
	Schema->TrySetDefaultValue(*SetNumber->FindPinChecked(TEXT("BlueprintNumber")), TEXT("11"));
	UEdGraphPin* Then = Entry->FindPinChecked(UEdGraphSchema_K2::PN_Then);
	Then->BreakAllPinLinks();
	if (!Schema->TryCreateConnection(Then, SetNumber->FindPinChecked(UEdGraphSchema_K2::PN_Execute)))
	{
		return nullptr;
	}
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
	FCompilerResultsLog Log;
	FKismetEditorUtilities::CompileBlueprint(Blueprint, EBlueprintCompileOptions::None, &Log);
	if (Log.NumErrors > 0)
	{
		return nullptr;
	}
	return Blueprint;
}

int32 CountLiveActors(UWorld& World)
{
	int32 Count = 0;
	for (TActorIterator<AActor> Actor(&World); Actor; ++Actor)
	{
		if (IsValid(*Actor))
		{
			++Count;
		}
	}
	return Count;
}

bool ValidateClass(UWorld& World, UOWTRuntimeActorDuplicator& Duplicator, UClass& Class, bool bBlueprint)
{
	AOWTDuplicationFixtureActor* Source = World.SpawnActor<AOWTDuplicationFixtureActor>(&Class);
	AOWTDuplicationFixtureActor* External = World.SpawnActor<AOWTDuplicationFixtureActor>();
	if (!Source)
	{
		return false;
	}
	if (!External)
	{
		Source->Destroy();
		return false;
	}

	Source->InstanceNumber = 97;
	Source->NativeNumber = 167;
	Source->SelfReference = Source;
	Source->ExternalReference = External;
	Source->Data->Number = 71;
	Source->Data->ActorReference = Source;
	Source->Data->ComponentReference = Source->Mesh;
	Source->Data->Nested = NewObject<UOWTDuplicationFixtureData>(Source->Data, TEXT("Nested"));
	Source->Data->Nested->Number = 43;
	Source->Data->Nested->ActorReference = Source;
	Source->References = {Source, Source->Mesh, Source->Data, External};
	Source->ReferenceMap.Add(Source, Source->Mesh);
	Source->ReferenceMap.Add(External, Source->Data);
	Source->UnloadedAsset = FSoftObjectPath(TEXT("/Game/NotLoadedForDuplication.Asset"));
	Source->Mesh->SetRelativeLocation(FVector(21, 32, 43));
	Source->Mesh->SetRenderCustomDepth(true);
	UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	Source->Mesh->SetStaticMesh(CubeMesh);
	Source->Tags.Add(TEXT("RuntimeInstanceTag"));

	UStaticMeshComponent* RuntimeMesh = NewObject<UStaticMeshComponent>(Source, TEXT("RuntimeMesh"));
	Source->AddInstanceComponent(RuntimeMesh);
	RuntimeMesh->SetupAttachment(Source->Mesh);
	RuntimeMesh->SetStaticMesh(CubeMesh);
	RuntimeMesh->SetRelativeLocation(FVector(50, 60, 70));
	RuntimeMesh->RegisterComponent();
	RuntimeMesh->SetVisibility(false);
	UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(UMaterial::GetDefaultMaterial(MD_Surface),
	                                                                      Source->Mesh, TEXT("RuntimeMaterial"));
	Material->PhysMaterial = GEngine->DefaultPhysMaterial;
	Material->PhysicalMaterialMap[0] = GEngine->DefaultPhysMaterial;
	Material->SetScalarParameterValue(TEXT("FixtureScalar"), 0.375f);
	Material->SetVectorParameterValue(TEXT("FixtureTint"), FLinearColor(0.2f, 0.4f, 0.6f));
	Source->Mesh->SetMaterial(0, Material);
	RuntimeMesh->SetMaterial(0, Material);
	Source->References.Add(RuntimeMesh);
	UOWTDuplicationFixtureSceneComponent* ResettingScene =
	    NewObject<UOWTDuplicationFixtureSceneComponent>(Source, TEXT("ResettingScene"));
	Source->AddInstanceComponent(ResettingScene);
	ResettingScene->SetupAttachment(Source->Mesh);
	ResettingScene->RegisterComponent();
	ResettingScene->SetRelativeLocation(FVector(61, 71, 81));

	FIntProperty* BlueprintNumber = FindFProperty<FIntProperty>(&Class, TEXT("BlueprintNumber"));
	UStaticMeshComponent* BPMesh = FindObjectFast<UStaticMeshComponent>(Source, TEXT("BPMesh"));
	if (bBlueprint)
	{
		if (!BlueprintNumber)
		{
			return false;
		}
		if (!BPMesh)
		{
			return false;
		}
		BlueprintNumber->SetPropertyValue_InContainer(Source, 109);
		BPMesh->SetStaticMesh(CubeMesh);
		BPMesh->SetRelativeLocation(FVector(17, 19, 23));
		BPMesh->SetRenderCustomDepth(true);
		Source->Data->Nested->ComponentReference = BPMesh;
	}

	Source->AttachToActor(External, FAttachmentTransformRules::KeepWorldTransform);
	const FTransform Before(FRotator(13, 27, 39), FVector(210, -320, 430), FVector(2, 3, 4));
	Source->SetActorTransform(Before);
	const FVector Offset(100, 0, 0);
	FTransform Expected = Before;
	Expected.AddToTranslation(Offset);
	FString Error;
	AOWTDuplicationFixtureActor* Copy =
	    Cast<AOWTDuplicationFixtureActor>(Duplicator.DuplicateActor(Source, Offset, Error));
	if (!Copy)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT duplicate fixture spawn: %s"), *Error);
		Source->Destroy();
		External->Destroy();
		return false;
	}
	if (!Copy->Data)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT duplicate lost its owned data."));
		Copy->Destroy();
		Source->Destroy();
		External->Destroy();
		return false;
	}
	if (!Copy->Data->Nested)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT duplicate lost its nested owned data."));
		Copy->Destroy();
		Source->Destroy();
		External->Destroy();
		return false;
	}
	if (Copy->References.Num() != 5)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT duplicate lost its reference array."));
		Copy->Destroy();
		Source->Destroy();
		External->Destroy();
		return false;
	}
	bool bValid = true;
	auto Verify = [&bValid, bBlueprint](bool Condition, const TCHAR* Message)
	{
		if (!Condition)
		{
			UE_LOG(LogTemp, Error, TEXT("OWT duplicate %s: %s"), bBlueprint ? TEXT("BP") : TEXT("Native"), Message);
			bValid = false;
		}
	};
	Verify(Copy->GetClass() == &Class, TEXT("class"));
	Verify(Copy->GetLevel() == Source->GetLevel(), TEXT("level"));
	Verify(Copy->GetActorTransform().Equals(Expected, 0.001), TEXT("world transform and scale"));
	Verify(Copy->TransformAtBeginPlay.Equals(Expected, 0.001), TEXT("transform restored before BeginPlay"));
	Verify(Copy->RegisteredComponentLocationAtBeginPlay.Equals(FVector(61, 71, 81)),
	       TEXT("component transform restored after custom OnRegister before BeginPlay"));
	USceneComponent* ResettingCopy = FindObjectFast<USceneComponent>(Copy, TEXT("ResettingScene"));
	Verify(ResettingCopy && ResettingCopy->GetRelativeLocation().Equals(FVector(61, 71, 81)),
	       TEXT("component transform survives custom OnRegister"));
	Verify(Copy->GetAttachParentActor() == nullptr, TEXT("external attachment detached"));
	Verify(Copy->GetOwner() == nullptr, TEXT("owner policy"));
	Verify(Copy->GetInstigator() == nullptr, TEXT("instigator policy"));
	Verify(Copy->InstanceNumber == 97, TEXT("instance number"));
	Verify(Copy->NativeNumber == 167, TEXT("custom native configuration extension"));
	Verify(Copy->NativeNumberAtBeginPlay == 167, TEXT("custom native configuration before BeginPlay"));
	Verify(Copy->NumberAtBeginPlay == 97, TEXT("instance number visible in BeginPlay"));
	Verify(Copy->BeginPlayCount == 1, TEXT("BeginPlay called once"));
	Verify(Copy->DataNumberAtBeginPlay == 71, TEXT("instanced data visible in BeginPlay"));
	Verify(Copy->bReferencesValidAtBeginPlay, TEXT("references remapped before BeginPlay"));
	Verify(Copy->SelfReference == Copy, TEXT("self reference"));
	Verify(Copy->ExternalReference == External, TEXT("external reference"));
	Verify(Copy->Data != Source->Data, TEXT("instanced object separation"));
	Verify(Copy->Data->Nested != Source->Data->Nested, TEXT("nested instanced object separation"));
	Verify(Copy->Data->Nested->Number == 43, TEXT("nested instanced value"));
	Verify(Copy->Data->Nested->ActorReference == Copy, TEXT("nested back reference"));
	Verify(Copy->References[0] == Copy, TEXT("array self reference"));
	Verify(Copy->References[1] == Copy->Mesh, TEXT("array component reference"));
	Verify(Copy->References[2] == Copy->Data, TEXT("array instanced reference"));
	Verify(Copy->References[3] == External, TEXT("array external reference"));
	Verify(Copy->ReferenceMap.FindRef(Copy) == Copy->Mesh, TEXT("map key rehash and component value"));
	Verify(Copy->ReferenceMap.FindRef(External) == Copy->Data, TEXT("map external key and instanced value"));
	Verify(Copy->UnloadedAsset.ToSoftObjectPath() == Source->UnloadedAsset.ToSoftObjectPath(),
	       TEXT("unloaded soft path"));
	Verify(Copy->Mesh->GetStaticMesh() == CubeMesh, TEXT("shared mesh asset reference"));
	Verify(Copy->Mesh->bRenderCustomDepth, TEXT("component instance rendering flag"));
	UMaterialInstanceDynamic* MaterialCopy = Cast<UMaterialInstanceDynamic>(Copy->Mesh->GetMaterial(0));
	Verify(MaterialCopy && MaterialCopy != Material, TEXT("owned dynamic material independence"));
	if (MaterialCopy)
	{
		Verify(MaterialCopy->Parent == Material->Parent, TEXT("dynamic material shared asset parent"));
		Verify(MaterialCopy->PhysMaterial == Material->PhysMaterial,
		       TEXT("dynamic material physical material override"));
		Verify(MaterialCopy->PhysicalMaterialMap[0] == Material->PhysicalMaterialMap[0],
		       TEXT("dynamic material physical material mask mapping"));
		Verify(FMath::IsNearlyEqual(MaterialCopy->K2_GetScalarParameterValue(TEXT("FixtureScalar")), 0.375f),
		       TEXT("dynamic material scalar override"));
		Verify(MaterialCopy->K2_GetVectorParameterValue(TEXT("FixtureTint")).Equals(FLinearColor(0.2f, 0.4f, 0.6f)),
		       TEXT("dynamic material vector override"));
		MaterialCopy->SetScalarParameterValue(TEXT("FixtureScalar"), 0.875f);
		Verify(FMath::IsNearlyEqual(Material->K2_GetScalarParameterValue(TEXT("FixtureScalar")), 0.375f),
		       TEXT("dynamic material source remains independent"));
	}
	Verify(Copy->Mesh->GetRelativeLocation().Equals(FVector(21, 32, 43)), TEXT("native component transform"));
	Verify(Copy->Tags.Contains(TEXT("RuntimeInstanceTag")), TEXT("Actor tags"));
	UStaticMeshComponent* RuntimeCopy = FindObjectFast<UStaticMeshComponent>(Copy, TEXT("RuntimeMesh"));
	Verify(RuntimeCopy != nullptr, TEXT("runtime-added component exists"));
	if (RuntimeCopy)
	{
		Verify(RuntimeCopy != RuntimeMesh, TEXT("runtime component independence"));
		Verify(RuntimeCopy->GetOwner() == Copy, TEXT("runtime component owner"));
		Verify(RuntimeCopy->IsRegistered(), TEXT("runtime component registration"));
		Verify(RuntimeCopy->GetAttachParent() == Copy->Mesh, TEXT("runtime component attachment"));
		Verify(RuntimeCopy->GetRelativeLocation().Equals(FVector(50, 60, 70)), TEXT("runtime component transform"));
		Verify(!RuntimeCopy->IsVisible(), TEXT("runtime component visibility"));
		Verify(Copy->References[4] == RuntimeCopy, TEXT("runtime component reference"));
		Verify(RuntimeCopy->GetMaterial(0) == MaterialCopy, TEXT("shared internal material reference remapped"));
	}
	if (bBlueprint)
	{
		Verify(BlueprintNumber->GetPropertyValue_InContainer(Copy) == 109, TEXT("BP value restored after UCS"));
		Verify(Copy->BlueprintNumberAtBeginPlay == 109, TEXT("BP value restored before BeginPlay"));
		UStaticMeshComponent* BPCopy = FindObjectFast<UStaticMeshComponent>(Copy, TEXT("BPMesh"));
		Verify(BPCopy != nullptr, TEXT("SCS component exists"));
		if (BPCopy)
		{
			Verify(BPCopy != BPMesh, TEXT("SCS component independence"));
			Verify(BPCopy->GetRelativeLocation().Equals(FVector(17, 19, 23)), TEXT("SCS component instance transform"));
			Verify(BPCopy->bRenderCustomDepth, TEXT("SCS instance rendering flag"));
			Verify(Copy->Data->Nested->ComponentReference == BPCopy, TEXT("instanced to SCS reference"));
		}
	}
	Verify(Copy->GetComponents().Num() == Source->GetComponents().Num(), TEXT("helper removed and component count"));
	Verify(Source->GetActorTransform().Equals(Before, 0.001), TEXT("source transform unchanged"));
	Verify(Source->Data->ActorReference == Source, TEXT("source references unchanged"));
	Copy->Data->Number = 999;
	Verify(Source->Data->Number == 71, TEXT("source owned object remains independent"));
	Source->Destroy();
	Verify(IsValid(Copy->Mesh), TEXT("copy survives source destruction"));
	Verify(Copy->Mesh->IsRegistered(), TEXT("copy registration survives source destruction"));
	Copy->Destroy();
	External->Destroy();
	return bValid;
}

bool ValidateHierarchy(UWorld& World, UOWTRuntimeActorDuplicator& Duplicator)
{
	AOWTDuplicationFixtureActor* Source = World.SpawnActor<AOWTDuplicationFixtureActor>();
	if (!Source)
	{
		return false;
	}
	UChildActorComponent* ChildComponent = NewObject<UChildActorComponent>(Source, TEXT("RuntimeChild"));
	Source->AddInstanceComponent(ChildComponent);
	ChildComponent->SetupAttachment(Source->Mesh);
	ChildComponent->SetRelativeLocation(FVector(30, 40, 50));
	ChildComponent->SetChildActorClass(AOWTDuplicationChildFixtureActor::StaticClass());
	ChildComponent->RegisterComponent();
	AOWTDuplicationFixtureActor* Child = Cast<AOWTDuplicationFixtureActor>(ChildComponent->GetChildActor());
	if (!Child)
	{
		Source->Destroy();
		return false;
	}
	UChildActorComponent* NestedComponent = NewObject<UChildActorComponent>(Child, TEXT("NestedChild"));
	Child->AddInstanceComponent(NestedComponent);
	NestedComponent->SetupAttachment(Child->Mesh);
	NestedComponent->SetRelativeLocation(FVector(15, 25, 35));
	NestedComponent->SetChildActorClass(AOWTDuplicationGrandchildFixtureActor::StaticClass());
	NestedComponent->RegisterComponent();
	AOWTDuplicationFixtureActor* Grandchild = Cast<AOWTDuplicationFixtureActor>(NestedComponent->GetChildActor());
	if (!Grandchild)
	{
		Source->Destroy();
		return false;
	}
	Source->SelfReference = Source;
	Source->ExternalReference = Child;
	Child->SelfReference = Child;
	Child->ExternalReference = Grandchild;
	Grandchild->SelfReference = Grandchild;
	Grandchild->ExternalReference = Source;
	Child->Data->ActorReference = Grandchild;
	Grandchild->Data->ComponentReference = Source->Mesh;
	Child->InstanceNumber = 211;
	Grandchild->InstanceNumber = 307;
	Child->NativeNumber = 401;
	Grandchild->NativeNumber = 503;
	Source->SetActorTransform(FTransform(FRotator(12, 24, 36), FVector(200, 300, 400), FVector(2, 3, 4)));
	const FVector Offset(70, 80, 90);
	FTransform ExpectedChild = Child->GetActorTransform();
	ExpectedChild.AddToTranslation(Offset);
	FTransform ExpectedGrandchild = Grandchild->GetActorTransform();
	ExpectedGrandchild.AddToTranslation(Offset);
	FString Error;
	AOWTDuplicationFixtureActor* Copy =
	    Cast<AOWTDuplicationFixtureActor>(Duplicator.DuplicateActor(Source, Offset, Error));
	bool bValid = Copy != nullptr;
	if (!Copy)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT duplicate hierarchy: %s"), *Error);
		Source->Destroy();
		return false;
	}
	auto Verify = [&bValid](bool bCondition, const TCHAR* Message)
	{
		if (!bCondition)
		{
			UE_LOG(LogTemp, Error, TEXT("OWT duplicate hierarchy: %s"), Message);
			bValid = false;
		}
	};
	UChildActorComponent* CopyComponent = FindObjectFast<UChildActorComponent>(Copy, TEXT("RuntimeChild"));
	AOWTDuplicationFixtureActor* CopyChild =
	    CopyComponent ? Cast<AOWTDuplicationFixtureActor>(CopyComponent->GetChildActor()) : nullptr;
	UChildActorComponent* CopyNested =
	    CopyChild ? FindObjectFast<UChildActorComponent>(CopyChild, TEXT("NestedChild")) : nullptr;
	AOWTDuplicationFixtureActor* CopyGrandchild =
	    CopyNested ? Cast<AOWTDuplicationFixtureActor>(CopyNested->GetChildActor()) : nullptr;
	Verify(CopyChild && CopyGrandchild, TEXT("nested child Actor reconstruction"));
	if (CopyChild && CopyGrandchild)
	{
		Verify(CopyChild != Child && CopyGrandchild != Grandchild, TEXT("child independence"));
		Verify(CopyChild->GetParentComponent() == CopyComponent, TEXT("child parent component"));
		Verify(CopyGrandchild->GetParentComponent() == CopyNested, TEXT("grandchild parent component"));
		Verify(Copy->ExternalReference == CopyChild && CopyChild->ExternalReference == CopyGrandchild &&
		           CopyGrandchild->ExternalReference == Copy,
		       TEXT("cross-hierarchy references"));
		Verify(CopyChild->Data->ActorReference == CopyGrandchild &&
		           CopyGrandchild->Data->ComponentReference == Copy->Mesh,
		       TEXT("cross-hierarchy instanced references"));
		Verify(CopyChild->NumberAtBeginPlay == 211 && CopyGrandchild->NumberAtBeginPlay == 307,
		       TEXT("child configuration before BeginPlay"));
		Verify(CopyChild->NativeNumberAtBeginPlay == 401 && CopyGrandchild->NativeNumberAtBeginPlay == 503,
		       TEXT("child extension before BeginPlay"));
		Verify(CopyChild->BeginPlayCount == 1 && CopyGrandchild->BeginPlayCount == 1, TEXT("child BeginPlay once"));
		Verify(CopyChild->GetActorTransform().Equals(ExpectedChild, 0.01) &&
		           CopyGrandchild->GetActorTransform().Equals(ExpectedGrandchild, 0.01),
		       TEXT("hierarchy world offset once"));
	}
	Copy->Destroy();
	const int32 BeforeFailure = CountLiveActors(World);
	Grandchild->bRejectDuplicateRestore = true;
	Verify(Duplicator.DuplicateActor(Source, Offset, Error) == nullptr, TEXT("nested extension rejection"));
	Verify(!Error.IsEmpty() && CountLiveActors(World) == BeforeFailure, TEXT("failed hierarchy rollback"));
	Grandchild->bRejectDuplicateRestore = false;
	Verify(Duplicator.DuplicateActor(Child, Offset, Error) == nullptr, TEXT("independent managed child rejected"));
	Source->Destroy();
	return bValid;
}

bool ValidatePhysics(UWorld& World, UOWTRuntimeActorDuplicator& Duplicator)
{
	AStaticMeshActor* Source = World.SpawnActor<AStaticMeshActor>();
	if (!Source)
	{
		return false;
	}
	UStaticMeshComponent* Mesh = Source->GetStaticMeshComponent();
	Mesh->SetMobility(EComponentMobility::Movable);
	UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	TArray<UStaticMesh*> Meshes{Cube};
	FStaticMeshCompilingManager::Get().FinishCompilation(Meshes);
	Mesh->SetStaticMesh(Cube);
	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	Source->SetActorLocation(FVector(500, 600, 700));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetPhysicsLinearVelocity(FVector(12, 23, 34));
	Mesh->SetPhysicsAngularVelocityInRadians(FVector(0.1, 0.2, 0.3));
	if (!Mesh->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp, Error,
		       TEXT("OWT physics source setup failed: simulate flag %d, valid body %d, world physics %d, compiling %d"),
		       Mesh->BodyInstance.bSimulatePhysics, Mesh->BodyInstance.IsValidBodyInstance(),
		       World.bShouldSimulatePhysics, Cube->IsCompiling());
		Source->Destroy();
		return false;
	}
	const FVector Velocity = Mesh->GetPhysicsLinearVelocity();
	const FVector AngularVelocity = Mesh->GetPhysicsAngularVelocityInRadians();
	const FVector Location = Source->GetActorLocation();
	FString Error;
	AStaticMeshActor* Copy = Cast<AStaticMeshActor>(Duplicator.DuplicateActor(Source, FVector(100, 0, 0), Error));
	bool bValid = Copy != nullptr;
	if (Copy)
	{
		UStaticMeshComponent* CopyMesh = Copy->GetStaticMeshComponent();
		auto Verify = [&bValid](bool bCondition, const FString& Message)
		{
			if (!bCondition)
			{
				UE_LOG(LogTemp, Error, TEXT("OWT physics duplicate: %s"), *Message);
				bValid = false;
			}
		};
		Verify(CopyMesh->IsSimulatingPhysics(), TEXT("simulation enabled"));
		Verify(Copy->GetActorLocation().Equals(Location + FVector(100, 0, 0), 0.001),
		       FString::Printf(TEXT("location %s expected %s"), *Copy->GetActorLocation().ToString(),
		                       *(Location + FVector(100, 0, 0)).ToString()));
		Verify(CopyMesh->GetPhysicsLinearVelocity().Equals(Velocity, 0.001),
		       FString::Printf(TEXT("linear velocity %s expected %s"), *CopyMesh->GetPhysicsLinearVelocity().ToString(),
		                       *Velocity.ToString()));
		Verify(CopyMesh->GetPhysicsAngularVelocityInRadians().Equals(AngularVelocity, 0.001),
		       FString::Printf(TEXT("angular velocity %s expected %s"),
		                       *CopyMesh->GetPhysicsAngularVelocityInRadians().ToString(),
		                       *AngularVelocity.ToString()));
		Verify(Source->GetActorLocation().Equals(Location, 0.001), TEXT("source unchanged"));
		Copy->Destroy();
	}
	Source->Destroy();
	if (!bValid)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT static mesh physics duplication failed: %s"), *Error);
	}
	return bValid;
}

bool ValidateGenericActors(UWorld& World, UOWTRuntimeActorDuplicator& Duplicator)
{
	FActorSpawnParameters Parameters;
	Parameters.Name = TEXT("OWTDuplicateChair");
	AActor* Source = World.SpawnActor<AActor>(AActor::StaticClass(), FTransform::Identity, Parameters);
	Parameters.Name = TEXT("OWTDuplicateChair_4");
	AActor* Collision = World.SpawnActor<AActor>(AActor::StaticClass(), FTransform::Identity, Parameters);
	if (!Source || !Collision)
	{
		return false;
	}
	FString Error;
	AActor* Copy = Duplicator.DuplicateActor(Source, FVector::ZeroVector, Error);
	AActor* Repeated = Copy ? Duplicator.DuplicateActor(Copy, FVector::ZeroVector, Error) : nullptr;
	bool bValid = Copy && Repeated;
	if (Copy && Repeated)
	{
		bValid &= Copy->GetRootComponent() == nullptr;
		bValid &= Copy->GetFName() == TEXT("OWTDuplicateChair_5");
		bValid &= Repeated->GetFName() == TEXT("OWTDuplicateChair_6");
		Copy->Destroy();
		Repeated->Destroy();
	}
	APawn* Pawn = World.SpawnActor<APawn>();
	if (Pawn)
	{
		Pawn->AutoPossessPlayer = EAutoReceiveInput::Disabled;
		Pawn->AutoPossessAI = EAutoPossessAI::Disabled;
		AActor* PawnCopy = Duplicator.DuplicateActor(Pawn, FVector::ZeroVector, Error);
		bValid &= PawnCopy != nullptr;
		if (PawnCopy)
		{
			PawnCopy->Destroy();
		}
		Pawn->AutoPossessAI = EAutoPossessAI::Spawned;
		const int32 Before = CountLiveActors(World);
		bValid &= Duplicator.DuplicateActor(Pawn, FVector::ZeroVector, Error) == nullptr;
		bValid &= !Error.IsEmpty() && CountLiveActors(World) == Before;
		Pawn->Destroy();
	}
	else
	{
		bValid = false;
	}
	UPhysicsConstraintComponent* Constraint = NewObject<UPhysicsConstraintComponent>(Source);
	Source->AddInstanceComponent(Constraint);
	const int32 BeforeConstraint = CountLiveActors(World);
	bValid &= Duplicator.DuplicateActor(Source, FVector::ZeroVector, Error) == nullptr;
	bValid &= Error.Contains(TEXT("PhysicsConstraintComponent"));
	bValid &= CountLiveActors(World) == BeforeConstraint;
	Source->Destroy();
	Collision->Destroy();
	AOWTDuplicationFixtureActor* UnsafeSource = World.SpawnActor<AOWTDuplicationFixtureActor>();
	UPhysicsConstraintTemplate* UnsafeData = NewObject<UPhysicsConstraintTemplate>(UnsafeSource);
	UnsafeSource->References.Add(UnsafeData);
	const int32 BeforeUnsafeData = CountLiveActors(World);
	bValid &= Duplicator.DuplicateActor(UnsafeSource, FVector::ZeroVector, Error) == nullptr;
	bValid &= Error.Contains(TEXT("ConstraintInstance"));
	bValid &= CountLiveActors(World) == BeforeUnsafeData;
	UnsafeSource->Destroy();
	AOWTDuplicationOpaqueFixtureActor* Opaque = World.SpawnActor<AOWTDuplicationOpaqueFixtureActor>();
	FComponentReference Reference;
	Reference.OtherActor = Opaque;
	Opaque->Payload = FInstancedStruct::Make(Reference);
	const int32 BeforeOpaque = CountLiveActors(World);
	bValid &= Duplicator.DuplicateActor(Opaque, FVector::ZeroVector, Error) == nullptr;
	bValid &= Error.Contains(TEXT("InstancedStruct"));
	bValid &= CountLiveActors(World) == BeforeOpaque;
	bValid &= Opaque->Payload.Get<FComponentReference>().OtherActor == Opaque;
	Opaque->Destroy();
	if (!bValid)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT generic duplication naming/rootless/Pawn validation failed: %s"), *Error);
	}
	return bValid;
}

} // namespace

bool ValidateOWTRuntimeDuplication()
{
	TStrongObjectPtr<UBlueprint> Blueprint(CreateFixtureBlueprint());
	if (!Blueprint.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("OWT duplicate fixture Blueprint could not be compiled."));
		return false;
	}
	const UWorld::InitializationValues Initialization = UWorld::InitializationValues()
	                                                        .AllowAudioPlayback(false)
	                                                        .CreatePhysicsScene(true)
	                                                        .CreateNavigation(false)
	                                                        .CreateAISystem(false)
	                                                        .ShouldSimulatePhysics(true);
	UWorld* World =
	    UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &Initialization);
	if (!World)
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	World->InitializeActorsForPlay(FURL());
	// A headless fixture has no GameMode. Enable the normal spawn-to-BeginPlay path explicitly.
	World->SetBegunPlay(true);
	AVTBAttributeEditor* Editor = World->SpawnActor<AVTBAttributeEditor>();
	if (!Editor)
	{
		World->EndPlay(EEndPlayReason::Quit);
		World->DestroyWorld(false);
		GEngine->DestroyWorldContext(World);
		return false;
	}
	UOWTRuntimeActorDuplicator* Duplicator = NewObject<UOWTRuntimeActorDuplicator>(Editor);
	TStrongObjectPtr<UOWTRuntimeActorDuplicator> KeepDuplicator(Duplicator);
	if (!Duplicator->Initialize(Editor))
	{
		return false;
	}
	bool bValid = ValidateClass(*World, *Duplicator, *AOWTDuplicationFixtureActor::StaticClass(), false);
	const bool bBlueprintValid = ValidateClass(*World, *Duplicator, *Blueprint->GeneratedClass.Get(), true);
	bValid &= bBlueprintValid;
	bValid &= ValidateHierarchy(*World, *Duplicator);
	bValid &= ValidateGenericActors(*World, *Duplicator);
	bValid &= ValidatePhysics(*World, *Duplicator);

	FString Error;
	UOWTRuntimeActorDuplicator* Uninitialized = NewObject<UOWTRuntimeActorDuplicator>(Editor);
	const int32 Before = CountLiveActors(*World);
	bValid &= Uninitialized->DuplicateActor(Editor, FVector::ZeroVector, Error) == nullptr;
	bValid &= !Error.IsEmpty();
	bValid &= !Uninitialized->Initialize(World);
	bValid &= CountLiveActors(*World) == Before;
	AOWTDuplicationFixtureActor* Rejecting = World->SpawnActor<AOWTDuplicationFixtureActor>();
	Rejecting->bDestroyOnConstruction = true;
	const int32 BeforeConstructionFailure = CountLiveActors(*World);
	bValid &= Duplicator->DuplicateActor(Rejecting, FVector::ZeroVector, Error) == nullptr;
	bValid &= !Error.IsEmpty();
	bValid &= IsValid(Rejecting);
	bValid &= CountLiveActors(*World) == BeforeConstructionFailure;
	Rejecting->Destroy();
	Duplicator->Deinitialize();
	bValid &= Duplicator->GetWorld() == nullptr;
	bValid &= Duplicator->DuplicateActor(Editor, FVector::ZeroVector, Error) == nullptr;
	bValid &= !Error.IsEmpty();

	World->EndPlay(EEndPlayReason::Quit);
	World->DestroyWorld(false);
	GEngine->DestroyWorldContext(World);
	UE_LOG(LogTemp, Display, TEXT("OWT_DUPLICATION_VALIDATION: %s"), bValid ? TEXT("PASS") : TEXT("FAIL"));
	return bValid;
}
