// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveToolsContext.h"
#include "ToolContextInterfaces.h"
#include "VTBEditorInteractiveToolsContext.generated.h"

class APlayerController;
class AActor;
class FVTBEditorQueriesAPI;
class FVTBEditorTransactionsAPI;
class UGizmoViewContext;
class UVTBEditorTransformGizmo;
class UVTBEditorTransformGizmoBuilder;
struct FInputDeviceState;

enum class EVTBEditorRuntimePhase : uint8
{
	Uninitialized,
	Ready,
	ShutdownPending,
	ShuttingDown
};

UCLASS(Transient)
class SIMPLE_PROJ_API UVTBEditorInteractiveToolsContext : public UInteractiveToolsContext
{
	GENERATED_BODY()

public:
	UVTBEditorInteractiveToolsContext();

	virtual void Shutdown() override;
	virtual void BeginDestroy() override;

	bool InitializeRuntime(UWorld* World);
	bool IsRuntimeReady() const;
	UWorld* GetEditingWorld() const;
	EToolContextTransformGizmoMode GetGizmoMode() const;
	bool UpdateView(APlayerController* PlayerController);
	bool UpdateView();
	bool PostPointerInput(const FInputDeviceState& Input, bool bHover);
	void TickRuntime(float DeltaTime);
	bool ApplyTransformGizmoState(UObject* Owner, const TOptional<TArray<TWeakObjectPtr<AActor>>>& SelectionRequest);
	void UpdateTransformGizmoVisibility(bool bHasView);
	void SetSelection(const TArray<AActor*>& Actors);
	void SetCoordinateSystem(EToolContextCoordinateSystem CoordinateSystem);
	void SetGizmoMode(EToolContextTransformGizmoMode Mode);
	void CancelActiveInteraction();
	bool HasActiveMouseCapture() const;
	bool Undo();
	bool Redo();
	bool CanUndo() const;
	bool CanRedo() const;
	bool IsCancellingInteraction() const { return bCancellingInteraction; }
	bool IsReplayingTransaction() const;

private:
	UVTBEditorTransformGizmo* CreateTransformGizmo(UObject* Owner);
	bool BeginRuntimeUpdate();
	void EndRuntimeUpdate();

	UPROPERTY(Transient)
	TObjectPtr<UGizmoViewContext> GizmoViewContext;

	// Incomplete implementation types stay out of this header, including generated UObject code.
	TSharedPtr<FVTBEditorQueriesAPI> QueriesAPI;
	TSharedPtr<FVTBEditorTransactionsAPI> TransactionsAPI;
	FDelegateHandle ApplicationFocusHandle;

	UPROPERTY(Transient)
	TObjectPtr<UVTBEditorTransformGizmoBuilder> GizmoBuilder;

	UPROPERTY(Transient)
	TObjectPtr<UVTBEditorTransformGizmo> TransformGizmo;

	EVTBEditorRuntimePhase RuntimePhase;
	bool bCancellingInteraction;
	bool bUpdating;
};
