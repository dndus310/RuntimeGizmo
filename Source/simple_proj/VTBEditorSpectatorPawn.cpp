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
	, bHadMouseCursor(false)
	, bHadClickEvents(false)
	, bHadMouseOverEvents(false)
	, bHasPointerPosition(false)
{
	PrimaryActorTick.bCanEverTick = true;
	bAddDefaultMovementBindings = false;
}

void AVTBEditorSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
	StartInput();
	SetGizmoMode(CurrentGizmoMode);
}

void AVTBEditorSpectatorPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopInput();
	Super::EndPlay(EndPlayReason);
}

void AVTBEditorSpectatorPawn::UnPossessed()
{
	StopInput();
	Super::UnPossessed();
}

void AVTBEditorSpectatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StartInput();
	APlayerController* PlayerController = CachedPlayerController.Get();
	if (!PlayerController)
	{
		return;
	}

	// Focus loss or a mapping rebuild can remove the release event from Enhanced Input.
	const bool bMouseButtonReleased = PointerInput.Mouse.Left.bDown
		&& !PlayerController->IsInputKeyDown(EKeys::LeftMouseButton);
	if (bMouseButtonReleased)
	{
		CancelPointer();
	}
	if (IsGizmoCapturingMouse())
	{
		SendPointer(false, true, false);
	}
	else if (UpdatePointer())
	{
		PostPointerInput(true);
	}
	UpdateCameraInputLock();
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
		EnhancedInput->BindAction(Action, ETriggerEvent::Canceled, this, &ThisClass::OnSelectCanceled);
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
	if (!IsValid(PlayerController) || !PlayerController->IsLocalController())
	{
		PlayerController = nullptr;
	}
	if (CachedPlayerController.Get() == PlayerController
		&& (!PlayerController || CachedInputSubsystem.IsValid()))
	{
		return;
	}
	if (CachedPlayerController.Get() != PlayerController)
	{
		StopInput();
		if (!PlayerController)
		{
			return;
		}

		CachedPlayerController = PlayerController;
		bHadMouseCursor = PlayerController->bShowMouseCursor;
		bHadClickEvents = PlayerController->bEnableClickEvents;
		bHadMouseOverEvents = PlayerController->bEnableMouseOverEvents;
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}

	// Possession can precede LocalPlayer input subsystem initialization.
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer ? LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() : nullptr;
	if (InputSubsystem)
	{
		CachedInputSubsystem = InputSubsystem;
		AddedMappingContext = nullptr;
		UInputMappingContext* MappingContext = EditorMappingContext.LoadSynchronous();
		if (IsValid(MappingContext) && !InputSubsystem->HasMappingContext(MappingContext))
		{
			InputSubsystem->AddMappingContext(MappingContext, EditorInputPriority);
			AddedMappingContext = MappingContext;
		}
	}
}

void AVTBEditorSpectatorPawn::StopInput()
{
	APlayerController* PlayerController = CachedPlayerController.Get();
	if (PlayerController)
	{
		CancelPointer();
		PlayerController->bShowMouseCursor = bHadMouseCursor;
		PlayerController->bEnableClickEvents = bHadClickEvents;
		PlayerController->bEnableMouseOverEvents = bHadMouseOverEvents;
	}
	ResetCameraInputLock();
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = CachedInputSubsystem.Get())
	{
		if (AddedMappingContext)
		{
			InputSubsystem->RemoveMappingContext(AddedMappingContext);
		}
	}
	AddedMappingContext = nullptr;
	CachedInputSubsystem.Reset();
	CachedPlayerController.Reset();
	PointerInput = FInputDeviceState();
	bHasPointerPosition = false;
}

