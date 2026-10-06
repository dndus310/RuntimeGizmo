// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UI/OWTAttributeDetailsWidget.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTAttributeDetailsWidget() {}

// ********** Begin Cross Module References ********************************************************
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBAttributeEditor_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeDetailsWidget();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeDetailsWidget_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UOWTAttributeDetailsWidget Function IsBlockingWorldInput *****************
struct Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics
{
	struct OWTAttributeDetailsWidget_eventIsBlockingWorldInput_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/UI/OWTAttributeDetailsWidget.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function IsBlockingWorldInput constinit property declarations ******************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function IsBlockingWorldInput constinit property declarations ********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function IsBlockingWorldInput Property Definitions *****************************
void Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((OWTAttributeDetailsWidget_eventIsBlockingWorldInput_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(OWTAttributeDetailsWidget_eventIsBlockingWorldInput_Parms), &Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics::PropPointers) < 2048);
// ********** End Function IsBlockingWorldInput Property Definitions *******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTAttributeDetailsWidget, nullptr, "IsBlockingWorldInput", 	Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics::OWTAttributeDetailsWidget_eventIsBlockingWorldInput_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics::OWTAttributeDetailsWidget_eventIsBlockingWorldInput_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTAttributeDetailsWidget::execIsBlockingWorldInput)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsBlockingWorldInput();
	P_NATIVE_END;
}
// ********** End Class UOWTAttributeDetailsWidget Function IsBlockingWorldInput *******************

