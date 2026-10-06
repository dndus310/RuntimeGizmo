// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Duplication/OWTRuntimeDuplicationParticipant.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTRuntimeDuplicationParticipant() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UInterface();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
OWTRUNTIMEDUPLICATION_API UClass* Z_Construct_UClass_UOWTRuntimeDuplicationParticipant();
OWTRUNTIMEDUPLICATION_API UClass* Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_NoRegister();
UPackage* Z_Construct_UPackage__Script_OWTRuntimeDuplication();
// ********** End Cross Module References **********************************************************

// ********** Begin Interface UOWTRuntimeDuplicationParticipant Function RestoreRuntimeDuplicateState 
struct OWTRuntimeDuplicationParticipant_eventRestoreRuntimeDuplicateState_Parms
{
	UObject* SourceObject;
	TMap<UObject*,UObject*> DuplicatedObjects;
	FString OutError;
	bool ReturnValue;

	/** Constructor, initializes return property only **/
	OWTRuntimeDuplicationParticipant_eventRestoreRuntimeDuplicateState_Parms()
		: ReturnValue(false)
	{
	}
};
bool IOWTRuntimeDuplicationParticipant::RestoreRuntimeDuplicateState(UObject* SourceObject, TMap<UObject*,UObject*> const& DuplicatedObjects, FString& OutError)
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_RestoreRuntimeDuplicateState instead.");
	OWTRuntimeDuplicationParticipant_eventRestoreRuntimeDuplicateState_Parms Parms;
	return Parms.ReturnValue;
}
static FName NAME_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState = FName(TEXT("RestoreRuntimeDuplicateState"));
bool IOWTRuntimeDuplicationParticipant::Execute_RestoreRuntimeDuplicateState(UObject* O, UObject* SourceObject, TMap<UObject*,UObject*> const& DuplicatedObjects, FString& OutError)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UOWTRuntimeDuplicationParticipant::StaticClass()));
	OWTRuntimeDuplicationParticipant_eventRestoreRuntimeDuplicateState_Parms Parms;
	UFunction* const Func = O->FindFunction(NAME_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState);
	if (Func)
	{
		Parms.SourceObject=std::move(SourceObject);
		Parms.DuplicatedObjects=std::move(DuplicatedObjects);
		Parms.OutError=std::move(OutError);
		O->ProcessEvent(Func, &Parms);
		OutError=std::move(Parms.OutError);
	}
	else if (auto I = (IOWTRuntimeDuplicationParticipant*)(O->GetNativeInterfaceAddress(UOWTRuntimeDuplicationParticipant::StaticClass())))
	{
		Parms.ReturnValue = I->RestoreRuntimeDuplicateState_Implementation(SourceObject,DuplicatedObjects,OutError);
	}
	return Parms.ReturnValue;
}
struct Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Runtime Duplication" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n\x09 * Called on each destination Actor, component or owned object after the complete hierarchy is mapped,\n\x09 * before any duplicate Actor begins play. Read custom native configuration from SourceObject and\n\x09 * use DuplicatedObjects to translate internal references. Recreate resources, never copy live handles.\n\x09 * Returning false rolls back the newly spawned root and its ChildActorComponent hierarchy.\n\x09 * Construction and initialization callbacks have already run and their external side effects cannot be undone.\n\x09 */" },
#endif
		{ "ModuleRelativePath", "Public/Duplication/OWTRuntimeDuplicationParticipant.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Called on each destination Actor, component or owned object after the complete hierarchy is mapped,\nbefore any duplicate Actor begins play. Read custom native configuration from SourceObject and\nuse DuplicatedObjects to translate internal references. Recreate resources, never copy live handles.\nReturning false rolls back the newly spawned root and its ChildActorComponent hierarchy.\nConstruction and initialization callbacks have already run and their external side effects cannot be undone." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DuplicatedObjects_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function RestoreRuntimeDuplicateState constinit property declarations **********
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SourceObject;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_DuplicatedObjects_ValueProp;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_DuplicatedObjects_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_DuplicatedObjects;
	static const UECodeGen_Private::FStrPropertyParams NewProp_OutError;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function RestoreRuntimeDuplicateState constinit property declarations ************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function RestoreRuntimeDuplicateState Property Definitions *********************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::NewProp_SourceObject = { "SourceObject", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTRuntimeDuplicationParticipant_eventRestoreRuntimeDuplicateState_Parms, SourceObject), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::NewProp_DuplicatedObjects_ValueProp = { "DuplicatedObjects", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::NewProp_DuplicatedObjects_Key_KeyProp = { "DuplicatedObjects_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::NewProp_DuplicatedObjects = { "DuplicatedObjects", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTRuntimeDuplicationParticipant_eventRestoreRuntimeDuplicateState_Parms, DuplicatedObjects), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DuplicatedObjects_MetaData), NewProp_DuplicatedObjects_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::NewProp_OutError = { "OutError", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTRuntimeDuplicationParticipant_eventRestoreRuntimeDuplicateState_Parms, OutError), METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((OWTRuntimeDuplicationParticipant_eventRestoreRuntimeDuplicateState_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(OWTRuntimeDuplicationParticipant_eventRestoreRuntimeDuplicateState_Parms), &Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::NewProp_SourceObject,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::NewProp_DuplicatedObjects_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::NewProp_DuplicatedObjects_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::NewProp_DuplicatedObjects,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::NewProp_OutError,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::PropPointers) < 2048);
// ********** End Function RestoreRuntimeDuplicateState Property Definitions ***********************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTRuntimeDuplicationParticipant, nullptr, "RestoreRuntimeDuplicateState", 	Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::PropPointers), 
sizeof(OWTRuntimeDuplicationParticipant_eventRestoreRuntimeDuplicateState_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x0C420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(OWTRuntimeDuplicationParticipant_eventRestoreRuntimeDuplicateState_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IOWTRuntimeDuplicationParticipant::execRestoreRuntimeDuplicateState)
{
	P_GET_OBJECT(UObject,Z_Param_SourceObject);
	P_GET_TMAP_REF(UObject*,UObject*,Z_Param_Out_DuplicatedObjects);
	P_GET_PROPERTY_REF(FStrProperty,Z_Param_Out_OutError);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->RestoreRuntimeDuplicateState_Implementation(Z_Param_SourceObject,Z_Param_Out_DuplicatedObjects,Z_Param_Out_OutError);
	P_NATIVE_END;
}
// ********** End Interface UOWTRuntimeDuplicationParticipant Function RestoreRuntimeDuplicateState 

// ********** Begin Interface UOWTRuntimeDuplicationParticipant ************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTRuntimeDuplicationParticipant;
UClass* UOWTRuntimeDuplicationParticipant::GetPrivateStaticClass()
{
	using TClass = UOWTRuntimeDuplicationParticipant;
	if (!Z_Registration_Info_UClass_UOWTRuntimeDuplicationParticipant.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTRuntimeDuplicationParticipant"),
			Z_Registration_Info_UClass_UOWTRuntimeDuplicationParticipant.InnerSingleton,
			StaticRegisterNativesUOWTRuntimeDuplicationParticipant,
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
	return Z_Registration_Info_UClass_UOWTRuntimeDuplicationParticipant.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_NoRegister()
{
	return UOWTRuntimeDuplicationParticipant::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Duplication/OWTRuntimeDuplicationParticipant.h" },
	};
#endif // WITH_METADATA

// ********** Begin Interface UOWTRuntimeDuplicationParticipant constinit property declarations ****
// ********** End Interface UOWTRuntimeDuplicationParticipant constinit property declarations ******
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("RestoreRuntimeDuplicateState"), .Pointer = &IOWTRuntimeDuplicationParticipant::execRestoreRuntimeDuplicateState },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UOWTRuntimeDuplicationParticipant_RestoreRuntimeDuplicateState, "RestoreRuntimeDuplicateState" }, // 758270317
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<IOWTRuntimeDuplicationParticipant>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_Statics
UObject* (*const Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInterface,
	(UObject* (*)())Z_Construct_UPackage__Script_OWTRuntimeDuplication,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_Statics::ClassParams = {
	&UOWTRuntimeDuplicationParticipant::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_Statics::Class_MetaDataParams)
};
void UOWTRuntimeDuplicationParticipant::StaticRegisterNativesUOWTRuntimeDuplicationParticipant()
{
	UClass* Class = UOWTRuntimeDuplicationParticipant::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_Statics::Funcs));
}
UClass* Z_Construct_UClass_UOWTRuntimeDuplicationParticipant()
{
	if (!Z_Registration_Info_UClass_UOWTRuntimeDuplicationParticipant.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTRuntimeDuplicationParticipant.OuterSingleton, Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTRuntimeDuplicationParticipant.OuterSingleton;
}
UOWTRuntimeDuplicationParticipant::UOWTRuntimeDuplicationParticipant(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTRuntimeDuplicationParticipant);
// ********** End Interface UOWTRuntimeDuplicationParticipant **************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h__Script_OWTRuntimeDuplication_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTRuntimeDuplicationParticipant, UOWTRuntimeDuplicationParticipant::StaticClass, TEXT("UOWTRuntimeDuplicationParticipant"), &Z_Registration_Info_UClass_UOWTRuntimeDuplicationParticipant, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTRuntimeDuplicationParticipant), 3693878149U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h__Script_OWTRuntimeDuplication_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h__Script_OWTRuntimeDuplication_3721750031{
	TEXT("/Script/OWTRuntimeDuplication"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h__Script_OWTRuntimeDuplication_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h__Script_OWTRuntimeDuplication_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
