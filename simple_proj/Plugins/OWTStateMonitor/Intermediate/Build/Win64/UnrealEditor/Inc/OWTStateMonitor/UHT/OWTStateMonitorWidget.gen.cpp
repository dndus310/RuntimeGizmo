// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OWTStateMonitorWidget.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeOWTStateMonitorWidget() {}

// ********** Begin Cross Module References ********************************************************
OWTSTATEMONITOR_API UClass* Z_Construct_UClass_UOWTStateMonitorWidget();
OWTSTATEMONITOR_API UClass* Z_Construct_UClass_UOWTStateMonitorWidget_NoRegister();
UMG_API UClass* Z_Construct_UClass_UWidget();
UPackage* Z_Construct_UPackage__Script_OWTStateMonitor();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UOWTStateMonitorWidget Function ClearHistory *****************************
struct Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory_Statics
{
	struct OWTStateMonitorWidget_eventClearHistory_Parms
	{
		FName SourceId;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "State Monitor" },
		{ "ModuleRelativePath", "Public/OWTStateMonitorWidget.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function ClearHistory constinit property declarations **************************
	static const UECodeGen_Private::FNamePropertyParams NewProp_SourceId;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ClearHistory constinit property declarations ****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ClearHistory Property Definitions *************************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory_Statics::NewProp_SourceId = { "SourceId", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTStateMonitorWidget_eventClearHistory_Parms, SourceId), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory_Statics::NewProp_SourceId,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory_Statics::PropPointers) < 2048);
// ********** End Function ClearHistory Property Definitions ***************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTStateMonitorWidget, nullptr, "ClearHistory", 	Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory_Statics::OWTStateMonitorWidget_eventClearHistory_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory_Statics::OWTStateMonitorWidget_eventClearHistory_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTStateMonitorWidget::execClearHistory)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_SourceId);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ClearHistory(Z_Param_SourceId);
	P_NATIVE_END;
}
// ********** End Class UOWTStateMonitorWidget Function ClearHistory *******************************

// ********** Begin Class UOWTStateMonitorWidget Function RemoveSource *****************************
struct Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics
{
	struct OWTStateMonitorWidget_eventRemoveSource_Parms
	{
		FName SourceId;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "State Monitor" },
		{ "ModuleRelativePath", "Public/OWTStateMonitorWidget.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function RemoveSource constinit property declarations **************************
	static const UECodeGen_Private::FNamePropertyParams NewProp_SourceId;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function RemoveSource constinit property declarations ****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function RemoveSource Property Definitions *************************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::NewProp_SourceId = { "SourceId", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTStateMonitorWidget_eventRemoveSource_Parms, SourceId), METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((OWTStateMonitorWidget_eventRemoveSource_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(OWTStateMonitorWidget_eventRemoveSource_Parms), &Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::NewProp_SourceId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::PropPointers) < 2048);
// ********** End Function RemoveSource Property Definitions ***************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTStateMonitorWidget, nullptr, "RemoveSource", 	Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::OWTStateMonitorWidget_eventRemoveSource_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::OWTStateMonitorWidget_eventRemoveSource_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTStateMonitorWidget::execRemoveSource)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_SourceId);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->RemoveSource(Z_Param_SourceId);
	P_NATIVE_END;
}
// ********** End Class UOWTStateMonitorWidget Function RemoveSource *******************************

// ********** Begin Class UOWTStateMonitorWidget Function ResetBaseline ****************************
struct Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline_Statics
{
	struct OWTStateMonitorWidget_eventResetBaseline_Parms
	{
		FName SourceId;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "State Monitor" },
		{ "ModuleRelativePath", "Public/OWTStateMonitorWidget.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function ResetBaseline constinit property declarations *************************
	static const UECodeGen_Private::FNamePropertyParams NewProp_SourceId;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ResetBaseline constinit property declarations ***************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ResetBaseline Property Definitions ************************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline_Statics::NewProp_SourceId = { "SourceId", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTStateMonitorWidget_eventResetBaseline_Parms, SourceId), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline_Statics::NewProp_SourceId,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline_Statics::PropPointers) < 2048);
// ********** End Function ResetBaseline Property Definitions **************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTStateMonitorWidget, nullptr, "ResetBaseline", 	Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline_Statics::OWTStateMonitorWidget_eventResetBaseline_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline_Statics::OWTStateMonitorWidget_eventResetBaseline_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTStateMonitorWidget::execResetBaseline)
{
	P_GET_PROPERTY(FNameProperty,Z_Param_SourceId);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ResetBaseline(Z_Param_SourceId);
	P_NATIVE_END;
}
// ********** End Class UOWTStateMonitorWidget Function ResetBaseline ******************************

// ********** Begin Class UOWTStateMonitorWidget Function ResetMonitor *****************************
struct Z_Construct_UFunction_UOWTStateMonitorWidget_ResetMonitor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "State Monitor" },
		{ "ModuleRelativePath", "Public/OWTStateMonitorWidget.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function ResetMonitor constinit property declarations **************************
// ********** End Function ResetMonitor constinit property declarations ****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTStateMonitorWidget_ResetMonitor_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTStateMonitorWidget, nullptr, "ResetMonitor", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTStateMonitorWidget_ResetMonitor_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTStateMonitorWidget_ResetMonitor_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UOWTStateMonitorWidget_ResetMonitor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTStateMonitorWidget_ResetMonitor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UOWTStateMonitorWidget::execResetMonitor)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ResetMonitor();
	P_NATIVE_END;
}
// ********** End Class UOWTStateMonitorWidget Function ResetMonitor *******************************

