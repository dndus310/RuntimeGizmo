// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Events/OWTEventTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTEventTypes() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FDateTime();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
OWTEVENTCORE_API UEnum* Z_Construct_UEnum_OWTEventCore_EOWTEventDirection();
OWTEVENTCORE_API UFunction* Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature();
OWTEVENTCORE_API UScriptStruct* Z_Construct_UScriptStruct_FOWTEventRecord();
UPackage* Z_Construct_UPackage__Script_OWTEventCore();
// ********** End Cross Module References **********************************************************

// ********** Begin Delegate FOWTAttributeEventDynamic *********************************************
struct Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics
{
	struct _Script_OWTEventCore_eventOWTAttributeEventDynamic_Parms
	{
		FName Event;
		FString Json;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Events/OWTEventTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Json_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Delegate FOWTAttributeEventDynamic constinit property declarations *************
	static const UECodeGen_Private::FNamePropertyParams NewProp_Event;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Json;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Delegate FOWTAttributeEventDynamic constinit property declarations ***************
	static const UECodeGen_Private::FDelegateFunctionParams FuncParams;
};

// ********** Begin Delegate FOWTAttributeEventDynamic Property Definitions ************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics::NewProp_Event = { "Event", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_OWTEventCore_eventOWTAttributeEventDynamic_Parms, Event), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(_Script_OWTEventCore_eventOWTAttributeEventDynamic_Parms, Json), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Json_MetaData), NewProp_Json_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics::NewProp_Event,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics::NewProp_Json,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics::PropPointers) < 2048);
// ********** End Delegate FOWTAttributeEventDynamic Property Definitions **************************
const UECodeGen_Private::FDelegateFunctionParams Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UPackage__Script_OWTEventCore, nullptr, "OWTAttributeEventDynamic__DelegateSignature", 	Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics::PropPointers), 
sizeof(Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics::_Script_OWTEventCore_eventOWTAttributeEventDynamic_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00120000, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics::Function_MetaDataParams), Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics::_Script_OWTEventCore_eventOWTAttributeEventDynamic_Parms) < MAX_uint16);
UFunction* Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUDelegateFunction(&ReturnFunction, Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature_Statics::FuncParams);
	}
	return ReturnFunction;
}
void FOWTAttributeEventDynamic_DelegateWrapper(const FScriptDelegate& OWTAttributeEventDynamic, FName Event, const FString& Json)
{
	struct _Script_OWTEventCore_eventOWTAttributeEventDynamic_Parms
	{
		FName Event;
		FString Json;
	};
	_Script_OWTEventCore_eventOWTAttributeEventDynamic_Parms Parms;
	Parms.Event=Event;
	Parms.Json=Json;
	OWTAttributeEventDynamic.ProcessDelegate<UObject>(&Parms);
}
// ********** End Delegate FOWTAttributeEventDynamic ***********************************************

