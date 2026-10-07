#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/SListView.h"

class FOWTStateMonitorModel;
class SMultiLineEditableTextBox;
class SSearchBox;
struct FOWTStateMonitorHistoryEntry;
struct FOWTStateMonitorNode;
struct FOWTStateMonitorSource;
struct FOWTStateMonitorSourceOption;
struct FOWTStateMonitorViewNode;
template <typename ItemType>
class SComboBox;
template <typename ItemType>
class STreeView;

/** An embeddable, read-only state inspector. History pause never pauses the current state. */
class OWTSTATEMONITOR_API SOWTStateMonitor : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SOWTStateMonitor)
	{
	}

	SLATE_ARGUMENT(TSharedPtr<FOWTStateMonitorModel>, Model)

	SLATE_END_ARGS()

	SOWTStateMonitor();

	virtual void Tick(const FGeometry& AllottedGeometry, double CurrentTime, float DeltaTime) override;

	void Construct(const FArguments& InArgs);

	bool SelectSource(FName SourceId);

	void SetHistoryPaused(bool bPaused);

	void ClearHistory();

	FName GetSelectedSourceId() const;

	bool IsHistoryPaused() const;

private:
	TSharedRef<SWidget> BuildSourceControls();

	TSharedRef<SWidget> BuildViewControls();

	TSharedRef<SWidget> BuildCurrentView();

	TSharedRef<SWidget> BuildHistoryView();

	void RefreshModel(bool bForce = false);

	void RefreshCurrent();

	void RefreshHistory(bool bForce = false);

	void RefreshTree();

	void RefreshHistoryDetails();

	TSharedPtr<FOWTStateMonitorViewNode> UpdateTreeNode(
	    const TSharedPtr<FOWTStateMonitorNode>& Node, TMap<FString, TSharedPtr<FOWTStateMonitorViewNode>>& UpdatedNodes,
	    bool bParentMatches);

	bool MatchesFilter(const FOWTStateMonitorNode& Node) const;

	void SaveTreeState();

	void RestoreTreeState();

	void OnSourceChanged(TSharedPtr<FOWTStateMonitorSourceOption> Item, ESelectInfo::Type SelectInfo);

	void OnFilterChanged(const FText& Text);

	void OnHistorySelected(TSharedPtr<FOWTStateMonitorHistoryEntry> Item, ESelectInfo::Type SelectInfo);

	FReply OnPauseHistoryClicked();

	FReply OnClearHistoryClicked();

	FReply OnAcknowledgeClicked();

	FReply OnToggleJsonClicked();

	FReply OnCopyJsonClicked();

	FReply OnReturnToCurrentClicked();

	TSharedRef<SWidget> GenerateSourceOption(TSharedPtr<FOWTStateMonitorSourceOption> Item);

	TSharedRef<ITableRow> GenerateFieldRow(TSharedPtr<FOWTStateMonitorViewNode> Item,
	                                       const TSharedRef<STableViewBase>& OwnerTable);

	TSharedRef<ITableRow> GenerateHistoryRow(TSharedPtr<FOWTStateMonitorHistoryEntry> Item,
	                                         const TSharedRef<STableViewBase>& OwnerTable);

	void GetFieldChildren(TSharedPtr<FOWTStateMonitorViewNode> Item,
	                      TArray<TSharedPtr<FOWTStateMonitorViewNode>>& OutChildren) const;

	FText GetSourceText() const;

	FText GetCurrentStatusText() const;

	FText GetCurrentJsonText() const;

	FText GetHistoryStatusText() const;

	FText GetHistoryDetailsText() const;

	FText GetPauseHistoryText() const;

	FText GetViewToggleText() const;

	EVisibility GetTreeVisibility() const;

	EVisibility GetJsonVisibility() const;

	EVisibility GetEmptyVisibility() const;

	EVisibility GetHistoryDetailsVisibility() const;

	bool HasSource() const;

	TSharedPtr<FOWTStateMonitorModel> Model;
	TSharedPtr<FOWTStateMonitorSource> CurrentSource;
	TSharedPtr<FOWTStateMonitorHistoryEntry> SelectedHistory;
	TSharedPtr<SComboBox<TSharedPtr<FOWTStateMonitorSourceOption>>> SourceCombo;
	TSharedPtr<STreeView<TSharedPtr<FOWTStateMonitorViewNode>>> FieldTree;
	TSharedPtr<SListView<TSharedPtr<FOWTStateMonitorHistoryEntry>>> HistoryList;
	TSharedPtr<SMultiLineEditableTextBox> JsonView;
	TArray<TSharedPtr<FOWTStateMonitorSourceOption>> SourceOptions;
	TArray<TSharedPtr<FOWTStateMonitorViewNode>> RootItems;
	TArray<TSharedPtr<FOWTStateMonitorHistoryEntry>> HistoryItems;
	TMap<FString, TSharedPtr<FOWTStateMonitorViewNode>> FieldItems;
	TSet<FString> SavedExpandedPaths;
	FString SavedSelectedPath;
	FString FilterText;
	FText CurrentJsonText;
	FText HistoryDetailsText;
	FName SelectedSourceId;
	double HistoryTimeOriginSeconds;
	uint64 LastRevision;
	float SavedScrollOffset;
	bool bHistoryPaused;
	bool bShowJson;
	bool bChangingSource;
	bool bInitialExpansion;
};
