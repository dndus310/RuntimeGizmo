// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "VTBAttributeEditor.h"

#ifdef VTBOWTEDITOR_VTBAttributeEditor_generated_h
#error "VTBAttributeEditor.generated.h already included, missing '#pragma once' in VTBAttributeEditor.h"
#endif
#define VTBOWTEDITOR_VTBAttributeEditor_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UObject;
enum class EOWTTransformEditPhase : uint8;
enum class EOWTTransformField : uint8;
struct FGuid;
struct FOWTAttributeSnapshot;
struct FOWTDuplicationOperationSnapshot;
struct FOWTDuplicationOptions;
struct FOWTEventRecord;
struct FOWTModeSnapshot;
struct FOWTProceduralComponentSnapshot;
struct FOWTToolAvailability;
struct FSaveTransformContext;

// ********** Begin Class AVTBAttributeEditor ******************************************************
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
	virtual void Redo_Implementation(); \
	virtual void Undo_Implementation(); \
	DECLARE_FUNCTION(execRedo); \
	DECLARE_FUNCTION(execUndo); \
	DECLARE_FUNCTION(execSaveHistoryFromTransformContext); \
	DECLARE_FUNCTION(execSaveHistory); \
	DECLARE_FUNCTION(execParseSnapshotJson); \
	DECLARE_FUNCTION(execGetLatestEventSequence); \
	DECLARE_FUNCTION(execGetMonitorEntries); \
	DECLARE_FUNCTION(execGetSnapshot); \
	DECLARE_FUNCTION(execMarkSelectionBaseline); \
	DECLARE_FUNCTION(execGetProceduralComponents); \
	DECLARE_FUNCTION(execGetDuplicationOperations); \
	DECLARE_FUNCTION(execGetModeSnapshot); \
	DECLARE_FUNCTION(execGetAvailableTools); \
	DECLARE_FUNCTION(execCanCancelActiveTool); \
	DECLARE_FUNCTION(execCanAcceptActiveTool); \
	DECLARE_FUNCTION(execRequestEndTool); \
	DECLARE_FUNCTION(execRequestStartTool); \
	DECLARE_FUNCTION(execBeginDuplicateOperation); \
	DECLARE_FUNCTION(execRequestDuplicate); \
	DECLARE_FUNCTION(execRequestTransformField); \
	DECLARE_FUNCTION(execPublishRequest); \
	DECLARE_FUNCTION(execUnsubscribe); \
	DECLARE_FUNCTION(execSubscribeDynamic);


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h_21_CALLBACK_WRAPPERS
struct Z_Construct_UClass_AVTBAttributeEditor_Statics;
VTBOWTEDITOR_API UClass* Z_Construct_UClass_AVTBAttributeEditor_NoRegister();

#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h_21_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAVTBAttributeEditor(); \
	friend struct ::Z_Construct_UClass_AVTBAttributeEditor_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend VTBOWTEDITOR_API UClass* ::Z_Construct_UClass_AVTBAttributeEditor_NoRegister(); \
public: \
	DECLARE_CLASS2(AVTBAttributeEditor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/VTBOWTEditor"), Z_Construct_UClass_AVTBAttributeEditor_NoRegister) \
	DECLARE_SERIALIZER(AVTBAttributeEditor)


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h_21_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	AVTBAttributeEditor(AVTBAttributeEditor&&) = delete; \
	AVTBAttributeEditor(const AVTBAttributeEditor&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AVTBAttributeEditor); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AVTBAttributeEditor); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AVTBAttributeEditor) \
	NO_API virtual ~AVTBAttributeEditor();


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h_18_PROLOG
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h_21_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h_21_CALLBACK_WRAPPERS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h_21_INCLASS_NO_PURE_DECLS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h_21_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class AVTBAttributeEditor;

// ********** End Class AVTBAttributeEditor ********************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_VTBAttributeEditor_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
