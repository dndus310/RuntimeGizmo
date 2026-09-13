#include "CreateOWTLevelCommandlet.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/Blueprint.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/WorldSettings.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "KismetCompiler.h"
#include "Misc/PackageName.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "UObject/SavePackage.h"
#include "VTBOWTEditorGameMode.h"
#include "VTBOWTEditorGameState.h"
#include "VTBOWTEditorPlayerController.h"
#include "VTBOWTSpectator.h"

namespace
{
const TCHAR* MapPath = TEXT("/Game/VTBOWT/Maps/L_OWTEditSample");
const TCHAR* CubePath = TEXT("/Game/VTBOWT/Blueprints/BP_OWTEditableCube");

bool SaveAsset(UObject& Asset, const FString& Extension)
{
	UPackage* Package = Asset.GetOutermost();
	const FString Filename = FPackageName::LongPackageNameToFilename(Package->GetName(), Extension);
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(Filename), true);
	FSavePackageArgs Args;
	Args.TopLevelFlags = RF_Public | RF_Standalone;
	Args.SaveFlags = SAVE_NoError;
	return UPackage::SavePackage(Package, &Asset, *Filename, Args);
}

UBlueprint* CreateCubeBlueprint()
{
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (!Mesh)
	{
		return nullptr;
	}

	UPackage* Package = CreatePackage(CubePath);
	UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(AActor::StaticClass(), Package,
	                                                                TEXT("BP_OWTEditableCube"), BPTYPE_Normal);
	USCS_Node* Node =
	    Blueprint->SimpleConstructionScript->CreateNode(UStaticMeshComponent::StaticClass(), TEXT("Cube"));
	UStaticMeshComponent* Component = CastChecked<UStaticMeshComponent>(Node->ComponentTemplate);
	Component->SetStaticMesh(Mesh);
	Component->SetMobility(EComponentMobility::Movable);
	Component->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Blueprint->SimpleConstructionScript->AddNode(Node);

	FCompilerResultsLog Results;
	FKismetEditorUtilities::CompileBlueprint(Blueprint, EBlueprintCompileOptions::None, &Results);
	if (Results.NumErrors || !SaveAsset(*Blueprint, FPackageName::GetAssetPackageExtension()))
	{
		return nullptr;
	}

	FAssetRegistryModule::AssetCreated(Blueprint);
	return Blueprint;
}

bool CreateLevel(UClass& CubeClass)
{
	UPackage* Package = CreatePackage(MapPath);
	UWorld* World = UWorld::CreateWorld(EWorldType::Editor, false, TEXT("L_OWTEditSample"), Package);
	if (!World)
	{
		return false;
	}

	World->GetWorldSettings()->DefaultGameMode = AVTBOWTEditorGameMode::StaticClass();
	World->ClearFlags(RF_Transient);
	World->SetFlags(RF_Public | RF_Standalone);
	AActor* Cube = World->SpawnActor<AActor>(&CubeClass, FVector(0, 0, 50), FRotator::ZeroRotator);
	APlayerStart* Start = World->SpawnActor<APlayerStart>(FVector(-500, 0, 250), FRotator(-20, 0, 0));
	ADirectionalLight* Light = World->SpawnActor<ADirectionalLight>(FVector(0, 0, 400), FRotator(-45, -30, 0));
	if (!Cube || !Start || !Light)
	{
		World->DestroyWorld(false);
		return false;
	}

	Cube->SetActorLabel(TEXT("Editable Cube"));
	Start->SetActorLabel(TEXT("Editing Spectator Start"));
	Light->GetLightComponent()->SetMobility(EComponentMobility::Movable);
	Light->SetBrightness(3.0f);
	const bool bSaved = SaveAsset(*World, FPackageName::GetMapPackageExtension());
	World->DestroyWorld(false);
	return bSaved;
}

bool ValidateLevel()
{
	UWorld* World = LoadObject<UWorld>(nullptr, TEXT("/Game/VTBOWT/Maps/L_OWTEditSample.L_OWTEditSample"));
	UBlueprint* Cube =
	    LoadObject<UBlueprint>(nullptr, TEXT("/Game/VTBOWT/Blueprints/BP_OWTEditableCube.BP_OWTEditableCube"));
	UClass* PawnClass =
	    LoadClass<AVTBOWTSpectator>(nullptr, TEXT("/Game/VTBOWT/Blueprints/BP_VTBOWTSpectator.BP_VTBOWTSpectator_C"));
	if (!World || !Cube || !PawnClass ||
	    World->GetWorldSettings()->DefaultGameMode != AVTBOWTEditorGameMode::StaticClass())
	{
		return false;
	}

	const AVTBOWTEditorGameMode* Mode = GetDefault<AVTBOWTEditorGameMode>();
	if (Mode->DefaultPawnClass != PawnClass || Mode->SpectatorClass != PawnClass ||
	    Mode->PlayerControllerClass != AVTBOWTEditorPlayerController::StaticClass() ||
	    Mode->GameStateClass != AVTBOWTEditorGameState::StaticClass())
	{
		return false;
	}

	for (AActor* Actor : World->PersistentLevel->Actors)
	{
		if (!Actor || !Actor->IsA(Cube->GeneratedClass))
		{
			continue;
		}
		const UStaticMeshComponent* Mesh = Actor->FindComponentByClass<UStaticMeshComponent>();
		return Mesh && Mesh->GetStaticMesh() && Mesh->Mobility == EComponentMobility::Movable &&
		       Mesh->GetCollisionResponseToChannel(ECC_Visibility) == ECR_Block;
	}
	return false;
}
} // namespace

UCreateOWTLevelCommandlet::UCreateOWTLevelCommandlet()
{
	IsClient = false;
	IsServer = false;
	IsEditor = true;
	LogToConsole = true;
}

int32 UCreateOWTLevelCommandlet::Main(const FString& Params)
{
	if (Params.Contains(TEXT("ValidateOnly")))
	{
		const bool bValid = ValidateLevel();
		UE_LOG(LogTemp, Display, TEXT("OWT_LEVEL_VALIDATION: %s"), bValid ? TEXT("PASS") : TEXT("FAIL"));
		return bValid ? 0 : 1;
	}
	if (FPackageName::DoesPackageExist(MapPath))
	{
		UE_LOG(LogTemp, Error, TEXT("Sample assets already exist; refusing to overwrite user edits."));
		return 1;
	}

	UBlueprint* Blueprint =
	    FPackageName::DoesPackageExist(CubePath)
	        ? LoadObject<UBlueprint>(nullptr, TEXT("/Game/VTBOWT/Blueprints/BP_OWTEditableCube.BP_OWTEditableCube"))
	        : CreateCubeBlueprint();
	if (!Blueprint || !CreateLevel(*Blueprint->GeneratedClass.Get()))
	{
		return 1;
	}
	UE_LOG(LogTemp, Display, TEXT("OWT_LEVEL_CREATED: %s"), MapPath);
	return 0;
}