void AVTBEditorSpectatorPawn::UpdateCameraInputLock()
{
	APlayerController* PlayerController = CachedPlayerController.Get();
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

bool AVTBEditorSpectatorPawn::UpdatePointer()
{
	PointerInput.Mouse.Delta2D = FVector2D::ZeroVector;
	APlayerController* PlayerController = CachedPlayerController.Get();
	if (!PlayerController)
	{
		bHasPointerPosition = false;
		return false;
	}

	float MouseX = 0.0f;
	float MouseY = 0.0f;
	FVector RayOrigin;
	FVector RayDirection;
	if (!PlayerController->GetMousePosition(MouseX, MouseY)
		|| !PlayerController->DeprojectMousePositionToWorld(RayOrigin, RayDirection))
	{
		bHasPointerPosition = false;
		return false;
	}
	const FVector2D Position(MouseX, MouseY);
	PointerInput.Mouse.Delta2D = bHasPointerPosition ? Position - PointerInput.Mouse.Position2D : FVector2D::ZeroVector;
	PointerInput.Mouse.Position2D = Position;
	PointerInput.Mouse.WorldRay = FRay(RayOrigin, RayDirection.GetSafeNormal());
	bHasPointerPosition = true;

	PointerInput.InputDevice = EInputDevices::Mouse;
	PointerInput.SetModifierKeyStates(
		PlayerController->IsInputKeyDown(EKeys::LeftShift) || PlayerController->IsInputKeyDown(EKeys::RightShift),
		PlayerController->IsInputKeyDown(EKeys::LeftAlt) || PlayerController->IsInputKeyDown(EKeys::RightAlt),
		PlayerController->IsInputKeyDown(EKeys::LeftControl) || PlayerController->IsInputKeyDown(EKeys::RightControl),
		PlayerController->IsInputKeyDown(EKeys::LeftCommand) || PlayerController->IsInputKeyDown(EKeys::RightCommand));
	return true;
}

bool AVTBEditorSpectatorPawn::SendPointer(bool bPressed, bool bDown, bool bReleased)
{
	if (!UpdatePointer())
	{
		CancelPointer();
		return false;
	}
	PointerInput.Mouse.Left.SetStates(bPressed, bDown, bReleased);
	PostPointerInput(false);
	// Press/release are edges and must not leak into subsequent hover or drag events.
	PointerInput.Mouse.Left.bPressed = false;
	PointerInput.Mouse.Left.bReleased = false;
	UpdateCameraInputLock();
	return true;
}

void AVTBEditorSpectatorPawn::PostPointerInput(bool bHover)
{
	UVTBEditorInteractiveToolsContext* Context = GetToolsContext();
	if (!IsValid(Context))
	{
		return;
	}

	// Proxy callbacks may request a different selection while the router still uses the gizmo.
	Context->PostPointerInput(PointerInput, bHover);
}

void AVTBEditorSpectatorPawn::CancelPointer()
{
	UVTBEditorInteractiveToolsContext* Context = GetToolsContext();
	if (IsValid(Context) && CachedPlayerController.IsValid())
	{
		Context->CancelActiveInteraction();
	}
	PointerInput.Mouse.Left.SetStates(false, false, false);
	PointerInput.Mouse.Delta2D = FVector2D::ZeroVector;
	bHasPointerPosition = false;
	ResetCameraInputLock();
}

void AVTBEditorSpectatorPawn::SelectActor()
{
	APlayerController* PlayerController = GetPlayerController();
	if (!IsValid(PlayerController))
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
	if (!IsValid(World))
	{
		return;
	}
	if (AVTBEditorGameMode* GameMode = World->GetAuthGameMode<AVTBEditorGameMode>(); IsValid(GameMode))
	{
		GameMode->SetSelectedActors(Actors);
		return;
	}

	if (UVTBEditorSubsystem* Subsystem = World->GetSubsystem<UVTBEditorSubsystem>(); IsValid(Subsystem))
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
		Context->SetCoordinateSystem(CurrentCoordinateSystem);
	}
}

bool AVTBEditorSpectatorPawn::IsCameraNavigating() const
{
	const APlayerController* PlayerController = GetPlayerController();
	if (!IsValid(PlayerController))
	{
		return false;
	}

	return PlayerController->IsInputKeyDown(EKeys::RightMouseButton)
		&& !PlayerController->IsInputKeyDown(EKeys::LeftMouseButton);
}

bool AVTBEditorSpectatorPawn::IsGizmoCapturingMouse() const
{
	UVTBEditorInteractiveToolsContext* Context = GetToolsContext();
	if (!IsValid(Context))
	{
		return false;
	}
	return Context->HasActiveMouseCapture();
}

void AVTBEditorSpectatorPawn::OnSelectStarted()
{
	if (!SendPointer(true, true, false) || IsGizmoCapturingMouse())
	{
		return;
	}
	SelectActor();
}

void AVTBEditorSpectatorPawn::OnSelectCompleted()
{
	SendPointer(false, false, true);
}

void AVTBEditorSpectatorPawn::OnSelectCanceled()
{
	CancelPointer();
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
	CancelPointer();
	SendSelection(nullptr);
}

void AVTBEditorSpectatorPawn::OnSpaceStarted()
{
	const APlayerController* PlayerController = GetPlayerController();
	if (!PlayerController)
	{
		return;
	}
	const bool bControlDown = PlayerController->IsInputKeyDown(EKeys::LeftControl)
		|| PlayerController->IsInputKeyDown(EKeys::RightControl);
	if (!bControlDown)
	{
		return;
	}
	if (IsCameraNavigating() || IsGizmoCapturingMouse())
	{
		return;
	}
	if (CurrentGizmoMode == EToolContextTransformGizmoMode::Scale)
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
	if (!IsValid(World))
	{
		return nullptr;
	}

	UVTBEditorSubsystem* Subsystem = World->GetSubsystem<UVTBEditorSubsystem>();
	if (!IsValid(Subsystem))
	{
		return nullptr;
	}
	return Subsystem->GetRuntimeContext();
}
