// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VTBTransformGizmoTypes.h"
#include "VTBTransformGizmoComponent.generated.h"

class AActor;
class USceneComponent;
class UVTBInteractiveToolsSubsystem;

UCLASS(ClassGroup = (VTBGizmo), meta = (BlueprintSpawnableComponent))
class SIMPLE_PROJ_API UVTBTransformGizmoComponent final : public UActorComponent
{
	GENERATED_BODY()

public:
	UVTBTransformGizmoComponent();

	UFUNCTION(BlueprintCallable, Category = "VTB Transform Gizmo")
	bool SelectTransformTarget(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "VTB Transform Gizmo")
	void ClearSelection();

	UFUNCTION(BlueprintCallable, Category = "VTB Transform Gizmo")
	void ApplySettings(const FVTBTransformGizmoSettings& InSettings);

	UFUNCTION(BlueprintPure, Category = "VTB Transform Gizmo")
	FVTBTransformGizmoSettings GetSettings() const { return Settings; }

	UFUNCTION(BlueprintCallable, Category = "VTB Transform Gizmo")
	void SetTransformMode(EToolContextTransformGizmoMode NewMode);

	UFUNCTION(BlueprintCallable, Category = "VTB Transform Gizmo")
	void SetTranslationMode();

	UFUNCTION(BlueprintCallable, Category = "VTB Transform Gizmo")
	void SetRotationMode();

	UFUNCTION(BlueprintCallable, Category = "VTB Transform Gizmo")
	void SetScaleMode();

	UFUNCTION(BlueprintCallable, Category = "VTB Transform Gizmo")
	void SetCoordinateSystem(EToolContextCoordinateSystem NewCoordinateSystem);

	UFUNCTION(BlueprintCallable, Category = "VTB Transform Gizmo")
	void ToggleCoordinateSystem();

	UFUNCTION(BlueprintPure, Category = "VTB Transform Gizmo")
	bool IsGizmoInteracting() const;

	UFUNCTION(BlueprintPure, Category = "VTB Transform Gizmo")
	bool ShouldBlockViewportCameraInput() const;

	UFUNCTION(BlueprintPure, Category = "VTB Transform Gizmo")
	AActor* GetSelectedActor() const;

	UFUNCTION(BlueprintPure, Category = "VTB Transform Gizmo")
	USceneComponent* GetSelectedComponent() const;

	UFUNCTION(BlueprintPure, Category = "VTB Transform Gizmo")
	UVTBInteractiveToolsSubsystem* GetSubsystem() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditAnywhere, Category = "VTB Transform Gizmo")
	FVTBTransformGizmoSettings Settings;
};
