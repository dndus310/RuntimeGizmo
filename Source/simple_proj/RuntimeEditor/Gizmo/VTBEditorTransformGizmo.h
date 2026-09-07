#pragma once

#include "CoreMinimal.h"
#include "BaseGizmos/CombinedTransformGizmo.h"
#include "VTBEditorTransformGizmo.generated.h"

USTRUCT(BlueprintType)
struct SIMPLE_PROJ_API FVTBEditorGizmoBehaviorSettings
{
	GENERATED_BODY()

	FVTBEditorGizmoBehaviorSettings();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmo|Input")
	bool bRejectAltDrag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmo|Input")
	bool bRejectCtrlDrag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gizmo|Input")
	bool bRejectShiftDrag;
};

/** Adds runtime input policy to the stock combined gizmo composition. */
UCLASS(Transient)
class SIMPLE_PROJ_API UVTBEditorTransformGizmo : public UCombinedTransformGizmo
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetActiveTarget(UTransformProxy* Target, IToolContextTransactionProvider* TransactionProvider = nullptr) override;

	void UpdateBehavior(const FVTBEditorGizmoBehaviorSettings& Settings);

private:
	UPROPERTY(Transient)
	FVTBEditorGizmoBehaviorSettings BehaviorSettings;
};

/** Registered once per runtime context. Uses ITF's stock actor factory with runtime element filtering. */
UCLASS(Transient)
class SIMPLE_PROJ_API UVTBEditorTransformGizmoBuilder : public UCombinedTransformGizmoBuilder
{
	GENERATED_BODY()

public:
	UVTBEditorTransformGizmoBuilder();

	virtual UInteractiveGizmo* BuildGizmo(const FToolBuilderState& SceneState) const override;

	UPROPERTY(EditAnywhere, Category = "Gizmo")
	FVTBEditorGizmoBehaviorSettings BehaviorSettings;

	ETransformGizmoSubElements EnabledElements;

	static const FString BuilderIdentifier;
};
