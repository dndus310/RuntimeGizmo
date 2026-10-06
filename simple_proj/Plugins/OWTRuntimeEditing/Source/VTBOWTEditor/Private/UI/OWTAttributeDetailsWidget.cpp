#include "UI/OWTAttributeDetailsWidget.h"

#include "VTBAttributeEditor.h"
#include "VTBOWTEditorSubsystem.h"
#include "Dom/JsonObject.h"
#include "Events/OWTEventTypes.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
#include "InputCoreTypes.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"

#define LOCTEXT_NAMESPACE "OWTAttributeDetails"

struct FOWTMonitorEntry
{
	explicit FOWTMonitorEntry(const FOWTEventRecord& InRecord)
	    : Record(InRecord), Source(), RequestId(), OperationId(), Failure(), Payload(InRecord.Json), bExpanded(false)
	{
		TSharedPtr<FJsonObject> Object;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Record.Json);
		if (!Record.bPayloadTruncated && FJsonSerializer::Deserialize(Reader, Object) && Object.IsValid())
		{
			Object->TryGetStringField(TEXT("source"), Source);
			Object->TryGetStringField(TEXT("requestId"), RequestId);
			Object->TryGetStringField(TEXT("operationId"), OperationId);
			FString Code;
			FString Reason;
			Object->TryGetStringField(TEXT("code"), Code);
			Object->TryGetStringField(TEXT("reason"), Reason);
			Failure = Code.IsEmpty() ? Reason : Code + TEXT(": ") + Reason;
			Payload.Reset();
			const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Payload);
			FJsonSerializer::Serialize(Object.ToSharedRef(), Writer);
		}
		if (!Record.bValidJson)
		{
			Failure = TEXT("Invalid JSON payload");
		}
	}

	FOWTEventRecord Record;
	FString Source;
	FString RequestId;
	FString OperationId;
	FString Failure;
	FString Payload;
	bool bExpanded;
};

UOWTAttributeDetailsWidget::UOWTAttributeDetailsWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer), AttributeEditor(), Panel(), MonitorSearch(), EventList(), Fields(), MonitorEntries(),
      FilteredEntries(), Snapshot(), OperationSnapshot(), Subscription(), OperationId(),
      ActiveField(EOWTTransformField::LocationX), LastError(), MonitorFilter(), SubscriptionGeneration(0),
      LastMonitorSequence(-1), ClearedMonitorSequence(0), bApplyingSnapshot(false), bSliderActive(false),
      bStartingSlider(false), bMonitorVisible(false), bMonitorPaused(false)
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UOWTAttributeDetailsWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	FinishSliderOperation();
	ReleaseFieldCapture();
	Panel.Reset();
	MonitorSearch.Reset();
	EventList.Reset();
	Fields.Reset();
	Super::ReleaseSlateResources(bReleaseChildren);
}

void UOWTAttributeDetailsWidget::SetAttributeEditor(AVTBAttributeEditor* InEditor)
{
	check(IsInGameThread());
	if (AttributeEditor.Get() == InEditor)
	{
		SubscribeToEditor();
		return;
	}

	FinishSliderOperation();
	UnsubscribeFromEditor();
	AttributeEditor = InEditor;
	LastError.Reset();
	ResetMonitor();
	ApplySnapshot(FOWTAttributeSnapshot());
	SubscribeToEditor();
}

void UOWTAttributeDetailsWidget::SetMonitorVisible(bool bVisible)
{
	FinishSliderOperation();
	ReleaseFieldCapture();
	ClearPanelFocus();
	bMonitorVisible = bVisible;
	RefreshPanelVisibility();
	RefreshMonitorHistory(true);
}

void UOWTAttributeDetailsWidget::ToggleMonitor()
{
	SetMonitorVisible(!bMonitorVisible);
}

bool UOWTAttributeDetailsWidget::IsMonitorVisible() const
{
	return bMonitorVisible;
}

bool UOWTAttributeDetailsWidget::IsBlockingWorldInput() const
{
	if (!IsVisible())
	{
		return false;
	}
	if (!Panel.IsValid())
	{
		return false;
	}
	if (bSliderActive)
	{
		return true;
	}
	if (Panel->IsHovered())
	{
		return true;
	}
	if (Panel->HasAnyUserFocusOrFocusedDescendants())
	{
		return true;
	}

	return HasFieldCapture();
}

