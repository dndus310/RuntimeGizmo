#pragma once

#include "CoreMinimal.h"
#include "ToolTargets/ToolTarget.h"
#include "OWTActorToolTarget.generated.h"

UCLASS(Transient)
class VTBOWTEDITOR_API UOWTActorToolTarget : public UToolTarget
{
	GENERATED_BODY()

public:
	virtual bool IsValid() const override;
	AActor* GetActor() const;
	void Initialize(AActor& InActor);

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> Actor;
};

UCLASS(Transient)
class VTBOWTEDITOR_API UOWTActorToolTargetFactory : public UToolTargetFactory
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTarget(UObject* SourceObject, const FToolTargetTypeRequirements& Requirements) const override;
	virtual UToolTarget* BuildTarget(UObject* SourceObject, const FToolTargetTypeRequirements& Requirements) override;
};
