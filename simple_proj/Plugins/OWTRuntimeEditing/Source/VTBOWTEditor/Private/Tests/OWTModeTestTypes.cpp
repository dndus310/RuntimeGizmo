#include "Tests/OWTModeTestTypes.h"
#include "BaseBehaviors/SingleClickBehavior.h"
#include "InteractiveToolManager.h"
#include "PrimitiveDrawInterface.h"
#include "ToolContextInterfaces.h"

void UOWTModeTestTool::Setup()
{
	Super::Setup();
	if (bCancelDuringSetup)
	{
		GetToolManager()->PostActiveToolShutdownRequest(this, EToolShutdownType::Cancel);
		return;
	}
	USingleClickInputBehavior* Click = NewObject<USingleClickInputBehavior>(this);
	Click->Initialize(this);
	AddInputBehavior(Click);
}

void UOWTModeTestTool::Render(IToolsContextRenderAPI* API)
{
	++RenderCount;
	API->GetPrimitiveDrawInterface()->DrawLine(FVector(100, 0, 0), FVector(100, 20, 0), FLinearColor::Green, 0);
}

UInteractiveTool* UOWTModeTestBuilder::BuildTool(const FToolBuilderState& State) const
{
	return NewObject<UOWTModeTestTool>(State.ToolManager);
}
UInteractiveTool* UOWTModeSetupCancelBuilder::BuildTool(const FToolBuilderState& State) const
{
	UOWTModeTestTool* Tool = NewObject<UOWTModeTestTool>(State.ToolManager);
	Tool->bCancelDuringSetup = true;
	return Tool;
}
