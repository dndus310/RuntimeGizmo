#include "CreateOWTInputCommandlet.h"
#include "OWTGizmoValidation.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "EdGraphSchema_K2.h"
#include "Engine/Blueprint.h"
#include "Engine/InputDelegateBinding.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "HAL/FileManager.h"

#include "Input/VTBOWTModifierKeyTrigger.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "K2Node_CallFunction.h"
#include "K2Node_MakeStruct.h"
#include "Kismet/BlueprintInstancedStructLibrary.h"
#include "K2Node_EnhancedInputAction.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "KismetCompiler.h"
#include "Misc/PackageName.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "UObject/SavePackage.h"
#include "VTBOWTEditorSubsystem.h"
#include "VTBOWTSpectator.h"
#include "Components/SceneComponent.h"
#include "Context/VTBOWTEditorToolsContext.h"
#include "BaseGizmos/CombinedTransformGizmo.h"
#include "BaseGizmos/TransformProxy.h"
#include "Gizmos/Base/VTBOWTBaseTransformGizmo.h"
#include "Components/PrimitiveComponent.h"

namespace OWTInputAssets
{
const FString Root = TEXT("/Game/VTBOWT/Input/");
const FString PawnPackage = TEXT("/Game/VTBOWT/Blueprints/BP_VTBOWTSpectator");
struct FActionSpec
{
public:
	FActionSpec(const TCHAR* InName, UScriptStruct* (*InContextType)(), FKey InKey, bool InCtrl, bool InShift,
	            bool InNoShift)
	    : Name(InName), ContextType(InContextType), Key(InKey), Ctrl(InCtrl), Shift(InShift), NoShift(InNoShift)
	{
	}

public:
	const TCHAR* Name;
	UScriptStruct* (*ContextType)();
	FKey Key;
	bool Ctrl;
	bool Shift;
	bool NoShift;
};
const TArray<FActionSpec> Specs = {
    {TEXT("IA_Selectable"), &FOWTSelectObjectContext::StaticStruct, EKeys::LeftMouseButton, false, false, false},
    {TEXT("IA_Undo"), &FOWTUndoContext::StaticStruct, EKeys::Z, true, false, true},
    {TEXT("IA_Redo"), &FOWTRedoContext::StaticStruct, EKeys::Z, true, true, false},
    {TEXT("IA_TRSGizmoCoordinate"), &FOWTToggleCoordinateSystemContext::StaticStruct, EKeys::Tilde, true, false, false},
    {TEXT("IA_GizmoMode"), &FOWTToggleTransformSplineContext::StaticStruct, EKeys::T, true, false, false},
    {TEXT("IA_GizmoTranslation"), &FOWTSetTranslationContext::StaticStruct, EKeys::W, false, false, false},
    {TEXT("IA_GizmoRotation"), &FOWTSetRotationContext::StaticStruct, EKeys::E, false, false, false},
    {TEXT("IA_GizmoScale"), &FOWTSetScaleContext::StaticStruct, EKeys::R, false, false, false},
    {TEXT("IA_SelectCancel"), &FOWTHideSelectionGizmoContext::StaticStruct, EKeys::Escape, false, false, false},
    {TEXT("IA_Duplicate"), &FOWTDuplicateSelectionContext::StaticStruct, EKeys::D, true, false, false},
    {TEXT("IA_ToggleEditing"), &FOWTToggleEditingContext::StaticStruct, EKeys::F2, false, false, false}};

FString AssetFilename(const FString& Name)
{
	return FPackageName::LongPackageNameToFilename(Name, FPackageName::GetAssetPackageExtension());
}

bool SaveAsset(UObject* Asset)
{
	const FString Filename = AssetFilename(Asset->GetOutermost()->GetName());
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(Filename), true);
	FSavePackageArgs Args;
	Args.TopLevelFlags = RF_Public | RF_Standalone;
	if (!UPackage::SavePackage(Asset->GetOutermost(), Asset, *Filename, Args))
	{
		return false;
	}

	FAssetRegistryModule::AssetCreated(Asset);

	return true;
}

void Map(UInputMappingContext* IMC, UInputAction* Action, FKey Key, bool Ctrl, bool Shift, bool NoShift)
{
	FEnhancedActionKeyMapping& Mapping = IMC->MapKey(Action, Key);
	Mapping.Triggers.Add(NewObject<UInputTriggerPressed>(IMC));
	if (Ctrl || Shift || NoShift)
	{
		UVTBOWTModifierKeyTrigger* Modifiers = NewObject<UVTBOWTModifierKeyTrigger>(IMC);
		Modifiers->bRequireControl = Ctrl;
		Modifiers->bRequireShift = Shift;
		Modifiers->bDisallowShift = NoShift;
		Mapping.Triggers.Add(Modifiers);
	}
}

