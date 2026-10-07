// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Tools/OWTDuplicateTool.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTDuplicateTool() {}

// ********** Begin Cross Module References ********************************************************
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveTool();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeEditMode_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTDuplicateTool();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTDuplicateTool_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTDuplicateToolBuilder();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTDuplicateToolBuilder_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTModeToolBuilder();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UOWTDuplicateTool ********************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTDuplicateTool;
UClass* UOWTDuplicateTool::GetPrivateStaticClass()
{
	using TClass = UOWTDuplicateTool;
	if (!Z_Registration_Info_UClass_UOWTDuplicateTool.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTDuplicateTool"),
			Z_Registration_Info_UClass_UOWTDuplicateTool.InnerSingleton,
			StaticRegisterNativesUOWTDuplicateTool,
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
	return Z_Registration_Info_UClass_UOWTDuplicateTool.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTDuplicateTool_NoRegister()
{
	return UOWTDuplicateTool::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTDuplicateTool_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tools/OWTDuplicateTool.h" },
		{ "ModuleRelativePath", "Public/Tools/OWTDuplicateTool.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Mode_MetaData[] = {
		{ "ModuleRelativePath", "Public/Tools/OWTDuplicateTool.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTDuplicateTool constinit property declarations ************************
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_Mode;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UOWTDuplicateTool constinit property declarations **************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTDuplicateTool>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTDuplicateTool_Statics

// ********** Begin Class UOWTDuplicateTool Property Definitions ***********************************
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UOWTDuplicateTool_Statics::NewProp_Mode = { "Mode", nullptr, (EPropertyFlags)0x0044000000002000, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTDuplicateTool, Mode), Z_Construct_UClass_UOWTAttributeEditMode_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Mode_MetaData), NewProp_Mode_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOWTDuplicateTool_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTDuplicateTool_Statics::NewProp_Mode,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTDuplicateTool_Statics::PropPointers) < 2048);
// ********** End Class UOWTDuplicateTool Property Definitions *************************************
UObject* (*const Z_Construct_UClass_UOWTDuplicateTool_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveTool,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTDuplicateTool_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTDuplicateTool_Statics::ClassParams = {
	&UOWTDuplicateTool::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UOWTDuplicateTool_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UOWTDuplicateTool_Statics::PropPointers),
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTDuplicateTool_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTDuplicateTool_Statics::Class_MetaDataParams)
};
void UOWTDuplicateTool::StaticRegisterNativesUOWTDuplicateTool()
{
}
UClass* Z_Construct_UClass_UOWTDuplicateTool()
{
	if (!Z_Registration_Info_UClass_UOWTDuplicateTool.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTDuplicateTool.OuterSingleton, Z_Construct_UClass_UOWTDuplicateTool_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTDuplicateTool.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTDuplicateTool);
UOWTDuplicateTool::~UOWTDuplicateTool() {}
// ********** End Class UOWTDuplicateTool **********************************************************

// ********** Begin Class UOWTDuplicateToolBuilder *************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTDuplicateToolBuilder;
UClass* UOWTDuplicateToolBuilder::GetPrivateStaticClass()
{
	using TClass = UOWTDuplicateToolBuilder;
	if (!Z_Registration_Info_UClass_UOWTDuplicateToolBuilder.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTDuplicateToolBuilder"),
			Z_Registration_Info_UClass_UOWTDuplicateToolBuilder.InnerSingleton,
			StaticRegisterNativesUOWTDuplicateToolBuilder,
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
	return Z_Registration_Info_UClass_UOWTDuplicateToolBuilder.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTDuplicateToolBuilder_NoRegister()
{
	return UOWTDuplicateToolBuilder::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTDuplicateToolBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tools/OWTDuplicateTool.h" },
		{ "ModuleRelativePath", "Public/Tools/OWTDuplicateTool.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTDuplicateToolBuilder constinit property declarations *****************
// ********** End Class UOWTDuplicateToolBuilder constinit property declarations *******************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTDuplicateToolBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTDuplicateToolBuilder_Statics
UObject* (*const Z_Construct_UClass_UOWTDuplicateToolBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UOWTModeToolBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTDuplicateToolBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTDuplicateToolBuilder_Statics::ClassParams = {
	&UOWTDuplicateToolBuilder::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTDuplicateToolBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTDuplicateToolBuilder_Statics::Class_MetaDataParams)
};
void UOWTDuplicateToolBuilder::StaticRegisterNativesUOWTDuplicateToolBuilder()
{
}
UClass* Z_Construct_UClass_UOWTDuplicateToolBuilder()
{
	if (!Z_Registration_Info_UClass_UOWTDuplicateToolBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTDuplicateToolBuilder.OuterSingleton, Z_Construct_UClass_UOWTDuplicateToolBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTDuplicateToolBuilder.OuterSingleton;
}
UOWTDuplicateToolBuilder::UOWTDuplicateToolBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTDuplicateToolBuilder);
UOWTDuplicateToolBuilder::~UOWTDuplicateToolBuilder() {}
// ********** End Class UOWTDuplicateToolBuilder ***************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTDuplicateTool_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTDuplicateTool, UOWTDuplicateTool::StaticClass, TEXT("UOWTDuplicateTool"), &Z_Registration_Info_UClass_UOWTDuplicateTool, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTDuplicateTool), 4001938359U) },
		{ Z_Construct_UClass_UOWTDuplicateToolBuilder, UOWTDuplicateToolBuilder::StaticClass, TEXT("UOWTDuplicateToolBuilder"), &Z_Registration_Info_UClass_UOWTDuplicateToolBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTDuplicateToolBuilder), 4004942244U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTDuplicateTool_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTDuplicateTool_h__Script_VTBOWTEditor_114881408{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTDuplicateTool_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTDuplicateTool_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
