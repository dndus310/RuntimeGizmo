#pragma once

#include "CoreMinimal.h"

class FProperty;

/** Class-specific authoring rules. An unset result falls through to a less-specific rule. */
class VTBOWTEDITOR_API FOWTDuplicationPropertyPolicySet
{
public:
	using FRule = TFunction<TOptional<bool>(const UObject&, const FProperty&)>;

	bool Register(UClass* SupportedClass, FRule Rule);

	void Unregister(UClass* SupportedClass);

	TOptional<bool> Resolve(const UObject& Object, const FProperty& Property) const;

private:
	struct FRegistration
	{
		FRegistration(UClass* InClass, FRule InRule);

		TWeakObjectPtr<UClass> Class;
		FRule Rule;
	};

	TArray<FRegistration> Registrations;
};
