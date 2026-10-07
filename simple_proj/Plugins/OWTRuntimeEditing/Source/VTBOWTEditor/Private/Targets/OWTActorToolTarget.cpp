#include "Targets/OWTActorToolTarget.h"
#include "GameFramework/Actor.h"

bool UOWTActorToolTarget::IsValid() const
{
	AActor* Target = Actor.Get();
	if (!::IsValid(Target))
	{
		return false;
	}
	return !Target->IsActorBeingDestroyed();
}

AActor* UOWTActorToolTarget::GetActor() const
{
	return Actor.Get();
}
void UOWTActorToolTarget::Initialize(AActor& InActor)
{
	Actor = &InActor;
}

bool UOWTActorToolTargetFactory::CanBuildTarget(UObject* SourceObject,
                                                const FToolTargetTypeRequirements& Requirements) const
{
	const AActor* Actor = Cast<AActor>(SourceObject);
	if (!::IsValid(Actor))
	{
		return false;
	}
	if (Actor->IsActorBeingDestroyed())
	{
		return false;
	}
	return Requirements.AreSatisfiedBy(UOWTActorToolTarget::StaticClass());
}

UToolTarget* UOWTActorToolTargetFactory::BuildTarget(UObject* SourceObject,
                                                     const FToolTargetTypeRequirements& Requirements)
{
	if (!CanBuildTarget(SourceObject, Requirements))
	{
		return nullptr;
	}
	UOWTActorToolTarget* Target = NewObject<UOWTActorToolTarget>(this);
	Target->Initialize(*CastChecked<AActor>(SourceObject));
	return Target;
}
