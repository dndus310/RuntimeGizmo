// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Events/OWTNotificationCenter.h"
#include "Events/OWTEventTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTNotificationCenter() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
OWTEVENTCORE_API UClass* Z_Construct_UClass_UOWTNotificationCenter();
OWTEVENTCORE_API UClass* Z_Construct_UClass_UOWTNotificationCenter_NoRegister();
OWTEVENTCORE_API UEnum* Z_Construct_UEnum_OWTEventCore_EOWTEventDirection();
OWTEVENTCORE_API UFunction* Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature();
OWTEVENTCORE_API UScriptStruct* Z_Construct_UScriptStruct_FOWTEventRecord();
UPackage* Z_Construct_UPackage__Script_OWTEventCore();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UOWTNotificationCenter Function ClearEventHistory ************************
struct Z_Construct_UFunction_UOWTNotificationCenter_ClearEventHistory_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function ClearEventHistory constinit property declarations *********************
// ********** End Function ClearEventHistory constinit property declarations ***********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTNotificationCenter_ClearEventHistory_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTNotificationCenter, nullptr, "ClearEventHistory", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_ClearEventHistory_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTNotificationCenter_ClearEventHistory_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UOWTNotificationCenter_ClearEventHistory()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTNotificationCenter_ClearEventHistory_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTNotificationCenter::execClearEventHistory)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ClearEventHistory();
	P_NATIVE_END;
}
// ********** End Class UOWTNotificationCenter Function ClearEventHistory **************************

// ********** Begin Class UOWTNotificationCenter Function GetHistoryCapacity ***********************
struct Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity_Statics
{
	struct OWTNotificationCenter_eventGetHistoryCapacity_Parms
	{
		int32 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetHistoryCapacity constinit property declarations ********************
	static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetHistoryCapacity constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetHistoryCapacity Property Definitions *******************************
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventGetHistoryCapacity_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity_Statics::PropPointers) < 2048);
// ********** End Function GetHistoryCapacity Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTNotificationCenter, nullptr, "GetHistoryCapacity", 	Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity_Statics::OWTNotificationCenter_eventGetHistoryCapacity_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity_Statics::OWTNotificationCenter_eventGetHistoryCapacity_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTNotificationCenter::execGetHistoryCapacity)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int32*)Z_Param__Result=P_THIS->GetHistoryCapacity();
	P_NATIVE_END;
}
// ********** End Class UOWTNotificationCenter Function GetHistoryCapacity *************************

// ********** Begin Class UOWTNotificationCenter Function GetHistoryRevision ***********************
struct Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision_Statics
{
	struct OWTNotificationCenter_eventGetHistoryRevision_Parms
	{
		int64 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Events" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Changes on append, clear and capacity changes, including when no events are retained. */" },
#endif
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Changes on append, clear and capacity changes, including when no events are retained." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function GetHistoryRevision constinit property declarations ********************
	static const UECodeGen_Private::FInt64PropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetHistoryRevision constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetHistoryRevision Property Definitions *******************************
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventGetHistoryRevision_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision_Statics::PropPointers) < 2048);
// ********** End Function GetHistoryRevision Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTNotificationCenter, nullptr, "GetHistoryRevision", 	Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision_Statics::OWTNotificationCenter_eventGetHistoryRevision_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision_Statics::OWTNotificationCenter_eventGetHistoryRevision_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTNotificationCenter::execGetHistoryRevision)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int64*)Z_Param__Result=P_THIS->GetHistoryRevision();
	P_NATIVE_END;
}
// ********** End Class UOWTNotificationCenter Function GetHistoryRevision *************************

