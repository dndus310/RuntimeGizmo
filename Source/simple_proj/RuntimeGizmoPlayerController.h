// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RuntimeGizmoPlayerController.generated.h"

class URuntimeTransformGizmoComponent;

UCLASS()
class SIMPLE_PROJ_API ARuntimeGizmoPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARuntimeGizmoPlayerController();

	UFUNCTION(BlueprintPure, Category = "Runtime Transform Gizmo")
	URuntimeTransformGizmoComponent* GetRuntimeTransformGizmoComponent() const { return RuntimeTransformGizmoComponent; }

	UFUNCTION(BlueprintCallable, Category = "Runtime Transform Gizmo")
	void SelectTransformableActor(AActor* Actor);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void HandleTranslateMode();
	void HandleRotateMode();
	void HandleScaleMode();
	void HandleToggleCoordinateSystem();

	UPROPERTY(VisibleAnywhere, Category = "Runtime Transform Gizmo")
	TObjectPtr<URuntimeTransformGizmoComponent> RuntimeTransformGizmoComponent;
};
