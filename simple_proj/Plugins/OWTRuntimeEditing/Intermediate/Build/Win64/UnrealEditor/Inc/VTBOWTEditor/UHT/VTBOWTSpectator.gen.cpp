// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VTBOWTSpectator.h"
#include "StructUtils/InstancedStruct.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTSpectator() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FInstancedStruct();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_ASpectatorPawn();
ENGINE_API UEnum* Z_Construct_UEnum_Engine_ECollisionChannel();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UEnhancedInputComponent_NoRegister();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputAction_NoRegister();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputMappingContext_NoRegister();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBOWTSpectator();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBOWTSpectator_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AVTBOWTSpectator Function IsCameraNavigating *****************************
struct Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics
{
	struct VTBOWTSpectator_eventIsCameraNavigating_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Camera" },
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function IsCameraNavigating constinit property declarations ********************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function IsCameraNavigating constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function IsCameraNavigating Property Definitions *******************************
void Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((VTBOWTSpectator_eventIsCameraNavigating_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VTBOWTSpectator_eventIsCameraNavigating_Parms), &Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics::PropPointers) < 2048);
// ********** End Function IsCameraNavigating Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBOWTSpectator, nullptr, "IsCameraNavigating", 	Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics::VTBOWTSpectator_eventIsCameraNavigating_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics::VTBOWTSpectator_eventIsCameraNavigating_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBOWTSpectator::execIsCameraNavigating)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsCameraNavigating();
	P_NATIVE_END;
}
// ********** End Class AVTBOWTSpectator Function IsCameraNavigating *******************************

// ********** Begin Class AVTBOWTSpectator Function SendEditContext ********************************
struct Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics
{
	struct VTBOWTSpectator_eventSendEditContext_Parms
	{
		FInstancedStruct Context;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Context_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function SendEditContext constinit property declarations ***********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Context;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SendEditContext constinit property declarations *************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SendEditContext Property Definitions **********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTSpectator_eventSendEditContext_Parms, Context), Z_Construct_UScriptStruct_FInstancedStruct, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Context_MetaData), NewProp_Context_MetaData) }; // 3949785911
void Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((VTBOWTSpectator_eventSendEditContext_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VTBOWTSpectator_eventSendEditContext_Parms), &Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::NewProp_Context,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::PropPointers) < 2048);
// ********** End Function SendEditContext Property Definitions ************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBOWTSpectator, nullptr, "SendEditContext", 	Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::VTBOWTSpectator_eventSendEditContext_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::VTBOWTSpectator_eventSendEditContext_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBOWTSpectator::execSendEditContext)
{
	P_GET_STRUCT_REF(FInstancedStruct,Z_Param_Out_Context);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->SendEditContext(Z_Param_Out_Context);
	P_NATIVE_END;
}
// ********** End Class AVTBOWTSpectator Function SendEditContext **********************************

// ********** Begin Class AVTBOWTSpectator Function TraceSelectableObject **************************
struct Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject_Statics
{
	struct VTBOWTSpectator_eventTraceSelectableObject_Parms
	{
		AActor* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Selection" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Trace is an input-side responsibility. Only its actor result reaches editing code.\n" },
#endif
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Trace is an input-side responsibility. Only its actor result reaches editing code." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function TraceSelectableObject constinit property declarations *****************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function TraceSelectableObject constinit property declarations *******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function TraceSelectableObject Property Definitions ****************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTSpectator_eventTraceSelectableObject_Parms, ReturnValue), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject_Statics::PropPointers) < 2048);
// ********** End Function TraceSelectableObject Property Definitions ******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBOWTSpectator, nullptr, "TraceSelectableObject", 	Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject_Statics::VTBOWTSpectator_eventTraceSelectableObject_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject_Statics::VTBOWTSpectator_eventTraceSelectableObject_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBOWTSpectator::execTraceSelectableObject)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(AActor**)Z_Param__Result=P_THIS->TraceSelectableObject();
	P_NATIVE_END;
}
// ********** End Class AVTBOWTSpectator Function TraceSelectableObject ****************************