// ********** Begin Class UOWTNotificationCenter Function GetLatestSequence ************************
struct Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence_Statics
{
	struct OWTNotificationCenter_eventGetLatestSequence_Parms
	{
		int64 ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetLatestSequence constinit property declarations *********************
	static const UECodeGen_Private::FInt64PropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetLatestSequence constinit property declarations ***********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetLatestSequence Property Definitions ********************************
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventGetLatestSequence_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence_Statics::PropPointers) < 2048);
// ********** End Function GetLatestSequence Property Definitions **********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTNotificationCenter, nullptr, "GetLatestSequence", 	Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence_Statics::OWTNotificationCenter_eventGetLatestSequence_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence_Statics::OWTNotificationCenter_eventGetLatestSequence_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTNotificationCenter::execGetLatestSequence)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(int64*)Z_Param__Result=P_THIS->GetLatestSequence();
	P_NATIVE_END;
}
// ********** End Class UOWTNotificationCenter Function GetLatestSequence **************************

// ********** Begin Class UOWTNotificationCenter Function GetRecentEvents **************************
struct Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics
{
	struct OWTNotificationCenter_eventGetRecentEvents_Parms
	{
		int32 MaximumCount;
		TArray<FOWTEventRecord> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Events" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Latest MaximumCount records, returned oldest to newest. */" },
#endif
		{ "CPP_Default_MaximumCount", "256" },
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Latest MaximumCount records, returned oldest to newest." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function GetRecentEvents constinit property declarations ***********************
	static const UECodeGen_Private::FIntPropertyParams NewProp_MaximumCount;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetRecentEvents constinit property declarations *************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetRecentEvents Property Definitions **********************************
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::NewProp_MaximumCount = { "MaximumCount", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventGetRecentEvents_Parms, MaximumCount), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FOWTEventRecord, METADATA_PARAMS(0, nullptr) }; // 2725191315
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventGetRecentEvents_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 2725191315
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::NewProp_MaximumCount,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::NewProp_ReturnValue_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::PropPointers) < 2048);
// ********** End Function GetRecentEvents Property Definitions ************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTNotificationCenter, nullptr, "GetRecentEvents", 	Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::OWTNotificationCenter_eventGetRecentEvents_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::OWTNotificationCenter_eventGetRecentEvents_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTNotificationCenter::execGetRecentEvents)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_MaximumCount);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<FOWTEventRecord>*)Z_Param__Result=P_THIS->GetRecentEvents(Z_Param_MaximumCount);
	P_NATIVE_END;
}
// ********** End Class UOWTNotificationCenter Function GetRecentEvents ****************************

// ********** Begin Class UOWTNotificationCenter Function Initialize *******************************
struct Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics
{
	struct OWTNotificationCenter_eventInitialize_Parms
	{
		UObject* Owner;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Events" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** The center must have been created with Owner as its direct Outer. */" },
#endif
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The center must have been created with Owner as its direct Outer." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function Initialize constinit property declarations ****************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Owner;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function Initialize constinit property declarations ******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function Initialize Property Definitions ***************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::NewProp_Owner = { "Owner", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventInitialize_Parms, Owner), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((OWTNotificationCenter_eventInitialize_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(OWTNotificationCenter_eventInitialize_Parms), &Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::NewProp_Owner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::PropPointers) < 2048);
// ********** End Function Initialize Property Definitions *****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTNotificationCenter, nullptr, "Initialize", 	Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::OWTNotificationCenter_eventInitialize_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::OWTNotificationCenter_eventInitialize_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTNotificationCenter_Initialize()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTNotificationCenter_Initialize_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTNotificationCenter::execInitialize)
{
	P_GET_OBJECT(UObject,Z_Param_Owner);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->Initialize(Z_Param_Owner);
	P_NATIVE_END;
}
// ********** End Class UOWTNotificationCenter Function Initialize *********************************

// ********** Begin Class UOWTNotificationCenter Function IsReady **********************************
struct Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics
{
	struct OWTNotificationCenter_eventIsReady_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function IsReady constinit property declarations *******************************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function IsReady constinit property declarations *********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function IsReady Property Definitions ******************************************
void Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((OWTNotificationCenter_eventIsReady_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(OWTNotificationCenter_eventIsReady_Parms), &Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics::PropPointers) < 2048);
// ********** End Function IsReady Property Definitions ********************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTNotificationCenter, nullptr, "IsReady", 	Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics::OWTNotificationCenter_eventIsReady_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics::OWTNotificationCenter_eventIsReady_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTNotificationCenter_IsReady()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTNotificationCenter_IsReady_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTNotificationCenter::execIsReady)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsReady();
	P_NATIVE_END;
}
// ********** End Class UOWTNotificationCenter Function IsReady ************************************

// ********** Begin Class UOWTNotificationCenter Function Publish **********************************
struct Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics
{
	struct OWTNotificationCenter_eventPublish_Parms
	{
		FName Event;
		FString Json;
		FGuid Recipient;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Events" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Valid JSON only; a non-empty Recipient targets one subscription. Maximum payload: 65536 characters. */" },
#endif
		{ "CPP_Default_Recipient", "()" },
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Valid JSON only; a non-empty Recipient targets one subscription. Maximum payload: 65536 characters." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Json_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function Publish constinit property declarations *******************************
	static const UECodeGen_Private::FNamePropertyParams NewProp_Event;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Json;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Recipient;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function Publish constinit property declarations *********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function Publish Property Definitions ******************************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::NewProp_Event = { "Event", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventPublish_Parms, Event), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventPublish_Parms, Json), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Json_MetaData), NewProp_Json_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::NewProp_Recipient = { "Recipient", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventPublish_Parms, Recipient), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((OWTNotificationCenter_eventPublish_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(OWTNotificationCenter_eventPublish_Parms), &Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::NewProp_Event,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::NewProp_Json,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::NewProp_Recipient,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::PropPointers) < 2048);
