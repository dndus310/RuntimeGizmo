// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "State/OWTEditingSessionTypes.h"

#ifdef VTBOWTEDITOR_OWTEditingSessionTypes_generated_h
#error "OWTEditingSessionTypes.generated.h already included, missing '#pragma once' in OWTEditingSessionTypes.h"
#endif
#define VTBOWTEDITOR_OWTEditingSessionTypes_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin ScriptStruct FOWTModeSnapshot **************************************************
struct Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics;
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTEditingSessionTypes_h_39_GENERATED_BODY \
	friend struct ::Z_Construct_UScriptStruct_FOWTModeSnapshot_Statics; \
	static class UScriptStruct* StaticStruct();


struct FOWTModeSnapshot;
// ********** End ScriptStruct FOWTModeSnapshot ****************************************************

// ********** Begin ScriptStruct FOWTDuplicationOperationSnapshot **********************************
struct Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics;
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTEditingSessionTypes_h_66_GENERATED_BODY \
	friend struct ::Z_Construct_UScriptStruct_FOWTDuplicationOperationSnapshot_Statics; \
	static class UScriptStruct* StaticStruct();


struct FOWTDuplicationOperationSnapshot;
// ********** End ScriptStruct FOWTDuplicationOperationSnapshot ************************************

// ********** Begin ScriptStruct FOWTProceduralComponentSnapshot ***********************************
struct Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics;
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTEditingSessionTypes_h_101_GENERATED_BODY \
	friend struct ::Z_Construct_UScriptStruct_FOWTProceduralComponentSnapshot_Statics; \
	static class UScriptStruct* StaticStruct();


struct FOWTProceduralComponentSnapshot;
// ********** End ScriptStruct FOWTProceduralComponentSnapshot *************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_State_OWTEditingSessionTypes_h

// ********** Begin Enum EOWTDuplicationPhase ******************************************************
#define FOREACH_ENUM_EOWTDUPLICATIONPHASE(op) \
	op(EOWTDuplicationPhase::Accepted) \
	op(EOWTDuplicationPhase::Planning) \
	op(EOWTDuplicationPhase::Restoring) \
	op(EOWTDuplicationPhase::Committed) \
	op(EOWTDuplicationPhase::Failed) \
	op(EOWTDuplicationPhase::Cancelled) \
	op(EOWTDuplicationPhase::CleaningUp) 

enum class EOWTDuplicationPhase : uint8;
template<> struct TIsUEnumClass<EOWTDuplicationPhase> { enum { Value = true }; };
template<> VTBOWTEDITOR_NON_ATTRIBUTED_API UEnum* StaticEnum<EOWTDuplicationPhase>();
// ********** End Enum EOWTDuplicationPhase ********************************************************

// ********** Begin Enum EOWTProceduralState *******************************************************
#define FOREACH_ENUM_EOWTPROCEDURALSTATE(op) \
	op(EOWTProceduralState::NotRequested) \
	op(EOWTProceduralState::Scheduled) \
	op(EOWTProceduralState::WaitingForGenerationSource) \
	op(EOWTProceduralState::Generating) \
	op(EOWTProceduralState::Ready) \
	op(EOWTProceduralState::Failed) \
	op(EOWTProceduralState::Cancelled) \
	op(EOWTProceduralState::Cleaned) \
	op(EOWTProceduralState::CleaningUp) 

enum class EOWTProceduralState : uint8;
template<> struct TIsUEnumClass<EOWTProceduralState> { enum { Value = true }; };
template<> VTBOWTEDITOR_NON_ATTRIBUTED_API UEnum* StaticEnum<EOWTProceduralState>();
// ********** End Enum EOWTProceduralState *********************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
