#include "UI/OWTAttributeDetailsWidget.h"

#include "UI/OWTAttributeMonitorTypes.h"
#include "OWTStateMonitorModel.h"
#include "SOWTStateMonitor.h"

#include "VTBAttributeEditor.h"
#include "VTBOWTEditorSubsystem.h"
#include "Dom/JsonObject.h"
#include "Events/OWTEventTypes.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
#include "HAL/PlatformTime.h"
#include "InputCoreTypes.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "OWTAttributeDetails"

UOWTAttributeDetailsWidget::UOWTAttributeDetailsWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	AttributeEditor(),
	Panel(),
	ToolRows(),
	MonitorModel(),
	MonitorView(),
	Fields(),
	Snapshot(),
	OperationSnapshot(),
	ModeSnapshot(),
	AvailableTools(),
	Subscription(),
	OperationId(),
	ActiveField(EOWTTransformField::LocationX),
	LastError(),
	SubscriptionGeneration(0),
	LastMonitorSequence(-1),
	LastMonitorRefreshTime(0.0),
	bApplyingSnapshot(false),
	bSliderActive(false),
	bStartingSlider(false),
	bMonitorVisible(false)
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UOWTAttributeDetailsWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	FinishSliderOperation();
	ReleaseFieldCapture();
	Panel.Reset();
	ToolRows.Reset();
	MonitorView.Reset();
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
	RefreshSessionState();
}

void UOWTAttributeDetailsWidget::SetMonitorVisible(bool bVisible)
{
	FinishSliderOperation();
	ReleaseFieldCapture();
	ClearPanelFocus();
	bMonitorVisible = bVisible;
	RefreshPanelVisibility();
	RefreshSessionState();
	RefreshMonitorState(true);
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
	AddHeaderRows(*Contents);
	Contents->AddSlot().AutoHeight().Padding(0, 0, 0, 12)[BuildNavigationTabs()];
	Contents->AddSlot().FillHeight(1)[SNew(SOverlay)
	+ SOverlay::Slot()
	[
		BuildDetails()
	]
	+ SOverlay::Slot()
	[
		BuildMonitor()
	]];
	Contents->AddSlot().AutoHeight().Padding(0, 10, 0, 0)[SNew(STextBlock).Text(LOCTEXT("PanelHelp", "F2: edit on/off     F3: state monitor")).ColorAndOpacity(FLinearColor(0.62f, 0.67f, 0.73f))];

	return SNew(SOverlay).Visibility(EVisibility::SelfHitTestInvisible)
		+ SOverlay::Slot().HAlign(HAlign_Right).VAlign(VAlign_Fill).Padding(12)
		[
			SNew(SBox).WidthOverride_Lambda([this]() { return bMonitorVisible ? 620.0f : 420.0f; })
			[
				SAssignNew(Panel, SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
				.BorderBackgroundColor(FLinearColor(0.035f, 0.045f, 0.06f, 0.97f))
				.Padding(16)
				.OnMouseButtonDown_Lambda([](const FGeometry&, const FPointerEvent&){
					return FReply::Handled();
				})
				.OnMouseButtonUp_Lambda([](const FGeometry&, const FPointerEvent&){
					return FReply::Handled();
				})
				[
					Contents
				]
			]
		];
}

void UOWTAttributeDetailsWidget::AddHeaderRows(SVerticalBox& Contents)
{
	Contents.AddSlot().AutoHeight().Padding(0, 0, 0, 10)
	[
		SNew(STextBlock)
		.Text(LOCTEXT("Title", "RUNTIME EDITOR"))
		.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
	];
	Contents.AddSlot().AutoHeight().Padding(0, 0, 0, 6)
	[
		SNew(STextBlock)
		.Text_UObject(this, &UOWTAttributeDetailsWidget::GetStateOverview)
		.AutoWrapText(true)
	];
	Contents.AddSlot().AutoHeight().Padding(0, 0, 0, 6)
	[
		SNew(STextBlock)
		.Text_UObject(this, &UOWTAttributeDetailsWidget::GetModeStatus)
		.AutoWrapText(true)
	];
	Contents.AddSlot().AutoHeight().Padding(0, 0, 0, 12)
	[
		SNew(STextBlock)
		.Text_UObject(this, &UOWTAttributeDetailsWidget::GetStateIdentity)
		.ToolTipText_Lambda([this]()
		{
			return FText::FromString(TEXT("Editor: ") + Snapshot.EditorId + TEXT("\nObject: ") + Snapshot.ObjectId);
		})
		.ColorAndOpacity(FLinearColor(0.62f, 0.67f, 0.73f))
		.AutoWrapText(true)
	];
}

TSharedRef<SWidget> UOWTAttributeDetailsWidget::BuildNavigationTabs()
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot().FillWidth(1)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
			.ContentPadding(FMargin(8, 6))
			.OnClicked_UObject(this, &UOWTAttributeDetailsWidget::OnDetailsClicked)
			[
				SNew(STextBlock).Text(LOCTEXT("DetailsTab", "Details"))
			]
		]
		+ SHorizontalBox::Slot().FillWidth(1).Padding(4, 0, 0, 0)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
			.ContentPadding(FMargin(8, 6))
			.OnClicked_UObject(this, &UOWTAttributeDetailsWidget::OnMonitorClicked)
			[
				SNew(STextBlock).Text(LOCTEXT("EventsTab", "Monitor  /  F3"))
			]
		];
}

