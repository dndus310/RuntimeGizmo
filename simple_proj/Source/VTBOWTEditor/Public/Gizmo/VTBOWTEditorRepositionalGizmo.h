#pragma once

#include "CoreMinimal.h"
#include "BaseGizmos/RepositionableTransformGizmo.h"
#include "Gizmo/VTBOWTEditorGizmoInteraction.h"
#include "VTBOWTEditorRepositionalGizmo.generated.h"

class AActor;
class UTransformProxy;
class UVTBOWTEditorGizmoSelection;

UCLASS()
class VTBOWTEDITOR_API UVTBOWTEditorRepositionalGizmoBuilder : public URepositionableTransformGizmoBuilder
{
	GENERATED_BODY()

public:
	UVTBOWTEditorRepositionalGizmoBuilder();
	virtual UInteractiveGizmo* BuildGizmo(const FToolBuilderState& SceneState) const override;

	ETransformGizmoSubElements EnabledElements;
	static const FString BuilderIdentifier;
};

UCLASS()
class VTBOWTEDITOR_API UVTBOWTEditorRepositionalGizmo : public URepositionableTransformGizmo
{
	GENERATED_BODY()

public:
	UVTBOWTEditorRepositionalGizmo();

	virtual void Setup() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Shutdown() override;
	virtual void SetActiveTarget(UTransformProxy* Target, IToolContextTransactionProvider* TransactionProvider = nullptr) override;
	virtual void ClearActiveTarget() override;

	bool SetSelection(UWorld* InWorld, const TArray<TWeakObjectPtr<AActor>>& Actors, USceneComponent* FrameComponent = nullptr);
	USceneComponent* GetSelectionFrame() const;
	bool IsSelectionStateStale() const;
	bool RefreshSelection();
	void RebuildFromCurrentTransforms();
	void GetSelectedActors(TArray<AActor*>& OutActors) const;
	UTransformProxy* GetTransformProxy() const;
	bool SetEnabledElements(ETransformGizmoSubElements Elements);

private:
	friend class FVTBOWTEditorGizmoInteraction;
	FVTBOWTEditorGizmoInteraction Interaction;

	UPROPERTY(Transient)
	TObjectPtr<UVTBOWTEditorGizmoSelection> Selection;
};
