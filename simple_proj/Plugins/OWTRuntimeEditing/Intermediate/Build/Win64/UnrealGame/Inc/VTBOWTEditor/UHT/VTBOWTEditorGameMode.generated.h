// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "VTBOWTEditorGameMode.h"

#ifdef VTBOWTEDITOR_VTBOWTEditorGameMode_generated_h
#error "VTBOWTEditorGameMode.generated.h already included, missing '#pragma once' in VTBOWTEditorGameMode.h"
#endif
#define VTBOWTEDITOR_VTBOWTEditorGameMode_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AVTBAttributeEditor;

// ********** Begin Interface UVTBOWTEditorGameModeProvider ****************************************
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	virtual AVTBAttributeEditor* GetAttributeEditor_Implementation() { return NULL; }; \
	DECLARE_FUNCTION(execGetAttributeEditor);


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_16_CALLBACK_WRAPPERS
struct Z_Construct_UClass_UVTBOWTEditorGameModeProvider_Statics;
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTEditorGameModeProvider_NoRegister();

#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_16_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UVTBOWTEditorGameModeProvider(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UVTBOWTEditorGameModeProvider(UVTBOWTEditorGameModeProvider&&) = delete; \
	UVTBOWTEditorGameModeProvider(const UVTBOWTEditorGameModeProvider&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UVTBOWTEditorGameModeProvider); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UVTBOWTEditorGameModeProvider); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UVTBOWTEditorGameModeProvider) \
	virtual ~UVTBOWTEditorGameModeProvider() = default;


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_16_GENERATED_UINTERFACE_BODY() \
private: \
	static void StaticRegisterNativesUVTBOWTEditorGameModeProvider(); \
	friend struct ::Z_Construct_UClass_UVTBOWTEditorGameModeProvider_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend VTBOWTEDITOR_API UClass* ::Z_Construct_UClass_UVTBOWTEditorGameModeProvider_NoRegister(); \
public: \
	DECLARE_CLASS2(UVTBOWTEditorGameModeProvider, UInterface, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Interface), CASTCLASS_None, TEXT("/Script/VTBOWTEditor"), Z_Construct_UClass_UVTBOWTEditorGameModeProvider_NoRegister) \
	DECLARE_SERIALIZER(UVTBOWTEditorGameModeProvider)


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_16_GENERATED_BODY \
	PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_16_GENERATED_UINTERFACE_BODY() \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_16_ENHANCED_CONSTRUCTORS \
private: \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_16_INCLASS_IINTERFACE_NO_PURE_DECLS \
protected: \
	virtual ~IVTBOWTEditorGameModeProvider() {} \
public: \
	typedef UVTBOWTEditorGameModeProvider UClassType; \
	typedef IVTBOWTEditorGameModeProvider ThisClass; \
	static AVTBAttributeEditor* Execute_GetAttributeEditor(UObject* O); \
	virtual UObject* _getUObject() const { return nullptr; }


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_13_PROLOG
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_21_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_16_CALLBACK_WRAPPERS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_16_INCLASS_IINTERFACE_NO_PURE_DECLS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UVTBOWTEditorGameModeProvider;

// ********** End Interface UVTBOWTEditorGameModeProvider ******************************************

// ********** Begin Class AVTBOWTEditorGameMode ****************************************************
struct Z_Construct_UClass_AVTBOWTEditorGameMode_Statics;
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBOWTEditorGameMode_NoRegister();

#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_31_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAVTBOWTEditorGameMode(); \
	friend struct ::Z_Construct_UClass_AVTBOWTEditorGameMode_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend VTBOWTEDITOR_API UClass* ::Z_Construct_UClass_AVTBOWTEditorGameMode_NoRegister(); \
public: \
	DECLARE_CLASS2(AVTBOWTEditorGameMode, AGameMode, COMPILED_IN_FLAGS(0 | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/VTBOWTEditor"), Z_Construct_UClass_AVTBOWTEditorGameMode_NoRegister) \
	DECLARE_SERIALIZER(AVTBOWTEditorGameMode) \
	virtual UObject* _getUObject() const override { return const_cast<AVTBOWTEditorGameMode*>(this); }


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_31_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	AVTBOWTEditorGameMode(AVTBOWTEditorGameMode&&) = delete; \
	AVTBOWTEditorGameMode(const AVTBOWTEditorGameMode&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AVTBOWTEditorGameMode); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AVTBOWTEditorGameMode); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AVTBOWTEditorGameMode) \
	NO_API virtual ~AVTBOWTEditorGameMode();


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_28_PROLOG
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_31_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_31_INCLASS_NO_PURE_DECLS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h_31_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class AVTBOWTEditorGameMode;

// ********** End Class AVTBOWTEditorGameMode ******************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorGameMode_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