TSharedRef<SWidget> UOWTAttributeDetailsWidget::BuildDetails()
{
	TSharedRef<SVerticalBox> Contents = SNew(SVerticalBox);
	Contents->AddSlot().AutoHeight().Padding(0, 0, 0, 4)[
		SNew(STextBlock).Text_UObject(this, &UOWTAttributeDetailsWidget::GetObjectName).Font(FCoreStyle::GetDefaultFontStyle("Bold", 12)).AutoWrapText(true)];
	Contents->AddSlot().AutoHeight().Padding(0, 0, 0, 12)[SNew(STextBlock)
	.Text_UObject(this, &UOWTAttributeDetailsWidget::GetObjectClass)
	.ColorAndOpacity(FLinearColor(0.62f, 0.67f, 0.73f))
	.AutoWrapText(true)];
	Contents->AddSlot().AutoHeight().Padding(0, 0, 0, 14)[SNew(STextBlock).Text_UObject(this, &UOWTAttributeDetailsWidget::GetStatusText).AutoWrapText(true)];
	Contents->AddSlot().AutoHeight()[BuildTransformSection(LOCTEXT("Location", "Location  /  cm"), EOWTTransformField::LocationX, 1.0)];
	Contents->AddSlot().AutoHeight()[BuildTransformSection(LOCTEXT("Rotation", "Rotation  /  degrees"), EOWTTransformField::RotationRoll, 1.0)];
	Contents->AddSlot().AutoHeight()[BuildTransformSection(LOCTEXT("Scale", "Scale"), EOWTTransformField::ScaleX, 0.01)];
	Contents->AddSlot().AutoHeight().Padding(0, 4, 0, 12)[BuildTools()];
	Contents->AddSlot().AutoHeight().Padding(0, 0, 0, 12)[
		SNew(STextBlock).Text_UObject(this, &UOWTAttributeDetailsWidget::GetErrorText).ColorAndOpacity(FLinearColor(1.0f, 0.58f, 0.40f)).AutoWrapText(true)];
	Contents->AddSlot().AutoHeight()[SNew(STextBlock)
	.Text(LOCTEXT("Help", "World transform\nClick a value to type, or drag to adjust.\nShift: " "faster   Ctrl: finer   Esc: cancel drag"))
	.ColorAndOpacity(FLinearColor(0.62f, 0.67f, 0.73f))
	.AutoWrapText(true)];

	return SNew(SScrollBox).Visibility_UObject(this, &UOWTAttributeDetailsWidget::GetDetailsVisibility).ConsumeMouseWheel(EConsumeMouseWheel::Always)
		+ SScrollBox::Slot()
		[
			Contents
		];
}

TSharedRef<SWidget> UOWTAttributeDetailsWidget::BuildMonitor()
{
	if (!MonitorModel.IsValid())
	{
		FOWTStateMonitorSettings Settings;
		Settings.MaxNodesPerSnapshot = 4096;
		Settings.MaxTextCharacters = 16384;
		MonitorModel = MakeShared<FOWTStateMonitorModel>(Settings);
	}

	RefreshMonitorState(true);
	return SAssignNew(MonitorView, SOWTStateMonitor)
		.Model(MonitorModel)
		.Visibility_UObject(this, &UOWTAttributeDetailsWidget::GetMonitorVisibility);
}

