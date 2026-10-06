// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Gizmos/Base/VTBOWTTransformGizmoBehavior.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTTransformGizmoBehavior() {}

// ********** Begin Cross Module References ********************************************************
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UAxisAngleGizmo();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UAxisPositionGizmo();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UClickDragInputBehavior();
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveGizmoBuilder();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmo();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmo_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmo();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmo_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTTransformGizmoBehavior();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVTBOWTTransformGizmoBehavior ********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior;
UClass* UVTBOWTTransformGizmoBehavior::GetPrivateStaticClass()
{
	using TClass = UVTBOWTTransformGizmoBehavior;
	if (!Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTTransformGizmoBehavior"),
			Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior.InnerSingleton,
			StaticRegisterNativesUVTBOWTTransformGizmoBehavior,
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
	return Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_NoRegister()
{
	return UVTBOWTTransformGizmoBehavior::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Gizmos/Base/VTBOWTTransformGizmoBehavior.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Base/VTBOWTTransformGizmoBehavior.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTTransformGizmoBehavior constinit property declarations ************
// ********** End Class UVTBOWTTransformGizmoBehavior constinit property declarations **************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTTransformGizmoBehavior>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UClickDragInputBehavior,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics::ClassParams = {
	&UVTBOWTTransformGizmoBehavior::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics::Class_MetaDataParams)
};
void UVTBOWTTransformGizmoBehavior::StaticRegisterNativesUVTBOWTTransformGizmoBehavior()
{
}
UClass* Z_Construct_UClass_UVTBOWTTransformGizmoBehavior()
{
	if (!Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior.OuterSingleton, Z_Construct_UClass_UVTBOWTTransformGizmoBehavior_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTTransformGizmoBehavior);
UVTBOWTTransformGizmoBehavior::~UVTBOWTTransformGizmoBehavior() {}
// ********** End Class UVTBOWTTransformGizmoBehavior **********************************************

// ********** Begin Class UVTBOWTAxisPositionGizmo *************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo;
UClass* UVTBOWTAxisPositionGizmo::GetPrivateStaticClass()
{
	using TClass = UVTBOWTAxisPositionGizmo;
	if (!Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTAxisPositionGizmo"),
			Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo.InnerSingleton,
			StaticRegisterNativesUVTBOWTAxisPositionGizmo,
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
	return Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmo_NoRegister()
{
	return UVTBOWTAxisPositionGizmo::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Gizmos/Base/VTBOWTTransformGizmoBehavior.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Base/VTBOWTTransformGizmoBehavior.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTAxisPositionGizmo constinit property declarations *****************
// ********** End Class UVTBOWTAxisPositionGizmo constinit property declarations *******************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTAxisPositionGizmo>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UAxisPositionGizmo,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics::ClassParams = {
	&UVTBOWTAxisPositionGizmo::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics::Class_MetaDataParams)
};
void UVTBOWTAxisPositionGizmo::StaticRegisterNativesUVTBOWTAxisPositionGizmo()
{
}
UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmo()
{
	if (!Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo.OuterSingleton, Z_Construct_UClass_UVTBOWTAxisPositionGizmo_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo.OuterSingleton;
}
UVTBOWTAxisPositionGizmo::UVTBOWTAxisPositionGizmo() {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTAxisPositionGizmo);
UVTBOWTAxisPositionGizmo::~UVTBOWTAxisPositionGizmo() {}
// ********** End Class UVTBOWTAxisPositionGizmo ***************************************************

// ********** Begin Class UVTBOWTAxisAngleGizmo ****************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo;
UClass* UVTBOWTAxisAngleGizmo::GetPrivateStaticClass()
{
	using TClass = UVTBOWTAxisAngleGizmo;
	if (!Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTAxisAngleGizmo"),
			Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo.InnerSingleton,
			StaticRegisterNativesUVTBOWTAxisAngleGizmo,
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
	return Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmo_NoRegister()
{
	return UVTBOWTAxisAngleGizmo::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Gizmos/Base/VTBOWTTransformGizmoBehavior.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Base/VTBOWTTransformGizmoBehavior.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTAxisAngleGizmo constinit property declarations ********************
// ********** End Class UVTBOWTAxisAngleGizmo constinit property declarations **********************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTAxisAngleGizmo>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UAxisAngleGizmo,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics::ClassParams = {
	&UVTBOWTAxisAngleGizmo::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics::Class_MetaDataParams)
};
void UVTBOWTAxisAngleGizmo::StaticRegisterNativesUVTBOWTAxisAngleGizmo()
{
}
UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmo()
{
	if (!Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo.OuterSingleton, Z_Construct_UClass_UVTBOWTAxisAngleGizmo_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTAxisAngleGizmo);
UVTBOWTAxisAngleGizmo::~UVTBOWTAxisAngleGizmo() {}
// ********** End Class UVTBOWTAxisAngleGizmo ******************************************************

// ********** Begin Class UVTBOWTAxisPositionGizmoBuilder ******************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder;
UClass* UVTBOWTAxisPositionGizmoBuilder::GetPrivateStaticClass()
{
	using TClass = UVTBOWTAxisPositionGizmoBuilder;
	if (!Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTAxisPositionGizmoBuilder"),
			Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder.InnerSingleton,
			StaticRegisterNativesUVTBOWTAxisPositionGizmoBuilder,
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
	return Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_NoRegister()
{
	return UVTBOWTAxisPositionGizmoBuilder::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Gizmos/Base/VTBOWTTransformGizmoBehavior.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Base/VTBOWTTransformGizmoBehavior.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTAxisPositionGizmoBuilder constinit property declarations **********
// ********** End Class UVTBOWTAxisPositionGizmoBuilder constinit property declarations ************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTAxisPositionGizmoBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveGizmoBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics::ClassParams = {
	&UVTBOWTAxisPositionGizmoBuilder::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics::Class_MetaDataParams)
};
void UVTBOWTAxisPositionGizmoBuilder::StaticRegisterNativesUVTBOWTAxisPositionGizmoBuilder()
{
}
UClass* Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder()
{
	if (!Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder.OuterSingleton, Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder.OuterSingleton;
}
UVTBOWTAxisPositionGizmoBuilder::UVTBOWTAxisPositionGizmoBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTAxisPositionGizmoBuilder);
UVTBOWTAxisPositionGizmoBuilder::~UVTBOWTAxisPositionGizmoBuilder() {}
// ********** End Class UVTBOWTAxisPositionGizmoBuilder ********************************************

// ********** Begin Class UVTBOWTAxisAngleGizmoBuilder *********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder;
UClass* UVTBOWTAxisAngleGizmoBuilder::GetPrivateStaticClass()
{
	using TClass = UVTBOWTAxisAngleGizmoBuilder;
	if (!Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTAxisAngleGizmoBuilder"),
			Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder.InnerSingleton,
			StaticRegisterNativesUVTBOWTAxisAngleGizmoBuilder,
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
	return Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_NoRegister()
{
	return UVTBOWTAxisAngleGizmoBuilder::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Gizmos/Base/VTBOWTTransformGizmoBehavior.h" },
		{ "ModuleRelativePath", "Public/Gizmos/Base/VTBOWTTransformGizmoBehavior.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTAxisAngleGizmoBuilder constinit property declarations *************
// ********** End Class UVTBOWTAxisAngleGizmoBuilder constinit property declarations ***************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTAxisAngleGizmoBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveGizmoBuilder,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics::ClassParams = {
	&UVTBOWTAxisAngleGizmoBuilder::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics::Class_MetaDataParams)
};
void UVTBOWTAxisAngleGizmoBuilder::StaticRegisterNativesUVTBOWTAxisAngleGizmoBuilder()
{
}
UClass* Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder()
{
	if (!Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder.OuterSingleton, Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder.OuterSingleton;
}
UVTBOWTAxisAngleGizmoBuilder::UVTBOWTAxisAngleGizmoBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTAxisAngleGizmoBuilder);
UVTBOWTAxisAngleGizmoBuilder::~UVTBOWTAxisAngleGizmoBuilder() {}
// ********** End Class UVTBOWTAxisAngleGizmoBuilder ***********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTTransformGizmoBehavior_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVTBOWTTransformGizmoBehavior, UVTBOWTTransformGizmoBehavior::StaticClass, TEXT("UVTBOWTTransformGizmoBehavior"), &Z_Registration_Info_UClass_UVTBOWTTransformGizmoBehavior, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTTransformGizmoBehavior), 3818230871U) },
		{ Z_Construct_UClass_UVTBOWTAxisPositionGizmo, UVTBOWTAxisPositionGizmo::StaticClass, TEXT("UVTBOWTAxisPositionGizmo"), &Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmo, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTAxisPositionGizmo), 1813165387U) },
		{ Z_Construct_UClass_UVTBOWTAxisAngleGizmo, UVTBOWTAxisAngleGizmo::StaticClass, TEXT("UVTBOWTAxisAngleGizmo"), &Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmo, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTAxisAngleGizmo), 343259500U) },
		{ Z_Construct_UClass_UVTBOWTAxisPositionGizmoBuilder, UVTBOWTAxisPositionGizmoBuilder::StaticClass, TEXT("UVTBOWTAxisPositionGizmoBuilder"), &Z_Registration_Info_UClass_UVTBOWTAxisPositionGizmoBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTAxisPositionGizmoBuilder), 190848540U) },
		{ Z_Construct_UClass_UVTBOWTAxisAngleGizmoBuilder, UVTBOWTAxisAngleGizmoBuilder::StaticClass, TEXT("UVTBOWTAxisAngleGizmoBuilder"), &Z_Registration_Info_UClass_UVTBOWTAxisAngleGizmoBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTAxisAngleGizmoBuilder), 948445282U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTTransformGizmoBehavior_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTTransformGizmoBehavior_h__Script_VTBOWTEditor_1415722876{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTTransformGizmoBehavior_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Gizmos_Base_VTBOWTTransformGizmoBehavior_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
