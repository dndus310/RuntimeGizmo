#include "VTBOWTSpectator.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Context/OWTEditContexts.h"
#include "BaseGizmos/GizmoActor.h"
#include "Interfaces/OWTEditContextReceiver.h"
#include "VTBOWTEditorSubsystem.h"
#include "EnhancedInputComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "InputActionValue.h"

AVTBOWTSpectator::AVTBOWTSpectator()
    : EnhancedInput(CreateDefaultSubobject<UEnhancedInputComponent>(TEXT("EnhancedInputComponent"))),
      MappingContext(nullptr), CameraNavigateAction(nullptr), CameraMoveAction(nullptr), CameraLookAction(nullptr),
      EditContextReceiver(nullptr), SelectionTraceChannel(ECC_Visibility), SelectionTraceDistance(1000000.f),
      CameraLookSensitivity(0.15f), MappingSubsystem(), bPointerDown(false), bSelectionConsumed(false),
      bCameraNavigating(false)
{
	// Default spectator WASD bindings conflict with the requested W/E/R gizmo shortcuts.
	bAddDefaultMovementBindings = false;
	OverrideInputComponentClass = UEnhancedInputComponent::StaticClass();
	EnhancedInput->bAutoRegister = false;
}

void AVTBOWTSpectator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (!ensureMsgf(CameraNavigateAction && CameraMoveAction && CameraLookAction,
	                TEXT("OWT spectator requires its camera input actions.")))
	{
		return;
	}

	Input->BindAction(CameraNavigateAction, ETriggerEvent::Started, this, &ThisClass::OnCameraNavigationStarted);
	Input->BindAction(CameraNavigateAction, ETriggerEvent::Completed, this, &ThisClass::StopCameraNavigation);
	Input->BindAction(CameraNavigateAction, ETriggerEvent::Canceled, this, &ThisClass::StopCameraNavigation);
	Input->BindAction(CameraMoveAction, ETriggerEvent::Triggered, this, &ThisClass::OnCameraMove);
	Input->BindAction(CameraLookAction, ETriggerEvent::Triggered, this, &ThisClass::OnCameraLook);
}

void AVTBOWTSpectator::PawnClientRestart()
{
	Super::PawnClientRestart();
	RefreshInputMapping();
}

void AVTBOWTSpectator::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	RefreshInputMapping();
}

void AVTBOWTSpectator::UnPossessed()
{
	StopCameraNavigation();
	RemoveInputMapping();

	Super::UnPossessed();
}

bool AVTBOWTSpectator::SendEditContext(const FInstancedStruct& Context)
{
	check(IsInGameThread());

	if (!Context.IsValid())
	{
		return false;
	}
	// Navigation owns mouse/WASD until RMB is released. Editing requests stay on the input side.
	if (bCameraNavigating)
	{
		return false;
	}

	if (Context.GetPtr<FOWTSelectObjectContext>() && bSelectionConsumed)
	{
		bSelectionConsumed = false;
		return true;
	}

	UObject* Receiver = ResolveEditContextReceiver();
	if (!Receiver)
	{
		return false;
	}

	return IOWTEditContextReceiver::Execute_ReceiveEditContext(Receiver, Context);
}

AActor* AVTBOWTSpectator::TraceSelectableObject()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	UWorld* World = GetWorld();
	if (!PC || !World || bCameraNavigating)
	{
		return nullptr;
	}

	FVector Origin;
	FVector Direction;
	if (!PC->DeprojectMousePositionToWorld(Origin, Direction))
	{
		return nullptr;
	}

	UpdatePointer(true, true, false);
	bPointerDown = true;
	UVTBOWTEditorSubsystem* Hub = World->GetSubsystem<UVTBOWTEditorSubsystem>();
	bSelectionConsumed = Hub && Hub->HasGizmoCapture();
	if (bSelectionConsumed)
	{
		GetMovementComponent()->StopMovementImmediately();
		ConsumeMovementInputVector();
		return Hub->SelectedObject.Get();
	}
	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(OWTSelection), true, this);
	const FVector TraceEnd = Origin + Direction * SelectionTraceDistance;
	const bool bHit = World->LineTraceSingleByChannel(Hit, Origin, TraceEnd, SelectionTraceChannel, Params);
	if (!bHit)
	{
		return nullptr;
	}

	return Hit.GetActor() && !Hit.GetActor()->IsA<AGizmoActor>() ? Hit.GetActor() : nullptr;
}

void AVTBOWTSpectator::RefreshInputMapping()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	ULocalPlayer* Player = PC ? PC->GetLocalPlayer() : nullptr;
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
	    Player ? Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() : nullptr;
	if (!Subsystem)
	{
		RemoveInputMapping();
		return;
	}
	if (!ensureMsgf(MappingContext, TEXT("OWT spectator requires IMC_OWTEdit.")))
	{
		return;
	}
	RemoveInputMapping();
	Subsystem->AddMappingContext(MappingContext, 100);
	MappingSubsystem = Subsystem;
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;
}

