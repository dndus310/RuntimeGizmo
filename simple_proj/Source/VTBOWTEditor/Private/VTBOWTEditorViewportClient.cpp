#include "VTBOWTEditorViewportClient.h"

#include "Context/VTBOWTEditorToolsContext.h"
#include "Context/IVTBOWTEditorViewport.h"
#include "Engine/World.h"
#include "VTBOWTEditorModeSubsystem.h"
#include "SceneView.h"

bool UVTBOWTEditorViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	return Super::InputKey(EventArgs);
}

bool UVTBOWTEditorViewportClient::InputAxis(const FInputKeyEventArgs& Args)
{
	return Super::InputAxis(Args);
}

bool UVTBOWTEditorViewportClient::InputTouch(FViewport* InViewport, const FInputDeviceId DeviceId, uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force,
	uint32 TouchpadIndex, const uint64 Timestamp)
{
	return Super::InputTouch(InViewport, DeviceId, Handle, Type, TouchLocation, Force, TouchpadIndex, Timestamp);
}

void UVTBOWTEditorViewportClient::MouseMove(FViewport* InViewport, int32 X, int32 Y)
{
	Super::MouseMove(InViewport, X, Y);
}

void UVTBOWTEditorViewportClient::CapturedMouseMove(FViewport* InViewport, int32 X, int32 Y)
{
	Super::CapturedMouseMove(InViewport, X, Y);
}

void UVTBOWTEditorViewportClient::Draw(FViewport* InViewport, FCanvas* Canvas)
{
	Super::Draw(InViewport, Canvas);
}

void UVTBOWTEditorViewportClient::Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (UWorld* EditingWorld = GetWorld())
	{
		if (UVTBOWTEditorModeSubsystem* Subsystem = EditingWorld->GetSubsystem<UVTBOWTEditorModeSubsystem>())
		{
			Subsystem->GetToolsContext()->GetViewport().Render(View, PDI);
		}
	}
}
