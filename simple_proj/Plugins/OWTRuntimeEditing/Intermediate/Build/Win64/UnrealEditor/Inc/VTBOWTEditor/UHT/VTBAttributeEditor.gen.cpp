// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VTBAttributeEditor.h"
#include "Context/SaveTransformContext.h"
#include "Events/OWTAttributeTypes.h"
#include "Events/OWTEventTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBAttributeEditor() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
ENGINE_API UClass* Z_Construct_UClass_AActor();
OWTEVENTCORE_API UClass* Z_Construct_UClass_UOWTNotificationCenter_NoRegister();
OWTEVENTCORE_API UFunction* Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature();
OWTEVENTCORE_API UScriptStruct* Z_Construct_UScriptStruct_FOWTEventRecord();
OWTRUNTIMEDUPLICATION_API UClass* Z_Construct_UClass_UOWTRuntimeActorDuplicator_NoRegister();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBAttributeEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBAttributeEditor_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeStateStore_NoRegister();
VTBOWTEDITOR_API UEnum* Z_Construct_UEnum_VTBOWTEditor_EOWTTransformEditPhase();
VTBOWTEDITOR_API UEnum* Z_Construct_UEnum_VTBOWTEditor_EOWTTransformField();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTAttributeSnapshot();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FSaveTransformContext();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AVTBAttributeEditor Function GetLatestEventSequence **********************
struct Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence_Statics
{
	struct VTBAttributeEditor_eventGetLatestEventSequence_Parms
	{
		int64 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Monitor" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetLatestEventSequence constinit property declarations ****************
	static const UECodeGen_Private::FInt64PropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetLatestEventSequence constinit property declarations ******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetLatestEventSequence Property Definitions ***************************
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventGetLatestEventSequence_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence_Statics::PropPointers) < 2048);
// ********** End Function GetLatestEventSequence Property Definitions *****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBAttributeEditor, nullptr, "GetLatestEventSequence", 	Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence_Statics::VTBAttributeEditor_eventGetLatestEventSequence_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence_Statics::VTBAttributeEditor_eventGetLatestEventSequence_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBAttributeEditor::execGetLatestEventSequence)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int64*)Z_Param__Result=P_THIS->GetLatestEventSequence();
	P_NATIVE_END;
}
// ********** End Class AVTBAttributeEditor Function GetLatestEventSequence ************************

// ********** Begin Class AVTBAttributeEditor Function GetMonitorEntries ***************************
struct Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics
{
	struct VTBAttributeEditor_eventGetMonitorEntries_Parms
	{
		int32 MaximumCount;
		TArray<FOWTEventRecord> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Monitor" },
		{ "CPP_Default_MaximumCount", "256" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetMonitorEntries constinit property declarations *********************
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaximumCount;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetMonitorEntries constinit property declarations ***********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetMonitorEntries Property Definitions ********************************
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::NewProp_MaximumCount = { "MaximumCount", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventGetMonitorEntries_Parms, MaximumCount), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FOWTEventRecord, METADATA_PARAMS(0, nullptr) }; // 2725191315
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventGetMonitorEntries_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 2725191315
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::NewProp_MaximumCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::PropPointers) < 2048);
// ********** End Function GetMonitorEntries Property Definitions **********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBAttributeEditor, nullptr, "GetMonitorEntries", 	Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::VTBAttributeEditor_eventGetMonitorEntries_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::VTBAttributeEditor_eventGetMonitorEntries_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBAttributeEditor::execGetMonitorEntries)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_MaximumCount);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<FOWTEventRecord>*)Z_Param__Result=P_THIS->GetMonitorEntries(Z_Param_MaximumCount);
	P_NATIVE_END;
}
// ********** End Class AVTBAttributeEditor Function GetMonitorEntries *****************************

