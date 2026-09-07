#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "InputActionValue.h"
#include "InputState.h"
#include "ToolContextInterfaces.h"
#include "VTBEditorSpectatorPawn.generated.h"

class UInputAction;
class UInputMappingContext;
class UVTBEditorInteractiveToolsContext;

UCLASS()
class SIMPLE_PROJ_API AVTBEditorSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	AVTBEditorSpectatorPawn();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void StartInput();
	void StopInput();
	void UpdatePointer();
	void UpdateCameraInputLock();
	void ResetCameraInputLock();
	void SendPointer(bool bPressed, bool bDown, bool bReleased);
	void SelectActor();
	void SendSelection(AActor* Actor);
	void SetGizmoMode(EToolContextTransformGizmoMode Mode);
	void MoveCamera(const FVector& Axis);
	void LookCamera(const FVector2D& Axis);
	bool IsCameraNavigating() const;
	bool IsGizmoCapturingMouse() const;

	void OnSelectStarted();
	void OnSelectCompleted();
	void OnTranslationStarted();
	void OnRotationStarted();
	void OnScaleStarted();
	void OnGizmoModeStarted();
	void OnSelectCancelStarted();
	void OnSpaceStarted();
	void OnCameraMoveTriggered(const FInputActionValue& Value);
	void OnCameraLookTriggered(const FInputActionValue& Value);

	APlayerController* GetPlayerController() const;
	UVTBEditorInteractiveToolsContext* GetToolsContext() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "VTB Editor|Input")
	TSoftObjectPtr<UInputMappingContext> EditorMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "VTB Editor|Input")
	TSoftObjectPtr<UInputAction> EditSelectAction;

	UPROPERTY(EditDefaultsOnly, Category = "VTB Editor|Input")
	TSoftObjectPtr<UInputAction> EditTranslationAction;

	UPROPERTY(EditDefaultsOnly, Category = "VTB Editor|Input")
	TSoftObjectPtr<UInputAction> EditRotationAction;

	UPROPERTY(EditDefaultsOnly, Category = "VTB Editor|Input")
	TSoftObjectPtr<UInputAction> EditScaleAction;

	UPROPERTY(EditDefaultsOnly, Category = "VTB Editor|Input")
	TSoftObjectPtr<UInputAction> ChagneGizmoModeAction;

	UPROPERTY(EditDefaultsOnly, Category = "VTB Editor|Input")
	TSoftObjectPtr<UInputAction> EditSelectCancelAction;

	UPROPERTY(EditDefaultsOnly, Category = "VTB Editor|Input")
	TSoftObjectPtr<UInputAction> EditSpaceAction;

	UPROPERTY(EditDefaultsOnly, Category = "VTB Editor|Input")
	TSoftObjectPtr<UInputAction> CameraMoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "VTB Editor|Input")
	TSoftObjectPtr<UInputAction> CameraLookAction;

	UPROPERTY(Transient)
	TWeakObjectPtr<APlayerController> CachedPlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "VTB Editor|Input")
	int32 EditorInputPriority;

	FInputDeviceState PointerInput;
	EToolContextTransformGizmoMode CurrentGizmoMode;
	EToolContextCoordinateSystem CurrentCoordinateSystem;
	bool bMoveInputIgnored;
	bool bLookInputIgnored;
};