// ********** Begin Class UOWTStateMonitorWidget Function SubmitSnapshot ***************************
struct Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics
{
	struct OWTStateMonitorWidget_eventSubmitSnapshot_Parms
	{
		FName SourceId;
		FText Label;
		int32 Snapshot;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "AutoCreateRefTerm", "Snapshot" },
		{ "Category", "State Monitor" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Blueprint accepts any reflected struct. Native callers use SubmitStructSnapshot. */" },
#endif
		{ "CustomStructureParam", "Snapshot" },
		{ "CustomThunk", "true" },
		{ "ModuleRelativePath", "Public/OWTStateMonitorWidget.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Blueprint accepts any reflected struct. Native callers use SubmitStructSnapshot." },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Snapshot_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function SubmitSnapshot constinit property declarations ************************
	static const UECodeGen_Private::FNamePropertyParams NewProp_SourceId;
	static const UECodeGen_Private::FTextPropertyParams NewProp_Label;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Snapshot;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SubmitSnapshot constinit property declarations **************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SubmitSnapshot Property Definitions ***********************************
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::NewProp_SourceId = { "SourceId", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTStateMonitorWidget_eventSubmitSnapshot_Parms, SourceId), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::NewProp_Label = { "Label", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTStateMonitorWidget_eventSubmitSnapshot_Parms, Label), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::NewProp_Snapshot = { "Snapshot", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(OWTStateMonitorWidget_eventSubmitSnapshot_Parms, Snapshot), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Snapshot_MetaData), NewProp_Snapshot_MetaData) };
void Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((OWTStateMonitorWidget_eventSubmitSnapshot_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(OWTStateMonitorWidget_eventSubmitSnapshot_Parms), &Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::NewProp_SourceId,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::NewProp_Label,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::NewProp_Snapshot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::PropPointers) < 2048);
// ********** End Function SubmitSnapshot Property Definitions *************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UOWTStateMonitorWidget, nullptr, "SubmitSnapshot", 	Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::OWTStateMonitorWidget_eventSubmitSnapshot_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::Function_MetaDataParams), Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::OWTStateMonitorWidget_eventSubmitSnapshot_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot_Statics::FuncParams);
	}
	return ReturnFunction;
}
// ********** End Class UOWTStateMonitorWidget Function SubmitSnapshot *****************************

