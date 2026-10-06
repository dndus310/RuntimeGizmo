#include "CreateVTBSampleCommandlet.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "EdGraphSchema_K2.h"
#include "Engine/Blueprint.h"
#include "HAL/FileManager.h"
#include "K2Node_CallFunction.h"
#include "K2Node_Event.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "KismetCompiler.h"
#include "Misc/PackageName.h"
#include "Misc/Paths.h"
#include "UObject/SavePackage.h"
#include "VTBAttributeEditor.h"

UCreateVTBSampleCommandlet::UCreateVTBSampleCommandlet()
{
	IsClient = false;
	IsServer = false;
	IsEditor = true;
	LogToConsole = true;
}

int32 UCreateVTBSampleCommandlet::Main(const FString& Params)
{
	const FString PackageName = TEXT("/Game/VTBSamples/BP_VTBAttributeEditorSample");
	const FString Filename =
	    FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());

	// Never overwrite an existing sample or the user's subsequent graph edits.
	if (FPaths::FileExists(Filename))
	{
		UE_LOG(LogTemp, Error, TEXT("Sample already exists; leaving it unchanged: %s"), *Filename);

		return 1;
	}

	UPackage* Package = CreatePackage(*PackageName);
	UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(AVTBAttributeEditor::StaticClass(), Package,
	                                                                TEXT("BP_VTBAttributeEditorSample"), BPTYPE_Normal);
	if (!Blueprint)
	{
		return 1;
	}

	UEdGraph* Graph = FBlueprintEditorUtils::FindEventGraph(Blueprint);
	if (!Graph)
	{
		Graph = FBlueprintEditorUtils::CreateNewGraph(Blueprint, TEXT("EventGraph"), UEdGraph::StaticClass(),
		                                              UEdGraphSchema_K2::StaticClass());
		FBlueprintEditorUtils::AddUbergraphPage(Blueprint, Graph);
	}

	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();

	auto FindOrCreateEvent = [Graph](UClass* Parent, FName FunctionName, int32 Y)
	{
		for (UEdGraphNode* Node : Graph->Nodes)
		{
			UK2Node_Event* Event = Cast<UK2Node_Event>(Node);
			if (Event && Event->EventReference.GetMemberName() == FunctionName)
			{
				Event->NodePosX = 0;
				Event->NodePosY = Y;

				return Event;
			}
		}

		UK2Node_Event* Event = NewObject<UK2Node_Event>(Graph);
		Event->EventReference.SetExternalMember(FunctionName, Parent);
		Event->bOverrideFunction = true;
		Graph->AddNode(Event, false, false);
		Event->CreateNewGuid();
		Event->PostPlacedNewNode();
		Event->AllocateDefaultPins();
		Event->NodePosY = Y;

		return Event;
	};

	auto AddCall = [Graph](UFunction* Function, int32 Y)
	{
		UK2Node_CallFunction* Call = NewObject<UK2Node_CallFunction>(Graph);
		Call->SetFromFunction(Function);
		Graph->AddNode(Call, false, false);
		Call->CreateNewGuid();
		Call->PostPlacedNewNode();
		Call->AllocateDefaultPins();
		Call->NodePosX = 400;
		Call->NodePosY = Y;

		return Call;
	};

	UK2Node_Event* BeginPlay = FindOrCreateEvent(AActor::StaticClass(), TEXT("ReceiveBeginPlay"), 0);
	UK2Node_CallFunction* SaveHistory =
	    AddCall(AVTBAttributeEditor::StaticClass()->FindFunctionByName(TEXT("SaveHistory")), 0);
	UK2Node_Event* SaveEvent = FindOrCreateEvent(AVTBAttributeEditor::StaticClass(), TEXT("SaveHistory"), 300);
	UK2Node_CallFunction* Print =
	    AddCall(UKismetSystemLibrary::StaticClass()->FindFunctionByName(TEXT("PrintString")), 300);

	Schema->TrySetDefaultValue(*Print->FindPinChecked(TEXT("InString")),
	                           TEXT("VTB Sample: SaveHistory Blueprint event executed!"));
	Schema->TrySetDefaultValue(*Print->FindPinChecked(TEXT("Duration")), TEXT("5.0"));
	Schema->TrySetDefaultValue(*Print->FindPinChecked(TEXT("bPrintToLog")), TEXT("true"));
	if (!Schema->TryCreateConnection(BeginPlay->FindPinChecked(UEdGraphSchema_K2::PN_Then),
	                                 SaveHistory->FindPinChecked(UEdGraphSchema_K2::PN_Execute)) ||
	    !Schema->TryCreateConnection(SaveEvent->FindPinChecked(UEdGraphSchema_K2::PN_Then),
	                                 Print->FindPinChecked(UEdGraphSchema_K2::PN_Execute)))
	{
		UE_LOG(LogTemp, Error, TEXT("Could not connect sample graph pins."));

		return 1;
	}

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
	FCompilerResultsLog Results;
	FKismetEditorUtilities::CompileBlueprint(Blueprint, EBlueprintCompileOptions::None, &Results);
	if (Results.NumErrors > 0 || Blueprint->Status == BS_Error)
	{
		UE_LOG(LogTemp, Error, TEXT("Sample Blueprint compilation failed."));

		return 1;
	}

	IFileManager::Get().MakeDirectory(*FPaths::GetPath(Filename), true);
	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.SaveFlags = SAVE_NoError;
	if (!UPackage::SavePackage(Package, Blueprint, *Filename, SaveArgs))
	{
		return 1;
	}

	FAssetRegistryModule::AssetCreated(Blueprint);
	UE_LOG(LogTemp, Display, TEXT("VTB_SAMPLE_SUCCESS: Compiled and saved %s (%d warnings)"), *PackageName,
	       Results.NumWarnings);

	return 0;
}
