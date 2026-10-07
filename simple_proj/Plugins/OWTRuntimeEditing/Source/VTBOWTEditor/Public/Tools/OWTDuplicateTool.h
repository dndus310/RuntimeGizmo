#pragma once

#include "CoreMinimal.h"
#include "InteractiveTool.h"
#include "InteractiveToolBuilder.h"
#include "Tools/OWTModeToolBuilder.h"
#include "OWTDuplicateTool.generated.h"

class UOWTAttributeEditMode;

UCLASS(Transient)
class VTBOWTEDITOR_API UOWTDuplicateTool : public UInteractiveTool
{
	GENERATED_BODY()

public:
	UOWTDuplicateTool();

	virtual void Setup() override;
	virtual void OnTick(float DeltaTime) override;
	virtual void Shutdown(EToolShutdownType ShutdownType) override;

	virtual bool HasAccept() const override
	{
		return false;
	}

	virtual bool HasCancel() const override
	{
		return true;
	}

	void Initialize(UOWTAttributeEditMode& InMode);

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UOWTAttributeEditMode> Mode;

	bool bExecuted;
};

UCLASS(Transient)
class VTBOWTEDITOR_API UOWTDuplicateToolBuilder : public UOWTModeToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool RequestStart(UOWTAttributeEditMode& Mode, FName ToolId, FString& OutError) override;
	virtual bool CanBuildTool(const FToolBuilderState& State) const override;
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& State) const override;
};
