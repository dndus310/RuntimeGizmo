// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Extensions/OWTToolDescriptor.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTToolDescriptor() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UClass_NoRegister();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolBuilder_NoRegister();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTToolAvailability();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTToolDescriptor();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FOWTToolDescriptor ************************************************
struct Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTToolDescriptor); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTToolDescriptor); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ToolId_MetaData[] = {
		{ "Category", "OWTToolDescriptor" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Label_MetaData[] = {
		{ "Category", "OWTToolDescriptor" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Category_MetaData[] = {
		{ "Category", "OWTToolDescriptor" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Description_MetaData[] = {
		{ "Category", "OWTToolDescriptor" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_BuilderClass_MetaData[] = {
		{ "Category", "OWTToolDescriptor" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bRequiresHistory_MetaData[] = {
		{ "Category", "OWTToolDescriptor" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bRequiresMeshRendering_MetaData[] = {
		{ "Category", "OWTToolDescriptor" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTToolDescriptor constinit property declarations ****************
	static const UECodeGen_Private::FNamePropertyParams NewProp_ToolId;
	static const UECodeGen_Private::FTextPropertyParams NewProp_Label;
	static const UECodeGen_Private::FTextPropertyParams NewProp_Category;
	static const UECodeGen_Private::FTextPropertyParams NewProp_Description;
	static const UECodeGen_Private::FClassPropertyParams NewProp_BuilderClass;
	static void NewProp_bRequiresHistory_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bRequiresHistory;
	static void NewProp_bRequiresMeshRendering_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bRequiresMeshRendering;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTToolDescriptor constinit property declarations ******************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTToolDescriptor>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTToolDescriptor;
class UScriptStruct* FOWTToolDescriptor::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTToolDescriptor.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTToolDescriptor.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTToolDescriptor, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTToolDescriptor"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTToolDescriptor.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTToolDescriptor Property Definitions ***************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_ToolId = { "ToolId", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTToolDescriptor, ToolId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ToolId_MetaData), NewProp_ToolId_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_Label = { "Label", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTToolDescriptor, Label), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Label_MetaData), NewProp_Label_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_Category = { "Category", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTToolDescriptor, Category), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Category_MetaData), NewProp_Category_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_Description = { "Description", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTToolDescriptor, Description), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Description_MetaData), NewProp_Description_MetaData) };
const UECodeGen_Private::FClassPropertyParams Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_BuilderClass = { "BuilderClass", nullptr, (EPropertyFlags)0x0014000000000015, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTToolDescriptor, BuilderClass), Z_Construct_UClass_UClass_NoRegister, Z_Construct_UClass_UInteractiveToolBuilder_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_BuilderClass_MetaData), NewProp_BuilderClass_MetaData) };
void Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_bRequiresHistory_SetBit(void* Obj)
{
	((FOWTToolDescriptor*)Obj)->bRequiresHistory = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_bRequiresHistory = { "bRequiresHistory", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTToolDescriptor), &Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_bRequiresHistory_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRequiresHistory_MetaData), NewProp_bRequiresHistory_MetaData) };
void Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_bRequiresMeshRendering_SetBit(void* Obj)
{
	((FOWTToolDescriptor*)Obj)->bRequiresMeshRendering = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_bRequiresMeshRendering = { "bRequiresMeshRendering", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTToolDescriptor), &Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_bRequiresMeshRendering_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRequiresMeshRendering_MetaData), NewProp_bRequiresMeshRendering_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_ToolId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_Label,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_Category,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_Description,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_BuilderClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_bRequiresHistory,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewProp_bRequiresMeshRendering,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTToolDescriptor Property Definitions *****************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTToolDescriptor",
	Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::PropPointers),
	sizeof(FOWTToolDescriptor),
	alignof(FOWTToolDescriptor),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTToolDescriptor()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTToolDescriptor.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTToolDescriptor.InnerSingleton, Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTToolDescriptor.InnerSingleton);
}
// ********** End ScriptStruct FOWTToolDescriptor **************************************************

// ********** Begin ScriptStruct FOWTToolAvailability **********************************************
struct Z_Construct_UScriptStruct_FOWTToolAvailability_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTToolAvailability); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTToolAvailability); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ToolId_MetaData[] = {
		{ "Category", "OWTToolAvailability" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Label_MetaData[] = {
		{ "Category", "OWTToolAvailability" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Category_MetaData[] = {
		{ "Category", "OWTToolAvailability" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Reason_MetaData[] = {
		{ "Category", "OWTToolAvailability" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEnabled_MetaData[] = {
		{ "Category", "OWTToolAvailability" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bActive_MetaData[] = {
		{ "Category", "OWTToolAvailability" },
		{ "ModuleRelativePath", "Public/Extensions/OWTToolDescriptor.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTToolAvailability constinit property declarations **************
	static const UECodeGen_Private::FNamePropertyParams NewProp_ToolId;
	static const UECodeGen_Private::FTextPropertyParams NewProp_Label;
	static const UECodeGen_Private::FTextPropertyParams NewProp_Category;
	static const UECodeGen_Private::FTextPropertyParams NewProp_Reason;
	static void NewProp_bEnabled_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnabled;
	static void NewProp_bActive_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bActive;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTToolAvailability constinit property declarations ****************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTToolAvailability>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTToolAvailability_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTToolAvailability;
class UScriptStruct* FOWTToolAvailability::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTToolAvailability.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTToolAvailability.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTToolAvailability, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTToolAvailability"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTToolAvailability.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTToolAvailability Property Definitions *************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_ToolId = { "ToolId", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTToolAvailability, ToolId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ToolId_MetaData), NewProp_ToolId_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_Label = { "Label", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTToolAvailability, Label), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Label_MetaData), NewProp_Label_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_Category = { "Category", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTToolAvailability, Category), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Category_MetaData), NewProp_Category_MetaData) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_Reason = { "Reason", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTToolAvailability, Reason), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Reason_MetaData), NewProp_Reason_MetaData) };
void Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_bEnabled_SetBit(void* Obj)
{
	((FOWTToolAvailability*)Obj)->bEnabled = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_bEnabled = { "bEnabled", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTToolAvailability), &Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_bEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEnabled_MetaData), NewProp_bEnabled_MetaData) };
void Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_bActive_SetBit(void* Obj)
{
	((FOWTToolAvailability*)Obj)->bActive = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_bActive = { "bActive", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTToolAvailability), &Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_bActive_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bActive_MetaData), NewProp_bActive_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_ToolId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_Label,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_Category,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_Reason,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_bEnabled,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewProp_bActive,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTToolAvailability Property Definitions ***************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTToolAvailability",
	Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::PropPointers),
	sizeof(FOWTToolAvailability),
	alignof(FOWTToolAvailability),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTToolAvailability()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTToolAvailability.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTToolAvailability.InnerSingleton, Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTToolAvailability.InnerSingleton);
}
// ********** End ScriptStruct FOWTToolAvailability ************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Extensions_OWTToolDescriptor_h__Script_VTBOWTEditor_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FOWTToolDescriptor::StaticStruct, Z_Construct_UScriptStruct_FOWTToolDescriptor_Statics::NewStructOps, TEXT("OWTToolDescriptor"),&Z_Registration_Info_UScriptStruct_FOWTToolDescriptor, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTToolDescriptor), 1508595900U) },
		{ FOWTToolAvailability::StaticStruct, Z_Construct_UScriptStruct_FOWTToolAvailability_Statics::NewStructOps, TEXT("OWTToolAvailability"),&Z_Registration_Info_UScriptStruct_FOWTToolAvailability, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTToolAvailability), 3209918832U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Extensions_OWTToolDescriptor_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Extensions_OWTToolDescriptor_h__Script_VTBOWTEditor_3789191356{
	TEXT("/Script/VTBOWTEditor"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Extensions_OWTToolDescriptor_h__Script_VTBOWTEditor_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Extensions_OWTToolDescriptor_h__Script_VTBOWTEditor_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
