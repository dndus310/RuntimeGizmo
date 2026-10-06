// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Context/VTBOWTEditorToolsContext.h"
#include "Context/OWTGizmoSnapSettings.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTEditorToolsContext() {}

// ********** Begin Cross Module References ********************************************************
INTERACTIVETOOLSFRAMEWORK_API UClass* Z_Construct_UClass_UInteractiveToolsContext();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTEditorToolsContext();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTEditorToolsContext_NoRegister();
VTBOWTEDITOR_API UScriptStruct* Z_Construct_UScriptStruct_FOWTGizmoSnapSettings();
// ********** End Cross Module References **********************************************************

// ********** Begin Class UVTBOWTEditorToolsContext Function GetSnapSettings ***********************
struct Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings_Statics
{
	struct VTBOWTEditorToolsContext_eventGetSnapSettings_Parms
	{
		FOWTGizmoSnapSettings ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Snapping" },
		{ "ModuleRelativePath", "Public/Context/VTBOWTEditorToolsContext.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetSnapSettings constinit property declarations ***********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetSnapSettings constinit property declarations *************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetSnapSettings Property Definitions **********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTEditorToolsContext_eventGetSnapSettings_Parms, ReturnValue), Z_Construct_UScriptStruct_FOWTGizmoSnapSettings, METADATA_PARAMS(0, nullptr) }; // 4038205799
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings_Statics::PropPointers) < 2048);
// ********** End Function GetSnapSettings Property Definitions ************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTEditorToolsContext, nullptr, "GetSnapSettings", 	Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings_Statics::VTBOWTEditorToolsContext_eventGetSnapSettings_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings_Statics::VTBOWTEditorToolsContext_eventGetSnapSettings_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTEditorToolsContext::execGetSnapSettings)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(FOWTGizmoSnapSettings*)Z_Param__Result=P_THIS->GetSnapSettings();
	P_NATIVE_END;
}
// ********** End Class UVTBOWTEditorToolsContext Function GetSnapSettings *************************

// ********** Begin Class UVTBOWTEditorToolsContext Function SetSnapSettings ***********************
struct Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics
{
	struct VTBOWTEditorToolsContext_eventSetSnapSettings_Parms
	{
		FOWTGizmoSnapSettings Settings;
		bool ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "OWT|Snapping" },
		{ "ModuleRelativePath", "Public/Context/VTBOWTEditorToolsContext.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Settings_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA

// ********** Begin Function SetSnapSettings constinit property declarations ***********************
	static const UECodeGen_Private::FStructPropertyParams NewProp_Settings;
	static void NewProp_ReturnValue_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function SetSnapSettings constinit property declarations *************************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function SetSnapSettings Property Definitions **********************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::NewProp_Settings = { "Settings", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTEditorToolsContext_eventSetSnapSettings_Parms, Settings), Z_Construct_UScriptStruct_FOWTGizmoSnapSettings, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Settings_MetaData), NewProp_Settings_MetaData) }; // 4038205799
void Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::NewProp_ReturnValue_SetBit(void* Obj)
{
	((VTBOWTEditorToolsContext_eventSetSnapSettings_Parms*)Obj)->ReturnValue = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VTBOWTEditorToolsContext_eventSetSnapSettings_Parms), &Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::NewProp_Settings,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::PropPointers) < 2048);
// ********** End Function SetSnapSettings Property Definitions ************************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTEditorToolsContext, nullptr, "SetSnapSettings", 	Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::PropPointers), 
sizeof(Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::VTBOWTEditorToolsContext_eventSetSnapSettings_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::VTBOWTEditorToolsContext_eventSetSnapSettings_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UVTBOWTEditorToolsContext::execSetSnapSettings)
{
	P_GET_STRUCT_REF(FOWTGizmoSnapSettings,Z_Param_Out_Settings);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(bool*)Z_Param__Result=P_THIS->SetSnapSettings(Z_Param_Out_Settings);
	P_NATIVE_END;
}
// ********** End Class UVTBOWTEditorToolsContext Function SetSnapSettings *************************