TSharedRef<SWidget> UOWTAttributeDetailsWidget::RebuildWidget()
{
	Super::RebuildWidget();
	Fields.Reset();
	TSharedRef<SVerticalBox> Contents = SNew(SVerticalBox);
	Contents->AddSlot().AutoHeight().Padding(0, 0, 0, 10)
	    [SNew(STextBlock).Text(LOCTEXT("Title", "RUNTIME EDITOR")).Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))];
	Contents->AddSlot().AutoHeight().Padding(
	    0, 0, 0,
	    6)[SNew(STextBlock).Text_UObject(this, &UOWTAttributeDetailsWidget::GetStateOverview).AutoWrapText(true)];
	Contents->AddSlot().AutoHeight().Padding(
	    0, 0, 0, 12)[SNew(STextBlock)
	                     .Text_UObject(this, &UOWTAttributeDetailsWidget::GetStateIdentity)
	                     .ToolTipText_Lambda(
	                         [this]()
	                         {
		                         return FText::FromString(TEXT("Editor: ") + Snapshot.EditorId + TEXT("\nObject: ") +
		                                                  Snapshot.ObjectId);
	                         })
	                     .ColorAndOpacity(FLinearColor(0.62f, 0.67f, 0.73f))
	                     .AutoWrapText(true)];
	Contents->AddSlot().AutoHeight().Padding(
	    0, 0, 0, 12)[SNew(SHorizontalBox) +
	                 SHorizontalBox::Slot().FillWidth(
	                     1)[SNew(SButton)
	                            .HAlign(HAlign_Center)
	                            .ContentPadding(FMargin(8, 6))
	                            .OnClicked_UObject(this, &UOWTAttributeDetailsWidget::OnDetailsClicked)
	                                [SNew(STextBlock).Text(LOCTEXT("DetailsTab", "Details"))]] +
	                 SHorizontalBox::Slot().FillWidth(1).Padding(
	                     4, 0, 0, 0)[SNew(SButton)
	                                     .HAlign(HAlign_Center)
	                                     .ContentPadding(FMargin(8, 6))
	                                     .OnClicked_UObject(this, &UOWTAttributeDetailsWidget::OnMonitorClicked)
	                                         [SNew(STextBlock).Text(LOCTEXT("EventsTab", "Events  /  F3"))]]];
	Contents->AddSlot().FillHeight(
	    1)[SNew(SOverlay) + SOverlay::Slot()[BuildDetails()] + SOverlay::Slot()[BuildMonitor()]];
	Contents->AddSlot().AutoHeight().Padding(0, 10, 0,
	                                         0)[SNew(STextBlock)
	                                                .Text(LOCTEXT("PanelHelp", "F2: edit on/off     F3: event monitor"))
	                                                .ColorAndOpacity(FLinearColor(0.62f, 0.67f, 0.73f))];

	return SNew(SOverlay).Visibility(EVisibility::SelfHitTestInvisible) +
	       SOverlay::Slot()
	           .HAlign(HAlign_Right)
	           .VAlign(VAlign_Fill)
	           .Padding(12)[SNew(SBox).WidthOverride(
	               420)[SAssignNew(Panel, SBorder)
	                        .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
	                        .BorderBackgroundColor(FLinearColor(0.035f, 0.045f, 0.06f, 0.97f))
	                        .Padding(16)
	                        .OnMouseButtonDown_Lambda(
	                            [](const FGeometry&, const FPointerEvent&)
	                            {
		                            return FReply::Handled();
	                            })
	                        .OnMouseButtonUp_Lambda(
	                            [](const FGeometry&, const FPointerEvent&)
	                            {
		                            return FReply::Handled();
	                            })[Contents]]];
}

TSharedRef<SWidget> UOWTAttributeDetailsWidget::BuildDetails()
{
	TSharedRef<SVerticalBox> Contents = SNew(SVerticalBox);
	Contents->AddSlot().AutoHeight().Padding(0, 0, 0,
	                                         4)[SNew(STextBlock)
	                                                .Text_UObject(this, &UOWTAttributeDetailsWidget::GetObjectName)
	                                                .Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
	                                                .AutoWrapText(true)];
	Contents->AddSlot().AutoHeight().Padding(0, 0, 0,
	                                         12)[SNew(STextBlock)
	                                                 .Text_UObject(this, &UOWTAttributeDetailsWidget::GetObjectClass)
	                                                 .ColorAndOpacity(FLinearColor(0.62f, 0.67f, 0.73f))
	                                                 .AutoWrapText(true)];
	Contents->AddSlot().AutoHeight().Padding(
	    0, 0, 0,
	    14)[SNew(STextBlock).Text_UObject(this, &UOWTAttributeDetailsWidget::GetStatusText).AutoWrapText(true)];
	Contents->AddSlot().AutoHeight()[BuildTransformSection(LOCTEXT("Location", "Location  /  cm"),
	                                                       EOWTTransformField::LocationX, 1.0)];
	Contents->AddSlot().AutoHeight()[BuildTransformSection(LOCTEXT("Rotation", "Rotation  /  degrees"),
	                                                       EOWTTransformField::RotationRoll, 1.0)];
	Contents->AddSlot()
	    .AutoHeight()[BuildTransformSection(LOCTEXT("Scale", "Scale"), EOWTTransformField::ScaleX, 0.01)];
	Contents->AddSlot().AutoHeight().Padding(
	    0, 4, 0, 12)[SNew(SButton)
	                     .HAlign(HAlign_Center)
	                     .ContentPadding(FMargin(12, 8))
	                     .IsEnabled_UObject(this, &UOWTAttributeDetailsWidget::CanDuplicateSelection)
	                     .OnClicked_UObject(this, &UOWTAttributeDetailsWidget::OnDuplicateClicked)
	                         [SNew(STextBlock).Text(LOCTEXT("Duplicate", "Duplicate actor"))]];
	Contents->AddSlot().AutoHeight().Padding(0, 0, 0,
	                                         12)[SNew(STextBlock)
	                                                 .Text_UObject(this, &UOWTAttributeDetailsWidget::GetErrorText)
	                                                 .ColorAndOpacity(FLinearColor(1.0f, 0.58f, 0.40f))
	                                                 .AutoWrapText(true)];
	Contents->AddSlot()
	    .AutoHeight()[SNew(STextBlock)
	                      .Text(LOCTEXT("Help", "World transform\nClick a value to type, or drag to adjust.\nShift: "
	                                            "faster   Ctrl: finer   Esc: cancel drag"))
	                      .ColorAndOpacity(FLinearColor(0.62f, 0.67f, 0.73f))
	                      .AutoWrapText(true)];

	return SNew(SScrollBox)
	           .Visibility_UObject(this, &UOWTAttributeDetailsWidget::GetDetailsVisibility)
	           .ConsumeMouseWheel(EConsumeMouseWheel::Always) +
	       SScrollBox::Slot()[Contents];
}

