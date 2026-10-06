// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Interfaces/OWTEditContextReceiver.h"
#include "StructUtils/InstancedStruct.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTEditContextReceiver() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UInterface();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FInstancedStruct();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTEditContextReceiver();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTEditContextReceiver_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Interface UOWTEditContextReceiver Function ReceiveEditContext ******************
struct OWTEditContextReceiver_eventReceiveEditContext_Parms
{
	FInstancedStruct Context;
	bool ReturnValue;

	/** Constructor, initializes return property only **/
	OWTEditContextReceiver_eventReceiveEditContext_Parms()
		: ReturnValue(false)
	{
	}
};
bool IOWTEditContextReceiver::ReceiveEditContext(FInstancedStruct const& Context)
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_ReceiveEditContext instead.");
	OWTEditContextReceiver_eventReceiveEditContext_Parms Parms;
	return Parms.ReturnValue;
}
static FName NAME_UOWTEditContextReceiver_ReceiveEditContext = FName(TEXT("ReceiveEditContext"));
bool IOWTEditContextReceiver::Execute_ReceiveEditContext(UObject* O, FInstancedStruct const& Context)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UOWTEditContextReceiver::StaticClass()));
	OWTEditContextReceiver_eventReceiveEditContext_Parms Parms;
	UFunction* const Func = O->FindFunction(NAME_UOWTEditContextReceiver_ReceiveEditContext);
	if (Func)
	{
		Parms.Context=std::move(Context);
		O->ProcessEvent(Func, &Parms);
	}
	else if (auto I = (IOWTEditContextReceiver*)(O->GetNativeInterfaceAddress(UOWTEditContextReceiver::StaticClass())))
	{
		Parms.ReturnValue = I->ReceiveEditContext_Implementation(Context);
	}
	return Parms.ReturnValue;
}
struct Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/Interfaces/OWTEditContextReceiver.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Context_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function ReceiveEditContext constinit property declarations ********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Context;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ReceiveEditContext constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ReceiveEditContext Property Definitions *******************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTEditContextReceiver_eventReceiveEditContext_Parms, Context), Z_Construct_UScriptStruct_FInstancedStruct, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Context_MetaData), NewProp_Context_MetaData) }; // 3949785911
void Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((OWTEditContextReceiver_eventReceiveEditContext_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(OWTEditContextReceiver_eventReceiveEditContext_Parms), &Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics::NewProp_Context,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics::PropPointers) < 2048);
// ********** End Function ReceiveEditContext Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTEditContextReceiver, nullptr, "ReceiveEditContext", 	Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics::PropPointers), 
sizeof(OWTEditContextReceiver_eventReceiveEditContext_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(OWTEditContextReceiver_eventReceiveEditContext_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IOWTEditContextReceiver::execReceiveEditContext)
{
	P_GET_STRUCT_REF(FInstancedStruct,Z_Param_Out_Context);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->ReceiveEditContext_Implementation(Z_Param_Out_Context);
	P_NATIVE_END;
}
// ********** End Interface UOWTEditContextReceiver Function ReceiveEditContext ********************

// ********** Begin Interface UOWTEditContextReceiver **********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTEditContextReceiver;
UClass* UOWTEditContextReceiver::GetPrivateStaticClass()
{
	using TClass = UOWTEditContextReceiver;
	if (!Z_Registration_Info_UClass_UOWTEditContextReceiver.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTEditContextReceiver"),
			Z_Registration_Info_UClass_UOWTEditContextReceiver.InnerSingleton,
			StaticRegisterNativesUOWTEditContextReceiver,
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
	return Z_Registration_Info_UClass_UOWTEditContextReceiver.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTEditContextReceiver_NoRegister()
{
	return UOWTEditContextReceiver::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTEditContextReceiver_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Interfaces/OWTEditContextReceiver.h" },
	};
#endif // WITH_METADATA

// ********** Begin Interface UOWTEditContextReceiver constinit property declarations **************
// ********** End Interface UOWTEditContextReceiver constinit property declarations ****************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("ReceiveEditContext"), .Pointer = &IOWTEditContextReceiver::execReceiveEditContext },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UOWTEditContextReceiver_ReceiveEditContext, "ReceiveEditContext" }, // 2571910876
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<IOWTEditContextReceiver>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTEditContextReceiver_Statics
UObject* (*const Z_Construct_UClass_UOWTEditContextReceiver_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInterface,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTEditContextReceiver_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTEditContextReceiver_Statics::ClassParams = {
	&UOWTEditContextReceiver::StaticClass,
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
	0x001040A1u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTEditContextReceiver_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTEditContextReceiver_Statics::Class_MetaDataParams)
};
void UOWTEditContextReceiver::StaticRegisterNativesUOWTEditContextReceiver()
{
	UClass* Class = UOWTEditContextReceiver::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UOWTEditContextReceiver_Statics::Funcs));
}
UClass* Z_Construct_UClass_UOWTEditContextReceiver()
{
	if (!Z_Registration_Info_UClass_UOWTEditContextReceiver.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTEditContextReceiver.OuterSingleton, Z_Construct_UClass_UOWTEditContextReceiver_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTEditContextReceiver.OuterSingleton;
}
UOWTEditContextReceiver::UOWTEditContextReceiver(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTEditContextReceiver);
// ********** End Interface UOWTEditContextReceiver ************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTEditContextReceiver, UOWTEditContextReceiver::StaticClass, TEXT("UOWTEditContextReceiver"), &Z_Registration_Info_UClass_UOWTEditContextReceiver, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTEditContextReceiver), 1559580018U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h__Script_VTBOWTEditor_4151890491{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
