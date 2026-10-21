#pragma once

#include "CoreMinimal.h"

class UPCGGraphInterface;

/**
 * Clones the GraphInstance parent chain an Actor owns so each duplicate owns its own instances.
 * External graph assets stay shared. Every cloned source object is remembered as adapter-owned.
 */
class FOWTPCGGraphCloner
{
public:
	FOWTPCGGraphCloner();
	/** Returns the graph to use inside DestinationOwner, or nullptr with Error set. */
	UPCGGraphInterface* Clone(UPCGGraphInterface* Graph, const UObject& SourceOwner, UObject& DestinationOwner, FString& Error);
	void RecordCopy(const UObject* Source, UObject* Copy);
	void AddOwnedSource(const UObject* Source);
	void Reset();
	bool IsOwnedSource(const UObject& Object) const;
	UObject* FindCopy(const UObject* Source) const;
	const TMap<const UObject*, UObject*>& GetCopies() const;

private:
	TSet<const UObject*> OwnedSources;
	TSet<const UObject*> CloningGraphs;
	TMap<const UObject*, UObject*> Copies;
};