// ********** Begin Enum EOWTEventDirection ********************************************************
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EOWTEventDirection;
static UEnum* EOWTEventDirection_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EOWTEventDirection.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EOWTEventDirection.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_OWTEventCore_EOWTEventDirection, (UObject*)Z_Construct_UPackage__Script_OWTEventCore(), TEXT("EOWTEventDirection"));
	}
	return Z_Registration_Info_UEnum_EOWTEventDirection.OuterSingleton;
}
template<> OWTEVENTCORE_NON_ATTRIBUTED_API UEnum* StaticEnum<EOWTEventDirection>()
{
	return EOWTEventDirection_StaticEnum();
}
struct Z_Construct_UEnum_OWTEventCore_EOWTEventDirection_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Inbound.Name", "EOWTEventDirection::Inbound" },
		{ "ModuleRelativePath", "Public/Events/OWTEventTypes.h" },
		{ "Outbound.Name", "EOWTEventDirection::Outbound" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EOWTEventDirection::Inbound", (int64)EOWTEventDirection::Inbound },
		{ "EOWTEventDirection::Outbound", (int64)EOWTEventDirection::Outbound },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
}; // struct Z_Construct_UEnum_OWTEventCore_EOWTEventDirection_Statics 
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_OWTEventCore_EOWTEventDirection_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_OWTEventCore,
	nullptr,
	"EOWTEventDirection",
	"EOWTEventDirection",
	Z_Construct_UEnum_OWTEventCore_EOWTEventDirection_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_OWTEventCore_EOWTEventDirection_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_OWTEventCore_EOWTEventDirection_Statics::Enum_MetaDataParams), Z_Construct_UEnum_OWTEventCore_EOWTEventDirection_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_OWTEventCore_EOWTEventDirection()
{
	if (!Z_Registration_Info_UEnum_EOWTEventDirection.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EOWTEventDirection.InnerSingleton, Z_Construct_UEnum_OWTEventCore_EOWTEventDirection_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EOWTEventDirection.InnerSingleton;
}
// ********** End Enum EOWTEventDirection **********************************************************

// ********** Begin ScriptStruct FOWTEventRecord ***************************************************
struct Z_Construct_UScriptStruct_FOWTEventRecord_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTEventRecord); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTEventRecord); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** A diagnostic copy of an event. The original payload is never retained beyond the journal limit. */" },
#endif
		{ "ModuleRelativePath", "Public/Events/OWTEventTypes.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "A diagnostic copy of an event. The original payload is never retained beyond the journal limit." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TimestampUtc_MetaData[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTEventTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Event_MetaData[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTEventTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Json_MetaData[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTEventTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Recipient_MetaData[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTEventTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Direction_MetaData[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTEventTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Sequence_MetaData[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTEventTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bValidJson_MetaData[] = {
		{ "Category", "OWT|Events" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Whether the original payload was valid JSON, before journal truncation. */" },
#endif
		{ "ModuleRelativePath", "Public/Events/OWTEventTypes.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Whether the original payload was valid JSON, before journal truncation." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bPayloadTruncated_MetaData[] = {
		{ "Category", "OWT|Events" },
		{ "ModuleRelativePath", "Public/Events/OWTEventTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTEventRecord constinit property declarations *******************
	static const UECodeGen_Private::FStructPropertyParams NewProp_TimestampUtc;
	static const UECodeGen_Private::FNamePropertyParams NewProp_Event;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Json;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Recipient;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Direction_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Direction;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_Sequence;
	static void NewProp_bValidJson_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bValidJson;
	static void NewProp_bPayloadTruncated_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bPayloadTruncated;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTEventRecord constinit property declarations *********************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTEventRecord>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTEventRecord_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTEventRecord;
class UScriptStruct* FOWTEventRecord::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTEventRecord.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTEventRecord.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTEventRecord, (UObject*)Z_Construct_UPackage__Script_OWTEventCore(), TEXT("OWTEventRecord"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTEventRecord.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTEventRecord Property Definitions ******************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_TimestampUtc = { "TimestampUtc", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTEventRecord, TimestampUtc), Z_Construct_UScriptStruct_FDateTime, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TimestampUtc_MetaData), NewProp_TimestampUtc_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_Event = { "Event", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTEventRecord, Event), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Event_MetaData), NewProp_Event_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTEventRecord, Json), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Json_MetaData), NewProp_Json_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_Recipient = { "Recipient", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTEventRecord, Recipient), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Recipient_MetaData), NewProp_Recipient_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_Direction_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_Direction = { "Direction", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTEventRecord, Direction), Z_Construct_UEnum_OWTEventCore_EOWTEventDirection, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Direction_MetaData), NewProp_Direction_MetaData) }; // 2648378897
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_Sequence = { "Sequence", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTEventRecord, Sequence), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Sequence_MetaData), NewProp_Sequence_MetaData) };
void Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_bValidJson_SetBit(void* Obj)
{
	((FOWTEventRecord*)Obj)->bValidJson = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_bValidJson = { "bValidJson", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTEventRecord), &Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_bValidJson_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bValidJson_MetaData), NewProp_bValidJson_MetaData) };
void Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_bPayloadTruncated_SetBit(void* Obj)
{
	((FOWTEventRecord*)Obj)->bPayloadTruncated = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_bPayloadTruncated = { "bPayloadTruncated", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTEventRecord), &Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_bPayloadTruncated_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bPayloadTruncated_MetaData), NewProp_bPayloadTruncated_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTEventRecord_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_TimestampUtc,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_Event,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_Json,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_Recipient,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_Direction_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_Direction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_Sequence,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_bValidJson,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewProp_bPayloadTruncated,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTEventRecord_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTEventRecord Property Definitions ********************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTEventRecord_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_OWTEventCore,
	nullptr,
	&NewStructOps,
	"OWTEventRecord",
	Z_Construct_UScriptStruct_FOWTEventRecord_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTEventRecord_Statics::PropPointers),
	sizeof(FOWTEventRecord),
	alignof(FOWTEventRecord),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTEventRecord_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTEventRecord_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTEventRecord()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTEventRecord.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTEventRecord.InnerSingleton, Z_Construct_UScriptStruct_FOWTEventRecord_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTEventRecord.InnerSingleton);
}
// ********** End ScriptStruct FOWTEventRecord *****************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTEventTypes_h__Script_OWTEventCore_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EOWTEventDirection_StaticEnum, TEXT("EOWTEventDirection"), &Z_Registration_Info_UEnum_EOWTEventDirection, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2648378897U) },
	};
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FOWTEventRecord::StaticStruct, Z_Construct_UScriptStruct_FOWTEventRecord_Statics::NewStructOps, TEXT("OWTEventRecord"),&Z_Registration_Info_UScriptStruct_FOWTEventRecord, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTEventRecord), 2725191315U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTEventTypes_h__Script_OWTEventCore_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTEventTypes_h__Script_OWTEventCore_2861364472{
	TEXT("/Script/OWTEventCore"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTEventTypes_h__Script_OWTEventCore_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTEventTypes_h__Script_OWTEventCore_Statics::ScriptStructInfo),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTEventTypes_h__Script_OWTEventCore_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTEventTypes_h__Script_OWTEventCore_Statics::EnumInfo),
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