// ********** Begin Class UOWTStateMonitorWidget ***************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UOWTStateMonitorWidget;
UClass* UOWTStateMonitorWidget::GetPrivateStaticClass()
{
	using TClass = UOWTStateMonitorWidget;
	if (!Z_Registration_Info_UClass_UOWTStateMonitorWidget.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("OWTStateMonitorWidget"),
			Z_Registration_Info_UClass_UOWTStateMonitorWidget.InnerSingleton,
			StaticRegisterNativesUOWTStateMonitorWidget,
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
	return Z_Registration_Info_UClass_UOWTStateMonitorWidget.InnerSingleton;
}
UClass* Z_Construct_UClass_UOWTStateMonitorWidget_NoRegister()
{
	return UOWTStateMonitorWidget::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UOWTStateMonitorWidget_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Embeddable, read-only runtime monitor. Submit reflected value copies from the owning application. */" },
#endif
		{ "DisplayName", "State Monitor" },
		{ "IncludePath", "OWTStateMonitorWidget.h" },
		{ "ModuleRelativePath", "Public/OWTStateMonitorWidget.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Embeddable, read-only runtime monitor. Submit reflected value copies from the owning application." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Class UOWTStateMonitorWidget constinit property declarations *******************
// ********** End Class UOWTStateMonitorWidget constinit property declarations *********************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("ClearHistory"), .Pointer = &UOWTStateMonitorWidget::execClearHistory },
		{ .NameUTF8 = UTF8TEXT("RemoveSource"), .Pointer = &UOWTStateMonitorWidget::execRemoveSource },
		{ .NameUTF8 = UTF8TEXT("ResetBaseline"), .Pointer = &UOWTStateMonitorWidget::execResetBaseline },
		{ .NameUTF8 = UTF8TEXT("ResetMonitor"), .Pointer = &UOWTStateMonitorWidget::execResetMonitor },
		{ .NameUTF8 = UTF8TEXT("SubmitSnapshot"), .Pointer = &UOWTStateMonitorWidget::execSubmitSnapshot },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UOWTStateMonitorWidget_ClearHistory, "ClearHistory" }, // 1652634131
		{ &Z_Construct_UFunction_UOWTStateMonitorWidget_RemoveSource, "RemoveSource" }, // 3982887915
		{ &Z_Construct_UFunction_UOWTStateMonitorWidget_ResetBaseline, "ResetBaseline" }, // 3456174315
		{ &Z_Construct_UFunction_UOWTStateMonitorWidget_ResetMonitor, "ResetMonitor" }, // 2473031928
		{ &Z_Construct_UFunction_UOWTStateMonitorWidget_SubmitSnapshot, "SubmitSnapshot" }, // 2884661744
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOWTStateMonitorWidget>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UOWTStateMonitorWidget_Statics
UObject* (*const Z_Construct_UClass_UOWTStateMonitorWidget_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_OWTStateMonitor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTStateMonitorWidget_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UOWTStateMonitorWidget_Statics::ClassParams = {
	&UOWTStateMonitorWidget::StaticClass,
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
	0x00B000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOWTStateMonitorWidget_Statics::Class_MetaDataParams), Z_Construct_UClass_UOWTStateMonitorWidget_Statics::Class_MetaDataParams)
};
void UOWTStateMonitorWidget::StaticRegisterNativesUOWTStateMonitorWidget()
{
	UClass* Class = UOWTStateMonitorWidget::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UOWTStateMonitorWidget_Statics::Funcs));
}
UClass* Z_Construct_UClass_UOWTStateMonitorWidget()
{
	if (!Z_Registration_Info_UClass_UOWTStateMonitorWidget.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOWTStateMonitorWidget.OuterSingleton, Z_Construct_UClass_UOWTStateMonitorWidget_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UOWTStateMonitorWidget.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UOWTStateMonitorWidget);
UOWTStateMonitorWidget::~UOWTStateMonitorWidget() {}
// ********** End Class UOWTStateMonitorWidget *****************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTStateMonitor_Source_OWTStateMonitor_Public_OWTStateMonitorWidget_h__Script_OWTStateMonitor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOWTStateMonitorWidget, UOWTStateMonitorWidget::StaticClass, TEXT("UOWTStateMonitorWidget"), &Z_Registration_Info_UClass_UOWTStateMonitorWidget, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOWTStateMonitorWidget), 239563561U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTStateMonitor_Source_OWTStateMonitor_Public_OWTStateMonitorWidget_h__Script_OWTStateMonitor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTStateMonitor_Source_OWTStateMonitor_Public_OWTStateMonitorWidget_h__Script_OWTStateMonitor_1444475711{
	TEXT("/Script/OWTStateMonitor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTStateMonitor_Source_OWTStateMonitor_Public_OWTStateMonitorWidget_h__Script_OWTStateMonitor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTStateMonitor_Source_OWTStateMonitor_Public_OWTStateMonitorWidget_h__Script_OWTStateMonitor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
