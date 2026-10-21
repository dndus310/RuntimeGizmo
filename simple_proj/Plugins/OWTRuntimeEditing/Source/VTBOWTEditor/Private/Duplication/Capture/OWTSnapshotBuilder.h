#pragma once

#include "CoreMinimal.h"
#include "Duplication/Pipeline/OWTDuplicationContext.h"

class AActor;
class UActorComponent;
struct FOWTRootSnapshot;

/**
 * Fills a root snapshot from an already validated hierarchy: the root, its ChildActorComponent descendants,
 * their components and every owned instanced object reachable from captured reflected state.
 */
class FOWTSnapshotBuilder
{
public:
	FOWTSnapshotBuilder(const FOWTDuplicationContext& InContext, FOWTRootSnapshot& InSnapshot);
	bool Capture();

private:
	bool CaptureActor(AActor& Actor);
	bool CaptureComponent(UActorComponent& Component);
	bool CaptureAdapterReferences();
	bool CaptureReachableObjects();
	bool ValidateOwnedObjects() const;
	/** Captures Reference when the hierarchy owns it. References to external objects stay shared. */
	bool CaptureReference(UObject* Reference, bool bInstanced);
	bool CaptureOwnedObject(UObject& Object);
	bool IsInCapturedActor(const UObject& Object) const;

private:
	const FOWTDuplicationContext& Context;
	FOWTRootSnapshot& Snapshot;
	/** Objects reached through instanced references; they are owned even without DefaultToInstanced. */
	TSet<UObject*> ExplicitInstancedObjects;
};
