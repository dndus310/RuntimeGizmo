// Fill out your copyright notice in the Description page of Project Settings.


#include "VTBEditorGameMode.h"

#include "VTBOWTEditGameState.h"
#include "VTBEditorSpectatorPawn.h"
#include "VTBOWTEditPlayerController.h"

namespace
{
	bool IsValidSelectionActor(const AActor* Actor, const UWorld* World)
	{
		if (!IsValid(Actor) || !IsValid(World))
		{
			return false;
		}

		return !Actor->IsActorBeingDestroyed() && Actor->GetWorld() == World;
	}
}

AVTBEditorGameMode::AVTBEditorGameMode()
{
	DefaultPawnClass = AVTBEditorSpectatorPawn::StaticClass();
	GameStateClass = AVTBOWTEditGameState::StaticClass();
	PlayerControllerClass = AVTBOWTEditPlayerController::StaticClass();
}

void AVTBEditorGameMode::InitGameState()
{
	Super::InitGameState();

	if (AVTBOWTEditGameState* EditorGameState = GetGameState<AVTBOWTEditGameState>())
	{
		EditorGameState->UpdateEditorState(bEditMode);
		EditorGameState->UpdateActiveGizmoMode(ActiveGizmoMode);
	}
}

void AVTBEditorGameMode::SetEditorState_Implementation(bool bNewState)
{
	bEditMode = bNewState;
	if (AVTBOWTEditGameState* EditorGameState = GetGameState<AVTBOWTEditGameState>())
	{
		EditorGameState->UpdateEditorState(bEditMode);
	}
}

void AVTBEditorGameMode::SetActiveGizmoMode_Implementation(EActiveGizmoMode NewGizmoMode)
{
	if (NewGizmoMode != EActiveGizmoMode::Transform && NewGizmoMode != EActiveGizmoMode::Spline)
	{
		return;
	}

	ActiveGizmoMode = NewGizmoMode;
	if (AVTBOWTEditGameState* EditorGameState = GetGameState<AVTBOWTEditGameState>())
	{
		EditorGameState->UpdateActiveGizmoMode(ActiveGizmoMode);
	}
}

void AVTBEditorGameMode::SetSelectedActors(const TArray<AActor*>& Actors)
{
	TArray<TWeakObjectPtr<AActor>> NewSelection;
	for (AActor* Actor : Actors)
	{
		if (IsValidSelectionActor(Actor, GetWorld()))
		{
			NewSelection.AddUnique(Actor);
		}
	}

	if (SelectedActors == NewSelection)
	{
		return;
	}

	SelectedActors = MoveTemp(NewSelection);
	SelectionChanged.Broadcast();
}

void AVTBEditorGameMode::ClearSelection()
{
	SetSelectedActors({});
}

void AVTBEditorGameMode::GetSelectionSnapshot(TArray<TWeakObjectPtr<AActor>>& OutActors) const
{
	OutActors.Reset(SelectedActors.Num());
	for (const TWeakObjectPtr<AActor>& Actor : SelectedActors)
	{
		if (IsValidSelectionActor(Actor.Get(), GetWorld()))
		{
			OutActors.Add(Actor);
		}
	}
}

FVTBSelectionChanged& AVTBEditorGameMode::OnSelectionChanged()
{
	return SelectionChanged;
}
