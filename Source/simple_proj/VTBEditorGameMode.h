// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UObject/Interface.h"
#include "VTBEditorGameMode.generated.h"

/** Listeners pull a complete selection snapshot after this game-thread notification. */
DECLARE_MULTICAST_DELEGATE(FVTBSelectionChanged);

/** Shared by GameMode, PlayerController, or another native selection provider. */
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UVTBSelectionSource : public UInterface
{
	GENERATED_BODY()
};

class SIMPLE_PROJ_API IVTBSelectionSource
{
	GENERATED_BODY()

public:
	virtual void GetSelectionSnapshot(TArray<TWeakObjectPtr<AActor>>& OutActors) const = 0;
	virtual FVTBSelectionChanged& OnSelectionChanged() = 0;
};

UCLASS()
class SIMPLE_PROJ_API AVTBEditorGameMode : public AGameMode, public IVTBSelectionSource
{
	GENERATED_BODY()

public:
	AVTBEditorGameMode();

	UFUNCTION(BlueprintCallable, Category = "VTB Editor|Selection")
	void SetSelectedActors(const TArray<AActor*>& Actors);

	UFUNCTION(BlueprintCallable, Category = "VTB Editor|Selection")
	void ClearSelection();

	virtual void GetSelectionSnapshot(TArray<TWeakObjectPtr<AActor>>& OutActors) const override;
	virtual FVTBSelectionChanged& OnSelectionChanged() override { return SelectionChanged; }

private:
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> SelectedActors;

	FVTBSelectionChanged SelectionChanged;
};
