#include "Duplication/OWTDuplicationAdapter.h"

#include "GameFramework/Actor.h"

IOWTDuplicationAdapter::IOWTDuplicationAdapter() : Role(EOWTDuplicationAdapterRole::Primary), bCapturedSubjects(false)
{
}

bool IOWTDuplicationAdapter::ValidateObject(const UObject& Object, FString& Error) const
{
	const AActor* Actor = Cast<AActor>(&Object);
	return !Actor || ValidateActor(*Actor, Error);
}

bool IOWTDuplicationAdapter::CaptureObject(UObject& Object, FString& Error)
{
	AActor* Actor = Cast<AActor>(&Object);
	return !Actor || CaptureActor(*Actor, Error);
}

bool IOWTDuplicationAdapter::AcceptsObject(const UObject& Object) const
{
	const UClass* Class = RegisteredClass.Get();
	const bool bMatchesRegisteredClass = Class && Object.IsA(Class);
	// A more specific primary registration shadows this one for its subclasses.
	const bool bShadowed = bMatchesRegisteredClass && MoreSpecificClasses.ContainsByPredicate(
	    [&Object](const TWeakObjectPtr<UClass>& MoreSpecific)
	    {
		    const UClass* Subclass = MoreSpecific.Get();
		    return Subclass && Object.IsA(Subclass);
	    });
	return bMatchesRegisteredClass && !bShadowed;
}

bool IOWTDuplicationAdapter::IsAuxiliary() const
{
	return Role == EOWTDuplicationAdapterRole::Auxiliary;
}

bool IOWTDuplicationAdapter::CaptureSubject(UObject& Object, FString& Error)
{
	check(AcceptsObject(Object));
	bCapturedSubjects = true;
	return CaptureObject(Object, Error);
}

bool IOWTDuplicationAdapter::HasCapturedSubjects() const
{
	return bCapturedSubjects;
}

bool FOWTDuplicationAdapterRegistry::Register(UClass* SupportedClass, FFactory Factory, EOWTDuplicationAdapterRole Role)
{
	// One primary and one auxiliary may share a class; a second registration of the same role is rejected.
	const bool bAlreadyRegistered = Registrations.ContainsByPredicate(
	    [SupportedClass, Role](const FRegistration& Entry)
	    {
		    return Entry.Class == SupportedClass && Entry.Role == Role;
	    });
	const bool bCanRegister = SupportedClass && Factory && !bAlreadyRegistered;

	if (bCanRegister)
	{
		Registrations.Emplace(SupportedClass, MoveTemp(Factory), Role);
	}
	return bCanRegister;
}

void FOWTDuplicationAdapterRegistry::Unregister(UClass* SupportedClass, EOWTDuplicationAdapterRole Role)
{
	Registrations.RemoveAll(
	    [SupportedClass, Role](const FRegistration& Entry)
	    {
		    return Entry.Class == SupportedClass && Entry.Role == Role;
	    });
}

TArray<TUniquePtr<IOWTDuplicationAdapter>> FOWTDuplicationAdapterRegistry::CreateAdapters() const
{
	TArray<TUniquePtr<IOWTDuplicationAdapter>> Result;
	for (const FRegistration& Entry : Registrations)
	{
		UClass* Class = Entry.Class.Get();
		if (!Class)
		{
			continue;
		}
		TUniquePtr<IOWTDuplicationAdapter> Adapter = Entry.Factory();
		checkf(Adapter, TEXT("A registered duplication adapter factory must return an instance."));
		Adapter->RegisteredClass = Class;
		Adapter->Role = Entry.Role;
		for (const FRegistration& Candidate : Registrations)
		{
			UClass* Subclass = Candidate.Class.Get();
			const bool bBothPrimary = Entry.Role == EOWTDuplicationAdapterRole::Primary &&
			                          Candidate.Role == EOWTDuplicationAdapterRole::Primary;
			const bool bMoreSpecific = Subclass && Subclass != Class && Subclass->IsChildOf(Class);
			if (bBothPrimary && bMoreSpecific)
			{
				Adapter->MoreSpecificClasses.Add(Subclass);
			}
		}
		Result.Add(MoveTemp(Adapter));
	}
	return Result;
}

FOWTDuplicationPropertyPolicySet& FOWTDuplicationAdapterRegistry::GetPropertyPolicies()
{
	return PropertyPolicies;
}

const FOWTDuplicationPropertyPolicySet& FOWTDuplicationAdapterRegistry::GetPropertyPolicies() const
{
	return PropertyPolicies;
}

FOWTDuplicationSourceRuleSet& FOWTDuplicationAdapterRegistry::GetSourceRules()
{
	return SourceRules;
}

const FOWTDuplicationSourceRuleSet& FOWTDuplicationAdapterRegistry::GetSourceRules() const
{
	return SourceRules;
}
