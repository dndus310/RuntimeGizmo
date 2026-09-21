#include "VTBOWTEditPlayerController.h"

#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "InputCoreTypes.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Context/VTBOWTEditorToolsContext.h"
#include "Context/IVTBOWTEditorUndoRedo.h"
#include "VTBOWTEditorModeSubsystem.h"
#include "VTBRuntimeDemo.h"

void AVTBOWTEditPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController() && FParse::Param(FCommandLine::Get(), TEXT("VTBRuntimeDemo")))
	{
		GetWorld()->SpawnActor<AVTBRuntimeDemo>();
		ClientSetHUD(AVTBRuntimeDemoHUD::StaticClass());
	}
}

void AVTBOWTEditPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindKey(FInputChord(EKeys::Z, false, true, false, false), IE_Pressed,
		this, &ThisClass::UndoRuntimeEdit);
	InputComponent->BindKey(FInputChord(EKeys::Y, false, true, false, false), IE_Pressed,
		this, &ThisClass::RedoRuntimeEdit);
}

void AVTBOWTEditPlayerController::UndoRuntimeEdit()
{
	if (UVTBOWTEditorModeSubsystem* Runtime = GetWorld()->GetSubsystem<UVTBOWTEditorModeSubsystem>())
	{
		Runtime->GetToolsContext()->GetUndoRedo().Undo();
	}
}

void AVTBOWTEditPlayerController::RedoRuntimeEdit()
{
	if (UVTBOWTEditorModeSubsystem* Runtime = GetWorld()->GetSubsystem<UVTBOWTEditorModeSubsystem>())
	{
		Runtime->GetToolsContext()->GetUndoRedo().Redo();
	}
}
