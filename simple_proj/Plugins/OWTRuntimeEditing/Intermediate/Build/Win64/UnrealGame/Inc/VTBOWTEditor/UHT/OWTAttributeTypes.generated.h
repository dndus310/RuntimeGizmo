// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Events/OWTAttributeTypes.h"

#ifdef VTBOWTEDITOR_OWTAttributeTypes_generated_h
#error "OWTAttributeTypes.generated.h already included, missing '#pragma once' in OWTAttributeTypes.h"
#endif
#define VTBOWTEDITOR_OWTAttributeTypes_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin ScriptStruct FOWTAttributeSnapshot *********************************************
struct Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics;
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Events_OWTAttributeTypes_h_33_GENERATED_BODY \
	friend struct ::Z_Construct_UScriptStruct_FOWTAttributeSnapshot_Statics; \
	static class UScriptStruct* StaticStruct();


struct FOWTAttributeSnapshot;
// ********** End ScriptStruct FOWTAttributeSnapshot ***********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Events_OWTAttributeTypes_h

// ********** Begin Enum EOWTTransformField ********************************************************
#define FOREACH_ENUM_EOWTTRANSFORMFIELD(op) \
	op(EOWTTransformField::LocationX) \
	op(EOWTTransformField::LocationY) \
	op(EOWTTransformField::LocationZ) \
	op(EOWTTransformField::RotationRoll) \
	op(EOWTTransformField::RotationPitch) \
	op(EOWTTransformField::RotationYaw) \
	op(EOWTTransformField::ScaleX) \
	op(EOWTTransformField::ScaleY) \
	op(EOWTTransformField::ScaleZ) 

enum class EOWTTransformField : uint8;
template<> struct TIsUEnumClass<EOWTTransformField> { enum { Value = true }; };
template<> VTBOWTEDITOR_NON_ATTRIBUTED_API UEnum* StaticEnum<EOWTTransformField>();
// ********** End Enum EOWTTransformField **********************************************************

// ********** Begin Enum EOWTTransformEditPhase ****************************************************
#define FOREACH_ENUM_EOWTTRANSFORMEDITPHASE(op) \
	op(EOWTTransformEditPhase::Begin) \
	op(EOWTTransformEditPhase::Update) \
	op(EOWTTransformEditPhase::Commit) \
	op(EOWTTransformEditPhase::Cancel) 

enum class EOWTTransformEditPhase : uint8;
template<> struct TIsUEnumClass<EOWTTransformEditPhase> { enum { Value = true }; };
template<> VTBOWTEDITOR_NON_ATTRIBUTED_API UEnum* StaticEnum<EOWTTransformEditPhase>();
// ********** End Enum EOWTTransformEditPhase ******************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
