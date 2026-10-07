#pragma once

#include "CoreMinimal.h"
#include "VTBOWTEditorGameMode.h"
#include "OWTPlaygroundGameMode.generated.h"

/** Host setup for the authored playground maps. Editing remains in the runtime plugin. */
UCLASS()
class SIMPLE_PROJ_API AOWTPlaygroundGameMode : public AVTBOWTEditorGameMode
{
	GENERATED_BODY()

public:
	AOWTPlaygroundGameMode();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "OWT|Playground")
	bool IsPlaygroundReady() const;

private:
	void InitializePlayground();
	AActor* FindInitialSelection() const;

private:
	bool bPlaygroundReady;
};
