#include "Duplication/OWTDuplicationSourceRules.h"

FOWTSourceDecision FOWTSourceDecision::Allow()
{
	return FOWTSourceDecision(EKind::Allow, FString());
}

FOWTSourceDecision FOWTSourceDecision::Reject(FString Reason)
{
	return FOWTSourceDecision(EKind::Reject, MoveTemp(Reason));
}

FOWTSourceDecision FOWTSourceDecision::Defer()
{
	return FOWTSourceDecision(EKind::Defer, FString());
}

bool FOWTSourceDecision::IsDeferred() const
{
	return Kind == EKind::Defer;
}

bool FOWTSourceDecision::IsRejected() const
{
	return Kind == EKind::Reject;
}

const FString& FOWTSourceDecision::GetReason() const
{
	return Reason;
}

FOWTSourceDecision::FOWTSourceDecision(EKind InKind, FString InReason) : Reason(MoveTemp(InReason)), Kind(InKind)
{
}

FOWTDuplicationSourceRuleSet::FRegistration::FRegistration(UClass* InClass, FRule InRule)
    : Class(InClass), Rule(MoveTemp(InRule))
{
}

bool FOWTDuplicationSourceRuleSet::Register(UClass* SupportedClass, FRule Rule)
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

void FOWTDuplicationSourceRuleSet::Unregister(UClass* SupportedClass)
{
	check(IsInGameThread());
	Registrations.RemoveAll(
	    [SupportedClass](const FRegistration& Registration)
	    {
		    return Registration.Class == SupportedClass;
	    });
}

bool FOWTDuplicationSourceRuleSet::Admit(const UObject& Object, FString& OutError) const
{
	check(IsInGameThread());
	FOWTSourceDecision Decision = FOWTSourceDecision::Defer();
	for (const UClass* Class = Object.GetClass(); Class && Decision.IsDeferred(); Class = Class->GetSuperClass())
	{
		for (const FRegistration& Registration : Registrations)
		{
			if (Decision.IsDeferred() && Registration.Class == Class)
			{
				Decision = Registration.Rule(Object);
			}
		}
	}

	const bool bAdmitted = !Decision.IsRejected();
	if (!bAdmitted)
	{
		OutError = Decision.GetReason();
	}
	return bAdmitted;
}
