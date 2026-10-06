// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Context/VTBOWTSceneSnappingManager.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTSceneSnappingManager() {}

// ********** Begin Cross Module References ********************************************************
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_USceneSnappingManager();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTSceneSnappingManager();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTSceneSnappingManager_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVTBOWTSceneSnappingManager **********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTSceneSnappingManager;
UClass* UVTBOWTSceneSnappingManager::GetPrivateStaticClass()
{
	using TClass = UVTBOWTSceneSnappingManager;
	if (!Z_Registration_Info_UClass_UVTBOWTSceneSnappingManager.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTSceneSnappingManager"),
			Z_Registration_Info_UClass_UVTBOWTSceneSnappingManager.InnerSingleton,
			StaticRegisterNativesUVTBOWTSceneSnappingManager,
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
	return Z_Registration_Info_UClass_UVTBOWTSceneSnappingManager.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTSceneSnappingManager_NoRegister()
{
	return UVTBOWTSceneSnappingManager::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTSceneSnappingManager_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Runtime grid queries used by CombinedTransformGizmo's translation parameter sources.\n" },
#endif
		{ "IncludePath", "Context/VTBOWTSceneSnappingManager.h" },
		{ "ModuleRelativePath", "Public/Context/VTBOWTSceneSnappingManager.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Runtime grid queries used by CombinedTransformGizmo's translation parameter sources." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTSceneSnappingManager constinit property declarations **************
// ********** End Class UVTBOWTSceneSnappingManager constinit property declarations ****************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTSceneSnappingManager>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTSceneSnappingManager_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTSceneSnappingManager_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_USceneSnappingManager,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTSceneSnappingManager_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTSceneSnappingManager_Statics::ClassParams = {
	&UVTBOWTSceneSnappingManager::StaticClass,
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
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTSceneSnappingManager_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTSceneSnappingManager_Statics::Class_MetaDataParams)
};
void UVTBOWTSceneSnappingManager::StaticRegisterNativesUVTBOWTSceneSnappingManager()
{
}
UClass* Z_Construct_UClass_UVTBOWTSceneSnappingManager()
{
	if (!Z_Registration_Info_UClass_UVTBOWTSceneSnappingManager.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTSceneSnappingManager.OuterSingleton, Z_Construct_UClass_UVTBOWTSceneSnappingManager_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTSceneSnappingManager.OuterSingleton;
}
UVTBOWTSceneSnappingManager::UVTBOWTSceneSnappingManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTSceneSnappingManager);
UVTBOWTSceneSnappingManager::~UVTBOWTSceneSnappingManager() {}
// ********** End Class UVTBOWTSceneSnappingManager ************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_VTBOWTSceneSnappingManager_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVTBOWTSceneSnappingManager, UVTBOWTSceneSnappingManager::StaticClass, TEXT("UVTBOWTSceneSnappingManager"), &Z_Registration_Info_UClass_UVTBOWTSceneSnappingManager, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTSceneSnappingManager), 4289440846U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_VTBOWTSceneSnappingManager_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_VTBOWTSceneSnappingManager_h__Script_VTBOWTEditor_932620711{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_VTBOWTSceneSnappingManager_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_VTBOWTSceneSnappingManager_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
