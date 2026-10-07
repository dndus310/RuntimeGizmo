// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Tools/OWTAttributeEditTool.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTAttributeEditTool() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FRotator();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveTool();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolBuilder();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolPropertySet();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeEditMode_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeEditTool();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeEditTool_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeEditToolBuilder();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeEditToolBuilder_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTTransformProperties();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTTransformProperties_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTBaseTransformGizmo_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UOWTTransformProperties **************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTTransformProperties;
UClass* UOWTTransformProperties::GetPrivateStaticClass()
{
	using TClass = UOWTTransformProperties;
	if (!Z_Registration_Info_UClass_UOWTTransformProperties.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTTransformProperties"),
			Z_Registration_Info_UClass_UOWTTransformProperties.InnerSingleton,
			StaticRegisterNativesUOWTTransformProperties,
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
	return Z_Registration_Info_UClass_UOWTTransformProperties.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTTransformProperties_NoRegister()
{
	return UOWTTransformProperties::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTTransformProperties_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tools/OWTAttributeEditTool.h" },
		{ "ModuleRelativePath", "Public/Tools/OWTAttributeEditTool.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Location_MetaData[] = {
		{ "Category", "Transform" },
		{ "ModuleRelativePath", "Public/Tools/OWTAttributeEditTool.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Rotation_MetaData[] = {
		{ "Category", "Transform" },
		{ "ModuleRelativePath", "Public/Tools/OWTAttributeEditTool.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Scale_MetaData[] = {
		{ "Category", "Transform" },
		{ "ModuleRelativePath", "Public/Tools/OWTAttributeEditTool.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTTransformProperties constinit property declarations ******************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Location;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Rotation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Scale;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UOWTTransformProperties constinit property declarations ********************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTTransformProperties>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTTransformProperties_Statics

// ********** Begin Class UOWTTransformProperties Property Definitions *****************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOWTTransformProperties_Statics::NewProp_Location = { "Location", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTTransformProperties, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Location_MetaData), NewProp_Location_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOWTTransformProperties_Statics::NewProp_Rotation = { "Rotation", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTTransformProperties, Rotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Rotation_MetaData), NewProp_Rotation_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOWTTransformProperties_Statics::NewProp_Scale = { "Scale", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTTransformProperties, Scale), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Scale_MetaData), NewProp_Scale_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOWTTransformProperties_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTTransformProperties_Statics::NewProp_Location,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTTransformProperties_Statics::NewProp_Rotation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTTransformProperties_Statics::NewProp_Scale,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTTransformProperties_Statics::PropPointers) < 2048);
// ********** End Class UOWTTransformProperties Property Definitions *******************************
UObject* (*const Z_Construct_UClass_UOWTTransformProperties_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveToolPropertySet,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTTransformProperties_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTTransformProperties_Statics::ClassParams = {
	&UOWTTransformProperties::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UOWTTransformProperties_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UOWTTransformProperties_Statics::PropPointers),
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTTransformProperties_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTTransformProperties_Statics::Class_MetaDataParams)
};
void UOWTTransformProperties::StaticRegisterNativesUOWTTransformProperties()
{
}
UClass* Z_Construct_UClass_UOWTTransformProperties()
{
	if (!Z_Registration_Info_UClass_UOWTTransformProperties.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTTransformProperties.OuterSingleton, Z_Construct_UClass_UOWTTransformProperties_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTTransformProperties.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTTransformProperties);
UOWTTransformProperties::~UOWTTransformProperties() {}
// ********** End Class UOWTTransformProperties ****************************************************

// ********** Begin Class UOWTAttributeEditTool ****************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTAttributeEditTool;
UClass* UOWTAttributeEditTool::GetPrivateStaticClass()
{
	using TClass = UOWTAttributeEditTool;
	if (!Z_Registration_Info_UClass_UOWTAttributeEditTool.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTAttributeEditTool"),
			Z_Registration_Info_UClass_UOWTAttributeEditTool.InnerSingleton,
			StaticRegisterNativesUOWTAttributeEditTool,
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
	return Z_Registration_Info_UClass_UOWTAttributeEditTool.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTAttributeEditTool_NoRegister()
{
	return UOWTAttributeEditTool::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTAttributeEditTool_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tools/OWTAttributeEditTool.h" },
		{ "ModuleRelativePath", "Public/Tools/OWTAttributeEditTool.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Mode_MetaData[] = {
		{ "ModuleRelativePath", "Public/Tools/OWTAttributeEditTool.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TransformGizmo_MetaData[] = {
		{ "ModuleRelativePath", "Public/Tools/OWTAttributeEditTool.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Properties_MetaData[] = {
		{ "ModuleRelativePath", "Public/Tools/OWTAttributeEditTool.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTAttributeEditTool constinit property declarations ********************
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_Mode;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TransformGizmo;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Properties;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UOWTAttributeEditTool constinit property declarations **********************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTAttributeEditTool>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTAttributeEditTool_Statics

// ********** Begin Class UOWTAttributeEditTool Property Definitions *******************************
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UOWTAttributeEditTool_Statics::NewProp_Mode = { "Mode", nullptr, (EPropertyFlags)0x0044000000002000, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditTool, Mode), Z_Construct_UClass_UOWTAttributeEditMode_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Mode_MetaData), NewProp_Mode_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UOWTAttributeEditTool_Statics::NewProp_TransformGizmo = { "TransformGizmo", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditTool, TransformGizmo), Z_Construct_UClass_UVTBOWTBaseTransformGizmo_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TransformGizmo_MetaData), NewProp_TransformGizmo_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UOWTAttributeEditTool_Statics::NewProp_Properties = { "Properties", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeEditTool, Properties), Z_Construct_UClass_UOWTTransformProperties_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Properties_MetaData), NewProp_Properties_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOWTAttributeEditTool_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditTool_Statics::NewProp_Mode,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditTool_Statics::NewProp_TransformGizmo,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeEditTool_Statics::NewProp_Properties,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeEditTool_Statics::PropPointers) < 2048);
// ********** End Class UOWTAttributeEditTool Property Definitions *********************************
UObject* (*const Z_Construct_UClass_UOWTAttributeEditTool_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveTool,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeEditTool_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTAttributeEditTool_Statics::ClassParams = {
	&UOWTAttributeEditTool::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UOWTAttributeEditTool_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeEditTool_Statics::PropPointers),
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeEditTool_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTAttributeEditTool_Statics::Class_MetaDataParams)
};
void UOWTAttributeEditTool::StaticRegisterNativesUOWTAttributeEditTool()
{
}
UClass* Z_Construct_UClass_UOWTAttributeEditTool()
{
	if (!Z_Registration_Info_UClass_UOWTAttributeEditTool.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTAttributeEditTool.OuterSingleton, Z_Construct_UClass_UOWTAttributeEditTool_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTAttributeEditTool.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTAttributeEditTool);
UOWTAttributeEditTool::~UOWTAttributeEditTool() {}
// ********** End Class UOWTAttributeEditTool ******************************************************

// ********** Begin Class UOWTAttributeEditToolBuilder *********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTAttributeEditToolBuilder;
UClass* UOWTAttributeEditToolBuilder::GetPrivateStaticClass()
{
	using TClass = UOWTAttributeEditToolBuilder;
	if (!Z_Registration_Info_UClass_UOWTAttributeEditToolBuilder.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTAttributeEditToolBuilder"),
			Z_Registration_Info_UClass_UOWTAttributeEditToolBuilder.InnerSingleton,
			StaticRegisterNativesUOWTAttributeEditToolBuilder,
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
	return Z_Registration_Info_UClass_UOWTAttributeEditToolBuilder.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTAttributeEditToolBuilder_NoRegister()
{
	return UOWTAttributeEditToolBuilder::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTAttributeEditToolBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tools/OWTAttributeEditTool.h" },
		{ "ModuleRelativePath", "Public/Tools/OWTAttributeEditTool.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTAttributeEditToolBuilder constinit property declarations *************
// ********** End Class UOWTAttributeEditToolBuilder constinit property declarations ***************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTAttributeEditToolBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTAttributeEditToolBuilder_Statics
UObject* (*const Z_Construct_UClass_UOWTAttributeEditToolBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveToolBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeEditToolBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTAttributeEditToolBuilder_Statics::ClassParams = {
	&UOWTAttributeEditToolBuilder::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeEditToolBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTAttributeEditToolBuilder_Statics::Class_MetaDataParams)
};
void UOWTAttributeEditToolBuilder::StaticRegisterNativesUOWTAttributeEditToolBuilder()
{
}
UClass* Z_Construct_UClass_UOWTAttributeEditToolBuilder()
{
	if (!Z_Registration_Info_UClass_UOWTAttributeEditToolBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTAttributeEditToolBuilder.OuterSingleton, Z_Construct_UClass_UOWTAttributeEditToolBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTAttributeEditToolBuilder.OuterSingleton;
}
UOWTAttributeEditToolBuilder::UOWTAttributeEditToolBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTAttributeEditToolBuilder);
UOWTAttributeEditToolBuilder::~UOWTAttributeEditToolBuilder() {}
// ********** End Class UOWTAttributeEditToolBuilder ***********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTAttributeEditTool_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTTransformProperties, UOWTTransformProperties::StaticClass, TEXT("UOWTTransformProperties"), &Z_Registration_Info_UClass_UOWTTransformProperties, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTTransformProperties), 3242045702U) },
		{ Z_Construct_UClass_UOWTAttributeEditTool, UOWTAttributeEditTool::StaticClass, TEXT("UOWTAttributeEditTool"), &Z_Registration_Info_UClass_UOWTAttributeEditTool, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTAttributeEditTool), 400973208U) },
		{ Z_Construct_UClass_UOWTAttributeEditToolBuilder, UOWTAttributeEditToolBuilder::StaticClass, TEXT("UOWTAttributeEditToolBuilder"), &Z_Registration_Info_UClass_UOWTAttributeEditToolBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTAttributeEditToolBuilder), 381289438U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTAttributeEditTool_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTAttributeEditTool_h__Script_VTBOWTEditor_1977222777{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTAttributeEditTool_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Tools_OWTAttributeEditTool_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
