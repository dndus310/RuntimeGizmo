// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ToolContextInterfaces.h"
#include "VTBTransformGizmoTypes.h"
#include "VTBViewportToolInput.h"
#include "VTBInteractiveToolsSubsystem.generated.h"

class APlayerController;
class FViewport;
class UInteractiveToolsContext;
class UMaterialInterface;
class USceneComponent;
class UVTBTransformGizmoInteraction;
class UVTBTransformSelection;

UCLASS()
class SIMPLE_PROJ_API UVTBInteractiveToolsSubsystem final : public UTickableWorldSubsystem, public IToolsContextQueriesAPI, public IToolsContextTransactionsAPI
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	void InitializeToolsContext(APlayerController* InPlayerController);
	void ShutdownToolsContext();
	void ShutdownToolsContextForPlayer(APlayerController* InPlayerController);

	UFUNCTION(BlueprintCallable, Category = "VTB Runtime Tools")
	void ApplySettings(const FVTBTransformGizmoSettings& InSettings);

	const FVTBTransformGizmoSettings& GetSettings() const { return Settings; }

	UFUNCTION(BlueprintCallable, Category = "VTB Runtime Tools")
	bool SelectTransformTarget(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "VTB Runtime Tools")
	void ClearSelection();

	UFUNCTION(BlueprintCallable, Category = "VTB Runtime Tools")
	void SetTransformMode(EToolContextTransformGizmoMode NewMode);

	UFUNCTION(BlueprintCallable, Category = "VTB Runtime Tools")
	void SetTranslationMode();

	UFUNCTION(BlueprintCallable, Category = "VTB Runtime Tools")
	void SetRotationMode();

	UFUNCTION(BlueprintCallable, Category = "VTB Runtime Tools")
	void SetScaleMode();

	UFUNCTION(BlueprintCallable, Category = "VTB Runtime Tools")
	void SetCoordinateSystem(EToolContextCoordinateSystem NewCoordinateSystem);

	UFUNCTION(BlueprintCallable, Category = "VTB Runtime Tools")
	void ToggleCoordinateSystem();

	UFUNCTION(BlueprintPure, Category = "VTB Runtime Tools")
	bool IsGizmoInteracting() const;

	UFUNCTION(BlueprintPure, Category = "VTB Runtime Tools")
	bool ShouldBlockViewportCameraInput() const;

	void RefreshInputStateAndCameraLock();

	UFUNCTION(BlueprintPure, Category = "VTB Runtime Tools")
	AActor* GetSelectedActor() const;

	UFUNCTION(BlueprintPure, Category = "VTB Runtime Tools")
	USceneComponent* GetSelectedComponent() const;

	UInteractiveToolsContext* GetToolsContext() const { return ToolsContext; }

protected:
	virtual UWorld* GetCurrentEditingWorld() const override;
	virtual void GetCurrentSelectionState(FToolBuilderState& StateOut) const override;
	virtual void GetCurrentViewState(FViewCameraState& StateOut) const override;
	virtual EToolContextCoordinateSystem GetCurrentCoordinateSystem() const override;
	virtual EToolContextTransformGizmoMode GetCurrentTransformGizmoMode() const override { return Settings.TransformMode; }
	virtual FToolContextSnappingConfiguration GetCurrentSnappingSettings() const override;
	virtual UMaterialInterface* GetStandardMaterial(EStandardToolContextMaterials MaterialType) const override;
	virtual FViewport* GetHoveredViewport() const override;
	virtual FViewport* GetFocusedViewport() const override;

	virtual void DisplayMessage(const FText& Message, EToolMessageLevel Level) override;
	virtual void PostInvalidation() override;
	virtual void BeginUndoTransaction(const FText& Description) override;
	virtual void EndUndoTransaction() override;
	virtual void AppendChange(UObject* TargetObject, TUniquePtr<FToolCommandChange> Change, const FText& Description) override;
	virtual bool RequestSelectionChange(const FSelectedObjectsChangeList& SelectionChange) override;

private:
	void RegisterSelectionInteraction();
	void DeregisterSelectionInteraction();
	bool CanChangeSelectionFromInput() const;
	void SyncActiveGizmoSettings();
	FVTBTransformGizmoSettings MakeActiveGizmoSettings() const;
	EToolContextCoordinateSystem GetEffectiveCoordinateSystem() const;
	void UpdateGizmoViewContext() const;
	void ExecuteViewportToolCommand(EVTBViewportToolCommand Command);
	APlayerController* GetPlayerController() const;
	bool IsViewportCameraActor(AActor* Actor) const;
	bool IsTransformEditInProgress() const;

	UPROPERTY(Transient)
	TObjectPtr<UInteractiveToolsContext> ToolsContext;

	UPROPERTY(Transient)
	TObjectPtr<UVTBTransformGizmoInteraction> TransformInteraction;

	UPROPERTY(Transient)
	TObjectPtr<UVTBTransformSelection> SelectionInteraction;

	TWeakObjectPtr<APlayerController> ActivePlayerController;
	FVTBTransformGizmoSettings Settings;
	FVTBViewportToolInput ViewportInput;

	bool bHasOpenTransaction = false;
};
