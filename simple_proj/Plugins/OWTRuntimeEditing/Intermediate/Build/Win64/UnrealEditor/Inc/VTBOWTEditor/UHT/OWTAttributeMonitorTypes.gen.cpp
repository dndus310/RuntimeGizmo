// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UI/OWTAttributeMonitorTypes.h"
#include "Events/OWTAttributeTypes.h"
#include "Events/OWTEventTypes.h"
#include "Extensions/OWTToolDescriptor.h"
#include "State/OWTEditingSessionTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTAttributeMonitorTypes() {}

// ********** Begin Cross Module References ********************************************************
OWTEVENTCORE_API UScriptStruct* Z_Construct_UScriptStruct_FOWTEventRecord();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTAttributeSnapshot();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTModeSnapshot();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTToolAvailability();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FOWTAttributeMonitorSnapshot **************************************
struct Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTAttributeMonitorSnapshot); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTAttributeMonitorSnapshot); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Runtime editor adapter data. The reusable viewer has no knowledge of these fields. */" },
#endif
		{ "ModuleRelativePath", "Private/UI/OWTAttributeMonitorTypes.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Runtime editor adapter data. The reusable viewer has no knowledge of these fields." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Selection_MetaData[] = {
		{ "ModuleRelativePath", "Private/UI/OWTAttributeMonitorTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Mode_MetaData[] = {
		{ "ModuleRelativePath", "Private/UI/OWTAttributeMonitorTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Tools_MetaData[] = {
		{ "ModuleRelativePath", "Private/UI/OWTAttributeMonitorTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DuplicationOperations_MetaData[] = {
		{ "ModuleRelativePath", "Private/UI/OWTAttributeMonitorTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ProceduralComponents_MetaData[] = {
		{ "ModuleRelativePath", "Private/UI/OWTAttributeMonitorTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTAttributeMonitorSnapshot constinit property declarations ******
	static const UECodeGen_Private::FStructPropertyParams NewProp_Selection;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Mode;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Tools_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Tools;
	static const UECodeGen_Private::FStructPropertyParams NewProp_DuplicationOperations_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_DuplicationOperations;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ProceduralComponents_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ProceduralComponents;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTAttributeMonitorSnapshot constinit property declarations ********
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTAttributeMonitorSnapshot>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorSnapshot;
class UScriptStruct* FOWTAttributeMonitorSnapshot::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorSnapshot.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorSnapshot.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTAttributeMonitorSnapshot"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorSnapshot.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTAttributeMonitorSnapshot Property Definitions *****************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_Selection = { "Selection", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeMonitorSnapshot, Selection), Z_Construct_UScriptStruct_FOWTAttributeSnapshot, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Selection_MetaData), NewProp_Selection_MetaData) }; // 1151875563
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_Mode = { "Mode", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeMonitorSnapshot, Mode), Z_Construct_UScriptStruct_FOWTModeSnapshot, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Mode_MetaData), NewProp_Mode_MetaData) }; // 1626674559
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_Tools_Inner = { "Tools", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FOWTToolAvailability, METADATA_PARAMS(0, nullptr) }; // 3209918832
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_Tools = { "Tools", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeMonitorSnapshot, Tools), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Tools_MetaData), NewProp_Tools_MetaData) }; // 3209918832
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_DuplicationOperations_Inner = { "DuplicationOperations", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot, METADATA_PARAMS(0, nullptr) }; // 2137892919
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_DuplicationOperations = { "DuplicationOperations", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeMonitorSnapshot, DuplicationOperations), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DuplicationOperations_MetaData), NewProp_DuplicationOperations_MetaData) }; // 2137892919
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_ProceduralComponents_Inner = { "ProceduralComponents", nullptr, (EPropertyFlags)0x0000008000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot, METADATA_PARAMS(0, nullptr) }; // 3712393560
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_ProceduralComponents = { "ProceduralComponents", nullptr, (EPropertyFlags)0x0010008000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeMonitorSnapshot, ProceduralComponents), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ProceduralComponents_MetaData), NewProp_ProceduralComponents_MetaData) }; // 3712393560
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_Selection,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_Mode,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_Tools_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_Tools,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_DuplicationOperations_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_DuplicationOperations,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_ProceduralComponents_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewProp_ProceduralComponents,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTAttributeMonitorSnapshot Property Definitions *******************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTAttributeMonitorSnapshot",
	Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::PropPointers),
	sizeof(FOWTAttributeMonitorSnapshot),
	alignof(FOWTAttributeMonitorSnapshot),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000005),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorSnapshot.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorSnapshot.InnerSingleton, Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorSnapshot.InnerSingleton);
}
// ********** End ScriptStruct FOWTAttributeMonitorSnapshot ****************************************

// ********** Begin ScriptStruct FOWTAttributeMonitorEvents ****************************************
struct Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTAttributeMonitorEvents); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTAttributeMonitorEvents); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Transport evidence is a separate source and never supplies the current Actor state. */" },
#endif
		{ "ModuleRelativePath", "Private/UI/OWTAttributeMonitorTypes.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Transport evidence is a separate source and never supplies the current Actor state." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Journal_MetaData[] = {
		{ "ModuleRelativePath", "Private/UI/OWTAttributeMonitorTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTAttributeMonitorEvents constinit property declarations ********
	static const UECodeGen_Private::FStructPropertyParams NewProp_Journal_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Journal;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTAttributeMonitorEvents constinit property declarations **********
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTAttributeMonitorEvents>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorEvents;
class UScriptStruct* FOWTAttributeMonitorEvents::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorEvents.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorEvents.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTAttributeMonitorEvents"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorEvents.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTAttributeMonitorEvents Property Definitions *******************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics::NewProp_Journal_Inner = { "Journal", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FOWTEventRecord, METADATA_PARAMS(0, nullptr) }; // 2725191315
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics::NewProp_Journal = { "Journal", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeMonitorEvents, Journal), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Journal_MetaData), NewProp_Journal_MetaData) }; // 2725191315
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics::NewProp_Journal_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics::NewProp_Journal,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTAttributeMonitorEvents Property Definitions *********************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTAttributeMonitorEvents",
	Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics::PropPointers),
	sizeof(FOWTAttributeMonitorEvents),
	alignof(FOWTAttributeMonitorEvents),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorEvents.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorEvents.InnerSingleton, Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorEvents.InnerSingleton);
}
// ********** End ScriptStruct FOWTAttributeMonitorEvents ******************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_UI_OWTAttributeMonitorTypes_h__Script_VTBOWTEditor_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FOWTAttributeMonitorSnapshot::StaticStruct, Z_Construct_UScriptStruct_FOWTAttributeMonitorSnapshot_Statics::NewStructOps, TEXT("OWTAttributeMonitorSnapshot"),&Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorSnapshot, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTAttributeMonitorSnapshot), 3186390914U) },
		{ FOWTAttributeMonitorEvents::StaticStruct, Z_Construct_UScriptStruct_FOWTAttributeMonitorEvents_Statics::NewStructOps, TEXT("OWTAttributeMonitorEvents"),&Z_Registration_Info_UScriptStruct_FOWTAttributeMonitorEvents, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTAttributeMonitorEvents), 2733832953U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_UI_OWTAttributeMonitorTypes_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_UI_OWTAttributeMonitorTypes_h__Script_VTBOWTEditor_2893693184{
	TEXT("/Script/VTBOWTEditor"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_UI_OWTAttributeMonitorTypes_h__Script_VTBOWTEditor_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_UI_OWTAttributeMonitorTypes_h__Script_VTBOWTEditor_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
