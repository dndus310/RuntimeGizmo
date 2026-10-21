#pragma once

#include "CoreMinimal.h"
#include "Duplication/OWTDuplicationSourceRules.h"

/** Rejects engine classes whose native state the core cannot rebuild. Applications may Allow a subclass. */
class FOWTEngineSourceRules
{
public:
	static void Register(FOWTDuplicationSourceRuleSet& Rules);

private:
	static FOWTSourceDecision AdmitInfo(const UObject& Object);
	static FOWTSourceDecision AdmitController(const UObject& Object);
	static FOWTSourceDecision AdmitBrush(const UObject& Object);
	static FOWTSourceDecision AdmitPawn(const UObject& Object);
	static FOWTSourceDecision AdmitPhysicsConstraint(const UObject& Object);
	static FOWTSourceDecision AdmitPrimitive(const UObject& Object);
};