TSharedRef<SWidget> UOWTAttributeDetailsWidget::BuildMonitor()
{
	return SNew(SVerticalBox).Visibility_UObject(this, &UOWTAttributeDetailsWidget::GetMonitorVisibility) +
	       SVerticalBox::Slot().AutoHeight().Padding(
	           0, 0, 0,
	           8)[SNew(SHorizontalBox) +
	              SHorizontalBox::Slot().FillWidth(
	                  1)[SNew(SButton)
	                         .HAlign(HAlign_Center)
	                         .ContentPadding(FMargin(8, 5))
	                         .OnClicked_UObject(this, &UOWTAttributeDetailsWidget::OnPauseMonitorClicked)
	                             [SNew(STextBlock)
	                                  .Text_UObject(this, &UOWTAttributeDetailsWidget::GetPauseMonitorText)]] +
	              SHorizontalBox::Slot().FillWidth(1).Padding(
	                  4, 0, 0,
	                  0)[SNew(SButton)
	                         .HAlign(HAlign_Center)
	                         .ContentPadding(FMargin(8, 5))
	                         .ToolTipText(LOCTEXT("ClearViewHelp", "Clear this view. The event journal is retained."))
	                         .OnClicked_UObject(this, &UOWTAttributeDetailsWidget::OnClearMonitorClicked)
	                             [SNew(STextBlock).Text(LOCTEXT("ClearView", "Clear view"))]]] +
	       SVerticalBox::Slot().AutoHeight().Padding(
	           0, 0, 0, 8)[SAssignNew(MonitorSearch, SSearchBox)
	                           .HintText(LOCTEXT("FilterHint", "Filter topic, source, ID or payload"))
	                           .InitialText(FText::FromString(MonitorFilter))
	                           .OnTextChanged_UObject(this, &UOWTAttributeDetailsWidget::OnMonitorFilterChanged)] +
	       SVerticalBox::Slot().AutoHeight().Padding(
	           0, 0, 0, 8)[SNew(STextBlock)
	                           .Text_UObject(this, &UOWTAttributeDetailsWidget::GetMonitorStatus)
	                           .ColorAndOpacity(FLinearColor(0.62f, 0.67f, 0.73f))
	                           .AutoWrapText(true)] +
	       SVerticalBox::Slot().FillHeight(
	           1)[SAssignNew(EventList, SListView<TSharedPtr<FOWTMonitorEntry>>)
	                  .ListItemsSource(&FilteredEntries)
	                  .SelectionMode(ESelectionMode::None)
	                  .ConsumeMouseWheel(EConsumeMouseWheel::Always)
	                  .OnGenerateRow_UObject(this, &UOWTAttributeDetailsWidget::OnGenerateMonitorRow)];
}

void UOWTAttributeDetailsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SubscribeToEditor();
}

void UOWTAttributeDetailsWidget::NativeDestruct()
{
	FinishSliderOperation();
	UnsubscribeFromEditor();
	TGuardValue<bool> ApplyingSnapshot(bApplyingSnapshot, true);
	ReleaseFieldCapture();
	ClearPanelFocus();
	Super::NativeDestruct();
}

void UOWTAttributeDetailsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!AttributeEditor.IsValid())
	{
		if (!Snapshot.EditorId.IsEmpty())
		{
			UnsubscribeFromEditor();
			ApplySnapshot(FOWTAttributeSnapshot());
		}
		return;
	}
	RefreshMonitorHistory();
	if (!bSliderActive)
	{
		return;
	}
	if (!HasFieldCapture())
	{
		FinishSliderOperation();
	}
}

void UOWTAttributeDetailsWidget::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	FinishSliderOperation();
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
}

FReply UOWTAttributeDetailsWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.IsRepeat() && (InKeyEvent.GetKey() == EKeys::F2 || InKeyEvent.GetKey() == EKeys::F3))
	{
		return FReply::Handled();
	}
	if (InKeyEvent.GetKey() == EKeys::F3)
	{
		ToggleMonitor();
		return FReply::Handled();
	}
	if (InKeyEvent.GetKey() == EKeys::F2)
	{
		AVTBAttributeEditor* Editor = AttributeEditor.Get();
		if (Editor && Editor->GetWorld())
		{
			UVTBOWTEditorSubsystem* Subsystem = Editor->GetWorld()->GetSubsystem<UVTBOWTEditorSubsystem>();
			if (Subsystem)
			{
				Subsystem->ToggleEditing();
				return FReply::Handled();
			}
		}
	}
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		if (bSliderActive)
		{
			CancelSliderOperation();
			return FReply::Handled().ReleaseMouseCapture();
		}
	}

	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

