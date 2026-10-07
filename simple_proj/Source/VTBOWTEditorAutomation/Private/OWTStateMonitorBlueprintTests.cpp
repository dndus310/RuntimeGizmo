#if WITH_DEV_AUTOMATION_TESTS

#include "OWTStateMonitorModel.h"
#include "OWTStateMonitorWidget.h"
#include "EdGraph/EdGraph.h"
#include "EdGraphSchema_K2.h"
#include "Engine/Blueprint.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "K2Node_CallFunction.h"
#include "K2Node_FunctionEntry.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/AutomationTest.h"
#include "UObject/NoExportTypes.h"
#include "UObject/StrongObjectPtr.h"
#include "UObject/StructOnScope.h"
#include "UObject/UnrealType.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTStateMonitorBlueprintTest, "OWT.StateMonitor.BlueprintStructInput",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FOWTStateMonitorBlueprintTest::RunTest(const FString& Parameters)
{
	const FName BlueprintName =
	    MakeUniqueObjectName(GetTransientPackage(), UBlueprint::StaticClass(), TEXT("MonitorStructInput"));
	TStrongObjectPtr<UBlueprint> Blueprint(FKismetEditorUtilities::CreateBlueprint(
	    UObject::StaticClass(), GetTransientPackage(), BlueprintName, BPTYPE_Normal, UBlueprint::StaticClass(),
	    UBlueprintGeneratedClass::StaticClass()));
	if (!TestNotNull(TEXT("Transient Blueprint"), Blueprint.Get()))
	{
		return false;
	}

	UEdGraph* Graph = FBlueprintEditorUtils::CreateNewGraph(Blueprint.Get(), TEXT("CaptureState"),
	                                                        UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());
	FBlueprintEditorUtils::AddFunctionGraph(Blueprint.Get(), Graph, true, static_cast<UFunction*>(nullptr));
	TArray<UK2Node_FunctionEntry*> Entries;
	Graph->GetNodesOfClass(Entries);
	if (!TestEqual(TEXT("Function entry"), Entries.Num(), 1))
	{
		return false;
	}

	UK2Node_FunctionEntry* Entry = Entries[0];
	FEdGraphPinType MonitorType;
	MonitorType.PinCategory = UEdGraphSchema_K2::PC_Object;
	MonitorType.PinSubCategoryObject = UOWTStateMonitorWidget::StaticClass();
	UEdGraphPin* MonitorPin = Entry->CreateUserDefinedPin(TEXT("Monitor"), MonitorType, EGPD_Output);
	FEdGraphPinType StateType;
	StateType.PinCategory = UEdGraphSchema_K2::PC_Struct;
	StateType.PinSubCategoryObject = TBaseStructure<FTransform>::Get();
	UEdGraphPin* StatePin = Entry->CreateUserDefinedPin(TEXT("State"), StateType, EGPD_Output);

	UK2Node_CallFunction* Submit = NewObject<UK2Node_CallFunction>(Graph);
	Submit->FunctionReference.SetExternalMember(GET_FUNCTION_NAME_CHECKED(UOWTStateMonitorWidget, SubmitSnapshot),
	                                            UOWTStateMonitorWidget::StaticClass());
	Graph->AddNode(Submit, false, false);
	Submit->CreateNewGuid();
	Submit->AllocateDefaultPins();
	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();
	bool bConnected =
	    TestTrue(TEXT("Execution connected"),
	             Schema->TryCreateConnection(Entry->FindPinChecked(UEdGraphSchema_K2::PN_Then), Submit->GetExecPin()));
	bConnected &= TestTrue(TEXT("Widget connected"),
	                       Schema->TryCreateConnection(MonitorPin, Submit->FindPinChecked(UEdGraphSchema_K2::PN_Self)));
	bConnected &= TestTrue(TEXT("Wildcard accepts FTransform"),
	                       Schema->TryCreateConnection(StatePin, Submit->FindPinChecked(TEXT("Snapshot"))));
	if (!bConnected)
	{
		return false;
	}
	Schema->TrySetDefaultValue(*Submit->FindPinChecked(TEXT("SourceId")), TEXT("BlueprintContract"));
	Schema->TrySetDefaultText(*Submit->FindPinChecked(TEXT("Label")), FText::FromString(TEXT("Blueprint transform")));
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint.Get());
	FKismetEditorUtilities::CompileBlueprint(Blueprint.Get());
	if (!TestTrue(TEXT("Wildcard Blueprint compiles"), Blueprint->Status != BS_Error))
	{
		return false;
	}

	TStrongObjectPtr<UObject> Instance(NewObject<UObject>(GetTransientPackage(), Blueprint->GeneratedClass));
	TStrongObjectPtr<UOWTStateMonitorWidget> Monitor(NewObject<UOWTStateMonitorWidget>());
	UFunction* Capture = Instance->FindFunction(TEXT("CaptureState"));
	if (!TestNotNull(TEXT("Compiled function"), Capture))
	{
		return false;
	}
	FStructOnScope Arguments(Capture);
	FObjectPropertyBase* MonitorProperty = FindFProperty<FObjectPropertyBase>(Capture, TEXT("Monitor"));
	FStructProperty* StateProperty = FindFProperty<FStructProperty>(Capture, TEXT("State"));
	if (!TestNotNull(TEXT("Widget parameter"), MonitorProperty))
	{
		return false;
	}
	if (!TestNotNull(TEXT("Struct parameter"), StateProperty))
	{
		return false;
	}
	MonitorProperty->SetObjectPropertyValue_InContainer(Arguments.GetStructMemory(), Monitor.Get());
	FTransform& State = *StateProperty->ContainerPtrToValuePtr<FTransform>(Arguments.GetStructMemory());
	State = FTransform(FRotator(10, 20, 30), FVector(321, 456, 789), FVector(2, 3, 4));
	Instance->ProcessEvent(Capture, Arguments.GetStructMemory());
	TSharedPtr<FOWTStateMonitorSource> Source = Monitor->GetMonitorModel()->FindSource(TEXT("BlueprintContract"));
	if (!TestTrue(TEXT("Blueprint thunk submits actual struct"), Source.IsValid()))
	{
		return false;
	}
	TestTrue(TEXT("Reflected transform values preserved"), Source->CurrentJson.Contains(TEXT("321")));
	TestTrue(TEXT("Reflected transform type preserved"), Source->StructType.Contains(TEXT("Transform")));
	const uint64 FirstSequence = Source->Sequence;
	State.SetTranslation(FVector(999, 456, 789));
	Instance->ProcessEvent(Capture, Arguments.GetStructMemory());
	Source = Monitor->GetMonitorModel()->FindSource(TEXT("BlueprintContract"));
	TestTrue(TEXT("Second BP submission updates current"), Source->Sequence > FirstSequence);
	TestTrue(TEXT("Field changes recorded"), Source->Changes.Num() > 0);
	Monitor->ClearHistory(TEXT("BlueprintContract"));
	TestTrue(TEXT("UMG clear preserves current"),
	         Monitor->GetMonitorModel()->FindSource(TEXT("BlueprintContract")).IsValid());
	Monitor->ResetMonitor();
	TestEqual(TEXT("UMG reset removes sources"), Monitor->GetMonitorModel()->GetSources().Num(), 0);
	return true;
}

#endif
