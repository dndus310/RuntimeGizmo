#include "Gizmo/VTBOWTEditorGizmoVisualComponent.h"

#include "BaseGizmos/CombinedTransformGizmo.h"
#include "BaseGizmos/GizmoRenderingUtil.h"
#include "BaseGizmos/TransformSubGizmoUtil.h"
#include "BaseGizmos/ViewAdjustedStaticMeshGizmoComponent.h"
#include "BaseGizmos/ViewBasedTransformAdjusters.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "UObject/SoftObjectPath.h"

namespace
{
	constexpr TCHAR RuntimeGizmoMaterialPath[] = TEXT("/Script/Engine.MaterialInstanceConstant'/Engine/InteractiveToolsFramework/Materials/GizmoComponentMaterial_NotDimmed.GizmoComponentMaterial_NotDimmed'");
	constexpr TCHAR RuntimeGizmoMaterialFallbackPath[] = TEXT("/Engine/InteractiveToolsFramework/Materials/GizmoComponentMaterial_NotDimmed");
	constexpr float CombinedScalePlaneOffset = 120.0f;
	constexpr float ScaleOnlyPlaneOffset = 75.0f;

	UMaterialInterface* LoadRuntimeGizmoMaterial()
	{
		UMaterialInterface* Material = Cast<UMaterialInterface>(FSoftObjectPath(RuntimeGizmoMaterialPath).TryLoad());
		if (!IsValid(Material))
		{
			Material = LoadObject<UMaterialInterface>(nullptr, RuntimeGizmoMaterialFallbackPath);
		}

		return Material;
	}

	void ApplyStockScalePlaneLayout(ACombinedTransformGizmoActor* GizmoActor, bool bCombinedMode)
	{
		if (!IsValid(GizmoActor) || !IsValid(GizmoActor->GetRootComponent()))
		{
			return;
		}

		const float PlaneOffset = bCombinedMode ? CombinedScalePlaneOffset : ScaleOnlyPlaneOffset;
		const FVector CornerPosition(0.0f, PlaneOffset, PlaneOffset);
		const FVector CornerScale(0.5f);
		const FTransform BaseTransform(FQuat::Identity, CornerPosition, CornerScale);

		const TPair<UPrimitiveComponent*, EAxis::Type> PlaneEntries[] =
		{
			{ GizmoActor->PlaneScaleXY, EAxis::Z },
			{ GizmoActor->PlaneScaleXZ, EAxis::Y },
			{ GizmoActor->PlaneScaleYZ, EAxis::X }
		};
		for (const TPair<UPrimitiveComponent*, EAxis::Type>& Entry : PlaneEntries)
		{
			UViewAdjustedStaticMeshGizmoComponent* Component = Cast<UViewAdjustedStaticMeshGizmoComponent>(Entry.Key);
			if (!IsValid(Component))
			{
				continue;
			}

			const FTransform DesiredTransform = UE::GizmoUtil::GetRotatedBasisTransform(BaseTransform, Entry.Value);
			const bool bTransformChanged = !Component->GetRelativeTransform().Equals(DesiredTransform);
			if (bTransformChanged)
			{
				Component->SetRelativeTransform(DesiredTransform);
			}
			if (bTransformChanged || !Component->GetTransformAdjuster().IsValid())
			{
				UE::GizmoRenderingUtil::FSubGizmoTransformAdjuster::AddTransformAdjuster(
					Component, GizmoActor->GetRootComponent(), bCombinedMode);
			}
		}
	}
}

UVTBOWTEditorGizmoVisualComponent* UVTBOWTEditorGizmoVisualComponent::FindOrAddTo(ACombinedTransformGizmoActor* GizmoActor)
{
	if (!IsValid(GizmoActor))
	{
		return nullptr;
	}
	UVTBOWTEditorGizmoVisualComponent* Component = GizmoActor->FindComponentByClass<UVTBOWTEditorGizmoVisualComponent>();
	if (!Component)
	{
		Component = NewObject<UVTBOWTEditorGizmoVisualComponent>(GizmoActor, NAME_None, RF_Transient);
		GizmoActor->AddInstanceComponent(Component);
	}
	if (!Component->IsRegistered())
	{
		Component->RegisterComponent();
	}
	return Component;
}

