// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VTBOWTEditorGameMode.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");
void EmptyLinkFunctionForGeneratedCodeVTBOWTEditorGameMode() {}

// ********** Begin Cross Module References ********************************************************
COREUOBJECT_API UClass* Z_Construct_UClass_UInterface();
ENGINE_API UClass* Z_Construct_UClass_AGameMode();
UPackage* Z_Construct_UPackage__Script_VTBOWTEditor();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBAttributeEditor_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBOWTEditorGameMode();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBOWTEditorGameMode_NoRegister();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTEditorGameModeProvider();
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTEditorGameModeProvider_NoRegister();
// ********** End Cross Module References **********************************************************

// ********** Begin Interface UVTBOWTEditorGameModeProvider Function GetAttributeEditor ************
struct VTBOWTEditorGameModeProvider_eventGetAttributeEditor_Parms
{
	AVTBAttributeEditor* ReturnValue;

	/** Constructor, initializes return property only **/
	VTBOWTEditorGameModeProvider_eventGetAttributeEditor_Parms()
		: ReturnValue(NULL)
	{
	}
};
AVTBAttributeEditor* IVTBOWTEditorGameModeProvider::GetAttributeEditor()
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_GetAttributeEditor instead.");
	VTBOWTEditorGameModeProvider_eventGetAttributeEditor_Parms Parms;
	return Parms.ReturnValue;
}
static FName NAME_UVTBOWTEditorGameModeProvider_GetAttributeEditor = FName(TEXT("GetAttributeEditor"));
AVTBAttributeEditor* IVTBOWTEditorGameModeProvider::Execute_GetAttributeEditor(UObject* O)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UVTBOWTEditorGameModeProvider::StaticClass()));
	VTBOWTEditorGameModeProvider_eventGetAttributeEditor_Parms Parms;
	UFunction* const Func = O->FindFunction(NAME_UVTBOWTEditorGameModeProvider_GetAttributeEditor);
	if (Func)
	{
		O->ProcessEvent(Func, &Parms);
	}
	else if (auto I = (IVTBOWTEditorGameModeProvider*)(O->GetNativeInterfaceAddress(UVTBOWTEditorGameModeProvider::StaticClass())))
	{
		Parms.ReturnValue = I->GetAttributeEditor_Implementation();
	}
	return Parms.ReturnValue;
}
struct Z_Construct_UFunction_UVTBOWTEditorGameModeProvider_GetAttributeEditor_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/VTBOWTEditorGameMode.h" },
	};
#endif // WITH_METADATA

// ********** Begin Function GetAttributeEditor constinit property declarations ********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Function GetAttributeEditor constinit property declarations **********************
	static const UECodeGen_Private::FFunctionParams FuncParams;
};

// ********** Begin Function GetAttributeEditor Property Definitions *******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVTBOWTEditorGameModeProvider_GetAttributeEditor_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VTBOWTEditorGameModeProvider_eventGetAttributeEditor_Parms, ReturnValue), Z_Construct_UClass_AVTBAttributeEditor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVTBOWTEditorGameModeProvider_GetAttributeEditor_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVTBOWTEditorGameModeProvider_GetAttributeEditor_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorGameModeProvider_GetAttributeEditor_Statics::PropPointers) < 2048);
// ********** End Function GetAttributeEditor Property Definitions *********************************
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVTBOWTEditorGameModeProvider_GetAttributeEditor_Statics::FuncParams = { { (UObject*(*)())Z_Construct_UClass_UVTBOWTEditorGameModeProvider, nullptr, "GetAttributeEditor", 	Z_Construct_UFunction_UVTBOWTEditorGameModeProvider_GetAttributeEditor_Statics::PropPointers, 
	UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorGameModeProvider_GetAttributeEditor_Statics::PropPointers), 
sizeof(VTBOWTEditorGameModeProvider_eventGetAttributeEditor_Parms),
RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVTBOWTEditorGameModeProvider_GetAttributeEditor_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVTBOWTEditorGameModeProvider_GetAttributeEditor_Statics::Function_MetaDataParams)},  };
static_assert(sizeof(VTBOWTEditorGameModeProvider_eventGetAttributeEditor_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UVTBOWTEditorGameModeProvider_GetAttributeEditor()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVTBOWTEditorGameModeProvider_GetAttributeEditor_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IVTBOWTEditorGameModeProvider::execGetAttributeEditor)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	*(AVTBAttributeEditor**)Z_Param__Result=P_THIS->GetAttributeEditor_Implementation();
	P_NATIVE_END;
}
// ********** End Interface UVTBOWTEditorGameModeProvider Function GetAttributeEditor **************

