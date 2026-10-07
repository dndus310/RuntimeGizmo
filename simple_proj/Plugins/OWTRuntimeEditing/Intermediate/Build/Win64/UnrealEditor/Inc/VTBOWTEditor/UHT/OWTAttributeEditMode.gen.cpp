// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Modes/OWTAttributeEditMode.h"
#include "Context/OWTGizmoSnapSettings.h"
#include "Extensions/OWTToolDescriptor.h"
#include "State/OWTEditingSessionTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTAttributeEditMode() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolBuilder_NoRegister();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UToolTargetFactory_NoRegister();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeEditMode();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeEditMode_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeEditSessionContext_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTEditContextReceiver_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTRuntimeActorDuplicator_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTEditorSubsystem_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTEditorToolsContext_NoRegister();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTGizmoSnapSettings();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTModeSnapshot();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTRegisteredTool();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTToolDescriptor();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FOWTRegisteredTool ************************************************
struct Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTRegisteredTool); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTRegisteredTool); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Descriptor_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Builder_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ProviderId_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Token_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTRegisteredTool constinit property declarations ****************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Descriptor;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Builder;
	static const UECodeGen_Private::FNamePropertyParams NewProp_ProviderId;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Token;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTRegisteredTool constinit property declarations ******************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTRegisteredTool>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTRegisteredTool;
class UScriptStruct* FOWTRegisteredTool::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTRegisteredTool.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTRegisteredTool.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTRegisteredTool, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTRegisteredTool"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTRegisteredTool.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTRegisteredTool Property Definitions ***************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::NewProp_Descriptor = { "Descriptor", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTRegisteredTool, Descriptor), Z_Construct_UScriptStruct_FOWTToolDescriptor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Descriptor_MetaData), NewProp_Descriptor_MetaData) }; // 1508595900
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::NewProp_Builder = { "Builder", nullptr, (EPropertyFlags)0x0114000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTRegisteredTool, Builder), Z_Construct_UClass_UInteractiveToolBuilder_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Builder_MetaData), NewProp_Builder_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::NewProp_ProviderId = { "ProviderId", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTRegisteredTool, ProviderId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ProviderId_MetaData), NewProp_ProviderId_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::NewProp_Token = { "Token", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTRegisteredTool, Token), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Token_MetaData), NewProp_Token_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::NewProp_Descriptor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::NewProp_Builder,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::NewProp_ProviderId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::NewProp_Token,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTRegisteredTool Property Definitions *****************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTRegisteredTool",
	Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::PropPointers),
	sizeof(FOWTRegisteredTool),
	alignof(FOWTRegisteredTool),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTRegisteredTool()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTRegisteredTool.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTRegisteredTool.InnerSingleton, Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTRegisteredTool.InnerSingleton);
}
// ********** End ScriptStruct FOWTRegisteredTool **************************************************

