// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Interfaces/OWTEditContextReceiver.h"

#ifdef VTBOWTEDITOR_OWTEditContextReceiver_generated_h
#error "OWTEditContextReceiver.generated.h already included, missing '#pragma once' in OWTEditContextReceiver.h"
#endif
#define VTBOWTEDITOR_OWTEditContextReceiver_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FInstancedStruct;

// ********** Begin Interface UOWTEditContextReceiver **********************************************
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h_11_RPC_WRAPPERS_NO_PURE_DECLS \
	virtual bool ReceiveEditContext_Implementation(FInstancedStruct const& Context) { return false; }; \
	DECLARE_FUNCTION(execReceiveEditContext);


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h_11_CALLBACK_WRAPPERS
struct Z_Construct_UClass_UOWTEditContextReceiver_Statics;
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UOWTEditContextReceiver_NoRegister();

#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h_11_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UOWTEditContextReceiver(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UOWTEditContextReceiver(UOWTEditContextReceiver&&) = delete; \
	UOWTEditContextReceiver(const UOWTEditContextReceiver&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UOWTEditContextReceiver); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UOWTEditContextReceiver); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UOWTEditContextReceiver) \
	virtual ~UOWTEditContextReceiver() = default;


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h_11_GENERATED_UINTERFACE_BODY() \
private: \
	static void StaticRegisterNativesUOWTEditContextReceiver(); \
	friend struct ::Z_Construct_UClass_UOWTEditContextReceiver_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend VTBOWTEDITOR_API UClass* ::Z_Construct_UClass_UOWTEditContextReceiver_NoRegister(); \
public: \
	DECLARE_CLASS2(UOWTEditContextReceiver, UInterface, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Interface), CASTCLASS_None, TEXT("/Script/VTBOWTEditor"), Z_Construct_UClass_UOWTEditContextReceiver_NoRegister) \
	DECLARE_SERIALIZER(UOWTEditContextReceiver)


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h_11_GENERATED_BODY \
	PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h_11_GENERATED_UINTERFACE_BODY() \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h_11_ENHANCED_CONSTRUCTORS \
private: \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h_11_INCLASS_IINTERFACE_NO_PURE_DECLS \
protected: \
	virtual ~IOWTEditContextReceiver() {} \
public: \
	typedef UOWTEditContextReceiver UClassType; \
	typedef IOWTEditContextReceiver ThisClass; \
	static bool Execute_ReceiveEditContext(UObject* O, FInstancedStruct const& Context); \
	virtual UObject* _getUObject() const { return nullptr; }


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h_8_PROLOG
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h_11_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h_11_CALLBACK_WRAPPERS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h_11_INCLASS_IINTERFACE_NO_PURE_DECLS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UOWTEditContextReceiver;

// ********** End Interface UOWTEditContextReceiver ************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Interfaces_OWTEditContextReceiver_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
