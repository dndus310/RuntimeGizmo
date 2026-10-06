// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Events/OWTAttributeTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTAttributeTypes() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FTransform();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UEnum* Z_Construct_UEnum_VTBOWTEditor_EOWTTransformEditPhase();
VTBOWTEDITOR_API UEnum* Z_Construct_UEnum_VTBOWTEditor_EOWTTransformField();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTAttributeSnapshot();
// ********** End Cross Module References **********************************************************

// ********** Begin Enum EOWTTransformField ********************************************************
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EOWTTransformField;
static UEnum* EOWTTransformField_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EOWTTransformField.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EOWTTransformField.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_VTBOWTEditor_EOWTTransformField, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("EOWTTransformField"));
	}
	return Z_Registration_Info_UEnum_EOWTTransformField.OuterSingleton;
}
template<> VTBOWTEDITOR_NON_ATTRIBUTED_API UEnum* StaticEnum<EOWTTransformField>()
{
	return EOWTTransformField_StaticEnum();
}
struct Z_Construct_UEnum_VTBOWTEditor_EOWTTransformField_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "LocationX.Name", "EOWTTransformField::LocationX" },
		{ "LocationY.Name", "EOWTTransformField::LocationY" },
		{ "LocationZ.Name", "EOWTTransformField::LocationZ" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
		{ "RotationPitch.Name", "EOWTTransformField::RotationPitch" },
		{ "RotationRoll.Name", "EOWTTransformField::RotationRoll" },
		{ "RotationYaw.Name", "EOWTTransformField::RotationYaw" },
		{ "ScaleX.Name", "EOWTTransformField::ScaleX" },
		{ "ScaleY.Name", "EOWTTransformField::ScaleY" },
		{ "ScaleZ.Name", "EOWTTransformField::ScaleZ" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EOWTTransformField::LocationX", (int64)EOWTTransformField::LocationX },
		{ "EOWTTransformField::LocationY", (int64)EOWTTransformField::LocationY },
		{ "EOWTTransformField::LocationZ", (int64)EOWTTransformField::LocationZ },
		{ "EOWTTransformField::RotationRoll", (int64)EOWTTransformField::RotationRoll },
		{ "EOWTTransformField::RotationPitch", (int64)EOWTTransformField::RotationPitch },
		{ "EOWTTransformField::RotationYaw", (int64)EOWTTransformField::RotationYaw },
		{ "EOWTTransformField::ScaleX", (int64)EOWTTransformField::ScaleX },
		{ "EOWTTransformField::ScaleY", (int64)EOWTTransformField::ScaleY },
		{ "EOWTTransformField::ScaleZ", (int64)EOWTTransformField::ScaleZ },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
}; // struct Z_Construct_UEnum_VTBOWTEditor_EOWTTransformField_Statics 
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_VTBOWTEditor_EOWTTransformField_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	"EOWTTransformField",
	"EOWTTransformField",
	Z_Construct_UEnum_VTBOWTEditor_EOWTTransformField_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_VTBOWTEditor_EOWTTransformField_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_VTBOWTEditor_EOWTTransformField_Statics::Enum_MetaDataParams), Z_Construct_UEnum_VTBOWTEditor_EOWTTransformField_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_VTBOWTEditor_EOWTTransformField()
{
	if (!Z_Registration_Info_UEnum_EOWTTransformField.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EOWTTransformField.InnerSingleton, Z_Construct_UEnum_VTBOWTEditor_EOWTTransformField_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EOWTTransformField.InnerSingleton;
}
// ********** End Enum EOWTTransformField **********************************************************

// ********** Begin Enum EOWTTransformEditPhase ****************************************************
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EOWTTransformEditPhase;
static UEnum* EOWTTransformEditPhase_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EOWTTransformEditPhase.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EOWTTransformEditPhase.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_VTBOWTEditor_EOWTTransformEditPhase, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("EOWTTransformEditPhase"));
	}
	return Z_Registration_Info_UEnum_EOWTTransformEditPhase.OuterSingleton;
}
template<> VTBOWTEDITOR_NON_ATTRIBUTED_API UEnum* StaticEnum<EOWTTransformEditPhase>()
{
	return EOWTTransformEditPhase_StaticEnum();
}
struct Z_Construct_UEnum_VTBOWTEditor_EOWTTransformEditPhase_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "Begin.Name", "EOWTTransformEditPhase::Begin" },
		{ "BlueprintType", "true" },
		{ "Cancel.Name", "EOWTTransformEditPhase::Cancel" },
		{ "Commit.Name", "EOWTTransformEditPhase::Commit" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
		{ "Update.Name", "EOWTTransformEditPhase::Update" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "EOWTTransformEditPhase::Begin", (int64)EOWTTransformEditPhase::Begin },
		{ "EOWTTransformEditPhase::Update", (int64)EOWTTransformEditPhase::Update },
		{ "EOWTTransformEditPhase::Commit", (int64)EOWTTransformEditPhase::Commit },
		{ "EOWTTransformEditPhase::Cancel", (int64)EOWTTransformEditPhase::Cancel },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
}; // struct Z_Construct_UEnum_VTBOWTEditor_EOWTTransformEditPhase_Statics 
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_VTBOWTEditor_EOWTTransformEditPhase_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	"EOWTTransformEditPhase",
	"EOWTTransformEditPhase",
	Z_Construct_UEnum_VTBOWTEditor_EOWTTransformEditPhase_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_VTBOWTEditor_EOWTTransformEditPhase_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::EnumClass,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_VTBOWTEditor_EOWTTransformEditPhase_Statics::Enum_MetaDataParams), Z_Construct_UEnum_VTBOWTEditor_EOWTTransformEditPhase_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_VTBOWTEditor_EOWTTransformEditPhase()
{
	if (!Z_Registration_Info_UEnum_EOWTTransformEditPhase.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EOWTTransformEditPhase.InnerSingleton, Z_Construct_UEnum_VTBOWTEditor_EOWTTransformEditPhase_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EOWTTransformEditPhase.InnerSingleton;
}
// ********** End Enum EOWTTransformEditPhase ******************************************************

// ********** Begin ScriptStruct FOWTAttributeSnapshot *********************************************
struct Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTAttributeSnapshot); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTAttributeSnapshot); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EditorId_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectId_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectName_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ObjectClass_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DisabledReason_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ActiveMode_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GizmoMode_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GizmoCoordinateSystem_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SelectionRevision_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StateRevision_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bEditingEnabled_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bHasSelection_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bCanEditTransform_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bIsModifying_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bHasChanges_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Transform_MetaData[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/Events/OWTAttributeTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTAttributeSnapshot constinit property declarations *************
	static const UECodeGen_Private::FStrPropertyParams NewProp_EditorId;
	static const UECodeGen_Private::FStrPropertyParams NewProp_ObjectId;
	static const UECodeGen_Private::FStrPropertyParams NewProp_ObjectName;
	static const UECodeGen_Private::FStrPropertyParams NewProp_ObjectClass;
	static const UECodeGen_Private::FStrPropertyParams NewProp_DisabledReason;
	static const UECodeGen_Private::FStrPropertyParams NewProp_ActiveMode;
	static const UECodeGen_Private::FStrPropertyParams NewProp_GizmoMode;
	static const UECodeGen_Private::FStrPropertyParams NewProp_GizmoCoordinateSystem;
	static const UECodeGen_Private::FIntPropertyParams NewProp_SelectionRevision;
	static const UECodeGen_Private::FIntPropertyParams NewProp_StateRevision;
	static void NewProp_bEditingEnabled_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEditingEnabled;
	static void NewProp_bHasSelection_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bHasSelection;
	static void NewProp_bCanEditTransform_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bCanEditTransform;
	static void NewProp_bIsModifying_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bIsModifying;
	static void NewProp_bHasChanges_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bHasChanges;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Transform;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTAttributeSnapshot constinit property declarations ***************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTAttributeSnapshot>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTAttributeSnapshot;
class UScriptStruct* FOWTAttributeSnapshot::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTAttributeSnapshot.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTAttributeSnapshot.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTAttributeSnapshot, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTAttributeSnapshot"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTAttributeSnapshot.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTAttributeSnapshot Property Definitions ************************
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_EditorId = { "EditorId", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeSnapshot, EditorId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EditorId_MetaData), NewProp_EditorId_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_ObjectId = { "ObjectId", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeSnapshot, ObjectId), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectId_MetaData), NewProp_ObjectId_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_ObjectName = { "ObjectName", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeSnapshot, ObjectName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectName_MetaData), NewProp_ObjectName_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_ObjectClass = { "ObjectClass", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeSnapshot, ObjectClass), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ObjectClass_MetaData), NewProp_ObjectClass_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_DisabledReason = { "DisabledReason", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeSnapshot, DisabledReason), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DisabledReason_MetaData), NewProp_DisabledReason_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_ActiveMode = { "ActiveMode", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeSnapshot, ActiveMode), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ActiveMode_MetaData), NewProp_ActiveMode_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_GizmoMode = { "GizmoMode", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeSnapshot, GizmoMode), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GizmoMode_MetaData), NewProp_GizmoMode_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_GizmoCoordinateSystem = { "GizmoCoordinateSystem", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeSnapshot, GizmoCoordinateSystem), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GizmoCoordinateSystem_MetaData), NewProp_GizmoCoordinateSystem_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_SelectionRevision = { "SelectionRevision", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeSnapshot, SelectionRevision), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SelectionRevision_MetaData), NewProp_SelectionRevision_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_StateRevision = { "StateRevision", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeSnapshot, StateRevision), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StateRevision_MetaData), NewProp_StateRevision_MetaData) };
void Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bEditingEnabled_SetBit(void* Obj)
{
	((FOWTAttributeSnapshot*)Obj)->bEditingEnabled = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bEditingEnabled = { "bEditingEnabled", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTAttributeSnapshot), &Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bEditingEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bEditingEnabled_MetaData), NewProp_bEditingEnabled_MetaData) };
void Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bHasSelection_SetBit(void* Obj)
{
	((FOWTAttributeSnapshot*)Obj)->bHasSelection = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bHasSelection = { "bHasSelection", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTAttributeSnapshot), &Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bHasSelection_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bHasSelection_MetaData), NewProp_bHasSelection_MetaData) };
void Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bCanEditTransform_SetBit(void* Obj)
{
	((FOWTAttributeSnapshot*)Obj)->bCanEditTransform = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bCanEditTransform = { "bCanEditTransform", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTAttributeSnapshot), &Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bCanEditTransform_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bCanEditTransform_MetaData), NewProp_bCanEditTransform_MetaData) };
void Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bIsModifying_SetBit(void* Obj)
{
	((FOWTAttributeSnapshot*)Obj)->bIsModifying = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bIsModifying = { "bIsModifying", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTAttributeSnapshot), &Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bIsModifying_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bIsModifying_MetaData), NewProp_bIsModifying_MetaData) };
void Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bHasChanges_SetBit(void* Obj)
{
	((FOWTAttributeSnapshot*)Obj)->bHasChanges = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bHasChanges = { "bHasChanges", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTAttributeSnapshot), &Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bHasChanges_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bHasChanges_MetaData), NewProp_bHasChanges_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_Transform = { "Transform", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTAttributeSnapshot, Transform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Transform_MetaData), NewProp_Transform_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_EditorId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_ObjectId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_ObjectName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_ObjectClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_DisabledReason,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_ActiveMode,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_GizmoMode,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_GizmoCoordinateSystem,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_SelectionRevision,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_StateRevision,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bEditingEnabled,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bHasSelection,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bCanEditTransform,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bIsModifying,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_bHasChanges,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewProp_Transform,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTAttributeSnapshot Property Definitions **************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTAttributeSnapshot",
	Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::PropPointers),
	sizeof(FOWTAttributeSnapshot),
	alignof(FOWTAttributeSnapshot),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTAttributeSnapshot()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTAttributeSnapshot.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTAttributeSnapshot.InnerSingleton, Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTAttributeSnapshot.InnerSingleton);
}
// ********** End ScriptStruct FOWTAttributeSnapshot ***********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Events_OWTAttributeTypes_h__Script_VTBOWTEditor_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EOWTTransformField_StaticEnum, TEXT("EOWTTransformField"), &Z_Registration_Info_UEnum_EOWTTransformField, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 839127366U) },
		{ EOWTTransformEditPhase_StaticEnum, TEXT("EOWTTransformEditPhase"), &Z_Registration_Info_UEnum_EOWTTransformEditPhase, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1024194901U) },
	};
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FOWTAttributeSnapshot::StaticStruct, Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics::NewStructOps, TEXT("OWTAttributeSnapshot"),&Z_Registration_Info_UScriptStruct_FOWTAttributeSnapshot, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTAttributeSnapshot), 1151875563U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Events_OWTAttributeTypes_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Events_OWTAttributeTypes_h__Script_VTBOWTEditor_1208783835{
	TEXT("/Script/VTBOWTEditor"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Events_OWTAttributeTypes_h__Script_VTBOWTEditor_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Events_OWTAttributeTypes_h__Script_VTBOWTEditor_Statics::ScriptStructInfo),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Events_OWTAttributeTypes_h__Script_VTBOWTEditor_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Events_OWTAttributeTypes_h__Script_VTBOWTEditor_Statics::EnumInfo),
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
