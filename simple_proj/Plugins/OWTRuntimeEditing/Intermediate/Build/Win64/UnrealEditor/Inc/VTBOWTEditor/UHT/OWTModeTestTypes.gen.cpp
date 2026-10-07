// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Tests/OWTModeTestTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTModeTestTypes() {}

// ********** Begin Cross Module References ********************************************************
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveTool();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolBuilder();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTModeNullBuilder();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTModeNullBuilder_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTModeRejectBuilder();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTModeRejectBuilder_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTModeSetupCancelBuilder();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTModeSetupCancelBuilder_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTModeTestBuilder();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTModeTestBuilder_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTModeTestTool();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTModeTestTool_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UOWTModeTestTool *********************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTModeTestTool;
UClass* UOWTModeTestTool::GetPrivateStaticClass()
{
	using TClass = UOWTModeTestTool;
	if (!Z_Registration_Info_UClass_UOWTModeTestTool.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTModeTestTool"),
			Z_Registration_Info_UClass_UOWTModeTestTool.InnerSingleton,
			StaticRegisterNativesUOWTModeTestTool,
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
	return Z_Registration_Info_UClass_UOWTModeTestTool.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTModeTestTool_NoRegister()
{
	return UOWTModeTestTool::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTModeTestTool_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tests/OWTModeTestTypes.h" },
		{ "ModuleRelativePath", "Private/Tests/OWTModeTestTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTModeTestTool constinit property declarations *************************
// ********** End Class UOWTModeTestTool constinit property declarations ***************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTModeTestTool>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTModeTestTool_Statics
UObject* (*const Z_Construct_UClass_UOWTModeTestTool_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveTool,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTModeTestTool_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTModeTestTool_Statics::ClassParams = {
	&UOWTModeTestTool::StaticClass,
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
	0x000000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTModeTestTool_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTModeTestTool_Statics::Class_MetaDataParams)
};
void UOWTModeTestTool::StaticRegisterNativesUOWTModeTestTool()
{
}
UClass* Z_Construct_UClass_UOWTModeTestTool()
{
	if (!Z_Registration_Info_UClass_UOWTModeTestTool.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTModeTestTool.OuterSingleton, Z_Construct_UClass_UOWTModeTestTool_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTModeTestTool.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTModeTestTool);
UOWTModeTestTool::~UOWTModeTestTool() {}
// ********** End Class UOWTModeTestTool ***********************************************************

// ********** Begin Class UOWTModeTestBuilder ******************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTModeTestBuilder;
UClass* UOWTModeTestBuilder::GetPrivateStaticClass()
{
	using TClass = UOWTModeTestBuilder;
	if (!Z_Registration_Info_UClass_UOWTModeTestBuilder.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTModeTestBuilder"),
			Z_Registration_Info_UClass_UOWTModeTestBuilder.InnerSingleton,
			StaticRegisterNativesUOWTModeTestBuilder,
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
	return Z_Registration_Info_UClass_UOWTModeTestBuilder.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTModeTestBuilder_NoRegister()
{
	return UOWTModeTestBuilder::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTModeTestBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tests/OWTModeTestTypes.h" },
		{ "ModuleRelativePath", "Private/Tests/OWTModeTestTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTModeTestBuilder constinit property declarations **********************
// ********** End Class UOWTModeTestBuilder constinit property declarations ************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTModeTestBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTModeTestBuilder_Statics
UObject* (*const Z_Construct_UClass_UOWTModeTestBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveToolBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTModeTestBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTModeTestBuilder_Statics::ClassParams = {
	&UOWTModeTestBuilder::StaticClass,
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
	0x000000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTModeTestBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTModeTestBuilder_Statics::Class_MetaDataParams)
};
void UOWTModeTestBuilder::StaticRegisterNativesUOWTModeTestBuilder()
{
}
UClass* Z_Construct_UClass_UOWTModeTestBuilder()
{
	if (!Z_Registration_Info_UClass_UOWTModeTestBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTModeTestBuilder.OuterSingleton, Z_Construct_UClass_UOWTModeTestBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTModeTestBuilder.OuterSingleton;
}
UOWTModeTestBuilder::UOWTModeTestBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTModeTestBuilder);
UOWTModeTestBuilder::~UOWTModeTestBuilder() {}
// ********** End Class UOWTModeTestBuilder ********************************************************

// ********** Begin Class UOWTModeRejectBuilder ****************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTModeRejectBuilder;
UClass* UOWTModeRejectBuilder::GetPrivateStaticClass()
{
	using TClass = UOWTModeRejectBuilder;
	if (!Z_Registration_Info_UClass_UOWTModeRejectBuilder.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTModeRejectBuilder"),
			Z_Registration_Info_UClass_UOWTModeRejectBuilder.InnerSingleton,
			StaticRegisterNativesUOWTModeRejectBuilder,
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
	return Z_Registration_Info_UClass_UOWTModeRejectBuilder.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTModeRejectBuilder_NoRegister()
{
	return UOWTModeRejectBuilder::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTModeRejectBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tests/OWTModeTestTypes.h" },
		{ "ModuleRelativePath", "Private/Tests/OWTModeTestTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTModeRejectBuilder constinit property declarations ********************
// ********** End Class UOWTModeRejectBuilder constinit property declarations **********************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTModeRejectBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTModeRejectBuilder_Statics
UObject* (*const Z_Construct_UClass_UOWTModeRejectBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UOWTModeTestBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTModeRejectBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTModeRejectBuilder_Statics::ClassParams = {
	&UOWTModeRejectBuilder::StaticClass,
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
	0x000000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTModeRejectBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTModeRejectBuilder_Statics::Class_MetaDataParams)
};
void UOWTModeRejectBuilder::StaticRegisterNativesUOWTModeRejectBuilder()
{
}
UClass* Z_Construct_UClass_UOWTModeRejectBuilder()
{
	if (!Z_Registration_Info_UClass_UOWTModeRejectBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTModeRejectBuilder.OuterSingleton, Z_Construct_UClass_UOWTModeRejectBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTModeRejectBuilder.OuterSingleton;
}
UOWTModeRejectBuilder::UOWTModeRejectBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTModeRejectBuilder);
UOWTModeRejectBuilder::~UOWTModeRejectBuilder() {}
// ********** End Class UOWTModeRejectBuilder ******************************************************

// ********** Begin Class UOWTModeNullBuilder ******************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTModeNullBuilder;
UClass* UOWTModeNullBuilder::GetPrivateStaticClass()
{
	using TClass = UOWTModeNullBuilder;
	if (!Z_Registration_Info_UClass_UOWTModeNullBuilder.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTModeNullBuilder"),
			Z_Registration_Info_UClass_UOWTModeNullBuilder.InnerSingleton,
			StaticRegisterNativesUOWTModeNullBuilder,
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
	return Z_Registration_Info_UClass_UOWTModeNullBuilder.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTModeNullBuilder_NoRegister()
{
	return UOWTModeNullBuilder::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTModeNullBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tests/OWTModeTestTypes.h" },
		{ "ModuleRelativePath", "Private/Tests/OWTModeTestTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTModeNullBuilder constinit property declarations **********************
// ********** End Class UOWTModeNullBuilder constinit property declarations ************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTModeNullBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTModeNullBuilder_Statics
UObject* (*const Z_Construct_UClass_UOWTModeNullBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UOWTModeTestBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTModeNullBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTModeNullBuilder_Statics::ClassParams = {
	&UOWTModeNullBuilder::StaticClass,
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
	0x000000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTModeNullBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTModeNullBuilder_Statics::Class_MetaDataParams)
};
void UOWTModeNullBuilder::StaticRegisterNativesUOWTModeNullBuilder()
{
}
UClass* Z_Construct_UClass_UOWTModeNullBuilder()
{
	if (!Z_Registration_Info_UClass_UOWTModeNullBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTModeNullBuilder.OuterSingleton, Z_Construct_UClass_UOWTModeNullBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTModeNullBuilder.OuterSingleton;
}
UOWTModeNullBuilder::UOWTModeNullBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTModeNullBuilder);
UOWTModeNullBuilder::~UOWTModeNullBuilder() {}
// ********** End Class UOWTModeNullBuilder ********************************************************

// ********** Begin Class UOWTModeSetupCancelBuilder ***********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTModeSetupCancelBuilder;
UClass* UOWTModeSetupCancelBuilder::GetPrivateStaticClass()
{
	using TClass = UOWTModeSetupCancelBuilder;
	if (!Z_Registration_Info_UClass_UOWTModeSetupCancelBuilder.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTModeSetupCancelBuilder"),
			Z_Registration_Info_UClass_UOWTModeSetupCancelBuilder.InnerSingleton,
			StaticRegisterNativesUOWTModeSetupCancelBuilder,
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
	return Z_Registration_Info_UClass_UOWTModeSetupCancelBuilder.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTModeSetupCancelBuilder_NoRegister()
{
	return UOWTModeSetupCancelBuilder::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTModeSetupCancelBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tests/OWTModeTestTypes.h" },
		{ "ModuleRelativePath", "Private/Tests/OWTModeTestTypes.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTModeSetupCancelBuilder constinit property declarations ***************
// ********** End Class UOWTModeSetupCancelBuilder constinit property declarations *****************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTModeSetupCancelBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTModeSetupCancelBuilder_Statics
UObject* (*const Z_Construct_UClass_UOWTModeSetupCancelBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UOWTModeTestBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTModeSetupCancelBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTModeSetupCancelBuilder_Statics::ClassParams = {
	&UOWTModeSetupCancelBuilder::StaticClass,
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
	0x000000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTModeSetupCancelBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTModeSetupCancelBuilder_Statics::Class_MetaDataParams)
};
void UOWTModeSetupCancelBuilder::StaticRegisterNativesUOWTModeSetupCancelBuilder()
{
}
UClass* Z_Construct_UClass_UOWTModeSetupCancelBuilder()
{
	if (!Z_Registration_Info_UClass_UOWTModeSetupCancelBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTModeSetupCancelBuilder.OuterSingleton, Z_Construct_UClass_UOWTModeSetupCancelBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTModeSetupCancelBuilder.OuterSingleton;
}
UOWTModeSetupCancelBuilder::UOWTModeSetupCancelBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTModeSetupCancelBuilder);
UOWTModeSetupCancelBuilder::~UOWTModeSetupCancelBuilder() {}
// ********** End Class UOWTModeSetupCancelBuilder *************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTModeTestTypes_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTModeTestTool, UOWTModeTestTool::StaticClass, TEXT("UOWTModeTestTool"), &Z_Registration_Info_UClass_UOWTModeTestTool, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTModeTestTool), 1081423036U) },
		{ Z_Construct_UClass_UOWTModeTestBuilder, UOWTModeTestBuilder::StaticClass, TEXT("UOWTModeTestBuilder"), &Z_Registration_Info_UClass_UOWTModeTestBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTModeTestBuilder), 4001984872U) },
		{ Z_Construct_UClass_UOWTModeRejectBuilder, UOWTModeRejectBuilder::StaticClass, TEXT("UOWTModeRejectBuilder"), &Z_Registration_Info_UClass_UOWTModeRejectBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTModeRejectBuilder), 674522581U) },
		{ Z_Construct_UClass_UOWTModeNullBuilder, UOWTModeNullBuilder::StaticClass, TEXT("UOWTModeNullBuilder"), &Z_Registration_Info_UClass_UOWTModeNullBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTModeNullBuilder), 652841993U) },
		{ Z_Construct_UClass_UOWTModeSetupCancelBuilder, UOWTModeSetupCancelBuilder::StaticClass, TEXT("UOWTModeSetupCancelBuilder"), &Z_Registration_Info_UClass_UOWTModeSetupCancelBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTModeSetupCancelBuilder), 878504028U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTModeTestTypes_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTModeTestTypes_h__Script_VTBOWTEditor_2306081213{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTModeTestTypes_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTModeTestTypes_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
