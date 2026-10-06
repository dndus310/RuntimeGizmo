// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VTBOWTEditorPlayerController.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTEditorPlayerController() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UClass_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_APlayerController();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBOWTEditorPlayerController();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBOWTEditorPlayerController_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeDetailsWidget_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class AVTBOWTEditorPlayerController Function GetAttributeDetailsWidget *********
struct Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget_Statics
{
	struct VTBOWTEditorPlayerController_eventGetAttributeDetailsWidget_Parms
	{
		UOWTAttributeDetailsWidget* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|UI" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorPlayerController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetAttributeDetailsWidget constinit property declarations *************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetAttributeDetailsWidget constinit property declarations ***************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetAttributeDetailsWidget Property Definitions ************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTEditorPlayerController_eventGetAttributeDetailsWidget_Parms, ReturnValue), Z_Construct_UClass_UOWTAttributeDetailsWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget_Statics::PropPointers) < 2048);
// ********** End Function GetAttributeDetailsWidget Property Definitions **************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBOWTEditorPlayerController, nullptr, "GetAttributeDetailsWidget", 	Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget_Statics::VTBOWTEditorPlayerController_eventGetAttributeDetailsWidget_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget_Statics::VTBOWTEditorPlayerController_eventGetAttributeDetailsWidget_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBOWTEditorPlayerController::execGetAttributeDetailsWidget)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UOWTAttributeDetailsWidget**)Z_Param__Result=P_THIS->GetAttributeDetailsWidget();
	P_NATIVE_END;
}
// ********** End Class AVTBOWTEditorPlayerController Function GetAttributeDetailsWidget ***********

// ********** Begin Class AVTBOWTEditorPlayerController Function IsAttributeUIBlockingInput ********
struct Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics
{
	struct VTBOWTEditorPlayerController_eventIsAttributeUIBlockingInput_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|UI" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorPlayerController.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function IsAttributeUIBlockingInput constinit property declarations ************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function IsAttributeUIBlockingInput constinit property declarations **************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function IsAttributeUIBlockingInput Property Definitions ***********************
void Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((VTBOWTEditorPlayerController_eventIsAttributeUIBlockingInput_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VTBOWTEditorPlayerController_eventIsAttributeUIBlockingInput_Parms), &Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics::PropPointers) < 2048);
// ********** End Function IsAttributeUIBlockingInput Property Definitions *************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBOWTEditorPlayerController, nullptr, "IsAttributeUIBlockingInput", 	Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics::VTBOWTEditorPlayerController_eventIsAttributeUIBlockingInput_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics::VTBOWTEditorPlayerController_eventIsAttributeUIBlockingInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBOWTEditorPlayerController::execIsAttributeUIBlockingInput)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsAttributeUIBlockingInput();
	P_NATIVE_END;
}
// ********** End Class AVTBOWTEditorPlayerController Function IsAttributeUIBlockingInput **********

// ********** Begin Class AVTBOWTEditorPlayerController Function ToggleEventMonitor ****************
struct Z_Construct_UFunction_AVTBOWTEditorPlayerController_ToggleEventMonitor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|UI" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorPlayerController.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function ToggleEventMonitor constinit property declarations ********************
// ********** End Function ToggleEventMonitor constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AVTBOWTEditorPlayerController_ToggleEventMonitor_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_AVTBOWTEditorPlayerController, nullptr, "ToggleEventMonitor", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AVTBOWTEditorPlayerController_ToggleEventMonitor_Statics::Function_MetaDataParams), Z_Construct_UFunction_AVTBOWTEditorPlayerController_ToggleEventMonitor_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_AVTBOWTEditorPlayerController_ToggleEventMonitor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AVTBOWTEditorPlayerController_ToggleEventMonitor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(AVTBOWTEditorPlayerController::execToggleEventMonitor)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ToggleEventMonitor();
	P_NATIVE_END;
}
// ********** End Class AVTBOWTEditorPlayerController Function ToggleEventMonitor ******************

