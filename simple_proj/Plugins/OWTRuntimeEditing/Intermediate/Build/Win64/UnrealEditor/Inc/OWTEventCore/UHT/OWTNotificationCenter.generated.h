// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Events/OWTNotificationCenter.h"

#ifdef OWTEVENTCORE_OWTNotificationCenter_generated_h
#error "OWTNotificationCenter.generated.h already included, missing '#pragma once' in OWTNotificationCenter.h"
#endif
#define OWTEVENTCORE_OWTNotificationCenter_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UObject;
enum class EOWTEventDirection : uint8;
struct FGuid;
struct FOWTEventRecord;

// ********** Begin Class UOWTNotificationCenter ***************************************************
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTNotificationCenter_h_29_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetHistoryCapacity); \
	DECLARE_FUNCTION(execGetHistoryRevision); \
	DECLARE_FUNCTION(execGetLatestSequence); \
	DECLARE_FUNCTION(execGetRecentEvents); \
	DECLARE_FUNCTION(execIsReady); \
	DECLARE_FUNCTION(execSetHistoryCapacity); \
	DECLARE_FUNCTION(execClearEventHistory); \
	DECLARE_FUNCTION(execUnsubscribe); \
	DECLARE_FUNCTION(execShutdown); \
	DECLARE_FUNCTION(execRecordEvent); \
	DECLARE_FUNCTION(execPublish); \
	DECLARE_FUNCTION(execSubscribeDynamic); \
	DECLARE_FUNCTION(execInitialize);


struct Z_Construct_UClass_UOWTNotificationCenter_Statics;
OWTEVENTCORE_API UClass* Z_Construct_UClass_UOWTNotificationCenter_NoRegister();

#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTNotificationCenter_h_29_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUOWTNotificationCenter(); \
	friend struct ::Z_Construct_UClass_UOWTNotificationCenter_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend OWTEVENTCORE_API UClass* ::Z_Construct_UClass_UOWTNotificationCenter_NoRegister(); \
public: \
	DECLARE_CLASS2(UOWTNotificationCenter, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/OWTEventCore"), Z_Construct_UClass_UOWTNotificationCenter_NoRegister) \
	DECLARE_SERIALIZER(UOWTNotificationCenter)


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTNotificationCenter_h_29_ENHANCED_CONSTRUCTORS \
	/** Deleted move- and copy-constructors, should never be used */ \
	UOWTNotificationCenter(UOWTNotificationCenter&&) = delete; \
	UOWTNotificationCenter(const UOWTNotificationCenter&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UOWTNotificationCenter); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UOWTNotificationCenter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UOWTNotificationCenter) \
	NO_API virtual ~UOWTNotificationCenter();


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTNotificationCenter_h_26_PROLOG
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTNotificationCenter_h_29_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTNotificationCenter_h_29_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTNotificationCenter_h_29_INCLASS_NO_PURE_DECLS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTNotificationCenter_h_29_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UOWTNotificationCenter;

// ********** End Class UOWTNotificationCenter *****************************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTEventCore_Public_Events_OWTNotificationCenter_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