// ********** Begin Class UVTBOWTEditorToolsContext ************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTEditorToolsContext;
UClass* UVTBOWTEditorToolsContext::GetPrivateStaticClass()
{
	using TClass = UVTBOWTEditorToolsContext;
	if (!Z_Registration_Info_UClass_UVTBOWTEditorToolsContext.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTEditorToolsContext"),
			Z_Registration_Info_UClass_UVTBOWTEditorToolsContext.InnerSingleton,
			StaticRegisterNativesUVTBOWTEditorToolsContext,
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
	return Z_Registration_Info_UClass_UVTBOWTEditorToolsContext.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTEditorToolsContext_NoRegister()
{
	return UVTBOWTEditorToolsContext::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Context/VTBOWTEditorToolsContext.h" },
		{ "ModuleRelativePath", "Public/Context/VTBOWTEditorToolsContext.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SnapSettings_MetaData[] = {
		{ "ModuleRelativePath", "Public/Context/VTBOWTEditorToolsContext.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class UVTBOWTEditorToolsContext constinit property declarations ****************
	static const UECodeGen_Private::FStructPropertyParams NewProp_SnapSettings;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class UVTBOWTEditorToolsContext constinit property declarations ******************
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("GetSnapSettings"), .Pointer = &UVTBOWTEditorToolsContext::execGetSnapSettings },
		{ .NameUTF8 = UTF8TEXT("SetSnapSettings"), .Pointer = &UVTBOWTEditorToolsContext::execSetSnapSettings },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UVTBOWTEditorToolsContext_GetSnapSettings, "GetSnapSettings" }, // 211811575
		{ &Z_Construct_UFunction_UVTBOWTEditorToolsContext_SetSnapSettings, "SetSnapSettings" }, // 1829244679
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVTBOWTEditorToolsContext>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics

// ********** Begin Class UVTBOWTEditorToolsContext Property Definitions ***************************
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics::NewProp_SnapSettings = { "SnapSettings", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVTBOWTEditorToolsContext, SnapSettings), Z_Construct_UScriptStruct_FOWTGizmoSnapSettings, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SnapSettings_MetaData), NewProp_SnapSettings_MetaData) }; // 4038205799
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics::NewProp_SnapSettings,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics::PropPointers) < 2048);
// ********** End Class UVTBOWTEditorToolsContext Property Definitions *****************************
UObject* (*const Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInteractiveToolsContext,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics::ClassParams = {
	&UVTBOWTEditorToolsContext::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics::PropPointers),
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics::Class_MetaDataParams)
};
void UVTBOWTEditorToolsContext::StaticRegisterNativesUVTBOWTEditorToolsContext()
{
	UClass* Class = UVTBOWTEditorToolsContext::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics::Funcs));
}
UClass* Z_Construct_UClass_UVTBOWTEditorToolsContext()
{
	if (!Z_Registration_Info_UClass_UVTBOWTEditorToolsContext.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTEditorToolsContext.OuterSingleton, Z_Construct_UClass_UVTBOWTEditorToolsContext_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTEditorToolsContext.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTEditorToolsContext);
UVTBOWTEditorToolsContext::~UVTBOWTEditorToolsContext() {}
// ********** End Class UVTBOWTEditorToolsContext **************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_VTBOWTEditorToolsContext_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVTBOWTEditorToolsContext, UVTBOWTEditorToolsContext::StaticClass, TEXT("UVTBOWTEditorToolsContext"), &Z_Registration_Info_UClass_UVTBOWTEditorToolsContext, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTEditorToolsContext), 1602538083U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_VTBOWTEditorToolsContext_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_VTBOWTEditorToolsContext_h__Script_VTBOWTEditor_2103366763{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_VTBOWTEditorToolsContext_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Context_VTBOWTEditorToolsContext_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
