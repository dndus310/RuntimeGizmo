// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Tests/OWTPCGConfigurationTestTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTPCGConfigurationTestTypes() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FOWTPCGConfigurationTestSchema ************************************
struct Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTPCGConfigurationTestSchema); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTPCGConfigurationTestSchema); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Fields introduced here require no additions to the configuration transfer implementation. */" },
#endif
		{ "ModuleRelativePath", "Private/Tests/OWTPCGConfigurationTestTypes.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Fields introduced here require no additions to the configuration transfer implementation." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FutureSetting_MetaData[] = {
		{ "Category", "OWTPCGConfigurationTestSchema" },
		{ "ModuleRelativePath", "Private/Tests/OWTPCGConfigurationTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FutureWeights_MetaData[] = {
		{ "Category", "OWTPCGConfigurationTestSchema" },
		{ "ModuleRelativePath", "Private/Tests/OWTPCGConfigurationTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FutureReference_MetaData[] = {
		{ "Category", "OWTPCGConfigurationTestSchema" },
		{ "ModuleRelativePath", "Private/Tests/OWTPCGConfigurationTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObservedValue_MetaData[] = {
		{ "Category", "OWTPCGConfigurationTestSchema" },
		{ "ModuleRelativePath", "Private/Tests/OWTPCGConfigurationTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WorkingValue_MetaData[] = {
		{ "Category", "OWTPCGConfigurationTestSchema" },
		{ "ModuleRelativePath", "Private/Tests/OWTPCGConfigurationTestTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InternalValue_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTPCGConfigurationTestTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTPCGConfigurationTestSchema constinit property declarations ****
	static const UECodeGen_Private::FStrPropertyParams NewProp_FutureSetting;
	static const UECodeGen_Private::FIntPropertyParams NewProp_FutureWeights_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_FutureWeights;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_FutureReference;
	static const UECodeGen_Private::FIntPropertyParams NewProp_ObservedValue;
	static const UECodeGen_Private::FIntPropertyParams NewProp_WorkingValue;
	static const UECodeGen_Private::FIntPropertyParams NewProp_InternalValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTPCGConfigurationTestSchema constinit property declarations ******
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTPCGConfigurationTestSchema>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTPCGConfigurationTestSchema;
class UScriptStruct* FOWTPCGConfigurationTestSchema::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTPCGConfigurationTestSchema.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTPCGConfigurationTestSchema.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTPCGConfigurationTestSchema"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTPCGConfigurationTestSchema.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTPCGConfigurationTestSchema Property Definitions ***************
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewProp_FutureSetting = { "FutureSetting", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTPCGConfigurationTestSchema, FutureSetting), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FutureSetting_MetaData), NewProp_FutureSetting_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewProp_FutureWeights_Inner = { "FutureWeights", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewProp_FutureWeights = { "FutureWeights", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTPCGConfigurationTestSchema, FutureWeights), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FutureWeights_MetaData), NewProp_FutureWeights_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewProp_FutureReference = { "FutureReference", nullptr, (EPropertyFlags)0x0114000000000001, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTPCGConfigurationTestSchema, FutureReference), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FutureReference_MetaData), NewProp_FutureReference_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewProp_ObservedValue = { "ObservedValue", nullptr, (EPropertyFlags)0x0010000000020001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTPCGConfigurationTestSchema, ObservedValue), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObservedValue_MetaData), NewProp_ObservedValue_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewProp_WorkingValue = { "WorkingValue", nullptr, (EPropertyFlags)0x0010000000002001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTPCGConfigurationTestSchema, WorkingValue), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WorkingValue_MetaData), NewProp_WorkingValue_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewProp_InternalValue = { "InternalValue", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTPCGConfigurationTestSchema, InternalValue), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InternalValue_MetaData), NewProp_InternalValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewProp_FutureSetting,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewProp_FutureWeights_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewProp_FutureWeights,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewProp_FutureReference,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewProp_ObservedValue,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewProp_WorkingValue,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewProp_InternalValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTPCGConfigurationTestSchema Property Definitions *****************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTPCGConfigurationTestSchema",
	Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::PropPointers),
	sizeof(FOWTPCGConfigurationTestSchema),
	alignof(FOWTPCGConfigurationTestSchema),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTPCGConfigurationTestSchema.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTPCGConfigurationTestSchema.InnerSingleton, Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTPCGConfigurationTestSchema.InnerSingleton);
}
// ********** End ScriptStruct FOWTPCGConfigurationTestSchema **************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTPCGConfigurationTestTypes_h__Script_VTBOWTEditor_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FOWTPCGConfigurationTestSchema::StaticStruct, Z_Construct_UScriptStruct_FOWTPCGConfigurationTestSchema_Statics::NewStructOps, TEXT("OWTPCGConfigurationTestSchema"),&Z_Registration_Info_UScriptStruct_FOWTPCGConfigurationTestSchema, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTPCGConfigurationTestSchema), 3713238292U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTPCGConfigurationTestTypes_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTPCGConfigurationTestTypes_h__Script_VTBOWTEditor_3425636172{
	TEXT("/Script/VTBOWTEditor"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTPCGConfigurationTestTypes_h__Script_VTBOWTEditor_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTPCGConfigurationTestTypes_h__Script_VTBOWTEditor_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
