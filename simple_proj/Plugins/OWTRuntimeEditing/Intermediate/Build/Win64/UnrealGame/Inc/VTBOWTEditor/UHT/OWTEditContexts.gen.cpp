// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Context/OWTEditContexts.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTEditContexts() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTDuplicateSelectionContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTGizmoPointerContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTHideSelectionGizmoContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTRedoContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTSelectObjectContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTSetRotationContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTSetScaleContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTSetTranslationContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTToggleCoordinateSystemContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTToggleEditingContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTToggleTransformSplineContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTUndoContext();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FOWTSelectObjectContext *******************************************
struct Z_Construct_UScriptStruct_FOWTSelectObjectContext_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTSelectObjectContext); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTSelectObjectContext); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SelectedObject_MetaData[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTSelectObjectContext constinit property declarations ***********
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_SelectedObject;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTSelectObjectContext constinit property declarations *************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTSelectObjectContext>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTSelectObjectContext_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTSelectObjectContext;
class UScriptStruct* FOWTSelectObjectContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTSelectObjectContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTSelectObjectContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTSelectObjectContext, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTSelectObjectContext"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTSelectObjectContext.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTSelectObjectContext Property Definitions **********************
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UScriptStruct_FOWTSelectObjectContext_Statics::NewProp_SelectedObject = { "SelectedObject", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTSelectObjectContext, SelectedObject), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SelectedObject_MetaData), NewProp_SelectedObject_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTSelectObjectContext_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTSelectObjectContext_Statics::NewProp_SelectedObject,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTSelectObjectContext_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTSelectObjectContext Property Definitions ************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTSelectObjectContext_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTSelectObjectContext",
	Z_Construct_UScriptStruct_FOWTSelectObjectContext_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTSelectObjectContext_Statics::PropPointers),
	sizeof(FOWTSelectObjectContext),
	alignof(FOWTSelectObjectContext),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTSelectObjectContext_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTSelectObjectContext_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTSelectObjectContext()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTSelectObjectContext.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTSelectObjectContext.InnerSingleton, Z_Construct_UScriptStruct_FOWTSelectObjectContext_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTSelectObjectContext.InnerSingleton);
}
// ********** End ScriptStruct FOWTSelectObjectContext *********************************************

// ********** Begin ScriptStruct FOWTUndoContext ***************************************************
struct Z_Construct_UScriptStruct_FOWTUndoContext_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTUndoContext); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTUndoContext); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTUndoContext constinit property declarations *******************
// ********** End ScriptStruct FOWTUndoContext constinit property declarations *********************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTUndoContext>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTUndoContext_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTUndoContext;
class UScriptStruct* FOWTUndoContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTUndoContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTUndoContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTUndoContext, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTUndoContext"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTUndoContext.OuterSingleton;
	}
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTUndoContext_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTUndoContext",
	nullptr,
	0,
	sizeof(FOWTUndoContext),
	alignof(FOWTUndoContext),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTUndoContext_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTUndoContext_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTUndoContext()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTUndoContext.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTUndoContext.InnerSingleton, Z_Construct_UScriptStruct_FOWTUndoContext_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTUndoContext.InnerSingleton);
}
// ********** End ScriptStruct FOWTUndoContext *****************************************************

// ********** Begin ScriptStruct FOWTRedoContext ***************************************************
struct Z_Construct_UScriptStruct_FOWTRedoContext_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTRedoContext); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTRedoContext); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTRedoContext constinit property declarations *******************
// ********** End ScriptStruct FOWTRedoContext constinit property declarations *********************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTRedoContext>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTRedoContext_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTRedoContext;
class UScriptStruct* FOWTRedoContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTRedoContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTRedoContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTRedoContext, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTRedoContext"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTRedoContext.OuterSingleton;
	}
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTRedoContext_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTRedoContext",
	nullptr,
	0,
	sizeof(FOWTRedoContext),
	alignof(FOWTRedoContext),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTRedoContext_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTRedoContext_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTRedoContext()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTRedoContext.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTRedoContext.InnerSingleton, Z_Construct_UScriptStruct_FOWTRedoContext_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTRedoContext.InnerSingleton);
}
// ********** End ScriptStruct FOWTRedoContext *****************************************************

