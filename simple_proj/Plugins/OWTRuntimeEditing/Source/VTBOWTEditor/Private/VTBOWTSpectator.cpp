#include "VTBOWTSpectator.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Context/OWTEditContexts.h"
#include "BaseGizmos/GizmoActor.h"
#include "Interfaces/OWTEditContextReceiver.h"
#include "VTBOWTEditorSubsystem.h"
#include "VTBOWTEditorPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputModifiers.h"
#include "Input/VTBOWTModifierKeyTrigger.h"

namespace
{
struct FOWTFallbackEditAction
{
	const TCHAR* Name;
	UScriptStruct* (*ContextType)();
	FKey Key;
	bool bControl;
	bool bShift;
	bool bNoShift;
};

const TArray<FOWTFallbackEditAction> FallbackActionSpecs = {
    {TEXT("IA_Selectable"), &FOWTSelectObjectContext::StaticStruct, EKeys::LeftMouseButton, false, false, false},
    {TEXT("IA_Undo"), &FOWTUndoContext::StaticStruct, EKeys::Z, true, false, true},
    {TEXT("IA_Redo"), &FOWTRedoContext::StaticStruct, EKeys::Z, true, true, false},
    {TEXT("IA_TRSGizmoCoordinate"), &FOWTToggleCoordinateSystemContext::StaticStruct, EKeys::Tilde, true, false, false},
    {TEXT("IA_GizmoMode"), &FOWTToggleTransformSplineContext::StaticStruct, EKeys::T, true, false, false},
    {TEXT("IA_GizmoTranslation"), &FOWTSetTranslationContext::StaticStruct, EKeys::W, false, false, false},
    {TEXT("IA_GizmoRotation"), &FOWTSetRotationContext::StaticStruct, EKeys::E, false, false, false},
    {TEXT("IA_GizmoScale"), &FOWTSetScaleContext::StaticStruct, EKeys::R, false, false, false},
    {TEXT("IA_SelectCancel"), &FOWTHideSelectionGizmoContext::StaticStruct, EKeys::Escape, false, false, false},
    {TEXT("IA_Duplicate"), &FOWTDuplicateSelectionContext::StaticStruct, EKeys::D, true, false, false},
    {TEXT("IA_ToggleEditing"), &FOWTToggleEditingContext::StaticStruct, EKeys::F2, false, false, false}};

void MapEditingAction(UInputMappingContext& Context, UInputAction& Action, FKey Key, bool bControl, bool bShift,
                      bool bNoShift)
{
	FEnhancedActionKeyMapping& Mapping = Context.MapKey(&Action, Key);
	Mapping.Triggers.Add(NewObject<UInputTriggerPressed>(&Context));
	if (bControl || bShift || bNoShift)
	{
		UVTBOWTModifierKeyTrigger* Modifiers = NewObject<UVTBOWTModifierKeyTrigger>(&Context);
		Modifiers->bRequireControl = bControl;
		Modifiers->bRequireShift = bShift;
		Modifiers->bDisallowShift = bNoShift;
		Mapping.Triggers.Add(Modifiers);
	}
}

void MapCameraMovement(UInputMappingContext& Context, UInputAction& Action, FKey Key, bool bVertical, bool bNegative)
{
	FEnhancedActionKeyMapping& Mapping = Context.MapKey(&Action, Key);
	if (bVertical)
	{
		Mapping.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>(&Context));
	}
	if (bNegative)
	{
		Mapping.Modifiers.Add(NewObject<UInputModifierNegate>(&Context));
	}
}

UInputAction* CreateCameraAction(UObject& Owner, FName Name, EInputActionValueType Type)
{
	UInputAction* Action = NewObject<UInputAction>(&Owner, Name, RF_Transient);
	Action->ValueType = Type;
	Action->bConsumeInput = false;
	Action->AccumulationBehavior = EInputActionAccumulationBehavior::Cumulative;
	return Action;
}
} // namespace

AVTBOWTSpectator::AVTBOWTSpectator()
    : EnhancedInput(CreateDefaultSubobject<UEnhancedInputComponent>(TEXT("EnhancedInputComponent"))),
      MappingContext(nullptr), CameraNavigateAction(nullptr), CameraMoveAction(nullptr), CameraLookAction(nullptr),
      EditContextReceiver(nullptr), SelectionTraceChannel(ECC_Visibility), SelectionTraceDistance(1000000.f),
      CameraLookSensitivity(0.15f), FallbackMappingContext(nullptr), FallbackEditActions(), MappingSubsystem(),
      AppliedMappingContext(), NativeInputBindingHandles(), bPointerDown(false), bSelectionConsumed(false),
      bCameraNavigating(false), bIgnorePointerUntilRelease(false)
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
	InitializeInputMapping();
	RemoveNativeInputBindings(*Input);
	BindNativeInput(*Input);
}