// ********** Begin Class AVTBAttributeEditor Function GetSnapshot *********************************
struct Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot_Statics
{
	struct VTBAttributeEditor_eventGetSnapshot_Parms
	{
		FOWTAttributeSnapshot ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetSnapshot constinit property declarations ***************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetSnapshot constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetSnapshot Property Definitions **************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventGetSnapshot_Parms, ReturnValue), Z_Construct_UScriptStruct_FOWTAttributeSnapshot, METADATA_PARAMS(0, nullptr) }; // 1151875563
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot_Statics::PropPointers) < 2048);
// ********** End Function GetSnapshot Property Definitions ****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBAttributeEditor, nullptr, "GetSnapshot", 	Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot_Statics::VTBAttributeEditor_eventGetSnapshot_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot_Statics::VTBAttributeEditor_eventGetSnapshot_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBAttributeEditor::execGetSnapshot)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FOWTAttributeSnapshot*)Z_Param__Result=P_THIS->GetSnapshot();
	P_NATIVE_END;
}
// ********** End Class AVTBAttributeEditor Function GetSnapshot ***********************************

// ********** Begin Class AVTBAttributeEditor Function MarkSelectionBaseline ***********************
struct Z_Construct_UFunction_AVTBAttributeEditor_MarkSelectionBaseline_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function MarkSelectionBaseline constinit property declarations *****************
// ********** End Function MarkSelectionBaseline constinit property declarations *******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBAttributeEditor_MarkSelectionBaseline_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBAttributeEditor, nullptr, "MarkSelectionBaseline", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_MarkSelectionBaseline_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBAttributeEditor_MarkSelectionBaseline_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_AVTBAttributeEditor_MarkSelectionBaseline()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBAttributeEditor_MarkSelectionBaseline_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBAttributeEditor::execMarkSelectionBaseline)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->MarkSelectionBaseline();
	P_NATIVE_END;
}
// ********** End Class AVTBAttributeEditor Function MarkSelectionBaseline *************************

// ********** Begin Class AVTBAttributeEditor Function ParseSnapshotJson ***************************
struct Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics
{
	struct VTBAttributeEditor_eventParseSnapshotJson_Parms
	{
		FString Json;
		FOWTAttributeSnapshot OutSnapshot;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Json_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function ParseSnapshotJson constinit property declarations *********************
	static const UECodeGen_Private::FStrPropertyParams NewProp_Json;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutSnapshot;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ParseSnapshotJson constinit property declarations ***********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ParseSnapshotJson Property Definitions ********************************
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventParseSnapshotJson_Parms, Json), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Json_MetaData), NewProp_Json_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::NewProp_OutSnapshot = { "OutSnapshot", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventParseSnapshotJson_Parms, OutSnapshot), Z_Construct_UScriptStruct_FOWTAttributeSnapshot, METADATA_PARAMS(0, nullptr) }; // 1151875563
void Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((VTBAttributeEditor_eventParseSnapshotJson_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VTBAttributeEditor_eventParseSnapshotJson_Parms), &Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::NewProp_Json,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::NewProp_OutSnapshot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::PropPointers) < 2048);
// ********** End Function ParseSnapshotJson Property Definitions **********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBAttributeEditor, nullptr, "ParseSnapshotJson", 	Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::VTBAttributeEditor_eventParseSnapshotJson_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::VTBAttributeEditor_eventParseSnapshotJson_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBAttributeEditor::execParseSnapshotJson)
{
	P_GET_PROPERTY(FStrProperty,Z_Param_Json);
	P_GET_STRUCT_REF(FOWTAttributeSnapshot,Z_Param_Out_OutSnapshot);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=AVTBAttributeEditor::ParseSnapshotJson(Z_Param_Json,Z_Param_Out_OutSnapshot);
	P_NATIVE_END;
}
// ********** End Class AVTBAttributeEditor Function ParseSnapshotJson *****************************

// ********** Begin Class AVTBAttributeEditor Function PublishRequest ******************************
struct Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics
{
	struct VTBAttributeEditor_eventPublishRequest_Parms
	{
		FName Event;
		FString Json;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Json_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function PublishRequest constinit property declarations ************************
	static const UECodeGen_Private::FNamePropertyParams NewProp_Event;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Json;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function PublishRequest constinit property declarations **************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function PublishRequest Property Definitions ***********************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::NewProp_Event = { "Event", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventPublishRequest_Parms, Event), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventPublishRequest_Parms, Json), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Json_MetaData), NewProp_Json_MetaData) };
void Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((VTBAttributeEditor_eventPublishRequest_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VTBAttributeEditor_eventPublishRequest_Parms), &Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::NewProp_Event,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::NewProp_Json,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::PropPointers) < 2048);
// ********** End Function PublishRequest Property Definitions *************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBAttributeEditor, nullptr, "PublishRequest", 	Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::VTBAttributeEditor_eventPublishRequest_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::VTBAttributeEditor_eventPublishRequest_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBAttributeEditor::execPublishRequest)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_Event);
	P_GET_PROPERTY(FStrProperty,Z_Param_Json);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->PublishRequest(Z_Param_Event,Z_Param_Json);
	P_NATIVE_END;
}
// ********** End Class AVTBAttributeEditor Function PublishRequest ********************************

// ********** Begin Class AVTBAttributeEditor Function Redo ****************************************
static FName NAME_AVTBAttributeEditor_Redo = FName(TEXT("Redo"));
void AVTBAttributeEditor::Redo()
{
	UFunction* Func = FindFunctionChecked(NAME_AVTBAttributeEditor_Redo);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
	ProcessEvent(Func,NULL);
	}
	else
	{
		Redo_Implementation();
	}
}
struct Z_Construct_UFunction_AVTBAttributeEditor_Redo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "History" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function Redo constinit property declarations **********************************
// ********** End Function Redo constinit property declarations ************************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBAttributeEditor_Redo_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBAttributeEditor, nullptr, "Redo", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08080C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_Redo_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBAttributeEditor_Redo_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_AVTBAttributeEditor_Redo()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBAttributeEditor_Redo_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBAttributeEditor::execRedo)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Redo_Implementation();
	P_NATIVE_END;
}
// ********** End Class AVTBAttributeEditor Function Redo ******************************************

// ********** Begin Class AVTBAttributeEditor Function RequestDuplicate ****************************
struct Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics
{
	struct VTBAttributeEditor_eventRequestDuplicate_Parms
	{
		FOWTAttributeSnapshot Expected;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Expected_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function RequestDuplicate constinit property declarations **********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Expected;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function RequestDuplicate constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function RequestDuplicate Property Definitions *********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::NewProp_Expected = { "Expected", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventRequestDuplicate_Parms, Expected), Z_Construct_UScriptStruct_FOWTAttributeSnapshot, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Expected_MetaData), NewProp_Expected_MetaData) }; // 1151875563
void Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((VTBAttributeEditor_eventRequestDuplicate_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VTBAttributeEditor_eventRequestDuplicate_Parms), &Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::NewProp_Expected,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::PropPointers) < 2048);
// ********** End Function RequestDuplicate Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBAttributeEditor, nullptr, "RequestDuplicate", 	Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::VTBAttributeEditor_eventRequestDuplicate_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::VTBAttributeEditor_eventRequestDuplicate_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBAttributeEditor::execRequestDuplicate)
{
	P_GET_STRUCT_REF(FOWTAttributeSnapshot,Z_Param_Out_Expected);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->RequestDuplicate(Z_Param_Out_Expected);
	P_NATIVE_END;
}
// ********** End Class AVTBAttributeEditor Function RequestDuplicate ******************************

// ********** Begin Class AVTBAttributeEditor Function RequestTransformField ***********************
struct Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics
{
	struct VTBAttributeEditor_eventRequestTransformField_Parms
	{
		FOWTAttributeSnapshot Expected;
		EOWTTransformField Field;
		double Value;
		EOWTTransformEditPhase Phase;
		FGuid OperationId;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Expected_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function RequestTransformField constinit property declarations *****************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Expected;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Field_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Field;
	static const UECodeGen_Private::FDoublePropertyParams NewProp_Value;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Phase_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Phase;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OperationId;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function RequestTransformField constinit property declarations *******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function RequestTransformField Property Definitions ****************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_Expected = { "Expected", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventRequestTransformField_Parms, Expected), Z_Construct_UScriptStruct_FOWTAttributeSnapshot, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Expected_MetaData), NewProp_Expected_MetaData) }; // 1151875563
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_Field_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_Field = { "Field", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventRequestTransformField_Parms, Field), Z_Construct_UEnum_VTBOWTEditor_EOWTTransformField, METADATA_PARAMS(0, nullptr) }; // 839127366
const UECodeGen_Private::FDoublePropertyParams Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_Value = { "Value", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Double, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventRequestTransformField_Parms, Value), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_Phase_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_Phase = { "Phase", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventRequestTransformField_Parms, Phase), Z_Construct_UEnum_VTBOWTEditor_EOWTTransformEditPhase, METADATA_PARAMS(0, nullptr) }; // 1024194901
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_OperationId = { "OperationId", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventRequestTransformField_Parms, OperationId), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((VTBAttributeEditor_eventRequestTransformField_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VTBAttributeEditor_eventRequestTransformField_Parms), &Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_Expected,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_Field_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_Field,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_Value,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_Phase_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_Phase,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_OperationId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::PropPointers) < 2048);
// ********** End Function RequestTransformField Property Definitions ******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBAttributeEditor, nullptr, "RequestTransformField", 	Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::VTBAttributeEditor_eventRequestTransformField_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::VTBAttributeEditor_eventRequestTransformField_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBAttributeEditor::execRequestTransformField)
{
	P_GET_STRUCT_REF(FOWTAttributeSnapshot,Z_Param_Out_Expected);
	P_GET_ENUM(EOWTTransformField,Z_Param_Field);
	P_GET_PROPERTY(FDoubleProperty,Z_Param_Value);
	P_GET_ENUM(EOWTTransformEditPhase,Z_Param_Phase);
	P_GET_STRUCT(FGuid,Z_Param_OperationId);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->RequestTransformField(Z_Param_Out_Expected,EOWTTransformField(Z_Param_Field),Z_Param_Value,EOWTTransformEditPhase(Z_Param_Phase),Z_Param_OperationId);
	P_NATIVE_END;
}
// ********** End Class AVTBAttributeEditor Function RequestTransformField *************************