TSharedRef<SWidget> UOWTAttributeDetailsWidget::BuildTransformSection(const FText& Title, EOWTTransformField FirstField,
                                                                      double Delta)
{
	TSharedRef<SVerticalBox> Rows = SNew(SVerticalBox);
	Rows->AddSlot().AutoHeight().Padding(
	    0, 0, 0, 6)[SNew(STextBlock).Text(Title).Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))];
	const bool bRotation = FirstField == EOWTTransformField::RotationRoll;
	const FText Labels[] = {LOCTEXT("X", "X"), LOCTEXT("Y", "Y"), LOCTEXT("Z", "Z")};
	const FText RotationLabels[] = {LOCTEXT("Roll", "Roll / X"), LOCTEXT("Pitch", "Pitch / Y"),
	                                LOCTEXT("Yaw", "Yaw / Z")};
	for (int32 Axis = 0; Axis < 3; ++Axis)
	{
		const EOWTTransformField Field = static_cast<EOWTTransformField>(static_cast<uint8>(FirstField) + Axis);
		AddTransformField(*Rows, Field, bRotation ? RotationLabels[Axis] : Labels[Axis], Delta);
	}

	return SNew(SBox)
	    .Visibility_UObject(this, &UOWTAttributeDetailsWidget::GetTransformVisibility)
	    .Padding(FMargin(0, 0, 0, 14))[Rows];
}

void UOWTAttributeDetailsWidget::AddTransformField(SVerticalBox& Rows, EOWTTransformField Field, const FText& Label,
                                                   double Delta)
{
	TSharedPtr<SSpinBox<double>> SpinBox;
	Rows.AddSlot().AutoHeight().Padding(
	    0, 2)[SNew(SHorizontalBox) +
	          SHorizontalBox::Slot().AutoWidth().VAlign(
	              VAlign_Center)[SNew(SBox).WidthOverride(64)[SNew(STextBlock).Text(Label)]] +
	          SHorizontalBox::Slot().FillWidth(
	              1)[SAssignNew(SpinBox, SSpinBox<double>)
	                     .Value_UObject(this, &UOWTAttributeDetailsWidget::GetFieldValue, Field)
	                     .IsEnabled_UObject(this, &UOWTAttributeDetailsWidget::CanEditFields)
	                     .MinValue(TOptional<double>())
	                     .MaxValue(TOptional<double>())
	                     .MinSliderValue(TOptional<double>())
	                     .MaxSliderValue(TOptional<double>())
	                     .Delta(Delta)
	                     .LinearDeltaSensitivity(6)
	                     .MinFractionalDigits(1)
	                     .MaxFractionalDigits(3)
	                     .EnableWheel(false)
	                     .ClearKeyboardFocusOnCommit(true)
	                     .ContentPadding(FMargin(8, 5))
	                     .OnBeginSliderMovement_UObject(this, &UOWTAttributeDetailsWidget::OnSliderBegin, Field)
	                     .OnValueChanged_UObject(this, &UOWTAttributeDetailsWidget::OnSliderValueChanged, Field)
	                     .OnEndSliderMovement_UObject(this, &UOWTAttributeDetailsWidget::OnSliderEnd, Field)
	                     .OnValueCommitted_UObject(this, &UOWTAttributeDetailsWidget::OnValueCommitted, Field)]];
	Fields.Add(SpinBox);
}

void UOWTAttributeDetailsWidget::SubscribeToEditor()
{
	if (Subscription.IsValid())
	{
		return;
	}
	AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (!Editor)
	{
		return;
	}

	const uint64 StartedGeneration = ++SubscriptionGeneration;
	const FGuid NewSubscription = Editor->Subscribe(
	    this, FOWTAttributeEventNative::CreateUObject(this, &UOWTAttributeDetailsWidget::OnEditorEvent));
	if (SubscriptionGeneration != StartedGeneration || AttributeEditor.Get() != Editor)
	{
		// The synchronous initial refresh may have detached or rebound this view through another subscriber.
		if (IsValid(Editor) && NewSubscription.IsValid())
		{
			Editor->Unsubscribe(NewSubscription);
		}
		return;
	}
	Subscription = NewSubscription;
}

void UOWTAttributeDetailsWidget::ApplySnapshot(const FOWTAttributeSnapshot& InSnapshot)
{
	TGuardValue<bool> ApplyingSnapshot(bApplyingSnapshot, true);
	bool bSelectionChanged = Snapshot.EditorId != InSnapshot.EditorId;
	if (Snapshot.SelectionRevision != InSnapshot.SelectionRevision)
	{
		bSelectionChanged = true;
	}
	if (Snapshot.ObjectId != InSnapshot.ObjectId)
	{
		bSelectionChanged = true;
	}
	if (Snapshot.bEditingEnabled && !InSnapshot.bEditingEnabled)
	{
		bSelectionChanged = true;
	}
	if (bSliderActive)
	{
		if (!bStartingSlider)
		{
			if (!InSnapshot.bIsModifying)
			{
				ResetSliderOperation();
				ReleaseFieldCapture();
			}
		}
	}
	if (bSelectionChanged)
	{
		ResetSliderOperation();
		ReleaseFieldCapture();
		ClearPanelFocus();
		LastError.Reset();
	}

	Snapshot = InSnapshot;
	RefreshPanelVisibility();
}

