// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "State/OWTAttributeStateStore.h"
#include "Events/OWTAttributeTypes.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTAttributeStateStore() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeStateStore();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeStateStore_NoRegister();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTAttributeSnapshot();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UOWTAttributeStateStore Function GetSnapshot *****************************
struct Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot_Statics
{
	struct OWTAttributeStateStore_eventGetSnapshot_Parms
	{
		FOWTAttributeSnapshot ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/State/OWTAttributeStateStore.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetSnapshot constinit property declarations ***************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetSnapshot constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetSnapshot Property Definitions **************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTAttributeStateStore_eventGetSnapshot_Parms, ReturnValue), Z_Construct_UScriptStruct_FOWTAttributeSnapshot, METADATA_PARAMS(0, nullptr) }; // 1151875563
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot_Statics::PropPointers) < 2048);
// ********** End Function GetSnapshot Property Definitions ****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTAttributeStateStore, nullptr, "GetSnapshot", 	Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot_Statics::OWTAttributeStateStore_eventGetSnapshot_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot_Statics::OWTAttributeStateStore_eventGetSnapshot_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTAttributeStateStore::execGetSnapshot)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FOWTAttributeSnapshot*)Z_Param__Result=P_THIS->GetSnapshot();
	P_NATIVE_END;
}
// ********** End Class UOWTAttributeStateStore Function GetSnapshot *******************************

// ********** Begin Class UOWTAttributeStateStore **************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTAttributeStateStore;
UClass* UOWTAttributeStateStore::GetPrivateStaticClass()
{
	using TClass = UOWTAttributeStateStore;
	if (!Z_Registration_Info_UClass_UOWTAttributeStateStore.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTAttributeStateStore"),
			Z_Registration_Info_UClass_UOWTAttributeStateStore.InnerSingleton,
			StaticRegisterNativesUOWTAttributeStateStore,
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
	return Z_Registration_Info_UClass_UOWTAttributeStateStore.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTAttributeStateStore_NoRegister()
{
	return UOWTAttributeStateStore::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTAttributeStateStore_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Stores observed Actor state. Only the owning editor can update it; JSON never enters this store.\n" },
#endif
		{ "IncludePath", "State/OWTAttributeStateStore.h" },
		{ "IsBlueprintBase", "false" },
		{ "ModuleRelativePath", "Public/State/OWTAttributeStateStore.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Stores observed Actor state. Only the owning editor can update it; JSON never enters this store." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTAttributeStateStore constinit property declarations ******************
// ********** End Class UOWTAttributeStateStore constinit property declarations ********************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("GetSnapshot"), .Pointer = &UOWTAttributeStateStore::execGetSnapshot },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UOWTAttributeStateStore_GetSnapshot, "GetSnapshot" }, // 3070021262
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTAttributeStateStore>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTAttributeStateStore_Statics
UObject* (*const Z_Construct_UClass_UOWTAttributeStateStore_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeStateStore_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTAttributeStateStore_Statics::ClassParams = {
	&UOWTAttributeStateStore::StaticClass,
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
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeStateStore_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTAttributeStateStore_Statics::Class_MetaDataParams)
};
void UOWTAttributeStateStore::StaticRegisterNativesUOWTAttributeStateStore()
{
	UClass* Class = UOWTAttributeStateStore::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UOWTAttributeStateStore_Statics::Funcs));
}
UClass* Z_Construct_UClass_UOWTAttributeStateStore()
{
	if (!Z_Registration_Info_UClass_UOWTAttributeStateStore.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTAttributeStateStore.OuterSingleton, Z_Construct_UClass_UOWTAttributeStateStore_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTAttributeStateStore.OuterSingleton;
}
UOWTAttributeStateStore::UOWTAttributeStateStore(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTAttributeStateStore);
UOWTAttributeStateStore::~UOWTAttributeStateStore() {}
// ********** End Class UOWTAttributeStateStore ****************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTAttributeStateStore_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTAttributeStateStore, UOWTAttributeStateStore::StaticClass, TEXT("UOWTAttributeStateStore"), &Z_Registration_Info_UClass_UOWTAttributeStateStore, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTAttributeStateStore), 2935558241U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTAttributeStateStore_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTAttributeStateStore_h__Script_VTBOWTEditor_2590576467{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTAttributeStateStore_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTAttributeStateStore_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
