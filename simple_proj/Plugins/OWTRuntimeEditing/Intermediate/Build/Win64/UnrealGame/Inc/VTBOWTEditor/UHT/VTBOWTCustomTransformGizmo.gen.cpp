// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Gizmos/Custom/VTBOWTCustomTransformGizmo.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTCustomTransformGizmo() {}

// ********** Begin Cross Module References ********************************************************
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveGizmoBuilder();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTBaseTransformGizmo();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTCustomTransformGizmo();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTCustomTransformGizmo_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTCustomTransformGizmoBuilder();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTCustomTransformGizmoBuilder_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVTBOWTCustomTransformGizmoBuilder ***************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmoBuilder;
UClass* UVTBOWTCustomTransformGizmoBuilder::GetPrivateStaticClass()
{
	using TClass = UVTBOWTCustomTransformGizmoBuilder;
	if (!Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmoBuilder.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTCustomTransformGizmoBuilder"),
			Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmoBuilder.InnerSingleton,
			StaticRegisterNativesUVTBOWTCustomTransformGizmoBuilder,
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
	return Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmoBuilder.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTCustomTransformGizmoBuilder_NoRegister()
{
	return UVTBOWTCustomTransformGizmoBuilder::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTCustomTransformGizmoBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Gizmos/Custom/VTBOWTCustomTransformGizmo.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Custom/VTBOWTCustomTransformGizmo.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTCustomTransformGizmoBuilder constinit property declarations *******
// ********** End Class UVTBOWTCustomTransformGizmoBuilder constinit property declarations *********
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTCustomTransformGizmoBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTCustomTransformGizmoBuilder_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTCustomTransformGizmoBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveGizmoBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTCustomTransformGizmoBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTCustomTransformGizmoBuilder_Statics::ClassParams = {
	&UVTBOWTCustomTransformGizmoBuilder::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTCustomTransformGizmoBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTCustomTransformGizmoBuilder_Statics::Class_MetaDataParams)
};
void UVTBOWTCustomTransformGizmoBuilder::StaticRegisterNativesUVTBOWTCustomTransformGizmoBuilder()
{
}
UClass* Z_Construct_UClass_UVTBOWTCustomTransformGizmoBuilder()
{
	if (!Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmoBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmoBuilder.OuterSingleton, Z_Construct_UClass_UVTBOWTCustomTransformGizmoBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmoBuilder.OuterSingleton;
}
UVTBOWTCustomTransformGizmoBuilder::UVTBOWTCustomTransformGizmoBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTCustomTransformGizmoBuilder);
UVTBOWTCustomTransformGizmoBuilder::~UVTBOWTCustomTransformGizmoBuilder() {}
// ********** End Class UVTBOWTCustomTransformGizmoBuilder *****************************************

// ********** Begin Class UVTBOWTCustomTransformGizmo **********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmo;
UClass* UVTBOWTCustomTransformGizmo::GetPrivateStaticClass()
{
	using TClass = UVTBOWTCustomTransformGizmo;
	if (!Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmo.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTCustomTransformGizmo"),
			Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmo.InnerSingleton,
			StaticRegisterNativesUVTBOWTCustomTransformGizmo,
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
	return Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmo.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTCustomTransformGizmo_NoRegister()
{
	return UVTBOWTCustomTransformGizmo::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTCustomTransformGizmo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Shares target binding and runtime interactions; its builder selects the custom handle actor.\n" },
#endif
		{ "IncludePath", "Gizmos/Custom/VTBOWTCustomTransformGizmo.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Custom/VTBOWTCustomTransformGizmo.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Shares target binding and runtime interactions; its builder selects the custom handle actor." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTCustomTransformGizmo constinit property declarations **************
// ********** End Class UVTBOWTCustomTransformGizmo constinit property declarations ****************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTCustomTransformGizmo>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTCustomTransformGizmo_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTCustomTransformGizmo_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UVTBOWTBaseTransformGizmo,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTCustomTransformGizmo_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTCustomTransformGizmo_Statics::ClassParams = {
	&UVTBOWTCustomTransformGizmo::StaticClass,
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
	0x009000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTCustomTransformGizmo_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTCustomTransformGizmo_Statics::Class_MetaDataParams)
};
void UVTBOWTCustomTransformGizmo::StaticRegisterNativesUVTBOWTCustomTransformGizmo()
{
}
UClass* Z_Construct_UClass_UVTBOWTCustomTransformGizmo()
{
	if (!Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmo.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmo.OuterSingleton, Z_Construct_UClass_UVTBOWTCustomTransformGizmo_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmo.OuterSingleton;
}
UVTBOWTCustomTransformGizmo::UVTBOWTCustomTransformGizmo() {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTCustomTransformGizmo);
UVTBOWTCustomTransformGizmo::~UVTBOWTCustomTransformGizmo() {}
// ********** End Class UVTBOWTCustomTransformGizmo ************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Custom_VTBOWTCustomTransformGizmo_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVTBOWTCustomTransformGizmoBuilder, UVTBOWTCustomTransformGizmoBuilder::StaticClass, TEXT("UVTBOWTCustomTransformGizmoBuilder"), &Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmoBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTCustomTransformGizmoBuilder), 926250220U) },
		{ Z_Construct_UClass_UVTBOWTCustomTransformGizmo, UVTBOWTCustomTransformGizmo::StaticClass, TEXT("UVTBOWTCustomTransformGizmo"), &Z_Registration_Info_UClass_UVTBOWTCustomTransformGizmo, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTCustomTransformGizmo), 3034003373U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Custom_VTBOWTCustomTransformGizmo_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Custom_VTBOWTCustomTransformGizmo_h__Script_VTBOWTEditor_3527011614{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Custom_VTBOWTCustomTransformGizmo_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Custom_VTBOWTCustomTransformGizmo_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