// ********** End Function Publish Property Definitions ********************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTNotificationCenter, nullptr, "Publish", 	Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::OWTNotificationCenter_eventPublish_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::OWTNotificationCenter_eventPublish_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTNotificationCenter_Publish()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTNotificationCenter_Publish_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTNotificationCenter::execPublish)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_Event);
	P_GET_PROPERTY(FStrProperty,Z_Param_Json);
	P_GET_STRUCT(FGuid,Z_Param_Recipient);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->Publish(Z_Param_Event,Z_Param_Json,Z_Param_Recipient);
	P_NATIVE_END;
}
// ********** End Class UOWTNotificationCenter Function Publish ************************************

// ********** Begin Class UOWTNotificationCenter Function RecordEvent ******************************
struct Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics
{
	struct OWTNotificationCenter_eventRecordEvent_Parms
	{
		FName Event;
		FString Json;
		EOWTEventDirection Direction;
		FGuid Recipient;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Events" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Journal without delivering. Malformed and truncated input remain visible for request diagnostics. */" },
#endif
		{ "CPP_Default_Direction", "Inbound" },
		{ "CPP_Default_Recipient", "()" },
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Journal without delivering. Malformed and truncated input remain visible for request diagnostics." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Json_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function RecordEvent constinit property declarations ***************************
	static const UECodeGen_Private::FNamePropertyParams NewProp_Event;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Json;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Direction_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Direction;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Recipient;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function RecordEvent constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function RecordEvent Property Definitions **************************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::NewProp_Event = { "Event", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventRecordEvent_Parms, Event), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventRecordEvent_Parms, Json), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Json_MetaData), NewProp_Json_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::NewProp_Direction_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::NewProp_Direction = { "Direction", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventRecordEvent_Parms, Direction), Z_Construct_UEnum_OWTEventCore_EOWTEventDirection, METADATA_PARAMS(0, nullptr) }; // 2648378897
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::NewProp_Recipient = { "Recipient", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventRecordEvent_Parms, Recipient), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((OWTNotificationCenter_eventRecordEvent_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(OWTNotificationCenter_eventRecordEvent_Parms), &Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::NewProp_Event,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::NewProp_Json,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::NewProp_Direction_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::NewProp_Direction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::NewProp_Recipient,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::PropPointers) < 2048);
// ********** End Function RecordEvent Property Definitions ****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTNotificationCenter, nullptr, "RecordEvent", 	Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::OWTNotificationCenter_eventRecordEvent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::OWTNotificationCenter_eventRecordEvent_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTNotificationCenter::execRecordEvent)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_Event);
	P_GET_PROPERTY(FStrProperty,Z_Param_Json);
	P_GET_ENUM(EOWTEventDirection,Z_Param_Direction);
	P_GET_STRUCT(FGuid,Z_Param_Recipient);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->RecordEvent(Z_Param_Event,Z_Param_Json,EOWTEventDirection(Z_Param_Direction),Z_Param_Recipient);
	P_NATIVE_END;
}
// ********** End Class UOWTNotificationCenter Function RecordEvent ********************************

