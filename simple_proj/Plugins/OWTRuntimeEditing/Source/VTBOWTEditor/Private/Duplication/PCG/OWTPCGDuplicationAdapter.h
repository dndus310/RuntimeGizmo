#pragma once

#include "Duplication/OWTDuplicationAdapter.h"
#include "Duplication/PCG/OWTPCGConfigurationSnapshot.h"
#include "Duplication/PCG/OWTPCGGenerationTracker.h"
#include "Duplication/PCG/OWTPCGGraphCloner.h"
#include "Duplication/PCG/OWTPCGReferenceRemapper.h"

class UPCGComponent;

/**
 * Primary adapter for UPCGComponent. The duplicate receives the source's authored configuration and owned graph
 * instances; generated output is never copied. Generation stays suppressed until authored commit and is then
 * requested again according to the source trigger and FOWTDuplicationOptions::GenerationPolicy.
 */
class FOWTPCGDuplicationAdapter final : public IOWTDuplicationAdapter
{
public:
	FOWTPCGDuplicationAdapter();
	virtual ~FOWTPCGDuplicationAdapter() override;
	virtual bool Initialize(UWorld& World, FString& Error) override;
	virtual bool ValidateObject(const UObject& Object, FString& Error) const override;
	virtual bool CaptureObject(UObject& Object, FString& Error) override;
	virtual void VisitCapturedReferences(TFunctionRef<void(UObject*&, bool)> Visitor) override;
	virtual void PrepareDestination(UObject& Destination) override;
	virtual bool Restore(TMap<UObject*, UObject*>& Mapping, FString& Error) override;
	virtual bool ValidateReferences(const TMap<UObject*, UObject*>& Mapping, FString& Error) override;
	virtual void PrepareCommit(const FGuid& OperationId, const FOWTDuplicationOptions& Options, const TMap<UObject*, UObject*>& Mapping) override;
	virtual void Commit(const FGuid& OperationId, const FOWTDuplicationOptions& Options, const TMap<UObject*, UObject*>& Mapping) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Rollback(const TMap<UObject*, UObject*>& Mapping) override;
	virtual void Shutdown() override;
	virtual bool IsManagedObject(const UObject& Object) const override;
	virtual bool OwnsProperty(const UObject& Object, const FProperty& Property) const override;
	virtual bool OwnsObject(const UObject& Object) const override;
	virtual TArray<FOWTProceduralComponentSnapshot> GetProceduralComponents() const override;

private:
	static bool IsBusy(const UPCGComponent& Component);
	static bool IsBusyWithLocals(UPCGComponent& Component);
	/** Rejects destinations that a construction callback already started generating or cleaning. */
	static bool ValidateDestinationLifecycle(const UPCGComponent& Target, FString& Error);
	bool CaptureGraphInstance(UPCGComponent& Component, FOWTPCGConfigurationSnapshot& Captured, FString& Error);
	bool RestoreConfiguration(const FOWTPCGConfigurationSnapshot& Captured, UPCGComponent& Target, TMap<UObject*, UObject*>& Mapping, FString& Error);
	void RestoreSchedulingPolicy(const FOWTPCGConfigurationSnapshot& Captured, UPCGComponent& Target, TMap<UObject*, UObject*>& Mapping);
	bool RestoreGraphInstance(const FOWTPCGConfigurationSnapshot& Captured, UPCGComponent& Target, TMap<UObject*, UObject*>& Mapping, FString& Error);
	UPCGComponent* FindTarget(const FOWTPCGConfigurationSnapshot& Captured, const TMap<UObject*, UObject*>& Mapping) const;

private:
	/** Every PCG component in the world, used to recognize objects managed by any component's resources. */
	TArray<TWeakObjectPtr<UPCGComponent>> WorldComponents;
	TArray<TUniquePtr<FOWTPCGConfigurationSnapshot>> Configurations;
	FOWTPCGGraphCloner GraphCloner;
	FOWTPCGReferenceRemapper ReferenceRemapper;
	FOWTPCGGenerationTracker GenerationTracker;
};
