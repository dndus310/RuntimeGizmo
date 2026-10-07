// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Targets/OWTActorToolTarget.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTActorToolTarget() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UToolTarget();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UToolTargetFactory();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTActorToolTarget();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTActorToolTarget_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTActorToolTargetFactory();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTActorToolTargetFactory_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UOWTActorToolTarget ******************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTActorToolTarget;
UClass* UOWTActorToolTarget::GetPrivateStaticClass()
{
	using TClass = UOWTActorToolTarget;
	if (!Z_Registration_Info_UClass_UOWTActorToolTarget.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTActorToolTarget"),
			Z_Registration_Info_UClass_UOWTActorToolTarget.InnerSingleton,
			StaticRegisterNativesUOWTActorToolTarget,
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
	return Z_Registration_Info_UClass_UOWTActorToolTarget.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTActorToolTarget_NoRegister()
{
	return UOWTActorToolTarget::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTActorToolTarget_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Targets/OWTActorToolTarget.h" },
		{ "ModuleRelativePath", "Public/Targets/OWTActorToolTarget.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Actor_MetaData[] = {
		{ "ModuleRelativePath", "Public/Targets/OWTActorToolTarget.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTActorToolTarget constinit property declarations **********************
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_Actor;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UOWTActorToolTarget constinit property declarations ************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTActorToolTarget>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTActorToolTarget_Statics

// ********** Begin Class UOWTActorToolTarget Property Definitions *********************************
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UOWTActorToolTarget_Statics::NewProp_Actor = { "Actor", nullptr, (EPropertyFlags)0x0044000000002000, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTActorToolTarget, Actor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Actor_MetaData), NewProp_Actor_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOWTActorToolTarget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTActorToolTarget_Statics::NewProp_Actor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTActorToolTarget_Statics::PropPointers) < 2048);
// ********** End Class UOWTActorToolTarget Property Definitions ***********************************
UObject* (*const Z_Construct_UClass_UOWTActorToolTarget_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UToolTarget,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTActorToolTarget_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTActorToolTarget_Statics::ClassParams = {
	&UOWTActorToolTarget::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UOWTActorToolTarget_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UOWTActorToolTarget_Statics::PropPointers),
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTActorToolTarget_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTActorToolTarget_Statics::Class_MetaDataParams)
};
void UOWTActorToolTarget::StaticRegisterNativesUOWTActorToolTarget()
{
}
UClass* Z_Construct_UClass_UOWTActorToolTarget()
{
	if (!Z_Registration_Info_UClass_UOWTActorToolTarget.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTActorToolTarget.OuterSingleton, Z_Construct_UClass_UOWTActorToolTarget_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTActorToolTarget.OuterSingleton;
}
UOWTActorToolTarget::UOWTActorToolTarget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTActorToolTarget);
UOWTActorToolTarget::~UOWTActorToolTarget() {}
// ********** End Class UOWTActorToolTarget ********************************************************

// ********** Begin Class UOWTActorToolTargetFactory ***********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTActorToolTargetFactory;
UClass* UOWTActorToolTargetFactory::GetPrivateStaticClass()
{
	using TClass = UOWTActorToolTargetFactory;
	if (!Z_Registration_Info_UClass_UOWTActorToolTargetFactory.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTActorToolTargetFactory"),
			Z_Registration_Info_UClass_UOWTActorToolTargetFactory.InnerSingleton,
			StaticRegisterNativesUOWTActorToolTargetFactory,
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
	return Z_Registration_Info_UClass_UOWTActorToolTargetFactory.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTActorToolTargetFactory_NoRegister()
{
	return UOWTActorToolTargetFactory::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTActorToolTargetFactory_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Targets/OWTActorToolTarget.h" },
		{ "ModuleRelativePath", "Public/Targets/OWTActorToolTarget.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTActorToolTargetFactory constinit property declarations ***************
// ********** End Class UOWTActorToolTargetFactory constinit property declarations *****************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTActorToolTargetFactory>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTActorToolTargetFactory_Statics
UObject* (*const Z_Construct_UClass_UOWTActorToolTargetFactory_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UToolTargetFactory,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTActorToolTargetFactory_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTActorToolTargetFactory_Statics::ClassParams = {
	&UOWTActorToolTargetFactory::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTActorToolTargetFactory_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTActorToolTargetFactory_Statics::Class_MetaDataParams)
};
void UOWTActorToolTargetFactory::StaticRegisterNativesUOWTActorToolTargetFactory()
{
}
UClass* Z_Construct_UClass_UOWTActorToolTargetFactory()
{
	if (!Z_Registration_Info_UClass_UOWTActorToolTargetFactory.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTActorToolTargetFactory.OuterSingleton, Z_Construct_UClass_UOWTActorToolTargetFactory_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTActorToolTargetFactory.OuterSingleton;
}
UOWTActorToolTargetFactory::UOWTActorToolTargetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTActorToolTargetFactory);
UOWTActorToolTargetFactory::~UOWTActorToolTargetFactory() {}
// ********** End Class UOWTActorToolTargetFactory *************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Targets_OWTActorToolTarget_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTActorToolTarget, UOWTActorToolTarget::StaticClass, TEXT("UOWTActorToolTarget"), &Z_Registration_Info_UClass_UOWTActorToolTarget, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTActorToolTarget), 619143218U) },
		{ Z_Construct_UClass_UOWTActorToolTargetFactory, UOWTActorToolTargetFactory::StaticClass, TEXT("UOWTActorToolTargetFactory"), &Z_Registration_Info_UClass_UOWTActorToolTargetFactory, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTActorToolTargetFactory), 3641415158U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Targets_OWTActorToolTarget_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Targets_OWTActorToolTarget_h__Script_VTBOWTEditor_2163362127{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Targets_OWTActorToolTarget_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Targets_OWTActorToolTarget_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
