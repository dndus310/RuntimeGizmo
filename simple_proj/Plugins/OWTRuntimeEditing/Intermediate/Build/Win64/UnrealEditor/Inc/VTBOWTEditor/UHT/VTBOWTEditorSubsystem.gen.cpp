// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VTBOWTEditorSubsystem.h"
#include "Context/OWTGizmoSnapSettings.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTEditorSubsystem() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UTickableWorldSubsystem();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBAttributeEditor_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTEditContextReceiver_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTBaseTransformGizmo_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTEditorSubsystem();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTEditorSubsystem_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTEditorToolsContext_NoRegister();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTGizmoSnapSettings();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVTBOWTEditorSubsystem Function GetAttributeEditor ***********************
struct Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor_Statics
{
	struct VTBOWTEditorSubsystem_eventGetAttributeEditor_Parms
	{
		AVTBAttributeEditor* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorSubsystem.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetAttributeEditor constinit property declarations ********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetAttributeEditor constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetAttributeEditor Property Definitions *******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTEditorSubsystem_eventGetAttributeEditor_Parms, ReturnValue), Z_Construct_UClass_AVTBAttributeEditor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor_Statics::PropPointers) < 2048);
// ********** End Function GetAttributeEditor Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTEditorSubsystem, nullptr, "GetAttributeEditor", 	Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor_Statics::VTBOWTEditorSubsystem_eventGetAttributeEditor_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor_Statics::VTBOWTEditorSubsystem_eventGetAttributeEditor_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTEditorSubsystem::execGetAttributeEditor)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(AVTBAttributeEditor**)Z_Param__Result=P_THIS->GetAttributeEditor();
	P_NATIVE_END;
}
// ********** End Class UVTBOWTEditorSubsystem Function GetAttributeEditor *************************

// ********** Begin Class UVTBOWTEditorSubsystem Function GetGizmoSnapSettings *********************
struct Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings_Statics
{
	struct VTBOWTEditorSubsystem_eventGetGizmoSnapSettings_Parms
	{
		FOWTGizmoSnapSettings ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Snapping" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorSubsystem.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetGizmoSnapSettings constinit property declarations ******************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetGizmoSnapSettings constinit property declarations ********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetGizmoSnapSettings Property Definitions *****************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTEditorSubsystem_eventGetGizmoSnapSettings_Parms, ReturnValue), Z_Construct_UScriptStruct_FOWTGizmoSnapSettings, METADATA_PARAMS(0, nullptr) }; // 4038205799
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings_Statics::PropPointers) < 2048);
// ********** End Function GetGizmoSnapSettings Property Definitions *******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTEditorSubsystem, nullptr, "GetGizmoSnapSettings", 	Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings_Statics::VTBOWTEditorSubsystem_eventGetGizmoSnapSettings_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings_Statics::VTBOWTEditorSubsystem_eventGetGizmoSnapSettings_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTEditorSubsystem::execGetGizmoSnapSettings)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FOWTGizmoSnapSettings*)Z_Param__Result=P_THIS->GetGizmoSnapSettings();
	P_NATIVE_END;
}
// ********** End Class UVTBOWTEditorSubsystem Function GetGizmoSnapSettings ***********************