// ********** Begin Class UOWTNotificationCenter Function SetHistoryCapacity ***********************
struct Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity_Statics
{
	struct OWTNotificationCenter_eventSetHistoryCapacity_Parms
	{
		int32 Capacity;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetHistoryCapacity constinit property declarations ********************
	static const UECodeGen_Private::FIntPropertyParams NewProp_Capacity;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetHistoryCapacity constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetHistoryCapacity Property Definitions *******************************
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity_Statics::NewProp_Capacity = { "Capacity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventSetHistoryCapacity_Parms, Capacity), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity_Statics::NewProp_Capacity,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity_Statics::PropPointers) < 2048);
// ********** End Function SetHistoryCapacity Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTNotificationCenter, nullptr, "SetHistoryCapacity", 	Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity_Statics::OWTNotificationCenter_eventSetHistoryCapacity_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity_Statics::OWTNotificationCenter_eventSetHistoryCapacity_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTNotificationCenter::execSetHistoryCapacity)
{
	P_GET_PROPERTY(FIntProperty,Z_Param_Capacity);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetHistoryCapacity(Z_Param_Capacity);
	P_NATIVE_END;
}
// ********** End Class UOWTNotificationCenter Function SetHistoryCapacity *************************

// ********** Begin Class UOWTNotificationCenter Function Shutdown *********************************
struct Z_Construct_UFunction_UOWTNotificationCenter_Shutdown_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function Shutdown constinit property declarations ******************************
// ********** End Function Shutdown constinit property declarations ********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTNotificationCenter_Shutdown_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTNotificationCenter, nullptr, "Shutdown", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_Shutdown_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTNotificationCenter_Shutdown_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UOWTNotificationCenter_Shutdown()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTNotificationCenter_Shutdown_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTNotificationCenter::execShutdown)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Shutdown();
	P_NATIVE_END;
}
// ********** End Class UOWTNotificationCenter Function Shutdown ***********************************

// ********** Begin Class UOWTNotificationCenter Function SubscribeDynamic *************************
struct Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics
{
	struct OWTNotificationCenter_eventSubscribeDynamic_Parms
	{
		UObject* Receiver;
		FScriptDelegate Callback;
		FGuid ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Callback_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function SubscribeDynamic constinit property declarations **********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Receiver;
	static const UECodeGen_Private::FDelegatePropertyParams NewProp_Callback;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SubscribeDynamic constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SubscribeDynamic Property Definitions *********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::NewProp_Receiver = { "Receiver", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventSubscribeDynamic_Parms, Receiver), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FDelegatePropertyParams Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::NewProp_Callback = { "Callback", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Delegate, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventSubscribeDynamic_Parms, Callback), Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Callback_MetaData), NewProp_Callback_MetaData) }; // 317585756
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventSubscribeDynamic_Parms, ReturnValue), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::NewProp_Receiver,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::NewProp_Callback,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::PropPointers) < 2048);
// ********** End Function SubscribeDynamic Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTNotificationCenter, nullptr, "SubscribeDynamic", 	Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::OWTNotificationCenter_eventSubscribeDynamic_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::OWTNotificationCenter_eventSubscribeDynamic_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTNotificationCenter::execSubscribeDynamic)
{
	P_GET_OBJECT(UObject,Z_Param_Receiver);
	P_GET_PROPERTY_REF(FDelegateProperty,Z_Param_Out_Callback);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FGuid*)Z_Param__Result=P_THIS->SubscribeDynamic(Z_Param_Receiver,FOWTAttributeEventDynamic(Z_Param_Out_Callback));
	P_NATIVE_END;
}
// ********** End Class UOWTNotificationCenter Function SubscribeDynamic ***************************

