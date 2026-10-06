// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Duplication/OWTRuntimeActorDuplicator.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTRuntimeActorDuplicator() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
OWTRUNTIMEDUPLICATION_API UClass* Z_Construct_UClass_UOWTRuntimeActorDuplicator();
OWTRUNTIMEDUPLICATION_API UClass* Z_Construct_UClass_UOWTRuntimeActorDuplicator_NoRegister();
UPackage* Z_Construct_UPackage__Script_OWTRuntimeDuplication();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UOWTRuntimeActorDuplicator Function Deinitialize *************************
struct Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Deinitialize_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Runtime Duplication" },
		{ "ModuleRelativePath", "Public/Duplication/OWTRuntimeActorDuplicator.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function Deinitialize constinit property declarations **************************
// ********** End Function Deinitialize constinit property declarations ****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Deinitialize_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTRuntimeActorDuplicator, nullptr, "Deinitialize", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Deinitialize_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Deinitialize_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Deinitialize()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Deinitialize_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTRuntimeActorDuplicator::execDeinitialize)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Deinitialize();
	P_NATIVE_END;
}
// ********** End Class UOWTRuntimeActorDuplicator Function Deinitialize ***************************

// ********** Begin Class UOWTRuntimeActorDuplicator Function DuplicateActor ***********************
struct Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics
{
	struct OWTRuntimeActorDuplicator_eventDuplicateActor_Parms
	{
		AActor* Source;
		FVector WorldOffset;
		FString OutError;
		AActor* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Runtime Duplication" },
		{ "ModuleRelativePath", "Public/Duplication/OWTRuntimeActorDuplicator.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_WorldOffset_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function DuplicateActor constinit property declarations ************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Source;
	static const UECodeGen_Private::FStructPropertyParams NewProp_WorldOffset;
	static const UECodeGen_Private::FStrPropertyParams NewProp_OutError;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function DuplicateActor constinit property declarations **************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function DuplicateActor Property Definitions ***********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::NewProp_Source = { "Source", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTRuntimeActorDuplicator_eventDuplicateActor_Parms, Source), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::NewProp_WorldOffset = { "WorldOffset", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTRuntimeActorDuplicator_eventDuplicateActor_Parms, WorldOffset), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_WorldOffset_MetaData), NewProp_WorldOffset_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::NewProp_OutError = { "OutError", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTRuntimeActorDuplicator_eventDuplicateActor_Parms, OutError), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTRuntimeActorDuplicator_eventDuplicateActor_Parms, ReturnValue), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::NewProp_Source,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::NewProp_WorldOffset,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::NewProp_OutError,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::PropPointers) < 2048);
// ********** End Function DuplicateActor Property Definitions *************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTRuntimeActorDuplicator, nullptr, "DuplicateActor", 	Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::OWTRuntimeActorDuplicator_eventDuplicateActor_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::OWTRuntimeActorDuplicator_eventDuplicateActor_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTRuntimeActorDuplicator::execDuplicateActor)
{
	P_GET_OBJECT(AActor,Z_Param_Source);
	P_GET_STRUCT_REF(FVector,Z_Param_Out_WorldOffset);
	P_GET_PROPERTY_REF(FStrProperty,Z_Param_Out_OutError);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(AActor**)Z_Param__Result=P_THIS->DuplicateActor(Z_Param_Source,Z_Param_Out_WorldOffset,Z_Param_Out_OutError);
	P_NATIVE_END;
}
// ********** End Class UOWTRuntimeActorDuplicator Function DuplicateActor *************************

