#include "VTBOWTEditorGizmoManager.h"

#include "BaseGizmos/TransformProxy.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Gizmo/VTBOWTEditorRepositionalGizmo.h"
#include "UObject/UObjectHash.h"

namespace
{
	ETransformGizmoSubElements GetElementsForSelection(int32 ActorCount)
	{
		ETransformGizmoSubElements Elements = ETransformGizmoSubElements::TranslateAllAxes
			| ETransformGizmoSubElements::TranslateAllPlanes
			| ETransformGizmoSubElements::RotateAllAxes
			| ETransformGizmoSubElements::ScaleUniform;
		if (ActorCount == 1)
		{
			Elements |= ETransformGizmoSubElements::ScaleAllAxes | ETransformGizmoSubElements::ScaleAllPlanes;
		}
		return Elements;
	}
}

const FString UVTBOWTEditorGizmoManager::SelectionInstanceIdentifier(TEXT("VTBOWT.SelectionTransform"));

void UVTBOWTEditorGizmoManager::Initialize(IToolsContextQueriesAPI* InQueriesAPI,
	IToolsContextTransactionsAPI* InTransactionsAPI, UInputRouter* InInputRouter)
{
	Super::Initialize(InQueriesAPI, InTransactionsAPI, InInputRouter);
}

bool UVTBOWTEditorGizmoManager::EnsureSelectionGizmo()
{
	if (IsValid(SelectionGizmo))
	{
		return true;
	}
	if (!SelectionBuilder)
	{
		SelectionBuilder = NewObject<UVTBOWTEditorRepositionalGizmoBuilder>(this);
		RegisterGizmoType(UVTBOWTEditorRepositionalGizmoBuilder::BuilderIdentifier, SelectionBuilder);
	}
	SelectionGizmo = Cast<UVTBOWTEditorRepositionalGizmo>(CreateGizmo(
		UVTBOWTEditorRepositionalGizmoBuilder::BuilderIdentifier, SelectionInstanceIdentifier, this));
	if (IsValid(SelectionGizmo))
	{
		SelectionGizmo->SetVisibility(false);
		return true;
	}
	return false;
}

bool UVTBOWTEditorGizmoManager::SynchronizeSelection(
	const TOptional<FVTBOWTTransformSelection>& Request, TFunctionRef<bool()> PrepareChange)
{
	IToolsContextQueriesAPI* Queries = GetContextQueriesAPI();
	UWorld* EditingWorld = Queries ? Queries->GetCurrentEditingWorld() : nullptr;
	if (!IsValid(EditingWorld) || EditingWorld->bIsTearingDown)
	{
		return false;
	}
	TArray<AActor*> PreviousActors;
	GetSelection(PreviousActors);
	TArray<AActor*> DesiredActors = PreviousActors;
	TArray<TWeakObjectPtr<AActor>> RequestedActors;
	if (Request.IsSet())
	{
		DesiredActors.Reset();
		for (const TWeakObjectPtr<AActor>& WeakActor : Request->Actors)
		{
			AActor* Actor = WeakActor.Get();
			if (IsValid(Actor) && !Actor->IsActorBeingDestroyed() && Actor->GetWorld() == EditingWorld)
			{
				DesiredActors.AddUnique(Actor);
				RequestedActors.AddUnique(Actor);
			}
		}
	}
	const bool bHasGizmo = IsValid(SelectionGizmo);
	if (!bHasGizmo && DesiredActors.IsEmpty())
	{
		return true;
	}

	const bool bSelectionChanged = Request.IsSet() && (DesiredActors != PreviousActors
		|| (bHasGizmo && Request->FrameComponent.Get() != SelectionGizmo->GetSelectionFrame()));
	const bool bRefreshTargets = bHasGizmo && SelectionGizmo->IsSelectionStateStale();
	const bool bEnabled = Queries->GetCurrentTransformGizmoMode() != EToolContextTransformGizmoMode::NoGizmo;
	const ETransformGizmoSubElements DesiredElements = GetElementsForSelection(DesiredActors.Num());
	const bool bElementsChanged = bEnabled && bHasGizmo
		&& (!IsValid(SelectionGizmo->GetGizmoActor()) || SelectionGizmo->GetGizmoElements() != DesiredElements);
	const bool bTargetChanged = bHasGizmo && SelectionGizmo->ActiveTarget
		!= (bEnabled ? SelectionGizmo->GetTransformProxy() : nullptr);
	if (bHasGizmo && !bSelectionChanged && !bRefreshTargets && !bElementsChanged && !bTargetChanged)
	{
		return true;
	}

	if (!PrepareChange() || GetContextQueriesAPI() != Queries || !EnsureSelectionGizmo())
	{
		return false;
	}
	if (Request.IsSet())
	{
		SelectionGizmo->SetSelection(EditingWorld, RequestedActors, Request->FrameComponent.Get());
	}
	else
	{
		SelectionGizmo->RefreshSelection();
	}
	TArray<AActor*> SelectedActors;
	SelectionGizmo->GetSelectedActors(SelectedActors);
	UTransformProxy* Proxy = SelectionGizmo->GetTransformProxy();
	if (!bEnabled || !Proxy)
	{
		if (SelectionGizmo->ActiveTarget)
		{
			SelectionGizmo->ClearActiveTarget();
		}
		SelectionGizmo->SetVisibility(false);
		return true;
	}

	SelectionGizmo->SetEnabledElements(GetElementsForSelection(SelectedActors.Num()));
	if (!IsValid(SelectionGizmo->GetGizmoActor()))
	{
		return false;
	}
	const TWeakObjectPtr<UVTBOWTEditorRepositionalGizmo> WeakGizmo(SelectionGizmo);
	const bool bSingleActor = SelectedActors.Num() == 1;
	SelectionGizmo->SetIsNonUniformScaleAllowedFunction([WeakGizmo, bSingleActor]()
	{
		return bSingleActor && WeakGizmo.IsValid()
			&& WeakGizmo->CurrentCoordinateSystem == EToolContextCoordinateSystem::Local;
	});
	if (SelectionGizmo->ActiveTarget != Proxy)
	{
		SelectionGizmo->SetActiveTarget(Proxy, this);
	}
	return true;
}

void UVTBOWTEditorGizmoManager::GetSelection(TArray<AActor*>& OutActors) const
{
	if (IsValid(SelectionGizmo))
	{
		SelectionGizmo->GetSelectedActors(OutActors);
	}
	else
	{
		OutActors.Reset();
	}
}

void UVTBOWTEditorGizmoManager::UpdateSelectionVisibility(bool bHasView)
{
	if (IsValid(SelectionGizmo))
	{
		SelectionGizmo->SetVisibility(bHasView && SelectionGizmo->ActiveTarget != nullptr);
	}
}

void UVTBOWTEditorGizmoManager::Shutdown()
{
	TArray<UObject*> ManagedObjects;
	GetObjectsWithOuter(this, ManagedObjects);
	for (UObject* Object : ManagedObjects)
	{
		if (UCombinedTransformGizmo* Gizmo = Cast<UCombinedTransformGizmo>(Object);
			IsValid(Gizmo) && Gizmo->ActiveTarget)
		{
			Gizmo->ClearActiveTarget();
		}
	}
	Super::Shutdown();
	if (SelectionBuilder)
	{
		DeregisterGizmoType(UVTBOWTEditorRepositionalGizmoBuilder::BuilderIdentifier);
	}
	SelectionGizmo = nullptr;
	SelectionBuilder = nullptr;
}