// ********** Begin Class AVTBAttributeEditor Function SaveHistory *********************************
static FName NAME_AVTBAttributeEditor_SaveHistory = FName(TEXT("SaveHistory"));
void AVTBAttributeEditor::SaveHistory()
{
	UFunction* Func = FindFunctionChecked(NAME_AVTBAttributeEditor_SaveHistory);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
	ProcessEvent(Func,NULL);
	}
	else
	{
		SaveHistory_Implementation();
	}
}
struct Z_Construct_UFunction_AVTBAttributeEditor_SaveHistory_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "History" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SaveHistory constinit property declarations ***************************
// ********** End Function SaveHistory constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBAttributeEditor_SaveHistory_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBAttributeEditor, nullptr, "SaveHistory", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_SaveHistory_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBAttributeEditor_SaveHistory_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_AVTBAttributeEditor_SaveHistory()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBAttributeEditor_SaveHistory_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBAttributeEditor::execSaveHistory)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SaveHistory_Implementation();
	P_NATIVE_END;
}
// ********** End Class AVTBAttributeEditor Function SaveHistory ***********************************

// ********** Begin Class AVTBAttributeEditor Function SaveHistoryFromTransformContext *************
struct Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext_Statics
{
	struct VTBAttributeEditor_eventSaveHistoryFromTransformContext_Parms
	{
		FSaveTransformContext Context;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "History" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SaveHistoryFromTransformContext constinit property declarations *******
	static const UECodeGen_Private::FStructPropertyParams NewProp_Context;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SaveHistoryFromTransformContext constinit property declarations *********
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SaveHistoryFromTransformContext Property Definitions ******************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventSaveHistoryFromTransformContext_Parms, Context), Z_Construct_UScriptStruct_FSaveTransformContext, METADATA_PARAMS(0, nullptr) }; // 4210459777
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext_Statics::NewProp_Context,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext_Statics::PropPointers) < 2048);
// ********** End Function SaveHistoryFromTransformContext Property Definitions ********************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBAttributeEditor, nullptr, "SaveHistoryFromTransformContext", 	Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext_Statics::VTBAttributeEditor_eventSaveHistoryFromTransformContext_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext_Statics::VTBAttributeEditor_eventSaveHistoryFromTransformContext_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBAttributeEditor::execSaveHistoryFromTransformContext)
{
	P_GET_STRUCT(FSaveTransformContext,Z_Param_Context);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SaveHistoryFromTransformContext(Z_Param_Context);
	P_NATIVE_END;
}
// ********** End Class AVTBAttributeEditor Function SaveHistoryFromTransformContext ***************

