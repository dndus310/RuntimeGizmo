#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "StructUtils/InstancedStruct.h"
#include "VTBOWTSpectator.generated.h"

class UEnhancedInputComponent;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
struct FInputActionValue;

UCLASS(Blueprintable)
class VTBOWTEDITOR_API AVTBOWTSpectator : public ASpectatorPawn
{
	GENERATED_BODY()

public:
	AVTBOWTSpectator();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PawnClientRestart() override;
	virtual void NotifyControllerChanged() override;
	virtual void UnPossessed() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "OWT|Editing")
	bool SendEditContext(const FInstancedStruct& Context);

	// Trace is an input-side responsibility. Only its actor result reaches editing code.
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "OWT|Selection")
	AActor* TraceSelectableObject();

	UFUNCTION(BlueprintPure, Category = "OWT|Camera")
	bool IsCameraNavigating() const;

protected:
	virtual UInputComponent* CreatePlayerInputComponent() override;
	virtual void DestroyPlayerInputComponent() override;

private:
	void RefreshInputMapping();
	void UpdatePointer(bool bPressed, bool bDown, bool bReleased);
	void StopCameraNavigation();
	void RemoveInputMapping();
	void OnCameraNavigationStarted();
	void OnCameraMove(const FInputActionValue& Value);
	void OnCameraLook(const FInputActionValue& Value);
	bool CanNavigateCamera() const;
	bool HasGizmoCapture() const;
	UObject* ResolveEditContextReceiver() const;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UEnhancedInputComponent> EnhancedInput;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OWT|Camera")
	TObjectPtr<UInputAction> CameraNavigateAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OWT|Camera")
	TObjectPtr<UInputAction> CameraMoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OWT|Camera")
	TObjectPtr<UInputAction> CameraLookAction;

	// Optional replacement receiver; defaults to the world editing subsystem.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OWT|Editing",
	          meta = (MustImplement = "/Script/VTBOWTEditor.OWTEditContextReceiver"))
	TObjectPtr<UObject> EditContextReceiver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OWT|Selection")
	TEnumAsByte<ECollisionChannel> SelectionTraceChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OWT|Selection")
	float SelectionTraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OWT|Camera", meta = (ClampMin = "0.01"))
	float CameraLookSensitivity;

private:
	TWeakObjectPtr<UEnhancedInputLocalPlayerSubsystem> MappingSubsystem;
	bool bPointerDown;
	bool bSelectionConsumed;
	bool bCameraNavigating;
};
