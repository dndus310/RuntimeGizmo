#include "Duplication/OWTDuplicationPropertyPolicy.h"

#include "UObject/UnrealType.h"

FOWTDuplicationPropertyPolicySet::FRegistration::FRegistration(UClass* InClass, FRule InRule)
    : Class(InClass), Rule(MoveTemp(InRule))
{
}

bool FOWTDuplicationPropertyPolicySet::Register(UClass* SupportedClass, FRule Rule)
{
	check(IsInGameThread());
	const bool bAlreadyRegistered = Registrations.ContainsByPredicate(
	    [SupportedClass](const FRegistration& Registration)
	    {
		    return Registration.Class == SupportedClass;
	    });
	const bool bCanRegister = SupportedClass && Rule && !bAlreadyRegistered;

	if (bCanRegister)
	{
		Registrations.Emplace(SupportedClass, MoveTemp(Rule));
	}
	return bCanRegister;
}

void FOWTDuplicationPropertyPolicySet::Unregister(UClass* SupportedClass)
{
	check(IsInGameThread());
	Registrations.RemoveAll(
	    [SupportedClass](const FRegistration& Registration)
	    {
		    return Registration.Class == SupportedClass;
	    });
}

TOptional<bool> FOWTDuplicationPropertyPolicySet::Resolve(const UObject& Object, const FProperty& Property) const
{
	check(IsInGameThread());
	// Exact classes are visited before their bases, independent of provider load order.
	TOptional<bool> Decision;
	for (const UClass* Class = Object.GetClass(); Class && !Decision.IsSet(); Class = Class->GetSuperClass())
	{
		for (const FRegistration& Registration : Registrations)
		{
			if (!Decision.IsSet() && Registration.Class == Class)
			{
				Decision = Registration.Rule(Object, Property);
			}
		}
	}
	return Decision;
}
