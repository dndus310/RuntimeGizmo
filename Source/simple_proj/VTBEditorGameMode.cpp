#include "VTBEditorGameMode.h"

#include "VTBOWTEditGameState.h"
#include "VTBEditorSpectatorPawn.h"
#include "VTBOWTEditPlayerController.h"
#include "Components/SceneComponent.h"

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
	SetSelection(Actors, nullptr);
}

void AVTBEditorGameMode::SetSelectedComponent(USceneComponent* Component)
{
	AActor* Actor = IsValid(Component) ? Component->GetOwner() : nullptr;
	SetSelection({Actor}, Component);
}

void AVTBEditorGameMode::SetSelection(const TArray<AActor*>& Actors, USceneComponent* FrameComponent)
{
	TArray<TWeakObjectPtr<AActor>> NewSelection;
	for (AActor* Actor : Actors)
	{
		if (IsValidSelectionActor(Actor, GetWorld()))
		{
			NewSelection.AddUnique(Actor);
		}
	}

	USceneComponent* NewFrame = IsValid(FrameComponent) && NewSelection.Contains(FrameComponent->GetOwner())
		? FrameComponent : nullptr;
	if (SelectedActors == NewSelection && SelectedFrameComponent.Get() == NewFrame)
	{
		return;
	}

	SelectedActors = MoveTemp(NewSelection);
	SelectedFrameComponent = NewFrame;
	SelectionChanged.Broadcast();
}

void AVTBEditorGameMode::ClearSelection()
{
	SetSelectedActors({});
}

void AVTBEditorGameMode::GetSelection(TArray<TWeakObjectPtr<AActor>>& OutActors) const
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

FVTBOWTSelectionChanged& AVTBEditorGameMode::OnSelectionChanged()
{
	return SelectionChanged;
}

USceneComponent* AVTBEditorGameMode::GetSelectionFrame() const
{
	return SelectedFrameComponent.Get();
}

bool AVTBEditorGameMode::ApplySelectionChange(const FSelectedObjectsChangeList& Change)
{
	TArray<AActor*> Actors;
	USceneComponent* FrameComponent = nullptr;
	if (Change.ModificationType == ESelectedObjectsModificationType::Add
		|| Change.ModificationType == ESelectedObjectsModificationType::Remove)
	{
		FrameComponent = SelectedFrameComponent.Get();
		for (const TWeakObjectPtr<AActor>& Actor : SelectedActors)
		{
			Actors.Add(Actor.Get());
		}
	}
	else if (Change.ModificationType != ESelectedObjectsModificationType::Replace
		&& Change.ModificationType != ESelectedObjectsModificationType::Clear)
	{
		return false;
	}
	if (Change.ModificationType != ESelectedObjectsModificationType::Clear)
	{
		TArray<AActor*> RequestedActors = Change.Actors;
		for (UActorComponent* Component : Change.Components)
		{
			if (IsValid(Component))
			{
				AActor* Actor = Component->GetOwner();
				RequestedActors.Add(Actor);
				if (Change.ModificationType != ESelectedObjectsModificationType::Remove
					&& IsValidSelectionActor(Actor, GetWorld()))
				{
					if (USceneComponent* SceneComponent = Cast<USceneComponent>(Component))
					{
						FrameComponent = SceneComponent;
					}
				}
			}
		}
		for (AActor* Actor : RequestedActors)
		{
			if (Change.ModificationType == ESelectedObjectsModificationType::Remove)
			{
				Actors.Remove(Actor);
			}
			else
			{
				Actors.Add(Actor);
			}
		}
	}
	SetSelection(Actors, FrameComponent);
	return true;
}
