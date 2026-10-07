#include "Duplication/OWTDuplicationPropertyPolicy.h"

#include "UObject/UnrealType.h"

FOWTDuplicationPropertyPolicySet::FRegistration::FRegistration(UClass* InClass, FRule InRule)
    : Class(InClass), Rule(MoveTemp(InRule))
{
}

bool FOWTDuplicationPropertyPolicySet::Register(UClass* SupportedClass, FRule Rule)
{
	check(IsInGameThread());
	if (!SupportedClass)
	{
		return false;
	}
	if (!Rule)
	{
		return false;
	}
	for (const FRegistration& Registration : Registrations)
	{
		if (Registration.Class == SupportedClass)
		{
			return false;
		}
	}
	Registrations.Emplace(SupportedClass, MoveTemp(Rule));
	return true;
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
	for (const UClass* Class = Object.GetClass(); Class; Class = Class->GetSuperClass())
	{
		for (const FRegistration& Registration : Registrations)
		{
			if (Registration.Class != Class)
			{
				continue;
			}
			const TOptional<bool> Result = Registration.Rule(Object, Property);
			if (Result.IsSet())
			{
				return Result;
			}
		}
	}
	return {};
}
