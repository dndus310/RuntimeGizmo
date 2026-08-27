// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToolContextInterfaces.h"
#include "RuntimeTransformGizmoComponent.generated.h"

class FViewport;
class UCombinedTransformGizmo;
class UInteractiveToolsContext;
class UMaterialInterface;
class UPrimitiveComponent;
class USceneComponent;
class UTransformProxy;
struct FInputDeviceState;

UCLASS(ClassGroup = (RuntimeGizmo), meta = (BlueprintSpawnableComponent))
class SIMPLE_PROJ_API URuntimeTransformGizmoComponent final : public UActorComponent, public IToolsContextQueriesAPI, public IToolsContextTransactionsAPI
{
	GENERATED_BODY()

public:
	URuntimeTransformGizmoComponent();

	UFUNCTION(BlueprintCallable, Category = "Runtime Transform Gizmo")
	void SelectActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Runtime Transform Gizmo")
	void ClearSelection();

	UFUNCTION(BlueprintCallable, Category = "Runtime Transform Gizmo")
	void SetTransformMode(EToolContextTransformGizmoMode NewMode);

	UFUNCTION(BlueprintCallable, Category = "Runtime Transform Gizmo")
	void SetTranslationMode();

	UFUNCTION(BlueprintCallable, Category = "Runtime Transform Gizmo")
	void SetRotationMode();

	UFUNCTION(BlueprintCallable, Category = "Runtime Transform Gizmo")
	void SetScaleMode();

	UFUNCTION(BlueprintCallable, Category = "Runtime Transform Gizmo")
	void SetCoordinateSystem(EToolContextCoordinateSystem NewCoordinateSystem);

	UFUNCTION(BlueprintCallable, Category = "Runtime Transform Gizmo")
	void ToggleCoordinateSystem();

	UFUNCTION(BlueprintPure, Category = "Runtime Transform Gizmo")
	bool IsGizmoInteracting() const;

	UFUNCTION(BlueprintPure, Category = "Runtime Transform Gizmo")
	AActor* GetSelectedActor() const { return SelectedActor.Get(); }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual UWorld* GetCurrentEditingWorld() const override;
	virtual void GetCurrentSelectionState(FToolBuilderState& StateOut) const override;
	virtual void GetCurrentViewState(FViewCameraState& StateOut) const override;
	virtual EToolContextCoordinateSystem GetCurrentCoordinateSystem() const override { return CoordinateSystem; }
	virtual EToolContextTransformGizmoMode GetCurrentTransformGizmoMode() const override { return CurrentGizmoMode; }
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
	void InitializeToolsContext();
	void ShutdownToolsContext();
	void EnsureTransformGizmo();
	void SyncActiveGizmoSettings();
	void ConfigureGizmoRenderPriority() const;
	void ApplyAlwaysOnTopGizmoMaterial(UPrimitiveComponent* Primitive) const;
	FLinearColor ResolveGizmoColor(UPrimitiveComponent* Primitive) const;
	void UpdateGizmoViewContext() const;
	bool BuildMouseInputState(FInputDeviceState& InputState);
	USceneComponent* ResolveTransformComponent(AActor* Actor) const;
	APlayerController* GetOwningPlayerController() const;
	void SetSelectedActorVisual(AActor* Actor, bool bSelected) const;
	void ApplySelectedOutline(UPrimitiveComponent* PrimitiveComponent);
	void ClearSelectedOutline();
	void HandleProxyTransformChanged(UTransformProxy* Proxy, FTransform NewTransform);

	UPROPERTY(Transient)
	TObjectPtr<UInteractiveToolsContext> ToolsContext;

	UPROPERTY(Transient)
	TObjectPtr<UCombinedTransformGizmo> ActiveGizmo;

	UPROPERTY(Transient)
	TObjectPtr<UTransformProxy> ActiveProxy;

	UPROPERTY(Transient)
	TObjectPtr<AActor> SelectedActor;

	UPROPERTY(Transient)
	TObjectPtr<USceneComponent> SelectedComponent;

	TWeakObjectPtr<UPrimitiveComponent> SelectedOutlineComponent;
	bool bPreviousRenderCustomDepth = false;
	int32 PreviousCustomDepthStencilValue = 0;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo")
	EToolContextCoordinateSystem CoordinateSystem = EToolContextCoordinateSystem::World;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo")
	EToolContextTransformGizmoMode CurrentGizmoMode = EToolContextTransformGizmoMode::Translation;

	static constexpr int32 SelectedOutlineStencilValue = 2;
	static constexpr int32 GizmoTranslucentSortPriority = 10000;


	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Snapping")
	bool bEnablePositionSnapping = false;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Snapping", meta = (EditCondition = "bEnablePositionSnapping"))
	FVector PositionSnapGrid = FVector(10.0);

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Snapping")
	bool bEnableRotationSnapping = false;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Snapping", meta = (EditCondition = "bEnableRotationSnapping"))
	FRotator RotationSnapGrid = FRotator(15.0, 15.0, 15.0);

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Snapping")
	bool bEnableScaleSnapping = false;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Snapping", meta = (EditCondition = "bEnableScaleSnapping"))
	float ScaleSnapGrid = 0.1f;

	FVector2D LastMousePosition = FVector2D::ZeroVector;
	bool bHasLastMousePosition = false;
	bool bWasLeftMouseDown = false;
	bool bWasMiddleMouseDown = false;
	bool bWasRightMouseDown = false;
	bool bHasOpenTransaction = false;
};
