// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Modes/VTBOWTObjectEditMode.h"
#include "Context/OWTEditContexts.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTObjectEditMode() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
COREUOBJECT_API UClass* Z_Construct_UClass_UScriptStruct_NoRegister();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTAttributeEditMode();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTObjectEditMode();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTObjectEditMode_NoRegister();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTContextHandlerBinding();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTDuplicateSelectionContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTHideSelectionGizmoContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTRedoContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTSelectObjectContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTSetRotationContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTSetScaleContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTSetTranslationContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTToggleCoordinateSystemContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTToggleTransformSplineContext();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTUndoContext();
// ********** End Cross Module References **********************************************************

// ********** Begin ScriptStruct FOWTContextHandlerBinding *****************************************
struct Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics
{
	static inline consteval int32 GetStructSize() { return sizeof(FOWTContextHandlerBinding); }
	static inline consteval int16 GetStructAlignment() { return alignof(FOWTContextHandlerBinding); }
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Receiver_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FunctionName_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
#endif // WITH_METADATA

// ********** Begin ScriptStruct FOWTContextHandlerBinding constinit property declarations *********
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_Receiver;
	static const UECodeGen_Private::FNamePropertyParams NewProp_FunctionName;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End ScriptStruct FOWTContextHandlerBinding constinit property declarations ***********
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FOWTContextHandlerBinding>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
}; // struct Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_FOWTContextHandlerBinding;
class UScriptStruct* FOWTContextHandlerBinding::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTContextHandlerBinding.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_FOWTContextHandlerBinding.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FOWTContextHandlerBinding, (UObject*)Z_Construct_UPackage__Script_VTBOWTEditor(), TEXT("OWTContextHandlerBinding"));
	}
	return Z_Registration_Info_UScriptStruct_FOWTContextHandlerBinding.OuterSingleton;
	}

// ********** Begin ScriptStruct FOWTContextHandlerBinding Property Definitions ********************
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics::NewProp_Receiver = { "Receiver", nullptr, (EPropertyFlags)0x0014000000000000, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTContextHandlerBinding, Receiver), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Receiver_MetaData), NewProp_Receiver_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics::NewProp_FunctionName = { "FunctionName", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FOWTContextHandlerBinding, FunctionName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FunctionName_MetaData), NewProp_FunctionName_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics::NewProp_Receiver,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics::NewProp_FunctionName,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics::PropPointers) < 2048);
// ********** End ScriptStruct FOWTContextHandlerBinding Property Definitions **********************
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
	nullptr,
	&NewStructOps,
	"OWTContextHandlerBinding",
	Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics::PropPointers),
	sizeof(FOWTContextHandlerBinding),
	alignof(FOWTContextHandlerBinding),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FOWTContextHandlerBinding()
{
	if (!Z_Registration_Info_UScriptStruct_FOWTContextHandlerBinding.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_FOWTContextHandlerBinding.InnerSingleton, Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics::StructParams);
	}
	return CastChecked<UScriptStruct>(Z_Registration_Info_UScriptStruct_FOWTContextHandlerBinding.InnerSingleton);
}
// ********** End ScriptStruct FOWTContextHandlerBinding *******************************************

