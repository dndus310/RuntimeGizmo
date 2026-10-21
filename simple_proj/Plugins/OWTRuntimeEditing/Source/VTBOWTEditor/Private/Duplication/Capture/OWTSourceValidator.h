#pragma once

#include "CoreMinimal.h"

class AActor;
class FOWTDuplicationSourceRuleSet;
class UMaterialInstanceDynamic;
class UWorld;

/** Decides whether a source Actor or owned material can be duplicated by the core. */
class FOWTSourceValidator
{
public:
	/**
	 * Core invariants every source must satisfy, followed by the registered admission rules for the Actor and each
	 * of its components. bChildActor permits Actors managed by a ChildActorComponent of a captured parent.
	 */
	static bool ValidateActor(const AActor* Source, const UWorld& World, bool bChildActor, const FOWTDuplicationSourceRuleSet& Rules, FString& Error);
	/** The core rebuilds dynamic materials through public MID APIs, which cover only these parameter kinds. */
	static bool ValidateMaterial(const UMaterialInstanceDynamic& Material, FString& Error);

private:
	static bool ValidateInstance(const AActor* Source, const UWorld& World, FString& Error);
	static bool AdmitComponents(const AActor& Source, const FOWTDuplicationSourceRuleSet& Rules, FString& Error);
};
