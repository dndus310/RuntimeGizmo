#pragma once

#include "PCGGraph.h"
#include "RuntimeGen/SchedulingPolicies/PCGSchedulingPolicyBase.h"
#include "OWTPCGTestGraph.generated.h"

class AActor;
class UPCGComponent;

UCLASS(Transient, EditInlineNew, DefaultToInstanced)
class UOWTPCGTestPolicyData : public UObject
{
	GENERATED_BODY()

public:
	UOWTPCGTestPolicyData() : Actor(nullptr), Component(nullptr)
	{
	}

	UPROPERTY()
	TObjectPtr<AActor> Actor;
	UPROPERTY()
	TObjectPtr<UPCGComponent> Component;
};

UCLASS(Transient)
class UOWTPCGTestPolicy : public UPCGSchedulingPolicyBase
{
	GENERATED_BODY()

public:
	UOWTPCGTestPolicy() : Actor(nullptr), Data(nullptr)
	{
	}

	virtual double CalculatePriority(const IPCGGenSourceBase*, const FBox&, bool) const override
	{
		return 1.0;
	}
	virtual bool IsEquivalent(const UPCGSchedulingPolicyBase* Other) const override
	{
		return Other == this;
	}

	UPROPERTY()
	TObjectPtr<AActor> Actor;
	UPROPERTY(Instanced)
	TObjectPtr<UOWTPCGTestPolicyData> Data;
};

UCLASS(Transient)
class UOWTPCGTestOpaquePolicy : public UOWTPCGTestPolicy
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FInstancedPropertyBag Opaque;
};

/** Native test fixture exposes typed graph configuration without reflection strings or engine edits. */
UCLASS(Transient)
class UOWTPCGTestGraph : public UPCGGraph
{
	GENERATED_BODY()

public:
	void ConfigureHierarchicalGeneration()
	{
		bUseHierarchicalGeneration = true;
		HiGenGridSize = EPCGHiGenGrid::Grid32;
	}
};
