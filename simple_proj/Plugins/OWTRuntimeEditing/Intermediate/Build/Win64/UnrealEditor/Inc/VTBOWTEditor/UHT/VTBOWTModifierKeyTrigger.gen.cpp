// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Input/VTBOWTModifierKeyTrigger.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTModifierKeyTrigger() {}

// ********** Begin Cross Module References ********************************************************
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputTrigger();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTModifierKeyTrigger();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTModifierKeyTrigger_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVTBOWTModifierKeyTrigger ************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTModifierKeyTrigger;
UClass* UVTBOWTModifierKeyTrigger::GetPrivateStaticClass()
{
	using TClass = UVTBOWTModifierKeyTrigger;
	if (!Z_Registration_Info_UClass_UVTBOWTModifierKeyTrigger.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTModifierKeyTrigger"),
			Z_Registration_Info_UClass_UVTBOWTModifierKeyTrigger.InnerSingleton,
			StaticRegisterNativesUVTBOWTModifierKeyTrigger,
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
	return Z_Registration_Info_UClass_UVTBOWTModifierKeyTrigger.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTModifierKeyTrigger_NoRegister()
{
	return UVTBOWTModifierKeyTrigger::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Used alongside Pressed on individual IMC mappings, never by the editing system.\n" },
#endif
		{ "DisplayName", "OWT Modifier Keys" },
		{ "IncludePath", "Input/VTBOWTModifierKeyTrigger.h" },
		{ "ModuleRelativePath", "Public/Input/VTBOWTModifierKeyTrigger.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Used alongside Pressed on individual IMC mappings, never by the editing system." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bRequireControl_MetaData[] = {
		{ "Category", "Modifiers" },
		{ "ModuleRelativePath", "Public/Input/VTBOWTModifierKeyTrigger.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bRequireShift_MetaData[] = {
		{ "Category", "Modifiers" },
		{ "ModuleRelativePath", "Public/Input/VTBOWTModifierKeyTrigger.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bDisallowShift_MetaData[] = {
		{ "Category", "Modifiers" },
		{ "ModuleRelativePath", "Public/Input/VTBOWTModifierKeyTrigger.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTModifierKeyTrigger constinit property declarations ****************
	static void NewProp_bRequireControl_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bRequireControl;
	static void NewProp_bRequireShift_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bRequireShift;
	static void NewProp_bDisallowShift_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bDisallowShift;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UVTBOWTModifierKeyTrigger constinit property declarations ******************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTModifierKeyTrigger>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics

// ********** Begin Class UVTBOWTModifierKeyTrigger Property Definitions ***************************
void Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::NewProp_bRequireControl_SetBit(void* Obj)
{
	((UVTBOWTModifierKeyTrigger*)Obj)->bRequireControl = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::NewProp_bRequireControl = { "bRequireControl", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UVTBOWTModifierKeyTrigger), &Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::NewProp_bRequireControl_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRequireControl_MetaData), NewProp_bRequireControl_MetaData) };
void Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::NewProp_bRequireShift_SetBit(void* Obj)
{
	((UVTBOWTModifierKeyTrigger*)Obj)->bRequireShift = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::NewProp_bRequireShift = { "bRequireShift", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UVTBOWTModifierKeyTrigger), &Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::NewProp_bRequireShift_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bRequireShift_MetaData), NewProp_bRequireShift_MetaData) };
void Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::NewProp_bDisallowShift_SetBit(void* Obj)
{
	((UVTBOWTModifierKeyTrigger*)Obj)->bDisallowShift = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::NewProp_bDisallowShift = { "bDisallowShift", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UVTBOWTModifierKeyTrigger), &Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::NewProp_bDisallowShift_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bDisallowShift_MetaData), NewProp_bDisallowShift_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::NewProp_bRequireControl,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::NewProp_bRequireShift,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::NewProp_bDisallowShift,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::PropPointers) < 2048);
// ********** End Class UVTBOWTModifierKeyTrigger Property Definitions *****************************
UObject* (*const Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInputTrigger,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::ClassParams = {
	&UVTBOWTModifierKeyTrigger::StaticClass,
	"Input",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::PropPointers),
	0,
	0x401030A6u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::Class_MetaDataParams)
};
void UVTBOWTModifierKeyTrigger::StaticRegisterNativesUVTBOWTModifierKeyTrigger()
{
}
UClass* Z_Construct_UClass_UVTBOWTModifierKeyTrigger()
{
	if (!Z_Registration_Info_UClass_UVTBOWTModifierKeyTrigger.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTModifierKeyTrigger.OuterSingleton, Z_Construct_UClass_UVTBOWTModifierKeyTrigger_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTModifierKeyTrigger.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTModifierKeyTrigger);
UVTBOWTModifierKeyTrigger::~UVTBOWTModifierKeyTrigger() {}
// ********** End Class UVTBOWTModifierKeyTrigger **************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Input_VTBOWTModifierKeyTrigger_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVTBOWTModifierKeyTrigger, UVTBOWTModifierKeyTrigger::StaticClass, TEXT("UVTBOWTModifierKeyTrigger"), &Z_Registration_Info_UClass_UVTBOWTModifierKeyTrigger, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTModifierKeyTrigger), 595753485U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Input_VTBOWTModifierKeyTrigger_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Input_VTBOWTModifierKeyTrigger_h__Script_VTBOWTEditor_1726861158{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Input_VTBOWTModifierKeyTrigger_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Input_VTBOWTModifierKeyTrigger_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
