// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Events/OWTEventTypes.h"

#ifdef OWTEVENTCORE_OWTEventTypes_generated_h
#error "OWTEventTypes.generated.h already included, missing '#pragma once' in OWTEventTypes.h"
#endif
#define OWTEVENTCORE_OWTEventTypes_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS

// ********** Begin Delegate FOWTAttributeEventDynamic *********************************************
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTEventTypes_h_8_DELEGATE \
OWTEVENTCORE_API void FOWTAttributeEventDynamic_DelegateWrapper(const FScriptDelegate& OWTAttributeEventDynamic, FName Event, const FString& Json);


// ********** End Delegate FOWTAttributeEventDynamic ***********************************************

// ********** Begin ScriptStruct FOWTEventRecord ***************************************************
struct Z_Construct_UScriptStruct_FOWTEventRecord_Statics;
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTEventTypes_h_21_GENERATED_BODY \
	friend struct ::Z_Construct_UScriptStruct_FOWTEventRecord_Statics; \
	static class UScriptStruct* StaticStruct();


struct FOWTEventRecord;
// ********** End ScriptStruct FOWTEventRecord *****************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTEventTypes_h

// ********** Begin Enum EOWTEventDirection ********************************************************
#define FOREACH_ENUM_EOWTEVENTDIRECTION(op) \
	op(EOWTEventDirection::Inbound) \
	op(EOWTEventDirection::Outbound) 

enum class EOWTEventDirection : uint8;
template<> struct TIsUEnumClass<EOWTEventDirection> { enum { Value = true }; };
template<> OWTEVENTCORE_NON_ATTRIBUTED_API UEnum* StaticEnum<EOWTEventDirection>();
// ********** End Enum EOWTEventDirection **********************************************************

PRAGMA_ENABLE_DEPRECATION_WARNINGS
