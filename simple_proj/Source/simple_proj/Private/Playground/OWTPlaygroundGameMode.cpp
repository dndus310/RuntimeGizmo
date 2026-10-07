#include "Playground/OWTPlaygroundGameMode.h"

#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Playground/OWTPlaygroundHUD.h"
#include "TimerManager.h"
#include "VTBOWTEditorSubsystem.h"

AOWTPlaygroundGameMode::AOWTPlaygroundGameMode() : bPlaygroundReady(false)
{
	HUDClass = AOWTPlaygroundHUD::StaticClass();
}

void AOWTPlaygroundGameMode::BeginPlay()
{
	Super::BeginPlay();
	// Select after all authored actors have completed their initial BeginPlay setup.
	GetWorldTimerManager().SetTimerForNextTick(this, &ThisClass::InitializePlayground);
}

void AOWTPlaygroundGameMode::InitializePlayground()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	if (World->bIsTearingDown)
	{
		return;
	}
	UVTBOWTEditorSubsystem* Editor = World->GetSubsystem<UVTBOWTEditorSubsystem>();
	if (!Editor)
	{
		return;
	}
	if (!Editor->GetAttributeEditor())
	{
		return;
	}
	if (!Editor->IsEditingEnabled())
	{
		Editor->ToggleEditing();
	}
	if (!Editor->IsEditingEnabled())
	{
		return;
	}
	if (AActor* InitialSelection = FindInitialSelection())
	{
		Editor->SetSelectedObject(InitialSelection);
	}
	bPlaygroundReady = true;
}

AActor* AOWTPlaygroundGameMode::FindInitialSelection() const
{
	AActor* Fallback = nullptr;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor->IsActorBeingDestroyed())
		{
			continue;
		}
		const USceneComponent* Root = Actor->GetRootComponent();
		if (!Root)
		{
			continue;
		}
		if (Root->Mobility != EComponentMobility::Movable)
		{
			continue;
		}
		if (Actor->ActorHasTag(TEXT("OWT.Playground.InitialSelection")))
		{
			return Actor;
		}
		if (!Fallback)
		{
			if (Actor->ActorHasTag(TEXT("OWT.Playground.Editable")))
			{
				Fallback = Actor;
			}
		}
	}
	return Fallback;
}

bool AOWTPlaygroundGameMode::IsPlaygroundReady() const
{
	return bPlaygroundReady;
}