// ********** Begin ScriptStruct FOWTToggleCoordinateSystemContext *********************************
struct Z_Construct_UScriptStruct_FOWTToggleCoordinateSystemContext_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTToggleCoordinateSystemContext); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTToggleCoordinateSystemContext); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTToggleCoordinateSystemContext constinit property declarations *
// ********** End ScriptStruct FOWTToggleCoordinateSystemContext constinit property declarations ***
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTToggleCoordinateSystemContext>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTToggleCoordinateSystemContext_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTToggleCoordinateSystemContext;
class UScriptStruct* FOWTToggleCoordinateSystemContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTToggleCoordinateSystemContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTToggleCoordinateSystemContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTToggleCoordinateSystemContext, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTToggleCoordinateSystemContext"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTToggleCoordinateSystemContext.OuterSingleton;
	}
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTToggleCoordinateSystemContext_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTToggleCoordinateSystemContext",
	nullptr,
	0,
	sizeof(FOWTToggleCoordinateSystemContext),
	alignof(FOWTToggleCoordinateSystemContext),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTToggleCoordinateSystemContext_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTToggleCoordinateSystemContext_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTToggleCoordinateSystemContext()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTToggleCoordinateSystemContext.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTToggleCoordinateSystemContext.InnerSingleton, Z_Construct_UScriptStruct_FOWTToggleCoordinateSystemContext_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTToggleCoordinateSystemContext.InnerSingleton);
}
// ********** End ScriptStruct FOWTToggleCoordinateSystemContext ***********************************

// ********** Begin ScriptStruct FOWTToggleTransformSplineContext **********************************
struct Z_Construct_UScriptStruct_FOWTToggleTransformSplineContext_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTToggleTransformSplineContext); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTToggleTransformSplineContext); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTToggleTransformSplineContext constinit property declarations **
// ********** End ScriptStruct FOWTToggleTransformSplineContext constinit property declarations ****
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTToggleTransformSplineContext>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTToggleTransformSplineContext_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTToggleTransformSplineContext;
class UScriptStruct* FOWTToggleTransformSplineContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTToggleTransformSplineContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTToggleTransformSplineContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTToggleTransformSplineContext, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTToggleTransformSplineContext"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTToggleTransformSplineContext.OuterSingleton;
	}
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTToggleTransformSplineContext_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTToggleTransformSplineContext",
	nullptr,
	0,
	sizeof(FOWTToggleTransformSplineContext),
	alignof(FOWTToggleTransformSplineContext),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTToggleTransformSplineContext_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTToggleTransformSplineContext_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTToggleTransformSplineContext()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTToggleTransformSplineContext.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTToggleTransformSplineContext.InnerSingleton, Z_Construct_UScriptStruct_FOWTToggleTransformSplineContext_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTToggleTransformSplineContext.InnerSingleton);
}
// ********** End ScriptStruct FOWTToggleTransformSplineContext ************************************

// ********** Begin ScriptStruct FOWTSetTranslationContext *****************************************
struct Z_Construct_UScriptStruct_FOWTSetTranslationContext_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTSetTranslationContext); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTSetTranslationContext); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTSetTranslationContext constinit property declarations *********
// ********** End ScriptStruct FOWTSetTranslationContext constinit property declarations ***********
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTSetTranslationContext>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTSetTranslationContext_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTSetTranslationContext;
class UScriptStruct* FOWTSetTranslationContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTSetTranslationContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTSetTranslationContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTSetTranslationContext, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTSetTranslationContext"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTSetTranslationContext.OuterSingleton;
	}
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTSetTranslationContext_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTSetTranslationContext",
	nullptr,
	0,
	sizeof(FOWTSetTranslationContext),
	alignof(FOWTSetTranslationContext),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTSetTranslationContext_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTSetTranslationContext_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTSetTranslationContext()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTSetTranslationContext.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTSetTranslationContext.InnerSingleton, Z_Construct_UScriptStruct_FOWTSetTranslationContext_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTSetTranslationContext.InnerSingleton);
}
// ********** End ScriptStruct FOWTSetTranslationContext *******************************************

