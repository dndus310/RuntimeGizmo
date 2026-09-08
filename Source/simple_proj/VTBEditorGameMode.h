// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "VTBEditorModeInterface.h"
#include "RuntimeEditor/Selection/VTBSelectionSource.h"
#include "VTBEditorGameMode.generated.h"

UCLASS()
class SIMPLE_PROJ_API AVTBEditorGameMode : public AGameMode, public IVTBOWTEditorModeControl, public IVTBSelectionSource
{
	GENERATED_BODY()

public:
	AVTBEditorGameMode();
	virtual void InitGameState() override;

	virtual void SetEditorState_Implementation(bool bNewState) override;
	virtual void SetActiveGizmoMode_Implementation(EActiveGizmoMode NewGizmoMode) override;

	UFUNCTION(BlueprintCallable, Category = "VTB Editor|Selection")
	void SetSelectedActors(const TArray<AActor*>& Actors);

	UFUNCTION(BlueprintCallable, Category = "VTB Editor|Selection")
	void ClearSelection();

	virtual void GetSelectionSnapshot(TArray<TWeakObjectPtr<AActor>>& OutActors) const override;
	virtual FVTBSelectionChanged& OnSelectionChanged() override;

private:
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> SelectedActors;

	FVTBSelectionChanged SelectionChanged;

	// Preserve commands received before GameState is created.
	bool bEditMode = false;
	EActiveGizmoMode ActiveGizmoMode = EActiveGizmoMode::Transform;
};