void UOWTAttributeDetailsWidget::RefreshPanelVisibility()
{
	SetVisibility(Snapshot.bEditingEnabled || bMonitorVisible ? ESlateVisibility::SelfHitTestInvisible
	                                                          : ESlateVisibility::Collapsed);
}

void UOWTAttributeDetailsWidget::RefreshMonitorHistory(bool bForce)
{
	if (!bMonitorVisible || bMonitorPaused)
	{
		return;
	}
	const AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (!Editor)
	{
		return;
	}
	const int64 LatestSequence = Editor->GetLatestEventSequence();
	if (!bForce && LatestSequence == LastMonitorSequence)
	{
		return;
	}

	TMap<int64, TSharedPtr<FOWTMonitorEntry>> Existing;
	for (const TSharedPtr<FOWTMonitorEntry>& Entry : MonitorEntries)
	{
		Existing.Add(Entry->Record.Sequence, Entry);
	}
	const TArray<FOWTEventRecord> Records = Editor->GetMonitorEntries(256);
	MonitorEntries.Reset();
	for (int32 Index = Records.Num() - 1; Index >= 0; --Index)
	{
		const FOWTEventRecord& Record = Records[Index];
		if (Record.Sequence <= ClearedMonitorSequence)
		{
			continue;
		}
		const TSharedPtr<FOWTMonitorEntry>* Found = Existing.Find(Record.Sequence);
		MonitorEntries.Add(Found ? *Found : MakeShared<FOWTMonitorEntry>(Record));
	}
	LastMonitorSequence = LatestSequence;
	RefreshMonitorFilter();
}

void UOWTAttributeDetailsWidget::RefreshMonitorFilter()
{
	FilteredEntries.Reset();
	for (const TSharedPtr<FOWTMonitorEntry>& Entry : MonitorEntries)
	{
		if (!MonitorFilter.IsEmpty())
		{
			if (!Entry->Record.Event.ToString().Contains(MonitorFilter) &&
			    !Entry->Record.Json.Contains(MonitorFilter) &&
			    !LexToString(Entry->Record.Sequence).Contains(MonitorFilter))
			{
				continue;
			}
		}
		FilteredEntries.Add(Entry);
	}
	if (EventList.IsValid())
	{
		EventList->RequestListRefresh();
	}
}

void UOWTAttributeDetailsWidget::FinishSliderOperation()
{
	if (!bSliderActive)
	{
		return;
	}

	ResetSliderOperation();
	AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (Editor)
	{
		Editor->FinishActiveOperation();
	}
}

void UOWTAttributeDetailsWidget::CancelSliderOperation()
{
	if (!bSliderActive)
	{
		return;
	}

	const FGuid CancelledOperation = OperationId;
	const FOWTAttributeSnapshot Expected = OperationSnapshot;
	const EOWTTransformField Field = ActiveField;
	ResetSliderOperation();
	AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (Editor)
	{
		Editor->RequestTransformField(Expected, Field, GetFieldValue(Field), EOWTTransformEditPhase::Cancel,
		                              CancelledOperation);
	}
}

void UOWTAttributeDetailsWidget::ResetSliderOperation()
{
	bSliderActive = false;
	OperationId.Invalidate();
	OperationSnapshot = FOWTAttributeSnapshot();
}

void UOWTAttributeDetailsWidget::UnsubscribeFromEditor()
{
	++SubscriptionGeneration;
	AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (Editor)
	{
		if (Subscription.IsValid())
		{
			Editor->Unsubscribe(Subscription);
		}
	}
	Subscription.Invalidate();
}

void UOWTAttributeDetailsWidget::ClearPanelFocus()
{
	if (!Panel.IsValid())
	{
		return;
	}
	if (!FSlateApplication::IsInitialized())
	{
		return;
	}
	if (Panel->HasAnyUserFocusOrFocusedDescendants())
	{
		FSlateApplication::Get().ClearKeyboardFocus();
	}
}

void UOWTAttributeDetailsWidget::ReleaseFieldCapture()
{
	if (!FSlateApplication::IsInitialized())
	{
		return;
	}
	if (HasFieldCapture())
	{
		FSlateApplication::Get().ReleaseAllPointerCapture();
	}
}

void UOWTAttributeDetailsWidget::ResetMonitor()
{
	MonitorEntries.Reset();
	FilteredEntries.Reset();
	MonitorFilter.Reset();
	if (MonitorSearch.IsValid())
	{
		MonitorSearch->SetText(FText::GetEmpty());
	}
	LastMonitorSequence = -1;
	ClearedMonitorSequence = 0;
	bMonitorPaused = false;
	if (EventList.IsValid())
	{
		EventList->RequestListRefresh();
	}
}

void UOWTAttributeDetailsWidget::OnMonitorFilterChanged(const FText& Text)
{
	MonitorFilter = Text.ToString();
	RefreshMonitorFilter();
}

