#include "Duplication/OWTDuplicationAdapter.h"

#include "GameFramework/Actor.h"

IOWTDuplicationAdapter::IOWTDuplicationAdapter() : Role(EOWTDuplicationAdapterRole::Primary), bCapturedSubjects(false)
{
}

bool IOWTDuplicationAdapter::ValidateObject(const UObject& Object, FString& Error) const
{
	if (const AActor* Actor = Cast<AActor>(&Object))
	{
		return ValidateActor(*Actor, Error);
	}
	return true;
}

bool IOWTDuplicationAdapter::CaptureObject(UObject& Object, FString& Error)
{
	if (AActor* Actor = Cast<AActor>(&Object))
	{
		return CaptureActor(*Actor, Error);
	}
	return true;
}

bool IOWTDuplicationAdapter::AcceptsObject(const UObject& Object) const
{
	const UClass* Class = RegisteredClass.Get();
	if (!Class)
	{
		return false;
	}
	if (!Object.IsA(Class))
	{
		return false;
	}
	for (const TWeakObjectPtr<UClass>& MoreSpecific : MoreSpecificClasses)
	{
		if (const UClass* Subclass = MoreSpecific.Get())
		{
			if (Object.IsA(Subclass))
			{
				return false;
			}
		}
	}
	return true;
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
	if (!SupportedClass)
	{
		return false;
	}
	if (!Factory)
	{
		return false;
	}
	for (const FRegistration& Entry : Registrations)
	{
		if (Entry.Class == SupportedClass)
		{
			if (Entry.Role == Role)
			{
				return false;
			}
		}
	}
	Registrations.Emplace(SupportedClass, MoveTemp(Factory), Role);
	return true;
}

void FOWTDuplicationAdapterRegistry::Unregister(UClass* SupportedClass, EOWTDuplicationAdapterRole Role)
{
	Registrations.RemoveAll(
	    [SupportedClass, Role](const FRegistration& Entry)
	    {
		    if (Entry.Class != SupportedClass)
		    {
			    return false;
		    }
		    return Entry.Role == Role;
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
		if (Entry.Role == EOWTDuplicationAdapterRole::Primary)
		{
			for (const FRegistration& Candidate : Registrations)
			{
				if (Candidate.Role != EOWTDuplicationAdapterRole::Primary)
				{
					continue;
				}
				UClass* Subclass = Candidate.Class.Get();
				if (!Subclass)
				{
					continue;
				}
				if (Subclass == Class)
				{
					continue;
				}
				if (Subclass->IsChildOf(Class))
				{
					Adapter->MoreSpecificClasses.Add(Subclass);
				}
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
