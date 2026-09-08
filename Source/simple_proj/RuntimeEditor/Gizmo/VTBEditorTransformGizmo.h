#pragma once

#include "CoreMinimal.h"
#include "BaseGizmos/CombinedTransformGizmo.h"
#include "UObject/Object.h"
#include "VTBEditorTransformGizmo.generated.h"

class AActor;
class AVTBEditorTransformGizmoActor;
class UGizmoViewContext;
class USceneComponent;
class UTransformProxy;
class UWorld;
class UVTBEditorInteractiveToolsContext;
class UVTBEditorTransformGizmoBehavior;

UCLASS()
class SIMPLE_PROJ_API UVTBEditorTransformGizmoBuilder : public UCombinedTransformGizmoBuilder
{
	GENERATED_BODY()

public:
	UVTBEditorTransformGizmoBuilder();

	virtual UInteractiveGizmo* BuildGizmo(const FToolBuilderState& SceneState) const override;

	ETransformGizmoSubElements EnabledElements;

	static const FString BuilderIdentifier;
};

/** Reserved extension point for project-specific handles. The default builder uses ITF's stock actor. */
UCLASS()
class SIMPLE_PROJ_API AVTBEditorTransformGizmoActor : public ACombinedTransformGizmoActor
{
	GENERATED_BODY()

public:
	AVTBEditorTransformGizmoActor();
};

UCLASS()
class SIMPLE_PROJ_API UVTBEditorTransformGizmo : public UCombinedTransformGizmo
{
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Shutdown() override;
	virtual void SetActiveTarget(UTransformProxy* Target, IToolContextTransactionProvider* TransactionProvider = nullptr) override;

	bool SetSelection(UWorld* InWorld, const TArray<TWeakObjectPtr<AActor>>& Actors);
	bool RefreshSelection();
	void RebuildFromCurrentTransforms();
	bool ApplyRuntimeState(UVTBEditorInteractiveToolsContext* Context, EToolContextTransformGizmoMode GizmoMode, const TOptional<TArray<TWeakObjectPtr<AActor>>>& SelectionRequest);
	UTransformProxy* GetTransformProxy() const { return TransformProxy; }
	void GetSelectedActors(TArray<AActor*>& OutActors) const;

	// Rebuilds only the visible actor; the caller cancels interactions before changing its elements.
	bool SetEnabledElements(ETransformGizmoSubElements Elements);
	void UpdateBehavior(bool bRejectAltDrag, bool bRejectCtrlDrag = false, bool bRejectShiftDrag = false);

private:
	bool RebuildTargetIfNeeded(bool bForceRebuild = false);
	void CacheTargetTransforms(UTransformProxy* Proxy, FTransform Transform);

	UPROPERTY(Transient)
	TObjectPtr<UVTBEditorTransformGizmoBehavior> Behavior;

	UPROPERTY(Transient)
	TObjectPtr<UTransformProxy> TransformProxy;

	UPROPERTY(Transient)
	TWeakObjectPtr<UWorld> TargetWorld;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> RequestedActors;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<USceneComponent>> TargetComponents;

	TArray<FTransform> TargetTransforms;
};

UCLASS()
class SIMPLE_PROJ_API UVTBEditorTransformGizmoBehavior : public UObject
{
	GENERATED_BODY()

public:
	void UpdateSettings(bool bRejectAltDrag, bool bRejectCtrlDrag = false, bool bRejectShiftDrag = false);
	void ConfigureSubGizmo(UInteractiveGizmo* SubGizmo);

private:
	UPROPERTY(Transient)
	bool bRejectAltDrag = true;

	UPROPERTY(Transient)
	bool bRejectCtrlDrag = false;

	UPROPERTY(Transient)
	bool bRejectShiftDrag = false;
};