UInputAction* CreateCameraAction(const TCHAR* Name, EInputActionValueType Type)
{
	const FString PackageName = Root + Name;
	UInputAction* Action = FPackageName::DoesPackageExist(PackageName)
	                           ? LoadObject<UInputAction>(nullptr, *PackageName)
	                           : NewObject<UInputAction>(CreatePackage(*PackageName), Name, RF_Public | RF_Standalone);
	if (!Action)
	{
		return nullptr;
	}
	Action->ValueType = Type;
	Action->bConsumeInput = false;
	Action->AccumulationBehavior = EInputActionAccumulationBehavior::Cumulative;
	return Action;
}

void MapCameraMovement(UInputMappingContext& IMC, UInputAction& Action, FKey Key, bool bVertical, bool bNegative)
{
	FEnhancedActionKeyMapping& Mapping = IMC.MapKey(&Action, Key);
	if (bVertical)
	{
		Mapping.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>(&IMC));
	}
	if (bNegative)
	{
		Mapping.Modifiers.Add(NewObject<UInputModifierNegate>(&IMC));
	}
}

bool UpgradeCameraInput(UBlueprint& BP, UInputMappingContext& IMC)
{
	const FString BackupDir = FPaths::ProjectSavedDir() / TEXT("Backups/BeforeCameraInput");
	IFileManager::Get().MakeDirectory(*BackupDir, true);
	for (UObject* Asset : TArray<UObject*>{&BP, &IMC})
	{
		const FString Source = AssetFilename(Asset->GetOutermost()->GetName());
		if (!FPaths::FileExists(Source))
		{
			continue;
		}

		const FString Backup = BackupDir / (Asset->GetName() + TEXT(".uasset"));
		if (!FPaths::FileExists(Backup) && IFileManager::Get().Copy(*Backup, *Source) != COPY_OK)
		{
			return false;
		}
	}
	UInputAction* Navigate = CreateCameraAction(TEXT("IA_CameraNavigate"), EInputActionValueType::Boolean);
	UInputAction* Move = CreateCameraAction(TEXT("IA_CameraMove"), EInputActionValueType::Axis2D);
	UInputAction* Look = CreateCameraAction(TEXT("IA_CameraLook"), EInputActionValueType::Axis2D);
	if (!Navigate || !Move || !Look)
	{
		return false;
	}
	IMC.UnmapAllKeysFromAction(Navigate);
	IMC.UnmapAllKeysFromAction(Move);
	IMC.UnmapAllKeysFromAction(Look);
	IMC.MapKey(Navigate, EKeys::RightMouseButton);
	IMC.MapKey(Look, EKeys::Mouse2D);
	MapCameraMovement(IMC, *Move, EKeys::W, true, false);
	MapCameraMovement(IMC, *Move, EKeys::S, true, true);
	MapCameraMovement(IMC, *Move, EKeys::A, false, true);
	MapCameraMovement(IMC, *Move, EKeys::D, false, false);

	FKismetEditorUtilities::CompileBlueprint(&BP);
	AVTBOWTSpectator* Defaults = CastChecked<AVTBOWTSpectator>(BP.GeneratedClass->GetDefaultObject());
	Defaults->CameraNavigateAction = Navigate;
	Defaults->CameraMoveAction = Move;
	Defaults->CameraLookAction = Look;
	return SaveAsset(Navigate) && SaveAsset(Move) && SaveAsset(Look) && SaveAsset(&IMC) && SaveAsset(&BP);
}

