#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ToolContextInterfaces.h"
#include "VTBOWTEditorGizmoVisualComponent.generated.h"

class ACombinedTransformGizmoActor;
class UPrimitiveComponent;

UCLASS(Transient)
class VTBOWTEDITOR_API UVTBOWTEditorGizmoVisualComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static UVTBOWTEditorGizmoVisualComponent* FindOrAddTo(ACombinedTransformGizmoActor* GizmoActor);

	void ApplyMaterials();
	void UpdateCoordinateSystem(const TArray<TObjectPtr<UPrimitiveComponent>>& ActiveComponents,
		TFunctionRef<void(UPrimitiveComponent*)> UpdateComponent);
	void Update(EToolContextTransformGizmoMode Mode, bool bNonUniformScaleAllowed,
		UPrimitiveComponent* ActiveRotationComponent,
		TConstArrayView<TWeakObjectPtr<UPrimitiveComponent>> NonUniformScaleComponents);
};
