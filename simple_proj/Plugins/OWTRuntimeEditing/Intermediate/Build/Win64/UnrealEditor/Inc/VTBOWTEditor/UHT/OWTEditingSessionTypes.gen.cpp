// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "State/OWTEditingSessionTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTEditingSessionTypes() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FGuid();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UActorComponent_NoRegister();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UEnum* Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationPhase();
VTBOWTEDITOR_API UEnum* Z_Construct_UEnum_VTBOWTEditor_EOWTProceduralState();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTModeSnapshot();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot();
// ********** End Cross Module References **********************************************************

// ********** Begin Enum EOWTDuplicationPhase ******************************************************
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EOWTDuplicationPhase;
static UEnum* EOWTDuplicationPhase_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EOWTDuplicationPhase.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EOWTDuplicationPhase.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationPhase, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("EOWTDuplicationPhase"));
	}
	return Z_Registration_Info_UEnum_EOWTDuplicationPhase.OuterSingleton;
}
template<> VTBOWTEDITOR_NON_ATTRIBUTED_API UEnum* StaticEnum<EOWTDuplicationPhase>()
{
	return EOWTDuplicationPhase_StaticEnum();
}
struct Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationPhase_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Accepted.Name", "EOWTDuplicationPhase::Accepted" },
		{ "BlueprintType", "true" },
		{ "Cancelled.Name", "EOWTDuplicationPhase::Cancelled" },
		{ "CleaningUp.Name", "EOWTDuplicationPhase::CleaningUp" },
		{ "Committed.Name", "EOWTDuplicationPhase::Committed" },
		{ "Failed.Name", "EOWTDuplicationPhase::Failed" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
		{ "Planning.Name", "EOWTDuplicationPhase::Planning" },
		{ "Restoring.Name", "EOWTDuplicationPhase::Restoring" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EOWTDuplicationPhase::Accepted", (int64)EOWTDuplicationPhase::Accepted },
		{ "EOWTDuplicationPhase::Planning", (int64)EOWTDuplicationPhase::Planning },
		{ "EOWTDuplicationPhase::Restoring", (int64)EOWTDuplicationPhase::Restoring },
		{ "EOWTDuplicationPhase::Committed", (int64)EOWTDuplicationPhase::Committed },
		{ "EOWTDuplicationPhase::Failed", (int64)EOWTDuplicationPhase::Failed },
		{ "EOWTDuplicationPhase::Cancelled", (int64)EOWTDuplicationPhase::Cancelled },
		{ "EOWTDuplicationPhase::CleaningUp", (int64)EOWTDuplicationPhase::CleaningUp },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
}; // struct Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationPhase_Statics 
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationPhase_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	"EOWTDuplicationPhase",
	"EOWTDuplicationPhase",
	Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationPhase_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationPhase_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationPhase_Statics::Enum_MetaDataParams), Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationPhase_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationPhase()
{
	if (!Z_Registration_Info_UEnum_EOWTDuplicationPhase.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EOWTDuplicationPhase.InnerSingleton, Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationPhase_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EOWTDuplicationPhase.InnerSingleton;
}
// ********** End Enum EOWTDuplicationPhase ********************************************************

// ********** Begin Enum EOWTProceduralState *******************************************************
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EOWTProceduralState;
static UEnum* EOWTProceduralState_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EOWTProceduralState.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EOWTProceduralState.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_VTBOWTEditor_EOWTProceduralState, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("EOWTProceduralState"));
	}
	return Z_Registration_Info_UEnum_EOWTProceduralState.OuterSingleton;
}
template<> VTBOWTEDITOR_NON_ATTRIBUTED_API UEnum* StaticEnum<EOWTProceduralState>()
{
	return EOWTProceduralState_StaticEnum();
}
struct Z_Construct_UEnum_VTBOWTEditor_EOWTProceduralState_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Cancelled.Name", "EOWTProceduralState::Cancelled" },
		{ "Cleaned.Name", "EOWTProceduralState::Cleaned" },
		{ "CleaningUp.Name", "EOWTProceduralState::CleaningUp" },
		{ "Failed.Name", "EOWTProceduralState::Failed" },
		{ "Generating.Name", "EOWTProceduralState::Generating" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
		{ "NotRequested.Name", "EOWTProceduralState::NotRequested" },
		{ "Ready.Name", "EOWTProceduralState::Ready" },
		{ "Scheduled.Name", "EOWTProceduralState::Scheduled" },
		{ "WaitingForGenerationSource.Name", "EOWTProceduralState::WaitingForGenerationSource" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EOWTProceduralState::NotRequested", (int64)EOWTProceduralState::NotRequested },
		{ "EOWTProceduralState::Scheduled", (int64)EOWTProceduralState::Scheduled },
		{ "EOWTProceduralState::WaitingForGenerationSource", (int64)EOWTProceduralState::WaitingForGenerationSource },
		{ "EOWTProceduralState::Generating", (int64)EOWTProceduralState::Generating },
		{ "EOWTProceduralState::Ready", (int64)EOWTProceduralState::Ready },
		{ "EOWTProceduralState::Failed", (int64)EOWTProceduralState::Failed },
		{ "EOWTProceduralState::Cancelled", (int64)EOWTProceduralState::Cancelled },
		{ "EOWTProceduralState::Cleaned", (int64)EOWTProceduralState::Cleaned },
		{ "EOWTProceduralState::CleaningUp", (int64)EOWTProceduralState::CleaningUp },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
}; // struct Z_Construct_UEnum_VTBOWTEditor_EOWTProceduralState_Statics 
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_VTBOWTEditor_EOWTProceduralState_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	"EOWTProceduralState",
	"EOWTProceduralState",
	Z_Construct_UEnum_VTBOWTEditor_EOWTProceduralState_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_VTBOWTEditor_EOWTProceduralState_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_VTBOWTEditor_EOWTProceduralState_Statics::Enum_MetaDataParams), Z_Construct_UEnum_VTBOWTEditor_EOWTProceduralState_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_VTBOWTEditor_EOWTProceduralState()
{
	if (!Z_Registration_Info_UEnum_EOWTProceduralState.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EOWTProceduralState.InnerSingleton, Z_Construct_UEnum_VTBOWTEditor_EOWTProceduralState_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EOWTProceduralState.InnerSingleton;
}
// ********** End Enum EOWTProceduralState *********************************************************

// ********** Begin ScriptStruct FOWTModeSnapshot **************************************************
struct Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTModeSnapshot); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTModeSnapshot); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Observed mode/tool state. It is independent of Actor attributes and JSON delivery. */" },
#endif
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Observed mode/tool state. It is independent of Actor attributes and JSON delivery." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ModeId_MetaData[] = {
		{ "Category", "OWT|Mode" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ActiveToolId_MetaData[] = {
		{ "Category", "OWT|Mode" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Lifecycle_MetaData[] = {
		{ "Category", "OWT|Mode" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DisabledReason_MetaData[] = {
		{ "Category", "OWT|Mode" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Revision_MetaData[] = {
		{ "Category", "OWT|Mode" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEditingEnabled_MetaData[] = {
		{ "Category", "OWT|Mode" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanStartTools_MetaData[] = {
		{ "Category", "OWT|Mode" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTModeSnapshot constinit property declarations ******************
	static const UECodeGen_Private::FNamePropertyParams NewProp_ModeId;
	static const UECodeGen_Private::FNamePropertyParams NewProp_ActiveToolId;
	static const UECodeGen_Private::FNamePropertyParams NewProp_Lifecycle;
	static const UECodeGen_Private::FStrPropertyParams NewProp_DisabledReason;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Revision;
	static void NewProp_bEditingEnabled_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEditingEnabled;
	static void NewProp_bCanStartTools_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanStartTools;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTModeSnapshot constinit property declarations ********************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTModeSnapshot>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTModeSnapshot;
class UScriptStruct* FOWTModeSnapshot::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTModeSnapshot.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTModeSnapshot.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTModeSnapshot, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTModeSnapshot"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTModeSnapshot.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTModeSnapshot Property Definitions *****************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_ModeId = { "ModeId", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTModeSnapshot, ModeId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ModeId_MetaData), NewProp_ModeId_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_ActiveToolId = { "ActiveToolId", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTModeSnapshot, ActiveToolId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ActiveToolId_MetaData), NewProp_ActiveToolId_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_Lifecycle = { "Lifecycle", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTModeSnapshot, Lifecycle), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Lifecycle_MetaData), NewProp_Lifecycle_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_DisabledReason = { "DisabledReason", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTModeSnapshot, DisabledReason), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DisabledReason_MetaData), NewProp_DisabledReason_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_Revision = { "Revision", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTModeSnapshot, Revision), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Revision_MetaData), NewProp_Revision_MetaData) };
void Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_bEditingEnabled_SetBit(void* Obj)
{
	((FOWTModeSnapshot*)Obj)->bEditingEnabled = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_bEditingEnabled = { "bEditingEnabled", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTModeSnapshot), &Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_bEditingEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEditingEnabled_MetaData), NewProp_bEditingEnabled_MetaData) };
void Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_bCanStartTools_SetBit(void* Obj)
{
	((FOWTModeSnapshot*)Obj)->bCanStartTools = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_bCanStartTools = { "bCanStartTools", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTModeSnapshot), &Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_bCanStartTools_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanStartTools_MetaData), NewProp_bCanStartTools_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_ModeId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_ActiveToolId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_Lifecycle,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_DisabledReason,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_Revision,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_bEditingEnabled,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewProp_bCanStartTools,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTModeSnapshot Property Definitions *******************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTModeSnapshot",
	Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::PropPointers),
	sizeof(FOWTModeSnapshot),
	alignof(FOWTModeSnapshot),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTModeSnapshot()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTModeSnapshot.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTModeSnapshot.InnerSingleton, Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTModeSnapshot.InnerSingleton);
}
// ********** End ScriptStruct FOWTModeSnapshot ****************************************************

// ********** Begin ScriptStruct FOWTDuplicationOperationSnapshot **********************************
struct Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTDuplicationOperationSnapshot); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTDuplicationOperationSnapshot); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SourceActor_MetaData[] = {
		{ "Category", "OWT|Duplication" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DuplicateActor_MetaData[] = {
		{ "Category", "OWT|Duplication" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OperationId_MetaData[] = {
		{ "Category", "OWT|Duplication" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RequestId_MetaData[] = {
		{ "Category", "OWT|Duplication" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Source_MetaData[] = {
		{ "Category", "OWT|Duplication" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OriginalObjectId_MetaData[] = {
		{ "Category", "OWT|Duplication" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DuplicateObjectId_MetaData[] = {
		{ "Category", "OWT|Duplication" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_HierarchyScope_MetaData[] = {
		{ "Category", "OWT|Duplication" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Phase_MetaData[] = {
		{ "Category", "OWT|Duplication" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Error_MetaData[] = {
		{ "Category", "OWT|Duplication" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Revision_MetaData[] = {
		{ "Category", "OWT|Duplication" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTDuplicationOperationSnapshot constinit property declarations **
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_SourceActor;
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_DuplicateActor;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OperationId;
	static const UECodeGen_Private::FStrPropertyParams NewProp_RequestId;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Source;
	static const UECodeGen_Private::FStrPropertyParams NewProp_OriginalObjectId;
	static const UECodeGen_Private::FStrPropertyParams NewProp_DuplicateObjectId;
	static const UECodeGen_Private::FNamePropertyParams NewProp_HierarchyScope;
	static const UECodeGen_Private::FBytePropertyParams NewProp_Phase_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_Phase;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Error;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Revision;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTDuplicationOperationSnapshot constinit property declarations ****
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTDuplicationOperationSnapshot>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTDuplicationOperationSnapshot;
class UScriptStruct* FOWTDuplicationOperationSnapshot::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTDuplicationOperationSnapshot.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTDuplicationOperationSnapshot.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTDuplicationOperationSnapshot"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTDuplicationOperationSnapshot.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTDuplicationOperationSnapshot Property Definitions *************
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_SourceActor = { "SourceActor", nullptr, (EPropertyFlags)0x0014000000000014, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTDuplicationOperationSnapshot, SourceActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SourceActor_MetaData), NewProp_SourceActor_MetaData) };
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_DuplicateActor = { "DuplicateActor", nullptr, (EPropertyFlags)0x0014000000000014, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTDuplicationOperationSnapshot, DuplicateActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DuplicateActor_MetaData), NewProp_DuplicateActor_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_OperationId = { "OperationId", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTDuplicationOperationSnapshot, OperationId), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OperationId_MetaData), NewProp_OperationId_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_RequestId = { "RequestId", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTDuplicationOperationSnapshot, RequestId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RequestId_MetaData), NewProp_RequestId_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_Source = { "Source", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTDuplicationOperationSnapshot, Source), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Source_MetaData), NewProp_Source_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_OriginalObjectId = { "OriginalObjectId", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTDuplicationOperationSnapshot, OriginalObjectId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OriginalObjectId_MetaData), NewProp_OriginalObjectId_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_DuplicateObjectId = { "DuplicateObjectId", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTDuplicationOperationSnapshot, DuplicateObjectId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DuplicateObjectId_MetaData), NewProp_DuplicateObjectId_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_HierarchyScope = { "HierarchyScope", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTDuplicationOperationSnapshot, HierarchyScope), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_HierarchyScope_MetaData), NewProp_HierarchyScope_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_Phase_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_Phase = { "Phase", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTDuplicationOperationSnapshot, Phase), Z_Construct_UEnum_VTBOWTEditor_EOWTDuplicationPhase, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Phase_MetaData), NewProp_Phase_MetaData) }; // 924638324
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_Error = { "Error", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTDuplicationOperationSnapshot, Error), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Error_MetaData), NewProp_Error_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_Revision = { "Revision", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTDuplicationOperationSnapshot, Revision), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Revision_MetaData), NewProp_Revision_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_SourceActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_DuplicateActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_OperationId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_RequestId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_Source,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_OriginalObjectId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_DuplicateObjectId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_HierarchyScope,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_Phase_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_Phase,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_Error,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewProp_Revision,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTDuplicationOperationSnapshot Property Definitions ***************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTDuplicationOperationSnapshot",
	Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::PropPointers),
	sizeof(FOWTDuplicationOperationSnapshot),
	alignof(FOWTDuplicationOperationSnapshot),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTDuplicationOperationSnapshot.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTDuplicationOperationSnapshot.InnerSingleton, Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTDuplicationOperationSnapshot.InnerSingleton);
}
// ********** End ScriptStruct FOWTDuplicationOperationSnapshot ************************************

// ********** Begin ScriptStruct FOWTProceduralComponentSnapshot ***********************************
struct Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTProceduralComponentSnapshot); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTProceduralComponentSnapshot); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[] = {
		{ "Category", "OWT|Procedural" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OperationId_MetaData[] = {
		{ "Category", "OWT|Procedural" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ComponentId_MetaData[] = {
		{ "Category", "OWT|Procedural" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ComponentName_MetaData[] = {
		{ "Category", "OWT|Procedural" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GraphPath_MetaData[] = {
		{ "Category", "OWT|Procedural" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Trigger_MetaData[] = {
		{ "Category", "OWT|Procedural" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_State_MetaData[] = {
		{ "Category", "OWT|Procedural" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GenerationAttempt_MetaData[] = {
		{ "Category", "OWT|Procedural" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Reason_MetaData[] = {
		{ "Category", "OWT|Procedural" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Revision_MetaData[] = {
		{ "Category", "OWT|Procedural" },
		{ "ModuleRelativePath", "Public/State/OWTEditingSessionTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTProceduralComponentSnapshot constinit property declarations ***
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_Component;
	static const UECodeGen_Private::FStructPropertyParams NewProp_OperationId;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ComponentId;
	static const UECodeGen_Private::FStrPropertyParams NewProp_ComponentName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_GraphPath;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Trigger;
	static const UECodeGen_Private::FBytePropertyParams NewProp_State_Underlying;
	static const UECodeGen_Private::FEnumPropertyParams NewProp_State;
	static const UECodeGen_Private::FIntPropertyParams NewProp_GenerationAttempt;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Reason;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Revision;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTProceduralComponentSnapshot constinit property declarations *****
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTProceduralComponentSnapshot>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTProceduralComponentSnapshot;
class UScriptStruct* FOWTProceduralComponentSnapshot::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTProceduralComponentSnapshot.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTProceduralComponentSnapshot.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTProceduralComponentSnapshot"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTProceduralComponentSnapshot.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTProceduralComponentSnapshot Property Definitions **************
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x001400000008001c, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTProceduralComponentSnapshot, Component), Z_Construct_UClass_UActorComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Component_MetaData), NewProp_Component_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_OperationId = { "OperationId", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTProceduralComponentSnapshot, OperationId), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OperationId_MetaData), NewProp_OperationId_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_ComponentId = { "ComponentId", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTProceduralComponentSnapshot, ComponentId), Z_Construct_UScriptStruct_FGuid, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ComponentId_MetaData), NewProp_ComponentId_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_ComponentName = { "ComponentName", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTProceduralComponentSnapshot, ComponentName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ComponentName_MetaData), NewProp_ComponentName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_GraphPath = { "GraphPath", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTProceduralComponentSnapshot, GraphPath), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GraphPath_MetaData), NewProp_GraphPath_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_Trigger = { "Trigger", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTProceduralComponentSnapshot, Trigger), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Trigger_MetaData), NewProp_Trigger_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_State_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_State = { "State", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTProceduralComponentSnapshot, State), Z_Construct_UEnum_VTBOWTEditor_EOWTProceduralState, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_State_MetaData), NewProp_State_MetaData) }; // 2258199400
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_GenerationAttempt = { "GenerationAttempt", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTProceduralComponentSnapshot, GenerationAttempt), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GenerationAttempt_MetaData), NewProp_GenerationAttempt_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_Reason = { "Reason", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTProceduralComponentSnapshot, Reason), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Reason_MetaData), NewProp_Reason_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_Revision = { "Revision", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTProceduralComponentSnapshot, Revision), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Revision_MetaData), NewProp_Revision_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_Component,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_OperationId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_ComponentId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_ComponentName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_GraphPath,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_Trigger,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_State_Underlying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_State,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_GenerationAttempt,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_Reason,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewProp_Revision,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTProceduralComponentSnapshot Property Definitions ****************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTProceduralComponentSnapshot",
	Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::PropPointers),
	sizeof(FOWTProceduralComponentSnapshot),
	alignof(FOWTProceduralComponentSnapshot),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000205),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTProceduralComponentSnapshot.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTProceduralComponentSnapshot.InnerSingleton, Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTProceduralComponentSnapshot.InnerSingleton);
}
// ********** End ScriptStruct FOWTProceduralComponentSnapshot *************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTEditingSessionTypes_h__Script_VTBOWTEditor_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EOWTDuplicationPhase_StaticEnum, TEXT("EOWTDuplicationPhase"), &Z_Registration_Info_UEnum_EOWTDuplicationPhase, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 924638324U) },
		{ EOWTProceduralState_StaticEnum, TEXT("EOWTProceduralState"), &Z_Registration_Info_UEnum_EOWTProceduralState, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 2258199400U) },
	};
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FOWTModeSnapshot::StaticStruct, Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics::NewStructOps, TEXT("OWTModeSnapshot"),&Z_Registration_Info_UScriptStruct_FOWTModeSnapshot, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTModeSnapshot), 1626674559U) },
		{ FOWTDuplicationOperationSnapshot::StaticStruct, Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics::NewStructOps, TEXT("OWTDuplicationOperationSnapshot"),&Z_Registration_Info_UScriptStruct_FOWTDuplicationOperationSnapshot, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTDuplicationOperationSnapshot), 2137892919U) },
		{ FOWTProceduralComponentSnapshot::StaticStruct, Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics::NewStructOps, TEXT("OWTProceduralComponentSnapshot"),&Z_Registration_Info_UScriptStruct_FOWTProceduralComponentSnapshot, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTProceduralComponentSnapshot), 3712393560U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTEditingSessionTypes_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTEditingSessionTypes_h__Script_VTBOWTEditor_1461173068{
	TEXT("/Script/VTBOWTEditor"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTEditingSessionTypes_h__Script_VTBOWTEditor_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTEditingSessionTypes_h__Script_VTBOWTEditor_Statics::ScriptStructInfo),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTEditingSessionTypes_h__Script_VTBOWTEditor_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTEditingSessionTypes_h__Script_VTBOWTEditor_Statics::EnumInfo),
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
