// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Modes/VTBOWTObjectEditMode.h"

#ifdef VTBOWTEDITOR_VTBOWTObjectEditMode_generated_h
#error "VTBOWTObjectEditMode.generated.h already included, missing '#pragma once' in VTBOWTObjectEditMode.h"
#endif
#define VTBOWTEDITOR_VTBOWTObjectEditMode_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UObject;
class UScriptStruct;
struct FOWTDuplicateSelectionContext;
struct FOWTHideSelectionGizmoContext;
struct FOWTRedoContext;
struct FOWTSelectObjectContext;
struct FOWTSetRotationContext;
struct FOWTSetScaleContext;
struct FOWTSetTranslationContext;
struct FOWTToggleCoordinateSystemContext;
struct FOWTToggleTransformSplineContext;
struct FOWTUndoContext;

// ********** Begin ScriptStruct FOWTContextHandlerBinding *****************************************
struct Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics;
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h_14_GENERATED_BODY \
	friend struct ::Z_Construct_UScriptStruct_FOWTContextHandlerBinding_Statics; \
	VTBOWTEDITOR_API static class UScriptStruct* StaticStruct();


struct FOWTContextHandlerBinding;
// ********** End ScriptStruct FOWTContextHandlerBinding *******************************************

// ********** Begin Class UVTBOWTObjectEditMode ****************************************************
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h_36_RPC_WRAPPERS_NO_PURE_DECLS \
	virtual void SetScale_Implementation(FOWTSetScaleContext const& Context); \
	virtual void SetRotation_Implementation(FOWTSetRotationContext const& Context); \
	virtual void SetTranslation_Implementation(FOWTSetTranslationContext const& Context); \
	virtual void HideSelectionGizmo_Implementation(FOWTHideSelectionGizmoContext const& Context); \
	virtual void DuplicateSelection_Implementation(FOWTDuplicateSelectionContext const& Context); \
	virtual void ToggleTransformSpline_Implementation(FOWTToggleTransformSplineContext const& Context); \
	virtual void ToggleCoordinateSystem_Implementation(FOWTToggleCoordinateSystemContext const& Context); \
	virtual void Redo_Implementation(FOWTRedoContext const& Context); \
	virtual void Undo_Implementation(FOWTUndoContext const& Context); \
	virtual void SelectObject_Implementation(FOWTSelectObjectContext const& Context); \
	virtual void InitializeContextBindings_Implementation(); \
	DECLARE_FUNCTION(execSetScale); \
	DECLARE_FUNCTION(execSetRotation); \
	DECLARE_FUNCTION(execSetTranslation); \
	DECLARE_FUNCTION(execUnbindContextHandler); \
	DECLARE_FUNCTION(execHideSelectionGizmo); \
	DECLARE_FUNCTION(execDuplicateSelection); \
	DECLARE_FUNCTION(execToggleTransformSpline); \
	DECLARE_FUNCTION(execToggleCoordinateSystem); \
	DECLARE_FUNCTION(execRedo); \
	DECLARE_FUNCTION(execUndo); \
	DECLARE_FUNCTION(execSelectObject); \
	DECLARE_FUNCTION(execBindContextHandler); \
	DECLARE_FUNCTION(execInitializeContextBindings);


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h_36_CALLBACK_WRAPPERS
struct Z_Construct_UClass_UVTBOWTObjectEditMode_Statics;
VTBOWTEDITOR_API UClass* Z_Construct_UClass_UVTBOWTObjectEditMode_NoRegister();

#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h_36_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUVTBOWTObjectEditMode(); \
	friend struct ::Z_Construct_UClass_UVTBOWTObjectEditMode_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend VTBOWTEDITOR_API UClass* ::Z_Construct_UClass_UVTBOWTObjectEditMode_NoRegister(); \
public: \
	DECLARE_CLASS2(UVTBOWTObjectEditMode, UOWTAttributeEditMode, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/VTBOWTEditor"), Z_Construct_UClass_UVTBOWTObjectEditMode_NoRegister) \
	DECLARE_SERIALIZER(UVTBOWTObjectEditMode)


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h_36_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	UVTBOWTObjectEditMode(UVTBOWTObjectEditMode&&) = delete; \
	UVTBOWTObjectEditMode(const UVTBOWTObjectEditMode&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UVTBOWTObjectEditMode); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UVTBOWTObjectEditMode); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UVTBOWTObjectEditMode) \
	NO_API virtual ~UVTBOWTObjectEditMode();


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h_33_PROLOG
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h_36_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h_36_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h_36_CALLBACK_WRAPPERS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h_36_INCLASS_NO_PURE_DECLS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h_36_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UVTBOWTObjectEditMode;

// ********** End Class UVTBOWTObjectEditMode ******************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Modes_VTBOWTObjectEditMode_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
