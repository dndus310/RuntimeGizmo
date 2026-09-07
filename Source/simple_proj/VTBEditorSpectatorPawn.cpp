#include "VTBEditorSpectatorPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "RuntimeEditor/Context/VTBEditorInteractiveToolsContext.h"
#include "RuntimeEditor/VTBEditorSubsystem.h"
#include "VTBEditorGameMode.h"

#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "InputRouter.h"

AVTBEditorSpectatorPawn::AVTBEditorSpectatorPawn()
	: EditorMappingContext(FSoftObjectPath(TEXT("/Game/RuntimeEditor/Input/IMC_VTBEditor.IMC_VTBEditor")))
	, EditSelectAction(FSoftObjectPath(TEXT("/Game/RuntimeEditor/Input/IA_EditSelect.IA_EditSelect")))
	, EditTranslationAction(FSoftObjectPath(TEXT("/Game/RuntimeEditor/Input/IA_EditTranslation.IA_EditTranslation")))
	, EditRotationAction(FSoftObjectPath(TEXT("/Game/RuntimeEditor/Input/IA_EditRotation.IA_EditRotation")))
	, EditScaleAction(FSoftObjectPath(TEXT("/Game/RuntimeEditor/Input/IA_EditScale.IA_EditScale")))
	, ChagneGizmoModeAction(FSoftObjectPath(TEXT("/Game/RuntimeEditor/Input/IA_ChagneGizmoMode.IA_ChagneGizmoMode")))
	, EditSelectCancelAction(FSoftObjectPath(TEXT("/Game/RuntimeEditor/Input/IA_EditSelectCancel.IA_EditSelectCancel")))
	, EditSpaceAction(FSoftObjectPath(TEXT("/Game/RuntimeEditor/Input/IA_EditSpace.IA_EditSpace")))
	, CameraMoveAction(FSoftObjectPath(TEXT("/Game/RuntimeEditor/Input/IA_CameraMove.IA_CameraMove")))
	, CameraLookAction(FSoftObjectPath(TEXT("/Game/RuntimeEditor/Input/IA_CameraLook.IA_CameraLook")))
	, EditorInputPriority(100)
	, CurrentGizmoMode(EToolContextTransformGizmoMode::Translation)
	, CurrentCoordinateSystem(EToolContextCoordinateSystem::World)
	, bMoveInputIgnored(false)
	, bLookInputIgnored(false)
{
	PrimaryActorTick.bCanEverTick = true;
	bAddDefaultMovementBindings = false;
}

void AVTBEditorSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
	StartInput();
	SetGizmoMode(CurrentGizmoMode);
	if (UVTBEditorInteractiveToolsContext* Context = GetToolsContext())
	{
		Context->SetCoordinateSystem(CurrentCoordinateSystem);
	}
}

void AVTBEditorSpectatorPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopInput();
	Super::EndPlay(EndPlayReason);
}

void AVTBEditorSpectatorPawn::Tick(float DeltaTime)
{
	UpdatePointer();
	UpdateCameraInputLock();
	if (UVTBEditorInteractiveToolsContext* Context = GetToolsContext())
	{
		if (Context->InputRouter->HasActiveMouseCapture())
		{
			SendPointer(false, true, false);
			return;
		}
		Context->InputRouter->PostHoverInputEvent(PointerInput);
	}
	Super::Tick(DeltaTime);
}

void AVTBEditorSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	StartInput();

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput)
	{
		return;
	}

	if (const UInputAction* Action = EditSelectAction.LoadSynchronous())
	{
		EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ThisClass::OnSelectStarted);
		EnhancedInput->BindAction(Action, ETriggerEvent::Completed, this, &ThisClass::OnSelectCompleted);
	}
	if (const UInputAction* Action = EditTranslationAction.LoadSynchronous())
	{
		EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ThisClass::OnTranslationStarted);
	}
	if (const UInputAction* Action = EditRotationAction.LoadSynchronous())
	{
		EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ThisClass::OnRotationStarted);
	}
	if (const UInputAction* Action = EditScaleAction.LoadSynchronous())
	{
		EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ThisClass::OnScaleStarted);
	}
	if (const UInputAction* Action = ChagneGizmoModeAction.LoadSynchronous())
	{
		EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ThisClass::OnGizmoModeStarted);
	}
	if (const UInputAction* Action = EditSelectCancelAction.LoadSynchronous())
	{
		EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ThisClass::OnSelectCancelStarted);
	}
	if (const UInputAction* Action = EditSpaceAction.LoadSynchronous())
	{
		EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &ThisClass::OnSpaceStarted);
	}
	if (const UInputAction* Action = CameraMoveAction.LoadSynchronous())
	{
		EnhancedInput->BindAction(Action, ETriggerEvent::Triggered, this, &ThisClass::OnCameraMoveTriggered);
	}
	if (const UInputAction* Action = CameraLookAction.LoadSynchronous())
	{
		EnhancedInput->BindAction(Action, ETriggerEvent::Triggered, this, &ThisClass::OnCameraLookTriggered);
	}
}

void AVTBEditorSpectatorPawn::MoveCamera(const FVector& Axis)
{
	if (Axis.IsNearlyZero())
	{
		return;
	}
	if (!IsCameraNavigating() || IsGizmoCapturingMouse())
	{
		return;
	}

	AController* CameraController = GetController();
	if (!CameraController)
	{
		return;
	}

	const FRotationMatrix ControlSpace(CameraController->GetControlRotation());
	AddMovementInput(ControlSpace.GetScaledAxis(EAxis::X), Axis.X);
	AddMovementInput(ControlSpace.GetScaledAxis(EAxis::Y), Axis.Y);
	AddMovementInput(FVector::UpVector, Axis.Z);
}

void AVTBEditorSpectatorPawn::LookCamera(const FVector2D& Axis)
{
	if (Axis.IsNearlyZero())
	{
		return;
	}
	if (!IsCameraNavigating() || IsGizmoCapturingMouse())
	{
		return;
	}

	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(-Axis.Y);
}

void AVTBEditorSpectatorPawn::StartInput()
{
	APlayerController* PlayerController = GetPlayerController();
	if (!PlayerController)
	{
		return;
	}
	if (CachedPlayerController.Get() == PlayerController)
	{
		return;
	}
	if (CachedPlayerController.IsValid())
	{
		StopInput();
	}

	CachedPlayerController = PlayerController;
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer ? LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() : nullptr;
	if (const UInputMappingContext* MappingContext = EditorMappingContext.LoadSynchronous())
	{
		if (InputSubsystem)
		{
			InputSubsystem->AddMappingContext(MappingContext, EditorInputPriority);
		}
	}
}

void AVTBEditorSpectatorPawn::StopInput()
{
	ResetCameraInputLock();

	APlayerController* PlayerController = CachedPlayerController.Get();
	ULocalPlayer* LocalPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr;
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer ? LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() : nullptr;
	if (const UInputMappingContext* MappingContext = EditorMappingContext.LoadSynchronous())
	{
		if (InputSubsystem)
		{
			InputSubsystem->RemoveMappingContext(MappingContext);
		}
	}
	CachedPlayerController.Reset();
}

void AVTBEditorSpectatorPawn::UpdateCameraInputLock()
{
	APlayerController* PlayerController = GetPlayerController();
	if (!PlayerController)
	{
		return;
	}

	const bool bBlockMove = IsGizmoCapturingMouse() || PointerInput.Mouse.Left.bDown;
	const bool bBlockLook = bBlockMove;
	if (bMoveInputIgnored != bBlockMove)
	{
		PlayerController->SetIgnoreMoveInput(bBlockMove);
		bMoveInputIgnored = bBlockMove;
	}
	if (bLookInputIgnored != bBlockLook)
	{
		PlayerController->SetIgnoreLookInput(bBlockLook);
		bLookInputIgnored = bBlockLook;
	}
}