void UVTBOWTEditorGizmoVisualComponent::ApplyMaterials()
{
	ACombinedTransformGizmoActor* GizmoActor = Cast<ACombinedTransformGizmoActor>(GetOwner());
	if (!IsValid(GizmoActor))
	{
		return;
	}

	UMaterialInterface* BaseMaterial = LoadRuntimeGizmoMaterial();
	if (!ensureMsgf(IsValid(BaseMaterial), TEXT("Runtime gizmo material could not be loaded.")))
	{
		return;
	}

	TArray<UViewAdjustedStaticMeshGizmoComponent*> Components;
	GizmoActor->GetComponents(Components);
	for (UViewAdjustedStaticMeshGizmoComponent* Component : Components)
	{
		if (!IsValid(Component) || Component->GetNumMaterials() == 0)
		{
			continue;
		}

		FLinearColor GizmoColor = FLinearColor::White;
		UMaterialInstanceDynamic* ExistingMaterial = Cast<UMaterialInstanceDynamic>(Component->GetMaterial(0));
		if (ExistingMaterial)
		{
			GizmoColor = ExistingMaterial->K2_GetVectorParameterValue(TEXT("GizmoColor"));
		}

		UMaterialInstanceDynamic* RuntimeMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, Component);
		RuntimeMaterial->SetVectorParameterValue(TEXT("GizmoColor"), GizmoColor);
		Component->SetAllMaterials(RuntimeMaterial);
		UMaterialInstanceDynamic* HoverMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, Component);
		HoverMaterial->SetVectorParameterValue(TEXT("GizmoColor"), FLinearColor::Yellow);
		Component->SetHoverOverrideMaterial(HoverMaterial);
	}
}

void UVTBOWTEditorGizmoVisualComponent::UpdateCoordinateSystem(
	const TArray<TObjectPtr<UPrimitiveComponent>>& ActiveComponents,
	TFunctionRef<void(UPrimitiveComponent*)> UpdateComponent)
{
	ACombinedTransformGizmoActor* GizmoActor = Cast<ACombinedTransformGizmoActor>(GetOwner());
	if (!IsValid(GizmoActor))
	{
		return;
	}
	TArray<UViewAdjustedStaticMeshGizmoComponent*> ViewAdjustedComponents;
	GizmoActor->GetComponents(ViewAdjustedComponents);
	for (UViewAdjustedStaticMeshGizmoComponent* Component : ViewAdjustedComponents)
	{
		if (IsValid(Component) && !ActiveComponents.Contains(Component))
		{
			UpdateComponent(Component);
		}
	}
}

void UVTBOWTEditorGizmoVisualComponent::Update(EToolContextTransformGizmoMode Mode,
	bool bNonUniformScaleAllowed,
	UPrimitiveComponent* ActiveRotationComponent,
	TConstArrayView<TWeakObjectPtr<UPrimitiveComponent>> NonUniformScaleComponents)
{
	ACombinedTransformGizmoActor* GizmoActor = Cast<ACombinedTransformGizmoActor>(GetOwner());
	if (!IsValid(GizmoActor))
	{
		return;
	}

	const bool bScaleMode = Mode == EToolContextTransformGizmoMode::Scale;
	const bool bCombinedMode = Mode == EToolContextTransformGizmoMode::Combined;
	const bool bScaleVisible = bScaleMode || bCombinedMode;
	if (bScaleVisible)
	{
		ApplyStockScalePlaneLayout(GizmoActor, bCombinedMode);
	}

	for (const TWeakObjectPtr<UPrimitiveComponent>& WeakComponent : NonUniformScaleComponents)
	{
		UPrimitiveComponent* Component = WeakComponent.Get();
		if (IsValid(Component))
		{
			Component->SetVisibility(bScaleVisible && bNonUniformScaleAllowed);
		}
	}

	const bool bRotationMode = Mode == EToolContextTransformGizmoMode::Rotation
		|| Mode == EToolContextTransformGizmoMode::Combined;
	UPrimitiveComponent* RotationComponents[] = { GizmoActor->RotateX, GizmoActor->RotateY, GizmoActor->RotateZ };
	for (UPrimitiveComponent* Component : RotationComponents)
	{
		if (IsValid(Component))
		{
			Component->SetVisibility(bRotationMode
				&& (ActiveRotationComponent == nullptr || Component == ActiveRotationComponent));
		}
	}
	if (IsValid(GizmoActor->RotationSphere))
	{
		GizmoActor->RotationSphere->SetVisibility(bRotationMode && ActiveRotationComponent == nullptr);
	}
}