// ********** Begin Class UOWTNotificationCenter Function Unsubscribe ******************************
struct Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics
{
	struct OWTNotificationCenter_eventUnsubscribe_Parms
	{
		FGuid Handle;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
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
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::NewProp_Handle = { "Handle", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTNotificationCenter_eventUnsubscribe_Parms, Handle), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((OWTNotificationCenter_eventUnsubscribe_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(OWTNotificationCenter_eventUnsubscribe_Parms), &Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::NewProp_Handle,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::PropPointers) < 2048);
// ********** End Function Unsubscribe Property Definitions ****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTNotificationCenter, nullptr, "Unsubscribe", 	Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::OWTNotificationCenter_eventUnsubscribe_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::OWTNotificationCenter_eventUnsubscribe_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTNotificationCenter::execUnsubscribe)
{
	P_GET_STRUCT(FGuid,Z_Param_Handle);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->Unsubscribe(Z_Param_Handle);
	P_NATIVE_END;
}
// ********** End Class UOWTNotificationCenter Function Unsubscribe ********************************

// ********** Begin Class UOWTNotificationCenter ***************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTNotificationCenter;
UClass* UOWTNotificationCenter::GetPrivateStaticClass()
{
	using TClass = UOWTNotificationCenter;
	if (!Z_Registration_Info_UClass_UOWTNotificationCenter.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTNotificationCenter"),
			Z_Registration_Info_UClass_UOWTNotificationCenter.InnerSingleton,
			StaticRegisterNativesUOWTNotificationCenter,
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
	return Z_Registration_Info_UClass_UOWTNotificationCenter.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTNotificationCenter_NoRegister()
{
	return UOWTNotificationCenter::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTNotificationCenter_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * Owner-scoped, game-thread JSON transport. Receivers and the owner are held weakly.\n * The owner calls Shutdown when its session ends (for Actors, from EndPlay).\n */" },
#endif
		{ "IncludePath", "Events/OWTNotificationCenter.h" },
		{ "IsBlueprintBase", "false" },
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Owner-scoped, game-thread JSON transport. Receivers and the owner are held weakly.\nThe owner calls Shutdown when its session ends (for Actors, from EndPlay)." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OwnerObject_MetaData[] = {
		{ "ModuleRelativePath", "Public/Events/OWTNotificationCenter.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTNotificationCenter constinit property declarations *******************
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_OwnerObject;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UOWTNotificationCenter constinit property declarations *********************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("ClearEventHistory"), .Pointer = &UOWTNotificationCenter::execClearEventHistory },
		{ .NameUTF8 = UTF8TEXT("GetHistoryCapacity"), .Pointer = &UOWTNotificationCenter::execGetHistoryCapacity },
		{ .NameUTF8 = UTF8TEXT("GetHistoryRevision"), .Pointer = &UOWTNotificationCenter::execGetHistoryRevision },
		{ .NameUTF8 = UTF8TEXT("GetLatestSequence"), .Pointer = &UOWTNotificationCenter::execGetLatestSequence },
		{ .NameUTF8 = UTF8TEXT("GetRecentEvents"), .Pointer = &UOWTNotificationCenter::execGetRecentEvents },
		{ .NameUTF8 = UTF8TEXT("Initialize"), .Pointer = &UOWTNotificationCenter::execInitialize },
		{ .NameUTF8 = UTF8TEXT("IsReady"), .Pointer = &UOWTNotificationCenter::execIsReady },
		{ .NameUTF8 = UTF8TEXT("Publish"), .Pointer = &UOWTNotificationCenter::execPublish },
		{ .NameUTF8 = UTF8TEXT("RecordEvent"), .Pointer = &UOWTNotificationCenter::execRecordEvent },
		{ .NameUTF8 = UTF8TEXT("SetHistoryCapacity"), .Pointer = &UOWTNotificationCenter::execSetHistoryCapacity },
		{ .NameUTF8 = UTF8TEXT("Shutdown"), .Pointer = &UOWTNotificationCenter::execShutdown },
		{ .NameUTF8 = UTF8TEXT("SubscribeDynamic"), .Pointer = &UOWTNotificationCenter::execSubscribeDynamic },
		{ .NameUTF8 = UTF8TEXT("Unsubscribe"), .Pointer = &UOWTNotificationCenter::execUnsubscribe },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UOWTNotificationCenter_ClearEventHistory, "ClearEventHistory" }, // 1908506639
		{ &Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryCapacity, "GetHistoryCapacity" }, // 3626779860
		{ &Z_Construct_UFunction_UOWTNotificationCenter_GetHistoryRevision, "GetHistoryRevision" }, // 1061107611
		{ &Z_Construct_UFunction_UOWTNotificationCenter_GetLatestSequence, "GetLatestSequence" }, // 418756345
		{ &Z_Construct_UFunction_UOWTNotificationCenter_GetRecentEvents, "GetRecentEvents" }, // 622390788
		{ &Z_Construct_UFunction_UOWTNotificationCenter_Initialize, "Initialize" }, // 37410890
		{ &Z_Construct_UFunction_UOWTNotificationCenter_IsReady, "IsReady" }, // 2169195697
		{ &Z_Construct_UFunction_UOWTNotificationCenter_Publish, "Publish" }, // 3583632992
		{ &Z_Construct_UFunction_UOWTNotificationCenter_RecordEvent, "RecordEvent" }, // 1932414184
		{ &Z_Construct_UFunction_UOWTNotificationCenter_SetHistoryCapacity, "SetHistoryCapacity" }, // 1664300194
		{ &Z_Construct_UFunction_UOWTNotificationCenter_Shutdown, "Shutdown" }, // 3015811407
		{ &Z_Construct_UFunction_UOWTNotificationCenter_SubscribeDynamic, "SubscribeDynamic" }, // 1401575259
		{ &Z_Construct_UFunction_UOWTNotificationCenter_Unsubscribe, "Unsubscribe" }, // 3302005889
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTNotificationCenter>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTNotificationCenter_Statics

// ********** Begin Class UOWTNotificationCenter Property Definitions ******************************
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UOWTNotificationCenter_Statics::NewProp_OwnerObject = { "OwnerObject", nullptr, (EPropertyFlags)0x0044000000002000, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTNotificationCenter, OwnerObject), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OwnerObject_MetaData), NewProp_OwnerObject_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOWTNotificationCenter_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTNotificationCenter_Statics::NewProp_OwnerObject,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTNotificationCenter_Statics::PropPointers) < 2048);
// ********** End Class UOWTNotificationCenter Property Definitions ********************************
UObject* (*const Z_Construct_UClass_UOWTNotificationCenter_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_OWTEventCore,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTNotificationCenter_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTNotificationCenter_Statics::ClassParams = {
	&UOWTNotificationCenter::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UOWTNotificationCenter_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UOWTNotificationCenter_Statics::PropPointers),
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTNotificationCenter_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTNotificationCenter_Statics::Class_MetaDataParams)
};
void UOWTNotificationCenter::StaticRegisterNativesUOWTNotificationCenter()
{
	UClass* Class = UOWTNotificationCenter::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UOWTNotificationCenter_Statics::Funcs));
}
UClass* Z_Construct_UClass_UOWTNotificationCenter()
{
	if (!Z_Registration_Info_UClass_UOWTNotificationCenter.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTNotificationCenter.OuterSingleton, Z_Construct_UClass_UOWTNotificationCenter_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTNotificationCenter.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTNotificationCenter);
UOWTNotificationCenter::~UOWTNotificationCenter() {}
// ********** End Class UOWTNotificationCenter *****************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTNotificationCenter_h__Script_OWTEventCore_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTNotificationCenter, UOWTNotificationCenter::StaticClass, TEXT("UOWTNotificationCenter"), &Z_Registration_Info_UClass_UOWTNotificationCenter, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTNotificationCenter), 2978193723U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTNotificationCenter_h__Script_OWTEventCore_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTNotificationCenter_h__Script_OWTEventCore_2482424599{
	TEXT("/Script/OWTEventCore"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTNotificationCenter_h__Script_OWTEventCore_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTNotificationCenter_h__Script_OWTEventCore_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
