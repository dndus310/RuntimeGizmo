// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "VTBEditorModeInterface.h"
#include "VTBOWTEditGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVTBEditorStateChanged, bool, bEditMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVTBActiveGizmoModeChanged, EActiveGizmoMode, ActiveGizmoMode);

UCLASS()
class SIMPLE_PROJ_API AVTBOWTEditGameState : public AGameState, public IVTBOWTEditorModeState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool GetEditorState_Implementation() const override;
	virtual EActiveGizmoMode GetActiveGizmoMode_Implementation() const override;

	// GameMode forwards authoritative changes through these methods.
	void UpdateEditorState(bool bNewState);
	void UpdateActiveGizmoMode(EActiveGizmoMode NewGizmoMode);

	UPROPERTY(BlueprintAssignable, Category = "VTB Editor|Mode")
	FVTBEditorStateChanged OnEditorStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "VTB Editor|Mode")
	FVTBActiveGizmoModeChanged OnActiveGizmoModeChanged;

private:
	UPROPERTY(ReplicatedUsing = OnRep_EditMode)
	bool bEditMode = false;

	UPROPERTY(ReplicatedUsing = OnRep_ActiveGizmoMode)
	EActiveGizmoMode ActiveGizmoMode = EActiveGizmoMode::Transform;

	UFUNCTION()
	void OnRep_EditMode();

	UFUNCTION()
	void OnRep_ActiveGizmoMode();
};