TSharedRef<ITableRow> UOWTAttributeDetailsWidget::OnGenerateMonitorRow(TSharedPtr<FOWTMonitorEntry> Entry,
                                                                       const TSharedRef<STableViewBase>& OwnerTable)
{
	const FOWTEventRecord& Record = Entry->Record;
	const FString Direction = Record.Direction == EOWTEventDirection::Inbound ? TEXT("IN") : TEXT("OUT");
	const FString Heading =
	    FString::Printf(TEXT("#%lld  %s  %s"), Record.Sequence, *Direction, *Record.Event.ToString());
	const FString Summary =
	    FString::Printf(TEXT("%s UTC  |  %s\nreq %s  |  op %s"), *Record.TimestampUtc.ToString(TEXT("%H:%M:%S")),
	                    Entry->Source.IsEmpty() ? TEXT("-") : *Entry->Source,
	                    Entry->RequestId.IsEmpty() ? TEXT("-") : *Entry->RequestId.Left(8),
	                    Entry->OperationId.IsEmpty() ? TEXT("-") : *Entry->OperationId.Left(8));
	FString Metadata = FString::Printf(TEXT("source: %s\nrequestId: %s\noperationId: %s\nrecipient: %s"),
	                                   *Entry->Source, *Entry->RequestId, *Entry->OperationId,
	                                   Record.Recipient.IsValid() ? *Record.Recipient.ToString() : TEXT("broadcast"));
	if (Record.bPayloadTruncated)
	{
		Metadata += TEXT("\nPayload truncated in journal (16,384 characters).");
	}
	const FLinearColor HeadingColor =
	    Entry->Failure.IsEmpty() ? FLinearColor(0.78f, 0.88f, 0.98f) : FLinearColor(1.0f, 0.58f, 0.40f);

	return SNew(STableRow<TSharedPtr<FOWTMonitorEntry>>, OwnerTable)
	    .Padding(FMargin(
	        0, 0, 0,
	        8))[SNew(SExpandableArea)
	                .InitiallyCollapsed(!Entry->bExpanded)
	                .OnAreaExpansionChanged_Lambda(
	                    [Entry](bool bExpanded)
	                    {
		                    Entry->bExpanded = bExpanded;
	                    })
	                .HeaderContent()
	                    [SNew(SVerticalBox) +
	                     SVerticalBox::Slot().AutoHeight()[SNew(STextBlock)
	                                                           .Text(FText::FromString(Heading))
	                                                           .ColorAndOpacity(HeadingColor)
	                                                           .Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
	                                                           .AutoWrapText(true)] +
	                     SVerticalBox::Slot().AutoHeight().Padding(
	                         0, 3, 0, 0)[SNew(STextBlock)
	                                         .Text(FText::FromString(Summary))
	                                         .Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
	                                         .AutoWrapText(true)] +
	                     SVerticalBox::Slot().AutoHeight().Padding(0, 3, 0, 0)
	                         [SNew(STextBlock)
	                              .Text(FText::FromString(Entry->Failure))
	                              .ColorAndOpacity(HeadingColor)
	                              .Visibility(Entry->Failure.IsEmpty() ? EVisibility::Collapsed : EVisibility::Visible)
	                              .AutoWrapText(true)]]
	                .BodyContent()[SNew(SVerticalBox) +
	                               SVerticalBox::Slot().AutoHeight().Padding(
	                                   0, 8, 0, 6)[SNew(STextBlock)
	                                                   .Text(FText::FromString(Metadata))
	                                                   .AutoWrapText(true)
	                                                   .Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))] +
	                               SVerticalBox::Slot()
	                                   .AutoHeight()[SNew(STextBlock)
	                                                     .Text(FText::FromString(Entry->Payload))
	                                                     .AutoWrapText(true)
	                                                     .Font(FCoreStyle::GetDefaultFontStyle("Mono", 9))]]];
}

void UOWTAttributeDetailsWidget::OnEditorEvent(FName Event, const FString& Json)
{
	if (Event != TEXT("RequestRejected"))
	{
		const AVTBAttributeEditor* Editor = AttributeEditor.Get();
		if (!Editor)
		{
			return;
		}

		// Transport events invalidate the display; the typed store remains the state authority.
		ApplySnapshot(Editor->GetSnapshot());
		return;
	}

	TSharedPtr<FJsonObject> Rejection;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Json);
	if (!FJsonSerializer::Deserialize(Reader, Rejection))
	{
		return;
	}
	if (!Rejection.IsValid())
	{
		return;
	}
	Rejection->TryGetStringField(TEXT("reason"), LastError);
}

void UOWTAttributeDetailsWidget::OnSliderBegin(EOWTTransformField Field)
{
	if (bApplyingSnapshot)
	{
		return;
	}
	if (!CanEditFields())
	{
		return;
	}
	AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (!Editor)
	{
		return;
	}

	LastError.Reset();
	const FOWTAttributeSnapshot Expected = Snapshot;
	const FGuid StartedOperation = FGuid::NewGuid();
	const double InitialValue = GetFieldValue(Field);
	OperationSnapshot = Expected;
	OperationId = StartedOperation;
	ActiveField = Field;
	bSliderActive = true;
	bool bAccepted = false;
	{
		// The request refreshes external changes before it starts the operation.
		TGuardValue<bool> StartingSlider(bStartingSlider, true);
		bAccepted = Editor->RequestTransformField(Expected, Field, InitialValue, EOWTTransformEditPhase::Begin,
		                                          StartedOperation);
	}
	if (!bAccepted)
	{
		ResetSliderOperation();
		return;
	}
	if (!IsValid(Editor))
	{
		ResetSliderOperation();
		return;
	}

	const FOWTAttributeSnapshot Current = Editor->GetSnapshot();
	if (IsSliderBeginCurrent(*Editor, Current, StartedOperation))
	{
		return;
	}

	ResetSliderOperation();
	if (Current.bIsModifying)
	{
		// A callback may have closed or rebound the view. Only cancel the operation we just requested.
		Editor->RequestTransformField(Expected, Field, InitialValue, EOWTTransformEditPhase::Cancel, StartedOperation);
	}
}