void AVTBEditorSpectatorPawn::ResetCameraInputLock()
{
	APlayerController* PlayerController = CachedPlayerController.Get();
	if (PlayerController && bMoveInputIgnored)
	{
		PlayerController->SetIgnoreMoveInput(false);
	}
	if (PlayerController && bLookInputIgnored)
	{
		PlayerController->SetIgnoreLookInput(false);
	}
	bMoveInputIgnored = false;
	bLookInputIgnored = false;
}

void AVTBEditorSpectatorPawn::UpdatePointer()
{
	APlayerController* PlayerController = GetPlayerController();
	if (!PlayerController)
	{
		return;
	}

	float MouseX = 0.0f;
	float MouseY = 0.0f;
	if (PlayerController->GetMousePosition(MouseX, MouseY))
	{
		const FVector2D Position(MouseX, MouseY);
		PointerInput.Mouse.Delta2D = Position - PointerInput.Mouse.Position2D;
		PointerInput.Mouse.Position2D = Position;
	}

	FVector RayOrigin;
	FVector RayDirection;
	if (PlayerController->DeprojectMousePositionToWorld(RayOrigin, RayDirection))
	{
		PointerInput.Mouse.WorldRay = FRay(RayOrigin, RayDirection.GetSafeNormal());
	}

	PointerInput.InputDevice = EInputDevices::Mouse;
	PointerInput.SetModifierKeyStates(
		PlayerController->IsInputKeyDown(EKeys::LeftShift) || PlayerController->IsInputKeyDown(EKeys::RightShift),
		PlayerController->IsInputKeyDown(EKeys::LeftAlt) || PlayerController->IsInputKeyDown(EKeys::RightAlt),
		PlayerController->IsInputKeyDown(EKeys::LeftControl) || PlayerController->IsInputKeyDown(EKeys::RightControl),
		PlayerController->IsInputKeyDown(EKeys::LeftCommand) || PlayerController->IsInputKeyDown(EKeys::RightCommand));
}

void AVTBEditorSpectatorPawn::SendPointer(bool bPressed, bool bDown, bool bReleased)
{
	UpdatePointer();
	PointerInput.Mouse.Left.SetStates(bPressed, bDown, bReleased);
	if (UVTBEditorInteractiveToolsContext* Context = GetToolsContext())
	{
		Context->InputRouter->PostInputEvent(PointerInput);
	}
	UpdateCameraInputLock();
	if (bReleased)
	{
		PointerInput.Mouse.Left.SetStates(false, false, false);
		UpdateCameraInputLock();
	}
}

void AVTBEditorSpectatorPawn::SelectActor()
{
	APlayerController* PlayerController = GetPlayerController();
	if (!PlayerController)
	{
		SendSelection(nullptr);
		return;
	}

	FHitResult Hit;
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, Hit))
	{
		SendSelection(Hit.GetActor() == this ? nullptr : Hit.GetActor());
		return;
	}
	SendSelection(nullptr);
}

void AVTBEditorSpectatorPawn::SendSelection(AActor* Actor)
{
	TArray<AActor*> Actors;
	if (IsValid(Actor))
	{
		Actors.Add(Actor);
	}

	UWorld* World = GetWorld();
	AVTBEditorGameMode* GameMode = World ? World->GetAuthGameMode<AVTBEditorGameMode>() : nullptr;
	if (GameMode)
	{
		GameMode->SetSelectedActors(Actors);
		return;
	}

	if (UVTBEditorSubsystem* Subsystem = World ? World->GetSubsystem<UVTBEditorSubsystem>() : nullptr)
	{
		TArray<TWeakObjectPtr<AActor>> WeakActors;
		if (IsValid(Actor))
		{
			WeakActors.Add(Actor);
		}
		Subsystem->ReceiveSelection(WeakActors);
	}
}

void AVTBEditorSpectatorPawn::SetGizmoMode(EToolContextTransformGizmoMode Mode)
{
	CurrentGizmoMode = Mode;
	if (UVTBEditorInteractiveToolsContext* Context = GetToolsContext())
	{
		Context->SetGizmoMode(CurrentGizmoMode);
	}
}

