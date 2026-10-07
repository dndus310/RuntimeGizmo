#include "Tools/OWTDuplicateTool.h"
#include "Context/OWTAttributeEditSessionContext.h"
#include "ContextObjectStore.h"
#include "InteractiveToolManager.h"
#include "Modes/OWTAttributeEditMode.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorSubsystem.h"

namespace
{
UOWTAttributeEditMode* ResolveDuplicateMode(const FToolBuilderState& State)
{
	if (!State.ToolManager)
	{
		return nullptr;
	}
	UOWTAttributeEditSessionContext* Session =
	    State.ToolManager->GetContextObjectStore()->FindContext<UOWTAttributeEditSessionContext>();
	return Session ? Session->GetMode() : nullptr;
}
} // namespace

UOWTDuplicateTool::UOWTDuplicateTool() : Mode(), bExecuted(false)
{
}

void UOWTDuplicateTool::Initialize(UOWTAttributeEditMode& InMode)
{
	Mode = &InMode;
}
void UOWTDuplicateTool::Setup()
{
	Super::Setup();
	if (!Mode.IsValid())
	{
		GetToolManager()->PostActiveToolShutdownRequest(this, EToolShutdownType::Cancel);
		return;
	}
	if (!Mode->HasPendingDuplicate())
	{
		GetToolManager()->PostActiveToolShutdownRequest(this, EToolShutdownType::Cancel);
	}
}

void UOWTDuplicateTool::OnTick(float DeltaTime)
{
	if (bExecuted)
	{
		return;
	}
	bExecuted = true;
	if (Mode.IsValid())
	{
		Mode->ExecutePendingDuplicate(*this);
	}
}

void UOWTDuplicateTool::Shutdown(EToolShutdownType ShutdownType)
{
	if (ShutdownType == EToolShutdownType::Cancel)
	{
		if (Mode.IsValid())
		{
			Mode->CancelPendingDuplicate();
		}
	}
	Super::Shutdown(ShutdownType);
}

bool UOWTDuplicateToolBuilder::CanBuildTool(const FToolBuilderState& State) const
{
	UOWTAttributeEditMode* Mode = ResolveDuplicateMode(State);
	if (!Mode)
	{
		return false;
	}
	if (!Mode->IsEntered())
	{
		return false;
	}
	return IsValid(Mode->GetSelectedObject());
}

UInteractiveTool* UOWTDuplicateToolBuilder::BuildTool(const FToolBuilderState& State) const
{
	if (!CanBuildTool(State))
	{
		return nullptr;
	}
	UOWTDuplicateTool* Tool = NewObject<UOWTDuplicateTool>(State.ToolManager);
	Tool->Initialize(*ResolveDuplicateMode(State));
	return Tool;
}

bool UOWTDuplicateToolBuilder::RequestStart(UOWTAttributeEditMode& Mode, FName ToolId, FString& OutError)
{
	UVTBOWTEditorSubsystem* Subsystem = Mode.GetSubsystem();
	AVTBAttributeEditor* Editor = Subsystem ? Subsystem->GetAttributeEditor() : nullptr;
	if (!Editor)
	{
		OutError = TEXT("An AttributeEditor must be bound before duplicating.");
		return false;
	}
	if (!Editor->RequestDuplicate(Editor->GetSnapshot()))
	{
		OutError = TEXT("The duplicate request was rejected. See the event monitor for its reason.");
		return false;
	}
	return true;
}