// ********** Begin ScriptStruct FOWTSetRotationContext ********************************************
struct Z_Construct_UScriptStruct_FOWTSetRotationContext_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTSetRotationContext); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTSetRotationContext); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTSetRotationContext constinit property declarations ************
// ********** End ScriptStruct FOWTSetRotationContext constinit property declarations **************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTSetRotationContext>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTSetRotationContext_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTSetRotationContext;
class UScriptStruct* FOWTSetRotationContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTSetRotationContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTSetRotationContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTSetRotationContext, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTSetRotationContext"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTSetRotationContext.OuterSingleton;
	}
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTSetRotationContext_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTSetRotationContext",
	nullptr,
	0,
	sizeof(FOWTSetRotationContext),
	alignof(FOWTSetRotationContext),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTSetRotationContext_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTSetRotationContext_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTSetRotationContext()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTSetRotationContext.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTSetRotationContext.InnerSingleton, Z_Construct_UScriptStruct_FOWTSetRotationContext_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTSetRotationContext.InnerSingleton);
}
// ********** End ScriptStruct FOWTSetRotationContext **********************************************

// ********** Begin ScriptStruct FOWTSetScaleContext ***********************************************
struct Z_Construct_UScriptStruct_FOWTSetScaleContext_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTSetScaleContext); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTSetScaleContext); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTSetScaleContext constinit property declarations ***************
// ********** End ScriptStruct FOWTSetScaleContext constinit property declarations *****************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTSetScaleContext>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTSetScaleContext_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTSetScaleContext;
class UScriptStruct* FOWTSetScaleContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTSetScaleContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTSetScaleContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTSetScaleContext, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTSetScaleContext"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTSetScaleContext.OuterSingleton;
	}
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTSetScaleContext_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTSetScaleContext",
	nullptr,
	0,
	sizeof(FOWTSetScaleContext),
	alignof(FOWTSetScaleContext),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTSetScaleContext_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTSetScaleContext_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTSetScaleContext()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTSetScaleContext.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTSetScaleContext.InnerSingleton, Z_Construct_UScriptStruct_FOWTSetScaleContext_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTSetScaleContext.InnerSingleton);
}
// ********** End ScriptStruct FOWTSetScaleContext *************************************************

// ********** Begin ScriptStruct FOWTHideSelectionGizmoContext *************************************
struct Z_Construct_UScriptStruct_FOWTHideSelectionGizmoContext_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTHideSelectionGizmoContext); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTHideSelectionGizmoContext); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTHideSelectionGizmoContext constinit property declarations *****
// ********** End ScriptStruct FOWTHideSelectionGizmoContext constinit property declarations *******
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTHideSelectionGizmoContext>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTHideSelectionGizmoContext_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTHideSelectionGizmoContext;
class UScriptStruct* FOWTHideSelectionGizmoContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTHideSelectionGizmoContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTHideSelectionGizmoContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTHideSelectionGizmoContext, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTHideSelectionGizmoContext"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTHideSelectionGizmoContext.OuterSingleton;
	}
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTHideSelectionGizmoContext_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTHideSelectionGizmoContext",
	nullptr,
	0,
	sizeof(FOWTHideSelectionGizmoContext),
	alignof(FOWTHideSelectionGizmoContext),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTHideSelectionGizmoContext_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTHideSelectionGizmoContext_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTHideSelectionGizmoContext()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTHideSelectionGizmoContext.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTHideSelectionGizmoContext.InnerSingleton, Z_Construct_UScriptStruct_FOWTHideSelectionGizmoContext_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTHideSelectionGizmoContext.InnerSingleton);
}
// ********** End ScriptStruct FOWTHideSelectionGizmoContext ***************************************

// ********** Begin ScriptStruct FOWTDuplicateSelectionContext *************************************
struct Z_Construct_UScriptStruct_FOWTDuplicateSelectionContext_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTDuplicateSelectionContext); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTDuplicateSelectionContext); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTDuplicateSelectionContext constinit property declarations *****
// ********** End ScriptStruct FOWTDuplicateSelectionContext constinit property declarations *******
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTDuplicateSelectionContext>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTDuplicateSelectionContext_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTDuplicateSelectionContext;
class UScriptStruct* FOWTDuplicateSelectionContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTDuplicateSelectionContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTDuplicateSelectionContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTDuplicateSelectionContext, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTDuplicateSelectionContext"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTDuplicateSelectionContext.OuterSingleton;
	}
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTDuplicateSelectionContext_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTDuplicateSelectionContext",
	nullptr,
	0,
	sizeof(FOWTDuplicateSelectionContext),
	alignof(FOWTDuplicateSelectionContext),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTDuplicateSelectionContext_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTDuplicateSelectionContext_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTDuplicateSelectionContext()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTDuplicateSelectionContext.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTDuplicateSelectionContext.InnerSingleton, Z_Construct_UScriptStruct_FOWTDuplicateSelectionContext_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTDuplicateSelectionContext.InnerSingleton);
}
// ********** End ScriptStruct FOWTDuplicateSelectionContext ***************************************

