// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Duplication/OWTDuplicationRestoreComponent.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTDuplicationRestoreComponent() {}

// ********** Begin Cross Module References ********************************************************
ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTDuplicationRestoreComponent();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTDuplicationRestoreComponent_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UOWTDuplicationRestoreComponent ******************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTDuplicationRestoreComponent;
UClass* UOWTDuplicationRestoreComponent::GetPrivateStaticClass()
{
	using TClass = UOWTDuplicationRestoreComponent;
	if (!Z_Registration_Info_UClass_UOWTDuplicationRestoreComponent.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTDuplicationRestoreComponent"),
			Z_Registration_Info_UClass_UOWTDuplicationRestoreComponent.InnerSingleton,
			StaticRegisterNativesUOWTDuplicationRestoreComponent,
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
	return Z_Registration_Info_UClass_UOWTDuplicationRestoreComponent.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTDuplicationRestoreComponent_NoRegister()
{
	return UOWTDuplicationRestoreComponent::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTDuplicationRestoreComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Synchronous bridge between construction and actor initialization. Never retained by the duplicate. */" },
#endif
		{ "IncludePath", "Duplication/OWTDuplicationRestoreComponent.h" },
		{ "IsBlueprintBase", "false" },
		{ "ModuleRelativePath", "Private/Duplication/OWTDuplicationRestoreComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Synchronous bridge between construction and actor initialization. Never retained by the duplicate." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTDuplicationRestoreComponent constinit property declarations **********
// ********** End Class UOWTDuplicationRestoreComponent constinit property declarations ************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTDuplicationRestoreComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTDuplicationRestoreComponent_Statics
UObject* (*const Z_Construct_UClass_UOWTDuplicationRestoreComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UActorComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTDuplicationRestoreComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTDuplicationRestoreComponent_Statics::ClassParams = {
	&UOWTDuplicationRestoreComponent::StaticClass,
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
	0x00A000ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTDuplicationRestoreComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTDuplicationRestoreComponent_Statics::Class_MetaDataParams)
};
void UOWTDuplicationRestoreComponent::StaticRegisterNativesUOWTDuplicationRestoreComponent()
{
}
UClass* Z_Construct_UClass_UOWTDuplicationRestoreComponent()
{
	if (!Z_Registration_Info_UClass_UOWTDuplicationRestoreComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTDuplicationRestoreComponent.OuterSingleton, Z_Construct_UClass_UOWTDuplicationRestoreComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTDuplicationRestoreComponent.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTDuplicationRestoreComponent);
UOWTDuplicationRestoreComponent::~UOWTDuplicationRestoreComponent() {}
// ********** End Class UOWTDuplicationRestoreComponent ********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Duplication_OWTDuplicationRestoreComponent_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTDuplicationRestoreComponent, UOWTDuplicationRestoreComponent::StaticClass, TEXT("UOWTDuplicationRestoreComponent"), &Z_Registration_Info_UClass_UOWTDuplicationRestoreComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTDuplicationRestoreComponent), 2955272584U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Duplication_OWTDuplicationRestoreComponent_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Duplication_OWTDuplicationRestoreComponent_h__Script_VTBOWTEditor_3391104681{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Duplication_OWTDuplicationRestoreComponent_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Duplication_OWTDuplicationRestoreComponent_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