// ********** Begin Interface UVTBOWTEditorGameModeProvider ****************************************
FClassRegistrationInfo Z_Registration_Info_UClass_UVTBOWTEditorGameModeProvider;
UClass* UVTBOWTEditorGameModeProvider::GetPrivateStaticClass()
{
	using TClass = UVTBOWTEditorGameModeProvider;
	if (!Z_Registration_Info_UClass_UVTBOWTEditorGameModeProvider.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTEditorGameModeProvider"),
			Z_Registration_Info_UClass_UVTBOWTEditorGameModeProvider.InnerSingleton,
			StaticRegisterNativesUVTBOWTEditorGameModeProvider,
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
	return Z_Registration_Info_UClass_UVTBOWTEditorGameModeProvider.InnerSingleton;
}
UClass* Z_Construct_UClass_UVTBOWTEditorGameModeProvider_NoRegister()
{
	return UVTBOWTEditorGameModeProvider::GetPrivateStaticClass();
}
struct Z_Construct_UClass_UVTBOWTEditorGameModeProvider_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorGameMode.h" },
	};
#endif // WITH_METADATA

// ********** Begin Interface UVTBOWTEditorGameModeProvider constinit property declarations ********
// ********** End Interface UVTBOWTEditorGameModeProvider constinit property declarations **********
	static constexpr UE::CodeGen::FClassNativeFunction Funcs[] = {
		{ .NameUTF8 = UTF8TEXT("GetAttributeEditor"), .Pointer = &IVTBOWTEditorGameModeProvider::execGetAttributeEditor },
	};
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UVTBOWTEditorGameModeProvider_GetAttributeEditor, "GetAttributeEditor" }, // 4165561447
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<IVTBOWTEditorGameModeProvider>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_UVTBOWTEditorGameModeProvider_Statics
UObject* (*const Z_Construct_UClass_UVTBOWTEditorGameModeProvider_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInterface,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTEditorGameModeProvider_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVTBOWTEditorGameModeProvider_Statics::ClassParams = {
	&UVTBOWTEditorGameModeProvider::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVTBOWTEditorGameModeProvider_Statics::Class_MetaDataParams), Z_Construct_UClass_UVTBOWTEditorGameModeProvider_Statics::Class_MetaDataParams)
};
void UVTBOWTEditorGameModeProvider::StaticRegisterNativesUVTBOWTEditorGameModeProvider()
{
	UClass* Class = UVTBOWTEditorGameModeProvider::StaticClass();
	FNativeFunctionRegistrar::RegisterFunctions(Class, MakeConstArrayView(Z_Construct_UClass_UVTBOWTEditorGameModeProvider_Statics::Funcs));
}
UClass* Z_Construct_UClass_UVTBOWTEditorGameModeProvider()
{
	if (!Z_Registration_Info_UClass_UVTBOWTEditorGameModeProvider.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVTBOWTEditorGameModeProvider.OuterSingleton, Z_Construct_UClass_UVTBOWTEditorGameModeProvider_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVTBOWTEditorGameModeProvider.OuterSingleton;
}
UVTBOWTEditorGameModeProvider::UVTBOWTEditorGameModeProvider(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, UVTBOWTEditorGameModeProvider);
// ********** End Interface UVTBOWTEditorGameModeProvider ******************************************

// ********** Begin Class AVTBOWTEditorGameMode ****************************************************
FClassRegistrationInfo Z_Registration_Info_UClass_AVTBOWTEditorGameMode;
UClass* AVTBOWTEditorGameMode::GetPrivateStaticClass()
{
	using TClass = AVTBOWTEditorGameMode;
	if (!Z_Registration_Info_UClass_AVTBOWTEditorGameMode.InnerSingleton)
	{
		GetPrivateStaticClassBody(
			TClass::StaticPackage(),
			TEXT("VTBOWTEditorGameMode"),
			Z_Registration_Info_UClass_AVTBOWTEditorGameMode.InnerSingleton,
			StaticRegisterNativesAVTBOWTEditorGameMode,
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
	return Z_Registration_Info_UClass_AVTBOWTEditorGameMode.InnerSingleton;
}
UClass* Z_Construct_UClass_AVTBOWTEditorGameMode_NoRegister()
{
	return AVTBOWTEditorGameMode::GetPrivateStaticClass();
}
struct Z_Construct_UClass_AVTBOWTEditorGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "VTBOWTEditorGameMode.h" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AttributeEditor_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Attribute Editor" },
		{ "ModuleRelativePath", "Public/VTBOWTEditorGameMode.h" },
	};
#endif // WITH_METADATA

// ********** Begin Class AVTBOWTEditorGameMode constinit property declarations ********************
	static const UECodeGen_Private::FObjectPropertyParams NewProp_AttributeEditor;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
// ********** End Class AVTBOWTEditorGameMode constinit property declarations **********************
	static UObject* (*const DependentSingletons[])();
	static const UECodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AVTBOWTEditorGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
}; // struct Z_Construct_UClass_AVTBOWTEditorGameMode_Statics

// ********** Begin Class AVTBOWTEditorGameMode Property Definitions *******************************
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AVTBOWTEditorGameMode_Statics::NewProp_AttributeEditor = { "AttributeEditor", nullptr, (EPropertyFlags)0x0144000000022815, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AVTBOWTEditorGameMode, AttributeEditor), Z_Construct_UClass_AVTBAttributeEditor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AttributeEditor_MetaData), NewProp_AttributeEditor_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AVTBOWTEditorGameMode_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AVTBOWTEditorGameMode_Statics::NewProp_AttributeEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTEditorGameMode_Statics::PropPointers) < 2048);
// ********** End Class AVTBOWTEditorGameMode Property Definitions *********************************
UObject* (*const Z_Construct_UClass_AVTBOWTEditorGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameMode,
	(UObject* (*)())Z_Construct_UPackage__Script_VTBOWTEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTEditorGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_AVTBOWTEditorGameMode_Statics::InterfaceParams[] = {
	{ Z_Construct_UClass_UVTBOWTEditorGameModeProvider_NoRegister, (int32)VTABLE_OFFSET(AVTBOWTEditorGameMode, IVTBOWTEditorGameModeProvider), false },  // 1319997227
};
const UECodeGen_Private::FClassParams Z_Construct_UClass_AVTBOWTEditorGameMode_Statics::ClassParams = {
	&AVTBOWTEditorGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_AVTBOWTEditorGameMode_Statics::PropPointers,
	InterfaceParams,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTEditorGameMode_Statics::PropPointers),
	UE_ARRAY_COUNT(InterfaceParams),
	0x009002ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AVTBOWTEditorGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_AVTBOWTEditorGameMode_Statics::Class_MetaDataParams)
};
void AVTBOWTEditorGameMode::StaticRegisterNativesAVTBOWTEditorGameMode()
{
}
UClass* Z_Construct_UClass_AVTBOWTEditorGameMode()
{
	if (!Z_Registration_Info_UClass_AVTBOWTEditorGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AVTBOWTEditorGameMode.OuterSingleton, Z_Construct_UClass_AVTBOWTEditorGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AVTBOWTEditorGameMode.OuterSingleton;
}
DEFINE_VTABLE_PTR_HELPER_CTOR_NS(, AVTBOWTEditorGameMode);
AVTBOWTEditorGameMode::~AVTBOWTEditorGameMode() {}
// ********** End Class AVTBOWTEditorGameMode ******************************************************

// ********** Begin Registration *******************************************************************
struct Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h__Script_VTBOWTEditor_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVTBOWTEditorGameModeProvider, UVTBOWTEditorGameModeProvider::StaticClass, TEXT("UVTBOWTEditorGameModeProvider"), &Z_Registration_Info_UClass_UVTBOWTEditorGameModeProvider, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVTBOWTEditorGameModeProvider), 1319997227U) },
		{ Z_Construct_UClass_AVTBOWTEditorGameMode, AVTBOWTEditorGameMode::StaticClass, TEXT("AVTBOWTEditorGameMode"), &Z_Registration_Info_UClass_AVTBOWTEditorGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AVTBOWTEditorGameMode), 3140759098U) },
	};
}; // Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h__Script_VTBOWTEditor_Statics 
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h__Script_VTBOWTEditor_3146976351{
	TEXT("/Script/VTBOWTEditor"),
	Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h__Script_VTBOWTEditor_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h__Script_VTBOWTEditor_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0,
};
// ********** End Registration *********************************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
