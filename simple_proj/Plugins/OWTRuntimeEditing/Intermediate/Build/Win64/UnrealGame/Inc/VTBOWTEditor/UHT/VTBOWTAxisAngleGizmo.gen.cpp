// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Gizmos/Base/VTBOWTAxisAngleGizmo.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTAxisAngleGizmo() {}

// ********** Begin Cross Module References ********************************************************
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UAxisAngleGizmo();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveGizmoBuilder();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmo();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmo_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVTBOWTAxisAngleGizmo ****************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo;
UClass* UVTBOWTAxisAngleGizmo::GetPrivateStaticClass()
{
	using TClass = UVTBOWTAxisAngleGizmo;
	if (!Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTAxisAngleGizmo"),
			Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo.InnerSingleton,
			StaticRegisterNativesUVTBOWTAxisAngleGizmo,
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
	return Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmo_NoRegister()
{
	return UVTBOWTAxisAngleGizmo::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Gizmos/Base/VTBOWTAxisAngleGizmo.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Base/VTBOWTAxisAngleGizmo.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTAxisAngleGizmo constinit property declarations ********************
// ********** End Class UVTBOWTAxisAngleGizmo constinit property declarations **********************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTAxisAngleGizmo>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UAxisAngleGizmo,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics::ClassParams = {
	&UVTBOWTAxisAngleGizmo::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics::Class_MetaDataParams)
};
void UVTBOWTAxisAngleGizmo::StaticRegisterNativesUVTBOWTAxisAngleGizmo()
{
}
UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmo()
{
	if (!Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo.OuterSingleton, Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTAxisAngleGizmo);
UVTBOWTAxisAngleGizmo::~UVTBOWTAxisAngleGizmo() {}
// ********** End Class UVTBOWTAxisAngleGizmo ******************************************************

// ********** Begin Class UVTBOWTAxisAngleGizmoBuilder *********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder;
UClass* UVTBOWTAxisAngleGizmoBuilder::GetPrivateStaticClass()
{
	using TClass = UVTBOWTAxisAngleGizmoBuilder;
	if (!Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTAxisAngleGizmoBuilder"),
			Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder.InnerSingleton,
			StaticRegisterNativesUVTBOWTAxisAngleGizmoBuilder,
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
	return Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_NoRegister()
{
	return UVTBOWTAxisAngleGizmoBuilder::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Gizmos/Base/VTBOWTAxisAngleGizmo.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Base/VTBOWTAxisAngleGizmo.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTAxisAngleGizmoBuilder constinit property declarations *************
// ********** End Class UVTBOWTAxisAngleGizmoBuilder constinit property declarations ***************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTAxisAngleGizmoBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveGizmoBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics::ClassParams = {
	&UVTBOWTAxisAngleGizmoBuilder::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics::Class_MetaDataParams)
};
void UVTBOWTAxisAngleGizmoBuilder::StaticRegisterNativesUVTBOWTAxisAngleGizmoBuilder()
{
}
UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder()
{
	if (!Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder.OuterSingleton, Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder.OuterSingleton;
}
UVTBOWTAxisAngleGizmoBuilder::UVTBOWTAxisAngleGizmoBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTAxisAngleGizmoBuilder);
UVTBOWTAxisAngleGizmoBuilder::~UVTBOWTAxisAngleGizmoBuilder() {}
// ********** End Class UVTBOWTAxisAngleGizmoBuilder ***********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTAxisAngleGizmo_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVTBOWTAxisAngleGizmo, UVTBOWTAxisAngleGizmo::StaticClass, TEXT("UVTBOWTAxisAngleGizmo"), &Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTAxisAngleGizmo), 2158635657U) },
		{ Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder, UVTBOWTAxisAngleGizmoBuilder::StaticClass, TEXT("UVTBOWTAxisAngleGizmoBuilder"), &Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTAxisAngleGizmoBuilder), 1457660915U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTAxisAngleGizmo_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTAxisAngleGizmo_h__Script_VTBOWTEditor_1429395163{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTAxisAngleGizmo_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTAxisAngleGizmo_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
