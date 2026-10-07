// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Gizmos/Base/VTBOWTAxisPositionGizmo.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTAxisPositionGizmo() {}

// ********** Begin Cross Module References ********************************************************
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UAxisPositionGizmo();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveGizmoBuilder();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmo();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmo_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVTBOWTAxisPositionGizmo *************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo;
UClass* UVTBOWTAxisPositionGizmo::GetPrivateStaticClass()
{
	using TClass = UVTBOWTAxisPositionGizmo;
	if (!Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTAxisPositionGizmo"),
			Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo.InnerSingleton,
			StaticRegisterNativesUVTBOWTAxisPositionGizmo,
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
	return Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmo_NoRegister()
{
	return UVTBOWTAxisPositionGizmo::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Gizmos/Base/VTBOWTAxisPositionGizmo.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Base/VTBOWTAxisPositionGizmo.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTAxisPositionGizmo constinit property declarations *****************
// ********** End Class UVTBOWTAxisPositionGizmo constinit property declarations *******************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTAxisPositionGizmo>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UAxisPositionGizmo,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics::ClassParams = {
	&UVTBOWTAxisPositionGizmo::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics::Class_MetaDataParams)
};
void UVTBOWTAxisPositionGizmo::StaticRegisterNativesUVTBOWTAxisPositionGizmo()
{
}
UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmo()
{
	if (!Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo.OuterSingleton, Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo.OuterSingleton;
}
UVTBOWTAxisPositionGizmo::UVTBOWTAxisPositionGizmo() {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTAxisPositionGizmo);
UVTBOWTAxisPositionGizmo::~UVTBOWTAxisPositionGizmo() {}
// ********** End Class UVTBOWTAxisPositionGizmo ***************************************************

// ********** Begin Class UVTBOWTAxisPositionGizmoBuilder ******************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder;
UClass* UVTBOWTAxisPositionGizmoBuilder::GetPrivateStaticClass()
{
	using TClass = UVTBOWTAxisPositionGizmoBuilder;
	if (!Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTAxisPositionGizmoBuilder"),
			Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder.InnerSingleton,
			StaticRegisterNativesUVTBOWTAxisPositionGizmoBuilder,
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
	return Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_NoRegister()
{
	return UVTBOWTAxisPositionGizmoBuilder::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Gizmos/Base/VTBOWTAxisPositionGizmo.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Base/VTBOWTAxisPositionGizmo.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTAxisPositionGizmoBuilder constinit property declarations **********
// ********** End Class UVTBOWTAxisPositionGizmoBuilder constinit property declarations ************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTAxisPositionGizmoBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveGizmoBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics::ClassParams = {
	&UVTBOWTAxisPositionGizmoBuilder::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics::Class_MetaDataParams)
};
void UVTBOWTAxisPositionGizmoBuilder::StaticRegisterNativesUVTBOWTAxisPositionGizmoBuilder()
{
}
UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder()
{
	if (!Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder.OuterSingleton, Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder.OuterSingleton;
}
UVTBOWTAxisPositionGizmoBuilder::UVTBOWTAxisPositionGizmoBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTAxisPositionGizmoBuilder);
UVTBOWTAxisPositionGizmoBuilder::~UVTBOWTAxisPositionGizmoBuilder() {}
// ********** End Class UVTBOWTAxisPositionGizmoBuilder ********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTAxisPositionGizmo_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVTBOWTAxisPositionGizmo, UVTBOWTAxisPositionGizmo::StaticClass, TEXT("UVTBOWTAxisPositionGizmo"), &Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTAxisPositionGizmo), 2578107283U) },
		{ Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder, UVTBOWTAxisPositionGizmoBuilder::StaticClass, TEXT("UVTBOWTAxisPositionGizmoBuilder"), &Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTAxisPositionGizmoBuilder), 1242570337U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTAxisPositionGizmo_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTAxisPositionGizmo_h__Script_VTBOWTEditor_2669978330{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTAxisPositionGizmo_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTAxisPositionGizmo_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
