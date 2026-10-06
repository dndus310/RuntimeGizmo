// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Context/SaveTransformContext.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeSaveTransformContext() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FTransform();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FSaveTransformContext();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FSaveTransformContext *********************************************
struct Z_Construct_UScriptStruct_FSaveTransformContext_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FSaveTransformContext); }
	static inline consteval int16 GetStructAlignment() { return alignof(FSaveTransformContext); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Context/SaveTransformContext.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SaveTransform_MetaData[] = {
		{ "Category", "History" },
		{ "ModuleRelativePath", "Public/Context/SaveTransformContext.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectID_MetaData[] = {
		{ "Category", "History" },
		{ "ModuleRelativePath", "Public/Context/SaveTransformContext.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ModeName_MetaData[] = {
		{ "Category", "History" },
		{ "ModuleRelativePath", "Public/Context/SaveTransformContext.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FSaveTransformContext constinit property declarations *************
	static const UECodeGen_Private::FStructPropertyParams NewProp_SaveTransform;
	static const UECodeGen_Private::FStrPropertyParams NewProp_ObjectID;
	static const UECodeGen_Private::FStrPropertyParams NewProp_ModeName;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FSaveTransformContext constinit property declarations ***************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FSaveTransformContext>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FSaveTransformContext_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FSaveTransformContext;
class UScriptStruct* FSaveTransformContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FSaveTransformContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FSaveTransformContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FSaveTransformContext, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("SaveTransformContext"));
	}
	return Z_Registration_Info_UScriptStruct_FSaveTransformContext.OuterSingleton;
	}

// ********** Begin ScriptStruct FSaveTransformContext Property Definitions ************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FSaveTransformContext_Statics::NewProp_SaveTransform = { "SaveTransform", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FSaveTransformContext, SaveTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SaveTransform_MetaData), NewProp_SaveTransform_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FSaveTransformContext_Statics::NewProp_ObjectID = { "ObjectID", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FSaveTransformContext, ObjectID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectID_MetaData), NewProp_ObjectID_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FSaveTransformContext_Statics::NewProp_ModeName = { "ModeName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FSaveTransformContext, ModeName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ModeName_MetaData), NewProp_ModeName_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FSaveTransformContext_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSaveTransformContext_Statics::NewProp_SaveTransform,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSaveTransformContext_Statics::NewProp_ObjectID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FSaveTransformContext_Statics::NewProp_ModeName,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSaveTransformContext_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FSaveTransformContext Property Definitions **************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FSaveTransformContext_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"SaveTransformContext",
	Z_Construct_UScriptStruct_FSaveTransformContext_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSaveTransformContext_Statics::PropPointers),
	sizeof(FSaveTransformContext),
	alignof(FSaveTransformContext),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FSaveTransformContext_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FSaveTransformContext_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FSaveTransformContext()
{
	if (!Z_Registration_Info_UScriptStruct_FSaveTransformContext.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FSaveTransformContext.InnerSingleton, Z_Construct_UScriptStruct_FSaveTransformContext_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FSaveTransformContext.InnerSingleton);
}
// ********** End ScriptStruct FSaveTransformContext ***********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_SaveTransformContext_h__Script_VTBOWTEditor_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FSaveTransformContext::StaticStruct, Z_Construct_UScriptStruct_FSaveTransformContext_Statics::NewStructOps, TEXT("SaveTransformContext"),&Z_Registration_Info_UScriptStruct_FSaveTransformContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FSaveTransformContext), 4210459777U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_SaveTransformContext_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_SaveTransformContext_h__Script_VTBOWTEditor_572876190{
	TEXT("/Script/VTBOWTEditor"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_SaveTransformContext_h__Script_VTBOWTEditor_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_SaveTransformContext_h__Script_VTBOWTEditor_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