TSharedRef<SWidget> UOWTAttributeDetailsWidget::BuildTools()
{
	SAssignNew(ToolRows, SVerticalBox);
	RefreshToolRows(AvailableTools);
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight().Padding(0, 0, 0, 6)
		[
			SNew(STextBlock).Text(LOCTEXT("Tools", "Tools")).Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))
		]
		+ SVerticalBox::Slot().AutoHeight()
		[
			ToolRows.ToSharedRef()
		]
		+ SVerticalBox::Slot().AutoHeight().Padding(0, 6, 0, 0)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().FillWidth(1)
			[
				SNew(SButton).HAlign(HAlign_Center).IsEnabled_UObject(this, &UOWTAttributeDetailsWidget::CanEndTool, true).OnClicked_UObject(this, &UOWTAttributeDetailsWidget::OnEndToolClicked, true)
				[
					SNew(STextBlock).Text(LOCTEXT("AcceptTool", "Accept tool"))
				]
			]
			+ SHorizontalBox::Slot().FillWidth(1).Padding(4, 0, 0, 0)
			[
				SNew(SButton).HAlign(HAlign_Center).IsEnabled_UObject(this, &UOWTAttributeDetailsWidget::CanEndTool, false).OnClicked_UObject(this, &UOWTAttributeDetailsWidget::OnEndToolClicked,
					false)
				[
					SNew(STextBlock).Text(LOCTEXT("CancelTool", "Cancel tool"))
				]
			]
		];
}

void UOWTAttributeDetailsWidget::RefreshSessionState()
{
	const AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (!Editor)
	{
		ModeSnapshot = FOWTModeSnapshot();
		RefreshToolRows({});
		return;
	}

	// These are observed values. Event payloads and the journal pause state never overwrite them.
	ModeSnapshot = Editor->GetModeSnapshot();
	RefreshToolRows(Editor->GetAvailableTools());
}

void UOWTAttributeDetailsWidget::RefreshToolRows(const TArray<FOWTToolAvailability>& InTools)
{
	const bool bChanged = HaveToolRowsChanged(InTools);
	if (bChanged)
	{
		AvailableTools = InTools;
	}
	if (!ToolRows.IsValid())
	{
		return;
	}
	if (!bChanged)
	{
		if (ToolRows->NumSlots() == InTools.Num())
		{
			return;
		}
	}

	ToolRows->ClearChildren();
	for (const FOWTToolAvailability& Tool : AvailableTools)
	{
		FText Label = Tool.Label;
		if (Tool.bActive)
		{
			Label = FText::Format(LOCTEXT("ActiveTool", "{0}  (active)"), Label);
		}
		ToolRows->AddSlot().AutoHeight().Padding(0, 2)[SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
			.ContentPadding(FMargin(12, 6))
			.ToolTipText(Tool.Reason.IsEmpty() ? Tool.Category : Tool.Reason)
			.IsEnabled_UObject(this, &UOWTAttributeDetailsWidget::CanStartTool, Tool.ToolId)
			.OnClicked_UObject(this, &UOWTAttributeDetailsWidget::OnToolClicked, Tool.ToolId)
			[
				SNew(STextBlock).Text(Label)
			]
		]
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(STextBlock).Text(Tool.Reason).ColorAndOpacity(FLinearColor(0.62f, 0.67f, 0.73f)).Visibility(Tool.Reason.IsEmpty() ? EVisibility::Collapsed : EVisibility::Visible).AutoWrapText(true)
		]];
	}
}