// ********** Begin Class UVTBOWTEditorSubsystem Function SetActiveEditMode ************************
struct Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics
{
	struct VTBOWTEditorSubsystem_eventSetActiveEditMode_Parms
	{
		UObject* Mode;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorSubsystem.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetActiveEditMode constinit property declarations *********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Mode;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetActiveEditMode constinit property declarations ***********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetActiveEditMode Property Definitions ********************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::NewProp_Mode = { "Mode", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTEditorSubsystem_eventSetActiveEditMode_Parms, Mode), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((VTBOWTEditorSubsystem_eventSetActiveEditMode_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VTBOWTEditorSubsystem_eventSetActiveEditMode_Parms), &Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::NewProp_Mode,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::PropPointers) < 2048);
// ********** End Function SetActiveEditMode Property Definitions **********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTEditorSubsystem, nullptr, "SetActiveEditMode", 	Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::VTBOWTEditorSubsystem_eventSetActiveEditMode_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::VTBOWTEditorSubsystem_eventSetActiveEditMode_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTEditorSubsystem::execSetActiveEditMode)
{
	P_GET_OBJECT(UObject,Z_Param_Mode);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->SetActiveEditMode(Z_Param_Mode);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTEditorSubsystem Function SetActiveEditMode **************************

// ********** Begin Class UVTBOWTEditorSubsystem Function SetGizmoSnapSettings *********************
struct Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics
{
	struct VTBOWTEditorSubsystem_eventSetGizmoSnapSettings_Parms
	{
		FOWTGizmoSnapSettings Settings;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Snapping" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Settings_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetGizmoSnapSettings constinit property declarations ******************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Settings;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetGizmoSnapSettings constinit property declarations ********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetGizmoSnapSettings Property Definitions *****************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::NewProp_Settings = { "Settings", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTEditorSubsystem_eventSetGizmoSnapSettings_Parms, Settings), Z_Construct_UScriptStruct_FOWTGizmoSnapSettings, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Settings_MetaData), NewProp_Settings_MetaData) }; // 4038205799
void Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((VTBOWTEditorSubsystem_eventSetGizmoSnapSettings_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VTBOWTEditorSubsystem_eventSetGizmoSnapSettings_Parms), &Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::NewProp_Settings,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::PropPointers) < 2048);
// ********** End Function SetGizmoSnapSettings Property Definitions *******************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTEditorSubsystem, nullptr, "SetGizmoSnapSettings", 	Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::VTBOWTEditorSubsystem_eventSetGizmoSnapSettings_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::VTBOWTEditorSubsystem_eventSetGizmoSnapSettings_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTEditorSubsystem::execSetGizmoSnapSettings)
{
	P_GET_STRUCT_REF(FOWTGizmoSnapSettings,Z_Param_Out_Settings);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->SetGizmoSnapSettings(Z_Param_Out_Settings);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTEditorSubsystem Function SetGizmoSnapSettings ***********************

// ********** Begin Class UVTBOWTEditorSubsystem ***************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTEditorSubsystem;
UClass* UVTBOWTEditorSubsystem::GetPrivateStaticClass()
{
	using TClass = UVTBOWTEditorSubsystem;
	if (!Z_Registration_Info_UClass_UVTBOWTEditorSubsystem.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTEditorSubsystem"),
			Z_Registration_Info_UClass_UVTBOWTEditorSubsystem.InnerSingleton,
			StaticRegisterNativesUVTBOWTEditorSubsystem,
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
	return Z_Registration_Info_UClass_UVTBOWTEditorSubsystem.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTEditorSubsystem_NoRegister()
{
	return UVTBOWTEditorSubsystem::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "VTBOWTEditorSubsystem.h" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ActiveEditMode_MetaData[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SelectedObject_MetaData[] = {
		{ "Category", "OWT|Editing" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ToolsContext_MetaData[] = {
		{ "ModuleRelativePath", "Public/VTBOWTEditorSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TransformGizmo_MetaData[] = {
		{ "ModuleRelativePath", "Public/VTBOWTEditorSubsystem.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTEditorSubsystem constinit property declarations *******************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ActiveEditMode;
	static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_SelectedObject;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ToolsContext;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TransformGizmo;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UVTBOWTEditorSubsystem constinit property declarations *********************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("GetAttributeEditor"), .Pointer = &UVTBOWTEditorSubsystem::execGetAttributeEditor },
		{ .NameUTF8 = UTF8TEXT("GetGizmoSnapSettings"), .Pointer = &UVTBOWTEditorSubsystem::execGetGizmoSnapSettings },
		{ .NameUTF8 = UTF8TEXT("SetActiveEditMode"), .Pointer = &UVTBOWTEditorSubsystem::execSetActiveEditMode },
		{ .NameUTF8 = UTF8TEXT("SetGizmoSnapSettings"), .Pointer = &UVTBOWTEditorSubsystem::execSetGizmoSnapSettings },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetAttributeEditor, "GetAttributeEditor" }, // 2475848798
		{ &Z_Construct_UFunction_UVTBOWTEditorSubsystem_GetGizmoSnapSettings, "GetGizmoSnapSettings" }, // 430909826
		{ &Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetActiveEditMode, "SetActiveEditMode" }, // 941244997
		{ &Z_Construct_UFunction_UVTBOWTEditorSubsystem_SetGizmoSnapSettings, "SetGizmoSnapSettings" }, // 2711431774
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static const UECodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTEditorSubsystem>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics

// ********** Begin Class UVTBOWTEditorSubsystem Property Definitions ******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::NewProp_ActiveEditMode = { "ActiveEditMode", nullptr, (EPropertyFlags)0x0114000000002014, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVTBOWTEditorSubsystem, ActiveEditMode), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ActiveEditMode_MetaData), NewProp_ActiveEditMode_MetaData) };
const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::NewProp_SelectedObject = { "SelectedObject", nullptr, (EPropertyFlags)0x0014000000002014, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVTBOWTEditorSubsystem, SelectedObject), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SelectedObject_MetaData), NewProp_SelectedObject_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::NewProp_ToolsContext = { "ToolsContext", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVTBOWTEditorSubsystem, ToolsContext), Z_Construct_UClass_UVTBOWTEditorToolsContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ToolsContext_MetaData), NewProp_ToolsContext_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::NewProp_TransformGizmo = { "TransformGizmo", nullptr, (EPropertyFlags)0x0144000000002000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVTBOWTEditorSubsystem, TransformGizmo), Z_Construct_UClass_UVTBOWTBaseTransformGizmo_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TransformGizmo_MetaData), NewProp_TransformGizmo_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::NewProp_ActiveEditMode,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::NewProp_SelectedObject,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::NewProp_ToolsContext,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::NewProp_TransformGizmo,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::PropPointers) < 2048);
// ********** End Class UVTBOWTEditorSubsystem Property Definitions ********************************
UObject* (*const Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UTickableWorldSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::InterfaceParams[] = {
	{ Z_Construct_UClass_UOWTEditContextReceiver_NoRegister, (int32)VTABLE_OFFSET(UVTBOWTEditorSubsystem, IOWTEditContextReceiver), false },  // 1559580018
};
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::ClassParams = {
	&UVTBOWTEditorSubsystem::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::PropPointers,
	InterfaceParams,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::PropPointers),
	UE_ARRAY_COUNT(InterfaceParams),
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::Class_MetaDataParams)
};
void UVTBOWTEditorSubsystem::StaticRegisterNativesUVTBOWTEditorSubsystem()
{
	UClass* Class = UVTBOWTEditorSubsystem::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::Funcs));
}
UClass* Z_Construct_UClass_UVTBOWTEditorSubsystem()
{
	if (!Z_Registration_Info_UClass_UVTBOWTEditorSubsystem.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTEditorSubsystem.OuterSingleton, Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTEditorSubsystem.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTEditorSubsystem);
UVTBOWTEditorSubsystem::~UVTBOWTEditorSubsystem() {}
// ********** End Class UVTBOWTEditorSubsystem *****************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorSubsystem_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVTBOWTEditorSubsystem, UVTBOWTEditorSubsystem::StaticClass, TEXT("UVTBOWTEditorSubsystem"), &Z_Registration_Info_UClass_UVTBOWTEditorSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTEditorSubsystem), 2787357554U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorSubsystem_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorSubsystem_h__Script_VTBOWTEditor_4234487166{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorSubsystem_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorSubsystem_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
