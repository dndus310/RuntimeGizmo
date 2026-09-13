#pragma once

#include "CoreMinimal.h"
#include "InteractiveToolsContext.h"
#include "ToolContextInterfaces.h"
#include "Context/OWTGizmoSnapSettings.h"
#include "VTBOWTEditorToolsContext.generated.h"

class UVTBOWTEditorSubsystem;

UCLASS()
class VTBOWTEDITOR_API UVTBOWTEditorToolsContext : public UInteractiveToolsContext
{
	GENERATED_BODY()

public:
	UVTBOWTEditorToolsContext();

	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable, Category = "OWT|Snapping")
	bool SetSnapSettings(const FOWTGizmoSnapSettings& Settings);

	UFUNCTION(BlueprintPure, Category = "OWT|Snapping")
	FOWTGizmoSnapSettings GetSnapSettings() const;

	void InitializeContext(UVTBOWTEditorSubsystem& Subsystem);
	bool IsInitialized() const;

private:
	UPROPERTY(Transient)
	FOWTGizmoSnapSettings SnapSettings;

	TUniquePtr<IToolsContextQueriesAPI> Queries;
	TUniquePtr<IToolsContextTransactionsAPI> Transactions;
	bool bInitialized;
};
