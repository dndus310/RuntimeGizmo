// Fill out your copyright notice in the Description page of Project Settings.

#include "VTBOWTEditorPlayerController.h"
#include "UI/OWTAttributeDetailsWidget.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorGameMode.h"
#include "VTBOWTEditorSubsystem.h"
#include "EnhancedPlayerInput.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "InputCoreTypes.h"

AVTBOWTEditorPlayerController::AVTBOWTEditorPlayerController()
    : AttributeDetailsClass(UOWTAttributeDetailsWidget::StaticClass()), AttributeDetails(nullptr)
{
	bShowMouseCursor = true;
	OverridePlayerInputClass = UEnhancedPlayerInput::StaticClass();
}

void AVTBOWTEditorPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalController())
	{
		return;
	}
	if (IsRunningCommandlet())
	{
		return;
	}
	if (!AttributeDetailsClass)
	{
		return;
	}

	UVTBOWTEditorSubsystem* Subsystem = GetWorld()->GetSubsystem<UVTBOWTEditorSubsystem>();
	check(Subsystem);
	AVTBAttributeEditor* Editor = Subsystem->GetAttributeEditor();
	if (!Editor)
	{
		AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
		if (GameMode)
		{
			if (GameMode->GetClass()->ImplementsInterface(UVTBOWTEditorGameModeProvider::StaticClass()))
			{
				Editor = IVTBOWTEditorGameModeProvider::Execute_GetAttributeEditor(GameMode);
			}
		}
	}
	if (!Editor)
	{
		return;
	}

	Editor->BindSubsystem(Subsystem);
	AttributeDetails = CreateWidget<UOWTAttributeDetailsWidget>(this, AttributeDetailsClass);
	if (!ensureMsgf(AttributeDetails, TEXT("Could not create the runtime attribute sidebar.")))
	{
		return;
	}
	AttributeDetails->SetAttributeEditor(Editor);
	AttributeDetails->AddToViewport(20);
}

void AVTBOWTEditorPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AttributeDetails)
	{
		AttributeDetails->SetAttributeEditor(nullptr);
		AttributeDetails->RemoveFromParent();
		AttributeDetails = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

void AVTBOWTEditorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);
	if (PlayerInput)
	{
		// These keys belong to this editor. Engine development defaults otherwise change viewport shading.
		for (FKeyBind& Binding : PlayerInput->DebugExecBindings)
		{
			if (Binding.Key == EKeys::F2 || Binding.Key == EKeys::F3)
			{
				Binding.bDisabled = true;
			}
		}
	}
	InputComponent->BindKey(EKeys::F3, IE_Pressed, this, &AVTBOWTEditorPlayerController::ToggleEventMonitor);
}

void AVTBOWTEditorPlayerController::ToggleEventMonitor()
{
	if (!AttributeDetails)
	{
		return;
	}
	AttributeDetails->ToggleMonitor();
}

bool AVTBOWTEditorPlayerController::IsAttributeUIBlockingInput() const
{
	if (!AttributeDetails)
	{
		return false;
	}
	return AttributeDetails->IsBlockingWorldInput();
}

UOWTAttributeDetailsWidget* AVTBOWTEditorPlayerController::GetAttributeDetailsWidget() const
{
	return AttributeDetails;
}
