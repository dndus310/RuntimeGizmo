#include "CreateOWTPlaygroundCommandlet.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/ChildActorComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Dom/JsonObject.h"
#include "EdGraphSchema_K2.h"
#include "Engine/Blueprint.h"
#include "Engine/DirectionalLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/Level.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/SkyLight.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextRenderActor.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/WorldSettings.h"
#include "HAL/FileManager.h"
#include "K2Node_CallFunction.h"
#include "K2Node_FunctionEntry.h"
#include "K2Node_VariableGet.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "KismetCompiler.h"
#include "Materials/Material.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Misc/FileHelper.h"
#include "Misc/PackageName.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "Misc/ScopeExit.h"
#include "PCGComponent.h"
#include "PCGGraph.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "UObject/MetaData.h"
#include "UObject/SavePackage.h"
#include "UObject/UnrealType.h"

namespace
{
constexpr int32 MainActorCount = 54;
constexpr int32 StressActorCount = 600;
const FString ContentRoot = TEXT("/Game/OWTPlayground/");
const FName EnvironmentTag(TEXT("OWT.Playground.Environment"));
const FName EditableTag(TEXT("OWT.Playground.Editable"));

struct FFixtureDefinition
{
	FFixtureDefinition(const TCHAR* InName, const TCHAR* InTitle, const TCHAR* InDescription, int32 InColor)
	    : Name(InName), Title(InTitle), Description(InDescription), Color(InColor)
	{
	}
	FString Name;
	FString Title;
	FString Description;
	int32 Color;
};

const TArray<FFixtureDefinition> Fixtures = {
    {TEXT("ColorBlock"), TEXT("01  /  PRIMITIVES"), TEXT("Movable mesh  /  collision  /  material"), 0},
    {TEXT("StackedSculpture"), TEXT("02  /  HIERARCHY"), TEXT("Nested scene nodes  /  mixed local transforms"), 1},
    {TEXT("LightRig"), TEXT("03  /  LIGHTING"), TEXT("Point + spot lights  /  shadow-free accents"), 2},
    {TEXT("InstanceArray"), TEXT("04  /  INSTANCES"), TEXT("Six independent ISM instances"), 3},
    {TEXT("HierarchicalArray"), TEXT("05  /  HISM"), TEXT("Nine instances  /  hierarchical component"), 4},
    {TEXT("SplineDisplay"), TEXT("06  /  SPLINE"), TEXT("Four control points  /  authored mesh markers"), 5},
    {TEXT("ChildAssembly"), TEXT("07  /  CHILD ACTOR"), TEXT("Owned Blueprint child  /  assembly transform"), 6},
    {TEXT("PhysicsProp"), TEXT("08  /  PHYSICS"), TEXT("Physics-ready body  /  simulation off for editing"), 7},
    {TEXT("PCGDisplay"), TEXT("09  /  PROCEDURAL"), TEXT("PCG configuration  /  generate on demand"), 0}};

const TArray<FString> ColorNames = {TEXT("Coral"),     TEXT("Azure"), TEXT("Amber"),   TEXT("Mint"),
                                    TEXT("Violet"),    TEXT("Cyan"),  TEXT("Rose"),    TEXT("Lime"),
                                    TEXT("Porcelain"), TEXT("Slate"), TEXT("Backdrop")};
const TArray<FLinearColor> Colors = {
    FLinearColor(0.95f, 0.18f, 0.10f),   FLinearColor(0.08f, 0.40f, 0.95f), FLinearColor(1.0f, 0.57f, 0.08f),
    FLinearColor(0.12f, 0.74f, 0.48f),   FLinearColor(0.50f, 0.22f, 0.86f), FLinearColor(0.08f, 0.66f, 0.84f),
    FLinearColor(0.90f, 0.19f, 0.48f),   FLinearColor(0.62f, 0.83f, 0.14f), FLinearColor(0.50f, 0.57f, 0.64f),
    FLinearColor(0.065f, 0.095f, 0.15f), FLinearColor(0.27f, 0.30f, 0.34f)};

FString BlueprintPath(const FFixtureDefinition& Fixture)
{
	return ContentRoot + TEXT("Blueprints/BP_OWT") + Fixture.Name;
}

FString MaterialPath(int32 Index)
{
	return ContentRoot + TEXT("Materials/MI_OWT") + ColorNames[Index];
}

FString ManifestFilename()
{
	return FPaths::ProjectContentDir() / TEXT("OWTPlayground/OWTPlayground.generated.json");
}

TArray<FString> GeneratedPackages()
{
	TArray<FString> Packages;
	Packages.Add(ContentRoot + TEXT("Materials/M_OWTPlaygroundSurface"));
	for (int32 Index = 0; Index < ColorNames.Num(); ++Index)
	{
		Packages.Add(MaterialPath(Index));
	}
	for (const FFixtureDefinition& Fixture : Fixtures)
	{
		Packages.Add(BlueprintPath(Fixture));
	}
	Packages.Add(ContentRoot + TEXT("Maps/L_OWTPlayground"));
	Packages.Add(ContentRoot + TEXT("Maps/L_OWTStress"));
	return Packages;
}

FString AssetFilename(const FString& PackagePath)
{
	const FString Extension = PackagePath.Contains(TEXT("/Maps/")) ? FPackageName::GetMapPackageExtension()
	                                                               : FPackageName::GetAssetPackageExtension();
	return FPackageName::LongPackageNameToFilename(PackagePath, Extension);
}

bool WriteManifest(bool bComplete)
{
	TSharedRef<FJsonObject> Object = MakeShared<FJsonObject>();
	Object->SetStringField(TEXT("generator"), TEXT("CreateOWTPlayground"));
	Object->SetNumberField(TEXT("version"), 1);
	Object->SetBoolField(TEXT("complete"), bComplete);
	Object->SetNumberField(TEXT("mainEditableActors"), MainActorCount);
	Object->SetNumberField(TEXT("stressEditableActors"), StressActorCount);
	Object->SetStringField(TEXT("gameMode"), TEXT("/Script/simple_proj.OWTPlaygroundGameMode"));
	Object->SetStringField(TEXT("editableTag"), EditableTag.ToString());
	Object->SetStringField(TEXT("environmentTag"), EnvironmentTag.ToString());
	Object->SetStringField(TEXT("pcgGraph"), TEXT("/OWTRuntimeEditing/Tests/PCG_OWTDuplicate"));
	Object->SetStringField(TEXT("pcgTrigger"),
	                       TEXT("Blueprint/Stress: GenerateOnDemand; Main placed instances: GenerateOnLoad"));
	Object->SetBoolField(TEXT("physicsSimulationInitiallyEnabled"), false);
	Object->SetStringField(TEXT("editableVariables"), TEXT("FixtureLabel:string, Revision:int, Accent:LinearColor"));
	Object->SetStringField(TEXT("constructionGraph"), TEXT("BP_OWTLightRig: WarmPoint.SetLightColor(Accent)"));
	TArray<TSharedPtr<FJsonValue>> Assets;
	for (const FString& Path : GeneratedPackages())
	{
		Assets.Add(MakeShared<FJsonValueString>(Path));
	}
	Object->SetArrayField(TEXT("assets"), Assets);
	TArray<TSharedPtr<FJsonValue>> Archetypes;
	for (const FFixtureDefinition& Fixture : Fixtures)
	{
		TSharedRef<FJsonObject> Entry = MakeShared<FJsonObject>();
		Entry->SetStringField(TEXT("name"), Fixture.Name);
		Entry->SetStringField(TEXT("blueprint"), BlueprintPath(Fixture));
		Entry->SetStringField(TEXT("tag"), TEXT("OWT.Fixture.") + Fixture.Name);
		Entry->SetStringField(TEXT("components"), Fixture.Description);
		Archetypes.Add(MakeShared<FJsonValueObject>(Entry));
	}
	Object->SetArrayField(TEXT("archetypes"), Archetypes);
	FString Json;
	FJsonSerializer::Serialize(Object, TJsonWriterFactory<>::Create(&Json));
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(ManifestFilename()), true);
	return FFileHelper::SaveStringToFile(Json, *ManifestFilename());
}

