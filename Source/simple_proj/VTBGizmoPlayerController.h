// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VTBGizmoPlayerController.generated.h"

class UVTBTransformGizmoComponent;

UCLASS()
class SIMPLE_PROJ_API AVTBGizmoPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVTBGizmoPlayerController();

	UFUNCTION(BlueprintPure, Category = "VTB Transform Gizmo")
	UVTBTransformGizmoComponent* GetTransformGizmoComponent() const { return TransformGizmoComponent; }

	UFUNCTION(BlueprintCallable, Category = "VTB Transform Gizmo")
	bool SelectTransformTarget(AActor* TargetActor);

protected:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "VTB Transform Gizmo")
	TObjectPtr<UVTBTransformGizmoComponent> TransformGizmoComponent;
};
