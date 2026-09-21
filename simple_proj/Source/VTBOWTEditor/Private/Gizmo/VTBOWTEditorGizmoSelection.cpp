#include "Gizmo/VTBOWTEditorGizmoSelection.h"
#include "Gizmo/VTBOWTEditorActorTransform.h"

#include "BaseGizmos/TransformProxy.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

namespace
{
	bool IsSelectableActor(const AActor* Actor, const UWorld* TargetWorld)
	{
		if (!IsValid(TargetWorld) || !IsValid(Actor))
		{
			return false;
		}

		return !Actor->IsActorBeingDestroyed() && Actor->GetWorld() == TargetWorld;
	}
}

bool UVTBOWTEditorGizmoSelection::SetSelection(UWorld* InWorld, const TArray<TWeakObjectPtr<AActor>>& Actors,
	USceneComponent* FrameComponent)
{
	TargetWorld = InWorld;
	RequestedActors.Reset();
	TSet<AActor*> UniqueActors;
	for (const TWeakObjectPtr<AActor>& WeakActor : Actors)
	{
		AActor* Actor = WeakActor.Get();
		if (!IsSelectableActor(Actor, InWorld) || UniqueActors.Contains(Actor))
		{
			continue;
		}
		UniqueActors.Add(Actor);
		RequestedActors.Add(Actor);
	}
	RequestedFrameComponent = FrameComponent;
	if (!GetSelectionFrame())
	{
		RequestedFrameComponent.Reset();
	}

	return RebuildTargetIfNeeded();
}

USceneComponent* UVTBOWTEditorGizmoSelection::GetSelectionFrame() const
{
	USceneComponent* Frame = RequestedFrameComponent.Get();
	if (!Frame || !Frame->IsRegistered() || Frame->GetWorld() != TargetWorld.Get()
		|| !RequestedActors.Contains(Frame->GetOwner()))
	{
		return nullptr;
	}
	USceneComponent* Root = Frame->GetOwner()->GetRootComponent();
	return Frame == Root || Frame->IsAttachedTo(Root) ? Frame : nullptr;
}

bool UVTBOWTEditorGizmoSelection::RefreshSelection()
{
	RequestedActors.RemoveAll([this](const TWeakObjectPtr<AActor>& Actor)
	{
		return !IsSelectableActor(Actor.Get(), TargetWorld.Get());
	});
	if (!GetSelectionFrame())
	{
		RequestedFrameComponent.Reset();
	}
	return RebuildTargetIfNeeded();
}

bool UVTBOWTEditorGizmoSelection::IsSelectionStateStale() const
{
	for (const TWeakObjectPtr<AActor>& Actor : RequestedActors)
	{
		if (!IsSelectableActor(Actor.Get(), TargetWorld.Get()))
		{
			return true;
		}
	}
	if (!RequestedFrameComponent.IsExplicitlyNull() && !GetSelectionFrame())
	{
		return true;
	}
	TArray<USceneComponent*> Components;
	CollectTargetComponents(Components);
	return !IsTargetStateCurrent(Components);
}

void UVTBOWTEditorGizmoSelection::RebuildFromCurrentTransforms()
{
	RebuildTargetIfNeeded(true);
}

void UVTBOWTEditorGizmoSelection::GetSelectedActors(TArray<AActor*>& OutActors) const
{
	OutActors.Reset();
	for (const TWeakObjectPtr<AActor>& WeakActor : RequestedActors)
	{
		AActor* Actor = WeakActor.Get();
		if (IsSelectableActor(Actor, TargetWorld.Get()))
		{
			OutActors.Add(Actor);
		}
	}
}

UTransformProxy* UVTBOWTEditorGizmoSelection::GetTransformProxy() const
{
	return TransformProxy;
}

void UVTBOWTEditorGizmoSelection::Reset()
{
	if (TransformProxy)
	{
		TransformProxy->OnTransformChanged.RemoveAll(this);
		TransformProxy->OnPivotChanged.RemoveAll(this);
	}
	TransformProxy = nullptr;
	TargetWorld.Reset();
	RequestedActors.Reset();
	RequestedFrameComponent.Reset();
	Targets.Reset();
}

