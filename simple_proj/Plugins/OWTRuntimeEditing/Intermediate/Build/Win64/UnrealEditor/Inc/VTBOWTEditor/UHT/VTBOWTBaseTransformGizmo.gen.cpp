// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Gizmos/Base/VTBOWTBaseTransformGizmo.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTBaseTransformGizmo() {}

// ********** Begin Cross Module References ********************************************************
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UCombinedTransformGizmo();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveGizmoBuilder();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTBaseTransformGizmo();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTBaseTransformGizmo_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTBaseTransformGizmoBuilder();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTBaseTransformGizmoBuilder_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVTBOWTBaseTransformGizmoBuilder *****************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmoBuilder;
UClass* UVTBOWTBaseTransformGizmoBuilder::GetPrivateStaticClass()
{
	using TClass = UVTBOWTBaseTransformGizmoBuilder;
	if (!Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmoBuilder.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTBaseTransformGizmoBuilder"),
			Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmoBuilder.InnerSingleton,
			StaticRegisterNativesUVTBOWTBaseTransformGizmoBuilder,
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
	return Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmoBuilder.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTBaseTransformGizmoBuilder_NoRegister()
{
	return UVTBOWTBaseTransformGizmoBuilder::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTBaseTransformGizmoBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Registered and owned by the runtime GizmoManager. No LightGizmos/UnrealEd dependency.\n" },
#endif
		{ "IncludePath", "Gizmos/Base/VTBOWTBaseTransformGizmo.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Base/VTBOWTBaseTransformGizmo.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Registered and owned by the runtime GizmoManager. No LightGizmos/UnrealEd dependency." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTBaseTransformGizmoBuilder constinit property declarations *********
// ********** End Class UVTBOWTBaseTransformGizmoBuilder constinit property declarations ***********
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTBaseTransformGizmoBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTBaseTransformGizmoBuilder_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTBaseTransformGizmoBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveGizmoBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTBaseTransformGizmoBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTBaseTransformGizmoBuilder_Statics::ClassParams = {
	&UVTBOWTBaseTransformGizmoBuilder::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTBaseTransformGizmoBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTBaseTransformGizmoBuilder_Statics::Class_MetaDataParams)
};
void UVTBOWTBaseTransformGizmoBuilder::StaticRegisterNativesUVTBOWTBaseTransformGizmoBuilder()
{
}
UClass* Z_Construct_UClass_UVTBOWTBaseTransformGizmoBuilder()
{
	if (!Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmoBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmoBuilder.OuterSingleton, Z_Construct_UClass_UVTBOWTBaseTransformGizmoBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmoBuilder.OuterSingleton;
}
UVTBOWTBaseTransformGizmoBuilder::UVTBOWTBaseTransformGizmoBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTBaseTransformGizmoBuilder);
UVTBOWTBaseTransformGizmoBuilder::~UVTBOWTBaseTransformGizmoBuilder() {}
// ********** End Class UVTBOWTBaseTransformGizmoBuilder *******************************************

// ********** Begin Class UVTBOWTBaseTransformGizmo ************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmo;
UClass* UVTBOWTBaseTransformGizmo::GetPrivateStaticClass()
{
	using TClass = UVTBOWTBaseTransformGizmo;
	if (!Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmo.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTBaseTransformGizmo"),
			Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmo.InnerSingleton,
			StaticRegisterNativesUVTBOWTBaseTransformGizmo,
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
	return Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmo.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTBaseTransformGizmo_NoRegister()
{
	return UVTBOWTBaseTransformGizmo::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTBaseTransformGizmo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// CombinedTransformGizmo supplies runtime handles, hit testing and drag behaviors.\n// Its Setup/Shutdown lifecycle is driven exclusively by GizmoManager.\n" },
#endif
		{ "IncludePath", "Gizmos/Base/VTBOWTBaseTransformGizmo.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Base/VTBOWTBaseTransformGizmo.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "CombinedTransformGizmo supplies runtime handles, hit testing and drag behaviors.\nIts Setup/Shutdown lifecycle is driven exclusively by GizmoManager." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTBaseTransformGizmo constinit property declarations ****************
// ********** End Class UVTBOWTBaseTransformGizmo constinit property declarations ******************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTBaseTransformGizmo>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTBaseTransformGizmo_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTBaseTransformGizmo_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UCombinedTransformGizmo,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTBaseTransformGizmo_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTBaseTransformGizmo_Statics::ClassParams = {
	&UVTBOWTBaseTransformGizmo::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTBaseTransformGizmo_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTBaseTransformGizmo_Statics::Class_MetaDataParams)
};
void UVTBOWTBaseTransformGizmo::StaticRegisterNativesUVTBOWTBaseTransformGizmo()
{
}
UClass* Z_Construct_UClass_UVTBOWTBaseTransformGizmo()
{
	if (!Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmo.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmo.OuterSingleton, Z_Construct_UClass_UVTBOWTBaseTransformGizmo_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmo.OuterSingleton;
}
UVTBOWTBaseTransformGizmo::UVTBOWTBaseTransformGizmo() {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTBaseTransformGizmo);
UVTBOWTBaseTransformGizmo::~UVTBOWTBaseTransformGizmo() {}
// ********** End Class UVTBOWTBaseTransformGizmo **************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTBaseTransformGizmo_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVTBOWTBaseTransformGizmoBuilder, UVTBOWTBaseTransformGizmoBuilder::StaticClass, TEXT("UVTBOWTBaseTransformGizmoBuilder"), &Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmoBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTBaseTransformGizmoBuilder), 3908601145U) },
		{ Z_Construct_UClass_UVTBOWTBaseTransformGizmo, UVTBOWTBaseTransformGizmo::StaticClass, TEXT("UVTBOWTBaseTransformGizmo"), &Z_Registration_Info_UClass_UVTBOWTBaseTransformGizmo, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTBaseTransformGizmo), 2989544332U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTBaseTransformGizmo_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTBaseTransformGizmo_h__Script_VTBOWTEditor_2578346759{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTBaseTransformGizmo_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTBaseTransformGizmo_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
