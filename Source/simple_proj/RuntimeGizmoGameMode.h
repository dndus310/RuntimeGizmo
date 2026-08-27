// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RuntimeGizmoGameMode.generated.h"

UCLASS()
class SIMPLE_PROJ_API ARuntimeGizmoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARuntimeGizmoGameMode();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo")
	bool bSpawnDemoSphere = true;
};
