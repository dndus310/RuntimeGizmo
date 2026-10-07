#pragma once

#include "CoreMinimal.h"
#include "InteractiveTool.h"
#include "InteractiveToolBuilder.h"
#include "ToolContextInterfaces.h"
#include "BaseBehaviors/BehaviorTargetInterfaces.h"
#include "OWTModeTestTypes.generated.h"

UCLASS(Transient)
class UOWTModeTestTool : public UInteractiveTool, public IClickBehaviorTarget
{
	GENERATED_BODY()

public:
	UOWTModeTestTool() : bCancelDuringSetup(false), ClickCount(0), RenderCount(0), HUDCount(0)
	{
	}
	bool bCancelDuringSetup;
	int32 ClickCount;
	int32 RenderCount;
	int32 HUDCount;
	virtual void Setup() override;
	virtual bool HasAccept() const override
	{
		return true;
	}
	virtual bool CanAccept() const override
	{
		return true;
	}
	virtual bool HasCancel() const override
	{
		return true;
	}
	virtual FInputRayHit IsHitByClick(const FInputDeviceRay& Ray) override
	{
		return FInputRayHit(1.0);
	}
	virtual void OnClicked(const FInputDeviceRay& Ray) override
	{
		++ClickCount;
	}
	virtual void Render(IToolsContextRenderAPI* API) override;
	virtual void DrawHUD(FCanvas* Canvas, IToolsContextRenderAPI* API) override
	{
		++HUDCount;
	}
};

UCLASS(Transient)
class UOWTModeTestBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& State) const override
	{
		return State.World != nullptr;
	}
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& State) const override;
};

UCLASS(Transient)
class UOWTModeRejectBuilder : public UOWTModeTestBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& State) const override
	{
		return false;
	}
};

UCLASS(Transient)
class UOWTModeNullBuilder : public UOWTModeTestBuilder
{
	GENERATED_BODY()

public:
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& State) const override
	{
		return nullptr;
	}
};

UCLASS(Transient)
class UOWTModeSetupCancelBuilder : public UOWTModeTestBuilder
{
	GENERATED_BODY()

public:
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& State) const override;
};
