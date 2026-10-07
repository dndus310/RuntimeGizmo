// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Gizmos/Base/VTBOWTTransformGizmoBehavior.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTTransformGizmoBehavior() {}

// ********** Begin Cross Module References ********************************************************
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UClickDragInputBehavior();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTTransformGizmoBehavior();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVTBOWTTransformGizmoBehavior ********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior;
UClass* UVTBOWTTransformGizmoBehavior::GetPrivateStaticClass()
{
	using TClass = UVTBOWTTransformGizmoBehavior;
	if (!Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTTransformGizmoBehavior"),
			Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior.InnerSingleton,
			StaticRegisterNativesUVTBOWTTransformGizmoBehavior,
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
	return Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_NoRegister()
{
	return UVTBOWTTransformGizmoBehavior::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Input configuration only. Axis interactions and builders live in their own headers.\n" },
#endif
		{ "IncludePath", "Gizmos/Base/VTBOWTTransformGizmoBehavior.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Base/VTBOWTTransformGizmoBehavior.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Input configuration only. Axis interactions and builders live in their own headers." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTTransformGizmoBehavior constinit property declarations ************
// ********** End Class UVTBOWTTransformGizmoBehavior constinit property declarations **************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTTransformGizmoBehavior>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UClickDragInputBehavior,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics::ClassParams = {
	&UVTBOWTTransformGizmoBehavior::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics::Class_MetaDataParams)
};
void UVTBOWTTransformGizmoBehavior::StaticRegisterNativesUVTBOWTTransformGizmoBehavior()
{
}
UClass* Z_Construct_UClass_UVTBOWTTransformGizmoBehavior()
{
	if (!Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior.OuterSingleton, Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTTransformGizmoBehavior);
UVTBOWTTransformGizmoBehavior::~UVTBOWTTransformGizmoBehavior() {}
// ********** End Class UVTBOWTTransformGizmoBehavior **********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTTransformGizmoBehavior_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVTBOWTTransformGizmoBehavior, UVTBOWTTransformGizmoBehavior::StaticClass, TEXT("UVTBOWTTransformGizmoBehavior"), &Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTTransformGizmoBehavior), 361700121U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTTransformGizmoBehavior_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTTransformGizmoBehavior_h__Script_VTBOWTEditor_2165994397{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTTransformGizmoBehavior_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTTransformGizmoBehavior_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
