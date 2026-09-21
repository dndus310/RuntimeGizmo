#pragma once

#include "CoreMinimal.h"
#include "InteractiveGizmoManager.h"
#include "Selection/VTBOWTEditorSelectionSource.h"
#include "VTBOWTEditorGizmoManager.generated.h"

class AActor;
class UVTBOWTEditorRepositionalGizmo;
class UVTBOWTEditorRepositionalGizmoBuilder;

UCLASS()
class VTBOWTEDITOR_API UVTBOWTEditorGizmoManager : public UInteractiveGizmoManager
{
	GENERATED_BODY()

public:
	virtual void Initialize(IToolsContextQueriesAPI* InQueriesAPI,
		IToolsContextTransactionsAPI* InTransactionsAPI, UInputRouter* InInputRouter) override;
	bool SynchronizeSelection(const TOptional<FVTBOWTTransformSelection>& Request, TFunctionRef<bool()> PrepareChange);
	void GetSelection(TArray<AActor*>& OutActors) const;
	void UpdateSelectionVisibility(bool bHasView);
	UVTBOWTEditorRepositionalGizmo* GetSelectionGizmo() const
	{
		return SelectionGizmo;
	}
	virtual void Shutdown() override;

	static const FString SelectionInstanceIdentifier;

private:
	bool EnsureSelectionGizmo();

	UPROPERTY(Transient)
	TObjectPtr<UVTBOWTEditorRepositionalGizmoBuilder> SelectionBuilder;

	UPROPERTY(Transient)
	TObjectPtr<UVTBOWTEditorRepositionalGizmo> SelectionGizmo;
};