// ********** Begin Class AVTBAttributeEditor Function SubscribeDynamic ****************************
struct Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics
{
	struct VTBAttributeEditor_eventSubscribeDynamic_Parms
	{
		UObject* Subscriber;
		FScriptDelegate Callback;
		bool bSendSnapshot;
		FGuid ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Attributes" },
		{ "CPP_Default_bSendSnapshot", "true" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Callback_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function SubscribeDynamic constinit property declarations **********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Subscriber;
	static const UECodeGen_Private::FDelegatePropertyParams NewProp_Callback;
	static void NewProp_bSendSnapshot_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bSendSnapshot;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SubscribeDynamic constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SubscribeDynamic Property Definitions *********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::NewProp_Subscriber = { "Subscriber", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventSubscribeDynamic_Parms, Subscriber), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FDelegatePropertyParams Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::NewProp_Callback = { "Callback", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Delegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventSubscribeDynamic_Parms, Callback), Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Callback_MetaData), NewProp_Callback_MetaData) }; // 317585756
void Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::NewProp_bSendSnapshot_SetBit(void* Obj)
{
	((VTBAttributeEditor_eventSubscribeDynamic_Parms*)Obj)->bSendSnapshot = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::NewProp_bSendSnapshot = { "bSendSnapshot", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VTBAttributeEditor_eventSubscribeDynamic_Parms), &Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::NewProp_bSendSnapshot_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventSubscribeDynamic_Parms, ReturnValue), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::NewProp_Subscriber,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::NewProp_Callback,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::NewProp_bSendSnapshot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::PropPointers) < 2048);
// ********** End Function SubscribeDynamic Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBAttributeEditor, nullptr, "SubscribeDynamic", 	Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::VTBAttributeEditor_eventSubscribeDynamic_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::VTBAttributeEditor_eventSubscribeDynamic_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBAttributeEditor::execSubscribeDynamic)
{
	P_GET_OBJECT(UObject,Z_Param_Subscriber);
	P_GET_PROPERTY_REF(FDelegateProperty,Z_Param_Out_Callback);
	P_GET_UBOOL(Z_Param_bSendSnapshot);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FGuid*)Z_Param__Result=P_THIS->SubscribeDynamic(Z_Param_Subscriber,FOWTAttributeEventDynamic(Z_Param_Out_Callback),Z_Param_bSendSnapshot);
	P_NATIVE_END;
}
// ********** End Class AVTBAttributeEditor Function SubscribeDynamic ******************************

// ********** Begin Class AVTBAttributeEditor Function Undo ****************************************
static FName NAME_AVTBAttributeEditor_Undo = FName(TEXT("Undo"));
void AVTBAttributeEditor::Undo()
{
	UFunction* Func = FindFunctionChecked(NAME_AVTBAttributeEditor_Undo);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
	ProcessEvent(Func,NULL);
	}
	else
	{
		Undo_Implementation();
	}
}
struct Z_Construct_UFunction_AVTBAttributeEditor_Undo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "History" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function Undo constinit property declarations **********************************
// ********** End Function Undo constinit property declarations ************************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBAttributeEditor_Undo_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBAttributeEditor, nullptr, "Undo", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08080C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_Undo_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBAttributeEditor_Undo_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_AVTBAttributeEditor_Undo()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBAttributeEditor_Undo_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBAttributeEditor::execUndo)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Undo_Implementation();
	P_NATIVE_END;
}
// ********** End Class AVTBAttributeEditor Function Undo ******************************************

