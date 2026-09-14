#include "Gizmos/Base/VTBOWTBaseTransformGizmo.h"

#include "BaseGizmos/GizmoViewContext.h"
#include "BaseGizmos/TransformProxy.h"
#include "BaseGizmos/ViewAdjustedStaticMeshGizmoComponent.h"
#include "Components/SceneComponent.h"
#include "ContextObjectStore.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/StrongObjectPtr.h"

namespace
{
class FVTBOWTBaseTransformGizmoActorFactory final : public FCombinedTransformGizmoActorFactory
{
public:
	FVTBOWTBaseTransformGizmoActorFactory(UGizmoViewContext& ViewContext, UMaterialInterface& InMaterial)
	    : FCombinedTransformGizmoActorFactory(&ViewContext), Material(&InMaterial)
	{
	}

	virtual ACombinedTransformGizmoActor* CreateNewGizmoActor(UWorld* World) const override
	{
		ACombinedTransformGizmoActor* Actor = FCombinedTransformGizmoActorFactory::CreateNewGizmoActor(World);
		if (!ensureMsgf(Actor, TEXT("OWT default gizmo actor creation failed.")))
		{
			return nullptr;
		}

		ApplyMaterials(*Actor);
		return Actor;
	}

private:
	void ApplyMaterials(ACombinedTransformGizmoActor& Actor) const
	{
		// Includes the full-circle and full-axis substitutes used during interaction.
		TInlineComponentArray<UViewAdjustedStaticMeshGizmoComponent*> Components(&Actor);
		for (UViewAdjustedStaticMeshGizmoComponent* Component : Components)
		{
			UMaterialInterface* DefaultMaterial = Component->GetMaterial(0);
			check(DefaultMaterial);
			Component->SetAllMaterials(CreateMaterial(*DefaultMaterial, *Component));

			if (UMaterialInterface* HoverMaterial = Component->GetHoverOverrideMaterial())
			{
				Component->SetHoverOverrideMaterial(CreateMaterial(*HoverMaterial, *Component));
			}
		}
	}

	UMaterialInstanceDynamic* CreateMaterial(UMaterialInterface& Source, UObject& Outer) const
	{
		// The parent has OccludeByCustomDepth disabled at compile time; MIDs cannot change static switches.
		UMaterialInstanceDynamic* Instance = UMaterialInstanceDynamic::Create(Material.Get(), &Outer);
		Instance->CopyMaterialUniformParameters(&Source);
		return Instance;
	}

private:
	TStrongObjectPtr<UMaterialInterface> Material;
};
} // namespace

UInteractiveGizmo* UVTBOWTBaseTransformGizmoBuilder::BuildGizmo(const FToolBuilderState& SceneState) const
{
	// The subsystem registers this builder only after the runtime context is initialized.
	check(SceneState.World && SceneState.ToolManager && SceneState.GizmoManager);
	UGizmoViewContext* ViewContext = SceneState.ToolManager->GetContextObjectStore()->FindContext<UGizmoViewContext>();
	check(ViewContext);
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(
	    nullptr, TEXT("/Game/VTBOWT/Materials/MI_OWTGizmo_NotOccluded.MI_OWTGizmo_NotOccluded"));
	if (!ensureMsgf(Material, TEXT("OWT gizmo material is missing. Run Scripts/CreateOWTGizmoMaterial.py.")))
	{
		return nullptr;
	}

	UVTBOWTBaseTransformGizmo* Gizmo = NewObject<UVTBOWTBaseTransformGizmo>(SceneState.GizmoManager);
	Gizmo->SetWorld(SceneState.World);
	Gizmo->SetGizmoActorBuilder(MakeShared<FVTBOWTBaseTransformGizmoActorFactory>(*ViewContext, *Material));
	Gizmo->SetSubGizmoBuilderIdentifiers(TEXT("OWT.AxisPosition"),
	                                     UInteractiveGizmoManager::DefaultPlanePositionBuilderIdentifier,
	                                     TEXT("OWT.AxisAngle"));
	return Gizmo;
}

void UVTBOWTBaseTransformGizmo::SetTargetComponent(USceneComponent& Component)
{
	// Selection eligibility is checked by the subsystem before target assignment.
	UTransformProxy* Proxy = NewObject<UTransformProxy>(this);
	Proxy->AddComponent(&Component);
	SetActiveTarget(Proxy);
}

UTransformProxy* UVTBOWTBaseTransformGizmo::GetTransformProxy() const
{
	return ActiveTarget;
}
