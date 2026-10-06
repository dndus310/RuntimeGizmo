// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "VTBOWTEditorSubsystem.h"

#ifdef VTBOWTEDITOR_VTBOWTEditorSubsystem_generated_h
#error "VTBOWTEditorSubsystem.generated.h already included, missing '#pragma once' in VTBOWTEditorSubsystem.h"
#endif
#define VTBOWTEDITOR_VTBOWTEditorSubsystem_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AVTBAttributeEditor;
class UObject;
struct FOWTGizmoSnapSettings;

// ********** Begin Class UVTBOWTEditorSubsystem ***************************************************
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorSubsystem_h_24_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetAttributeEditor); \
	DECLARE_FUNCTION(execGetGizmoSnapSettings); \
	DECLARE_FUNCTION(execSetGizmoSnapSettings); \
	DECLARE_FUNCTION(execSetActiveEditMode);


struct Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics;
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTEditorSubsystem_NoRegister();

#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorSubsystem_h_24_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUVTBOWTEditorSubsystem(); \
	friend struct ::Z_Construct_UClass_UVTBOWTEditorSubsystem_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend VTBOWTEDITOR_API UClass* ::Z_Construct_UClass_UVTBOWTEditorSubsystem_NoRegister(); \
public: \
	DECLARE_CLASS2(UVTBOWTEditorSubsystem, UTickableWorldSubsystem, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/VTBOWTEditor"), Z_Construct_UClass_UVTBOWTEditorSubsystem_NoRegister) \
	DECLARE_SERIALIZER(UVTBOWTEditorSubsystem) \
	virtual UObject* _getUObject() const override { return const_cast<UVTBOWTEditorSubsystem*>(this); }


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorSubsystem_h_24_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	UVTBOWTEditorSubsystem(UVTBOWTEditorSubsystem&&) = delete; \
	UVTBOWTEditorSubsystem(const UVTBOWTEditorSubsystem&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UVTBOWTEditorSubsystem); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UVTBOWTEditorSubsystem); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UVTBOWTEditorSubsystem) \
	NO_API virtual ~UVTBOWTEditorSubsystem();


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorSubsystem_h_21_PROLOG
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorSubsystem_h_24_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorSubsystem_h_24_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorSubsystem_h_24_INCLASS_NO_PURE_DECLS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorSubsystem_h_24_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UVTBOWTEditorSubsystem;

// ********** End Class UVTBOWTEditorSubsystem *****************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBOWTEditorSubsystem_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