void UOWTAttributeDetailsWidget::OnSliderValueChanged(double Value, EOWTTransformField Field)
{
	if (bApplyingSnapshot)
	{
		return;
	}
	if (!bSliderActive)
	{
		return;
	}
	if (Field != ActiveField)
	{
		return;
	}
	AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (!Editor)
	{
		return;
	}

	Editor->RequestTransformField(OperationSnapshot, Field, Value, EOWTTransformEditPhase::Update, OperationId);
}

void UOWTAttributeDetailsWidget::OnSliderEnd(double Value, EOWTTransformField Field)
{
	if (bApplyingSnapshot)
	{
		return;
	}
	if (!bSliderActive)
	{
		return;
	}
	if (Field != ActiveField)
	{
		return;
	}

	const FGuid CommittedOperation = OperationId;
	const FOWTAttributeSnapshot Expected = OperationSnapshot;
	ResetSliderOperation();
	AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (Editor)
	{
		Editor->RequestTransformField(Expected, Field, Value, EOWTTransformEditPhase::Commit, CommittedOperation);
	}
}

void UOWTAttributeDetailsWidget::OnValueCommitted(double Value, ETextCommit::Type CommitType, EOWTTransformField Field)
{
	if (bApplyingSnapshot)
	{
		return;
	}
	if (bSliderActive)
	{
		return;
	}
	if (CommitType == ETextCommit::OnCleared)
	{
		return;
	}
	if (!CanEditFields())
	{
		return;
	}
	if (FMath::IsNearlyEqual(Value, GetFieldValue(Field), 1.e-9))
	{
		return;
	}
	AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (!Editor)
	{
		return;
	}

	LastError.Reset();
	Editor->RequestTransformField(Snapshot, Field, Value, EOWTTransformEditPhase::Commit, FGuid::NewGuid());
}

FReply UOWTAttributeDetailsWidget::OnDuplicateClicked()
{
	if (!CanDuplicateSelection())
	{
		return FReply::Handled();
	}
	AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (!Editor)
	{
		return FReply::Handled();
	}

	LastError.Reset();
	Editor->RequestDuplicate(Snapshot);
	return FReply::Handled();
}

FReply UOWTAttributeDetailsWidget::OnDetailsClicked()
{
	SetMonitorVisible(false);
	return FReply::Handled();
}

FReply UOWTAttributeDetailsWidget::OnMonitorClicked()
{
	SetMonitorVisible(true);
	return FReply::Handled();
}

FReply UOWTAttributeDetailsWidget::OnPauseMonitorClicked()
{
	bMonitorPaused = !bMonitorPaused;
	RefreshMonitorHistory(true);
	return FReply::Handled();
}

FReply UOWTAttributeDetailsWidget::OnClearMonitorClicked()
{
	const AVTBAttributeEditor* Editor = AttributeEditor.Get();
	ClearedMonitorSequence = Editor ? Editor->GetLatestEventSequence() : LastMonitorSequence;
	MonitorEntries.Reset();
	RefreshMonitorFilter();
	return FReply::Handled();
}

bool UOWTAttributeDetailsWidget::CanEditFields() const
{
	if (!AttributeEditor.IsValid())
	{
		return false;
	}
	if (!Snapshot.bEditingEnabled)
	{
		return false;
	}
	if (!Snapshot.bCanEditTransform)
	{
		return false;
	}
	if (Snapshot.bIsModifying)
	{
		return bSliderActive;
	}

	return true;
}

bool UOWTAttributeDetailsWidget::CanDuplicateSelection() const
{
	if (!Snapshot.bEditingEnabled)
	{
		return false;
	}
	if (!Snapshot.bHasSelection)
	{
		return false;
	}
	if (Snapshot.bIsModifying)
	{
		return false;
	}

	return AttributeEditor.IsValid();
}

bool UOWTAttributeDetailsWidget::IsSliderBeginCurrent(const AVTBAttributeEditor& Editor,
                                                      const FOWTAttributeSnapshot& Current,
                                                      FGuid StartedOperation) const
{
	if (!bSliderActive)
	{
		return false;
	}
	if (!OperationId.IsValid())
	{
		return false;
	}
	if (OperationId != StartedOperation)
	{
		return false;
	}
	if (AttributeEditor.Get() != &Editor)
	{
		return false;
	}
	if (!Current.bEditingEnabled)
	{
		return false;
	}
	if (Current.EditorId != OperationSnapshot.EditorId)
	{
		return false;
	}
	if (Current.ObjectId != OperationSnapshot.ObjectId)
	{
		return false;
	}
	if (Current.SelectionRevision != OperationSnapshot.SelectionRevision)
	{
		return false;
	}

	return Current.bIsModifying;
}

bool UOWTAttributeDetailsWidget::HasFieldCapture() const
{
	for (const TSharedPtr<SSpinBox<double>>& Field : Fields)
	{
		if (!Field.IsValid())
		{
			continue;
		}
		if (Field->HasMouseCapture())
		{
			return true;
		}
	}

	return false;
}