// ********** Begin ScriptStruct FOWTToggleEditingContext ******************************************
struct Z_Construct_UScriptStruct_FOWTToggleEditingContext_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTToggleEditingContext); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTToggleEditingContext); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTToggleEditingContext constinit property declarations **********
// ********** End ScriptStruct FOWTToggleEditingContext constinit property declarations ************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTToggleEditingContext>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTToggleEditingContext_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTToggleEditingContext;
class UScriptStruct* FOWTToggleEditingContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTToggleEditingContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTToggleEditingContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTToggleEditingContext, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTToggleEditingContext"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTToggleEditingContext.OuterSingleton;
	}
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTToggleEditingContext_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTToggleEditingContext",
	nullptr,
	0,
	sizeof(FOWTToggleEditingContext),
	alignof(FOWTToggleEditingContext),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTToggleEditingContext_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTToggleEditingContext_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTToggleEditingContext()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTToggleEditingContext.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTToggleEditingContext.InnerSingleton, Z_Construct_UScriptStruct_FOWTToggleEditingContext_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTToggleEditingContext.InnerSingleton);
}
// ********** End ScriptStruct FOWTToggleEditingContext ********************************************

// ********** Begin ScriptStruct FOWTGizmoPointerContext *******************************************
struct Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTGizmoPointerContext); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTGizmoPointerContext); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Semantic pointer state. No input action, mapping context or key identifier crosses the boundary.\n" },
#endif
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Semantic pointer state. No input action, mapping context or key identifier crosses the boundary." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RayOrigin_MetaData[] = {
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RayDirection_MetaData[] = {
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ScreenPosition_MetaData[] = {
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bPressed_MetaData[] = {
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bDown_MetaData[] = {
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bReleased_MetaData[] = {
		{ "ModuleRelativePath", "Public/Context/OWTEditContexts.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTGizmoPointerContext constinit property declarations ***********
	static const UECodeGen_Private::FStructPropertyParams NewProp_RayOrigin;
	static const UECodeGen_Private::FStructPropertyParams NewProp_RayDirection;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ScreenPosition;
	static void NewProp_bPressed_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bPressed;
	static void NewProp_bDown_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bDown;
	static void NewProp_bReleased_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bReleased;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTGizmoPointerContext constinit property declarations *************
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTGizmoPointerContext>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTGizmoPointerContext;
class UScriptStruct* FOWTGizmoPointerContext::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTGizmoPointerContext.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTGizmoPointerContext.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTGizmoPointerContext, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTGizmoPointerContext"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTGizmoPointerContext.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTGizmoPointerContext Property Definitions **********************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_RayOrigin = { "RayOrigin", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTGizmoPointerContext, RayOrigin), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RayOrigin_MetaData), NewProp_RayOrigin_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_RayDirection = { "RayDirection", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTGizmoPointerContext, RayDirection), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RayDirection_MetaData), NewProp_RayDirection_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_ScreenPosition = { "ScreenPosition", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTGizmoPointerContext, ScreenPosition), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ScreenPosition_MetaData), NewProp_ScreenPosition_MetaData) };
void Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_bPressed_SetBit(void* Obj)
{
	((FOWTGizmoPointerContext*)Obj)->bPressed = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_bPressed = { "bPressed", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTGizmoPointerContext), &Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_bPressed_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bPressed_MetaData), NewProp_bPressed_MetaData) };
void Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_bDown_SetBit(void* Obj)
{
	((FOWTGizmoPointerContext*)Obj)->bDown = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_bDown = { "bDown", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTGizmoPointerContext), &Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_bDown_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bDown_MetaData), NewProp_bDown_MetaData) };
void Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_bReleased_SetBit(void* Obj)
{
	((FOWTGizmoPointerContext*)Obj)->bReleased = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_bReleased = { "bReleased", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FOWTGizmoPointerContext), &Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_bReleased_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bReleased_MetaData), NewProp_bReleased_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_RayOrigin,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_RayDirection,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_ScreenPosition,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_bPressed,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_bDown,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewProp_bReleased,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTGizmoPointerContext Property Definitions ************************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTGizmoPointerContext",
	Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::PropPointers),
	sizeof(FOWTGizmoPointerContext),
	alignof(FOWTGizmoPointerContext),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000201),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTGizmoPointerContext()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTGizmoPointerContext.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTGizmoPointerContext.InnerSingleton, Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTGizmoPointerContext.InnerSingleton);
}
// ********** End ScriptStruct FOWTGizmoPointerContext *********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTEditContexts_h__Script_VTBOWTEditor_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FOWTSelectObjectContext::StaticStruct, Z_Construct_UScriptStruct_FOWTSelectObjectContext_Statics::NewStructOps, TEXT("OWTSelectObjectContext"),&Z_Registration_Info_UScriptStruct_FOWTSelectObjectContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTSelectObjectContext), 2535039331U) },
		{ FOWTUndoContext::StaticStruct, Z_Construct_UScriptStruct_FOWTUndoContext_Statics::NewStructOps, TEXT("OWTUndoContext"),&Z_Registration_Info_UScriptStruct_FOWTUndoContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTUndoContext), 2511388460U) },
		{ FOWTRedoContext::StaticStruct, Z_Construct_UScriptStruct_FOWTRedoContext_Statics::NewStructOps, TEXT("OWTRedoContext"),&Z_Registration_Info_UScriptStruct_FOWTRedoContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTRedoContext), 1020009757U) },
		{ FOWTToggleCoordinateSystemContext::StaticStruct, Z_Construct_UScriptStruct_FOWTToggleCoordinateSystemContext_Statics::NewStructOps, TEXT("OWTToggleCoordinateSystemContext"),&Z_Registration_Info_UScriptStruct_FOWTToggleCoordinateSystemContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTToggleCoordinateSystemContext), 1436422854U) },
		{ FOWTToggleTransformSplineContext::StaticStruct, Z_Construct_UScriptStruct_FOWTToggleTransformSplineContext_Statics::NewStructOps, TEXT("OWTToggleTransformSplineContext"),&Z_Registration_Info_UScriptStruct_FOWTToggleTransformSplineContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTToggleTransformSplineContext), 3653071224U) },
		{ FOWTSetTranslationContext::StaticStruct, Z_Construct_UScriptStruct_FOWTSetTranslationContext_Statics::NewStructOps, TEXT("OWTSetTranslationContext"),&Z_Registration_Info_UScriptStruct_FOWTSetTranslationContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTSetTranslationContext), 1555986386U) },
		{ FOWTSetRotationContext::StaticStruct, Z_Construct_UScriptStruct_FOWTSetRotationContext_Statics::NewStructOps, TEXT("OWTSetRotationContext"),&Z_Registration_Info_UScriptStruct_FOWTSetRotationContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTSetRotationContext), 2871991234U) },
		{ FOWTSetScaleContext::StaticStruct, Z_Construct_UScriptStruct_FOWTSetScaleContext_Statics::NewStructOps, TEXT("OWTSetScaleContext"),&Z_Registration_Info_UScriptStruct_FOWTSetScaleContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTSetScaleContext), 1655865385U) },
		{ FOWTHideSelectionGizmoContext::StaticStruct, Z_Construct_UScriptStruct_FOWTHideSelectionGizmoContext_Statics::NewStructOps, TEXT("OWTHideSelectionGizmoContext"),&Z_Registration_Info_UScriptStruct_FOWTHideSelectionGizmoContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTHideSelectionGizmoContext), 1898251007U) },
		{ FOWTDuplicateSelectionContext::StaticStruct, Z_Construct_UScriptStruct_FOWTDuplicateSelectionContext_Statics::NewStructOps, TEXT("OWTDuplicateSelectionContext"),&Z_Registration_Info_UScriptStruct_FOWTDuplicateSelectionContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTDuplicateSelectionContext), 1162015704U) },
		{ FOWTToggleEditingContext::StaticStruct, Z_Construct_UScriptStruct_FOWTToggleEditingContext_Statics::NewStructOps, TEXT("OWTToggleEditingContext"),&Z_Registration_Info_UScriptStruct_FOWTToggleEditingContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTToggleEditingContext), 2553258908U) },
		{ FOWTGizmoPointerContext::StaticStruct, Z_Construct_UScriptStruct_FOWTGizmoPointerContext_Statics::NewStructOps, TEXT("OWTGizmoPointerContext"),&Z_Registration_Info_UScriptStruct_FOWTGizmoPointerContext, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTGizmoPointerContext), 859356281U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTEditContexts_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTEditContexts_h__Script_VTBOWTEditor_2606593674{
	TEXT("/Script/VTBOWTEditor"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTEditContexts_h__Script_VTBOWTEditor_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTEditContexts_h__Script_VTBOWTEditor_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
