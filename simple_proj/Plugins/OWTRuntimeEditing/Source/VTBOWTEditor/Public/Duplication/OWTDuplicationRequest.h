#pragma once

#include "CoreMinimal.h"
#include "OWTDuplicationRequest.generated.h"

class AActor;

UENUM(BlueprintType)
enum class EOWTDuplicationHierarchyScope : uint8
{
	AuthoredHierarchy,
	ActorAndManagedChildren
};

UENUM(BlueprintType)
enum class EOWTDuplicationGenerationPolicy : uint8
{
	RegenerateIfSourceGenerated,
	KeepUnGenerated
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTDuplicationOptions
{
	GENERATED_BODY()

	FOWTDuplicationOptions()
	    : WorldOffset(FVector::ZeroVector), HierarchyScope(EOWTDuplicationHierarchyScope::AuthoredHierarchy),
	      GenerationPolicy(EOWTDuplicationGenerationPolicy::RegenerateIfSourceGenerated), IsCancellationRequested(),
	      OnAuthoredCommitted()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WorldOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EOWTDuplicationHierarchyScope HierarchyScope;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EOWTDuplicationGenerationPolicy GenerationPolicy;

	/** Checked after user callbacks and before authored commit. Never serialized or retained by observers. */
	TFunction<bool()> IsCancellationRequested;
	/** Called once after authored commit and before procedural activation. It cannot cancel that commit. */
	TFunction<void(AActor*)> OnAuthoredCommitted;
};
