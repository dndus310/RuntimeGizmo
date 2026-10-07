// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Tests/OWTStateMonitorTestTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTStateMonitorTestTypes() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FDateTime();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
OWTSTATEMONITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTStateMonitorTestNested();
OWTSTATEMONITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTStateMonitorTestState();
UPackage* Z_Construct_UPackage__Script_OWTStateMonitor();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FOWTStateMonitorTestNested ****************************************
struct Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTStateMonitorTestNested); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTStateMonitorTestNested); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Position_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Text_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Timestamp_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTStateMonitorTestNested constinit property declarations ********
	static const UECodeGen_Private::FStructPropertyParams NewProp_Position;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Text;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Timestamp;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTStateMonitorTestNested constinit property declarations **********
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTStateMonitorTestNested>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestNested;
class UScriptStruct* FOWTStateMonitorTestNested::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestNested.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestNested.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTStateMonitorTestNested, (UObject*)Z_Construct_UPackage__Script_OWTStateMonitor(), TEXT("OWTStateMonitorTestNested"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestNested.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTStateMonitorTestNested Property Definitions *******************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::NewProp_Position = { "Position", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTStateMonitorTestNested, Position), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Position_MetaData), NewProp_Position_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::NewProp_Text = { "Text", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTStateMonitorTestNested, Text), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Text_MetaData), NewProp_Text_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::NewProp_Timestamp = { "Timestamp", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTStateMonitorTestNested, Timestamp), Z_Construct_UScriptStruct_FDateTime, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Timestamp_MetaData), NewProp_Timestamp_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::NewProp_Position,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::NewProp_Text,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::NewProp_Timestamp,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTStateMonitorTestNested Property Definitions *********************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_OWTStateMonitor,
	nullptr,
	&NewStructOps,
	"OWTStateMonitorTestNested",
	Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::PropPointers),
	sizeof(FOWTStateMonitorTestNested),
	alignof(FOWTStateMonitorTestNested),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTStateMonitorTestNested()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestNested.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestNested.InnerSingleton, Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestNested.InnerSingleton);
}
// ********** End ScriptStruct FOWTStateMonitorTestNested ******************************************