// ********** Begin Class UVTBOWTObjectEditMode Function BindContextHandler ************************
struct Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics
{
	struct VTBOWTObjectEditMode_eventBindContextHandler_Parms
	{
		UScriptStruct* ContextType;
		UObject* Receiver;
		FName FunctionName;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Exactly one typed input parameter, no return value. Rebinding replaces the handler.\n" },
#endif
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Exactly one typed input parameter, no return value. Rebinding replaces the handler." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function BindContextHandler constinit property declarations ********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ContextType;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Receiver;
	static const UECodeGen_Private::FNamePropertyParams NewProp_FunctionName;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function BindContextHandler constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function BindContextHandler Property Definitions *******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::NewProp_ContextType = { "ContextType", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTObjectEditMode_eventBindContextHandler_Parms, ContextType), Z_Construct_UClass_UScriptStruct_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::NewProp_Receiver = { "Receiver", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTObjectEditMode_eventBindContextHandler_Parms, Receiver), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::NewProp_FunctionName = { "FunctionName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTObjectEditMode_eventBindContextHandler_Parms, FunctionName), METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((VTBOWTObjectEditMode_eventBindContextHandler_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VTBOWTObjectEditMode_eventBindContextHandler_Parms), &Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::NewProp_ContextType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::NewProp_Receiver,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::NewProp_FunctionName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::PropPointers) < 2048);
// ********** End Function BindContextHandler Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTObjectEditMode, nullptr, "BindContextHandler", 	Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::VTBOWTObjectEditMode_eventBindContextHandler_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::VTBOWTObjectEditMode_eventBindContextHandler_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTObjectEditMode::execBindContextHandler)
{
	P_GET_OBJECT(UScriptStruct,Z_Param_ContextType);
	P_GET_OBJECT(UObject,Z_Param_Receiver);
	P_GET_PROPERTY(FNameProperty,Z_Param_FunctionName);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->BindContextHandler(Z_Param_ContextType,Z_Param_Receiver,Z_Param_FunctionName);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTObjectEditMode Function BindContextHandler **************************

// ********** Begin Class UVTBOWTObjectEditMode Function DuplicateSelection ************************
struct VTBOWTObjectEditMode_eventDuplicateSelection_Parms
{
	FOWTDuplicateSelectionContext Context;
};
static FName NAME_UVTBOWTObjectEditMode_DuplicateSelection = FName(TEXT("DuplicateSelection"));
void UVTBOWTObjectEditMode::DuplicateSelection(FOWTDuplicateSelectionContext const& Context)
{
	UFunction* Func = FindFunctionChecked(NAME_UVTBOWTObjectEditMode_DuplicateSelection);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
		VTBOWTObjectEditMode_eventDuplicateSelection_Parms Parms;
		Parms.Context=Context;
	ProcessEvent(Func,&Parms);
	}
	else
	{
		DuplicateSelection_Implementation(Context);
	}
}
struct Z_Construct_UFunction_UVTBOWTObjectEditMode_DuplicateSelection_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Context_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function DuplicateSelection constinit property declarations ********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Context;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function DuplicateSelection constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function DuplicateSelection Property Definitions *******************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_DuplicateSelection_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTObjectEditMode_eventDuplicateSelection_Parms, Context), Z_Construct_UScriptStruct_FOWTDuplicateSelectionContext, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Context_MetaData), NewProp_Context_MetaData) }; // 1162015704
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTObjectEditMode_DuplicateSelection_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_DuplicateSelection_Statics::NewProp_Context,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_DuplicateSelection_Statics::PropPointers) < 2048);
// ********** End Function DuplicateSelection Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTObjectEditMode_DuplicateSelection_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTObjectEditMode, nullptr, "DuplicateSelection", 	Z_Construct_UFunction_UVTBOWTObjectEditMode_DuplicateSelection_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_DuplicateSelection_Statics::PropPointers), 
sizeof(VTBOWTObjectEditMode_eventDuplicateSelection_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_DuplicateSelection_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTObjectEditMode_DuplicateSelection_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(VTBOWTObjectEditMode_eventDuplicateSelection_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTObjectEditMode_DuplicateSelection()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTObjectEditMode_DuplicateSelection_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTObjectEditMode::execDuplicateSelection)
{
	P_GET_STRUCT_REF(FOWTDuplicateSelectionContext,Z_Param_Out_Context);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->DuplicateSelection_Implementation(Z_Param_Out_Context);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTObjectEditMode Function DuplicateSelection **************************

// ********** Begin Class UVTBOWTObjectEditMode Function HideSelectionGizmo ************************
struct VTBOWTObjectEditMode_eventHideSelectionGizmo_Parms
{
	FOWTHideSelectionGizmoContext Context;
};
static FName NAME_UVTBOWTObjectEditMode_HideSelectionGizmo = FName(TEXT("HideSelectionGizmo"));
void UVTBOWTObjectEditMode::HideSelectionGizmo(FOWTHideSelectionGizmoContext const& Context)
{
	UFunction* Func = FindFunctionChecked(NAME_UVTBOWTObjectEditMode_HideSelectionGizmo);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
		VTBOWTObjectEditMode_eventHideSelectionGizmo_Parms Parms;
		Parms.Context=Context;
	ProcessEvent(Func,&Parms);
	}
	else
	{
		HideSelectionGizmo_Implementation(Context);
	}
}
struct Z_Construct_UFunction_UVTBOWTObjectEditMode_HideSelectionGizmo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Context_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function HideSelectionGizmo constinit property declarations ********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Context;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function HideSelectionGizmo constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function HideSelectionGizmo Property Definitions *******************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_HideSelectionGizmo_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTObjectEditMode_eventHideSelectionGizmo_Parms, Context), Z_Construct_UScriptStruct_FOWTHideSelectionGizmoContext, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Context_MetaData), NewProp_Context_MetaData) }; // 1898251007
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTObjectEditMode_HideSelectionGizmo_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_HideSelectionGizmo_Statics::NewProp_Context,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_HideSelectionGizmo_Statics::PropPointers) < 2048);
// ********** End Function HideSelectionGizmo Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTObjectEditMode_HideSelectionGizmo_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTObjectEditMode, nullptr, "HideSelectionGizmo", 	Z_Construct_UFunction_UVTBOWTObjectEditMode_HideSelectionGizmo_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_HideSelectionGizmo_Statics::PropPointers), 
sizeof(VTBOWTObjectEditMode_eventHideSelectionGizmo_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_HideSelectionGizmo_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTObjectEditMode_HideSelectionGizmo_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(VTBOWTObjectEditMode_eventHideSelectionGizmo_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTObjectEditMode_HideSelectionGizmo()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTObjectEditMode_HideSelectionGizmo_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTObjectEditMode::execHideSelectionGizmo)
{
	P_GET_STRUCT_REF(FOWTHideSelectionGizmoContext,Z_Param_Out_Context);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->HideSelectionGizmo_Implementation(Z_Param_Out_Context);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTObjectEditMode Function HideSelectionGizmo **************************

// ********** Begin Class UVTBOWTObjectEditMode Function InitializeContextBindings *****************
static FName NAME_UVTBOWTObjectEditMode_InitializeContextBindings = FName(TEXT("InitializeContextBindings"));
void UVTBOWTObjectEditMode::InitializeContextBindings()
{
	UFunction* Func = FindFunctionChecked(NAME_UVTBOWTObjectEditMode_InitializeContextBindings);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
	ProcessEvent(Func,NULL);
	}
	else
	{
		InitializeContextBindings_Implementation();
	}
}
struct Z_Construct_UFunction_UVTBOWTObjectEditMode_InitializeContextBindings_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Runs once on first dispatch, after BP construction. Call parent when overriding.\n" },
#endif
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Runs once on first dispatch, after BP construction. Call parent when overriding." },
#endif
	};
