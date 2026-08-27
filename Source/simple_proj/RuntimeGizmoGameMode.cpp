// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeGizmoGameMode.h"

#include "Engine/World.h"
#include "RuntimeGizmoPawn.h"
#include "RuntimeGizmoPlayerController.h"
#include "RuntimeTransformableSphereActor.h"

ARuntimeGizmoGameMode::ARuntimeGizmoGameMode()
{
	PlayerControllerClass = ARuntimeGizmoPlayerController::StaticClass();
	DefaultPawnClass = ARuntimeGizmoPawn::StaticClass();
}

void ARuntimeGizmoGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!bSpawnDemoSphere || !GetWorld())
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Name = TEXT("RuntimeTransformableSphere");
	GetWorld()->SpawnActor<ARuntimeTransformableSphereActor>(
		ARuntimeTransformableSphereActor::StaticClass(),
		FVector(0.0, 0.0, 100.0),
		FRotator::ZeroRotator,
		SpawnParameters);
}
