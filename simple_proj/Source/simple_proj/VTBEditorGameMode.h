#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "VTBEditorModeInterface.h"
#include "Selection/VTBOWTEditorSelectionSource.h"
#include "VTBEditorGameMode.generated.h"

class USceneComponent;

UCLASS()
class SIMPLE_PROJ_API AVTBEditorGameMode : public AGameMode, public IVTBOWTEditorModeControl, public IVTBOWTEditorSelectionSource
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
	void SetSelectedComponent(USceneComponent* Component);

	UFUNCTION(BlueprintCallable, Category = "VTB Editor|Selection")
	void ClearSelection();

	virtual void GetSelection(TArray<TWeakObjectPtr<AActor>>& OutActors) const override;
	virtual USceneComponent* GetSelectionFrame() const override;
	virtual FVTBOWTSelectionChanged& OnSelectionChanged() override;
	virtual bool ApplySelectionChange(const FSelectedObjectsChangeList& Change) override;

private:
	void SetSelection(const TArray<AActor*>& Actors, USceneComponent* FrameComponent);

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> SelectedActors;

	UPROPERTY(Transient)
	TWeakObjectPtr<USceneComponent> SelectedFrameComponent;

	FVTBOWTSelectionChanged SelectionChanged;

	bool bEditMode = false;
	EActiveGizmoMode ActiveGizmoMode = EActiveGizmoMode::Transform;
};