bool PrepareOutput(bool bRegenerate)
{
	TSharedPtr<FJsonObject> PreviousManifest;
	FString Json;
	if (FFileHelper::LoadFileToString(Json, *ManifestFilename()))
	{
		FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Json), PreviousManifest);
	}
	TSet<FString> PreviouslyGenerated;
	if (PreviousManifest)
	{
		FString Generator;
		PreviousManifest->TryGetStringField(TEXT("generator"), Generator);
		if (Generator == TEXT("CreateOWTPlayground"))
		{
			const TArray<TSharedPtr<FJsonValue>>* Assets = nullptr;
			if (PreviousManifest->TryGetArrayField(TEXT("assets"), Assets))
			{
				for (const auto& Asset : *Assets)
				{
					PreviouslyGenerated.Add(Asset->AsString());
				}
			}
		}
	}
	const TArray<FString> Packages = GeneratedPackages();
	for (const FString& Path : Packages)
	{
		if (!FPaths::FileExists(AssetFilename(Path)))
		{
			continue;
		}
		if (!bRegenerate)
		{
			UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND: Existing asset protected: %s. Back up before -Regenerate."),
			       *Path);
			return false;
		}
		if (!PreviouslyGenerated.Contains(Path))
		{
			UE_LOG(LogTemp, Error,
			       TEXT("OWT_PLAYGROUND: Cannot replace an asset absent from this generator's manifest: %s"), *Path);
			return false;
		}
		if (FindPackage(nullptr, *Path))
		{
			UE_LOG(LogTemp, Error,
			       TEXT("OWT_PLAYGROUND: Regeneration requires an unloaded package/fresh commandlet: %s"), *Path);
			return false;
		}
	}
	for (const FString& Path : Packages)
	{
		check(Path.StartsWith(ContentRoot));
		const FString Filename = AssetFilename(Path);
		if (!FPaths::FileExists(Filename))
		{
			continue;
		}
		// Only these exact manifest-owned files are replaced. No directory or unrelated asset is deleted.
		if (!IFileManager::Get().Delete(*Filename, false, true))
		{
			return false;
		}
		for (const TCHAR* Extension : {TEXT("uexp"), TEXT("ubulk")})
		{
			const FString Sidecar = FPaths::ChangeExtension(Filename, Extension);
			if (FPaths::FileExists(Sidecar))
			{
				if (!IFileManager::Get().Delete(*Sidecar, false, true))
				{
					return false;
				}
			}
		}
	}
	return WriteManifest(false);
}

bool SaveAsset(UObject& Asset)
{
	UPackage* Package = Asset.GetOutermost();
	Package->GetMetaData().SetValue(&Asset, TEXT("OWTGenerator"), TEXT("CreateOWTPlayground"));
	Package->MarkPackageDirty();
	const FString Filename = AssetFilename(Package->GetName());
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(Filename), true);
	FSavePackageArgs Args;
	Args.TopLevelFlags = RF_Public | RF_Standalone;
	Args.SaveFlags = SAVE_NoError;
	FAssetRegistryModule::AssetCreated(&Asset);
	return UPackage::SavePackage(Package, &Asset, *Filename, Args);
}