bool BuildGraph(UBlueprint* BP, const TArray<UInputAction*>& Actions, int32 StartIndex = 0)
{
	UEdGraph* Graph = FBlueprintEditorUtils::FindEventGraph(BP);
	if (!Graph)
	{
		Graph = FBlueprintEditorUtils::CreateNewGraph(BP, TEXT("EventGraph"), UEdGraph::StaticClass(),
		                                              UEdGraphSchema_K2::StaticClass());
		FBlueprintEditorUtils::AddUbergraphPage(BP, Graph);
	}

	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();
	auto Place = [Graph](UK2Node* Node, int32 X, int32 Y)
	{
		Graph->AddNode(Node, false, false);
		Node->CreateNewGuid();
		Node->PostPlacedNewNode();
		Node->AllocateDefaultPins();
		Node->NodePosX = X;
		Node->NodePosY = Y;
	};
	auto Call = [&](FName Function, int32 X, int32 Y)
	{
		auto* Node = NewObject<UK2Node_CallFunction>(Graph);
		Node->SetFromFunction(AVTBOWTSpectator::StaticClass()->FindFunctionByName(Function));
		Place(Node, X, Y);

		return Node;
	};
	auto Wire = [Schema](UK2Node* From, FName Output, UK2Node* To, FName Input)
	{
		UEdGraphPin* A = From->FindPin(Output);
		UEdGraphPin* B = To->FindPin(Input);
		const bool bConnected = A && B && Schema->TryCreateConnection(A, B);
		if (!bConnected)
		{
			UE_LOG(LogTemp, Error, TEXT("Cannot wire %s.%s -> %s.%s (pins %d/%d)"), *From->GetName(),
			       *Output.ToString(), *To->GetName(), *Input.ToString(), A != nullptr, B != nullptr);
		}

		return bConnected;
	};
	for (int32 Index = StartIndex; Index < Specs.Num(); ++Index)
	{
		const int32 Y = Index * 480;
		auto* Event = NewObject<UK2Node_EnhancedInputAction>(Graph);
		Event->InputAction = Actions[Index];
		Place(Event, 0, Y);
		auto* Make = NewObject<UK2Node_MakeStruct>(Graph);
		Make->StructType = Specs[Index].ContextType();
		Place(Make, 350, Y + 180);
		auto* Pack = NewObject<UK2Node_CallFunction>(Graph);
		Pack->SetFromFunction(
		    UBlueprintInstancedStructLibrary::StaticClass()->FindFunctionByName(TEXT("MakeInstancedStruct")));
		Place(Pack, 750, Y);
		if (!Wire(Make, Make->StructType->GetFName(), Pack, TEXT("Value")))
		{
			return false;
		}

		auto* Send = Call(TEXT("SendEditContext"), 1100, Y);
		if (!Wire(Pack, UEdGraphSchema_K2::PN_ReturnValue, Send, TEXT("Context")) ||
		    !Wire(Pack, UEdGraphSchema_K2::PN_Then, Send, UEdGraphSchema_K2::PN_Execute))
		{
			return false;
		}

		if (Specs[Index].ContextType() == FOWTSelectObjectContext::StaticStruct())
		{
			auto* Trace = Call(TEXT("TraceSelectableObject"), 350, Y);
			if (!Wire(Event, TEXT("Triggered"), Trace, UEdGraphSchema_K2::PN_Execute) ||
			    !Wire(Trace, UEdGraphSchema_K2::PN_Then, Pack, UEdGraphSchema_K2::PN_Execute) ||
			    !Wire(Trace, UEdGraphSchema_K2::PN_ReturnValue, Make, TEXT("SelectedObject")))
			{
				return false;
			}
		}
		else if (!Wire(Event, TEXT("Triggered"), Pack, UEdGraphSchema_K2::PN_Execute))
		{
			return false;
		}
	}

	return true;
}

bool UpgradeTypedContexts(UBlueprint* BP)
{
	UEdGraph* Graph = FBlueprintEditorUtils::FindEventGraph(BP);
	if (!Graph)
	{
		return false;
	}

	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();
	const TArray<UEdGraphNode*> Nodes = Graph->Nodes;
	auto Place = [Graph](UK2Node* Node, int32 X, int32 Y)
	{
		Graph->AddNode(Node, false, false);
		Node->CreateNewGuid();
		Node->PostPlacedNewNode();
		Node->AllocateDefaultPins();
		Node->NodePosX = X;
		Node->NodePosY = Y;
	};
	for (UEdGraphNode* Node : Nodes)
	{
		UK2Node_CallFunction* Old = Cast<UK2Node_CallFunction>(Node);
		if (!Old || Old->FunctionReference.GetMemberName() != TEXT("MakeEditContext"))
		{
			continue;
		}

		UEdGraphPin* CommandPin = Old->FindPin(TEXT("Command"));
		UEdGraphPin* OldOutput = Old->FindPin(UEdGraphSchema_K2::PN_ReturnValue);
		if (!CommandPin || !OldOutput || OldOutput->LinkedTo.Num() != 1)
		{
			return false;
		}

		const FActionSpec* Spec = Specs.FindByPredicate(
		    [CommandPin](const FActionSpec& Candidate)
		    {
			    return Candidate.ContextType()->GetName() == TEXT("OWT") + CommandPin->DefaultValue + TEXT("Context");
		    });
		if (!Spec)
		{
			return false;
		}

		UEdGraphPin* SendContext = OldOutput->LinkedTo[0];
		UEdGraphPin* SendExec = SendContext->GetOwningNode()->FindPin(UEdGraphSchema_K2::PN_Execute);
		if (!SendExec || SendExec->LinkedTo.Num() != 1)
		{
			return false;
		}

		UEdGraphPin* PreviousExec = SendExec->LinkedTo[0];

		auto* Make = NewObject<UK2Node_MakeStruct>(Graph);
		Make->StructType = Spec->ContextType();
		Place(Make, Old->NodePosX - 300, Old->NodePosY + 100);
		auto* Pack = NewObject<UK2Node_CallFunction>(Graph);
		Pack->SetFromFunction(
		    UBlueprintInstancedStructLibrary::StaticClass()->FindFunctionByName(TEXT("MakeInstancedStruct")));
		Place(Pack, Old->NodePosX + 100, Old->NodePosY - 140);
		if (UEdGraphPin* Selected = Old->FindPin(TEXT("SelectedObject")))
		{
			if (UEdGraphPin* NewSelected = Make->FindPin(TEXT("SelectedObject")))
			{
				if (Selected->LinkedTo.Num() > 0 && !Schema->TryCreateConnection(Selected->LinkedTo[0], NewSelected))
				{
					return false;
				}
			}
		}

		SendExec->BreakAllPinLinks();
		SendContext->BreakAllPinLinks();
		if (!Schema->TryCreateConnection(Make->FindPinChecked(Make->StructType->GetFName()),
		                                 Pack->FindPinChecked(TEXT("Value"))) ||
		    !Schema->TryCreateConnection(PreviousExec, Pack->FindPinChecked(UEdGraphSchema_K2::PN_Execute)) ||
		    !Schema->TryCreateConnection(Pack->FindPinChecked(UEdGraphSchema_K2::PN_Then), SendExec) ||
		    !Schema->TryCreateConnection(Pack->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue), SendContext))
		{
			return false;
		}

		Old->BreakAllNodeLinks();
		Graph->RemoveNode(Old);
	}

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BP);

	return true;
}

