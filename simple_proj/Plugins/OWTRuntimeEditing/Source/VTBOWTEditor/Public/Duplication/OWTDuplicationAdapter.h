#pragma once

#include "CoreMinimal.h"
#include "Duplication/OWTDuplicationRequest.h"
#include "Duplication/OWTDuplicationPropertyPolicy.h"
#include "Duplication/OWTDuplicationSourceRules.h"
#include "State/OWTEditingSessionTypes.h"

class AActor;
class FProperty;

enum class EOWTDuplicationAdapterRole : uint8
{
	Primary,
	Auxiliary
};

/**
 * Class-specific duplication behavior for state the reflected core cannot copy on its own.
 * One adapter instance belongs to one operation and never mutates the source.
 *
 * Vocabulary used by the core:
 * - Authored: user-made configuration that is copied.
 * - Managed: output an adapter regenerates (for example PCG resources). It is never copied and must not be referenced.
 * - Owned: a property or object whose copy the adapter performs instead of the core. Only primaries may own.
 *
 * Callbacks arrive in this order: Initialize -> ValidateObject -> CaptureObject -> VisitCapturedReferences ->
 * ValidateReferences -> PrepareDestination -> Restore -> PrepareCommit -> Commit, or Rollback on failure.
 * Committed adapters that report procedural components keep receiving Tick until Shutdown.
 * Lighter extension points exist: FOWTDuplicationPropertyPolicySet for property inclusion,
 * FOWTDuplicationSourceRuleSet for source admission and IOWTRuntimeDuplicationParticipant for own classes.
 */
class VTBOWTEDITOR_API IOWTDuplicationAdapter
{
public:
	IOWTDuplicationAdapter();
	virtual ~IOWTDuplicationAdapter() = default;
	/** Validation and capture of authored subjects. Actor overloads are a convenience for Actor-registered adapters. */
	virtual bool ValidateObject(const UObject& Object, FString& Error) const;
	virtual bool CaptureObject(UObject& Object, FString& Error);
	virtual bool Initialize(UWorld& World, FString& Error)
	{
		return true;
	}
	virtual bool IsManagedObject(const UObject& Object) const
	{
		return false;
	}
	virtual bool ValidateActor(const AActor& Actor, FString& Error) const
	{
		return true;
	}
	virtual bool CaptureActor(AActor& Actor, FString& Error)
	{
		return true;
	}
	/** Ownership claims. A claimed property or object is skipped by the reflected core. */
	virtual bool OwnsProperty(const UObject& Object, const FProperty& Property) const
	{
		return false;
	}
	virtual bool OwnsObject(const UObject& Object) const
	{
		return false;
	}
	/** Reports references held by captured state so the core can capture owned instanced objects they reach. */
	virtual void VisitCapturedReferences(TFunctionRef<void(UObject*&, bool)> Visitor)
	{
	}
	/** Reconstruction. Mapping translates every captured source object to its duplicate. */
	virtual void PrepareDestination(UObject& Destination)
	{
	}
	virtual bool Restore(TMap<UObject*, UObject*>& Mapping, FString& Error)
	{
		return true;
	}
	virtual bool ValidateReferences(const TMap<UObject*, UObject*>& Mapping, FString& Error)
	{
		return true;
	}
	/** Transaction. PrepareCommit runs before OnAuthoredCommitted, Commit after it. Rollback replaces both on failure. */
	virtual void PrepareCommit(const FGuid& OperationId, const FOWTDuplicationOptions& Options,
	                           const TMap<UObject*, UObject*>& Mapping)
	{
	}
	virtual void Commit(const FGuid& OperationId, const FOWTDuplicationOptions& Options,
	                    const TMap<UObject*, UObject*>& Mapping)
	{
	}
	virtual void Rollback(const TMap<UObject*, UObject*>& Mapping)
	{
	}
	/** Observation after commit. The duplicator keeps an adapter alive while it reports procedural components. */
	virtual void Tick(float DeltaTime)
	{
	}
	virtual void Shutdown()
	{
	}
	virtual TArray<FOWTProceduralComponentSnapshot> GetProceduralComponents() const
	{
		return {};
	}
	/** Core dispatches one most-specific primary plus matching auxiliary policies for each authored object. */
	bool AcceptsObject(const UObject& Object) const;
	bool IsAuxiliary() const;
	bool CaptureSubject(UObject& Object, FString& Error);
	bool HasCapturedSubjects() const;
	FOWTProceduralChanged OnProceduralChanged;

private:
	friend class FOWTDuplicationAdapterRegistry;
	TWeakObjectPtr<UClass> RegisteredClass;
	TArray<TWeakObjectPtr<UClass>> MoreSpecificClasses;
	EOWTDuplicationAdapterRole Role;
	bool bCapturedSubjects;
};

/** Class-keyed factories and rules, held by the owning editing session rather than a global mutable singleton. */
class VTBOWTEDITOR_API FOWTDuplicationAdapterRegistry
{
public:
	using FFactory = TFunction<TUniquePtr<IOWTDuplicationAdapter>()>;
	bool Register(UClass* SupportedClass, FFactory Factory,
	              EOWTDuplicationAdapterRole Role = EOWTDuplicationAdapterRole::Primary);
	void Unregister(UClass* SupportedClass, EOWTDuplicationAdapterRole Role = EOWTDuplicationAdapterRole::Primary);
	TArray<TUniquePtr<IOWTDuplicationAdapter>> CreateAdapters() const;
	FOWTDuplicationPropertyPolicySet& GetPropertyPolicies();
	const FOWTDuplicationPropertyPolicySet& GetPropertyPolicies() const;
	FOWTDuplicationSourceRuleSet& GetSourceRules();
	const FOWTDuplicationSourceRuleSet& GetSourceRules() const;

private:
	struct FRegistration
	{
		FRegistration(UClass* InClass, FFactory InFactory, EOWTDuplicationAdapterRole InRole)
		    : Class(InClass), Factory(MoveTemp(InFactory)), Role(InRole)
		{
		}
		TWeakObjectPtr<UClass> Class;
		FFactory Factory;
		EOWTDuplicationAdapterRole Role;
	};
	TArray<FRegistration> Registrations;
	FOWTDuplicationPropertyPolicySet PropertyPolicies;
	FOWTDuplicationSourceRuleSet SourceRules;
};
