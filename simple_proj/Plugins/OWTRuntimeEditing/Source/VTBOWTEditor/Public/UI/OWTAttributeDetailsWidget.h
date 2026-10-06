#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Events/OWTAttributeTypes.h"
#include "Types/SlateEnums.h"
#include "OWTAttributeDetailsWidget.generated.h"

class AVTBAttributeEditor;
class ITableRow;
class SBorder;
class SSearchBox;
class STableViewBase;
class SVerticalBox;
struct FOWTMonitorEntry;
template <typename ItemType>
class SListView;
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
	TSharedRef<SWidget> BuildDetails();
	TSharedRef<SWidget> BuildMonitor();
	TSharedRef<SWidget> BuildTransformSection(const FText& Title, EOWTTransformField FirstField, double Delta);
	void AddTransformField(SVerticalBox& Rows, EOWTTransformField Field, const FText& Label, double Delta);
	void SubscribeToEditor();
	void ApplySnapshot(const FOWTAttributeSnapshot& InSnapshot);
	void RefreshPanelVisibility();
	void RefreshMonitorHistory(bool bForce = false);
	void RefreshMonitorFilter();
	void FinishSliderOperation();
	void CancelSliderOperation();
	void ResetSliderOperation();
	void UnsubscribeFromEditor();
	void ClearPanelFocus();
	void ReleaseFieldCapture();
	void ResetMonitor();
	void OnEditorEvent(FName Event, const FString& Json);
	void OnMonitorFilterChanged(const FText& Text);
	TSharedRef<ITableRow> OnGenerateMonitorRow(TSharedPtr<FOWTMonitorEntry> Entry,
	                                           const TSharedRef<STableViewBase>& OwnerTable);
	void OnSliderBegin(EOWTTransformField Field);
	void OnSliderValueChanged(double Value, EOWTTransformField Field);
	void OnSliderEnd(double Value, EOWTTransformField Field);
	void OnValueCommitted(double Value, ETextCommit::Type CommitType, EOWTTransformField Field);
	FReply OnDuplicateClicked();
	FReply OnDetailsClicked();
	FReply OnMonitorClicked();
	FReply OnPauseMonitorClicked();
	FReply OnClearMonitorClicked();
	bool CanEditFields() const;
	bool CanDuplicateSelection() const;
	bool IsSliderBeginCurrent(const AVTBAttributeEditor& Editor, const FOWTAttributeSnapshot& Current,
	                          FGuid StartedOperation) const;
	bool HasFieldCapture() const;
	EVisibility GetTransformVisibility() const;
	EVisibility GetDetailsVisibility() const;
	EVisibility GetMonitorVisibility() const;
	double GetFieldValue(EOWTTransformField Field) const;
	FText GetStateOverview() const;
	FText GetStateIdentity() const;
	FText GetMonitorStatus() const;
	FText GetPauseMonitorText() const;
	FText GetObjectName() const;
	FText GetObjectClass() const;
	FText GetStatusText() const;
	FText GetErrorText() const;

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<AVTBAttributeEditor> AttributeEditor;

	TSharedPtr<SBorder> Panel;
	TSharedPtr<SSearchBox> MonitorSearch;
	TSharedPtr<SListView<TSharedPtr<FOWTMonitorEntry>>> EventList;
	TArray<TSharedPtr<SSpinBox<double>>> Fields;
	TArray<TSharedPtr<FOWTMonitorEntry>> MonitorEntries;
	TArray<TSharedPtr<FOWTMonitorEntry>> FilteredEntries;
	FOWTAttributeSnapshot Snapshot;
	FOWTAttributeSnapshot OperationSnapshot;
	FGuid Subscription;
	FGuid OperationId;
	EOWTTransformField ActiveField;
	FString LastError;
	FString MonitorFilter;
	uint64 SubscriptionGeneration;
	int64 LastMonitorSequence;
	int64 ClearedMonitorSequence;
	bool bApplyingSnapshot;
	bool bSliderActive;
	bool bStartingSlider;
	bool bMonitorVisible;
	bool bMonitorPaused;
};
