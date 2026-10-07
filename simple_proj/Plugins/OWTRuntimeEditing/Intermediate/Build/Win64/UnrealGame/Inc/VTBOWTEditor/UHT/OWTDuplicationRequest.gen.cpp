// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Duplication/OWTDuplicationRequest.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTDuplicationRequest() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UEnum* Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationGenerationPolicy();
VTBOWTEDITOR_API UEnum* Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationHierarchyScope();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTDuplicationOptions();
// ********** End Cross Module References **********************************************************

// ********** Begin Enum EOWTDuplicationHierarchyScope *********************************************
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EOWTDuplicationHierarchyScope;
static UEnum* EOWTDuplicationHierarchyScope_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EOWTDuplicationHierarchyScope.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EOWTDuplicationHierarchyScope.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationHierarchyScope, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("EOWTDuplicationHierarchyScope"));
	}
	return Z_Registration_Info_UEnum_EOWTDuplicationHierarchyScope.OuterSingleton;
}
template<> VTBOWTEDITOR_NON_ATTRIBUTED_API UEnum* StaticEnum<EOWTDuplicationHierarchyScope>()
{
	return EOWTDuplicationHierarchyScope_StaticEnum();
}
struct Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationHierarchyScope_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "ActorAndManagedChildren.Name", "EOWTDuplicationHierarchyScope::ActorAndManagedChildren" },
		{ "AuthoredHierarchy.Name", "EOWTDuplicationHierarchyScope::AuthoredHierarchy" },
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Duplication/OWTDuplicationRequest.h" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EOWTDuplicationHierarchyScope::AuthoredHierarchy", (int64)EOWTDuplicationHierarchyScope::AuthoredHierarchy },
		{ "EOWTDuplicationHierarchyScope::ActorAndManagedChildren", (int64)EOWTDuplicationHierarchyScope::ActorAndManagedChildren },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
}; // struct Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationHierarchyScope_Statics 
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationHierarchyScope_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	"EOWTDuplicationHierarchyScope",
	"EOWTDuplicationHierarchyScope",
	Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationHierarchyScope_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationHierarchyScope_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationHierarchyScope_Statics::Enum_MetaDataParams), Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationHierarchyScope_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationHierarchyScope()
{
	if (!Z_Registration_Info_UEnum_EOWTDuplicationHierarchyScope.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EOWTDuplicationHierarchyScope.InnerSingleton, Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationHierarchyScope_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EOWTDuplicationHierarchyScope.InnerSingleton;
}
// ********** End Enum EOWTDuplicationHierarchyScope ***********************************************

// ********** Begin Enum EOWTDuplicationGenerationPolicy *******************************************
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EOWTDuplicationGenerationPolicy;
static UEnum* EOWTDuplicationGenerationPolicy_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EOWTDuplicationGenerationPolicy.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EOWTDuplicationGenerationPolicy.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationGenerationPolicy, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("EOWTDuplicationGenerationPolicy"));
	}
	return Z_Registration_Info_UEnum_EOWTDuplicationGenerationPolicy.OuterSingleton;
}
template<> VTBOWTEDITOR_NON_ATTRIBUTED_API UEnum* StaticEnum<EOWTDuplicationGenerationPolicy>()
{
	return EOWTDuplicationGenerationPolicy_StaticEnum();
}
struct Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationGenerationPolicy_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "KeepUnGenerated.Name", "EOWTDuplicationGenerationPolicy::KeepUnGenerated" },
		{ "ModuleRelativePath", "Public/Duplication/OWTDuplicationRequest.h" },
		{ "RegenerateIfSourceGenerated.Name", "EOWTDuplicationGenerationPolicy::RegenerateIfSourceGenerated" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EOWTDuplicationGenerationPolicy::RegenerateIfSourceGenerated", (int64)EOWTDuplicationGenerationPolicy::RegenerateIfSourceGenerated },
		{ "EOWTDuplicationGenerationPolicy::KeepUnGenerated", (int64)EOWTDuplicationGenerationPolicy::KeepUnGenerated },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
}; // struct Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationGenerationPolicy_Statics 
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationGenerationPolicy_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	"EOWTDuplicationGenerationPolicy",
	"EOWTDuplicationGenerationPolicy",
	Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationGenerationPolicy_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationGenerationPolicy_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationGenerationPolicy_Statics::Enum_MetaDataParams), Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationGenerationPolicy_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationGenerationPolicy()
{
	if (!Z_Registration_Info_UEnum_EOWTDuplicationGenerationPolicy.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EOWTDuplicationGenerationPolicy.InnerSingleton, Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationGenerationPolicy_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EOWTDuplicationGenerationPolicy.InnerSingleton;
}
// ********** End Enum EOWTDuplicationGenerationPolicy *********************************************

// ********** Begin ScriptStruct FOWTDuplicationOptions ********************************************
struct Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTDuplicationOptions); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTDuplicationOptions); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Duplication/OWTDuplicationRequest.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WorldOffset_MetaData[] = {
		{ "Category", "OWTDuplicationOptions" },
		{ "ModuleRelativePath", "Public/Duplication/OWTDuplicationRequest.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HierarchyScope_MetaData[] = {
		{ "Category", "OWTDuplicationOptions" },
		{ "ModuleRelativePath", "Public/Duplication/OWTDuplicationRequest.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GenerationPolicy_MetaData[] = {
		{ "Category", "OWTDuplicationOptions" },
		{ "ModuleRelativePath", "Public/Duplication/OWTDuplicationRequest.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTDuplicationOptions constinit property declarations ************
	static const UECodeGen_Private::FStructPropertyParams NewProp_WorldOffset;
	static const UECodeGen_Private::FBytePropertyParams NewProp_HierarchyScope_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_HierarchyScope;
	static const UECodeGen_Private::FBytePropertyParams NewProp_GenerationPolicy_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_GenerationPolicy;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTDuplicationOptions constinit property declarations **************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTDuplicationOptions>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTDuplicationOptions;
class UScriptStruct* FOWTDuplicationOptions::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTDuplicationOptions.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTDuplicationOptions.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTDuplicationOptions, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTDuplicationOptions"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTDuplicationOptions.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTDuplicationOptions Property Definitions ***********************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::NewProp_WorldOffset = { "WorldOffset", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTDuplicationOptions, WorldOffset), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WorldOffset_MetaData), NewProp_WorldOffset_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::NewProp_HierarchyScope_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::NewProp_HierarchyScope = { "HierarchyScope", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTDuplicationOptions, HierarchyScope), Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationHierarchyScope, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HierarchyScope_MetaData), NewProp_HierarchyScope_MetaData) }; // 2029029374
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::NewProp_GenerationPolicy_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::NewProp_GenerationPolicy = { "GenerationPolicy", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTDuplicationOptions, GenerationPolicy), Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationGenerationPolicy, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GenerationPolicy_MetaData), NewProp_GenerationPolicy_MetaData) }; // 916081954
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::NewProp_WorldOffset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::NewProp_HierarchyScope_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::NewProp_HierarchyScope,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::NewProp_GenerationPolicy_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::NewProp_GenerationPolicy,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTDuplicationOptions Property Definitions *************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTDuplicationOptions",
	Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::PropPointers),
	sizeof(FOWTDuplicationOptions),
	alignof(FOWTDuplicationOptions),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTDuplicationOptions()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTDuplicationOptions.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTDuplicationOptions.InnerSingleton, Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTDuplicationOptions.InnerSingleton);
}
// ********** End ScriptStruct FOWTDuplicationOptions **********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Duplication_OWTDuplicationRequest_h__Script_VTBOWTEditor_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EOWTDuplicationHierarchyScope_StaticEnum, TEXT("EOWTDuplicationHierarchyScope"), &Z_Registration_Info_UEnum_EOWTDuplicationHierarchyScope, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2029029374U) },
		{ EOWTDuplicationGenerationPolicy_StaticEnum, TEXT("EOWTDuplicationGenerationPolicy"), &Z_Registration_Info_UEnum_EOWTDuplicationGenerationPolicy, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 916081954U) },
	};
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FOWTDuplicationOptions::StaticStruct, Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics::NewStructOps, TEXT("OWTDuplicationOptions"),&Z_Registration_Info_UScriptStruct_FOWTDuplicationOptions, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTDuplicationOptions), 1929535697U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Duplication_OWTDuplicationRequest_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Duplication_OWTDuplicationRequest_h__Script_VTBOWTEditor_237587677{
	TEXT("/Script/VTBOWTEditor"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Duplication_OWTDuplicationRequest_h__Script_VTBOWTEditor_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Duplication_OWTDuplicationRequest_h__Script_VTBOWTEditor_Statics::ScriptStructInfo),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Duplication_OWTDuplicationRequest_h__Script_VTBOWTEditor_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Duplication_OWTDuplicationRequest_h__Script_VTBOWTEditor_Statics::EnumInfo),
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