// ********** Begin Class UOWTAttributeEditMode ****************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTAttributeEditMode;
UClass* UOWTAttributeEditMode::GetPrivateStaticClass()
{
	using TClass = UOWTAttributeEditMode;
	if (!Z_Registration_Info_UClass_UOWTAttributeEditMode.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTAttributeEditMode"),
			Z_Registration_Info_UClass_UOWTAttributeEditMode.InnerSingleton,
			StaticRegisterNativesUOWTAttributeEditMode,
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
	return Z_Registration_Info_UClass_UOWTAttributeEditMode.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTAttributeEditMode_NoRegister()
{
	return UOWTAttributeEditMode::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTAttributeEditMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Modes/OWTAttributeEditMode.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultToolId_MetaData[] = {
		{ "Category", "OWT|Tools" },
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AdditionalTools_MetaData[] = {
		{ "Category", "OWT|Tools" },
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Subsystem_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ToolsContext_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SessionContext_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Duplicator_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Selection_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RegisteredTools_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RegisteredTargets_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RegisteredServices_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Operations_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Snapshot_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SnapSettings_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/OWTAttributeEditMode.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTAttributeEditMode constinit property declarations ********************
	static const UECodeGen_Private::FNamePropertyParams NewProp_DefaultToolId;
	static const UECodeGen_Private::FStructPropertyParams NewProp_AdditionalTools_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_AdditionalTools;
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_Subsystem;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ToolsContext;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SessionContext;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Duplicator;
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_Selection;
	static const UECodeGen_Private::FStructPropertyParams NewProp_RegisteredTools_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_RegisteredTools;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_RegisteredTargets_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_RegisteredTargets;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_RegisteredServices_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_RegisteredServices;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Operations_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Operations;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Snapshot;
	static const UECodeGen_Private::FStructPropertyParams NewProp_SnapSettings;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UOWTAttributeEditMode constinit property declarations **********************
	static UObject* (*const DependentSingletons[])();
	static const UECodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTAttributeEditMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTAttributeEditMode_Statics

// ********** Begin Class UOWTAttributeEditMode Property Definitions *******************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_DefaultToolId = { "DefaultToolId", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditMode, DefaultToolId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultToolId_MetaData), NewProp_DefaultToolId_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_AdditionalTools_Inner = { "AdditionalTools", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FOWTToolDescriptor, METADATA_PARAMS(0, nullptr) }; // 1508595900
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_AdditionalTools = { "AdditionalTools", nullptr, (EPropertyFlags)0x0010000000010001, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditMode, AdditionalTools), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AdditionalTools_MetaData), NewProp_AdditionalTools_MetaData) }; // 1508595900
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_Subsystem = { "Subsystem", nullptr, (EPropertyFlags)0x0044000000002000, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditMode, Subsystem), Z_Construct_UClass_UVTBOWTEditorSubsystem_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Subsystem_MetaData), NewProp_Subsystem_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_ToolsContext = { "ToolsContext", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditMode, ToolsContext), Z_Construct_UClass_UVTBOWTEditorToolsContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ToolsContext_MetaData), NewProp_ToolsContext_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_SessionContext = { "SessionContext", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditMode, SessionContext), Z_Construct_UClass_UOWTAttributeEditSessionContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SessionContext_MetaData), NewProp_SessionContext_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_Duplicator = { "Duplicator", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditMode, Duplicator), Z_Construct_UClass_UOWTRuntimeActorDuplicator_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Duplicator_MetaData), NewProp_Duplicator_MetaData) };
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_Selection = { "Selection", nullptr, (EPropertyFlags)0x0044000000002000, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditMode, Selection), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Selection_MetaData), NewProp_Selection_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_RegisteredTools_Inner = { "RegisteredTools", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FOWTRegisteredTool, METADATA_PARAMS(0, nullptr) }; // 69269249
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_RegisteredTools = { "RegisteredTools", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditMode, RegisteredTools), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RegisteredTools_MetaData), NewProp_RegisteredTools_MetaData) }; // 69269249
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_RegisteredTargets_Inner = { "RegisteredTargets", nullptr, (EPropertyFlags)0x0104000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UToolTargetFactory_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_RegisteredTargets = { "RegisteredTargets", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditMode, RegisteredTargets), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RegisteredTargets_MetaData), NewProp_RegisteredTargets_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_RegisteredServices_Inner = { "RegisteredServices", nullptr, (EPropertyFlags)0x0104000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_RegisteredServices = { "RegisteredServices", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditMode, RegisteredServices), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RegisteredServices_MetaData), NewProp_RegisteredServices_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_Operations_Inner = { "Operations", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot, METADATA_PARAMS(0, nullptr) }; // 2137892919
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_Operations = { "Operations", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditMode, Operations), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Operations_MetaData), NewProp_Operations_MetaData) }; // 2137892919
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_Snapshot = { "Snapshot", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditMode, Snapshot), Z_Construct_UScriptStruct_FOWTModeSnapshot, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Snapshot_MetaData), NewProp_Snapshot_MetaData) }; // 1626674559
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_SnapSettings = { "SnapSettings", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditMode, SnapSettings), Z_Construct_UScriptStruct_FOWTGizmoSnapSettings, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SnapSettings_MetaData), NewProp_SnapSettings_MetaData) }; // 4038205799
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOWTAttributeEditMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_DefaultToolId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_AdditionalTools_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_AdditionalTools,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_Subsystem,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_ToolsContext,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_SessionContext,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_Duplicator,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_Selection,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_RegisteredTools_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_RegisteredTools,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_RegisteredTargets_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_RegisteredTargets,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_RegisteredServices_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_RegisteredServices,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_Operations_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_Operations,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_Snapshot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditMode_Statics::NewProp_SnapSettings,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeEditMode_Statics::PropPointers) < 2048);
// ********** End Class UOWTAttributeEditMode Property Definitions *********************************
UObject* (*const Z_Construct_UClass_UOWTAttributeEditMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeEditMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::InterfaceParams[] = {
	{ Z_Construct_UClass_UOWTEditContextReceiver_NoRegister, (int32)VTABLE_OFFSET(UOWTAttributeEditMode, IOWTEditContextReceiver), false },  // 1559580018
};
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTAttributeEditMode_Statics::ClassParams = {
	&UOWTAttributeEditMode::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UOWTAttributeEditMode_Statics::PropPointers,
	InterfaceParams,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeEditMode_Statics::PropPointers),
	UE_ARRAY_COUNT(InterfaceParams),
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeEditMode_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTAttributeEditMode_Statics::Class_MetaDataParams)
};
void UOWTAttributeEditMode::StaticRegisterNativesUOWTAttributeEditMode()
{
}
UClass* Z_Construct_UClass_UOWTAttributeEditMode()
{
	if (!Z_Registration_Info_UClass_UOWTAttributeEditMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTAttributeEditMode.OuterSingleton, Z_Construct_UClass_UOWTAttributeEditMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTAttributeEditMode.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTAttributeEditMode);
UOWTAttributeEditMode::~UOWTAttributeEditMode() {}
// ********** End Class UOWTAttributeEditMode ******************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_OWTAttributeEditMode_h__Script_VTBOWTEditor_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FOWTRegisteredTool::StaticStruct, Z_Construct_UScriptStruct_FOWTRegisteredTool_Statics::NewStructOps, TEXT("OWTRegisteredTool"),&Z_Registration_Info_UScriptStruct_FOWTRegisteredTool, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTRegisteredTool), 69269249U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTAttributeEditMode, UOWTAttributeEditMode::StaticClass, TEXT("UOWTAttributeEditMode"), &Z_Registration_Info_UClass_UOWTAttributeEditMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTAttributeEditMode), 59949426U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_OWTAttributeEditMode_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_OWTAttributeEditMode_h__Script_VTBOWTEditor_3326040403{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_OWTAttributeEditMode_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_OWTAttributeEditMode_h__Script_VTBOWTEditor_Statics::ClassInfo),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_OWTAttributeEditMode_h__Script_VTBOWTEditor_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_OWTAttributeEditMode_h__Script_VTBOWTEditor_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
