// Fill out your copyright notice in the Description page of Project Settings.


#include "VTBEditorGameMode.h"

#include "VTBEditorSpectatorPawn.h"

AVTBEditorGameMode::AVTBEditorGameMode()
{
	DefaultPawnClass = AVTBEditorSpectatorPawn::StaticClass();
}

void AVTBEditorGameMode::SetSelectedActors(const TArray<AActor*>& Actors)
{
	TArray<TWeakObjectPtr<AActor>> NewSelection;
	TSet<AActor*> UniqueActors;
	for (AActor* Actor : Actors)
	{
		if (IsValid(Actor) && !Actor->IsActorBeingDestroyed() && Actor->GetWorld() == GetWorld()
			&& !UniqueActors.Contains(Actor))
		{
			UniqueActors.Add(Actor);
			NewSelection.Add(Actor);
		}
	}
	if (SelectedActors != NewSelection)
	{
		SelectedActors = MoveTemp(NewSelection);
		SelectionChanged.Broadcast();
	}
}

void AVTBEditorGameMode::ClearSelection()
{
	SetSelectedActors(TArray<AActor*>());
}

void AVTBEditorGameMode::GetSelectionSnapshot(TArray<TWeakObjectPtr<AActor>>& OutActors) const
{
	OutActors.Reset();
	for (const TWeakObjectPtr<AActor>& WeakActor : SelectedActors)
	{
		if (AActor* Actor = WeakActor.Get(); IsValid(Actor) && !Actor->IsActorBeingDestroyed())
		{
			OutActors.Add(Actor);
		}
	}
}