// ********** Begin Class UOWTRuntimeActorDuplicator Function Initialize ***************************
struct Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics
{
	struct OWTRuntimeActorDuplicator_eventInitialize_Parms
	{
		UObject* Owner;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Runtime Duplication" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Owner must be this object's exact Outer and expose a live world. */" },
#endif
		{ "ModuleRelativePath", "Public/Duplication/OWTRuntimeActorDuplicator.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Owner must be this object's exact Outer and expose a live world." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function Initialize constinit property declarations ****************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Owner;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function Initialize constinit property declarations ******************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function Initialize Property Definitions ***************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::NewProp_Owner = { "Owner", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTRuntimeActorDuplicator_eventInitialize_Parms, Owner), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((OWTRuntimeActorDuplicator_eventInitialize_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(OWTRuntimeActorDuplicator_eventInitialize_Parms), &Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::NewProp_Owner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::PropPointers) < 2048);
// ********** End Function Initialize Property Definitions *****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTRuntimeActorDuplicator, nullptr, "Initialize", 	Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::OWTRuntimeActorDuplicator_eventInitialize_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::OWTRuntimeActorDuplicator_eventInitialize_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTRuntimeActorDuplicator::execInitialize)
{
	P_GET_OBJECT(UObject,Z_Param_Owner);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->Initialize(Z_Param_Owner);
	P_NATIVE_END;
}
// ********** End Class UOWTRuntimeActorDuplicator Function Initialize *****************************

// ********** Begin Class UOWTRuntimeActorDuplicator ***********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTRuntimeActorDuplicator;
UClass* UOWTRuntimeActorDuplicator::GetPrivateStaticClass()
{
	using TClass = UOWTRuntimeActorDuplicator;
	if (!Z_Registration_Info_UClass_UOWTRuntimeActorDuplicator.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTRuntimeActorDuplicator"),
			Z_Registration_Info_UClass_UOWTRuntimeActorDuplicator.InnerSingleton,
			StaticRegisterNativesUOWTRuntimeActorDuplicator,
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
	return Z_Registration_Info_UClass_UOWTRuntimeActorDuplicator.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTRuntimeActorDuplicator_NoRegister()
{
	return UOWTRuntimeActorDuplicator::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * Synchronous, owner-bound duplication of standalone world Actor configurations.\n * Copies reflected user state, editable component configuration, owned instanced objects and\n * ChildActorComponent hierarchies. External references remain shared; internal references are remapped.\n * Construction runs normally. Restoration completes before the root and child Actors begin play.\n * Engine lifecycle, delegates, timers and opaque native/simulation state are not snapshots.\n * Implement IOWTRuntimeDuplicationParticipant for class-specific native configuration restoration.\n */" },
#endif
		{ "IncludePath", "Duplication/OWTRuntimeActorDuplicator.h" },
		{ "ModuleRelativePath", "Public/Duplication/OWTRuntimeActorDuplicator.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Synchronous, owner-bound duplication of standalone world Actor configurations.\nCopies reflected user state, editable component configuration, owned instanced objects and\nChildActorComponent hierarchies. External references remain shared; internal references are remapped.\nConstruction runs normally. Restoration completes before the root and child Actors begin play.\nEngine lifecycle, delegates, timers and opaque native/simulation state are not snapshots.\nImplement IOWTRuntimeDuplicationParticipant for class-specific native configuration restoration." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_OwningObject_MetaData[] = {
		{ "ModuleRelativePath", "Public/Duplication/OWTRuntimeActorDuplicator.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTRuntimeActorDuplicator constinit property declarations ***************
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_OwningObject;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UOWTRuntimeActorDuplicator constinit property declarations *****************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("Deinitialize"), .Pointer = &UOWTRuntimeActorDuplicator::execDeinitialize },
		{ .NameUTF8 = UTF8TEXT("DuplicateActor"), .Pointer = &UOWTRuntimeActorDuplicator::execDuplicateActor },
		{ .NameUTF8 = UTF8TEXT("Initialize"), .Pointer = &UOWTRuntimeActorDuplicator::execInitialize },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Deinitialize, "Deinitialize" }, // 1856828320
		{ &Z_Construct_UFunction_UOWTRuntimeActorDuplicator_DuplicateActor, "DuplicateActor" }, // 1047442737
		{ &Z_Construct_UFunction_UOWTRuntimeActorDuplicator_Initialize, "Initialize" }, // 1355967701
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTRuntimeActorDuplicator>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics

// ********** Begin Class UOWTRuntimeActorDuplicator Property Definitions **************************
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics::NewProp_OwningObject = { "OwningObject", nullptr, (EPropertyFlags)0x0044000000002000, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTRuntimeActorDuplicator, OwningObject), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_OwningObject_MetaData), NewProp_OwningObject_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics::NewProp_OwningObject,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics::PropPointers) < 2048);
// ********** End Class UOWTRuntimeActorDuplicator Property Definitions ****************************
UObject* (*const Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_OWTRuntimeDuplication,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics::ClassParams = {
	&UOWTRuntimeActorDuplicator::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics::PropPointers),
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics::Class_MetaDataParams)
};
void UOWTRuntimeActorDuplicator::StaticRegisterNativesUOWTRuntimeActorDuplicator()
{
	UClass* Class = UOWTRuntimeActorDuplicator::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics::Funcs));
}
UClass* Z_Construct_UClass_UOWTRuntimeActorDuplicator()
{
	if (!Z_Registration_Info_UClass_UOWTRuntimeActorDuplicator.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTRuntimeActorDuplicator.OuterSingleton, Z_Construct_UClass_UOWTRuntimeActorDuplicator_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTRuntimeActorDuplicator.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTRuntimeActorDuplicator);
UOWTRuntimeActorDuplicator::~UOWTRuntimeActorDuplicator() {}
// ********** End Class UOWTRuntimeActorDuplicator *************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeActorDuplicator_h__Script_OWTRuntimeDuplication_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTRuntimeActorDuplicator, UOWTRuntimeActorDuplicator::StaticClass, TEXT("UOWTRuntimeActorDuplicator"), &Z_Registration_Info_UClass_UOWTRuntimeActorDuplicator, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTRuntimeActorDuplicator), 1804579059U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeActorDuplicator_h__Script_OWTRuntimeDuplication_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeActorDuplicator_h__Script_OWTRuntimeDuplication_2119286474{
	TEXT("/Script/OWTRuntimeDuplication"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeActorDuplicator_h__Script_OWTRuntimeDuplication_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeActorDuplicator_h__Script_OWTRuntimeDuplication_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
