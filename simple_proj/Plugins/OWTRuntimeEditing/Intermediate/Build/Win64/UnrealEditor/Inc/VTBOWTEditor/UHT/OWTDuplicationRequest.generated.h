// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Duplication/OWTDuplicationRequest.h"

#ifdef VTBOWTEDITOR_OWTDuplicationRequest_generated_h
#error "OWTDuplicationRequest.generated.h already included, missing '#pragma once' in OWTDuplicationRequest.h"
#endif
#define VTBOWTEDITOR_OWTDuplicationRequest_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin ScriptStruct FOWTDuplicationOptions ********************************************
struct Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics;
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Duplication_OWTDuplicationRequest_h_25_GENERATED_BODY \
	friend struct ::Z_Construct_UScriptStruct_FOWTDuplicationOptions_Statics; \
	static class UScriptStruct* StaticStruct();


struct FOWTDuplicationOptions;
// ********** End ScriptStruct FOWTDuplicationOptions **********************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_VTBOWTEditor_Public_Duplication_OWTDuplicationRequest_h

// ********** Begin Enum EOWTDuplicationHierarchyScope *********************************************
#define FOREACH_ENUM_EOWTDUPLICATIONHIERARCHYSCOPE(op) \
	op(EOWTDuplicationHierarchyScope::AuthoredHierarchy) \
	op(EOWTDuplicationHierarchyScope::ActorAndManagedChildren) 

enum class EOWTDuplicationHierarchyScope : uint8;
template<> struct TIsUEnumClass<EOWTDuplicationHierarchyScope> { enum { Value = true }; };
template<> VTBOWTEDITOR_NON_ATTRIBUTED_API UEnum* StaticEnum<EOWTDuplicationHierarchyScope>();
// ********** End Enum EOWTDuplicationHierarchyScope ***********************************************

// ********** Begin Enum EOWTDuplicationGenerationPolicy *******************************************
#define FOREACH_ENUM_EOWTDUPLICATIONGENERATIONPOLICY(op) \
	op(EOWTDuplicationGenerationPolicy::RegenerateIfSourceGenerated) \
	op(EOWTDuplicationGenerationPolicy::KeepUnGenerated) 

enum class EOWTDuplicationGenerationPolicy : uint8;
template<> struct TIsUEnumClass<EOWTDuplicationGenerationPolicy> { enum { Value = true }; };
template<> VTBOWTEDITOR_NON_ATTRIBUTED_API UEnum* StaticEnum<EOWTDuplicationGenerationPolicy>();
// ********** End Enum EOWTDuplicationGenerationPolicy *********************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
