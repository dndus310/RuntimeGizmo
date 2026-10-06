// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Duplication/OWTRuntimeDuplicationParticipant.h"

#ifdef OWTRUNTIMEDUPLICATION_OWTRuntimeDuplicationParticipant_generated_h
#error "OWTRuntimeDuplicationParticipant.generated.h already included, missing '#pragma once' in OWTRuntimeDuplicationParticipant.h"
#endif
#define OWTRUNTIMEDUPLICATION_OWTRuntimeDuplicationParticipant_generated_h

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UObject;

// ********** Begin Interface UOWTRuntimeDuplicationParticipant ************************************
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h_10_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execRestoreRuntimeDuplicateState);


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h_10_CALLBACK_WRAPPERS
struct Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_Statics;
OWTRUNTIMEDUPLICATION_API UClass* Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_NoRegister();

#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h_10_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UOWTRuntimeDuplicationParticipant(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	/** Deleted move- and copy-constructors, should never be used */ \
	UOWTRuntimeDuplicationParticipant(UOWTRuntimeDuplicationParticipant&&) = delete; \
	UOWTRuntimeDuplicationParticipant(const UOWTRuntimeDuplicationParticipant&) = delete; \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UOWTRuntimeDuplicationParticipant); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UOWTRuntimeDuplicationParticipant); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UOWTRuntimeDuplicationParticipant) \
	virtual ~UOWTRuntimeDuplicationParticipant() = default;


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h_10_GENERATED_UINTERFACE_BODY() \
private: \
	static void StaticRegisterNativesUOWTRuntimeDuplicationParticipant(); \
	friend struct ::Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_Statics; \
	static UClass* GetPrivateStaticClass(); \
	friend OWTRUNTIMEDUPLICATION_API UClass* ::Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_NoRegister(); \
public: \
	DECLARE_CLASS2(UOWTRuntimeDuplicationParticipant, UInterface, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Interface), CASTCLASS_None, TEXT("/Script/OWTRuntimeDuplication"), Z_Construct_UClass_UOWTRuntimeDuplicationParticipant_NoRegister) \
	DECLARE_SERIALIZER(UOWTRuntimeDuplicationParticipant)


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h_10_GENERATED_BODY \
	PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h_10_GENERATED_UINTERFACE_BODY() \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h_10_ENHANCED_CONSTRUCTORS \
private: \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h_10_INCLASS_IINTERFACE_NO_PURE_DECLS \
protected: \
	virtual ~IOWTRuntimeDuplicationParticipant() {} \
public: \
	typedef UOWTRuntimeDuplicationParticipant UClassType; \
	typedef IOWTRuntimeDuplicationParticipant ThisClass; \
	static bool Execute_RestoreRuntimeDuplicateState(UObject* O, UObject* SourceObject, TMap<UObject*,UObject*> const& DuplicatedObjects, FString& OutError); \
	virtual UObject* _getUObject() const { return nullptr; }


#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h_7_PROLOG
#define FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h_10_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h_10_CALLBACK_WRAPPERS \
	FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h_10_INCLASS_IINTERFACE_NO_PURE_DECLS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


class UOWTRuntimeDuplicationParticipant;

// ********** End Interface UOWTRuntimeDuplicationParticipant **************************************

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_jkyii_Desktop_New1006_RuntimeGizmo_Version_4_simple_proj_Plugins_OWTRuntimeEditing_Source_OWTRuntimeDuplication_Public_Duplication_OWTRuntimeDuplicationParticipant_h

PRAGMA_ENABLE_DEPRECATION_WARNINGS
