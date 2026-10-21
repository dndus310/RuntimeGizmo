#pragma once

#include "CoreMinimal.h"
#include "PCGCommon.h"
#include "State/OWTEditingSessionTypes.h"

class UPCGComponent;
struct FOWTDuplicationOptions;
struct FOWTPCGConfigurationSnapshot;

/**
 * Requests generation for committed PCG duplicates and reports their procedural state.
 * Partitioned components generate through partition-local components; their states are aggregated into one snapshot.
 */
class FOWTPCGGenerationTracker
{
public:
	explicit FOWTPCGGenerationTracker(FOWTProceduralChanged& InOnChanged);
	~FOWTPCGGenerationTracker();
	/** Binds observation before authored commit so activation can report its first change. */
	void Observe(UPCGComponent& Target, const FOWTPCGConfigurationSnapshot& Captured, const FGuid& OperationId, const FOWTDuplicationOptions& Options);
	/** Restores activation and requests generation according to each source trigger and the generation policy. */
	void RequestGenerations();
	void Tick();
	void Shutdown();
	TArray<FOWTProceduralComponentSnapshot> GetSnapshots() const;

private:
	struct FLocalObservation
	{
		FLocalObservation();

		FDelegateHandle Started;
		FDelegateHandle Generated;
		FDelegateHandle Cancelled;
		FDelegateHandle Cleaned;
		FPCGTaskId LastGenerationTask;
		bool bObservedCompletion;
		bool bCancelled;
	};

	struct FObservation
	{
		FObservation();

		FOWTProceduralComponentSnapshot Snapshot;
		FDelegateHandle Started;
		FDelegateHandle Generated;
		FDelegateHandle Cancelled;
		FDelegateHandle Cleaned;
		TMap<TWeakObjectPtr<UPCGComponent>, FLocalObservation> Locals;
		bool bActivated;
		bool bRequestGeneration;
		bool bOriginalCancelled;
	};

	static void ConfigureGeneration(FObservation& Observation, const UPCGComponent& Target, const FOWTPCGConfigurationSnapshot& Captured, const FOWTDuplicationOptions& Options);
	void RequestGeneration(int32 Index);
	void BindObservation(int32 Index, UPCGComponent& Target);
	void BindLocal(int32 Index, UPCGComponent& Component);
	static void UnbindLocal(UPCGComponent* Component, const FLocalObservation& Observation);
	void RefreshLocalObservations(int32 Index);
	void UpdateLocalAggregate(int32 Index);
	EOWTProceduralState CalculateAggregateState(const FObservation& Observation, UPCGComponent& Original) const;
	void ObserveLocalStart(int32 Index, UPCGComponent* Component);
	void ObserveLocalCompletion(int32 Index, UPCGComponent* Component);
	void SetState(int32 Index, EOWTProceduralState State, const FString& Reason = FString());
	FLocalObservation* FindObservedLocal(int32 Index, UPCGComponent* Component);

private:
	FOWTProceduralChanged& OnChanged;
	/** Delegates capture indices, so entries are only appended until Shutdown unbinds and clears them all. */
	TArray<TUniquePtr<FObservation>> Observations;
};