// ********** Begin Class AVTBOWTEditorPlayerController ********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_AVTBOWTEditorPlayerController;
UClass* AVTBOWTEditorPlayerController::GetPrivateStaticClass()
{
	using TClass = AVTBOWTEditorPlayerController;
	if (!Z_Registration_Info_UClass_AVTBOWTEditorPlayerController.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTEditorPlayerController"),
			Z_Registration_Info_UClass_AVTBOWTEditorPlayerController.InnerSingleton,
			StaticRegisterNativesAVTBOWTEditorPlayerController,
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
	return Z_Registration_Info_UClass_AVTBOWTEditorPlayerController.InnerSingleton;
}
UClass* Z_Construct_UClass_AVTBOWTEditorPlayerController_NoRegister()
{
	return AVTBOWTEditorPlayerController::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Collision Rendering Transformation" },
		{ "IncludePath", "VTBOWTEditorPlayerController.h" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorPlayerController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AttributeDetailsClass_MetaData[] = {
		{ "Category", "OWT|UI" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorPlayerController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AttributeDetails_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorPlayerController.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class AVTBOWTEditorPlayerController constinit property declarations ************
	static const UECodeGen_Private::FClassPropertyParams NewProp_AttributeDetailsClass;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_AttributeDetails;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class AVTBOWTEditorPlayerController constinit property declarations **************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("GetAttributeDetailsWidget"), .Pointer = &AVTBOWTEditorPlayerController::execGetAttributeDetailsWidget },
		{ .NameUTF8 = UTF8TEXT("IsAttributeUIBlockingInput"), .Pointer = &AVTBOWTEditorPlayerController::execIsAttributeUIBlockingInput },
		{ .NameUTF8 = UTF8TEXT("ToggleEventMonitor"), .Pointer = &AVTBOWTEditorPlayerController::execToggleEventMonitor },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_AVTBOWTEditorPlayerController_GetAttributeDetailsWidget, "GetAttributeDetailsWidget" }, // 2082934522
		{ &Z_Construct_UFunction_AVTBOWTEditorPlayerController_IsAttributeUIBlockingInput, "IsAttributeUIBlockingInput" }, // 548896693
		{ &Z_Construct_UFunction_AVTBOWTEditorPlayerController_ToggleEventMonitor, "ToggleEventMonitor" }, // 630956181
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AVTBOWTEditorPlayerController>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics

// ********** Begin Class AVTBOWTEditorPlayerController Property Definitions ***********************
const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::NewProp_AttributeDetailsClass = { "AttributeDetailsClass", nullptr, (EPropertyFlags)0x0014000000010015, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBOWTEditorPlayerController, AttributeDetailsClass), Z_Construct_UClass_UClass_NoRegister, Z_Construct_UClass_UOWTAttributeDetailsWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AttributeDetailsClass_MetaData), NewProp_AttributeDetailsClass_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::NewProp_AttributeDetails = { "AttributeDetails", nullptr, (EPropertyFlags)0x0144000000082008, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBOWTEditorPlayerController, AttributeDetails), Z_Construct_UClass_UOWTAttributeDetailsWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AttributeDetails_MetaData), NewProp_AttributeDetails_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::NewProp_AttributeDetailsClass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::NewProp_AttributeDetails,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::PropPointers) < 2048);
// ********** End Class AVTBOWTEditorPlayerController Property Definitions *************************
UObject* (*const Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_APlayerController,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::ClassParams = {
	&AVTBOWTEditorPlayerController::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::PropPointers),
	0,
	0x009002A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::Class_MetaDataParams), Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::Class_MetaDataParams)
};
void AVTBOWTEditorPlayerController::StaticRegisterNativesAVTBOWTEditorPlayerController()
{
	UClass* Class = AVTBOWTEditorPlayerController::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::Funcs));
}
UClass* Z_Construct_UClass_AVTBOWTEditorPlayerController()
{
	if (!Z_Registration_Info_UClass_AVTBOWTEditorPlayerController.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AVTBOWTEditorPlayerController.OuterSingleton, Z_Construct_UClass_AVTBOWTEditorPlayerController_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AVTBOWTEditorPlayerController.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, AVTBOWTEditorPlayerController);
AVTBOWTEditorPlayerController::~AVTBOWTEditorPlayerController() {}
// ********** End Class AVTBOWTEditorPlayerController **********************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorPlayerController_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AVTBOWTEditorPlayerController, AVTBOWTEditorPlayerController::StaticClass, TEXT("AVTBOWTEditorPlayerController"), &Z_Registration_Info_UClass_AVTBOWTEditorPlayerController, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AVTBOWTEditorPlayerController), 3890106472U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorPlayerController_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorPlayerController_h__Script_VTBOWTEditor_2646438374{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorPlayerController_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorPlayerController_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
