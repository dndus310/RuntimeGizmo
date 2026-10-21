#pragma once

#include "CoreMinimal.h"
#include "OWTDuplicationResult.generated.h"

class AActor;

/** Outcome of one duplication request. CreatedActors lists every Actor the request spawned, root first. */
USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTDuplicationResult
{
	GENERATED_BODY()

	FOWTDuplicationResult();

	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	TObjectPtr<AActor> Duplicate;

	/** Includes ChildActorComponent children and, for AuthoredHierarchy scope, attached Actors. */
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	TArray<TObjectPtr<AActor>> CreatedActors;

	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	FGuid OperationId;

	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	FString Error;

	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	bool bSucceeded;
};
