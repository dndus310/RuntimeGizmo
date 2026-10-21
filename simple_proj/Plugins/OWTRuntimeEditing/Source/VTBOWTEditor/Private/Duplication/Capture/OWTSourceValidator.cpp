#include "Duplication/Capture/OWTSourceValidator.h"

#include "Duplication/OWTDuplicationSourceRules.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"

bool FOWTSourceValidator::ValidateInstance(const AActor* Source, const UWorld& World, FString& Error)
{
	const bool bValid = IsValid(Source);
	const bool bAlive = bValid && !Source->IsActorBeingDestroyed();
	const bool bWorldInstance = bAlive && !Source->HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject);
	const bool bInWorld = bWorldInstance && Source->GetWorld() == &World;
	const bool bFiniteTransform = bInWorld && !Source->GetActorTransform().ContainsNaN();

	if (!bValid)
	{
		Error = TEXT("The source Actor is no longer valid.");
	}
	else if (!bAlive)
	{
		Error = TEXT("The source Actor is being destroyed.");
	}
	else if (!bWorldInstance)
	{
		Error = TEXT("Only world Actor instances can be duplicated.");
	}
	else if (!bInWorld)
	{
		Error = TEXT("The source Actor belongs to another world.");
	}
	else if (!bFiniteTransform)
	{
		Error = TEXT("The source Actor transform must be finite.");
	}
	return bFiniteTransform;
}

bool FOWTSourceValidator::AdmitComponents(const AActor& Source, const FOWTDuplicationSourceRuleSet& Rules, FString& Error)
{
	bool bAdmitted = true;
	TInlineComponentArray<UActorComponent*> Components(&Source);
	for (const UActorComponent* Component : Components)
	{
		bAdmitted = bAdmitted && Rules.Admit(*Component, Error);
	}
	return bAdmitted;
}
bool FOWTSourceValidator::ValidateActor(const AActor* Source, const UWorld& World, bool bChildActor,
                                        const FOWTDuplicationSourceRuleSet& Rules, FString& Error)
{
	const bool bAdmittedActor = ValidateInstance(Source, World, Error) && Rules.Admit(*Source, Error);
	const bool bOwnedPlacement = bAdmittedActor && (!Source->IsChildActor() || bChildActor);
	if (bAdmittedActor && !bOwnedPlacement)
	{
		Error = TEXT("Duplicate the owning Actor to include a ChildActorComponent-managed Actor.");
	}
	return bOwnedPlacement && AdmitComponents(*Source, Rules, Error);
}

bool FOWTSourceValidator::ValidateMaterial(const UMaterialInstanceDynamic& Material, FString& Error)
{
	const bool bBaseClass = Material.GetClass() == UMaterialInstanceDynamic::StaticClass();
	const bool bNoCollectionOverrides = Material.ParameterCollectionParameterValues.IsEmpty();
	const bool bNoSceneTextureOverrides = Material.UserSceneTextureOverrides.IsEmpty();
	const bool bNoNaniteOverride = !Material.NaniteOverrideMaterial.GetOverrideMaterial();
	const bool bGlobalSparseVolumes = !Material.SparseVolumeTextureParameterValues.ContainsByPredicate(
	    [](const FSparseVolumeTextureParameterValue& Parameter)
	    {
		    return Parameter.ParameterInfo.Association != EMaterialParameterAssociation::GlobalParameter;
	    });

	if (!bBaseClass)
	{
		Error = TEXT("Custom MID subclasses require an application-specific material adapter.");
	}
	else if (!bNoCollectionOverrides)
	{
		Error = TEXT("MID parameter collection overrides require a custom material adapter.");
	}
	else if (!bNoSceneTextureOverrides)
	{
		Error = TEXT("MID user scene texture overrides require a custom material adapter.");
	}
	else if (!bNoNaniteOverride)
	{
		Error = TEXT("Explicit MID Nanite overrides require a custom material adapter.");
	}
	else if (!bGlobalSparseVolumes)
	{
		Error = TEXT("Layered sparse-volume MID parameters require a custom material adapter.");
	}
	return bBaseClass && bNoCollectionOverrides && bNoSceneTextureOverrides && bNoNaniteOverride &&
	       bGlobalSparseVolumes;
}
