// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeOWTEventCore_init() {}
static_assert(!UE_WITH_CONSTINIT_UOBJECT, "This generated code can only be compiled with !UE_WITH_CONSTINIT_OBJECT");	OWTEVENTCORE_API UFunction* Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature();
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_OWTEventCore;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_OWTEventCore()
	{
		if (!Z_Registration_Info_UPackage__Script_OWTEventCore.OuterSingleton)
		{
		static UObject* (*const SingletonFuncArray[])() = {
			(UObject* (*)())Z_Construct_UDelegateFunction_OWTEventCore_OWTAttributeEventDynamic__DelegateSignature,
		};
		static const UECodeGen_Private::FPackageParams PackageParams = {
			"/Script/OWTEventCore",
			SingletonFuncArray,
			UE_ARRAY_COUNT(SingletonFuncArray),
			PKG_CompiledIn | 0x00000000,
			0x0FA5C6A8,
			0x22BA8161,
			METADATA_PARAMS(0, nullptr)
		};
		UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_OWTEventCore.OuterSingleton, PackageParams);
	}
	return Z_Registration_Info_UPackage__Script_OWTEventCore.OuterSingleton;
}
static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_OWTEventCore(Z_Construct_UPackage__Script_OWTEventCore, TEXT("/Script/OWTEventCore"), Z_Registration_Info_UPackage__Script_OWTEventCore, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x0FA5C6A8, 0x22BA8161));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
