#include "Duplication/Pipeline/OWTAdapterDispatcher.h"

#include "Duplication/OWTDuplicationAdapter.h"
#include "UObject/UnrealType.h"

FOWTAdapterDispatcher::FOWTAdapterDispatcher(TArray<TUniquePtr<IOWTDuplicationAdapter>> InAdapters)
    : Adapters(MoveTemp(InAdapters))
{
}

bool FOWTAdapterDispatcher::Initialize(UWorld& World, FString& Error)
{
	bool bInitialized = true;
	for (const TUniquePtr<IOWTDuplicationAdapter>& Adapter : Adapters)
	{
		bInitialized = bInitialized && Adapter->Initialize(World, Error);
	}
	return bInitialized;
}

bool FOWTAdapterDispatcher::ValidateSubject(const UObject& Object, FString& Error) const
{
	bool bValid = true;
	for (const TUniquePtr<IOWTDuplicationAdapter>& Adapter : Adapters)
	{
		bValid = bValid && (!Adapter->AcceptsObject(Object) || Adapter->ValidateObject(Object, Error));
	}
	return bValid;
}

bool FOWTAdapterDispatcher::CaptureSubject(UObject& Object, FString& Error)
{
	bool bCaptured = true;
	for (const TUniquePtr<IOWTDuplicationAdapter>& Adapter : Adapters)
	{
		bCaptured = bCaptured && (!Adapter->AcceptsObject(Object) || Adapter->CaptureSubject(Object, Error));
	}
	return bCaptured;
}

bool FOWTAdapterDispatcher::VisitCapturedReferences(TFunctionRef<bool(UObject*&, bool)> Visit)
{
	bool bVisited = true;
	for (const TUniquePtr<IOWTDuplicationAdapter>& Adapter : Adapters)
	{
		if (bVisited && Adapter->HasCapturedSubjects())
		{
			// Adapters cannot stop their own traversal, so later references are ignored after a failure.
			Adapter->VisitCapturedReferences([&bVisited, Visit](UObject*& Reference, bool bInstanced)
			{
				bVisited = bVisited && Visit(Reference, bInstanced);
			});
		}
	}
	return bVisited;
}

bool FOWTAdapterDispatcher::ValidateReferences(const TMap<UObject*, UObject*>& Mapping, FString& Error)
{
	bool bValid = true;
	for (const TUniquePtr<IOWTDuplicationAdapter>& Adapter : Adapters)
	{
		bValid = bValid && (!Adapter->HasCapturedSubjects() || Adapter->ValidateReferences(Mapping, Error));
	}
	return bValid;
}

void FOWTAdapterDispatcher::PrepareDestination(const UObject& Source, UObject& Destination)
{
	for (const TUniquePtr<IOWTDuplicationAdapter>& Adapter : Adapters)
	{
		if (Adapter->AcceptsObject(Destination) || Adapter->OwnsObject(Source))
		{
			Adapter->PrepareDestination(Destination);
		}
	}
}

bool FOWTAdapterDispatcher::Restore(TMap<UObject*, UObject*>& Mapping, FString& Error)
{
	bool bRestored = true;
	for (const TUniquePtr<IOWTDuplicationAdapter>& Adapter : Adapters)
	{
		bRestored = bRestored && (!Adapter->HasCapturedSubjects() || Adapter->Restore(Mapping, Error));
	}
	return bRestored;
}

void FOWTAdapterDispatcher::Rollback(const TMap<UObject*, UObject*>& Mapping)
{
	for (const TUniquePtr<IOWTDuplicationAdapter>& Adapter : Adapters)
	{
		if (Adapter->HasCapturedSubjects())
		{
			Adapter->Rollback(Mapping);
		}
	}
}

bool FOWTAdapterDispatcher::IsManagedObject(const UObject& Object) const
{
	return Adapters.ContainsByPredicate([&Object](const TUniquePtr<IOWTDuplicationAdapter>& Adapter)
	{
		return Adapter->IsManagedObject(Object);
	});
}

bool FOWTAdapterDispatcher::ResolvePropertyOwner(const UObject& Object, const FProperty& Property, bool& bOutOwned,
                                                 FString& Error) const
{
	bOutOwned = false;
	bool bResolved = true;
	for (const TUniquePtr<IOWTDuplicationAdapter>& Adapter : Adapters)
	{
		const bool bApplies = Adapter->AcceptsObject(Object) || Adapter->OwnsObject(Object);
		const bool bClaims = bResolved && bApplies && Adapter->OwnsProperty(Object, Property);
		if (bClaims && Adapter->IsAuxiliary())
		{
			Error = TEXT("AdapterOwnershipConflict: auxiliary policies cannot own properties.");
			bResolved = false;
		}
		else if (bClaims && bOutOwned)
		{
			Error = FString::Printf(TEXT("AdapterOwnershipConflict: multiple primary adapters claim %s."),
			                        *Property.GetPathName());
			bResolved = false;
		}
		else if (bClaims)
		{
			bOutOwned = true;
		}
	}
	return bResolved;
}

bool FOWTAdapterDispatcher::ResolveObjectOwner(const UObject& Object, bool& bOutOwned, FString& Error) const
{
	bOutOwned = false;
	bool bResolved = true;
	for (const TUniquePtr<IOWTDuplicationAdapter>& Adapter : Adapters)
	{
		const bool bClaims = bResolved && Adapter->OwnsObject(Object);
		if (bClaims && Adapter->IsAuxiliary())
		{
			Error = TEXT("AdapterOwnershipConflict: auxiliary policies cannot own objects.");
			bResolved = false;
		}
		else if (bClaims && bOutOwned)
		{
			Error = FString::Printf(TEXT("AdapterOwnershipConflict: multiple primary adapters claim %s."),
			                        *Object.GetPathName());
			bResolved = false;
		}
		else if (bClaims)
		{
			bOutOwned = true;
		}
	}
	return bResolved;
}

TArray<TUniquePtr<IOWTDuplicationAdapter>>& FOWTAdapterDispatcher::GetAdapters()
{
	return Adapters;
}