void UVTBOWTEditorGizmoSelection::CollectTargetComponents(TArray<USceneComponent*>& OutComponents) const
{
	OutComponents.Reset();
	TArray<USceneComponent*> Candidates;
	TSet<USceneComponent*> CandidateSet;
	for (const TWeakObjectPtr<AActor>& WeakActor : RequestedActors)
	{
		AActor* Actor = WeakActor.Get();
		if (!IsSelectableActor(Actor, TargetWorld.Get()))
		{
			continue;
		}

		USceneComponent* Root = Actor->GetRootComponent();
		if (IsValid(Root) && Root->IsRegistered()
			&& Root->Mobility == EComponentMobility::Movable
			&& !CandidateSet.Contains(Root))
		{
			Candidates.Add(Root);
			CandidateSet.Add(Root);
		}
	}

	for (USceneComponent* Candidate : Candidates)
	{
		bool bHasSelectedAncestor = false;
		for (USceneComponent* Parent = Candidate->GetAttachParent(); Parent; Parent = Parent->GetAttachParent())
		{
			if (CandidateSet.Contains(Parent))
			{
				bHasSelectedAncestor = true;
				break;
			}
		}
		if (!bHasSelectedAncestor)
		{
			OutComponents.Add(Candidate);
		}
	}
}

bool UVTBOWTEditorGizmoSelection::IsTargetStateCurrent(const TArray<USceneComponent*>& Components) const
{
	if (Components.Num() != Targets.Num())
	{
		return false;
	}
	for (const FTargetState& Target : Targets)
	{
		USceneComponent* Root = Target.RootComponent.Get();
		USceneComponent* Frame = Target.FrameComponent.Get();
		if (!Root || !Frame || !Components.Contains(Root) || GetFrameForRoot(Root) != Frame
			|| !Root->GetComponentTransform().Equals(Target.RootTransform)
			|| !Frame->GetComponentTransform().Equals(Target.FrameTransform))
		{
			return false;
		}
	}
	return true;
}

USceneComponent* UVTBOWTEditorGizmoSelection::GetFrameForRoot(USceneComponent* RootComponent) const
{
	USceneComponent* Frame = GetSelectionFrame();
	return Frame && (Frame == RootComponent || Frame->IsAttachedTo(RootComponent)) ? Frame : RootComponent;
}

bool UVTBOWTEditorGizmoSelection::RebuildTargetIfNeeded(bool bForceRebuild)
{
	TArray<USceneComponent*> Components;
	CollectTargetComponents(Components);
	if (!bForceRebuild && IsTargetStateCurrent(Components))
	{
		return false;
	}

	if (TransformProxy)
	{
		TransformProxy->OnTransformChanged.RemoveAll(this);
		TransformProxy->OnPivotChanged.RemoveAll(this);
	}
	TransformProxy = nullptr;
	Targets.Reset(Components.Num());
	if (!Components.IsEmpty())
	{
		TransformProxy = NewObject<UTransformProxy>(GetOuter(), NAME_None, RF_Transient);
		for (USceneComponent* Component : Components)
		{
			FTargetState& Target = Targets.AddDefaulted_GetRef();
			Target.RootComponent = Component;
			Target.FrameComponent = GetFrameForRoot(Component);
			UE::VTBOWTEditor::AddActorTransformTarget(*TransformProxy, Component, Target.FrameComponent.Get());
		}
		if (USceneComponent* Frame = GetSelectionFrame(); Frame && Components.Num() > 1)
		{
			TGuardValue<bool> PivotGuard(TransformProxy->bSetPivotMode, true);
			TransformProxy->SetTransform(Frame->GetComponentTransform());
		}
		TransformProxy->OnTransformChanged.AddUObject(this, &ThisClass::CacheTargetTransforms);
		TransformProxy->OnPivotChanged.AddUObject(this, &ThisClass::CacheTargetTransforms);
		CacheTargetTransforms(TransformProxy, TransformProxy->GetTransform());
	}
	return true;
}

void UVTBOWTEditorGizmoSelection::CacheTargetTransforms(UTransformProxy* Proxy, FTransform)
{
	if (Proxy != TransformProxy)
	{
		return;
	}

	for (FTargetState& Target : Targets)
	{
		USceneComponent* Root = Target.RootComponent.Get();
		USceneComponent* Frame = Target.FrameComponent.Get();
		Target.RootTransform = Root ? Root->GetComponentTransform() : FTransform::Identity;
		Target.FrameTransform = Frame ? Frame->GetComponentTransform() : FTransform::Identity;
	}
}
