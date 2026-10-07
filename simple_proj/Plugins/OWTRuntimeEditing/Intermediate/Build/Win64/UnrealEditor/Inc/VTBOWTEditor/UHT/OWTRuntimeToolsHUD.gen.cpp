// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Rendering/OWTRuntimeToolsHUD.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTRuntimeToolsHUD() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AHUD();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AOWTRuntimeToolsHUD();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AOWTRuntimeToolsHUD_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AOWTRuntimeToolsHUD ******************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_AOWTRuntimeToolsHUD;
UClass* AOWTRuntimeToolsHUD::GetPrivateStaticClass()
{
	using TClass = AOWTRuntimeToolsHUD;
	if (!Z_Registration_Info_UClass_AOWTRuntimeToolsHUD.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTRuntimeToolsHUD"),
			Z_Registration_Info_UClass_AOWTRuntimeToolsHUD.InnerSingleton,
			StaticRegisterNativesAOWTRuntimeToolsHUD,
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
	return Z_Registration_Info_UClass_AOWTRuntimeToolsHUD.InnerSingleton;
}
UClass* Z_Construct_UClass_AOWTRuntimeToolsHUD_NoRegister()
{
	return AOWTRuntimeToolsHUD::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AOWTRuntimeToolsHUD_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Other projects can derive their HUD from this class, or call Mode.RenderTools from their own HUD.\n" },
#endif
		{ "HideCategories", "Rendering Actor Input Replication" },
		{ "IncludePath", "Rendering/OWTRuntimeToolsHUD.h" },
		{ "ModuleRelativePath", "Public/Rendering/OWTRuntimeToolsHUD.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Other projects can derive their HUD from this class, or call Mode.RenderTools from their own HUD." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class AOWTRuntimeToolsHUD constinit property declarations **********************
// ********** End Class AOWTRuntimeToolsHUD constinit property declarations ************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AOWTRuntimeToolsHUD>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_AOWTRuntimeToolsHUD_Statics
UObject* (*const Z_Construct_UClass_AOWTRuntimeToolsHUD_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AHUD,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AOWTRuntimeToolsHUD_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AOWTRuntimeToolsHUD_Statics::ClassParams = {
	&AOWTRuntimeToolsHUD::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x009002ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AOWTRuntimeToolsHUD_Statics::Class_MetaDataParams), Z_Construct_UClass_AOWTRuntimeToolsHUD_Statics::Class_MetaDataParams)
};
void AOWTRuntimeToolsHUD::StaticRegisterNativesAOWTRuntimeToolsHUD()
{
}
UClass* Z_Construct_UClass_AOWTRuntimeToolsHUD()
{
	if (!Z_Registration_Info_UClass_AOWTRuntimeToolsHUD.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AOWTRuntimeToolsHUD.OuterSingleton, Z_Construct_UClass_AOWTRuntimeToolsHUD_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AOWTRuntimeToolsHUD.OuterSingleton;
}
AOWTRuntimeToolsHUD::AOWTRuntimeToolsHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, AOWTRuntimeToolsHUD);
AOWTRuntimeToolsHUD::~AOWTRuntimeToolsHUD() {}
// ********** End Class AOWTRuntimeToolsHUD ********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Rendering_OWTRuntimeToolsHUD_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AOWTRuntimeToolsHUD, AOWTRuntimeToolsHUD::StaticClass, TEXT("AOWTRuntimeToolsHUD"), &Z_Registration_Info_UClass_AOWTRuntimeToolsHUD, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AOWTRuntimeToolsHUD), 3416969682U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Rendering_OWTRuntimeToolsHUD_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Rendering_OWTRuntimeToolsHUD_h__Script_VTBOWTEditor_2383237744{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Rendering_OWTRuntimeToolsHUD_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Rendering_OWTRuntimeToolsHUD_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
