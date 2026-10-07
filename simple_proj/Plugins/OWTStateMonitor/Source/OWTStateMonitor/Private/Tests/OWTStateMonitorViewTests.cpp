#if WITH_DEV_AUTOMATION_TESTS

#include "SOWTStateMonitor.h"

#include "OWTStateMonitorModel.h"
#include "Tests/OWTStateMonitorTestTypes.h"
#include "Framework/Application/SlateApplication.h"
#include "Layout/Children.h"
#include "Misc/AutomationTest.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Views/STreeView.h"

namespace OWTStateMonitorViewTests
{
TSharedPtr<SWidget> FindTaggedWidget(const TSharedRef<SWidget>& Widget, FName Tag)
{
	if (Widget->GetTag() == Tag)
	{
		return Widget;
	}
	FChildren* Children = Widget->GetChildren();
	for (int32 Index = 0; Index < Children->Num(); ++Index)
	{
		TSharedPtr<SWidget> Found = FindTaggedWidget(Children->GetChildAt(Index), Tag);
		if (Found.IsValid())
		{
			return Found;
		}
	}
	return nullptr;
}

void RefreshView(const TSharedRef<SOWTStateMonitor>& View)
{
	const FGeometry Geometry = FGeometry::MakeRoot(FVector2D(600.0, 700.0), FSlateLayoutTransform());
	View->Tick(Geometry, FPlatformTime::Seconds(), 0.1f);
}
} // namespace OWTStateMonitorViewTests

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTStateMonitorViewTest, "OWT.StateMonitor.View.NavigationAndLiveHistory",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTStateMonitorViewTest::RunTest(const FString& Parameters)
{
	if (!TestTrue(TEXT("Slate is available for the real monitor view"), FSlateApplication::IsInitialized()))
	{
		return false;
	}
	FOWTStateMonitorSettings Settings;
	Settings.MaxHistoryEntries = 2;
	const TSharedPtr<FOWTStateMonitorModel> Model = MakeShared<FOWTStateMonitorModel>(Settings);
	FOWTStateMonitorTestState State;
	State.Nested.Text = TEXT("Needle A");
	Model->SubmitSnapshot(TEXT("First"), FText::FromString(TEXT("First source")),
	                      FOWTStateMonitorTestState::StaticStruct(), &State);
	Model->SubmitSnapshot(TEXT("Second"), FText::FromString(TEXT("Second source")),
	                      FOWTStateMonitorTestState::StaticStruct(), &State);
	const TSharedRef<SOWTStateMonitor> View = SNew(SOWTStateMonitor).Model(Model);
	const TSharedPtr<STreeView<TSharedPtr<FOWTStateMonitorViewNode>>> Tree =
	    StaticCastSharedPtr<STreeView<TSharedPtr<FOWTStateMonitorViewNode>>>(
	        OWTStateMonitorViewTests::FindTaggedWidget(View, TEXT("OWTMonitorTree")));
	const TSharedPtr<SSearchBox> Search =
	    StaticCastSharedPtr<SSearchBox>(OWTStateMonitorViewTests::FindTaggedWidget(View, TEXT("OWTMonitorSearch")));
	const TSharedPtr<SListView<TSharedPtr<FOWTStateMonitorHistoryEntry>>> History =
	    StaticCastSharedPtr<SListView<TSharedPtr<FOWTStateMonitorHistoryEntry>>>(
	        OWTStateMonitorViewTests::FindTaggedWidget(View, TEXT("OWTMonitorHistory")));
	const TSharedPtr<SMultiLineEditableTextBox> Json = StaticCastSharedPtr<SMultiLineEditableTextBox>(
	    OWTStateMonitorViewTests::FindTaggedWidget(View, TEXT("OWTMonitorJson")));
	const TSharedPtr<SMultiLineEditableTextBox> HistoryDetails = StaticCastSharedPtr<SMultiLineEditableTextBox>(
	    OWTStateMonitorViewTests::FindTaggedWidget(View, TEXT("OWTMonitorHistoryDetails")));
	if (!TestTrue(TEXT("Current tree is built"), Tree.IsValid()))
	{
		return false;
	}
	if (!TestTrue(TEXT("Search is built"), Search.IsValid()))
	{
		return false;
	}
	if (!TestTrue(TEXT("History list is built"), History.IsValid()))
	{
		return false;
	}
	if (!TestTrue(TEXT("JSON view is built"), Json.IsValid()))
	{
		return false;
	}
	if (!TestTrue(TEXT("History details are built"), HistoryDetails.IsValid()))
	{
		return false;
	}
	TestEqual(TEXT("First source selected by default"), View->GetSelectedSourceId(), FName(TEXT("First")));
	if (!TestEqual(TEXT("Current has one struct root"), Tree->GetRootItems().Num(), 1))
	{
		return false;
	}
	const TSharedPtr<FOWTStateMonitorViewNode> InitialRoot = Tree->GetRootItems()[0];
	Tree->SetItemExpansion(InitialRoot, false);
	Tree->SetSelection(InitialRoot, ESelectInfo::Direct);
	State.Nested.Text = TEXT("Needle B");
	Model->SubmitSnapshot(TEXT("First"), FText::FromString(TEXT("First source")),
	                      FOWTStateMonitorTestState::StaticStruct(), &State);
	OWTStateMonitorViewTests::RefreshView(View);
	TestTrue(TEXT("Capture refresh preserves field identity"), Tree->GetRootItems()[0] == InitialRoot);
	TestFalse(TEXT("Capture refresh preserves a collapsed branch"), Tree->IsItemExpanded(InitialRoot));
	TestTrue(TEXT("Capture refresh preserves selection"), Tree->GetSelectedItems().Contains(InitialRoot));
	TestTrue(TEXT("JSON reflects the new current state"), Json->GetText().ToString().Contains(TEXT("Needle B")));

	Search->SetText(FText::FromString(TEXT("no-field-matches-this-filter")));
	TestEqual(TEXT("Unmatched filter removes visible roots"), Tree->GetRootItems().Num(), 0);
	Search->SetText(FText::FromString(TEXT("Needle B")));
	TestEqual(TEXT("Value search includes its containing struct"), Tree->GetRootItems().Num(), 1);
	Search->SetText(FText::GetEmpty());
	TestFalse(TEXT("Clearing search restores the previous expansion"), Tree->IsItemExpanded(InitialRoot));
	TestTrue(TEXT("Clearing search restores the previous selection"), Tree->GetSelectedItems().Contains(InitialRoot));

	if (!TestTrue(TEXT("Changes produce inspectable history"), History->GetItems().Num() > 0))
	{
		return false;
	}
	const TSharedPtr<FOWTStateMonitorHistoryEntry> Inspected = History->GetItems()[0];
	History->SetSelection(Inspected, ESelectInfo::Direct);
	const FString FrozenDetails = HistoryDetails->GetText().ToString();
	View->SetHistoryPaused(true);
	const uint64 FrozenNewest = History->GetItems()[0]->Sequence;
	for (int32 Index = 0; Index < 4; ++Index)
	{
		State.Nested.Text = FString::Printf(TEXT("Live %d"), Index);
		Model->SubmitSnapshot(TEXT("First"), FText::FromString(TEXT("First source")),
		                      FOWTStateMonitorTestState::StaticStruct(), &State);
		OWTStateMonitorViewTests::RefreshView(View);
	}
	TestTrue(TEXT("History pause is active"), View->IsHistoryPaused());
	TestEqual(TEXT("Paused history retains its displayed entries"), History->GetItems()[0]->Sequence, FrozenNewest);
	TestTrue(TEXT("Current stays live while history pauses"), Json->GetText().ToString().Contains(TEXT("Live 3")));
	TestEqual(TEXT("An inspected change remains stable while current changes"), HistoryDetails->GetText().ToString(),
	          FrozenDetails);
	TestFalse(TEXT("Inspected record was evicted from model history"),
	          Model->FindSource(TEXT("First"))->History.Contains(Inspected));
	View->SetHistoryPaused(false);
	TestTrue(TEXT("Resuming shows latest retained entries"), History->GetItems()[0]->Sequence > FrozenNewest);
	TestEqual(TEXT("Resuming does not replace inspected change details"), HistoryDetails->GetText().ToString(),
	          FrozenDetails);
	TestTrue(TEXT("Another source can be selected"), View->SelectSource(TEXT("Second")));
	TestEqual(TEXT("Selection changes source ID"), View->GetSelectedSourceId(), FName(TEXT("Second")));
	TestTrue(TEXT("Selected source supplies current JSON"), Json->GetText().ToString().Contains(TEXT("Needle A")));
	TestFalse(TEXT("Unknown source is rejected"), View->SelectSource(TEXT("Missing")));
	View->ClearHistory();
	TestEqual(TEXT("Clear history updates selected view"), History->GetItems().Num(), 0);
	TestTrue(TEXT("Clear history retains selected current state"),
	         Json->GetText().ToString().Contains(TEXT("Needle A")));
	return true;
}

#endif
