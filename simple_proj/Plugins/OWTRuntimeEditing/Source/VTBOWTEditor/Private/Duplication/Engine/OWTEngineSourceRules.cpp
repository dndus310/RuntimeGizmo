#include "Duplication/Engine/OWTEngineSourceRules.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Brush.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Info.h"
#include "GameFramework/Pawn.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

void FOWTEngineSourceRules::Register(FOWTDuplicationSourceRuleSet& Rules)
{
	Rules.Register(AInfo::StaticClass(), AdmitInfo);
	Rules.Register(AController::StaticClass(), AdmitController);
	Rules.Register(ABrush::StaticClass(), AdmitBrush);
	Rules.Register(APawn::StaticClass(), AdmitPawn);
	Rules.Register(UPhysicsConstraintComponent::StaticClass(), AdmitPhysicsConstraint);
	Rules.Register(UPrimitiveComponent::StaticClass(), AdmitPrimitive);
}

FOWTSourceDecision FOWTEngineSourceRules::AdmitInfo(const UObject& Object)
{
	return FOWTSourceDecision::Reject(TEXT("World management Actors require an application-specific duplication policy."));
}

FOWTSourceDecision FOWTEngineSourceRules::AdmitController(const UObject& Object)
{
	return FOWTSourceDecision::Reject(TEXT("Controller Actors require an application-specific duplication policy."));
}

FOWTSourceDecision FOWTEngineSourceRules::AdmitBrush(const UObject& Object)
{
	return FOWTSourceDecision::Reject(TEXT("BSP/volume Actors require an application-specific duplication policy."));
}

FOWTSourceDecision FOWTEngineSourceRules::AdmitPawn(const UObject& Object)
{
	// A duplicate must not compete with its source for the same controller or player.
	const APawn& Pawn = *CastChecked<APawn>(&Object);
	const bool bPossessed = Pawn.GetController() != nullptr;
	const bool bAutoPossessPlayer = Pawn.AutoPossessPlayer != EAutoReceiveInput::Disabled;
	const bool bAutoPossessAI = Pawn.AutoPossessAI != EAutoPossessAI::Disabled;

	FOWTSourceDecision Decision = FOWTSourceDecision::Defer();
	if (bPossessed)
	{
		Decision = FOWTSourceDecision::Reject(TEXT("Unpossess the Pawn before duplicating its configuration."));
	}
	else if (bAutoPossessPlayer)
	{
		Decision = FOWTSourceDecision::Reject(TEXT("Disable automatic player possession before duplicating the Pawn."));
	}
	else if (bAutoPossessAI)
	{
		Decision = FOWTSourceDecision::Reject(TEXT("Disable automatic AI possession before duplicating the Pawn."));
	}
	return Decision;
}

FOWTSourceDecision FOWTEngineSourceRules::AdmitPhysicsConstraint(const UObject& Object)
{
	return FOWTSourceDecision::Reject(
	    TEXT("PhysicsConstraintComponent needs an explicit adapter to rebuild native constraint handles."));
}

FOWTSourceDecision FOWTEngineSourceRules::AdmitPrimitive(const UObject& Object)
{
	// The core restores velocity and sleep state only for one standalone static mesh body.
	const UPrimitiveComponent& Primitive = *CastChecked<UPrimitiveComponent>(&Object);
	const bool bSimulating = Primitive.IsSimulatingPhysics();
	const bool bStaticMesh = Primitive.IsA<UStaticMeshComponent>();
	const bool bInstanced = Primitive.IsA<UInstancedStaticMeshComponent>();
	const bool bWelded = bSimulating && Primitive.IsWelded();

	FOWTSourceDecision Decision = FOWTSourceDecision::Defer();
	if (bSimulating && !bStaticMesh)
	{
		Decision = FOWTSourceDecision::Reject(
		    TEXT("Simulated components other than StaticMesh require a custom physics adapter."));
	}
	else if (bSimulating && bInstanced)
	{
		Decision = FOWTSourceDecision::Reject(TEXT("Simulated instanced meshes require a custom physics adapter."));
	}
	else if (bWelded)
	{
		Decision = FOWTSourceDecision::Reject(TEXT("Simulated welded bodies require a custom physics adapter."));
	}
	return Decision;
}