UMaterial* CreateSurfaceMaterial()
{
	const FString Path = ContentRoot + TEXT("Materials/M_OWTPlaygroundSurface");
	UMaterial* Material =
	    NewObject<UMaterial>(CreatePackage(*Path), TEXT("M_OWTPlaygroundSurface"), RF_Public | RF_Standalone);
	UMaterialExpressionVectorParameter* Color = NewObject<UMaterialExpressionVectorParameter>(Material);
	Color->ParameterName = TEXT("SurfaceColor");
	Color->DefaultValue = FLinearColor::White;
	UMaterialExpressionScalarParameter* Roughness = NewObject<UMaterialExpressionScalarParameter>(Material);
	Roughness->ParameterName = TEXT("Roughness");
	Roughness->DefaultValue = 0.42f;
	UMaterialExpressionVectorParameter* Fill = NewObject<UMaterialExpressionVectorParameter>(Material);
	Fill->ParameterName = TEXT("FillColor");
	Fill->DefaultValue = FLinearColor(0.025f, 0.025f, 0.025f);
	Material->GetExpressionCollection().AddExpression(Color);
	Material->GetExpressionCollection().AddExpression(Roughness);
	Material->GetExpressionCollection().AddExpression(Fill);
	Material->GetEditorOnlyData()->BaseColor.Connect(0, Color);
	Material->GetEditorOnlyData()->Roughness.Connect(0, Roughness);
	Material->GetEditorOnlyData()->EmissiveColor.Connect(0, Fill);
	bool bNeedsRecompile = false;
	if (!Material->SetMaterialUsage(bNeedsRecompile, MATUSAGE_InstancedStaticMeshes))
	{
		UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND: Could not enable instanced static mesh material usage."));
		return nullptr;
	}
	Material->PostEditChange();
	return SaveAsset(*Material) ? Material : nullptr;
}

TArray<UMaterialInstanceConstant*> CreatePalette(UMaterial& Parent)
{
	TArray<UMaterialInstanceConstant*> Palette;
	for (int32 Index = 0; Index < Colors.Num(); ++Index)
	{
		const FString Path = MaterialPath(Index);
		UMaterialInstanceConstant* Material = NewObject<UMaterialInstanceConstant>(
		    CreatePackage(*Path), *FPackageName::GetShortName(Path), RF_Public | RF_Standalone);
		Material->SetParentEditorOnly(&Parent);
		Material->SetVectorParameterValueEditorOnly(TEXT("SurfaceColor"), Colors[Index]);
		Material->SetVectorParameterValueEditorOnly(TEXT("FillColor"), Colors[Index] * 0.12f);
		Material->PostEditChange();
		if (!SaveAsset(*Material))
		{
			return {};
		}
		Palette.Add(Material);
	}
	return Palette;
}

USCS_Node* AddSceneNode(UBlueprint& Blueprint, UClass* Class, const TCHAR* Name, USCS_Node* Parent)
{
	USCS_Node* Node = Blueprint.SimpleConstructionScript->CreateNode(Class, Name);
	if (Parent)
	{
		Parent->AddChildNode(Node);
	}
	else
	{
		Blueprint.SimpleConstructionScript->AddNode(Node);
	}
	if (USceneComponent* Scene = Cast<USceneComponent>(Node->ComponentTemplate))
	{
		Scene->SetMobility(EComponentMobility::Movable);
	}
	return Node;
}

USCS_Node* AddMesh(UBlueprint& Blueprint, USCS_Node* Parent, const TCHAR* Name, UStaticMesh& Mesh,
                   UMaterialInterface& Material, FVector Location, FVector Scale)
{
	USCS_Node* Node = AddSceneNode(Blueprint, UStaticMeshComponent::StaticClass(), Name, Parent);
	UStaticMeshComponent* Component = CastChecked<UStaticMeshComponent>(Node->ComponentTemplate);
	Component->SetStaticMesh(&Mesh);
	Component->SetMaterial(0, &Material);
	Component->SetRelativeLocation(Location);
	Component->SetRelativeScale3D(Scale);
	Component->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	return Node;
}

bool AddFixtureVariables(UBlueprint& Blueprint, const FFixtureDefinition& Fixture)
{
	FEdGraphPinType LabelType;
	LabelType.PinCategory = UEdGraphSchema_K2::PC_String;
	if (!FBlueprintEditorUtils::AddMemberVariable(&Blueprint, TEXT("FixtureLabel"), LabelType, Fixture.Name))
	{
		return false;
	}
	FEdGraphPinType RevisionType;
	RevisionType.PinCategory = UEdGraphSchema_K2::PC_Int;
	if (!FBlueprintEditorUtils::AddMemberVariable(&Blueprint, TEXT("Revision"), RevisionType, TEXT("1")))
	{
		return false;
	}
	FEdGraphPinType AccentType;
	AccentType.PinCategory = UEdGraphSchema_K2::PC_Struct;
	AccentType.PinSubCategoryObject = TBaseStructure<FLinearColor>::Get();
	if (!FBlueprintEditorUtils::AddMemberVariable(&Blueprint, TEXT("Accent"), AccentType,
	                                              Colors[Fixture.Color].ToString()))
	{
		return false;
	}
	for (const FName Variable : {FName(TEXT("FixtureLabel")), FName(TEXT("Revision")), FName(TEXT("Accent"))})
	{
		FBlueprintEditorUtils::SetBlueprintOnlyEditableFlag(&Blueprint, Variable, false);
		FBlueprintEditorUtils::SetBlueprintVariableCategory(&Blueprint, Variable, nullptr,
		                                                    FText::FromString(TEXT("Playground")));
	}
	return true;
}