bool AVTBEditorSpectatorPawn::IsCameraNavigating() const
{
	const APlayerController* PlayerController = GetPlayerController();
	return PlayerController
		&& PlayerController->IsInputKeyDown(EKeys::RightMouseButton)
		&& !PlayerController->IsInputKeyDown(EKeys::LeftMouseButton);
}

bool AVTBEditorSpectatorPawn::IsGizmoCapturingMouse() const
{
	if (UVTBEditorInteractiveToolsContext* Context = GetToolsContext())
	{
		return Context->InputRouter->HasActiveMouseCapture();
	}
	return false;
}

void AVTBEditorSpectatorPawn::OnSelectStarted()
{
	SendPointer(true, true, false);
	if (UVTBEditorInteractiveToolsContext* Context = GetToolsContext())
	{
		if (Context->InputRouter->HasActiveMouseCapture())
		{
			return;
		}
	}
	SelectActor();
}

void AVTBEditorSpectatorPawn::OnSelectCompleted()
{
	SendPointer(false, false, true);
}

void AVTBEditorSpectatorPawn::OnTranslationStarted()
{
	if (IsCameraNavigating() || IsGizmoCapturingMouse())
	{
		return;
	}
	SetGizmoMode(EToolContextTransformGizmoMode::Translation);
}

void AVTBEditorSpectatorPawn::OnRotationStarted()
{
	if (IsCameraNavigating() || IsGizmoCapturingMouse())
	{
		return;
	}
	SetGizmoMode(EToolContextTransformGizmoMode::Rotation);
}

void AVTBEditorSpectatorPawn::OnScaleStarted()
{
	if (IsCameraNavigating() || IsGizmoCapturingMouse())
	{
		return;
	}
	SetGizmoMode(EToolContextTransformGizmoMode::Scale);
}

void AVTBEditorSpectatorPawn::OnGizmoModeStarted()
{
	if (IsCameraNavigating() || IsGizmoCapturingMouse())
	{
		return;
	}
	switch (CurrentGizmoMode)
	{
	case EToolContextTransformGizmoMode::Translation:
		SetGizmoMode(EToolContextTransformGizmoMode::Rotation);
		break;
	case EToolContextTransformGizmoMode::Rotation:
		SetGizmoMode(EToolContextTransformGizmoMode::Scale);
		break;
	default:
		SetGizmoMode(EToolContextTransformGizmoMode::Translation);
		break;
	}
}

void AVTBEditorSpectatorPawn::OnSelectCancelStarted()
{
	if (UVTBEditorInteractiveToolsContext* Context = GetToolsContext())
	{
		Context->CancelActiveInteraction();
	}
	SendSelection(nullptr);
}

void AVTBEditorSpectatorPawn::OnSpaceStarted()
{
	if (IsCameraNavigating() || IsGizmoCapturingMouse())
	{
		return;
	}
	CurrentCoordinateSystem = CurrentCoordinateSystem == EToolContextCoordinateSystem::World
		? EToolContextCoordinateSystem::Local : EToolContextCoordinateSystem::World;
	if (UVTBEditorInteractiveToolsContext* Context = GetToolsContext())
	{
		Context->SetCoordinateSystem(CurrentCoordinateSystem);
	}
}

void AVTBEditorSpectatorPawn::OnCameraMoveTriggered(const FInputActionValue& Value)
{
	MoveCamera(Value.Get<FVector>());
}

void AVTBEditorSpectatorPawn::OnCameraLookTriggered(const FInputActionValue& Value)
{
	LookCamera(Value.Get<FVector2D>());
}

APlayerController* AVTBEditorSpectatorPawn::GetPlayerController() const
{
	return Cast<APlayerController>(GetController());
}

UVTBEditorInteractiveToolsContext* AVTBEditorSpectatorPawn::GetToolsContext() const
{
	UWorld* World = GetWorld();
	UVTBEditorSubsystem* Subsystem = World ? World->GetSubsystem<UVTBEditorSubsystem>() : nullptr;
	return Subsystem ? Subsystem->ToolsContext : nullptr;
}