bool Validate(UBlueprint* BP, UInputMappingContext* IMC, const TArray<UInputAction*>& Actions)
{
	FEditorScriptExecutionGuard ScriptGuard;
	if (!BP || !BP->GeneratedClass || !IMC || IMC->GetMappings().Num() != Specs.Num() + 7 ||
	    Actions.Num() != Specs.Num())
	{
		return false;
	}

	if (Actions.Contains(nullptr))
	{
		return false;
	}

	FCompilerResultsLog Results;
	FKismetEditorUtilities::CompileBlueprint(BP, EBlueprintCompileOptions::None, &Results);
	if (Results.NumErrors)
	{
		return false;
	}

	const UWorld::InitializationValues InitValues = UWorld::InitializationValues()
	                                                    .AllowAudioPlayback(false)
	                                                    .CreatePhysicsScene(true)
	                                                    .CreateNavigation(false)
	                                                    .CreateAISystem(false)
	                                                    .ShouldSimulatePhysics(false);
	UWorld* World =
	    UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &InitValues);
	AVTBOWTSpectator* Pawn = World->SpawnActor<AVTBOWTSpectator>(BP->GeneratedClass);
	UVTBOWTEditorSubsystem* Hub = World->GetSubsystem<UVTBOWTEditorSubsystem>();
	UOWTInputValidationMode* Mode = NewObject<UOWTInputValidationMode>(Hub);
	bool bOK = Pawn && Hub && Hub->SetActiveEditMode(Mode);
	UEnhancedInputComponent* Binding = Pawn ? Pawn->FindComponentByClass<UEnhancedInputComponent>() : nullptr;
	bOK &= Binding && Pawn->MappingContext == IMC;
	UE_LOG(LogTemp, Display, TEXT("OWT validate setup: Pawn=%d Hub=%d Binding=%d Context=%d"), Pawn != nullptr,
	       Hub != nullptr, Binding != nullptr, Binding && Pawn->MappingContext == IMC);

	if (bOK)
	{
		bOK &= !Hub->IsEditingEnabled();
		UEnhancedInputComponent* Input = Binding;
		UInputDelegateBinding::BindInputDelegates(Pawn->GetClass(), Input, Pawn);
		bOK &= !Pawn->SendEditContext(FInstancedStruct::Make<FOWTSetTranslationContext>());
		for (const auto& Event : Input->GetActionEventBindings())
		{
			if (Event->GetAction() == Actions.Last() && Event->GetTriggerEvent() == ETriggerEvent::Triggered)
			{
				Event->Execute(FInputActionInstance(Actions.Last()));
			}
		}
		bOK &= Hub->IsEditingEnabled() && Hub->GetTransformGizmoMode() == EToolContextTransformGizmoMode::Translation;
		for (int32 Index = 0; Index < Actions.Num() - 1; ++Index)
		{
			const int32 Before = Mode->ReceivedCount;
			for (const auto& Event : Input->GetActionEventBindings())
			{
				if (Event->GetAction() == Actions[Index] && Event->GetTriggerEvent() == ETriggerEvent::Triggered)
				{
					Event->Execute(FInputActionInstance(Actions[Index]));
				}
			}

			bOK &=
			    Mode->ReceivedCount == Before + 1 && Mode->LastRequest.GetScriptStruct() == Specs[Index].ContextType();
			UE_LOG(LogTemp, Display, TEXT("OWT event %s: delta=%d context=%s expected=%s bindings=%d"),
			       Specs[Index].Name, Mode->ReceivedCount - Before, *GetNameSafe(Mode->LastRequest.GetScriptStruct()),
			       *Specs[Index].ContextType()->GetName(), Input->GetActionEventBindings().Num());
		}

		// An already-hit actor crosses the interface unchanged; hide must not clear selection.
		AActor* Target = World->SpawnActor<AActor>();
		FOWTSelectObjectContext Selection;
		Selection.SelectedObject = Target;
		bOK &= Pawn->SendEditContext(FInstancedStruct::Make(Selection));
		bOK &= Hub->SelectedObject.Get() == Target &&
		       Mode->LastRequest.Get<FOWTSelectObjectContext>().SelectedObject.Get() == Target;
		bOK &= Pawn->SendEditContext(FInstancedStruct::Make<FOWTHideSelectionGizmoContext>());
		bOK &= Hub->SelectedObject.Get() == Target;
		bOK &= !Pawn->SendEditContext(FInstancedStruct());
		FOWTValidationContext Extension;
		Extension.Message = TEXT("Registered without router changes");
		Extension.Value = 42;
		const FInstancedStruct Payload = FInstancedStruct::Make(Extension);
		bOK &= !Pawn->SendEditContext(Payload);
		bOK &= !Mode->BindContext<FOWTValidationContext>(Mode, TEXT("MissingHandler"));
		bOK &= !Mode->BindContext<FOWTValidationContext>(Mode, GET_FUNCTION_NAME_CHECKED(UVTBOWTObjectEditMode, Undo));
		bOK &= Mode->BindContext<FOWTValidationContext>(
		    Mode, GET_FUNCTION_NAME_CHECKED(UOWTInputValidationMode, HandleValidation));
		bOK &= Pawn->SendEditContext(Payload) && Mode->ValidationCount == 1 &&
		       Mode->ValidationMessage == Extension.Message && Mode->ValidationValue == 42;
		// Rebinding replaces rather than accumulates handlers.
		UOWTInputValidationMode* Replacement = NewObject<UOWTInputValidationMode>(Hub);
		bOK &= Mode->BindContext<FOWTValidationContext>(
		    Replacement, GET_FUNCTION_NAME_CHECKED(UOWTInputValidationMode, HandleValidation));
		bOK &= Pawn->SendEditContext(Payload) && Replacement->ValidationCount == 1 && Mode->ValidationCount == 1;
		bOK &= Mode->UnbindContextHandler(FOWTValidationContext::StaticStruct());
		bOK &= !Pawn->SendEditContext(Payload);
		UOWTInputValidationMode* TemporaryReceiver = NewObject<UOWTInputValidationMode>(Hub);
		bOK &= Mode->BindContext<FOWTValidationContext>(
		    TemporaryReceiver, GET_FUNCTION_NAME_CHECKED(UOWTInputValidationMode, HandleValidation));
		TemporaryReceiver->MarkAsGarbage();
		bOK &= !Pawn->SendEditContext(Payload);
		UE_LOG(LogTemp, Display, TEXT("OWT graph/selection checks: %d"), bOK);

		// A movable root exercises real manager registration, proxy transforms and actor cleanup.
		USceneComponent* RootComponent = NewObject<USceneComponent>(Target);
		Target->SetRootComponent(RootComponent);
		Target->AddInstanceComponent(RootComponent);
		RootComponent->SetMobility(EComponentMobility::Movable);
		RootComponent->RegisterComponent();
		Hub->SetSelectedObject(Target);
		bOK &= Hub->GetToolsContext() && Hub->GetTransformGizmo() && Hub->GetTransformProxy();
		if (Hub->GetTransformGizmo())
		{
			bOK &= ValidateOWTBaseGizmoMaterials(*Hub);
			bOK &= ValidateOWTGizmoSnapping(*Pawn, *Hub, *Target);
			bOK &= ValidateOWTCameraNavigation(*Pawn, *Hub);
			bOK &= Hub->GetTransformGizmo()->IsA<UVTBOWTBaseTransformGizmo>();
			bOK &= Hub->GetTransformProxy()->GetOuter() == Hub->GetTransformGizmo();
			ACombinedTransformGizmoActor* Handles = Hub->GetTransformGizmo()->GetGizmoActor();
			bOK &= Handles->GetClass() == ACombinedTransformGizmoActor::StaticClass();
			const TArray<int32> AxisActions = {5, 6, 7};
			for (int32 AxisIndex = 0; AxisIndex < AxisActions.Num(); ++AxisIndex)
			{
				for (const auto& Event : Input->GetActionEventBindings())
				{
					if (Event->GetAction() == Actions[AxisActions[AxisIndex]] &&
					    Event->GetTriggerEvent() == ETriggerEvent::Triggered)
					{
						Event->Execute(FInputActionInstance(Event->GetAction()));
					}
				}
				bOK &= Handles->TranslateX->IsVisible() == (AxisIndex == 0);
				bOK &= Handles->TranslateXY->IsVisible() == (AxisIndex == 0);
				bOK &= Handles->RotateX->IsVisible() == (AxisIndex == 1);
				bOK &= Handles->UniformScale->IsVisible() == (AxisIndex == 2);
				bOK &= Handles->PlaneScaleXY->IsVisible() == (AxisIndex == 2);
			}
			// Deterministic world-sized handle in the headless world: test the real InputRouter/Behavior drag path.
			Hub->SetTransformGizmoMode(EToolContextTransformGizmoMode::Translation);
			PrepareOWTGizmoHeadlessHandles(*Handles);
			FOWTGizmoPointerContext Pointer;
			Pointer.RayOrigin = FVector(50, -100, 0);
			Pointer.RayDirection = FVector::YAxisVector;
			Pointer.bPressed = true;
			Pointer.bDown = true;
			bOK &= Pawn->SendEditContext(FInstancedStruct::Make(Pointer));
			bOK &= Hub->HasGizmoCapture();
			Pointer.bPressed = false;
			Pointer.RayOrigin.X = 70;
			bOK &= Pawn->SendEditContext(FInstancedStruct::Make(Pointer));
			bOK &= Target->GetActorLocation().Equals(FVector(20, 0, 0), 0.01);
			Pointer.bDown = false;
			Pointer.bReleased = true;
			bOK &= Pawn->SendEditContext(FInstancedStruct::Make(Pointer));
			bOK &= !Hub->HasGizmoCapture();
			UE_LOG(LogTemp, Display, TEXT("OWT F2/axis visibility/behavior drag checks: %d"), bOK);
			TWeakObjectPtr<AActor> GizmoActor = Hub->GetTransformGizmo()->GetGizmoActor();
			Hub->GetTransformProxy()->SetTransform(FTransform(FVector(100, 200, 300)));
			bOK &= Target->GetActorLocation().Equals(FVector(100, 200, 300));
			Hub->HideSelectionGizmo();
			bOK &= !Hub->GetTransformGizmo() && !Hub->GetTransformProxy() && !GizmoActor.IsValid();
			bOK &= Hub->SelectedObject.Get() == Target;
			Hub->ShowSelectionGizmo();
			bOK &= Hub->GetTransformGizmo() != nullptr;
			bOK &= ValidateOWTCustomGizmo(*Pawn, *Hub, *Target);
			Target->Destroy();
			Hub->Tick(0.016f);
			bOK &= !Hub->GetTransformGizmo();
		}
		UVTBOWTEditorToolsContext* ReleasedContext = Hub->GetToolsContext();
		bOK &= ReleasedContext && ReleasedContext->IsInitialized();
		Hub->ShutdownToolsContext();
		if (ReleasedContext)
		{
			ReleasedContext->Shutdown();
			bOK &= !ReleasedContext->IsInitialized() && !ReleasedContext->InputRouter &&
			       !ReleasedContext->ToolManager && !ReleasedContext->GizmoManager;
		}
		Hub->ShutdownToolsContext();
		bOK &= !Hub->GetToolsContext();
		bOK &= Hub->InitializeToolsContext();
		UE_LOG(LogTemp, Display, TEXT("OWT tools lifecycle checks: %d"), bOK);
	}

	// Test actual mapping triggers against all modifier combinations and both keyboard sides.
	APlayerController* PC = World->SpawnActor<APlayerController>();
	UOWTValidationPlayerInput* PlayerInput = NewObject<UOWTValidationPlayerInput>(PC);
	for (const FEnhancedActionKeyMapping& Mapping : IMC->GetMappings())
	{
		for (int32 Side = 0; Side < 2; ++Side)
		{
			for (int32 Mask = 0; Mask < 4; ++Mask)
			{
				const bool Ctrl = (Mask & 1) != 0, Shift = (Mask & 2) != 0;
				PlayerInput->SetHeld(EKeys::LeftControl, Ctrl && Side == 0);
				PlayerInput->SetHeld(EKeys::RightControl, Ctrl && Side == 1);
				PlayerInput->SetHeld(EKeys::LeftShift, Shift && Side == 0);
				PlayerInput->SetHeld(EKeys::RightShift, Shift && Side == 1);
				for (UInputTrigger* Trigger : Mapping.Triggers)
				{
					if (UVTBOWTModifierKeyTrigger* Modifiers = Cast<UVTBOWTModifierKeyTrigger>(Trigger))
					{
						const bool Expected = (!Modifiers->bRequireControl || Ctrl) &&
						                      (!Modifiers->bRequireShift || Shift) &&
						                      (!Modifiers->bDisallowShift || !Shift);
						bOK &= (Modifiers->UpdateState_Implementation(PlayerInput, FInputActionValue(true), 0.016f) ==
						        ETriggerState::Triggered) == Expected;
					}
					else if (UInputTriggerPressed* Pressed = Cast<UInputTriggerPressed>(Trigger))
					{
						Pressed->LastValue = FInputActionValue(false);
						bOK &= Pressed->UpdateState(PlayerInput, FInputActionValue(true), 0.016f) ==
						       ETriggerState::Triggered;
						Pressed->LastValue = FInputActionValue(true);
						bOK &=
						    Pressed->UpdateState(PlayerInput, FInputActionValue(true), 0.016f) == ETriggerState::None;
						Pressed->LastValue = FInputActionValue(false);
					}
				}
			}
		}
	}

	TWeakObjectPtr<AActor> ShutdownGizmoActor;
	if (Hub)
	{
		AActor* ShutdownTarget = World->SpawnActor<AActor>();
		USceneComponent* RootComponent = NewObject<USceneComponent>(ShutdownTarget);
		ShutdownTarget->SetRootComponent(RootComponent);
		ShutdownTarget->AddInstanceComponent(RootComponent);
		RootComponent->SetMobility(EComponentMobility::Movable);
		RootComponent->RegisterComponent();
		Hub->SetSelectedObject(ShutdownTarget);
		bOK &= Hub->GetTransformGizmo() != nullptr;
		if (Hub->GetTransformGizmo())
		{
			ShutdownGizmoActor = Hub->GetTransformGizmo()->GetGizmoActor();
		}
	}

	World->DestroyWorld(false);
	bOK &= Hub && !Hub->GetToolsContext() && !Hub->GetTransformGizmo() && !ShutdownGizmoActor.IsValid();
	UE_LOG(LogTemp, Display,
	       TEXT("OWT_INPUT_VALIDATION: %s (11 BP edit events; camera bindings; typed routing; new context "
	            "registration; invalid signature; "
	            "rebinding; unbinding; expired receiver; modifiers)"),
	       bOK ? TEXT("PASS") : TEXT("FAIL"));

	return bOK;
}
} // namespace OWTInputAssets

