// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Tests/OWTPCGTestGraph.h"
#include "StructUtils/PropertyBag.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTPCGTestGraph() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FInstancedPropertyBag();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
PCG_API UClass* Z_Construct_UClass_UPCGComponent_NoRegister();
PCG_API UClass* Z_Construct_UClass_UPCGGraph();
PCG_API UClass* Z_Construct_UClass_UPCGSchedulingPolicyBase();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTPCGTestGraph();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTPCGTestGraph_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTPCGTestOpaquePolicy();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTPCGTestOpaquePolicy_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTPCGTestPolicy();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTPCGTestPolicy_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTPCGTestPolicyData();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTPCGTestPolicyData_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UOWTPCGTestPolicyData ****************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTPCGTestPolicyData;
UClass* UOWTPCGTestPolicyData::GetPrivateStaticClass()
{
	using TClass = UOWTPCGTestPolicyData;
	if (!Z_Registration_Info_UClass_UOWTPCGTestPolicyData.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTPCGTestPolicyData"),
			Z_Registration_Info_UClass_UOWTPCGTestPolicyData.InnerSingleton,
			StaticRegisterNativesUOWTPCGTestPolicyData,
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
	return Z_Registration_Info_UClass_UOWTPCGTestPolicyData.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTPCGTestPolicyData_NoRegister()
{
	return UOWTPCGTestPolicyData::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTPCGTestPolicyData_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tests/OWTPCGTestGraph.h" },
		{ "ModuleRelativePath", "Private/Tests/OWTPCGTestGraph.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Actor_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTPCGTestGraph.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Private/Tests/OWTPCGTestGraph.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTPCGTestPolicyData constinit property declarations ********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Actor;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UOWTPCGTestPolicyData constinit property declarations **********************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTPCGTestPolicyData>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTPCGTestPolicyData_Statics

// ********** Begin Class UOWTPCGTestPolicyData Property Definitions *******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UOWTPCGTestPolicyData_Statics::NewProp_Actor = { "Actor", nullptr, (EPropertyFlags)0x0114000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTPCGTestPolicyData, Actor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Actor_MetaData), NewProp_Actor_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UOWTPCGTestPolicyData_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0114000000080008, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTPCGTestPolicyData, Component), Z_Construct_UClass_UPCGComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Component_MetaData), NewProp_Component_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOWTPCGTestPolicyData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTPCGTestPolicyData_Statics::NewProp_Actor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTPCGTestPolicyData_Statics::NewProp_Component,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTPCGTestPolicyData_Statics::PropPointers) < 2048);
// ********** End Class UOWTPCGTestPolicyData Property Definitions *********************************
UObject* (*const Z_Construct_UClass_UOWTPCGTestPolicyData_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTPCGTestPolicyData_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTPCGTestPolicyData_Statics::ClassParams = {
	&UOWTPCGTestPolicyData::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UOWTPCGTestPolicyData_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UOWTPCGTestPolicyData_Statics::PropPointers),
	0,
	0x00A010A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTPCGTestPolicyData_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTPCGTestPolicyData_Statics::Class_MetaDataParams)
};
void UOWTPCGTestPolicyData::StaticRegisterNativesUOWTPCGTestPolicyData()
{
}
UClass* Z_Construct_UClass_UOWTPCGTestPolicyData()
{
	if (!Z_Registration_Info_UClass_UOWTPCGTestPolicyData.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTPCGTestPolicyData.OuterSingleton, Z_Construct_UClass_UOWTPCGTestPolicyData_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTPCGTestPolicyData.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTPCGTestPolicyData);
UOWTPCGTestPolicyData::~UOWTPCGTestPolicyData() {}
// ********** End Class UOWTPCGTestPolicyData ******************************************************

// ********** Begin Class UOWTPCGTestPolicy ********************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTPCGTestPolicy;
UClass* UOWTPCGTestPolicy::GetPrivateStaticClass()
{
	using TClass = UOWTPCGTestPolicy;
	if (!Z_Registration_Info_UClass_UOWTPCGTestPolicy.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTPCGTestPolicy"),
			Z_Registration_Info_UClass_UOWTPCGTestPolicy.InnerSingleton,
			StaticRegisterNativesUOWTPCGTestPolicy,
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
	return Z_Registration_Info_UClass_UOWTPCGTestPolicy.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTPCGTestPolicy_NoRegister()
{
	return UOWTPCGTestPolicy::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTPCGTestPolicy_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tests/OWTPCGTestGraph.h" },
		{ "ModuleRelativePath", "Private/Tests/OWTPCGTestGraph.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Actor_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTPCGTestGraph.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Data_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Private/Tests/OWTPCGTestGraph.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTPCGTestPolicy constinit property declarations ************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Actor;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Data;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UOWTPCGTestPolicy constinit property declarations **************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTPCGTestPolicy>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTPCGTestPolicy_Statics

// ********** Begin Class UOWTPCGTestPolicy Property Definitions ***********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UOWTPCGTestPolicy_Statics::NewProp_Actor = { "Actor", nullptr, (EPropertyFlags)0x0114000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTPCGTestPolicy, Actor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Actor_MetaData), NewProp_Actor_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UOWTPCGTestPolicy_Statics::NewProp_Data = { "Data", nullptr, (EPropertyFlags)0x0116000000080008, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTPCGTestPolicy, Data), Z_Construct_UClass_UOWTPCGTestPolicyData_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Data_MetaData), NewProp_Data_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOWTPCGTestPolicy_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTPCGTestPolicy_Statics::NewProp_Actor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTPCGTestPolicy_Statics::NewProp_Data,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTPCGTestPolicy_Statics::PropPointers) < 2048);
// ********** End Class UOWTPCGTestPolicy Property Definitions *************************************
UObject* (*const Z_Construct_UClass_UOWTPCGTestPolicy_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UPCGSchedulingPolicyBase,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTPCGTestPolicy_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTPCGTestPolicy_Statics::ClassParams = {
	&UOWTPCGTestPolicy::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UOWTPCGTestPolicy_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UOWTPCGTestPolicy_Statics::PropPointers),
	0,
	0x008000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTPCGTestPolicy_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTPCGTestPolicy_Statics::Class_MetaDataParams)
};
void UOWTPCGTestPolicy::StaticRegisterNativesUOWTPCGTestPolicy()
{
}
UClass* Z_Construct_UClass_UOWTPCGTestPolicy()
{
	if (!Z_Registration_Info_UClass_UOWTPCGTestPolicy.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTPCGTestPolicy.OuterSingleton, Z_Construct_UClass_UOWTPCGTestPolicy_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTPCGTestPolicy.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTPCGTestPolicy);
UOWTPCGTestPolicy::~UOWTPCGTestPolicy() {}
// ********** End Class UOWTPCGTestPolicy **********************************************************

// ********** Begin Class UOWTPCGTestOpaquePolicy **************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTPCGTestOpaquePolicy;
UClass* UOWTPCGTestOpaquePolicy::GetPrivateStaticClass()
{
	using TClass = UOWTPCGTestOpaquePolicy;
	if (!Z_Registration_Info_UClass_UOWTPCGTestOpaquePolicy.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTPCGTestOpaquePolicy"),
			Z_Registration_Info_UClass_UOWTPCGTestOpaquePolicy.InnerSingleton,
			StaticRegisterNativesUOWTPCGTestOpaquePolicy,
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
	return Z_Registration_Info_UClass_UOWTPCGTestOpaquePolicy.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTPCGTestOpaquePolicy_NoRegister()
{
	return UOWTPCGTestOpaquePolicy::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTPCGTestOpaquePolicy_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tests/OWTPCGTestGraph.h" },
		{ "ModuleRelativePath", "Private/Tests/OWTPCGTestGraph.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Opaque_MetaData[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTPCGTestGraph.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTPCGTestOpaquePolicy constinit property declarations ******************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Opaque;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UOWTPCGTestOpaquePolicy constinit property declarations ********************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTPCGTestOpaquePolicy>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTPCGTestOpaquePolicy_Statics

// ********** Begin Class UOWTPCGTestOpaquePolicy Property Definitions *****************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UOWTPCGTestOpaquePolicy_Statics::NewProp_Opaque = { "Opaque", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTPCGTestOpaquePolicy, Opaque), Z_Construct_UScriptStruct_FInstancedPropertyBag, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Opaque_MetaData), NewProp_Opaque_MetaData) }; // 1261298821
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOWTPCGTestOpaquePolicy_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTPCGTestOpaquePolicy_Statics::NewProp_Opaque,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTPCGTestOpaquePolicy_Statics::PropPointers) < 2048);
// ********** End Class UOWTPCGTestOpaquePolicy Property Definitions *******************************
UObject* (*const Z_Construct_UClass_UOWTPCGTestOpaquePolicy_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UOWTPCGTestPolicy,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTPCGTestOpaquePolicy_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTPCGTestOpaquePolicy_Statics::ClassParams = {
	&UOWTPCGTestOpaquePolicy::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UOWTPCGTestOpaquePolicy_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UOWTPCGTestOpaquePolicy_Statics::PropPointers),
	0,
	0x008000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTPCGTestOpaquePolicy_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTPCGTestOpaquePolicy_Statics::Class_MetaDataParams)
};
void UOWTPCGTestOpaquePolicy::StaticRegisterNativesUOWTPCGTestOpaquePolicy()
{
}
UClass* Z_Construct_UClass_UOWTPCGTestOpaquePolicy()
{
	if (!Z_Registration_Info_UClass_UOWTPCGTestOpaquePolicy.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTPCGTestOpaquePolicy.OuterSingleton, Z_Construct_UClass_UOWTPCGTestOpaquePolicy_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTPCGTestOpaquePolicy.OuterSingleton;
}
UOWTPCGTestOpaquePolicy::UOWTPCGTestOpaquePolicy() {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTPCGTestOpaquePolicy);
UOWTPCGTestOpaquePolicy::~UOWTPCGTestOpaquePolicy() {}
// ********** End Class UOWTPCGTestOpaquePolicy ****************************************************

// ********** Begin Class UOWTPCGTestGraph *********************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTPCGTestGraph;
UClass* UOWTPCGTestGraph::GetPrivateStaticClass()
{
	using TClass = UOWTPCGTestGraph;
	if (!Z_Registration_Info_UClass_UOWTPCGTestGraph.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTPCGTestGraph"),
			Z_Registration_Info_UClass_UOWTPCGTestGraph.InnerSingleton,
			StaticRegisterNativesUOWTPCGTestGraph,
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
	return Z_Registration_Info_UClass_UOWTPCGTestGraph.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTPCGTestGraph_NoRegister()
{
	return UOWTPCGTestGraph::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTPCGTestGraph_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Native test fixture exposes typed graph configuration without reflection strings or engine edits. */" },
#endif
		{ "HideCategories", "Object" },
		{ "IncludePath", "Tests/OWTPCGTestGraph.h" },
		{ "ModuleRelativePath", "Private/Tests/OWTPCGTestGraph.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Native test fixture exposes typed graph configuration without reflection strings or engine edits." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTPCGTestGraph constinit property declarations *************************
// ********** End Class UOWTPCGTestGraph constinit property declarations ***************************
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTPCGTestGraph>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTPCGTestGraph_Statics
UObject* (*const Z_Construct_UClass_UOWTPCGTestGraph_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UPCGGraph,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTPCGTestGraph_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTPCGTestGraph_Statics::ClassParams = {
	&UOWTPCGTestGraph::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTPCGTestGraph_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTPCGTestGraph_Statics::Class_MetaDataParams)
};
void UOWTPCGTestGraph::StaticRegisterNativesUOWTPCGTestGraph()
{
}
UClass* Z_Construct_UClass_UOWTPCGTestGraph()
{
	if (!Z_Registration_Info_UClass_UOWTPCGTestGraph.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTPCGTestGraph.OuterSingleton, Z_Construct_UClass_UOWTPCGTestGraph_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTPCGTestGraph.OuterSingleton;
}
UOWTPCGTestGraph::UOWTPCGTestGraph(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTPCGTestGraph);
UOWTPCGTestGraph::~UOWTPCGTestGraph() {}
// ********** End Class UOWTPCGTestGraph ***********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTPCGTestGraph_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTPCGTestPolicyData, UOWTPCGTestPolicyData::StaticClass, TEXT("UOWTPCGTestPolicyData"), &Z_Registration_Info_UClass_UOWTPCGTestPolicyData, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTPCGTestPolicyData), 2530582038U) },
		{ Z_Construct_UClass_UOWTPCGTestPolicy, UOWTPCGTestPolicy::StaticClass, TEXT("UOWTPCGTestPolicy"), &Z_Registration_Info_UClass_UOWTPCGTestPolicy, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTPCGTestPolicy), 2665294009U) },
		{ Z_Construct_UClass_UOWTPCGTestOpaquePolicy, UOWTPCGTestOpaquePolicy::StaticClass, TEXT("UOWTPCGTestOpaquePolicy"), &Z_Registration_Info_UClass_UOWTPCGTestOpaquePolicy, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTPCGTestOpaquePolicy), 321098770U) },
		{ Z_Construct_UClass_UOWTPCGTestGraph, UOWTPCGTestGraph::StaticClass, TEXT("UOWTPCGTestGraph"), &Z_Registration_Info_UClass_UOWTPCGTestGraph, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTPCGTestGraph), 2198185822U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTPCGTestGraph_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTPCGTestGraph_h__Script_VTBOWTEditor_3926995689{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTPCGTestGraph_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Private_Tests_OWTPCGTestGraph_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
