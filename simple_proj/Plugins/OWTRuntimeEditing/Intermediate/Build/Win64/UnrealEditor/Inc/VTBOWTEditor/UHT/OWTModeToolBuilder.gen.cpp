// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Tools/OWTModeToolBuilder.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTModeToolBuilder() {}

// ********** Begin Cross Module References ********************************************************
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolBuilder();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTModeToolBuilder();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTModeToolBuilder_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UOWTModeToolBuilder ******************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTModeToolBuilder;
UClass* UOWTModeToolBuilder::GetPrivateStaticClass()
{
	using TClass = UOWTModeToolBuilder;
	if (!Z_Registration_Info_UClass_UOWTModeToolBuilder.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTModeToolBuilder"),
			Z_Registration_Info_UClass_UOWTModeToolBuilder.InnerSingleton,
			StaticRegisterNativesUOWTModeToolBuilder,
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
	return Z_Registration_Info_UClass_UOWTModeToolBuilder.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTModeToolBuilder_NoRegister()
{
	return UOWTModeToolBuilder::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTModeToolBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tools/OWTModeToolBuilder.h" },
		{ "ModuleRelativePath", "Public/Tools/OWTModeToolBuilder.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTModeToolBuilder constinit property declarations **********************
// ********** End Class UOWTModeToolBuilder constinit property declarations ************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTModeToolBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTModeToolBuilder_Statics
UObject* (*const Z_Construct_UClass_UOWTModeToolBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveToolBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTModeToolBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTModeToolBuilder_Statics::ClassParams = {
	&UOWTModeToolBuilder::StaticClass,
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
	0x001000A9u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTModeToolBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTModeToolBuilder_Statics::Class_MetaDataParams)
};
void UOWTModeToolBuilder::StaticRegisterNativesUOWTModeToolBuilder()
{
}
UClass* Z_Construct_UClass_UOWTModeToolBuilder()
{
	if (!Z_Registration_Info_UClass_UOWTModeToolBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTModeToolBuilder.OuterSingleton, Z_Construct_UClass_UOWTModeToolBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTModeToolBuilder.OuterSingleton;
}
UOWTModeToolBuilder::UOWTModeToolBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTModeToolBuilder);
UOWTModeToolBuilder::~UOWTModeToolBuilder() {}
// ********** End Class UOWTModeToolBuilder ********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTModeToolBuilder_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTModeToolBuilder, UOWTModeToolBuilder::StaticClass, TEXT("UOWTModeToolBuilder"), &Z_Registration_Info_UClass_UOWTModeToolBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTModeToolBuilder), 179713290U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTModeToolBuilder_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTModeToolBuilder_h__Script_VTBOWTEditor_2162896802{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTModeToolBuilder_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTModeToolBuilder_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
