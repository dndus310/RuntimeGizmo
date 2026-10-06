// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Tests/OWTEventTestObject.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTEventTestObject() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
OWTEVENTCORE_API UClass* Z_Construct_UClass_UOWTEventTestObject();
OWTEVENTCORE_API UClass* Z_Construct_UClass_UOWTEventTestObject_NoRegister();
UPackage* Z_Construct_UPackage__Script_OWTEventCore();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UOWTEventTestObject Function OnEvent *************************************
struct Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics
{
	struct OWTEventTestObject_eventOnEvent_Parms
	{
		FName Event;
		FString Json;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Private/Tests/OWTEventTestObject.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Json_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function OnEvent constinit property declarations *******************************
	static const UECodeGen_Private::FNamePropertyParams NewProp_Event;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Json;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function OnEvent constinit property declarations *********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function OnEvent Property Definitions ******************************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics::NewProp_Event = { "Event", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTEventTestObject_eventOnEvent_Parms, Event), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics::NewProp_Json = { "Json", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTEventTestObject_eventOnEvent_Parms, Json), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Json_MetaData), NewProp_Json_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics::NewProp_Event,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics::NewProp_Json,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics::PropPointers) < 2048);
// ********** End Function OnEvent Property Definitions ********************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTEventTestObject, nullptr, "OnEvent", 	Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics::OWTEventTestObject_eventOnEvent_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics::OWTEventTestObject_eventOnEvent_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTEventTestObject_OnEvent()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTEventTestObject_OnEvent_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTEventTestObject::execOnEvent)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_Event);
	P_GET_PROPERTY(FStrProperty,Z_Param_Json);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->OnEvent(Z_Param_Event,Z_Param_Json);
	P_NATIVE_END;
}
// ********** End Class UOWTEventTestObject Function OnEvent ***************************************

// ********** Begin Class UOWTEventTestObject ******************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTEventTestObject;
UClass* UOWTEventTestObject::GetPrivateStaticClass()
{
	using TClass = UOWTEventTestObject;
	if (!Z_Registration_Info_UClass_UOWTEventTestObject.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTEventTestObject"),
			Z_Registration_Info_UClass_UOWTEventTestObject.InnerSingleton,
			StaticRegisterNativesUOWTEventTestObject,
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
	return Z_Registration_Info_UClass_UOWTEventTestObject.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTEventTestObject_NoRegister()
{
	return UOWTEventTestObject::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTEventTestObject_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Concrete CoreUObject-only owner/receiver fixture; UObject itself is abstract in UE 5.7. */" },
#endif
		{ "IncludePath", "Tests/OWTEventTestObject.h" },
		{ "IsBlueprintBase", "false" },
		{ "ModuleRelativePath", "Private/Tests/OWTEventTestObject.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Concrete CoreUObject-only owner/receiver fixture; UObject itself is abstract in UE 5.7." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTEventTestObject constinit property declarations **********************
// ********** End Class UOWTEventTestObject constinit property declarations ************************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("OnEvent"), .Pointer = &UOWTEventTestObject::execOnEvent },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UOWTEventTestObject_OnEvent, "OnEvent" }, // 2964805665
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTEventTestObject>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTEventTestObject_Statics
UObject* (*const Z_Construct_UClass_UOWTEventTestObject_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_OWTEventCore,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTEventTestObject_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTEventTestObject_Statics::ClassParams = {
	&UOWTEventTestObject::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x000000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTEventTestObject_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTEventTestObject_Statics::Class_MetaDataParams)
};
void UOWTEventTestObject::StaticRegisterNativesUOWTEventTestObject()
{
	UClass* Class = UOWTEventTestObject::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UOWTEventTestObject_Statics::Funcs));
}
UClass* Z_Construct_UClass_UOWTEventTestObject()
{
	if (!Z_Registration_Info_UClass_UOWTEventTestObject.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTEventTestObject.OuterSingleton, Z_Construct_UClass_UOWTEventTestObject_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTEventTestObject.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTEventTestObject);
UOWTEventTestObject::~UOWTEventTestObject() {}
// ********** End Class UOWTEventTestObject ********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Private_Tests_OWTEventTestObject_h__Script_OWTEventCore_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTEventTestObject, UOWTEventTestObject::StaticClass, TEXT("UOWTEventTestObject"), &Z_Registration_Info_UClass_UOWTEventTestObject, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTEventTestObject), 1886151915U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Private_Tests_OWTEventTestObject_h__Script_OWTEventCore_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Private_Tests_OWTEventTestObject_h__Script_OWTEventCore_2336880554{
	TEXT("/Script/OWTEventCore"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Private_Tests_OWTEventTestObject_h__Script_OWTEventCore_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Private_Tests_OWTEventTestObject_h__Script_OWTEventCore_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