bool AddLightConstructionGraph(UBlueprint& Blueprint)
{
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(&Blueprint);
	UEdGraph* Graph = FBlueprintEditorUtils::FindUserConstructionScript(&Blueprint);
	if (!Graph)
	{
		return false;
	}
	UK2Node_FunctionEntry* Entry = nullptr;
	for (UEdGraphNode* Node : Graph->Nodes)
	{
		if (UK2Node_FunctionEntry* Candidate = Cast<UK2Node_FunctionEntry>(Node))
		{
			Entry = Candidate;
			break;
		}
	}
	if (!Entry)
	{
		return false;
	}
	auto AddGetter = [Graph](FName Variable, int32 Y)
	{
		UK2Node_VariableGet* Getter = NewObject<UK2Node_VariableGet>(Graph);
		Getter->VariableReference.SetSelfMember(Variable);
		Graph->AddNode(Getter, false, false);
		Getter->CreateNewGuid();
		Getter->PostPlacedNewNode();
		Getter->AllocateDefaultPins();
		Getter->NodePosX = 240;
		Getter->NodePosY = Y;
		return Getter;
	};
	UK2Node_VariableGet* Light = AddGetter(TEXT("WarmPoint"), 160);
	UK2Node_VariableGet* Accent = AddGetter(TEXT("Accent"), 300);
	UK2Node_CallFunction* Call = NewObject<UK2Node_CallFunction>(Graph);
	Call->SetFromFunction(ULightComponent::StaticClass()->FindFunctionByName(TEXT("SetLightColor")));
	Graph->AddNode(Call, false, false);
	Call->CreateNewGuid();
	Call->PostPlacedNewNode();
	Call->AllocateDefaultPins();
	Call->NodePosX = 560;
	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();
	UEdGraphPin* Then = Entry->FindPinChecked(UEdGraphSchema_K2::PN_Then);
	Then->BreakAllPinLinks();
	if (!Schema->TryCreateConnection(Then, Call->FindPinChecked(UEdGraphSchema_K2::PN_Execute)))
	{
		return false;
	}
	if (!Schema->TryCreateConnection(Light->FindPinChecked(TEXT("WarmPoint")),
	                                 Call->FindPinChecked(UEdGraphSchema_K2::PN_Self)))
	{
		return false;
	}
	if (!Schema->TryCreateConnection(Accent->FindPinChecked(TEXT("Accent")),
	                                 Call->FindPinChecked(TEXT("NewLightColor"))))
	{
		return false;
	}
	return true;
}

UBlueprint* CreateFixture(int32 Index, const TArray<UMaterialInstanceConstant*>& Palette, UClass* ChildClass,
                          UStaticMesh& Cube, UStaticMesh& Sphere, UStaticMesh& Cone, UPCGGraph& Graph)
{
	const FFixtureDefinition& Fixture = Fixtures[Index];
	const FString Path = BlueprintPath(Fixture);
	UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(AActor::StaticClass(), CreatePackage(*Path),
	                                                                *FPackageName::GetShortName(Path), BPTYPE_Normal);
	if (!Blueprint)
	{
		return nullptr;
	}
	UMaterialInterface& Color = *Palette[Fixture.Color];
	USCS_Node* Root;
	if (Index == 0)
	{
		Root = AddMesh(*Blueprint, nullptr, TEXT("ColorMesh"), Cube, Color, FVector::ZeroVector, FVector(0.85));
	}
	else if (Index == 7)
	{
		Root = AddMesh(*Blueprint, nullptr, TEXT("PhysicsBody"), Cube, Color, FVector::ZeroVector, FVector(0.8));
		UStaticMeshComponent* Body = CastChecked<UStaticMeshComponent>(Root->ComponentTemplate);
		Body->SetCollisionProfileName(TEXT("PhysicsActor"));
		Body->SetSimulatePhysics(false);
		Body->SetEnableGravity(true);
		Body->SetMassOverrideInKg(NAME_None, 12.0f, true);
	}
	else
	{
		Root = AddSceneNode(*Blueprint, USceneComponent::StaticClass(), TEXT("AssemblyRoot"), nullptr);
		AddMesh(*Blueprint, Root, TEXT("Pedestal"), Cube, *Palette[9], FVector(0, 0, -25), FVector(1.3, 1.3, 0.18));
	}
	if (Index == 1)
	{
		USCS_Node* Base =
		    AddMesh(*Blueprint, Root, TEXT("NestedBase"), Cube, Color, FVector(0, 0, 0), FVector(1.0, 1.0, 0.35));
		USCS_Node* Pivot = AddSceneNode(*Blueprint, USceneComponent::StaticClass(), TEXT("UpperPivot"), Base);
		CastChecked<USceneComponent>(Pivot->ComponentTemplate)->SetRelativeLocation(FVector(0, 0, 150));
		AddMesh(*Blueprint, Pivot, TEXT("Sphere"), Sphere, *Palette[2], FVector::ZeroVector, FVector(0.65, 0.65, 1.0));
		AddMesh(*Blueprint, Pivot, TEXT("Crown"), Cone, Color, FVector(0, 0, 140), FVector(0.5, 0.5, 1.0));
	}
	if (Index == 2)
	{
		AddMesh(*Blueprint, Root, TEXT("LampStem"), Cube, *Palette[9], FVector(0, 0, 20), FVector(0.18, 0.18, 1.0));
		AddMesh(*Blueprint, Root, TEXT("LampShade"), Cone, Color, FVector(0, 0, 80), FVector(0.75, 0.75, 0.5));
		USCS_Node* PointNode = AddSceneNode(*Blueprint, UPointLightComponent::StaticClass(), TEXT("WarmPoint"), Root);
		UPointLightComponent* Point = CastChecked<UPointLightComponent>(PointNode->ComponentTemplate);
		Point->SetRelativeLocation(FVector(0, 0, 50));
		Point->SetIntensity(180.0f);
		Point->SetLightColor(FLinearColor(1.0f, 0.72f, 0.38f));
		Point->SetAttenuationRadius(240.0f);
		Point->SetCastShadows(false);
		USCS_Node* SpotNode = AddSceneNode(*Blueprint, USpotLightComponent::StaticClass(), TEXT("DownSpot"), Root);
		USpotLightComponent* Spot = CastChecked<USpotLightComponent>(SpotNode->ComponentTemplate);
		Spot->SetRelativeLocation(FVector(0, 0, 75));
		Spot->SetRelativeRotation(FRotator(-90, 0, 0));
		Spot->SetIntensity(240.0f);
		Spot->SetAttenuationRadius(300.0f);
		Spot->SetOuterConeAngle(38.0f);
		Spot->SetCastShadows(false);
	}
	if (Index == 3 || Index == 4)
	{
		UClass* ComponentClass = Index == 3 ? UInstancedStaticMeshComponent::StaticClass()
		                                    : UHierarchicalInstancedStaticMeshComponent::StaticClass();
		USCS_Node* InstancesNode = AddSceneNode(*Blueprint, ComponentClass, TEXT("Instances"), Root);
		UInstancedStaticMeshComponent* Instances =
		    CastChecked<UInstancedStaticMeshComponent>(InstancesNode->ComponentTemplate);
		Instances->SetStaticMesh(Index == 3 ? &Cube : &Sphere);
		Instances->SetMaterial(0, &Color);
		Instances->SetCollisionProfileName(TEXT("BlockAllDynamic"));
		const int32 Count = Index == 3 ? 6 : 9;
		for (int32 Instance = 0; Instance < Count; ++Instance)
		{
			Instances->AddInstance(FTransform(FRotator(0, Instance * 13, 0),
			                                  FVector((Instance % 3 - 1) * 40, (Instance / 3 - 1) * 40, 20),
			                                  FVector(0.26)));
		}
	}
	if (Index == 5)
	{
		USCS_Node* SplineNode = AddSceneNode(*Blueprint, USplineComponent::StaticClass(), TEXT("RouteSpline"), Root);
		USplineComponent* Spline = CastChecked<USplineComponent>(SplineNode->ComponentTemplate);
		Spline->ClearSplinePoints(false);
		for (const FVector& Position :
		     {FVector(-55, -35, 0), FVector(-15, 35, 30), FVector(25, -25, 60), FVector(60, 30, 90)})
		{
			Spline->AddSplinePoint(Position, ESplineCoordinateSpace::Local, false);
		}
		Spline->UpdateSpline();
		for (int32 Marker = 0; Marker < 3; ++Marker)
		{
			const FString Name = FString::Printf(TEXT("RouteMarker%d"), Marker);
			AddMesh(*Blueprint, Root, *Name, Sphere, Color,
			        FVector((Marker - 1) * 45, (Marker % 2) * 35 - 15, Marker * 30), FVector(0.35));
		}
	}
	if (Index == 6)
	{
		check(ChildClass);
		USCS_Node* ChildNode =
		    AddSceneNode(*Blueprint, UChildActorComponent::StaticClass(), TEXT("OwnedColorBlock"), Root);
		UChildActorComponent* Child = CastChecked<UChildActorComponent>(ChildNode->ComponentTemplate);
		Child->SetChildActorClass(ChildClass);
		Child->SetRelativeLocation(FVector(0, 0, 30));
		Child->SetRelativeRotation(FRotator(0, 25, 0));
		Child->SetRelativeScale3D(FVector(0.75));
	}
	if (Index == 8)
	{
		AddMesh(*Blueprint, Root, TEXT("ProceduralMarker"), Cone, Color, FVector(0, 0, 30), FVector(0.5));
		USCS_Node* PCGNode = AddSceneNode(*Blueprint, UPCGComponent::StaticClass(), TEXT("Procedural"), nullptr);
		UPCGComponent* PCG = CastChecked<UPCGComponent>(PCGNode->ComponentTemplate);
		PCG->bActivated = true;
		PCG->bIsComponentPartitioned = false;
		PCG->GenerationTrigger = EPCGComponentGenerationTrigger::GenerateOnDemand;
		PCG->SetGraphLocal(&Graph);
		PCG->Seed = 137;
	}
	if (!AddFixtureVariables(*Blueprint, Fixture))
	{
		return nullptr;
	}
	if (Index == 2)
	{
		if (!AddLightConstructionGraph(*Blueprint))
		{
			return nullptr;
		}
	}
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
	FCompilerResultsLog Results;
	FKismetEditorUtilities::CompileBlueprint(Blueprint, EBlueprintCompileOptions::None, &Results);
	if (Results.NumErrors > 0)
	{
		return nullptr;
	}
	if (Blueprint->Status == BS_Error)
	{
		return nullptr;
	}
	return SaveAsset(*Blueprint) ? Blueprint : nullptr;
}

