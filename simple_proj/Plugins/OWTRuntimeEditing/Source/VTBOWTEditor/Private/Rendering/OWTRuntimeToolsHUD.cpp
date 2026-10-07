#include "Rendering/OWTRuntimeToolsHUD.h"
#include "Engine/World.h"
#include "Modes/OWTAttributeEditMode.h"
#include "VTBOWTEditorSubsystem.h"

void AOWTRuntimeToolsHUD::DrawHUD()
{
	Super::DrawHUD();
	UVTBOWTEditorSubsystem* Subsystem = GetWorld()->GetSubsystem<UVTBOWTEditorSubsystem>();
	if (!Subsystem)
	{
		return;
	}
	UOWTAttributeEditMode* Mode = Subsystem->GetAttributeEditMode();
	if (Mode)
	{
		Mode->RenderTools(Canvas, PlayerOwner);
	}
}
