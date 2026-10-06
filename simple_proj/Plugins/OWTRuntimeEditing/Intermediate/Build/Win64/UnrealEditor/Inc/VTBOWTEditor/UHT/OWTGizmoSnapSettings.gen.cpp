// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Context/OWTGizmoSnapSettings.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTGizmoSnapSettings() {}

// ********** Begin Cross Module References ********************************************************
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTGizmoSnapSettings();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FOWTGizmoSnapSettings *********************************************
struct Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTGizmoSnapSettings); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTGizmoSnapSettings); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Context/OWTGizmoSnapSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TranslationStep_MetaData[] = {
		{ "Category", "OWT|Snapping" },
		{ "ClampMin", "0.001" },
		{ "ModuleRelativePath", "Public/Context/OWTGizmoSnapSettings.h" },
		{ "Units", "cm" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RotationStepDegrees_MetaData[] = {
		{ "Category", "OWT|Snapping" },
		{ "ClampMin", "0.001" },
		{ "ModuleRelativePath", "Public/Context/OWTGizmoSnapSettings.h" },
		{ "Units", "deg" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ScaleStep_MetaData[] = {
		{ "Category", "OWT|Snapping" },
		{ "ClampMin", "0.001" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Additive scale increment: 0.1 changes a scale of 1.0 to 1.1.\n" },
#endif
		{ "ModuleRelativePath", "Public/Context/OWTGizmoSnapSettings.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Additive scale increment: 0.1 changes a scale of 1.0 to 1.1." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bTranslationEnabled_MetaData[] = {
		{ "Category", "OWT|Snapping" },
		{ "ModuleRelativePath", "Public/Context/OWTGizmoSnapSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bRotationEnabled_MetaData[] = {
		{ "Category", "OWT|Snapping" },
		{ "ModuleRelativePath", "Public/Context/OWTGizmoSnapSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bScaleEnabled_MetaData[] = {
		{ "Category", "OWT|Snapping" },
		{ "ModuleRelativePath", "Public/Context/OWTGizmoSnapSettings.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTGizmoSnapSettings constinit property declarations *************
	static const UECodeGen_Private::FFloatPropertyParams NewProp_TranslationStep;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RotationStepDegrees;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ScaleStep;
	static void NewProp_bTranslationEnabled_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bTranslationEnabled;
	static void NewProp_bRotationEnabled_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bRotationEnabled;
	static void NewProp_bScaleEnabled_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bScaleEnabled;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTGizmoSnapSettings constinit property declarations ***************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTGizmoSnapSettings>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTGizmoSnapSettings;
class UScriptStruct* FOWTGizmoSnapSettings::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTGizmoSnapSettings.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTGizmoSnapSettings.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTGizmoSnapSettings, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTGizmoSnapSettings"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTGizmoSnapSettings.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTGizmoSnapSettings Property Definitions ************************
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_TranslationStep = { "TranslationStep", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTGizmoSnapSettings, TranslationStep), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TranslationStep_MetaData), NewProp_TranslationStep_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_RotationStepDegrees = { "RotationStepDegrees", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTGizmoSnapSettings, RotationStepDegrees), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RotationStepDegrees_MetaData), NewProp_RotationStepDegrees_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_ScaleStep = { "ScaleStep", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTGizmoSnapSettings, ScaleStep), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ScaleStep_MetaData), NewProp_ScaleStep_MetaData) };
void Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_bTranslationEnabled_SetBit(void* Obj)
{
	((FOWTGizmoSnapSettings*)Obj)->bTranslationEnabled = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_bTranslationEnabled = { "bTranslationEnabled", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTGizmoSnapSettings), &Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_bTranslationEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bTranslationEnabled_MetaData), NewProp_bTranslationEnabled_MetaData) };
void Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_bRotationEnabled_SetBit(void* Obj)
{
	((FOWTGizmoSnapSettings*)Obj)->bRotationEnabled = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_bRotationEnabled = { "bRotationEnabled", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTGizmoSnapSettings), &Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_bRotationEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRotationEnabled_MetaData), NewProp_bRotationEnabled_MetaData) };
void Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_bScaleEnabled_SetBit(void* Obj)
{
	((FOWTGizmoSnapSettings*)Obj)->bScaleEnabled = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_bScaleEnabled = { "bScaleEnabled", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTGizmoSnapSettings), &Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_bScaleEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bScaleEnabled_MetaData), NewProp_bScaleEnabled_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_TranslationStep,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_RotationStepDegrees,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_ScaleStep,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_bTranslationEnabled,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_bRotationEnabled,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewProp_bScaleEnabled,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTGizmoSnapSettings Property Definitions **************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTGizmoSnapSettings",
	Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::PropPointers),
	sizeof(FOWTGizmoSnapSettings),
	alignof(FOWTGizmoSnapSettings),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTGizmoSnapSettings()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTGizmoSnapSettings.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTGizmoSnapSettings.InnerSingleton, Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTGizmoSnapSettings.InnerSingleton);
}
// ********** End ScriptStruct FOWTGizmoSnapSettings ***********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTGizmoSnapSettings_h__Script_VTBOWTEditor_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FOWTGizmoSnapSettings::StaticStruct, Z_Construct_UScriptStruct_FOWTGizmoSnapSettings_Statics::NewStructOps, TEXT("OWTGizmoSnapSettings"),&Z_Registration_Info_UScriptStruct_FOWTGizmoSnapSettings, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTGizmoSnapSettings), 4038205799U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTGizmoSnapSettings_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTGizmoSnapSettings_h__Script_VTBOWTEditor_2631293488{
	TEXT("/Script/VTBOWTEditor"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTGizmoSnapSettings_h__Script_VTBOWTEditor_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTGizmoSnapSettings_h__Script_VTBOWTEditor_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
