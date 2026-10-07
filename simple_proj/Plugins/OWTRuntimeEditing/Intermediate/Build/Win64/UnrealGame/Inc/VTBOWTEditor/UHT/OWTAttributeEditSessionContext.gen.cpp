// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Context/OWTAttributeEditSessionContext.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTAttributeEditSessionContext() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeEditMode_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeEditSessionContext();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeEditSessionContext_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UOWTAttributeEditSessionContext ******************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTAttributeEditSessionContext;
UClass* UOWTAttributeEditSessionContext::GetPrivateStaticClass()
{
	using TClass = UOWTAttributeEditSessionContext;
	if (!Z_Registration_Info_UClass_UOWTAttributeEditSessionContext.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTAttributeEditSessionContext"),
			Z_Registration_Info_UClass_UOWTAttributeEditSessionContext.InnerSingleton,
			StaticRegisterNativesUOWTAttributeEditSessionContext,
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
	return Z_Registration_Info_UClass_UOWTAttributeEditSessionContext.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTAttributeEditSessionContext_NoRegister()
{
	return UOWTAttributeEditSessionContext::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTAttributeEditSessionContext_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Context/OWTAttributeEditSessionContext.h" },
		{ "ModuleRelativePath", "Public/Context/OWTAttributeEditSessionContext.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Mode_MetaData[] = {
		{ "ModuleRelativePath", "Public/Context/OWTAttributeEditSessionContext.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTAttributeEditSessionContext constinit property declarations **********
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_Mode;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UOWTAttributeEditSessionContext constinit property declarations ************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTAttributeEditSessionContext>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTAttributeEditSessionContext_Statics

// ********** Begin Class UOWTAttributeEditSessionContext Property Definitions *********************
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UOWTAttributeEditSessionContext_Statics::NewProp_Mode = { "Mode", nullptr, (EPropertyFlags)0x0044000000002000, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditSessionContext, Mode), Z_Construct_UClass_UOWTAttributeEditMode_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Mode_MetaData), NewProp_Mode_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOWTAttributeEditSessionContext_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditSessionContext_Statics::NewProp_Mode,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeEditSessionContext_Statics::PropPointers) < 2048);
// ********** End Class UOWTAttributeEditSessionContext Property Definitions ***********************
UObject* (*const Z_Construct_UClass_UOWTAttributeEditSessionContext_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeEditSessionContext_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTAttributeEditSessionContext_Statics::ClassParams = {
	&UOWTAttributeEditSessionContext::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UOWTAttributeEditSessionContext_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeEditSessionContext_Statics::PropPointers),
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeEditSessionContext_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTAttributeEditSessionContext_Statics::Class_MetaDataParams)
};
void UOWTAttributeEditSessionContext::StaticRegisterNativesUOWTAttributeEditSessionContext()
{
}
UClass* Z_Construct_UClass_UOWTAttributeEditSessionContext()
{
	if (!Z_Registration_Info_UClass_UOWTAttributeEditSessionContext.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTAttributeEditSessionContext.OuterSingleton, Z_Construct_UClass_UOWTAttributeEditSessionContext_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTAttributeEditSessionContext.OuterSingleton;
}
UOWTAttributeEditSessionContext::UOWTAttributeEditSessionContext(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTAttributeEditSessionContext);
UOWTAttributeEditSessionContext::~UOWTAttributeEditSessionContext() {}
// ********** End Class UOWTAttributeEditSessionContext ********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTAttributeEditSessionContext_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTAttributeEditSessionContext, UOWTAttributeEditSessionContext::StaticClass, TEXT("UOWTAttributeEditSessionContext"), &Z_Registration_Info_UClass_UOWTAttributeEditSessionContext, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTAttributeEditSessionContext), 1742274037U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTAttributeEditSessionContext_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTAttributeEditSessionContext_h__Script_VTBOWTEditor_710219779{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTAttributeEditSessionContext_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_OWTAttributeEditSessionContext_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