UObject* AVTBOWTSpectator::ResolveEditContextReceiver() const
{
	UObject* Receiver = EditContextReceiver;
	if (!IsValid(Receiver))
	{
		UWorld* World = GetWorld();
		if (!World)
		{
			return nullptr;
		}

		Receiver = World->GetSubsystem<UVTBOWTEditorSubsystem>();
		if (!Receiver)
		{
			return nullptr;
		}
	}

	if (!ensureMsgf(Receiver->GetClass()->ImplementsInterface(UOWTEditContextReceiver::StaticClass()),
	                TEXT("OWT receiver %s must implement OWTEditContextReceiver."), *Receiver->GetPathName()))
	{
		return nullptr;
	}

	return Receiver;
}

UInputComponent* AVTBOWTSpectator::CreatePlayerInputComponent()
{
	return EnhancedInput;
}

void AVTBOWTSpectator::DestroyPlayerInputComponent()
{
	// Retain the native component across possession changes; rebuild its bindings on the next possession.
	EnhancedInput->ClearActionBindings();
	EnhancedInput->UnregisterComponent();
	InputComponent = nullptr;
}

void AVTBOWTSpectator::RemoveInputMapping()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = MappingSubsystem.Get())
	{
		Subsystem->RemoveMappingContext(MappingContext);
	}
	MappingSubsystem.Reset();
}

void AVTBOWTSpectator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC || !IsLocallyControlled())
	{
		return;
	}
	// Also handles capture initiated outside the Pawn, or a lost RMB release while focus changes.
	if (bCameraNavigating && (!PC->IsInputKeyDown(EKeys::RightMouseButton) || HasGizmoCapture()))
	{
		StopCameraNavigation();
	}
	if (bCameraNavigating)
	{
		return;
	}
	const bool bDown = PC->IsInputKeyDown(EKeys::LeftMouseButton);
	UpdatePointer(false, bDown, bPointerDown && !bDown);
	bPointerDown = bDown;
}

void AVTBOWTSpectator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopCameraNavigation();
	RemoveInputMapping();
	Super::EndPlay(EndPlayReason);
}

void AVTBOWTSpectator::OnCameraNavigationStarted()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC || !IsLocallyControlled() || HasGizmoCapture() || bCameraNavigating)
	{
		return;
	}

	bCameraNavigating = true;
	bPointerDown = false;
	PC->bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	PC->SetInputMode(InputMode);
}

void AVTBOWTSpectator::OnCameraMove(const FInputActionValue& Value)
{
	if (!CanNavigateCamera())
	{
		return;
	}

	const FVector2D Movement = Value.Get<FVector2D>();
	MoveForward(Movement.Y);
	MoveRight(Movement.X);
}

void AVTBOWTSpectator::OnCameraLook(const FInputActionValue& Value)
{
	if (!CanNavigateCamera())
	{
		return;
	}

	const FVector2D Look = Value.Get<FVector2D>() * CameraLookSensitivity;
	AddControllerYawInput(Look.X);
	AddControllerPitchInput(-Look.Y);
}

void AVTBOWTSpectator::StopCameraNavigation()
{
	if (!bCameraNavigating)
	{
		return;
	}

	bCameraNavigating = false;
	GetMovementComponent()->StopMovementImmediately();
	ConsumeMovementInputVector();
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}
	PC->RotationInput = FRotator::ZeroRotator;
	PC->bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputMode);
}

bool AVTBOWTSpectator::IsCameraNavigating() const
{
	return bCameraNavigating;
}

bool AVTBOWTSpectator::CanNavigateCamera() const
{
	return bCameraNavigating && IsLocallyControlled() && !HasGizmoCapture();
}

bool AVTBOWTSpectator::HasGizmoCapture() const
{
	const UVTBOWTEditorSubsystem* Hub = GetWorld()->GetSubsystem<UVTBOWTEditorSubsystem>();
	return Hub && Hub->HasGizmoCapture();
}

void AVTBOWTSpectator::UpdatePointer(bool bPressed, bool bDown, bool bReleased)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}
	FOWTGizmoPointerContext Pointer;
	if (!PC->DeprojectMousePositionToWorld(Pointer.RayOrigin, Pointer.RayDirection) ||
	    !PC->GetMousePosition(Pointer.ScreenPosition.X, Pointer.ScreenPosition.Y))
	{
		return;
	}
	Pointer.bPressed = bPressed;
	Pointer.bDown = bDown;
	Pointer.bReleased = bReleased;
	SendEditContext(FInstancedStruct::Make(Pointer));
}