void MarkEnvironment(AActor& Actor, const FString& Label)
{
	Actor.Tags.AddUnique(EnvironmentTag);
	Actor.SetActorLabel(Label);
	TInlineComponentArray<UPrimitiveComponent*> Components(&Actor);
	for (UPrimitiveComponent* Component : Components)
	{
		Component->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	}
}

AStaticMeshActor* AddStageMesh(UWorld& World, UStaticMesh& Mesh, UMaterialInterface& Material, const FString& Label,
                               FVector Location, FVector Scale)
{
	AStaticMeshActor* Actor = World.SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator);
	check(Actor);
	Actor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	Actor->GetStaticMeshComponent()->SetStaticMesh(&Mesh);
	Actor->GetStaticMeshComponent()->SetMaterial(0, &Material);
	Actor->SetActorScale3D(Scale);
	MarkEnvironment(*Actor, Label);
	return Actor;
}

void AddLabel(UWorld& World, const FString& Text, FVector Position, FVector Camera, float Size, FColor Color)
{
	ATextRenderActor* Label = World.SpawnActor<ATextRenderActor>(Position, (Camera - Position).Rotation());
	check(Label);
	Label->GetTextRender()->SetText(FText::FromString(Text));
	Label->GetTextRender()->SetHorizontalAlignment(EHTA_Center);
	Label->GetTextRender()->SetWorldSize(Size);
	Label->GetTextRender()->SetTextRenderColor(Color);
	Label->GetTextRender()->SetCastShadow(false);
	MarkEnvironment(*Label, Text);
}