void AVTBOWTSpectator::InitializeInputMapping()
{
	if (MappingContext)
	{
		return;
	}
	if (FallbackMappingContext)
	{
		MappingContext = FallbackMappingContext;
		return;
	}

	FallbackMappingContext = NewObject<UInputMappingContext>(this, TEXT("OWTDefaultInputMapping"), RF_Transient);
	MappingContext = FallbackMappingContext;
	CreateFallbackEditingActions();
	CreateFallbackCameraActions();
}

void AVTBOWTSpectator::CreateFallbackEditingActions()
{
	for (const FOWTFallbackEditAction& Spec : FallbackActionSpecs)
	{
		UInputAction* Action = NewObject<UInputAction>(FallbackMappingContext, Spec.Name, RF_Transient);
		Action->ValueType = EInputActionValueType::Boolean;
		FallbackEditActions.Add(Action);
		MapEditingAction(*FallbackMappingContext, *Action, Spec.Key, Spec.bControl, Spec.bShift, Spec.bNoShift);
		if (Spec.ContextType() == FOWTRedoContext::StaticStruct())
		{
			MapEditingAction(*FallbackMappingContext, *Action, EKeys::Y, true, false, false);
		}
	}
}

void AVTBOWTSpectator::CreateFallbackCameraActions()
{
	CameraNavigateAction =
	    CreateCameraAction(*FallbackMappingContext, TEXT("IA_CameraNavigate"), EInputActionValueType::Boolean);
	CameraMoveAction =
	    CreateCameraAction(*FallbackMappingContext, TEXT("IA_CameraMove"), EInputActionValueType::Axis2D);
	CameraLookAction =
	    CreateCameraAction(*FallbackMappingContext, TEXT("IA_CameraLook"), EInputActionValueType::Axis2D);
	FallbackMappingContext->MapKey(CameraNavigateAction, EKeys::RightMouseButton);
	FallbackMappingContext->MapKey(CameraLookAction, EKeys::Mouse2D);
	MapCameraMovement(*FallbackMappingContext, *CameraMoveAction, EKeys::W, true, false);
	MapCameraMovement(*FallbackMappingContext, *CameraMoveAction, EKeys::S, true, true);
	MapCameraMovement(*FallbackMappingContext, *CameraMoveAction, EKeys::A, false, true);
	MapCameraMovement(*FallbackMappingContext, *CameraMoveAction, EKeys::D, false, false);
}

void AVTBOWTSpectator::BindNativeInput(UEnhancedInputComponent& Input)
{
	if (MappingContext == FallbackMappingContext)
	{
		for (int32 Index = 0; Index < FallbackEditActions.Num(); ++Index)
		{
			NativeInputBindingHandles.Add(Input
			                                  .BindAction(FallbackEditActions[Index], ETriggerEvent::Triggered, this,
			                                              &ThisClass::OnFallbackEditAction, Index)
			                                  .GetHandle());
		}
	}
	if (!ensureMsgf(CameraNavigateAction, TEXT("OWT spectator requires a camera navigation action.")))
	{
		return;
	}
	if (!ensureMsgf(CameraMoveAction, TEXT("OWT spectator requires a camera movement action.")))
	{
		return;
	}
	if (!ensureMsgf(CameraLookAction, TEXT("OWT spectator requires a camera look action.")))
	{
		return;
	}

	NativeInputBindingHandles.Add(
	    Input.BindAction(CameraNavigateAction, ETriggerEvent::Started, this, &ThisClass::OnCameraNavigationStarted)
	        .GetHandle());
	NativeInputBindingHandles.Add(
	    Input.BindAction(CameraNavigateAction, ETriggerEvent::Completed, this, &ThisClass::StopCameraNavigation)
	        .GetHandle());
	NativeInputBindingHandles.Add(
	    Input.BindAction(CameraNavigateAction, ETriggerEvent::Canceled, this, &ThisClass::StopCameraNavigation)
	        .GetHandle());
	NativeInputBindingHandles.Add(
	    Input.BindAction(CameraMoveAction, ETriggerEvent::Triggered, this, &ThisClass::OnCameraMove).GetHandle());
	NativeInputBindingHandles.Add(
	    Input.BindAction(CameraLookAction, ETriggerEvent::Triggered, this, &ThisClass::OnCameraLook).GetHandle());
}

