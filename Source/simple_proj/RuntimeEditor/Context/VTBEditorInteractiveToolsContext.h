// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveToolsContext.h"
#include "ToolContextInterfaces.h"
#include "VTBEditorInteractiveToolsContext.generated.h"

class APlayerController;
class FVTBEditorQueriesAPI;
class FVTBEditorTransactionsAPI;
class UGizmoViewContext;

UCLASS(Transient)
class SIMPLE_PROJ_API UVTBEditorInteractiveToolsContext : public UInteractiveToolsContext
{
	GENERATED_BODY()

public:
	UVTBEditorInteractiveToolsContext();

	virtual void Shutdown() override;
	virtual void BeginDestroy() override;

	bool InitializeRuntime(UWorld* World);
	bool UpdateView(APlayerController* PlayerController);
	void TickRuntime(float DeltaTime);
	void SetSelection(const TArray<AActor*>& Actors);
	void SetCoordinateSystem(EToolContextCoordinateSystem CoordinateSystem);
	void SetGizmoMode(EToolContextTransformGizmoMode Mode);
	void CancelActiveInteraction();
	bool Undo();
	bool Redo();
	bool CanUndo() const;
	bool CanRedo() const;
	bool IsCancellingInteraction() const { return bCancellingInteraction; }

private:
	UPROPERTY(Transient)
	TObjectPtr<UGizmoViewContext> GizmoViewContext;

	// Incomplete implementation types stay out of this header, including generated UObject code.
	TSharedPtr<FVTBEditorQueriesAPI> QueriesAPI;
	TSharedPtr<FVTBEditorTransactionsAPI> TransactionsAPI;
	FDelegateHandle ApplicationFocusHandle;

	bool bRuntimeInitialized;
	bool bCancellingInteraction;
};