bool PopulateMap(const TArray<UBlueprint*>& Blueprints, const TArray<UMaterialInstanceConstant*>& Palette,
                 UStaticMesh& Cube, UClass& GameMode, bool bStress)
{
	const FString Name = bStress ? TEXT("L_OWTStress") : TEXT("L_OWTPlayground");
	const FString Path = ContentRoot + TEXT("Maps/") + Name;
	UWorld* World = UWorld::CreateWorld(EWorldType::Editor, false, *Name, CreatePackage(*Path));
	if (!World)
	{
		return false;
	}
	ON_SCOPE_EXIT
	{
		World->DestroyWorld(false);
	};
	World->ClearFlags(RF_Transient);
	World->SetFlags(RF_Public | RF_Standalone);
	World->GetWorldSettings()->DefaultGameMode = &GameMode;
	World->GetWorldSettings()->bForceNoPrecomputedLighting = true;
	const FVector Camera = bStress ? FVector(-2100, -2800, 2500) : FVector(-1550, -2200, 1600);
	const FVector LookAt = bStress ? FVector(1500, 900, 80) : FVector(800, 0, 80);
	APlayerStart* Start = World->SpawnActor<APlayerStart>(Camera, (LookAt - Camera).Rotation());
	check(Start);
	MarkEnvironment(*Start, TEXT("Camera / gallery entrance"));
	ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(FVector(0, 0, 1800), FRotator(-52, -30, 0));
	check(Sun);
	Sun->GetLightComponent()->SetMobility(EComponentMobility::Movable);
	Sun->GetLightComponent()->SetIntensity(5.0f);
	CastChecked<UDirectionalLightComponent>(Sun->GetLightComponent())->SetAtmosphereSunLight(true);
	CastChecked<UDirectionalLightComponent>(Sun->GetLightComponent())->SetForwardShadingPriority(1);
	MarkEnvironment(*Sun, TEXT("Studio / key light"));
	ADirectionalLight* Fill = World->SpawnActor<ADirectionalLight>(FVector(0, 0, 1200), FRotator(-25, 145, 0));
	check(Fill);
	Fill->GetLightComponent()->SetMobility(EComponentMobility::Movable);
	Fill->GetLightComponent()->SetIntensity(0.65f);
	Fill->GetLightComponent()->SetCastShadows(false);
	MarkEnvironment(*Fill, TEXT("Studio / soft shadow-free fill"));
	ASkyLight* Sky = World->SpawnActor<ASkyLight>();
	check(Sky);
	Sky->GetLightComponent()->SetMobility(EComponentMobility::Movable);
	Sky->GetLightComponent()->SetIntensity(1.6f);
	Sky->GetLightComponent()->SetRealTimeCaptureEnabled(true);
	Sky->GetLightComponent()->SetLowerHemisphereColor(FLinearColor(0.055f, 0.065f, 0.075f));
	MarkEnvironment(*Sky, TEXT("Studio / sky fill"));
	ASkyAtmosphere* Atmosphere = World->SpawnActor<ASkyAtmosphere>();
	check(Atmosphere);
	MarkEnvironment(*Atmosphere, TEXT("Studio / sky background"));
	AExponentialHeightFog* Haze = World->SpawnActor<AExponentialHeightFog>();
	check(Haze);
	Haze->GetComponent()->SetFogDensity(0.008f);
	Haze->GetComponent()->SetFogHeightFalloff(0.1f);
	Haze->GetComponent()->SetStartDistance(5000.0f);
	Haze->GetComponent()->SetFogMaxOpacity(0.85f);
	Haze->GetComponent()->SetFogInscatteringColor(FLinearColor(0.36f, 0.43f, 0.50f));
	MarkEnvironment(*Haze, TEXT("Studio / distant horizon haze"));
	APostProcessVolume* Exposure = World->SpawnActor<APostProcessVolume>();
	check(Exposure);
	Exposure->bUnbound = true;
	Exposure->Settings.bOverride_AutoExposureMethod = true;
	Exposure->Settings.AutoExposureMethod = AEM_Manual;
	Exposure->Settings.bOverride_AutoExposureApplyPhysicalCameraExposure = true;
	Exposure->Settings.AutoExposureApplyPhysicalCameraExposure = false;
	Exposure->Settings.bOverride_AutoExposureBias = true;
	Exposure->Settings.AutoExposureBias = 0.35f;
	Exposure->Settings.bOverride_BloomIntensity = true;
	Exposure->Settings.BloomIntensity = 0.12f;
	Exposure->Settings.bOverride_VignetteIntensity = true;
	Exposure->Settings.VignetteIntensity = 0.15f;
	MarkEnvironment(*Exposure, TEXT("Studio / fixed exposure"));
	AStaticMeshActor* Surround = AddStageMesh(*World, Cube, *Palette[10], TEXT("Studio / continuous neutral ground"),
	                                          FVector(950, 200, -120), FVector(20000, 20000, 1.0));
	Surround->GetStaticMeshComponent()->SetCastShadow(false);
	AddStageMesh(*World, Cube, *Palette[8], TEXT("Studio / porcelain floor"), FVector(950, 200, -45),
	             bStress ? FVector(100, 85, 0.5) : FVector(47, 40, 0.5));
	AddStageMesh(*World, Cube, *Palette[9], TEXT("Studio / rear accent wall"),
	             bStress ? FVector(5800, 900, 300) : FVector(2600, 0, 220),
	             bStress ? FVector(0.2, 75, 6) : FVector(0.2, 36, 4.5));
	const int32 Count = bStress ? StressActorCount : MainActorCount;
	for (int32 Index = 0; Index < Count; ++Index)
	{
		int32 FixtureIndex;
		FVector Position;
		if (bStress)
		{
			const int32 CheapTypes[] = {0, 1, 3, 4, 5};
			FixtureIndex = Index < Fixtures.Num() ? Index : CheapTypes[Index % UE_ARRAY_COUNT(CheapTypes)];
			Position = FVector((Index % 30) * 175 - 650, (Index / 30) * 185 - 1300, 45);
		}
		else
		{
			FixtureIndex = Index / 6;
			const int32 WithinBay = Index % 6;
			Position = FVector((FixtureIndex % 3) * 800 + (WithinBay % 3 - 1) * 190,
			                   (FixtureIndex / 3 - 1) * 850 + (WithinBay / 3) * 210 - 90, 55);
		}
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Name = FName(*FString::Printf(TEXT("%s_%d"), *Fixtures[FixtureIndex].Name, Index + 1));
		AActor* Actor = World->SpawnActor<AActor>(Blueprints[FixtureIndex]->GeneratedClass, Position,
		                                          FRotator(0, (Index % 3 - 1) * 12, 0), SpawnParameters);
		if (!Actor)
		{
			return false;
		}
		Actor->Tags.AddUnique(EditableTag);
		Actor->Tags.AddUnique(FName(*(TEXT("OWT.Fixture.") + Fixtures[FixtureIndex].Name)));
		Actor->Tags.AddUnique(FName(*FString::Printf(TEXT("OWT.FixtureId.%04d"), Index)));
		Actor->SetActorLabel(FString::Printf(TEXT("%s / %03d"), *Fixtures[FixtureIndex].Name, Index + 1));
		if (!bStress)
		{
			if (UPCGComponent* PCG = Actor->FindComponentByClass<UPCGComponent>())
			{
				PCG->GenerationTrigger = EPCGComponentGenerationTrigger::GenerateOnLoad;
			}
		}
		if (Index == 0)
		{
			Actor->Tags.AddUnique(TEXT("OWT.Playground.InitialSelection"));
		}
	}
	if (!bStress)
	{
		for (int32 Bay = 0; Bay < Fixtures.Num(); ++Bay)
		{
			const FVector Center((Bay % 3) * 800, (Bay / 3 - 1) * 850, 0);
			AddStageMesh(*World, Cube, *Palette[Fixtures[Bay].Color],
			             FString::Printf(TEXT("Display / bay %02d"), Bay + 1), Center + FVector(0, 25, -10),
			             FVector(6.9, 5.8, 0.12));
			AddLabel(*World, Fixtures[Bay].Title, Center + FVector(0, -310, 30), Camera, 30, FColor(20, 32, 50));
			AddLabel(*World, Fixtures[Bay].Description, Center + FVector(0, -355, 10), Camera, 16, FColor(50, 66, 85));
		}
	}
	AddLabel(*World, bStress ? TEXT("ATTRIBUTE LAB  /  STRESS 600") : TEXT("ATTRIBUTE LAB"),
	         bStress ? FVector(1000, -1700, 160) : FVector(800, -1500, 150), Camera, bStress ? 90 : 100,
	         FColor(20, 32, 50));
	AddLabel(*World,
	         bStress ? TEXT("600 BLUEPRINT ACTORS   /   SHARED ASSETS   /   BOUNDED LIGHTING")
	                 : TEXT("09 COMPONENT STUDIES   /   54 EDITABLE OBJECTS   /   RUNTIME ITF"),
	         bStress ? FVector(1000, -1780, 70) : FVector(800, -1600, 60), Camera, 24, FColor(50, 66, 85));
	const bool bSaved = SaveAsset(*World);
	UE_LOG(LogTemp, Display, TEXT("OWT_PLAYGROUND_MAP: %s editable=%d saved=%d"), *Path, Count, bSaved);
	return bSaved;
}

