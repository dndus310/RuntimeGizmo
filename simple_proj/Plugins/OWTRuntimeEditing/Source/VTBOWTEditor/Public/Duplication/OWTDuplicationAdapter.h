#pragma once

#include "CoreMinimal.h"
#include "Duplication/OWTDuplicationRequest.h"
#include "Duplication/OWTDuplicationPropertyPolicy.h"
#include "State/OWTEditingSessionTypes.h"

class AActor;
class FProperty;

enum class EOWTDuplicationAdapterRole : uint8
{
	Primary,
	Auxiliary
};

/** One adapter instance belongs to one operation. It never mutates the source. */
class VTBOWTEDITOR_API IOWTDuplicationAdapter
{
public:
	IOWTDuplicationAdapter();
	virtual ~IOWTDuplicationAdapter() = default;
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
	virtual bool OwnsProperty(const UObject& Object, const FProperty& Property) const
	{
		return false;
	}
	virtual bool OwnsObject(const UObject& Object) const
	{
		return false;
	}
	virtual void VisitCapturedReferences(TFunctionRef<void(UObject*&, bool)> Visitor)
	{
	}
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

/** Class-keyed factories, held by the owning editing session rather than a global mutable singleton. */
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
};
