// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Gizmos/Custom/VTBOWTTransformGizmoActor.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTTransformGizmoActor() {}

// ********** Begin Cross Module References ********************************************************
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_ACombinedTransformGizmoActor();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBOWTTransformGizmoActor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBOWTTransformGizmoActor_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AVTBOWTTransformGizmoActor ***********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_AVTBOWTTransformGizmoActor;
UClass* AVTBOWTTransformGizmoActor::GetPrivateStaticClass()
{
	using TClass = AVTBOWTTransformGizmoActor;
	if (!Z_Registration_Info_UClass_AVTBOWTTransformGizmoActor.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTTransformGizmoActor"),
			Z_Registration_Info_UClass_AVTBOWTTransformGizmoActor.InnerSingleton,
			StaticRegisterNativesAVTBOWTTransformGizmoActor,
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
	return Z_Registration_Info_UClass_AVTBOWTTransformGizmoActor.InnerSingleton;
}
UClass* Z_Construct_UClass_AVTBOWTTransformGizmoActor_NoRegister()
{
	return AVTBOWTTransformGizmoActor::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AVTBOWTTransformGizmoActor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Gizmos/Custom/VTBOWTTransformGizmoActor.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Custom/VTBOWTTransformGizmoActor.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class AVTBOWTTransformGizmoActor constinit property declarations ***************
// ********** End Class AVTBOWTTransformGizmoActor constinit property declarations *****************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AVTBOWTTransformGizmoActor>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_AVTBOWTTransformGizmoActor_Statics
UObject* (*const Z_Construct_UClass_AVTBOWTTransformGizmoActor_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ACombinedTransformGizmoActor,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTTransformGizmoActor_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AVTBOWTTransformGizmoActor_Statics::ClassParams = {
	&AVTBOWTTransformGizmoActor::StaticClass,
	"Engine",
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTTransformGizmoActor_Statics::Class_MetaDataParams), Z_Construct_UClass_AVTBOWTTransformGizmoActor_Statics::Class_MetaDataParams)
};
void AVTBOWTTransformGizmoActor::StaticRegisterNativesAVTBOWTTransformGizmoActor()
{
}
UClass* Z_Construct_UClass_AVTBOWTTransformGizmoActor()
{
	if (!Z_Registration_Info_UClass_AVTBOWTTransformGizmoActor.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AVTBOWTTransformGizmoActor.OuterSingleton, Z_Construct_UClass_AVTBOWTTransformGizmoActor_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AVTBOWTTransformGizmoActor.OuterSingleton;
}
AVTBOWTTransformGizmoActor::AVTBOWTTransformGizmoActor() {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, AVTBOWTTransformGizmoActor);
AVTBOWTTransformGizmoActor::~AVTBOWTTransformGizmoActor() {}
// ********** End Class AVTBOWTTransformGizmoActor *************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Custom_VTBOWTTransformGizmoActor_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AVTBOWTTransformGizmoActor, AVTBOWTTransformGizmoActor::StaticClass, TEXT("AVTBOWTTransformGizmoActor"), &Z_Registration_Info_UClass_AVTBOWTTransformGizmoActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AVTBOWTTransformGizmoActor), 3100860178U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Custom_VTBOWTTransformGizmoActor_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Custom_VTBOWTTransformGizmoActor_h__Script_VTBOWTEditor_183810309{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Custom_VTBOWTTransformGizmoActor_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Custom_VTBOWTTransformGizmoActor_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