bool ValidateMap(const FString& Name, UClass& GameMode, int32 ExpectedActors)
{
	const FString Path = ContentRoot + TEXT("Maps/") + Name;
	UWorld* World = LoadObject<UWorld>(nullptr, *(Path + TEXT(".") + Name));
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND_VALIDATE: Missing map %s"), *Path);
		return false;
	}
	if (!World->PersistentLevel)
	{
		return false;
	}
	if (World->GetWorldSettings()->DefaultGameMode != &GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND_VALIDATE: Unexpected GameMode in %s"), *Path);
		return false;
	}
	int32 EditableCount = 0;
	int32 InitialSelectionCount = 0;
	bool bSun = false;
	bool bSky = false;
	bool bAtmosphere = false;
	bool bFixedExposure = false;
	bool bCamera = false;
	TSet<FName> FixtureTags;
	for (AActor* Actor : World->PersistentLevel->Actors)
	{
		if (!IsValid(Actor))
		{
			continue;
		}
		if (Actor->ActorHasTag(EditableTag))
		{
			++EditableCount;
			for (const FFixtureDefinition& Fixture : Fixtures)
			{
				const FName Tag(*(TEXT("OWT.Fixture.") + Fixture.Name));
				if (Actor->ActorHasTag(Tag))
				{
					FixtureTags.Add(Tag);
				}
			}
		}
		if (Actor->ActorHasTag(TEXT("OWT.Playground.InitialSelection")))
		{
			++InitialSelectionCount;
		}
		if (const ADirectionalLight* Sun = Cast<ADirectionalLight>(Actor))
		{
			bSun = Sun->GetLightComponent()->Intensity > 0.0f;
		}
		if (const ASkyLight* Sky = Cast<ASkyLight>(Actor))
		{
			bSky = Sky->GetLightComponent()->Intensity > 0.0f;
		}
		if (Actor->IsA<ASkyAtmosphere>())
		{
			bAtmosphere = true;
		}
		if (Actor->IsA<APlayerStart>())
		{
			bCamera = true;
		}
		if (const APostProcessVolume* Exposure = Cast<APostProcessVolume>(Actor))
		{
			if (Exposure->bUnbound)
			{
				if (Exposure->Settings.bOverride_AutoExposureMethod)
				{
					bFixedExposure = Exposure->Settings.AutoExposureMethod == AEM_Manual;
				}
			}
		}
	}
	if (EditableCount != ExpectedActors)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND_VALIDATE: %s has %d editable actors, expected %d"), *Path,
		       EditableCount, ExpectedActors);
		return false;
	}
	if (InitialSelectionCount != 1)
	{
		return false;
	}
	if (FixtureTags.Num() != Fixtures.Num())
	{
		return false;
	}
	for (bool bRequirement : {bSun, bSky, bAtmosphere, bFixedExposure, bCamera})
	{
		if (!bRequirement)
		{
			UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND_VALIDATE: Missing lighting, exposure or camera in %s"), *Path);
			return false;
		}
	}
	UE_LOG(LogTemp, Display,
	       TEXT("OWT_PLAYGROUND_VALIDATE_MAP: %s editable=%d archetypes=%d lighting=ready exposure=manual"), *Path,
	       EditableCount, FixtureTags.Num());
	return true;
}