EVisibility UOWTAttributeDetailsWidget::GetTransformVisibility() const
{
	return Snapshot.bHasSelection ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility UOWTAttributeDetailsWidget::GetDetailsVisibility() const
{
	return bMonitorVisible ? EVisibility::Collapsed : EVisibility::Visible;
}

EVisibility UOWTAttributeDetailsWidget::GetMonitorVisibility() const
{
	return bMonitorVisible ? EVisibility::Visible : EVisibility::Collapsed;
}

FText UOWTAttributeDetailsWidget::GetStateOverview() const
{
	const TCHAR* Editing = Snapshot.bEditingEnabled ? TEXT("ON") : TEXT("OFF");
	const TCHAR* Modifying = Snapshot.bIsModifying ? TEXT("MODIFYING") : TEXT("Idle");
	const TCHAR* Dirty = Snapshot.bHasChanges ? TEXT("Yes") : TEXT("No");
	return FText::FromString(FString::Printf(
	    TEXT("Editing %s  |  %s  |  Changed %s\nMode %s  |  Gizmo %s / %s\nState rev %d  |  Selection rev %d"), Editing,
	    Modifying, Dirty, Snapshot.ActiveMode.IsEmpty() ? TEXT("-") : *Snapshot.ActiveMode,
	    Snapshot.GizmoMode.IsEmpty() ? TEXT("-") : *Snapshot.GizmoMode,
	    Snapshot.GizmoCoordinateSystem.IsEmpty() ? TEXT("-") : *Snapshot.GizmoCoordinateSystem, Snapshot.StateRevision,
	    Snapshot.SelectionRevision));
}

FText UOWTAttributeDetailsWidget::GetStateIdentity() const
{
	if (!AttributeEditor.IsValid())
	{
		return LOCTEXT("MonitorDisconnected", "Editor disconnected");
	}
	if (!Snapshot.bHasSelection)
	{
		return LOCTEXT("MonitorNoSelection", "Selection: none");
	}
	return FText::FromString(TEXT("Selection: ") + Snapshot.ObjectName + TEXT("\nID: ") + Snapshot.ObjectId);
}

FText UOWTAttributeDetailsWidget::GetMonitorStatus() const
{
	const TCHAR* State = bMonitorPaused ? TEXT("PAUSED (state stays live)") : TEXT("LIVE");
	if (FilteredEntries.IsEmpty())
	{
		return FText::FromString(
		    FString::Printf(TEXT("%s  |  No matching events\nNewest first; latest 256 retained."), State));
	}
	return FText::FromString(
	    FString::Printf(TEXT("%s  |  %d / %d events\nNewest first; latest 256 retained. Expand for JSON."), State,
	                    FilteredEntries.Num(), MonitorEntries.Num()));
}

FText UOWTAttributeDetailsWidget::GetPauseMonitorText() const
{
	return bMonitorPaused ? LOCTEXT("ResumeMonitor", "Resume") : LOCTEXT("PauseMonitor", "Pause display");
}

double UOWTAttributeDetailsWidget::GetFieldValue(EOWTTransformField Field) const
{
	const FVector Location = Snapshot.Transform.GetLocation();
	const FRotator Rotation = Snapshot.Transform.Rotator();
	const FVector Scale = Snapshot.Transform.GetScale3D();
	switch (Field)
	{
	case EOWTTransformField::LocationX:
		return Location.X;
	case EOWTTransformField::LocationY:
		return Location.Y;
	case EOWTTransformField::LocationZ:
		return Location.Z;
	case EOWTTransformField::RotationRoll:
		return Rotation.Roll;
	case EOWTTransformField::RotationPitch:
		return Rotation.Pitch;
	case EOWTTransformField::RotationYaw:
		return Rotation.Yaw;
	case EOWTTransformField::ScaleX:
		return Scale.X;
	case EOWTTransformField::ScaleY:
		return Scale.Y;
	case EOWTTransformField::ScaleZ:
		return Scale.Z;
	}

	checkNoEntry();
	return 0;
}

FText UOWTAttributeDetailsWidget::GetObjectName() const
{
	if (!Snapshot.bHasSelection)
	{
		return LOCTEXT("NoSelection", "No actor selected");
	}

	return FText::FromString(Snapshot.ObjectName);
}

FText UOWTAttributeDetailsWidget::GetObjectClass() const
{
	return FText::FromString(Snapshot.ObjectClass);
}

FText UOWTAttributeDetailsWidget::GetStatusText() const
{
	if (!Snapshot.bHasSelection)
	{
		return LOCTEXT("SelectionHint", "Select an actor in the scene to view its transform.");
	}
	if (!Snapshot.bCanEditTransform)
	{
		return FText::FromString(Snapshot.DisabledReason);
	}
	if (Snapshot.bIsModifying)
	{
		return LOCTEXT("Modifying", "Editing transform...");
	}
	if (Snapshot.bHasChanges)
	{
		return LOCTEXT("Modified", "Modified in this session");
	}

	return LOCTEXT("Unmodified", "No changes in this session");
}

FText UOWTAttributeDetailsWidget::GetErrorText() const
{
	return FText::FromString(LastError);
}

#undef LOCTEXT_NAMESPACE
