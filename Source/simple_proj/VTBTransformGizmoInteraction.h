// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/Function.h"
#include "VTBTransformGizmoTypes.h"
#include "UObject/Object.h"
#include "VTBTransformGizmoInteraction.generated.h"

class UCombinedTransformGizmo;
class UInteractiveToolsContext;
class UMaterialInterface;
class UPrimitiveComponent;
class USceneComponent;
class UTransformProxy;

UCLASS(Transient)
class SIMPLE_PROJ_API UVTBTransformGizmoInteraction final : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(
		UInteractiveToolsContext* InToolsContext,
		TUniqueFunction<void(bool)> TransformEditStateChangedCallbackIn = nullptr);
	void Shutdown();

	bool SelectTarget(
		AActor* TargetActor,
		bool bRequireTransformTargetInterface,
		const FVTBTransformGizmoSettings& Settings);
	void ClearSelection();
	void SyncGizmoSettings(const FVTBTransformGizmoSettings& Settings);

	AActor* GetSelectedActor() const { return SelectedActor.Get(); }
	USceneComponent* GetSelectedComponent() const { return SelectedComponent.Get(); }
	bool IsTransformEditInProgress() const { return bTransformEditInProgress; }

private:
	void DestroyActiveGizmo();
	void EnsureTransformGizmo(const FVTBTransformGizmoSettings& Settings);
	void ApplyActiveGizmoSettings();
	void ConfigureGizmoRenderPriority() const;
	void ApplyAlwaysOnTopGizmoMaterial(UPrimitiveComponent* Primitive) const;
	FLinearColor ResolveGizmoColor(UPrimitiveComponent* Primitive) const;
	USceneComponent* ResolveTransformComponent(AActor* TargetActor, bool bRequireTransformTargetInterface) const;
	void NotifySelectionChanged(AActor* TargetActor, bool bSelected) const;
	void ApplySelectedOutline(UPrimitiveComponent* PrimitiveComponent);
	void ClearSelectedOutline();
	void HandleBeginTransformEdit(UTransformProxy* Proxy);
	void HandleEndTransformEdit(UTransformProxy* Proxy);
	void HandleProxyTransformChanged(UTransformProxy* Proxy, FTransform NewTransform);
	void SetTransformEditInProgress(bool bInProgress);
	void ResetTransformEditState();
	void UpdateRotationDeltaDisplay(const FTransform& NewTransform, bool bFinalMessage);
	void UpdateRotationAxisIsolation(const FTransform& NewTransform);
	bool TryDetectRotationAxis(const FTransform& NewTransform, EAxis::Type& AxisOut) const;
	EAxis::Type GetHoveredRotationAxis() const;
	void SetRotationAxisIsolation(EAxis::Type VisibleAxis);
	void ResetRotationAxisVisibility();

	struct FTransformEditState
	{
		FTransform InitialTransform = FTransform::Identity;
		double LastDisplayedRotationDeltaDegrees = 0.0;
		bool bHasInitialTransform = false;
	};

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
	bool bTransformEditInProgress = false;
	FVTBTransformGizmoSettings ActiveSettings;
	FVTBTransformGizmoSettings PendingSettings;
	FTransformEditState TransformEditState;
	TUniqueFunction<void(bool)> TransformEditStateChangedCallback = [](bool) {};
	bool bHasPendingSettings = false;
	bool bRotationAxisIsolationActive = false;
	EAxis::Type IsolatedRotationAxis = EAxis::None;

	static constexpr int32 SelectedOutlineStencilValue = 2;
	static constexpr int32 GizmoTranslucentSortPriority = 10000;
	static constexpr uint64 RotationDeltaMessageKeyOffset = 1;
};