bool UOWTAttributeDetailsWidget::HaveToolRowsChanged(const TArray<FOWTToolAvailability>& InTools) const
{
	if (AvailableTools.Num() != InTools.Num())
	{
		return true;
	}

	for (int32 Index = 0; Index < InTools.Num(); ++Index)
	{
		const FOWTToolAvailability& Previous = AvailableTools[Index];
		const FOWTToolAvailability& Current = InTools[Index];
		if (Previous.ToolId != Current.ToolId)
		{
			return true;
		}
		if (!Previous.Label.EqualTo(Current.Label))
		{
			return true;
		}
		if (!Previous.Category.EqualTo(Current.Category))
		{
			return true;
		}
		if (!Previous.Reason.EqualTo(Current.Reason))
		{
			return true;
		}
		if (Previous.bEnabled != Current.bEnabled)
		{
			return true;
		}
		if (Previous.bActive != Current.bActive)
		{
			return true;
		}
	}
	return false;
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
			ResetMonitor();
		}
		RefreshSessionState();
		return;
	}
	RefreshSessionState();
	RefreshMonitorState();
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
	if (InKeyEvent.IsRepeat())
	{
		if (InKeyEvent.GetKey() == EKeys::F2)
		{
			return FReply::Handled();
		}
		if (InKeyEvent.GetKey() == EKeys::F3)
		{
			return FReply::Handled();
		}
	}
	if (InKeyEvent.GetKey() == EKeys::F3)
	{
		ToggleMonitor();
		return FReply::Handled();
	}
	if (InKeyEvent.GetKey() == EKeys::F2)
	{
		AVTBAttributeEditor* Editor = AttributeEditor.Get();
		if (Editor)
		{
			UWorld* World = Editor->GetWorld();
			if (World)
			{
				UVTBOWTEditorSubsystem* Subsystem = World->GetSubsystem<UVTBOWTEditorSubsystem>();
				if (Subsystem)
				{
					Subsystem->ToggleEditing();
					return FReply::Handled();
				}
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

TSharedRef<SWidget> UOWTAttributeDetailsWidget::BuildTransformSection(const FText& Title, EOWTTransformField FirstField, double Delta)
{
	TSharedRef<SVerticalBox> Rows = SNew(SVerticalBox);
	Rows->AddSlot().AutoHeight().Padding(0, 0, 0, 6)[SNew(STextBlock).Text(Title).Font(FCoreStyle::GetDefaultFontStyle("Bold", 11))];
	const bool bRotation = FirstField == EOWTTransformField::RotationRoll;
	const FText Labels[] = {LOCTEXT("X", "X"), LOCTEXT("Y", "Y"), LOCTEXT("Z", "Z")};
	const FText RotationLabels[] = {LOCTEXT("Roll", "Roll / X"), LOCTEXT("Pitch", "Pitch / Y"), LOCTEXT("Yaw", "Yaw / Z")};
	for (int32 Axis = 0; Axis < 3; ++Axis)
	{
		const EOWTTransformField Field = static_cast<EOWTTransformField>(static_cast<uint8>(FirstField) + Axis);
		AddTransformField(*Rows, Field, bRotation ? RotationLabels[Axis] : Labels[Axis], Delta);
	}

	return SNew(SBox).Visibility_UObject(this, &UOWTAttributeDetailsWidget::GetTransformVisibility).Padding(FMargin(0, 0, 0, 14))
		[
			Rows
		];
}

void UOWTAttributeDetailsWidget::AddTransformField(SVerticalBox& Rows, EOWTTransformField Field, const FText& Label, double Delta)
{
	TSharedPtr<SSpinBox<double>> SpinBox;
	Rows.AddSlot().AutoHeight().Padding(0, 2)[SNew(SHorizontalBox)
	+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
	[
		SNew(SBox).WidthOverride(64)
		[
			SNew(STextBlock).Text(Label)
		]
	]
	+ SHorizontalBox::Slot().FillWidth(1)
	[
		SAssignNew(SpinBox, SSpinBox<double>)
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
		.OnValueCommitted_UObject(this, &UOWTAttributeDetailsWidget::OnValueCommitted, Field)
	]];
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
	const FGuid NewSubscription = Editor->Subscribe(this, FOWTAttributeEventNative::CreateUObject(this, &UOWTAttributeDetailsWidget::OnEditorEvent));
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
	SetVisibility(Snapshot.bEditingEnabled || bMonitorVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UOWTAttributeDetailsWidget::RefreshMonitorState(bool bForce)
{
	if (!bMonitorVisible)
	{
		return;
	}
	if (!MonitorModel.IsValid())
	{
		return;
	}
	const AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (!Editor)
	{
		MonitorModel->Reset();
		return;
	}

	const double Now = FPlatformTime::Seconds();
	if (!bForce)
	{
		if (Now - LastMonitorRefreshTime < 0.1)
		{
			return;
		}
	}
	LastMonitorRefreshTime = Now;

	// Capture the authoritative typed stores directly. JSON events are transport evidence only.
	FOWTAttributeMonitorSnapshot Current;
	Current.Selection = Editor->GetSnapshot();
	Current.Mode = Editor->GetModeSnapshot();
	Current.Tools = Editor->GetAvailableTools();
	Current.DuplicationOperations = Editor->GetDuplicationOperations();
	Current.ProceduralComponents = Editor->GetProceduralComponents();
	MonitorModel->SubmitSnapshot(TEXT("Current"), LOCTEXT("CurrentSource", "Current editor state"),
		FOWTAttributeMonitorSnapshot::StaticStruct(), &Current);

	const int64 LatestSequence = Editor->GetLatestEventSequence();
	if (LatestSequence == LastMonitorSequence)
	{
		return;
	}

	FOWTAttributeMonitorEvents Events;
	Events.Journal = Editor->GetMonitorEntries(256);
	MonitorModel->SubmitSnapshot(TEXT("Events"), LOCTEXT("EventsSource", "JSON event journal"),
		FOWTAttributeMonitorEvents::StaticStruct(), &Events, false);
	LastMonitorSequence = LatestSequence;
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
		Editor->RequestTransformField(Expected, Field, GetFieldValue(Field), EOWTTransformEditPhase::Cancel, CancelledOperation);
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
	LastMonitorSequence = -1;
	LastMonitorRefreshTime = 0.0;
	if (MonitorModel.IsValid())
	{
		MonitorModel->Reset();
	}
	if (MonitorView.IsValid())
	{
		MonitorView->SetHistoryPaused(false);
	}
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
		RefreshSessionState();
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
		bAccepted = Editor->RequestTransformField(Expected, Field, InitialValue, EOWTTransformEditPhase::Begin, StartedOperation);
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

FReply UOWTAttributeDetailsWidget::OnToolClicked(FName ToolId)
{
	if (!CanStartTool(ToolId))
	{
		return FReply::Handled();
	}
	AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (!Editor)
	{
		return FReply::Handled();
	}

	LastError.Reset();
	Editor->RequestStartTool(ToolId);
	RefreshSessionState();
	return FReply::Handled();
}

FReply UOWTAttributeDetailsWidget::OnEndToolClicked(bool bAccept)
{
	if (!CanEndTool(bAccept))
	{
		return FReply::Handled();
	}
	AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (!Editor)
	{
		return FReply::Handled();
	}

	Editor->RequestEndTool(bAccept);
	RefreshSessionState();
	return FReply::Handled();
}

bool UOWTAttributeDetailsWidget::CanStartTool(FName ToolId) const
{
	if (!AttributeEditor.IsValid())
	{
		return false;
	}
	if (!ModeSnapshot.bEditingEnabled)
	{
		return false;
	}
	if (!ModeSnapshot.bCanStartTools)
	{
		return false;
	}
	if (bSliderActive)
	{
		return false;
	}
	for (const FOWTToolAvailability& Tool : AvailableTools)
	{
		if (Tool.ToolId == ToolId)
		{
			return Tool.bEnabled;
		}
	}
	return false;
}

bool UOWTAttributeDetailsWidget::CanEndTool(bool bAccept) const
{
	const AVTBAttributeEditor* Editor = AttributeEditor.Get();
	if (!Editor)
	{
		return false;
	}
	if (bAccept)
	{
		return Editor->CanAcceptActiveTool();
	}
	return Editor->CanCancelActiveTool();
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
	if (!ModeSnapshot.bCanStartTools)
	{
		return false;
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

bool UOWTAttributeDetailsWidget::IsSliderBeginCurrent(const AVTBAttributeEditor& Editor, const FOWTAttributeSnapshot& Current, FGuid StartedOperation) const
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
	return FText::FromString(FString::Printf(TEXT("Editing %s  |  %s  |  Changed %s\nGizmo %s / %s\nState rev %d  |  Selection rev %d"), Editing, Modifying, Dirty,
		Snapshot.GizmoMode.IsEmpty() ? TEXT("-") : *Snapshot.GizmoMode, Snapshot.GizmoCoordinateSystem.IsEmpty() ? TEXT("-") : *Snapshot.GizmoCoordinateSystem, Snapshot.StateRevision,
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

FText UOWTAttributeDetailsWidget::GetModeStatus() const
{
	FString Text = FString::Printf(TEXT("Mode %s  |  %s\nTool %s  |  Mode rev %d"), *ModeSnapshot.ModeId.ToString(), *ModeSnapshot.Lifecycle.ToString(), *ModeSnapshot.ActiveToolId.ToString(),
		ModeSnapshot.Revision);
	if (!ModeSnapshot.DisabledReason.IsEmpty())
	{
		Text += TEXT("\n") + ModeSnapshot.DisabledReason;
	}
	return FText::FromString(Text);
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