// ********** Begin ScriptStruct FOWTStateMonitorTestState *****************************************
struct Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTStateMonitorTestState); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTStateMonitorTestState); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Nested_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Items_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Counters_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Tags_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Object_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SoftObject_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Signed_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Unsigned_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnabled_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SpecialNumber_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StaticItems_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTStateMonitorTestTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTStateMonitorTestState constinit property declarations *********
	static const UECodeGen_Private::FStructPropertyParams NewProp_Nested;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Items_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Items;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Counters_ValueProp;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Counters_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_Counters;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Tags_ElementProp;
	static const UECodeGen_Private::FSetPropertyParams NewProp_Tags;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Object;
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_SoftObject;
	static const UECodeGen_Private::FInt64PropertyParams NewProp_Signed;
	static const UECodeGen_Private::FUInt64PropertyParams NewProp_Unsigned;
	static void NewProp_bEnabled_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnabled;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_SpecialNumber;
	static const UECodeGen_Private::FIntPropertyParams NewProp_StaticItems;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTStateMonitorTestState constinit property declarations ***********
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTStateMonitorTestState>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestState;
class UScriptStruct* FOWTStateMonitorTestState::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestState.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestState.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTStateMonitorTestState, (UObject*)Z_Construct_UPackage__Script_OWTStateMonitor(), TEXT("OWTStateMonitorTestState"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestState.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTStateMonitorTestState Property Definitions ********************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Nested = { "Nested", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTStateMonitorTestState, Nested), Z_Construct_UScriptStruct_FOWTStateMonitorTestNested, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Nested_MetaData), NewProp_Nested_MetaData) }; // 2339773889
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Items_Inner = { "Items", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Items = { "Items", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTStateMonitorTestState, Items), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Items_MetaData), NewProp_Items_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Counters_ValueProp = { "Counters", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Counters_Key_KeyProp = { "Counters_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Counters = { "Counters", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTStateMonitorTestState, Counters), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Counters_MetaData), NewProp_Counters_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Tags_ElementProp = { "Tags", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FSetPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Tags = { "Tags", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Set, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTStateMonitorTestState, Tags), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Tags_MetaData), NewProp_Tags_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Object = { "Object", nullptr, (EPropertyFlags)0x0114000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTStateMonitorTestState, Object), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Object_MetaData), NewProp_Object_MetaData) };
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_SoftObject = { "SoftObject", nullptr, (EPropertyFlags)0x0014000000000000, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTStateMonitorTestState, SoftObject), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SoftObject_MetaData), NewProp_SoftObject_MetaData) };
const UECodeGen_Private::FInt64PropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Signed = { "Signed", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Int64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTStateMonitorTestState, Signed), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Signed_MetaData), NewProp_Signed_MetaData) };
const UECodeGen_Private::FUInt64PropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Unsigned = { "Unsigned", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::UInt64, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTStateMonitorTestState, Unsigned), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Unsigned_MetaData), NewProp_Unsigned_MetaData) };
void Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_bEnabled_SetBit(void* Obj)
{
	((FOWTStateMonitorTestState*)Obj)->bEnabled = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_bEnabled = { "bEnabled", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTStateMonitorTestState), &Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_bEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnabled_MetaData), NewProp_bEnabled_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_SpecialNumber = { "SpecialNumber", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTStateMonitorTestState, SpecialNumber), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SpecialNumber_MetaData), NewProp_SpecialNumber_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_StaticItems = { "StaticItems", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, CPP_ARRAY_DIM(StaticItems, FOWTStateMonitorTestState), STRUCT_OFFSET(FOWTStateMonitorTestState, StaticItems), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StaticItems_MetaData), NewProp_StaticItems_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Nested,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Items_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Items,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Counters_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Counters_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Counters,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Tags_ElementProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Tags,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Object,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_SoftObject,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Signed,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_Unsigned,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_bEnabled,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_SpecialNumber,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewProp_StaticItems,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTStateMonitorTestState Property Definitions **********************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_OWTStateMonitor,
	nullptr,
	&NewStructOps,
	"OWTStateMonitorTestState",
	Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::PropPointers),
	sizeof(FOWTStateMonitorTestState),
	alignof(FOWTStateMonitorTestState),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTStateMonitorTestState()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestState.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestState.InnerSingleton, Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestState.InnerSingleton);
}
// ********** End ScriptStruct FOWTStateMonitorTestState *******************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTStateMonitor_Source_OWTStateMonitor_Private_Tests_OWTStateMonitorTestTypes_h__Script_OWTStateMonitor_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FOWTStateMonitorTestNested::StaticStruct, Z_Construct_UScriptStruct_FOWTStateMonitorTestNested_Statics::NewStructOps, TEXT("OWTStateMonitorTestNested"),&Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestNested, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTStateMonitorTestNested), 2339773889U) },
		{ FOWTStateMonitorTestState::StaticStruct, Z_Construct_UScriptStruct_FOWTStateMonitorTestState_Statics::NewStructOps, TEXT("OWTStateMonitorTestState"),&Z_Registration_Info_UScriptStruct_FOWTStateMonitorTestState, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTStateMonitorTestState), 1173367237U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTStateMonitor_Source_OWTStateMonitor_Private_Tests_OWTStateMonitorTestTypes_h__Script_OWTStateMonitor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTStateMonitor_Source_OWTStateMonitor_Private_Tests_OWTStateMonitorTestTypes_h__Script_OWTStateMonitor_1228484017{
	TEXT("/Script/OWTStateMonitor"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTStateMonitor_Source_OWTStateMonitor_Private_Tests_OWTStateMonitorTestTypes_h__Script_OWTStateMonitor_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTStateMonitor_Source_OWTStateMonitor_Private_Tests_OWTStateMonitorTestTypes_h__Script_OWTStateMonitor_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