void AVTBOWTSpectator::RemoveNativeInputBindings(UEnhancedInputComponent& Input)
{
	for (uint32 Handle : NativeInputBindingHandles)
	{
		Input.RemoveBindingByHandle(Handle);
	}
	NativeInputBindingHandles.Empty();
}

void AVTBOWTSpectator::OnFallbackEditAction(int32 ActionIndex)
{
	if (!FallbackActionSpecs.IsValidIndex(ActionIndex))
	{
		return;
	}

	const FOWTFallbackEditAction& Spec = FallbackActionSpecs[ActionIndex];
	FInstancedStruct Context(Spec.ContextType());
	if (FOWTSelectObjectContext* Selection = Context.GetMutablePtr<FOWTSelectObjectContext>())
	{
		Selection->SelectedObject = TraceSelectableObject();
	}
	SendEditContext(Context);
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
	if (IsAttributeUIBlockingInput())
	{
		if (!Context.GetPtr<FOWTToggleEditingContext>())
		{
			return false;
		}
	}

	if (Context.GetPtr<FOWTSelectObjectContext>())
	{
		if (bIgnorePointerUntilRelease)
		{
			return true;
		}
		if (bSelectionConsumed)
		{
			bSelectionConsumed = false;
			return true;
		}
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
	if (IsAttributeUIBlockingInput())
	{
		return nullptr;
	}
	if (bIgnorePointerUntilRelease)
	{
		return nullptr;
	}
	APlayerController* PC = Cast<APlayerController>(GetController());
	UWorld* World = GetWorld();
	if (!PC)
	{
		return nullptr;
	}
	if (!World)
	{
		return nullptr;
	}
	if (bCameraNavigating)
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
	InitializeInputMapping();
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
	AppliedMappingContext = MappingContext;
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
	NativeInputBindingHandles.Empty();
	EnhancedInput->UnregisterComponent();
	InputComponent = nullptr;
}

void AVTBOWTSpectator::RemoveInputMapping()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = MappingSubsystem.Get())
	{
		if (UInputMappingContext* AppliedContext = AppliedMappingContext.Get())
		{
			Subsystem->RemoveMappingContext(AppliedContext);
		}
	}
	MappingSubsystem.Reset();
	AppliedMappingContext.Reset();
}

void AVTBOWTSpectator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}
	if (!IsLocallyControlled())
	{
		return;
	}
	const bool bDown = PC->IsInputKeyDown(EKeys::LeftMouseButton);
	if (IsAttributeUIBlockingInput())
	{
		StopCameraNavigation();
		UVTBOWTEditorSubsystem* Subsystem = GetWorld()->GetSubsystem<UVTBOWTEditorSubsystem>();
		if (Subsystem)
		{
			Subsystem->TerminateGizmoCapture();
		}
		bIgnorePointerUntilRelease = bDown;
		bPointerDown = false;
		bSelectionConsumed = false;
		return;
	}
	if (bIgnorePointerUntilRelease)
	{
		bIgnorePointerUntilRelease = bDown;
		return;
	}
	// Also handles capture initiated outside the Pawn, or a lost RMB release while focus changes.
	if (bCameraNavigating)
	{
		if (!PC->IsInputKeyDown(EKeys::RightMouseButton))
		{
			StopCameraNavigation();
		}
		else if (HasGizmoCapture())
		{
			StopCameraNavigation();
		}
	}
	if (bCameraNavigating)
	{
		return;
	}
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
	if (IsAttributeUIBlockingInput())
	{
		return;
	}
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}
	if (!IsLocallyControlled())
	{
		return;
	}
	if (HasGizmoCapture())
	{
		return;
	}
	if (bCameraNavigating)
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
	if (!bCameraNavigating)
	{
		return false;
	}
	if (!IsLocallyControlled())
	{
		return false;
	}
	if (HasGizmoCapture())
	{
		return false;
	}
	return !IsAttributeUIBlockingInput();
}

bool AVTBOWTSpectator::IsAttributeUIBlockingInput() const
{
	const AVTBOWTEditorPlayerController* PC = Cast<AVTBOWTEditorPlayerController>(GetController());
	if (!PC)
	{
		return false;
	}
	return PC->IsAttributeUIBlockingInput();
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