// ********** Begin Class AVTBAttributeEditor Function Unsubscribe *********************************
struct Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics
{
	struct VTBAttributeEditor_eventUnsubscribe_Parms
	{
		FGuid Handle;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function Unsubscribe constinit property declarations ***************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Handle;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function Unsubscribe constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function Unsubscribe Property Definitions **************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::NewProp_Handle = { "Handle", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBAttributeEditor_eventUnsubscribe_Parms, Handle), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((VTBAttributeEditor_eventUnsubscribe_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VTBAttributeEditor_eventUnsubscribe_Parms), &Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::NewProp_Handle,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::PropPointers) < 2048);
// ********** End Function Unsubscribe Property Definitions ****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBAttributeEditor, nullptr, "Unsubscribe", 	Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::VTBAttributeEditor_eventUnsubscribe_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::VTBAttributeEditor_eventUnsubscribe_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBAttributeEditor::execUnsubscribe)
{
	P_GET_STRUCT(FGuid,Z_Param_Handle);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->Unsubscribe(Z_Param_Handle);
	P_NATIVE_END;
}
// ********** End Class AVTBAttributeEditor Function Unsubscribe ***********************************

// ********** Begin Class AVTBAttributeEditor ******************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_AVTBAttributeEditor;
UClass* AVTBAttributeEditor::GetPrivateStaticClass()
{
	using TClass = AVTBAttributeEditor;
	if (!Z_Registration_Info_UClass_AVTBAttributeEditor.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBAttributeEditor"),
			Z_Registration_Info_UClass_AVTBAttributeEditor.InnerSingleton,
			StaticRegisterNativesAVTBAttributeEditor,
			sizeof(TClass),
			alignof(TClass),
			TClass::StaticClassFlags,
			TClass::StaticClassCastFlags(),
			TClass::StaticConfigName(),
			(UClass::ClassConstructorType)InternalConstructor<TClass>,
			(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<TClass>,
			UOBJECT_CPPCLASS_STATICFUNCTIONS_FORCLASS(TClass),
			&TClass::Super::StaticClass,
			&TClass::WithinClass::StaticClass
		);
	}
	return Z_Registration_Info_UClass_AVTBAttributeEditor.InnerSingleton;
}
UClass* Z_Construct_UClass_AVTBAttributeEditor_NoRegister()
{
	return AVTBAttributeEditor::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AVTBAttributeEditor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "VTBAttributeEditor.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Notifications_MetaData[] = {
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StateStore_MetaData[] = {
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Duplicator_MetaData[] = {
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DuplicateWorldOffset_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/VTBAttributeEditor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class AVTBAttributeEditor constinit property declarations **********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Notifications;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_StateStore;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Duplicator;
	static const UECodeGen_Private::FStructPropertyParams NewProp_DuplicateWorldOffset;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class AVTBAttributeEditor constinit property declarations ************************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("GetLatestEventSequence"), .Pointer = &AVTBAttributeEditor::execGetLatestEventSequence },
		{ .NameUTF8 = UTF8TEXT("GetMonitorEntries"), .Pointer = &AVTBAttributeEditor::execGetMonitorEntries },
		{ .NameUTF8 = UTF8TEXT("GetSnapshot"), .Pointer = &AVTBAttributeEditor::execGetSnapshot },
		{ .NameUTF8 = UTF8TEXT("MarkSelectionBaseline"), .Pointer = &AVTBAttributeEditor::execMarkSelectionBaseline },
		{ .NameUTF8 = UTF8TEXT("ParseSnapshotJson"), .Pointer = &AVTBAttributeEditor::execParseSnapshotJson },
		{ .NameUTF8 = UTF8TEXT("PublishRequest"), .Pointer = &AVTBAttributeEditor::execPublishRequest },
		{ .NameUTF8 = UTF8TEXT("Redo"), .Pointer = &AVTBAttributeEditor::execRedo },
		{ .NameUTF8 = UTF8TEXT("RequestDuplicate"), .Pointer = &AVTBAttributeEditor::execRequestDuplicate },
		{ .NameUTF8 = UTF8TEXT("RequestTransformField"), .Pointer = &AVTBAttributeEditor::execRequestTransformField },
		{ .NameUTF8 = UTF8TEXT("SaveHistory"), .Pointer = &AVTBAttributeEditor::execSaveHistory },
		{ .NameUTF8 = UTF8TEXT("SaveHistoryFromTransformContext"), .Pointer = &AVTBAttributeEditor::execSaveHistoryFromTransformContext },
		{ .NameUTF8 = UTF8TEXT("SubscribeDynamic"), .Pointer = &AVTBAttributeEditor::execSubscribeDynamic },
		{ .NameUTF8 = UTF8TEXT("Undo"), .Pointer = &AVTBAttributeEditor::execUndo },
		{ .NameUTF8 = UTF8TEXT("Unsubscribe"), .Pointer = &AVTBAttributeEditor::execUnsubscribe },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_AVTBAttributeEditor_GetLatestEventSequence, "GetLatestEventSequence" }, // 841479116
		{ &Z_Construct_UFunction_AVTBAttributeEditor_GetMonitorEntries, "GetMonitorEntries" }, // 1102329724
		{ &Z_Construct_UFunction_AVTBAttributeEditor_GetSnapshot, "GetSnapshot" }, // 4260423473
		{ &Z_Construct_UFunction_AVTBAttributeEditor_MarkSelectionBaseline, "MarkSelectionBaseline" }, // 861980488
		{ &Z_Construct_UFunction_AVTBAttributeEditor_ParseSnapshotJson, "ParseSnapshotJson" }, // 918682028
		{ &Z_Construct_UFunction_AVTBAttributeEditor_PublishRequest, "PublishRequest" }, // 3054175536
		{ &Z_Construct_UFunction_AVTBAttributeEditor_Redo, "Redo" }, // 3569955812
		{ &Z_Construct_UFunction_AVTBAttributeEditor_RequestDuplicate, "RequestDuplicate" }, // 3580465023
		{ &Z_Construct_UFunction_AVTBAttributeEditor_RequestTransformField, "RequestTransformField" }, // 3972053399
		{ &Z_Construct_UFunction_AVTBAttributeEditor_SaveHistory, "SaveHistory" }, // 3235824542
		{ &Z_Construct_UFunction_AVTBAttributeEditor_SaveHistoryFromTransformContext, "SaveHistoryFromTransformContext" }, // 72644076
		{ &Z_Construct_UFunction_AVTBAttributeEditor_SubscribeDynamic, "SubscribeDynamic" }, // 742933633
		{ &Z_Construct_UFunction_AVTBAttributeEditor_Undo, "Undo" }, // 4198260269
		{ &Z_Construct_UFunction_AVTBAttributeEditor_Unsubscribe, "Unsubscribe" }, // 1286936818
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AVTBAttributeEditor>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_AVTBAttributeEditor_Statics

// ********** Begin Class AVTBAttributeEditor Property Definitions *********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVTBAttributeEditor_Statics::NewProp_Notifications = { "Notifications", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBAttributeEditor, Notifications), Z_Construct_UClass_UOWTNotificationCenter_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Notifications_MetaData), NewProp_Notifications_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVTBAttributeEditor_Statics::NewProp_StateStore = { "StateStore", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBAttributeEditor, StateStore), Z_Construct_UClass_UOWTAttributeStateStore_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StateStore_MetaData), NewProp_StateStore_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVTBAttributeEditor_Statics::NewProp_Duplicator = { "Duplicator", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBAttributeEditor, Duplicator), Z_Construct_UClass_UOWTRuntimeActorDuplicator_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Duplicator_MetaData), NewProp_Duplicator_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_AVTBAttributeEditor_Statics::NewProp_DuplicateWorldOffset = { "DuplicateWorldOffset", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBAttributeEditor, DuplicateWorldOffset), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DuplicateWorldOffset_MetaData), NewProp_DuplicateWorldOffset_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AVTBAttributeEditor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBAttributeEditor_Statics::NewProp_Notifications,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBAttributeEditor_Statics::NewProp_StateStore,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBAttributeEditor_Statics::NewProp_Duplicator,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBAttributeEditor_Statics::NewProp_DuplicateWorldOffset,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBAttributeEditor_Statics::PropPointers) < 2048);
// ********** End Class AVTBAttributeEditor Property Definitions ***********************************
UObject* (*const Z_Construct_UClass_AVTBAttributeEditor_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBAttributeEditor_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AVTBAttributeEditor_Statics::ClassParams = {
	&AVTBAttributeEditor::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_AVTBAttributeEditor_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_AVTBAttributeEditor_Statics::PropPointers),
	0,
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBAttributeEditor_Statics::Class_MetaDataParams), Z_Construct_UClass_AVTBAttributeEditor_Statics::Class_MetaDataParams)
};
void AVTBAttributeEditor::StaticRegisterNativesAVTBAttributeEditor()
{
	UClass* Class = AVTBAttributeEditor::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_AVTBAttributeEditor_Statics::Funcs));
}
UClass* Z_Construct_UClass_AVTBAttributeEditor()
{
	if (!Z_Registration_Info_UClass_AVTBAttributeEditor.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AVTBAttributeEditor.OuterSingleton, Z_Construct_UClass_AVTBAttributeEditor_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AVTBAttributeEditor.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, AVTBAttributeEditor);
AVTBAttributeEditor::~AVTBAttributeEditor() {}
// ********** End Class AVTBAttributeEditor ********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AVTBAttributeEditor, AVTBAttributeEditor::StaticClass, TEXT("AVTBAttributeEditor"), &Z_Registration_Info_UClass_AVTBAttributeEditor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AVTBAttributeEditor), 704450568U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h__Script_VTBOWTEditor_886305229{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
