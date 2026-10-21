#pragma once

#include "CoreMinimal.h"
#include "Duplication/Pipeline/OWTDuplicationContext.h"

class AActor;

/**
 * Collects and validates the authored source Actors before any destination exists.
 * ChildActorComponent children are always included; attached Actors only for AuthoredHierarchy scope.
 * Adapter-managed outputs are excluded and must not have authored Actors attached beneath them.
 */
class FOWTHierarchyCollector
{
public:
	FOWTHierarchyCollector(AActor& InSource, const FOWTDuplicationContext& InContext);
	bool Collect();
	/** Hands every collected Actor and authored component to the adapters that accept it. */
	bool CaptureAdapterSubjects();
	const TArray<AActor*>& GetActors() const;
	/** Actors that are spawned directly. ChildActorComponent children are recreated by their parent. */
	TArray<AActor*> GetRootActors() const;

private:
	bool CollectActor(AActor* Actor, bool bManagedChild);
	bool ValidateActor(AActor* Actor, bool bManagedChild) const;
	bool CollectChildActors(AActor& Actor);
	bool CollectAttachedActors(AActor& Actor);
	bool ValidateManagedDescendants(AActor& ManagedActor) const;
	TArray<UObject*> GetAdapterSubjects(AActor& Actor) const;

private:
	AActor& Source;
	const FOWTDuplicationContext& Context;
	TArray<AActor*> Actors;
	TSet<AActor*> Visited;
	TSet<AActor*> Visiting;
};
