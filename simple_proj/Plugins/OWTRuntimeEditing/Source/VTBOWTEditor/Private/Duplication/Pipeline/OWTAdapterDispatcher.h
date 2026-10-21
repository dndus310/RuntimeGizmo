#pragma once

#include "CoreMinimal.h"

class FProperty;
class IOWTDuplicationAdapter;
class UWorld;

/**
 * The adapters created for one operation. Every dispatch rule (applicability, ownership conflicts,
 * captured-subject filtering) lives here so capture and restore code only asks questions.
 */
class FOWTAdapterDispatcher
{
public:
	explicit FOWTAdapterDispatcher(TArray<TUniquePtr<IOWTDuplicationAdapter>> InAdapters);
	bool Initialize(UWorld& World, FString& Error);
	/** Dispatches validation to the most-specific primary and every auxiliary that accepts Object. */
	bool ValidateSubject(const UObject& Object, FString& Error) const;
	bool CaptureSubject(UObject& Object, FString& Error);
	/** Visits references of adapters that captured subjects. Visit returns false to stop with an error. */
	bool VisitCapturedReferences(TFunctionRef<bool(UObject*&, bool)> Visit);
	bool ValidateReferences(const TMap<UObject*, UObject*>& Mapping, FString& Error);
	void PrepareDestination(const UObject& Source, UObject& Destination);
	bool Restore(TMap<UObject*, UObject*>& Mapping, FString& Error);
	void Rollback(const TMap<UObject*, UObject*>& Mapping);
	bool IsManagedObject(const UObject& Object) const;
	/** Sets bOutOwned when exactly one applicable primary claims Property. Auxiliary or duplicate claims fail. */
	bool ResolvePropertyOwner(const UObject& Object, const FProperty& Property, bool& bOutOwned, FString& Error) const;
	/** Sets bOutOwned when exactly one primary claims Object. Auxiliary or duplicate claims fail. */
	bool ResolveObjectOwner(const UObject& Object, bool& bOutOwned, FString& Error) const;
	TArray<TUniquePtr<IOWTDuplicationAdapter>>& GetAdapters();

private:
	TArray<TUniquePtr<IOWTDuplicationAdapter>> Adapters;
};
