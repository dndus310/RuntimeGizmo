#include "OWTPCGFixtureAsset.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/BoxComponent.h"
#include "Elements/PCGCreatePoints.h"
#include "Elements/PCGStaticMeshSpawner.h"
#include "Engine/Blueprint.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"
#include "HAL/FileManager.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "MeshSelectors/PCGMeshSelectorWeighted.h"
#include "Misc/PackageName.h"
#include "Misc/Paths.h"
#include "PCGComponent.h"
#include "PCGGraph.h"
#include "PCGNode.h"
#include "UObject/SavePackage.h"

namespace
{
bool SaveFixture(UObject& Asset)
{
	const FString Filename = FPackageName::LongPackageNameToFilename(Asset.GetOutermost()->GetName(),
	                                                                 FPackageName::GetAssetPackageExtension());
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(Filename), true);
	FSavePackageArgs Args;
	Args.TopLevelFlags = RF_Public | RF_Standalone;
	FAssetRegistryModule::AssetCreated(&Asset);
	return UPackage::SavePackage(Asset.GetOutermost(), &Asset, *Filename, Args);
}
} // namespace

bool CreateOWTPCGFixtureAssets()
{
	const TCHAR* GraphPath = TEXT("/OWTRuntimeEditing/Tests/PCG_OWTDuplicate");
	UPCGGraph* Graph = LoadObject<UPCGGraph>(nullptr, GraphPath, nullptr, LOAD_NoWarn);
	if (!Graph)
	{
		UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
		if (!Mesh)
		{
			return false;
		}
		Graph = NewObject<UPCGGraph>(CreatePackage(GraphPath), TEXT("PCG_OWTDuplicate"), RF_Public | RF_Standalone);
		UPCGCreatePointsSettings* Points = nullptr;
		UPCGNode* PointNode = Graph->AddNodeOfType<UPCGCreatePointsSettings>(Points);
		Points->CoordinateSpace = EPCGCoordinateSpace::OriginalComponent;
		Points->PointsToCreate.SetNum(2);
		Points->PointsToCreate[1].Transform.SetLocation(FVector(50, 0, 0));
		UPCGStaticMeshSpawnerSettings* Spawner = nullptr;
		UPCGNode* SpawnerNode = Graph->AddNodeOfType<UPCGStaticMeshSpawnerSettings>(Spawner);
		Spawner->SetMeshSelectorType(UPCGMeshSelectorWeighted::StaticClass());
		Spawner->bSynchronousLoad = true;
		CastChecked<UPCGMeshSelectorWeighted>(Spawner->MeshSelectorParameters)
		    ->MeshEntries.Add(FPCGMeshSelectorWeightedEntry(Mesh, 1));
		Graph->AddEdge(PointNode, PCGPinConstants::DefaultOutputLabel, SpawnerNode, PCGPinConstants::DefaultInputLabel);
		Graph->AddEdge(SpawnerNode, PCGPinConstants::DefaultOutputLabel, Graph->GetOutputNode(),
		               PCGPinConstants::DefaultOutputLabel);
		if (!SaveFixture(*Graph))
		{
			return false;
		}
	}
	const TCHAR* BlueprintPath = TEXT("/OWTRuntimeEditing/Tests/BP_OWTPCGFixture");
	if (LoadObject<UBlueprint>(nullptr, BlueprintPath, nullptr, LOAD_NoWarn))
	{
		return true;
	}
	UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(AActor::StaticClass(), CreatePackage(BlueprintPath),
	                                                                TEXT("BP_OWTPCGFixture"), BPTYPE_Normal);
	if (!Blueprint)
	{
		return false;
	}
	USimpleConstructionScript* Script = Blueprint->SimpleConstructionScript;
	USCS_Node* Root = Script->CreateNode(UBoxComponent::StaticClass(), TEXT("Bounds"));
	CastChecked<UBoxComponent>(Root->ComponentTemplate)->SetBoxExtent(FVector(100));
	Script->AddNode(Root);
	USCS_Node* PCGNode = Script->CreateNode(UPCGComponent::StaticClass(), TEXT("Procedural"));
	UPCGComponent* PCG = CastChecked<UPCGComponent>(PCGNode->ComponentTemplate);
	PCG->bActivated = true;
	PCG->GenerationTrigger = EPCGComponentGenerationTrigger::GenerateOnDemand;
	PCG->SetGraphLocal(Graph);
	Script->AddNode(PCGNode);
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
	FKismetEditorUtilities::CompileBlueprint(Blueprint);
	if (Blueprint->Status == BS_Error)
	{
		return false;
	}
	return SaveFixture(*Blueprint);
}