bool ValidateOutput()
{
	FString Json;
	if (!FFileHelper::LoadFileToString(Json, *ManifestFilename()))
	{
		UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND_VALIDATE: Missing generator manifest."));
		return false;
	}
	TSharedPtr<FJsonObject> Manifest;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Json), Manifest))
	{
		return false;
	}
	FString Generator;
	Manifest->TryGetStringField(TEXT("generator"), Generator);
	if (Generator != TEXT("CreateOWTPlayground"))
	{
		return false;
	}
	bool bComplete = false;
	Manifest->TryGetBoolField(TEXT("complete"), bComplete);
	if (!bComplete)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND_VALIDATE: Asset generation was incomplete."));
		return false;
	}
	for (const FString& Path : GeneratedPackages())
	{
		if (!FPaths::FileExists(AssetFilename(Path)))
		{
			UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND_VALIDATE: Missing generated asset %s"), *Path);
			return false;
		}
	}
	const FString SurfacePath = ContentRoot + TEXT("Materials/M_OWTPlaygroundSurface.M_OWTPlaygroundSurface");
	const UMaterial* Surface = LoadObject<UMaterial>(nullptr, *SurfacePath);
	if (!Surface)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND_VALIDATE: Missing base surface material."));
		return false;
	}
	if (!Surface->GetUsageByFlag(MATUSAGE_InstancedStaticMeshes))
	{
		UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND_VALIDATE: Saved base material lacks instanced static mesh usage."));
		return false;
	}
	for (const FFixtureDefinition& Fixture : Fixtures)
	{
		const FString Path = BlueprintPath(Fixture);
		const FString ClassPath = Path + TEXT(".") + FPackageName::GetShortName(Path) + TEXT("_C");
		UClass* Class = LoadClass<AActor>(nullptr, *ClassPath);
		if (!Class)
		{
			UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND_VALIDATE: Missing runtime Blueprint class %s"), *ClassPath);
			return false;
		}
		if (!FindFProperty<FStrProperty>(Class, TEXT("FixtureLabel")))
		{
			return false;
		}
		if (!FindFProperty<FIntProperty>(Class, TEXT("Revision")))
		{
			return false;
		}
		const FStructProperty* Accent = FindFProperty<FStructProperty>(Class, TEXT("Accent"));
		if (!Accent)
		{
			return false;
		}
		if (Accent->Struct != TBaseStructure<FLinearColor>::Get())
		{
			return false;
		}
	}
	UClass* GameMode = LoadClass<AGameModeBase>(nullptr, TEXT("/Script/simple_proj.OWTPlaygroundGameMode"));
	if (!GameMode)
	{
		return false;
	}
	if (!ValidateMap(TEXT("L_OWTPlayground"), *GameMode, MainActorCount))
	{
		return false;
	}
	if (!ValidateMap(TEXT("L_OWTStress"), *GameMode, StressActorCount))
	{
		return false;
	}
	UE_LOG(LogTemp, Display,
	       TEXT("OWT_PLAYGROUND_VALIDATE_SUCCESS: %d assets, 9 runtime Blueprint classes, 2 maps and complete "
	            "manifest. No assets modified."),
	       GeneratedPackages().Num());
	return true;
}
} // namespace

UCreateOWTPlaygroundCommandlet::UCreateOWTPlaygroundCommandlet()
{
	IsClient = false;
	IsServer = false;
	IsEditor = true;
	LogToConsole = true;
}

int32 UCreateOWTPlaygroundCommandlet::Main(const FString& Params)
{
	if (FParse::Param(*Params, TEXT("ValidateOnly")))
	{
		return ValidateOutput() ? 0 : 1;
	}
	UClass* GameMode = LoadClass<AGameModeBase>(nullptr, TEXT("/Script/simple_proj.OWTPlaygroundGameMode"));
	UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	UStaticMesh* Sphere = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	UStaticMesh* Cone = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cone.Cone"));
	UPCGGraph* Graph =
	    LoadObject<UPCGGraph>(nullptr, TEXT("/OWTRuntimeEditing/Tests/PCG_OWTDuplicate.PCG_OWTDuplicate"));
	if (!GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND: Required playground GameMode unavailable."));
		return 1;
	}
	for (const UStaticMesh* Shape : {Cube, Sphere, Cone})
	{
		if (!Shape)
		{
			UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND: Required Engine shape unavailable."));
			return 1;
		}
	}
	if (!Graph)
	{
		UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND: Existing PCG fixture graph unavailable."));
		return 1;
	}
	if (!PrepareOutput(FParse::Param(*Params, TEXT("Regenerate"))))
	{
		return 1;
	}
	UMaterial* Surface = CreateSurfaceMaterial();
	if (!Surface)
	{
		return 1;
	}
	const TArray<UMaterialInstanceConstant*> Palette = CreatePalette(*Surface);
	if (Palette.Num() != Colors.Num())
	{
		return 1;
	}
	TArray<UBlueprint*> Blueprints;
	for (int32 Index = 0; Index < Fixtures.Num(); ++Index)
	{
		UClass* ChildClass = Blueprints.IsEmpty() ? nullptr : Blueprints[0]->GeneratedClass.Get();
		UBlueprint* Blueprint = CreateFixture(Index, Palette, ChildClass, *Cube, *Sphere, *Cone, *Graph);
		if (!Blueprint)
		{
			UE_LOG(LogTemp, Error, TEXT("OWT_PLAYGROUND: Failed Blueprint %s"), *Fixtures[Index].Name);
			return 1;
		}
		Blueprints.Add(Blueprint);
	}
	if (!PopulateMap(Blueprints, Palette, *Cube, *GameMode, false))
	{
		return 1;
	}
	if (!PopulateMap(Blueprints, Palette, *Cube, *GameMode, true))
	{
		return 1;
	}
	if (!WriteManifest(true))
	{
		return 1;
	}
	UE_LOG(
	    LogTemp, Display,
	    TEXT("OWT_PLAYGROUND_SUCCESS: 9 Blueprints, 12 materials, 54-Actor gallery, 600-Actor stress map. Manifest=%s"),
	    *ManifestFilename());
	return 0;
}
