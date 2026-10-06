// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VTBOWTEditorGameState.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTEditorGameState() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AGameState();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBOWTEditorGameState();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBOWTEditorGameState_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AVTBOWTEditorGameState ***************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_AVTBOWTEditorGameState;
UClass* AVTBOWTEditorGameState::GetPrivateStaticClass()
{
	using TClass = AVTBOWTEditorGameState;
	if (!Z_Registration_Info_UClass_AVTBOWTEditorGameState.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTEditorGameState"),
			Z_Registration_Info_UClass_AVTBOWTEditorGameState.InnerSingleton,
			StaticRegisterNativesAVTBOWTEditorGameState,
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
	return Z_Registration_Info_UClass_AVTBOWTEditorGameState.InnerSingleton;
}
UClass* Z_Construct_UClass_AVTBOWTEditorGameState_NoRegister()
{
	return AVTBOWTEditorGameState::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AVTBOWTEditorGameState_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n *\n */" },
#endif
		{ "HideCategories", "Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "VTBOWTEditorGameState.h" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorGameState.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA

// ********** Begin Class AVTBOWTEditorGameState constinit property declarations *******************
// ********** End Class AVTBOWTEditorGameState constinit property declarations *********************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AVTBOWTEditorGameState>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_AVTBOWTEditorGameState_Statics
UObject* (*const Z_Construct_UClass_AVTBOWTEditorGameState_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameState,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTEditorGameState_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AVTBOWTEditorGameState_Statics::ClassParams = {
	&AVTBOWTEditorGameState::StaticClass,
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
	0x009002A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTEditorGameState_Statics::Class_MetaDataParams), Z_Construct_UClass_AVTBOWTEditorGameState_Statics::Class_MetaDataParams)
};
void AVTBOWTEditorGameState::StaticRegisterNativesAVTBOWTEditorGameState()
{
}
UClass* Z_Construct_UClass_AVTBOWTEditorGameState()
{
	if (!Z_Registration_Info_UClass_AVTBOWTEditorGameState.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AVTBOWTEditorGameState.OuterSingleton, Z_Construct_UClass_AVTBOWTEditorGameState_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AVTBOWTEditorGameState.OuterSingleton;
}
AVTBOWTEditorGameState::AVTBOWTEditorGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, AVTBOWTEditorGameState);
AVTBOWTEditorGameState::~AVTBOWTEditorGameState() {}
// ********** End Class AVTBOWTEditorGameState *****************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameState_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AVTBOWTEditorGameState, AVTBOWTEditorGameState::StaticClass, TEXT("AVTBOWTEditorGameState"), &Z_Registration_Info_UClass_AVTBOWTEditorGameState, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AVTBOWTEditorGameState), 4068663635U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameState_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameState_h__Script_VTBOWTEditor_4183350319{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameState_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameState_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