#endif // WITH_METADATA

// ********** Begin Function InitializeContextBindings constinit property declarations *************
// ********** End Function InitializeContextBindings constinit property declarations ***************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTObjectEditMode_InitializeContextBindings_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTObjectEditMode, nullptr, "InitializeContextBindings", 	nullptr, 
	0, 
0,
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_InitializeContextBindings_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTObjectEditMode_InitializeContextBindings_Statics::Function_MetaDataParams)},  };
UFunction* Z_Construct_UFunction_UVTBOWTObjectEditMode_InitializeContextBindings()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTObjectEditMode_InitializeContextBindings_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTObjectEditMode::execInitializeContextBindings)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->InitializeContextBindings_Implementation();
	P_NATIVE_END;
}
// ********** End Class UVTBOWTObjectEditMode Function InitializeContextBindings *******************

// ********** Begin Class UVTBOWTObjectEditMode Function Redo **************************************
struct VTBOWTObjectEditMode_eventRedo_Parms
{
	FOWTRedoContext Context;
};
static FName NAME_UVTBOWTObjectEditMode_Redo = FName(TEXT("Redo"));
void UVTBOWTObjectEditMode::Redo(FOWTRedoContext const& Context)
{
	UFunction* Func = FindFunctionChecked(NAME_UVTBOWTObjectEditMode_Redo);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
		VTBOWTObjectEditMode_eventRedo_Parms Parms;
		Parms.Context=Context;
	ProcessEvent(Func,&Parms);
	}
	else
	{
		Redo_Implementation(Context);
	}
}
struct Z_Construct_UFunction_UVTBOWTObjectEditMode_Redo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Context_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function Redo constinit property declarations **********************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Context;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function Redo constinit property declarations ************************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function Redo Property Definitions *********************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_Redo_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTObjectEditMode_eventRedo_Parms, Context), Z_Construct_UScriptStruct_FOWTRedoContext, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Context_MetaData), NewProp_Context_MetaData) }; // 1020009757
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTObjectEditMode_Redo_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_Redo_Statics::NewProp_Context,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_Redo_Statics::PropPointers) < 2048);
// ********** End Function Redo Property Definitions ***********************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTObjectEditMode_Redo_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTObjectEditMode, nullptr, "Redo", 	Z_Construct_UFunction_UVTBOWTObjectEditMode_Redo_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_Redo_Statics::PropPointers), 
sizeof(VTBOWTObjectEditMode_eventRedo_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_Redo_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTObjectEditMode_Redo_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(VTBOWTObjectEditMode_eventRedo_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTObjectEditMode_Redo()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTObjectEditMode_Redo_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTObjectEditMode::execRedo)
{
	P_GET_STRUCT_REF(FOWTRedoContext,Z_Param_Out_Context);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Redo_Implementation(Z_Param_Out_Context);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTObjectEditMode Function Redo ****************************************

// ********** Begin Class UVTBOWTObjectEditMode Function SelectObject ******************************
struct VTBOWTObjectEditMode_eventSelectObject_Parms
{
	FOWTSelectObjectContext Context;
};
static FName NAME_UVTBOWTObjectEditMode_SelectObject = FName(TEXT("SelectObject"));
void UVTBOWTObjectEditMode::SelectObject(FOWTSelectObjectContext const& Context)
{
	UFunction* Func = FindFunctionChecked(NAME_UVTBOWTObjectEditMode_SelectObject);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
		VTBOWTObjectEditMode_eventSelectObject_Parms Parms;
		Parms.Context=Context;
	ProcessEvent(Func,&Parms);
	}
	else
	{
		SelectObject_Implementation(Context);
	}
}
struct Z_Construct_UFunction_UVTBOWTObjectEditMode_SelectObject_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Context_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function SelectObject constinit property declarations **************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Context;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SelectObject constinit property declarations ****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SelectObject Property Definitions *************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_SelectObject_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTObjectEditMode_eventSelectObject_Parms, Context), Z_Construct_UScriptStruct_FOWTSelectObjectContext, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Context_MetaData), NewProp_Context_MetaData) }; // 2535039331
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTObjectEditMode_SelectObject_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_SelectObject_Statics::NewProp_Context,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_SelectObject_Statics::PropPointers) < 2048);
// ********** End Function SelectObject Property Definitions ***************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTObjectEditMode_SelectObject_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTObjectEditMode, nullptr, "SelectObject", 	Z_Construct_UFunction_UVTBOWTObjectEditMode_SelectObject_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_SelectObject_Statics::PropPointers), 
sizeof(VTBOWTObjectEditMode_eventSelectObject_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_SelectObject_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTObjectEditMode_SelectObject_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(VTBOWTObjectEditMode_eventSelectObject_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTObjectEditMode_SelectObject()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTObjectEditMode_SelectObject_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTObjectEditMode::execSelectObject)
{
	P_GET_STRUCT_REF(FOWTSelectObjectContext,Z_Param_Out_Context);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SelectObject_Implementation(Z_Param_Out_Context);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTObjectEditMode Function SelectObject ********************************

// ********** Begin Class UVTBOWTObjectEditMode Function SetRotation *******************************
struct VTBOWTObjectEditMode_eventSetRotation_Parms
{
	FOWTSetRotationContext Context;
};
static FName NAME_UVTBOWTObjectEditMode_SetRotation = FName(TEXT("SetRotation"));
void UVTBOWTObjectEditMode::SetRotation(FOWTSetRotationContext const& Context)
{
	UFunction* Func = FindFunctionChecked(NAME_UVTBOWTObjectEditMode_SetRotation);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
		VTBOWTObjectEditMode_eventSetRotation_Parms Parms;
		Parms.Context=Context;
	ProcessEvent(Func,&Parms);
	}
	else
	{
		SetRotation_Implementation(Context);
	}
}
struct Z_Construct_UFunction_UVTBOWTObjectEditMode_SetRotation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Context_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetRotation constinit property declarations ***************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Context;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetRotation constinit property declarations *****************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetRotation Property Definitions **************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_SetRotation_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTObjectEditMode_eventSetRotation_Parms, Context), Z_Construct_UScriptStruct_FOWTSetRotationContext, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Context_MetaData), NewProp_Context_MetaData) }; // 2871991234
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTObjectEditMode_SetRotation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_SetRotation_Statics::NewProp_Context,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_SetRotation_Statics::PropPointers) < 2048);
// ********** End Function SetRotation Property Definitions ****************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTObjectEditMode_SetRotation_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTObjectEditMode, nullptr, "SetRotation", 	Z_Construct_UFunction_UVTBOWTObjectEditMode_SetRotation_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_SetRotation_Statics::PropPointers), 
sizeof(VTBOWTObjectEditMode_eventSetRotation_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_SetRotation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTObjectEditMode_SetRotation_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(VTBOWTObjectEditMode_eventSetRotation_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTObjectEditMode_SetRotation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTObjectEditMode_SetRotation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTObjectEditMode::execSetRotation)
{
	P_GET_STRUCT_REF(FOWTSetRotationContext,Z_Param_Out_Context);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetRotation_Implementation(Z_Param_Out_Context);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTObjectEditMode Function SetRotation *********************************

// ********** Begin Class UVTBOWTObjectEditMode Function SetScale **********************************
struct VTBOWTObjectEditMode_eventSetScale_Parms
{
	FOWTSetScaleContext Context;
};
static FName NAME_UVTBOWTObjectEditMode_SetScale = FName(TEXT("SetScale"));
void UVTBOWTObjectEditMode::SetScale(FOWTSetScaleContext const& Context)
{
	UFunction* Func = FindFunctionChecked(NAME_UVTBOWTObjectEditMode_SetScale);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
		VTBOWTObjectEditMode_eventSetScale_Parms Parms;
		Parms.Context=Context;
	ProcessEvent(Func,&Parms);
	}
	else
	{
		SetScale_Implementation(Context);
	}
}
struct Z_Construct_UFunction_UVTBOWTObjectEditMode_SetScale_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Context_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetScale constinit property declarations ******************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Context;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetScale constinit property declarations ********************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetScale Property Definitions *****************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_SetScale_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTObjectEditMode_eventSetScale_Parms, Context), Z_Construct_UScriptStruct_FOWTSetScaleContext, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Context_MetaData), NewProp_Context_MetaData) }; // 1655865385
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTObjectEditMode_SetScale_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_SetScale_Statics::NewProp_Context,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_SetScale_Statics::PropPointers) < 2048);
// ********** End Function SetScale Property Definitions *******************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTObjectEditMode_SetScale_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTObjectEditMode, nullptr, "SetScale", 	Z_Construct_UFunction_UVTBOWTObjectEditMode_SetScale_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_SetScale_Statics::PropPointers), 
sizeof(VTBOWTObjectEditMode_eventSetScale_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_SetScale_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTObjectEditMode_SetScale_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(VTBOWTObjectEditMode_eventSetScale_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTObjectEditMode_SetScale()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTObjectEditMode_SetScale_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTObjectEditMode::execSetScale)
{
	P_GET_STRUCT_REF(FOWTSetScaleContext,Z_Param_Out_Context);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetScale_Implementation(Z_Param_Out_Context);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTObjectEditMode Function SetScale ************************************

// ********** Begin Class UVTBOWTObjectEditMode Function SetTranslation ****************************
struct VTBOWTObjectEditMode_eventSetTranslation_Parms
{
	FOWTSetTranslationContext Context;
};
static FName NAME_UVTBOWTObjectEditMode_SetTranslation = FName(TEXT("SetTranslation"));
void UVTBOWTObjectEditMode::SetTranslation(FOWTSetTranslationContext const& Context)
{
	UFunction* Func = FindFunctionChecked(NAME_UVTBOWTObjectEditMode_SetTranslation);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
		VTBOWTObjectEditMode_eventSetTranslation_Parms Parms;
		Parms.Context=Context;
	ProcessEvent(Func,&Parms);
	}
	else
	{
		SetTranslation_Implementation(Context);
	}
}
struct Z_Construct_UFunction_UVTBOWTObjectEditMode_SetTranslation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Context_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetTranslation constinit property declarations ************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Context;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetTranslation constinit property declarations **************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetTranslation Property Definitions ***********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_SetTranslation_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTObjectEditMode_eventSetTranslation_Parms, Context), Z_Construct_UScriptStruct_FOWTSetTranslationContext, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Context_MetaData), NewProp_Context_MetaData) }; // 1555986386
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTObjectEditMode_SetTranslation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_SetTranslation_Statics::NewProp_Context,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_SetTranslation_Statics::PropPointers) < 2048);
// ********** End Function SetTranslation Property Definitions *************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTObjectEditMode_SetTranslation_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTObjectEditMode, nullptr, "SetTranslation", 	Z_Construct_UFunction_UVTBOWTObjectEditMode_SetTranslation_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_SetTranslation_Statics::PropPointers), 
sizeof(VTBOWTObjectEditMode_eventSetTranslation_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_SetTranslation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTObjectEditMode_SetTranslation_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(VTBOWTObjectEditMode_eventSetTranslation_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTObjectEditMode_SetTranslation()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTObjectEditMode_SetTranslation_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTObjectEditMode::execSetTranslation)
{
	P_GET_STRUCT_REF(FOWTSetTranslationContext,Z_Param_Out_Context);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SetTranslation_Implementation(Z_Param_Out_Context);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTObjectEditMode Function SetTranslation ******************************

// ********** Begin Class UVTBOWTObjectEditMode Function ToggleCoordinateSystem ********************
struct VTBOWTObjectEditMode_eventToggleCoordinateSystem_Parms
{
	FOWTToggleCoordinateSystemContext Context;
};
static FName NAME_UVTBOWTObjectEditMode_ToggleCoordinateSystem = FName(TEXT("ToggleCoordinateSystem"));
void UVTBOWTObjectEditMode::ToggleCoordinateSystem(FOWTToggleCoordinateSystemContext const& Context)
{
	UFunction* Func = FindFunctionChecked(NAME_UVTBOWTObjectEditMode_ToggleCoordinateSystem);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
		VTBOWTObjectEditMode_eventToggleCoordinateSystem_Parms Parms;
		Parms.Context=Context;
	ProcessEvent(Func,&Parms);
	}
	else
	{
		ToggleCoordinateSystem_Implementation(Context);
	}
}
struct Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleCoordinateSystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Context_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function ToggleCoordinateSystem constinit property declarations ****************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Context;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ToggleCoordinateSystem constinit property declarations ******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ToggleCoordinateSystem Property Definitions ***************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleCoordinateSystem_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTObjectEditMode_eventToggleCoordinateSystem_Parms, Context), Z_Construct_UScriptStruct_FOWTToggleCoordinateSystemContext, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Context_MetaData), NewProp_Context_MetaData) }; // 1436422854
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleCoordinateSystem_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleCoordinateSystem_Statics::NewProp_Context,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleCoordinateSystem_Statics::PropPointers) < 2048);
// ********** End Function ToggleCoordinateSystem Property Definitions *****************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleCoordinateSystem_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTObjectEditMode, nullptr, "ToggleCoordinateSystem", 	Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleCoordinateSystem_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleCoordinateSystem_Statics::PropPointers), 
sizeof(VTBOWTObjectEditMode_eventToggleCoordinateSystem_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleCoordinateSystem_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleCoordinateSystem_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(VTBOWTObjectEditMode_eventToggleCoordinateSystem_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleCoordinateSystem()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleCoordinateSystem_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTObjectEditMode::execToggleCoordinateSystem)
{
	P_GET_STRUCT_REF(FOWTToggleCoordinateSystemContext,Z_Param_Out_Context);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ToggleCoordinateSystem_Implementation(Z_Param_Out_Context);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTObjectEditMode Function ToggleCoordinateSystem **********************

// ********** Begin Class UVTBOWTObjectEditMode Function ToggleTransformSpline *********************
struct VTBOWTObjectEditMode_eventToggleTransformSpline_Parms
{
	FOWTToggleTransformSplineContext Context;
};
static FName NAME_UVTBOWTObjectEditMode_ToggleTransformSpline = FName(TEXT("ToggleTransformSpline"));
void UVTBOWTObjectEditMode::ToggleTransformSpline(FOWTToggleTransformSplineContext const& Context)
{
	UFunction* Func = FindFunctionChecked(NAME_UVTBOWTObjectEditMode_ToggleTransformSpline);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
		VTBOWTObjectEditMode_eventToggleTransformSpline_Parms Parms;
		Parms.Context=Context;
	ProcessEvent(Func,&Parms);
	}
	else
	{
		ToggleTransformSpline_Implementation(Context);
	}
}
struct Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleTransformSpline_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Context_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function ToggleTransformSpline constinit property declarations *****************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Context;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function ToggleTransformSpline constinit property declarations *******************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function ToggleTransformSpline Property Definitions ****************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleTransformSpline_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTObjectEditMode_eventToggleTransformSpline_Parms, Context), Z_Construct_UScriptStruct_FOWTToggleTransformSplineContext, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Context_MetaData), NewProp_Context_MetaData) }; // 3653071224
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleTransformSpline_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleTransformSpline_Statics::NewProp_Context,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleTransformSpline_Statics::PropPointers) < 2048);
// ********** End Function ToggleTransformSpline Property Definitions ******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleTransformSpline_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTObjectEditMode, nullptr, "ToggleTransformSpline", 	Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleTransformSpline_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleTransformSpline_Statics::PropPointers), 
sizeof(VTBOWTObjectEditMode_eventToggleTransformSpline_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleTransformSpline_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleTransformSpline_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(VTBOWTObjectEditMode_eventToggleTransformSpline_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleTransformSpline()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleTransformSpline_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTObjectEditMode::execToggleTransformSpline)
{
	P_GET_STRUCT_REF(FOWTToggleTransformSplineContext,Z_Param_Out_Context);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ToggleTransformSpline_Implementation(Z_Param_Out_Context);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTObjectEditMode Function ToggleTransformSpline ***********************

// ********** Begin Class UVTBOWTObjectEditMode Function UnbindContextHandler **********************
struct Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics
{
	struct VTBOWTObjectEditMode_eventUnbindContextHandler_Parms
	{
		UScriptStruct* ContextType;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function UnbindContextHandler constinit property declarations ******************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ContextType;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function UnbindContextHandler constinit property declarations ********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function UnbindContextHandler Property Definitions *****************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::NewProp_ContextType = { "ContextType", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTObjectEditMode_eventUnbindContextHandler_Parms, ContextType), Z_Construct_UClass_UScriptStruct_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((VTBOWTObjectEditMode_eventUnbindContextHandler_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VTBOWTObjectEditMode_eventUnbindContextHandler_Parms), &Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::NewProp_ContextType,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::PropPointers) < 2048);
// ********** End Function UnbindContextHandler Property Definitions *******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTObjectEditMode, nullptr, "UnbindContextHandler", 	Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::VTBOWTObjectEditMode_eventUnbindContextHandler_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::VTBOWTObjectEditMode_eventUnbindContextHandler_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTObjectEditMode::execUnbindContextHandler)
{
	P_GET_OBJECT(UScriptStruct,Z_Param_ContextType);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->UnbindContextHandler(Z_Param_ContextType);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTObjectEditMode Function UnbindContextHandler ************************

// ********** Begin Class UVTBOWTObjectEditMode Function Undo **************************************
struct VTBOWTObjectEditMode_eventUndo_Parms
{
	FOWTUndoContext Context;
};
static FName NAME_UVTBOWTObjectEditMode_Undo = FName(TEXT("Undo"));
void UVTBOWTObjectEditMode::Undo(FOWTUndoContext const& Context)
{
	UFunction* Func = FindFunctionChecked(NAME_UVTBOWTObjectEditMode_Undo);
	if (!Func->GetOwnerClass()->HasAnyClassFlags(CLASS_Native))
	{
		VTBOWTObjectEditMode_eventUndo_Parms Parms;
		Parms.Context=Context;
	ProcessEvent(Func,&Parms);
	}
	else
	{
		Undo_Implementation(Context);
	}
}
struct Z_Construct_UFunction_UVTBOWTObjectEditMode_Undo_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Context_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function Undo constinit property declarations **********************************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Context;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function Undo constinit property declarations ************************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function Undo Property Definitions *********************************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVTBOWTObjectEditMode_Undo_Statics::NewProp_Context = { "Context", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTObjectEditMode_eventUndo_Parms, Context), Z_Construct_UScriptStruct_FOWTUndoContext, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Context_MetaData), NewProp_Context_MetaData) }; // 2511388460
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTObjectEditMode_Undo_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTObjectEditMode_Undo_Statics::NewProp_Context,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_Undo_Statics::PropPointers) < 2048);
// ********** End Function Undo Property Definitions ***********************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTObjectEditMode_Undo_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTObjectEditMode, nullptr, "Undo", 	Z_Construct_UFunction_UVTBOWTObjectEditMode_Undo_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_Undo_Statics::PropPointers), 
sizeof(VTBOWTObjectEditMode_eventUndo_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTObjectEditMode_Undo_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTObjectEditMode_Undo_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(VTBOWTObjectEditMode_eventUndo_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTObjectEditMode_Undo()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTObjectEditMode_Undo_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTObjectEditMode::execUndo)
{
	P_GET_STRUCT_REF(FOWTUndoContext,Z_Param_Out_Context);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Undo_Implementation(Z_Param_Out_Context);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTObjectEditMode Function Undo ****************************************

// ********** Begin Class UVTBOWTObjectEditMode ****************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTObjectEditMode;
UClass* UVTBOWTObjectEditMode::GetPrivateStaticClass()
{
	using TClass = UVTBOWTObjectEditMode;
	if (!Z_Registration_Info_UClass_UVTBOWTObjectEditMode.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTObjectEditMode"),
			Z_Registration_Info_UClass_UVTBOWTObjectEditMode.InnerSingleton,
			StaticRegisterNativesUVTBOWTObjectEditMode,
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
	return Z_Registration_Info_UClass_UVTBOWTObjectEditMode.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTObjectEditMode_NoRegister()
{
	return UVTBOWTObjectEditMode::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTObjectEditMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Modes/VTBOWTObjectEditMode.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ContextHandlers_MetaData[] = {
		{ "ModuleRelativePath", "Public/Modes/VTBOWTObjectEditMode.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTObjectEditMode constinit property declarations ********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ContextHandlers_ValueProp;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ContextHandlers_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_ContextHandlers;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UVTBOWTObjectEditMode constinit property declarations **********************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("BindContextHandler"), .Pointer = &UVTBOWTObjectEditMode::execBindContextHandler },
		{ .NameUTF8 = UTF8TEXT("DuplicateSelection"), .Pointer = &UVTBOWTObjectEditMode::execDuplicateSelection },
		{ .NameUTF8 = UTF8TEXT("HideSelectionGizmo"), .Pointer = &UVTBOWTObjectEditMode::execHideSelectionGizmo },
		{ .NameUTF8 = UTF8TEXT("InitializeContextBindings"), .Pointer = &UVTBOWTObjectEditMode::execInitializeContextBindings },
		{ .NameUTF8 = UTF8TEXT("Redo"), .Pointer = &UVTBOWTObjectEditMode::execRedo },
		{ .NameUTF8 = UTF8TEXT("SelectObject"), .Pointer = &UVTBOWTObjectEditMode::execSelectObject },
		{ .NameUTF8 = UTF8TEXT("SetRotation"), .Pointer = &UVTBOWTObjectEditMode::execSetRotation },
		{ .NameUTF8 = UTF8TEXT("SetScale"), .Pointer = &UVTBOWTObjectEditMode::execSetScale },
		{ .NameUTF8 = UTF8TEXT("SetTranslation"), .Pointer = &UVTBOWTObjectEditMode::execSetTranslation },
		{ .NameUTF8 = UTF8TEXT("ToggleCoordinateSystem"), .Pointer = &UVTBOWTObjectEditMode::execToggleCoordinateSystem },
		{ .NameUTF8 = UTF8TEXT("ToggleTransformSpline"), .Pointer = &UVTBOWTObjectEditMode::execToggleTransformSpline },
		{ .NameUTF8 = UTF8TEXT("UnbindContextHandler"), .Pointer = &UVTBOWTObjectEditMode::execUnbindContextHandler },
		{ .NameUTF8 = UTF8TEXT("Undo"), .Pointer = &UVTBOWTObjectEditMode::execUndo },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UVTBOWTObjectEditMode_BindContextHandler, "BindContextHandler" }, // 3273888476
		{ &Z_Construct_UFunction_UVTBOWTObjectEditMode_DuplicateSelection, "DuplicateSelection" }, // 3684446741
		{ &Z_Construct_UFunction_UVTBOWTObjectEditMode_HideSelectionGizmo, "HideSelectionGizmo" }, // 2573133395
		{ &Z_Construct_UFunction_UVTBOWTObjectEditMode_InitializeContextBindings, "InitializeContextBindings" }, // 2610867092
		{ &Z_Construct_UFunction_UVTBOWTObjectEditMode_Redo, "Redo" }, // 1873110076
		{ &Z_Construct_UFunction_UVTBOWTObjectEditMode_SelectObject, "SelectObject" }, // 3268080896
		{ &Z_Construct_UFunction_UVTBOWTObjectEditMode_SetRotation, "SetRotation" }, // 138666140
		{ &Z_Construct_UFunction_UVTBOWTObjectEditMode_SetScale, "SetScale" }, // 2340056068
		{ &Z_Construct_UFunction_UVTBOWTObjectEditMode_SetTranslation, "SetTranslation" }, // 932129312
		{ &Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleCoordinateSystem, "ToggleCoordinateSystem" }, // 3518653149
		{ &Z_Construct_UFunction_UVTBOWTObjectEditMode_ToggleTransformSpline, "ToggleTransformSpline" }, // 665332826
		{ &Z_Construct_UFunction_UVTBOWTObjectEditMode_UnbindContextHandler, "UnbindContextHandler" }, // 710127159
		{ &Z_Construct_UFunction_UVTBOWTObjectEditMode_Undo, "Undo" }, // 3970763864
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTObjectEditMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTObjectEditMode_Statics

// ********** Begin Class UVTBOWTObjectEditMode Property Definitions *******************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::NewProp_ContextHandlers_ValueProp = { "ContextHandlers", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UScriptStruct_FOWTContextHandlerBinding, METADATA_PARAMS(0, nullptr) }; // 3647425897
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::NewProp_ContextHandlers_Key_KeyProp = { "ContextHandlers_Key", nullptr, (EPropertyFlags)0x0004000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UScriptStruct_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::NewProp_ContextHandlers = { "ContextHandlers", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVTBOWTObjectEditMode, ContextHandlers), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ContextHandlers_MetaData), NewProp_ContextHandlers_MetaData) }; // 3647425897
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::NewProp_ContextHandlers_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::NewProp_ContextHandlers_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::NewProp_ContextHandlers,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::PropPointers) < 2048);
// ********** End Class UVTBOWTObjectEditMode Property Definitions *********************************
UObject* (*const Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UOWTAttributeEditMode,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::ClassParams = {
	&UVTBOWTObjectEditMode::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::PropPointers),
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::Class_MetaDataParams)
};
void UVTBOWTObjectEditMode::StaticRegisterNativesUVTBOWTObjectEditMode()
{
	UClass* Class = UVTBOWTObjectEditMode::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::Funcs));
}
UClass* Z_Construct_UClass_UVTBOWTObjectEditMode()
{
	if (!Z_Registration_Info_UClass_UVTBOWTObjectEditMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTObjectEditMode.OuterSingleton, Z_Construct_UClass_UVTBOWTObjectEditMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTObjectEditMode.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTObjectEditMode);
UVTBOWTObjectEditMode::~UVTBOWTObjectEditMode() {}
// ********** End Class UVTBOWTObjectEditMode ******************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h__Script_VTBOWTEditor_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FOWTContextHandlerBinding::StaticStruct, Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics::NewStructOps, TEXT("OWTContextHandlerBinding"),&Z_Registration_Info_UScriptStruct_FOWTContextHandlerBinding, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FOWTContextHandlerBinding), 3647425897U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVTBOWTObjectEditMode, UVTBOWTObjectEditMode::StaticClass, TEXT("UVTBOWTObjectEditMode"), &Z_Registration_Info_UClass_UVTBOWTObjectEditMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTObjectEditMode), 3095699129U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h__Script_VTBOWTEditor_28973215{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h__Script_VTBOWTEditor_Statics::ClassInfo),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h__Script_VTBOWTEditor_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h__Script_VTBOWTEditor_Statics::ScriptStructInfo),
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