// ********** Begin Class UOWTAttributeDetailsWidget Function IsMonitorVisible *********************
struct Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics
{
	struct OWTAttributeDetailsWidget_eventIsMonitorVisible_Parms
	{
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Monitor" },
		{ "ModuleRelativePath", "Public/UI/OWTAttributeDetailsWidget.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function IsMonitorVisible constinit property declarations **********************
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function IsMonitorVisible constinit property declarations ************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function IsMonitorVisible Property Definitions *********************************
void Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((OWTAttributeDetailsWidget_eventIsMonitorVisible_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(OWTAttributeDetailsWidget_eventIsMonitorVisible_Parms), &Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics::PropPointers) < 2048);
// ********** End Function IsMonitorVisible Property Definitions ***********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTAttributeDetailsWidget, nullptr, "IsMonitorVisible", 	Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics::OWTAttributeDetailsWidget_eventIsMonitorVisible_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics::OWTAttributeDetailsWidget_eventIsMonitorVisible_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTAttributeDetailsWidget::execIsMonitorVisible)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->IsMonitorVisible();
	P_NATIVE_END;
}
// ********** End Class UOWTAttributeDetailsWidget Function IsMonitorVisible ***********************

// ********** Begin Class UOWTAttributeDetailsWidget Function SetAttributeEditor *******************
struct Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor_Statics
{
	struct OWTAttributeDetailsWidget_eventSetAttributeEditor_Parms
	{
		AVTBAttributeEditor* InEditor;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Attributes" },
		{ "ModuleRelativePath", "Public/UI/OWTAttributeDetailsWidget.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetAttributeEditor constinit property declarations ********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_InEditor;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetAttributeEditor constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetAttributeEditor Property Definitions *******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor_Statics::NewProp_InEditor = { "InEditor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTAttributeDetailsWidget_eventSetAttributeEditor_Parms, InEditor), Z_Construct_UClass_AVTBAttributeEditor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor_Statics::NewProp_InEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor_Statics::PropPointers) < 2048);
// ********** End Function SetAttributeEditor Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTAttributeDetailsWidget, nullptr, "SetAttributeEditor", 	Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor_Statics::OWTAttributeDetailsWidget_eventSetAttributeEditor_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor_Statics::OWTAttributeDetailsWidget_eventSetAttributeEditor_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTAttributeDetailsWidget::execSetAttributeEditor)
{
	P_GET_OBJECT(AVTBAttributeEditor,Z_Param_InEditor);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetAttributeEditor(Z_Param_InEditor);
	P_NATIVE_END;
}
// ********** End Class UOWTAttributeDetailsWidget Function SetAttributeEditor *********************

// ********** Begin Class UOWTAttributeDetailsWidget Function SetMonitorVisible ********************
struct Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics
{
	struct OWTAttributeDetailsWidget_eventSetMonitorVisible_Parms
	{
		bool bVisible;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Monitor" },
		{ "ModuleRelativePath", "Public/UI/OWTAttributeDetailsWidget.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetMonitorVisible constinit property declarations *********************
	static void NewProp_bVisible_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bVisible;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetMonitorVisible constinit property declarations ***********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetMonitorVisible Property Definitions ********************************
void Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics::NewProp_bVisible_SetBit(void* Obj)
{
	((OWTAttributeDetailsWidget_eventSetMonitorVisible_Parms*)Obj)->bVisible = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics::NewProp_bVisible = { "bVisible", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(OWTAttributeDetailsWidget_eventSetMonitorVisible_Parms), &Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics::NewProp_bVisible_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics::NewProp_bVisible,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics::PropPointers) < 2048);
// ********** End Function SetMonitorVisible Property Definitions **********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTAttributeDetailsWidget, nullptr, "SetMonitorVisible", 	Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics::OWTAttributeDetailsWidget_eventSetMonitorVisible_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics::OWTAttributeDetailsWidget_eventSetMonitorVisible_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTAttributeDetailsWidget::execSetMonitorVisible)
{
	P_GET_UBOOL(Z_Param_bVisible);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetMonitorVisible(Z_Param_bVisible);
	P_NATIVE_END;
}
// ********** End Class UOWTAttributeDetailsWidget Function SetMonitorVisible **********************

// ********** Begin Class UOWTAttributeDetailsWidget Function ToggleMonitor ************************
struct Z_Construct_UFunction_UOWTAttributeDetailsWidget_ToggleMonitor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Monitor" },
		{ "ModuleRelativePath", "Public/UI/OWTAttributeDetailsWidget.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function ToggleMonitor constinit property declarations *************************
// ********** End Function ToggleMonitor constinit property declarations ***************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTAttributeDetailsWidget_ToggleMonitor_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTAttributeDetailsWidget, nullptr, "ToggleMonitor", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTAttributeDetailsWidget_ToggleMonitor_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTAttributeDetailsWidget_ToggleMonitor_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UOWTAttributeDetailsWidget_ToggleMonitor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTAttributeDetailsWidget_ToggleMonitor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTAttributeDetailsWidget::execToggleMonitor)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ToggleMonitor();
	P_NATIVE_END;
}
// ********** End Class UOWTAttributeDetailsWidget Function ToggleMonitor **************************

// ********** Begin Class UOWTAttributeDetailsWidget ***********************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTAttributeDetailsWidget;
UClass* UOWTAttributeDetailsWidget::GetPrivateStaticClass()
{
	using TClass = UOWTAttributeDetailsWidget;
	if (!Z_Registration_Info_UClass_UOWTAttributeDetailsWidget.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTAttributeDetailsWidget"),
			Z_Registration_Info_UClass_UOWTAttributeDetailsWidget.InnerSingleton,
			StaticRegisterNativesUOWTAttributeDetailsWidget,
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
	return Z_Registration_Info_UClass_UOWTAttributeDetailsWidget.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTAttributeDetailsWidget_NoRegister()
{
	return UOWTAttributeDetailsWidget::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "UI/OWTAttributeDetailsWidget.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/UI/OWTAttributeDetailsWidget.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AttributeEditor_MetaData[] = {
		{ "ModuleRelativePath", "Public/UI/OWTAttributeDetailsWidget.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTAttributeDetailsWidget constinit property declarations ***************
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_AttributeEditor;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UOWTAttributeDetailsWidget constinit property declarations *****************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("IsBlockingWorldInput"), .Pointer = &UOWTAttributeDetailsWidget::execIsBlockingWorldInput },
		{ .NameUTF8 = UTF8TEXT("IsMonitorVisible"), .Pointer = &UOWTAttributeDetailsWidget::execIsMonitorVisible },
		{ .NameUTF8 = UTF8TEXT("SetAttributeEditor"), .Pointer = &UOWTAttributeDetailsWidget::execSetAttributeEditor },
		{ .NameUTF8 = UTF8TEXT("SetMonitorVisible"), .Pointer = &UOWTAttributeDetailsWidget::execSetMonitorVisible },
		{ .NameUTF8 = UTF8TEXT("ToggleMonitor"), .Pointer = &UOWTAttributeDetailsWidget::execToggleMonitor },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsBlockingWorldInput, "IsBlockingWorldInput" }, // 1724466650
		{ &Z_Construct_UFunction_UOWTAttributeDetailsWidget_IsMonitorVisible, "IsMonitorVisible" }, // 1898309044
		{ &Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetAttributeEditor, "SetAttributeEditor" }, // 2367690643
		{ &Z_Construct_UFunction_UOWTAttributeDetailsWidget_SetMonitorVisible, "SetMonitorVisible" }, // 3640361505
		{ &Z_Construct_UFunction_UOWTAttributeDetailsWidget_ToggleMonitor, "ToggleMonitor" }, // 1031953328
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTAttributeDetailsWidget>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics

// ********** Begin Class UOWTAttributeDetailsWidget Property Definitions **************************
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics::NewProp_AttributeEditor = { "AttributeEditor", nullptr, (EPropertyFlags)0x0044000000002000, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UOWTAttributeDetailsWidget, AttributeEditor), Z_Construct_UClass_AVTBAttributeEditor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AttributeEditor_MetaData), NewProp_AttributeEditor_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics::NewProp_AttributeEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics::PropPointers) < 2048);
// ********** End Class UOWTAttributeDetailsWidget Property Definitions ****************************
UObject* (*const Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics::ClassParams = {
	&UOWTAttributeDetailsWidget::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics::PropPointers),
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics::Class_MetaDataParams)
};
void UOWTAttributeDetailsWidget::StaticRegisterNativesUOWTAttributeDetailsWidget()
{
	UClass* Class = UOWTAttributeDetailsWidget::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics::Funcs));
}
UClass* Z_Construct_UClass_UOWTAttributeDetailsWidget()
{
	if (!Z_Registration_Info_UClass_UOWTAttributeDetailsWidget.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTAttributeDetailsWidget.OuterSingleton, Z_Construct_UClass_UOWTAttributeDetailsWidget_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTAttributeDetailsWidget.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTAttributeDetailsWidget);
UOWTAttributeDetailsWidget::~UOWTAttributeDetailsWidget() {}
// ********** End Class UOWTAttributeDetailsWidget *************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_UI_OWTAttributeDetailsWidget_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTAttributeDetailsWidget, UOWTAttributeDetailsWidget::StaticClass, TEXT("UOWTAttributeDetailsWidget"), &Z_Registration_Info_UClass_UOWTAttributeDetailsWidget, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTAttributeDetailsWidget), 3028366530U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_UI_OWTAttributeDetailsWidget_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_UI_OWTAttributeDetailsWidget_h__Script_VTBOWTEditor_1263354297{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_UI_OWTAttributeDetailsWidget_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_UI_OWTAttributeDetailsWidget_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
