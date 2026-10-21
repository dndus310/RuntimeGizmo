#include "Duplication/PCG/OWTPCGGraphCloner.h"

#include "PCGGraph.h"

FOWTPCGGraphCloner::FOWTPCGGraphCloner() : OwnedSources(), CloningGraphs(), Copies()
{
}

UPCGGraphInterface* FOWTPCGGraphCloner::Clone(UPCGGraphInterface* Graph, const UObject& SourceOwner,
                                              UObject& DestinationOwner, FString& Error)
{
	const bool bOwnedBySource = Graph && Graph->IsIn(&SourceOwner);
	UPCGGraphInstance* Instance = bOwnedBySource ? Cast<UPCGGraphInstance>(Graph) : nullptr;
	const bool bCycle = Instance && CloningGraphs.Contains(Graph);
	if (bOwnedBySource && !Instance)
	{
		Error = TEXT("Actor-owned PCG graph definitions require a graph construction adapter; use an external "
		             "graph asset or an owned GraphInstance.");
	}
	else if (bCycle)
	{
		Error = TEXT("The owned PCG graph instance parent chain contains a cycle.");
	}

	// External graph assets stay shared; owned instances are cloned from the root of the chain down.
	UPCGGraphInterface* Result = bOwnedBySource ? nullptr : Graph;
	if (Instance && !bCycle)
	{
		CloningGraphs.Add(Graph);
		UPCGGraphInterface* Parent = Clone(Instance->Graph, SourceOwner, DestinationOwner, Error);
		CloningGraphs.Remove(Graph);
		if (Error.IsEmpty())
		{
			UPCGGraphInstance* Copy = NewObject<UPCGGraphInstance>(&DestinationOwner);
			Copy->SetGraph(Parent);
			Copy->CopyParameterOverrides(Instance);
			RecordCopy(Graph, Copy);
			AddOwnedSource(Graph);
			Result = Copy;
		}
	}
	return Result;
}

void FOWTPCGGraphCloner::RecordCopy(const UObject* Source, UObject* Copy)
{
	Copies.Add(Source, Copy);
}

void FOWTPCGGraphCloner::AddOwnedSource(const UObject* Source)
{
	OwnedSources.Add(Source);
}

void FOWTPCGGraphCloner::Reset()
{
	OwnedSources.Reset();
	CloningGraphs.Reset();
	Copies.Reset();
}

bool FOWTPCGGraphCloner::IsOwnedSource(const UObject& Object) const
{
	return OwnedSources.Contains(&Object);
}

UObject* FOWTPCGGraphCloner::FindCopy(const UObject* Source) const
{
	return Copies.FindRef(Source);
}

const TMap<const UObject*, UObject*>& FOWTPCGGraphCloner::GetCopies() const
{
	return Copies;
}
