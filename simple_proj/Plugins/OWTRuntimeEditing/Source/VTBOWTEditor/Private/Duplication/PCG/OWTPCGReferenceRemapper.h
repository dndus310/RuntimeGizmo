#pragma once

#include "CoreMinimal.h"
#include "Duplication/Properties/OWTUnsupportedStructCache.h"

class FProperty;
class UPCGComponent;
class UPCGGraphInstance;
class UStruct;

/**
 * Walks the references a PCG configuration stores outside plain reflected component properties:
 * scheduling policy objects, owned graph instance parameter bags and interactive tool data.
 * References to managed PCG output are rejected because they would point into the source's generated result.
 */
class FOWTPCGReferenceRemapper
{
public:
	explicit FOWTPCGReferenceRemapper(TFunction<bool(const UObject&)> InIsManagedObject);
	/** Reports every reference so the core can capture owned instanced objects they reach. */
	void VisitCapturedReferences(UPCGComponent& Configuration, TFunctionRef<void(UObject*&, bool)> Visitor);
	bool Validate(UPCGComponent& Configuration, const TMap<UObject*, UObject*>& Mapping, FString& Error);
	bool Remap(UPCGComponent& Target, const TMap<UObject*, UObject*>& Mapping, FString& Error);

private:
	bool VisitConfiguration(UPCGComponent& Component, const TMap<UObject*, UObject*>& Mapping, bool bValidateOnly, FString& Error);
	bool VisitStruct(const UStruct& Struct, void* Memory, const TMap<UObject*, UObject*>& Mapping, bool bValidateOnly, FString& Error);
	bool VisitProperty(FProperty& Property, void* Value, const TMap<UObject*, UObject*>& Mapping, bool bValidateOnly, FString& Error);
	/** Graph instances owned by Component, from its default instance up the parent chain. */
	static TArray<UPCGGraphInstance*> GetOwnedGraphInstances(UPCGComponent& Component);
	static bool IsReflectedConfigurationProperty(const FProperty& Property);

private:
	TFunction<bool(const UObject&)> IsManagedObject;
	FOWTUnsupportedStructCache UnsupportedStructs;
};
