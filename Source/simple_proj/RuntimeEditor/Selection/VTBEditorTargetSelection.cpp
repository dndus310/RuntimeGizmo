#include "RuntimeEditor/Selection/VTBEditorTargetSelection.h"

#include "BaseGizmos/TransformProxy.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

namespace
{
	bool IsSelectableActor(const AActor* Actor, const UWorld* World)
	{
		return IsValid(World) && IsValid(Actor) && !Actor->IsActorBeingDestroyed() && Actor->GetWorld() == World;
	}
}

bool UVTBEditorTargetSelection::SetSelection(UWorld* World, const TArray<TWeakObjectPtr<AActor>>& Actors)
{
	TargetWorld = World;
	RequestedActors.Reset();
	TSet<AActor*> UniqueActors;
	for (const TWeakObjectPtr<AActor>& WeakActor : Actors)
	{
		AActor* Actor = WeakActor.Get();
		if (IsSelectableActor(Actor, World) && !UniqueActors.Contains(Actor))
		{
			UniqueActors.Add(Actor);
			RequestedActors.Add(Actor);
		}
	}

	return RebuildTargetIfNeeded();
}

bool UVTBEditorTargetSelection::RefreshSelection()
{
	RequestedActors.RemoveAll([this](const TWeakObjectPtr<AActor>& Actor)
	{
		return !IsSelectableActor(Actor.Get(), TargetWorld.Get());
	});
	return RebuildTargetIfNeeded();
}

void UVTBEditorTargetSelection::RebuildFromCurrentTransforms()
{
	RebuildTargetIfNeeded(true);
}

void UVTBEditorTargetSelection::GetSelectedActors(TArray<AActor*>& OutActors) const
{
	OutActors.Reset();
	for (const TWeakObjectPtr<AActor>& WeakActor : RequestedActors)
	{
		if (AActor* Actor = WeakActor.Get(); IsSelectableActor(Actor, TargetWorld.Get()))
		{
			OutActors.Add(Actor);
		}
	}
}

bool UVTBEditorTargetSelection::RebuildTargetIfNeeded(bool bForceRebuild)
{
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
		if (IsValid(Root) && Root->IsRegistered() && Root->Mobility == EComponentMobility::Movable
			&& !CandidateSet.Contains(Root))
		{
			Candidates.Add(Root);
			CandidateSet.Add(Root);
		}
	}

	TArray<USceneComponent*> Components;
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
		// An attached child already follows its selected ancestor. Adding both transforms it twice.
		if (!bHasSelectedAncestor)
		{
			Components.Add(Candidate);
		}
	}

	bool bSameTargets = Components.Num() == TargetComponents.Num();
	for (const TWeakObjectPtr<USceneComponent>& Previous : TargetComponents)
	{
		bSameTargets &= Previous.IsValid() && Components.Contains(Previous.Get());
	}
	if (bSameTargets && !bForceRebuild)
	{
		return false;
	}

	TransformProxy = nullptr;
	TargetComponents.Reset(Components.Num());
	if (!Components.IsEmpty())
	{
		TransformProxy = NewObject<UTransformProxy>(this, NAME_None, RF_Transient);
		for (USceneComponent* Component : Components)
		{
			TargetComponents.Add(Component);
			// Runtime history is supplied by the context; editor UObject::Modify is unnecessary.
			TransformProxy->AddComponent(Component, false);
		}
	}
	return true;
}