// ********** Begin Class AVTBOWTSpectator *********************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_AVTBOWTSpectator;
UClass* AVTBOWTSpectator::GetPrivateStaticClass()
{
	using TClass = AVTBOWTSpectator;
	if (!Z_Registration_Info_UClass_AVTBOWTSpectator.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTSpectator"),
			Z_Registration_Info_UClass_AVTBOWTSpectator.InnerSingleton,
			StaticRegisterNativesAVTBOWTSpectator,
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
	return Z_Registration_Info_UClass_AVTBOWTSpectator.InnerSingleton;
}
UClass* Z_Construct_UClass_AVTBOWTSpectator_NoRegister()
{
	return AVTBOWTSpectator::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AVTBOWTSpectator_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "VTBOWTSpectator.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EnhancedInput_MetaData[] = {
		{ "Category", "Input" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MappingContext_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CameraNavigateAction_MetaData[] = {
		{ "Category", "OWT|Camera" },
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CameraMoveAction_MetaData[] = {
		{ "Category", "OWT|Camera" },
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CameraLookAction_MetaData[] = {
		{ "Category", "OWT|Camera" },
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EditContextReceiver_MetaData[] = {
		{ "Category", "OWT|Editing" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Optional replacement receiver; defaults to the world editing subsystem.\n" },
#endif
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
		{ "MustImplement", "/Script/VTBOWTEditor.OWTEditContextReceiver" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Optional replacement receiver; defaults to the world editing subsystem." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SelectionTraceChannel_MetaData[] = {
		{ "Category", "OWT|Selection" },
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SelectionTraceDistance_MetaData[] = {
		{ "Category", "OWT|Selection" },
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CameraLookSensitivity_MetaData[] = {
		{ "Category", "OWT|Camera" },
		{ "ClampMin", "0.01" },
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FallbackMappingContext_MetaData[] = {
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FallbackEditActions_MetaData[] = {
		{ "ModuleRelativePath", "Public/VTBOWTSpectator.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class AVTBOWTSpectator constinit property declarations *************************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_EnhancedInput;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MappingContext;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CameraNavigateAction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CameraMoveAction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CameraLookAction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_EditContextReceiver;
	static const UECodeGen_Private::FBytePropertyParams NewProp_SelectionTraceChannel;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_SelectionTraceDistance;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_CameraLookSensitivity;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_FallbackMappingContext;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_FallbackEditActions_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_FallbackEditActions;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class AVTBOWTSpectator constinit property declarations ***************************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("IsCameraNavigating"), .Pointer = &AVTBOWTSpectator::execIsCameraNavigating },
		{ .NameUTF8 = UTF8TEXT("SendEditContext"), .Pointer = &AVTBOWTSpectator::execSendEditContext },
		{ .NameUTF8 = UTF8TEXT("TraceSelectableObject"), .Pointer = &AVTBOWTSpectator::execTraceSelectableObject },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_AVTBOWTSpectator_IsCameraNavigating, "IsCameraNavigating" }, // 503301236
		{ &Z_Construct_UFunction_AVTBOWTSpectator_SendEditContext, "SendEditContext" }, // 605858855
		{ &Z_Construct_UFunction_AVTBOWTSpectator_TraceSelectableObject, "TraceSelectableObject" }, // 2182377382
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AVTBOWTSpectator>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_AVTBOWTSpectator_Statics

// ********** Begin Class AVTBOWTSpectator Property Definitions ************************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_EnhancedInput = { "EnhancedInput", nullptr, (EPropertyFlags)0x01140000000a001d, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBOWTSpectator, EnhancedInput), Z_Construct_UClass_UEnhancedInputComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EnhancedInput_MetaData), NewProp_EnhancedInput_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_MappingContext = { "MappingContext", nullptr, (EPropertyFlags)0x0114000000010015, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBOWTSpectator, MappingContext), Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MappingContext_MetaData), NewProp_MappingContext_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_CameraNavigateAction = { "CameraNavigateAction", nullptr, (EPropertyFlags)0x0114000000010015, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBOWTSpectator, CameraNavigateAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CameraNavigateAction_MetaData), NewProp_CameraNavigateAction_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_CameraMoveAction = { "CameraMoveAction", nullptr, (EPropertyFlags)0x0114000000010015, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBOWTSpectator, CameraMoveAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CameraMoveAction_MetaData), NewProp_CameraMoveAction_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_CameraLookAction = { "CameraLookAction", nullptr, (EPropertyFlags)0x0114000000010015, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBOWTSpectator, CameraLookAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CameraLookAction_MetaData), NewProp_CameraLookAction_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_EditContextReceiver = { "EditContextReceiver", nullptr, (EPropertyFlags)0x0114000000000005, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBOWTSpectator, EditContextReceiver), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EditContextReceiver_MetaData), NewProp_EditContextReceiver_MetaData) };
const UECodeGen_Private::FBytePropertyParams Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_SelectionTraceChannel = { "SelectionTraceChannel", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBOWTSpectator, SelectionTraceChannel), Z_Construct_UEnum_Engine_ECollisionChannel, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SelectionTraceChannel_MetaData), NewProp_SelectionTraceChannel_MetaData) }; // 838391399
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_SelectionTraceDistance = { "SelectionTraceDistance", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBOWTSpectator, SelectionTraceDistance), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SelectionTraceDistance_MetaData), NewProp_SelectionTraceDistance_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_CameraLookSensitivity = { "CameraLookSensitivity", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBOWTSpectator, CameraLookSensitivity), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CameraLookSensitivity_MetaData), NewProp_CameraLookSensitivity_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_FallbackMappingContext = { "FallbackMappingContext", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBOWTSpectator, FallbackMappingContext), Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FallbackMappingContext_MetaData), NewProp_FallbackMappingContext_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_FallbackEditActions_Inner = { "FallbackEditActions", nullptr, (EPropertyFlags)0x0104000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_FallbackEditActions = { "FallbackEditActions", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBOWTSpectator, FallbackEditActions), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FallbackEditActions_MetaData), NewProp_FallbackEditActions_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AVTBOWTSpectator_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_EnhancedInput,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_MappingContext,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_CameraNavigateAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_CameraMoveAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_CameraLookAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_EditContextReceiver,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_SelectionTraceChannel,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_SelectionTraceDistance,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_CameraLookSensitivity,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_FallbackMappingContext,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_FallbackEditActions_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTSpectator_Statics::NewProp_FallbackEditActions,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTSpectator_Statics::PropPointers) < 2048);
// ********** End Class AVTBOWTSpectator Property Definitions **************************************
UObject* (*const Z_Construct_UClass_AVTBOWTSpectator_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ASpectatorPawn,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTSpectator_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AVTBOWTSpectator_Statics::ClassParams = {
	&AVTBOWTSpectator::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_AVTBOWTSpectator_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTSpectator_Statics::PropPointers),
	0,
	0x009002A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTSpectator_Statics::Class_MetaDataParams), Z_Construct_UClass_AVTBOWTSpectator_Statics::Class_MetaDataParams)
};
void AVTBOWTSpectator::StaticRegisterNativesAVTBOWTSpectator()
{
	UClass* Class = AVTBOWTSpectator::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_AVTBOWTSpectator_Statics::Funcs));
}
UClass* Z_Construct_UClass_AVTBOWTSpectator()
{
	if (!Z_Registration_Info_UClass_AVTBOWTSpectator.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AVTBOWTSpectator.OuterSingleton, Z_Construct_UClass_AVTBOWTSpectator_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AVTBOWTSpectator.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, AVTBOWTSpectator);
AVTBOWTSpectator::~AVTBOWTSpectator() {}
// ********** End Class AVTBOWTSpectator ***********************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTSpectator_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AVTBOWTSpectator, AVTBOWTSpectator::StaticClass, TEXT("AVTBOWTSpectator"), &Z_Registration_Info_UClass_AVTBOWTSpectator, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AVTBOWTSpectator), 251270820U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTSpectator_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTSpectator_h__Script_VTBOWTEditor_3953862043{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTSpectator_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTSpectator_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
