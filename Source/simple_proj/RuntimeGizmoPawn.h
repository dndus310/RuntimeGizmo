// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RuntimeGizmoPawn.generated.h"

class UCameraComponent;

UCLASS()
class SIMPLE_PROJ_API ARuntimeGizmoPawn : public APawn
{
	GENERATED_BODY()

public:
	ARuntimeGizmoPawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	void UpdateEditorViewportCamera(float DeltaSeconds);
	void SetFreeCameraActive(APlayerController* PlayerController, bool bActive);
	float GetEditorCameraSpeed(int32 SpeedSetting) const;
	float GetFlightCameraSpeed() const;
	void ChangeCameraSpeedByMouseWheel(float WheelDelta);
	void DollyCameraByMouseWheel(float WheelDelta);

	UPROPERTY(VisibleAnywhere, Category = "Runtime Transform Gizmo")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Runtime Transform Gizmo")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo")
	bool bUseDemoCameraPlacement = true;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Editor Viewport Camera")
	bool bEnableEditorViewportCamera = true;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Editor Viewport Camera")
	bool bHideCursorWhileLooking = true;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Editor Viewport Camera", meta = (ClampMin = "1", ClampMax = "8"))
	int32 CameraSpeedSetting = 4;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Editor Viewport Camera", meta = (ClampMin = "1", ClampMax = "8"))
	int32 MouseScrollCameraSpeedSetting = 4;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Editor Viewport Camera", meta = (ClampMin = "0.0001"))
	float CurrentCameraSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Editor Viewport Camera", meta = (ClampMin = "0.0001"))
	float MinCameraSpeed = 0.00001f;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Editor Viewport Camera", meta = (ClampMin = "1.0"))
	float MaxCameraSpeed = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Editor Viewport Camera", meta = (ClampMin = "1.0"))
	float CameraUnitsPerSecond = 1024.0f;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Editor Viewport Camera", meta = (ClampMin = "1.0"))
	float FastMoveMultiplier = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Editor Viewport Camera", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float SlowMoveMultiplier = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Editor Viewport Camera", meta = (ClampMin = "0.001"))
	float MouseLookSensitivity = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Editor Viewport Camera", meta = (ClampMin = "1.0"))
	float MouseWheelDollyScale = 32.0f;

	FVector2D StoredMousePosition = FVector2D::ZeroVector;
	bool bHasStoredMousePosition = false;
	bool bWasFreeCameraActive = false;
};
