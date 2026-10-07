#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Events/OWTAttributeTypes.h"
#include "Extensions/OWTToolDescriptor.h"
#include "State/OWTEditingSessionTypes.h"
#include "Types/SlateEnums.h"
#include "OWTAttributeDetailsWidget.generated.h"

class AVTBAttributeEditor;
class SBorder;
class SVerticalBox;
class FOWTStateMonitorModel;
class SOWTStateMonitor;
template <typename NumericType>
class SSpinBox;

UCLASS(BlueprintType, Blueprintable)
class VTBOWTEDITOR_API UOWTAttributeDetailsWidget : public UUserWidget
{
	GENERATED_BODY()

#if WITH_DEV_AUTOMATION_TESTS
	friend class FOWTAttributeDetailsContractTest;
#endif

public:
	UOWTAttributeDetailsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UFUNCTION(BlueprintCallable, Category = "OWT|Attributes")
	void SetAttributeEditor(AVTBAttributeEditor* InEditor);

	UFUNCTION(BlueprintCallable, Category = "OWT|Monitor")
	void SetMonitorVisible(bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "OWT|Monitor")
	void ToggleMonitor();

	UFUNCTION(BlueprintPure, Category = "OWT|Monitor")
	bool IsMonitorVisible() const;

	UFUNCTION(BlueprintPure, Category = "OWT|Attributes")
	bool IsBlockingWorldInput() const;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	void AddHeaderRows(SVerticalBox& Contents);

	TSharedRef<SWidget> BuildNavigationTabs();

	TSharedRef<SWidget> BuildDetails();

	TSharedRef<SWidget> BuildMonitor();

	TSharedRef<SWidget> BuildTools();

	TSharedRef<SWidget> BuildTransformSection(const FText& Title, EOWTTransformField FirstField, double Delta);

	void AddTransformField(SVerticalBox& Rows, EOWTTransformField Field, const FText& Label, double Delta);

	void SubscribeToEditor();

	void ApplySnapshot(const FOWTAttributeSnapshot& InSnapshot);

	void RefreshSessionState();

	void RefreshMonitorState(bool bForce = false);

	void RefreshToolRows(const TArray<FOWTToolAvailability>& InTools);

	void RefreshPanelVisibility();

	void FinishSliderOperation();

	void CancelSliderOperation();

	void ResetSliderOperation();

	void UnsubscribeFromEditor();

	void ClearPanelFocus();

	void ReleaseFieldCapture();

	void ResetMonitor();

	void OnEditorEvent(FName Event, const FString& Json);

	void OnSliderBegin(EOWTTransformField Field);

	void OnSliderValueChanged(double Value, EOWTTransformField Field);

	void OnSliderEnd(double Value, EOWTTransformField Field);

	void OnValueCommitted(double Value, ETextCommit::Type CommitType, EOWTTransformField Field);

	FReply OnDuplicateClicked();

	FReply OnToolClicked(FName ToolId);

	FReply OnEndToolClicked(bool bAccept);

	FReply OnDetailsClicked();

	FReply OnMonitorClicked();

	bool CanStartTool(FName ToolId) const;

	bool CanEndTool(bool bAccept) const;

	bool CanEditFields() const;

	bool CanDuplicateSelection() const;

	bool HaveToolRowsChanged(const TArray<FOWTToolAvailability>& InTools) const;

	bool IsSliderBeginCurrent(const AVTBAttributeEditor& Editor, const FOWTAttributeSnapshot& Current,
	                          FGuid StartedOperation) const;

	bool HasFieldCapture() const;

	EVisibility GetTransformVisibility() const;

	EVisibility GetDetailsVisibility() const;

	EVisibility GetMonitorVisibility() const;

	double GetFieldValue(EOWTTransformField Field) const;

	FText GetStateOverview() const;

	FText GetStateIdentity() const;

	FText GetModeStatus() const;

	FText GetObjectName() const;

	FText GetObjectClass() const;

	FText GetStatusText() const;

	FText GetErrorText() const;

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<AVTBAttributeEditor> AttributeEditor;

	TSharedPtr<SBorder> Panel;
	TSharedPtr<SVerticalBox> ToolRows;
	TSharedPtr<FOWTStateMonitorModel> MonitorModel;
	TSharedPtr<SOWTStateMonitor> MonitorView;
	TArray<TSharedPtr<SSpinBox<double>>> Fields;
	FOWTAttributeSnapshot Snapshot;
	FOWTAttributeSnapshot OperationSnapshot;
	FOWTModeSnapshot ModeSnapshot;
	TArray<FOWTToolAvailability> AvailableTools;
	FGuid Subscription;
	FGuid OperationId;
	EOWTTransformField ActiveField;
	FString LastError;
	uint64 SubscriptionGeneration;
	int64 LastMonitorSequence;
	double LastMonitorRefreshTime;
	bool bApplyingSnapshot;
	bool bSliderActive;
	bool bStartingSlider;
	bool bMonitorVisible;
};
