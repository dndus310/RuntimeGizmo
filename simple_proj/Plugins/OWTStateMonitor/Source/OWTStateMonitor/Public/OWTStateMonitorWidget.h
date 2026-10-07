#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "OWTStateMonitorWidget.generated.h"

class FOWTStateMonitorModel;
class SOWTStateMonitor;
class UScriptStruct;

/** Embeddable, read-only runtime monitor. Submit reflected value copies from the owning application. */
UCLASS(BlueprintType, meta = (DisplayName = "State Monitor"))
class OWTSTATEMONITOR_API UOWTStateMonitorWidget : public UWidget
{
	GENERATED_BODY()

public:
	UOWTStateMonitorWidget(const FObjectInitializer& ObjectInitializer);

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	/** Blueprint accepts any reflected struct. Native callers use SubmitStructSnapshot. */
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "State Monitor",
	          meta = (CustomStructureParam = "Snapshot", AutoCreateRefTerm = "Snapshot"))
	bool SubmitSnapshot(FName SourceId, FText Label, const int32& Snapshot);

	DECLARE_FUNCTION(execSubmitSnapshot);

	UFUNCTION(BlueprintCallable, Category = "State Monitor")
	bool RemoveSource(FName SourceId);

	UFUNCTION(BlueprintCallable, Category = "State Monitor")
	void ClearHistory(FName SourceId);

	UFUNCTION(BlueprintCallable, Category = "State Monitor")
	void ResetBaseline(FName SourceId);

	UFUNCTION(BlueprintCallable, Category = "State Monitor")
	void ResetMonitor();

	bool SubmitStructSnapshot(FName SourceId, const FText& Label, const UScriptStruct* StructType,
	                          const void* StructData);

	TSharedPtr<FOWTStateMonitorModel> GetMonitorModel();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TSharedPtr<FOWTStateMonitorModel> MonitorModel;
	TSharedPtr<SOWTStateMonitor> MonitorView;
};
