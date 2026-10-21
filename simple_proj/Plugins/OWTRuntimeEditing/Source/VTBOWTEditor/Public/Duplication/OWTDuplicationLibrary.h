#pragma once

#include "CoreMinimal.h"
#include "Duplication/OWTDuplicationRequest.h"
#include "Duplication/OWTDuplicationResult.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "State/OWTEditingSessionTypes.h"
#include "OWTDuplicationLibrary.generated.h"

class AActor;
class UOWTDuplicationSubsystem;

/**
 * One-call entry points for runtime Actor duplication. The Source's world subsystem performs the work.
 *
 *   FOWTDuplicationResult Result = UOWTDuplicationLibrary::DuplicateActorAtOffset(Source, FVector(200, 0, 0));
 *   if (!Result.bSucceeded) { UE_LOG(LogTemp, Warning, TEXT("%s"), *Result.Error); }
 */
UCLASS()
class VTBOWTEDITOR_API UOWTDuplicationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Duplicates Source and its authored hierarchy. Nothing remains in the world when it fails. */
	UFUNCTION(BlueprintCallable, Category = "OWT|Duplication")
	static FOWTDuplicationResult DuplicateActor(AActor* Source, const FOWTDuplicationOptions& Options);

	/** DuplicateActor with default options and the given world-space offset. */
	UFUNCTION(BlueprintCallable, Category = "OWT|Duplication")
	static FOWTDuplicationResult DuplicateActorAtOffset(AActor* Source, FVector WorldOffset);

	/** Reports why Source cannot be duplicated without spawning anything. */
	UFUNCTION(BlueprintCallable, Category = "OWT|Duplication")
	static bool CanDuplicateActor(AActor* Source, const FOWTDuplicationOptions& Options, FString& OutError);

	UFUNCTION(BlueprintPure, Category = "OWT|Duplication", meta = (WorldContext = "WorldContextObject"))
	static TArray<FOWTProceduralComponentSnapshot> GetProceduralComponents(const UObject* WorldContextObject);

	static UOWTDuplicationSubsystem* GetSubsystem(const UObject* WorldContextObject);
};
