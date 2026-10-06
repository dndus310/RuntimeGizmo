#include "VTBOWTEditorGameMode.h"

#include "Engine/World.h"
#include "EngineUtils.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorPlayerController.h"
#include "VTBOWTEditorGameState.h"
#include "VTBOWTSpectator.h"
#include "VTBOWTEditorSubsystem.h"

AVTBOWTEditorGameMode::AVTBOWTEditorGameMode() : AttributeEditor(nullptr)
{
	PlayerControllerClass = AVTBOWTEditorPlayerController::StaticClass();
	GameStateClass = AVTBOWTEditorGameState::StaticClass();
	SpectatorClass = AVTBOWTSpectator::StaticClass();
	// Use the editing spectator as the possessed pawn during normal Play as well.
	DefaultPawnClass = SpectatorClass;
}

AVTBAttributeEditor* AVTBOWTEditorGameMode::GetAttributeEditor_Implementation()
{
	check(IsInGameThread());

	if (IsValid(AttributeEditor.Get()))
	{
		return AttributeEditor.Get();
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}
	if (World->bIsTearingDown)
	{
		return nullptr;
	}

	AttributeEditor = FindAttributeEditor(*World);
	if (!AttributeEditor)
	{
		AttributeEditor = SpawnAttributeEditor(*World);
	}

	return AttributeEditor.Get();
}

void AVTBOWTEditorGameMode::BeginPlay()
{
	Super::BeginPlay();
	AVTBAttributeEditor* Editor = IVTBOWTEditorGameModeProvider::Execute_GetAttributeEditor(this);
	if (Editor)
	{
		Editor->BindSubsystem(GetWorld()->GetSubsystem<UVTBOWTEditorSubsystem>());
	}
}

AVTBAttributeEditor* AVTBOWTEditorGameMode::SpawnAttributeEditor(UWorld& World)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AVTBAttributeEditor* SpawnedEditor = World.SpawnActor<AVTBAttributeEditor>(AVTBAttributeEditor::StaticClass(),
	                                                                           FTransform::Identity, SpawnParameters);
	if (!ensureMsgf(SpawnedEditor, TEXT("Failed to spawn AttributeEditor in %s."), *World.GetName()))
	{
		return nullptr;
	}

	return SpawnedEditor;
}

AVTBAttributeEditor* AVTBOWTEditorGameMode::FindAttributeEditor(UWorld& World) const
{
	for (TActorIterator<AVTBAttributeEditor> It(&World); It; ++It)
	{
		AVTBAttributeEditor* Candidate = *It;
		if (!IsValid(Candidate))
		{
			continue;
		}

		return Candidate;
	}

	return nullptr;
}