void UOWTValidationPlayerInput::SetHeld(FKey Key, bool bHeld)
{
	GetKeyStateMap().FindOrAdd(Key).bDown = bHeld;
}

UCreateOWTInputCommandlet::UCreateOWTInputCommandlet()
{
	IsClient = false;
	IsServer = false;
	IsEditor = true;
	LogToConsole = true;
}

int32 UCreateOWTInputCommandlet::Main(const FString& Params)
{
	using namespace OWTInputAssets;
	const bool bValidateOnly = FParse::Param(*Params, TEXT("ValidateOnly"));
	TArray<UInputAction*> Actions;
	UInputMappingContext* IMC = nullptr;
	UBlueprint* BP = nullptr;
	if (bValidateOnly || FParse::Param(*Params, TEXT("UpgradeTypedContexts")) ||
	    FParse::Param(*Params, TEXT("UpgradeRuntimeInput")) || FParse::Param(*Params, TEXT("UpgradeCameraInput")))
	{
		for (const FActionSpec& Spec : Specs)
		{
			Actions.Add(LoadObject<UInputAction>(nullptr, *(Root + Spec.Name)));
		}

		IMC = LoadObject<UInputMappingContext>(nullptr, *(Root + TEXT("IMC_OWTEdit")));
		BP = LoadObject<UBlueprint>(nullptr, *PawnPackage);
		if (FParse::Param(*Params, TEXT("UpgradeRuntimeInput")))
		{
			if (!BP || !IMC)
			{
				return 1;
			}
			const FString BackupDir = FPaths::ProjectSavedDir() / TEXT("Backups/BeforeRuntimeInput");
			IFileManager::Get().MakeDirectory(*BackupDir, true);
			for (UObject* Asset : TArray<UObject*>{BP, IMC})
			{
				const FString Backup = BackupDir / (Asset->GetName() + TEXT(".uasset"));
				if (!FPaths::FileExists(Backup) &&
				    IFileManager::Get().Copy(*Backup, *AssetFilename(Asset->GetOutermost()->GetName())) != COPY_OK)
				{
					return 1;
				}
			}
			for (USCS_Node* Node : TArray<USCS_Node*>(BP->SimpleConstructionScript->GetAllNodes()))
			{
				if (Node->ComponentClass->GetFName() == TEXT("AbilityBindingComponent"))
				{
					BP->SimpleConstructionScript->RemoveNode(Node);
				}
			}
			if (!Actions.Last())
			{
				const FActionSpec& Spec = Specs.Last();
				UInputAction* Action =
				    NewObject<UInputAction>(CreatePackage(*(Root + Spec.Name)), Spec.Name, RF_Public | RF_Standalone);
				Action->ValueType = EInputActionValueType::Boolean;
				Actions.Last() = Action;
				Map(IMC, Action, Spec.Key, false, false, false);
				if (!BuildGraph(BP, Actions, Specs.Num() - 1) || !SaveAsset(Action))
				{
					return 1;
				}
			}
			FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BP);
			FKismetEditorUtilities::CompileBlueprint(BP);
			CastChecked<AVTBOWTSpectator>(BP->GeneratedClass->GetDefaultObject())->MappingContext = IMC;
			if (!SaveAsset(IMC) || !SaveAsset(BP))
			{
				return 1;
			}
		}
		if (FParse::Param(*Params, TEXT("UpgradeCameraInput")) && (!BP || !IMC || !UpgradeCameraInput(*BP, *IMC)))
		{
			return 1;
		}
		const bool bUpgrade = FParse::Param(*Params, TEXT("UpgradeTypedContexts"));
		if (bUpgrade)
		{
			const FString Backup =
			    FPaths::ProjectSavedDir() / TEXT("Backups/BP_VTBOWTSpectator-before-typed-contexts.uasset");
			IFileManager::Get().MakeDirectory(*FPaths::GetPath(Backup), true);
			if (!FPaths::FileExists(Backup) &&
			    IFileManager::Get().Copy(*Backup, *AssetFilename(PawnPackage)) != COPY_OK)
			{
				return 1;
			}

			if (!BP || !UpgradeTypedContexts(BP))
			{
				return 1;
			}
		}

		if (!Validate(BP, IMC, Actions))
		{
			return 1;
		}

		if (bUpgrade && !SaveAsset(BP))
		{
			return 1;
		}

		// Optional normalization of trigger runtime fields on assets generated by this tool.
		if (FParse::Param(*Params, TEXT("ResetTriggerState")) && !SaveAsset(IMC))
		{
			return 1;
		}

		return 0;
	}

	TArray<FString> Packages = {PawnPackage, Root + TEXT("IMC_OWTEdit"), Root + TEXT("IA_CameraNavigate"),
	                            Root + TEXT("IA_CameraMove"), Root + TEXT("IA_CameraLook")};
	for (const FActionSpec& Spec : Specs)
	{
		Packages.Add(Root + Spec.Name);
	}

	for (const FString& Package : Packages)
	{
		if (FPaths::FileExists(AssetFilename(Package)))
		{
			UE_LOG(LogTemp, Error, TEXT("Refusing to overwrite existing asset: %s. Use -ValidateOnly to check it."),
			       *Package);

			return 1;
		}
	}

	IMC = NewObject<UInputMappingContext>(CreatePackage(*(Root + TEXT("IMC_OWTEdit"))), TEXT("IMC_OWTEdit"),
	                                      RF_Public | RF_Standalone);
	for (const FActionSpec& Spec : Specs)
	{
		UInputAction* Action =
		    NewObject<UInputAction>(CreatePackage(*(Root + Spec.Name)), Spec.Name, RF_Public | RF_Standalone);
		Action->ValueType = EInputActionValueType::Boolean;
		Actions.Add(Action);
		Map(IMC, Action, Spec.Key, Spec.Ctrl, Spec.Shift, Spec.NoShift);
		if (Spec.ContextType() == FOWTRedoContext::StaticStruct())
		{
			Map(IMC, Action, EKeys::Y, true, false, false);
		}
	}

	BP = FKismetEditorUtilities::CreateBlueprint(AVTBOWTSpectator::StaticClass(), CreatePackage(*PawnPackage),
	                                             TEXT("BP_VTBOWTSpectator"), BPTYPE_Normal);
	if (!BP)
	{
		return 1;
	}

	CastChecked<AVTBOWTSpectator>(BP->GeneratedClass->GetDefaultObject())->MappingContext = IMC;
	if (!BuildGraph(BP, Actions))
	{
		UE_LOG(LogTemp, Error, TEXT("Graph wiring failed"));

		return 1;
	}

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BP);
	if (!UpgradeCameraInput(*BP, *IMC) || !Validate(BP, IMC, Actions))
	{
		return 1;
	}

	for (UInputAction* Action : Actions)
	{
		if (!SaveAsset(Action))
		{
			return 1;
		}
	}

	if (!SaveAsset(IMC) || !SaveAsset(BP))
	{
		return 1;
	}

	UE_LOG(LogTemp, Display, TEXT("OWT_INPUT_SUCCESS: Saved BP_VTBOWTSpectator, IMC_OWTEdit and 14 InputActions."));

	return 0;
}
