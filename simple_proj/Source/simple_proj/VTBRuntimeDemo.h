#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/HUD.h"
#include "VTBRuntimeDemo.generated.h"

UCLASS()
class AVTBRuntimeDemo : public AActor
{
	GENERATED_BODY()
public:
	AVTBRuntimeDemo();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
private:
	bool bPositionedCamera = false;
};

UCLASS()
class AVTBRuntimeDemoHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;
};
