#pragma once

#include "CoreMinimal.h"

/** Decision returned by a source admission rule. Defer falls through to a less-specific class rule. */
class VTBOWTEDITOR_API FOWTSourceDecision
{
public:
	static FOWTSourceDecision Allow();
	static FOWTSourceDecision Reject(FString Reason);
	static FOWTSourceDecision Defer();
	bool IsDeferred() const;
	bool IsRejected() const;
	const FString& GetReason() const;

private:
	enum class EKind : uint8
	{
		Defer,
		Allow,
		Reject
	};

	FOWTSourceDecision(EKind InKind, FString InReason);

private:
	FString Reason;
	EKind Kind;
};

/**
 * Class-specific admission rules for source Actors and their components.
 * Built-in rejections (controllers, BSP, constraints, simulated bodies...) are registered by the engine provider,
 * so an application that restores such state through an adapter or participant can Allow its own subclass.
 */
class VTBOWTEDITOR_API FOWTDuplicationSourceRuleSet
{
public:
	using FRule = TFunction<FOWTSourceDecision(const UObject&)>;

	bool Register(UClass* SupportedClass, FRule Rule);
	void Unregister(UClass* SupportedClass);
	/** Exact classes are consulted before their bases. Returns false with the rule's reason on rejection. */
	bool Admit(const UObject& Object, FString& OutError) const;

private:
	struct FRegistration
	{
		FRegistration(UClass* InClass, FRule InRule);

		TWeakObjectPtr<UClass> Class;
		FRule Rule;
	};

	TArray<FRegistration> Registrations;
};
