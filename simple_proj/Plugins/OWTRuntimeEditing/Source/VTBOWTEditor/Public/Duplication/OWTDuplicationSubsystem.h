#pragma once

#include "CoreMinimal.h"
#include "Duplication/OWTDuplicationRequest.h"
#include "Duplication/OWTDuplicationResult.h"
#include "State/OWTEditingSessionTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "OWTDuplicationSubsystem.generated.h"

class AActor;
class UOWTRuntimeActorDuplicator;

/**
 * Per-world owner of a runtime duplicator. It installs the registered providers on first use and ticks
 * procedural observation, so callers never manage a duplicator's Initialize/Tick/Deinitialize themselves.
 * Application adapters and rules can be added through GetDuplicator()->GetAdapterRegistry().
 */
UCLASS()
class VTBOWTEDITOR_API UOWTDuplicationSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UOWTDuplicationSubsystem();
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	UFUNCTION(BlueprintCallable, Category = "OWT|Duplication")
	FOWTDuplicationResult DuplicateActor(AActor* Source, const FOWTDuplicationOptions& Options);

	UFUNCTION(BlueprintCallable, Category = "OWT|Duplication")
	bool CanDuplicateActor(AActor* Source, const FOWTDuplicationOptions& Options, FString& OutError);

	UFUNCTION(BlueprintPure, Category = "OWT|Duplication")
	TArray<FOWTProceduralComponentSnapshot> GetProceduralComponents() const;

	/** Creates the duplicator on first use. Returns nullptr when the world cannot host duplication. */
	UOWTRuntimeActorDuplicator* GetDuplicator();

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UOWTRuntimeActorDuplicator> Duplicator;
};
