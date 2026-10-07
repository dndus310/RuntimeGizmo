#include "VTBAttributeEditor.h"

#include "AttributeEditor/OWTAttributeJson.h"

#include "Components/SceneComponent.h"
#include "BaseGizmos/CombinedTransformGizmo.h"
#include "State/OWTAttributeStateStore.h"
#include "Dom/JsonObject.h"
#include "Modes/OWTAttributeEditMode.h"
#include "Engine/World.h"
#include "Events/OWTNotificationCenter.h"
#include "VTBOWTEditorSubsystem.h"

AVTBAttributeEditor::AVTBAttributeEditor()
    : Notifications(nullptr), StateStore(nullptr), DuplicateWorldOffset(100.0, 0.0, 0.0), Subsystem(), BoundMode(),
      ObservedSelection(), OperationActor(), ProcessedRequestIds(), CompletedOperations(),
      ReportedDuplicateOperations(), LastModeSnapshot(), OperationStart(FTransform::Identity), EditorId(), SelectedId(),
      ActiveOperation(), SelectionRevision(0), StateRevision(0), bGizmoOperation(false), bApplyingTransform(false),
      bHandlingRequest(false), bEndingPlay(false)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AVTBAttributeEditor::BeginPlay()
{
	Super::BeginPlay();
	BindSubsystem(GetWorld()->GetSubsystem<UVTBOWTEditorSubsystem>());
}

void AVTBAttributeEditor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bEndingPlay = true;
	ClearOperation();
	UnbindMode();

	if (Notifications)
	{
		Notifications->Shutdown();
	}
	if (UVTBOWTEditorSubsystem* EditorSubsystem = Subsystem.Get())
	{
		if (EditorSubsystem->GetAttributeEditor() == this)
		{
			EditorSubsystem->RegisterAttributeEditor(nullptr);
		}
	}
	Subsystem.Reset();
	if (StateStore)
	{
		StateStore->Reset();
	}
	Super::EndPlay(EndPlayReason);
}

void AVTBAttributeEditor::BindSubsystem(UVTBOWTEditorSubsystem* InSubsystem)
{
	check(IsInGameThread());
	if (!IsValid(InSubsystem))
	{
		return;
	}
	if (InSubsystem->GetWorld() != GetWorld())
	{
		return;
	}
	if (bEndingPlay)
	{
		return;
	}
	if (Subsystem.Get() == InSubsystem)
	{
		if (Notifications)
		{
			return;
		}
	}

	UnbindMode();

	if (Notifications)
	{
		Notifications->Shutdown();
	}
	Subsystem = InSubsystem;
	EditorId = FGuid::NewGuid();
	Notifications = NewObject<UOWTNotificationCenter>(this);
	const bool bEventsInitialized = Notifications->Initialize(this);
	check(bEventsInitialized);
	StateStore = NewObject<UOWTAttributeStateStore>(this);
	InSubsystem->RegisterAttributeEditor(this);
	RefreshModeBindings();
	NotifySelectionChanged();
}

void AVTBAttributeEditor::RefreshModeBindings()
{
	UOWTAttributeEditMode* Mode = Subsystem.IsValid() ? Subsystem->GetAttributeEditMode() : nullptr;
	if (BoundMode.Get() == Mode)
	{
		return;
	}
	UnbindMode();
	if (!Mode)
	{
		return;
	}

	BoundMode = Mode;
	LastModeSnapshot = Mode->GetSnapshot();
	Mode->OnModeChanged.AddUObject(this, &ThisClass::OnModeChanged);
	Mode->OnDuplicationChanged.AddUObject(this, &ThisClass::OnDuplicationChanged);
	Mode->OnProceduralChanged.AddUObject(this, &ThisClass::OnProceduralChanged);
}

void AVTBAttributeEditor::UnbindMode()
{
	if (UOWTAttributeEditMode* Mode = BoundMode.Get())
	{
		Mode->OnModeChanged.RemoveAll(this);
		Mode->OnDuplicationChanged.RemoveAll(this);
		Mode->OnProceduralChanged.RemoveAll(this);
	}
	BoundMode.Reset();
}

FOWTModeSnapshot AVTBAttributeEditor::GetModeSnapshot() const
{
	const UOWTAttributeEditMode* Mode = Subsystem.IsValid() ? Subsystem->GetAttributeEditMode() : nullptr;
	return Mode ? Mode->GetSnapshot() : FOWTModeSnapshot();
}

TArray<FOWTDuplicationOperationSnapshot> AVTBAttributeEditor::GetDuplicationOperations() const
{
	const UOWTAttributeEditMode* Mode = Subsystem.IsValid() ? Subsystem->GetAttributeEditMode() : nullptr;
	return Mode ? Mode->GetDuplicationOperations() : TArray<FOWTDuplicationOperationSnapshot>();
}

TArray<FOWTProceduralComponentSnapshot> AVTBAttributeEditor::GetProceduralComponents() const
{
	const UOWTAttributeEditMode* Mode = Subsystem.IsValid() ? Subsystem->GetAttributeEditMode() : nullptr;
	return Mode ? Mode->GetProceduralComponents() : TArray<FOWTProceduralComponentSnapshot>();
}

TArray<FOWTToolAvailability> AVTBAttributeEditor::GetAvailableTools() const
{
	const UOWTAttributeEditMode* Mode = Subsystem.IsValid() ? Subsystem->GetAttributeEditMode() : nullptr;
	return Mode ? Mode->GetAvailableTools() : TArray<FOWTToolAvailability>();
}

bool AVTBAttributeEditor::CanAcceptActiveTool() const
{
	const UOWTAttributeEditMode* Mode = Subsystem.IsValid() ? Subsystem->GetAttributeEditMode() : nullptr;
	return Mode ? Mode->CanAcceptActiveTool() : false;
}

bool AVTBAttributeEditor::CanCancelActiveTool() const
{
	const UOWTAttributeEditMode* Mode = Subsystem.IsValid() ? Subsystem->GetAttributeEditMode() : nullptr;
	return Mode ? Mode->CanCancelActiveTool() : false;
}

bool AVTBAttributeEditor::RequestStartTool(FName ToolId)
{
	check(IsInGameThread());
	if (!IsReady())
	{
		return false;
	}
	RefreshModeBindings();
	if (!BoundMode.IsValid())
	{
		return false;
	}
	TSharedRef<FJsonObject> Request = MakeRequestObject(GetSnapshot());
	Request->SetStringField(TEXT("toolId"), ToolId.ToString());
	const FString RequestId = Request->GetStringField(TEXT("requestId"));
	Notifications->RecordEvent(TEXT("ToolStartRequested"), OWTAttributeJson::SerializeObject(Request),
	                           EOWTEventDirection::Inbound);
	if (ActiveOperation.IsValid())
	{
		EmitRejection(RequestId, TEXT("Busy"), TEXT("Finish the transform interaction before changing tools."));
		return false;
	}
	if (bHandlingRequest)
	{
		EmitRejection(RequestId, TEXT("Busy"), TEXT("Another edit request is being processed."));
		return false;
	}
	FString Error;
	if (!BoundMode->RequestToolStart(ToolId, Error))
	{
		if (IsReady())
		{
			EmitRejection(RequestId, TEXT("ToolUnavailable"), Error);
		}
		return false;
	}
	return true;
}

bool AVTBAttributeEditor::RequestEndTool(bool bAccept)
{
	check(IsInGameThread());
	if (!IsReady())
	{
		return false;
	}
	RefreshModeBindings();
	if (!BoundMode.IsValid())
	{
		return false;
	}
	TSharedRef<FJsonObject> Request = MakeRequestObject(GetSnapshot());
	Request->SetBoolField(TEXT("accept"), bAccept);
	const FString RequestId = Request->GetStringField(TEXT("requestId"));
	Notifications->RecordEvent(TEXT("ToolEndRequested"), OWTAttributeJson::SerializeObject(Request),
	                           EOWTEventDirection::Inbound);
	FString Error;
	if (!BoundMode->EndTool(bAccept, Error))
	{
		if (IsReady())
		{
			EmitRejection(RequestId, TEXT("ToolCannotEnd"), Error);
		}
		return false;
	}
	return true;
}

void AVTBAttributeEditor::OnModeChanged(const FOWTModeSnapshot& InSnapshot)
{
	const FOWTModeSnapshot Snapshot = InSnapshot;
	const TWeakObjectPtr<UOWTAttributeEditMode> EventMode = BoundMode;
	auto IsCurrentModeState = [this, EventMode, &Snapshot]()
	{
		if (!IsReady())
		{
			return false;
		}
		if (!EventMode.IsValid())
		{
			return false;
		}
		if (BoundMode != EventMode)
		{
			return false;
		}
		if (Subsystem->GetAttributeEditMode() != EventMode.Get())
		{
			return false;
		}
		return EventMode->GetSnapshot().Revision == Snapshot.Revision;
	};
	if (!IsCurrentModeState())
	{
		return;
	}

	const FName PreviousTool = LastModeSnapshot.ActiveToolId;
	LastModeSnapshot = Snapshot;
	++StateRevision;
	RebuildSnapshot();

	TSharedRef<FJsonObject> Object = MakeSnapshotObject(FString(), TEXT("Mode"));
	OWTAttributeJson::WriteModeSnapshot(*Object, Snapshot);
	if (PreviousTool != Snapshot.ActiveToolId)
	{
		if (!PreviousTool.IsNone())
		{
			Object->SetStringField(TEXT("toolId"), PreviousTool.ToString());
			Notifications->Publish(TEXT("ToolEnded"), OWTAttributeJson::SerializeObject(Object));
			if (!IsCurrentModeState())
			{
				return;
			}
		}
		if (!Snapshot.ActiveToolId.IsNone())
		{
			Object->SetStringField(TEXT("toolId"), Snapshot.ActiveToolId.ToString());
			Notifications->Publish(TEXT("ToolStarted"), OWTAttributeJson::SerializeObject(Object));
			if (!IsCurrentModeState())
			{
				return;
			}
		}
	}
	Object->SetStringField(TEXT("toolId"), Snapshot.ActiveToolId.ToString());
	Notifications->Publish(TEXT("EditorStateChanged"), OWTAttributeJson::SerializeObject(Object));
}

void AVTBAttributeEditor::OnDuplicationChanged(const FOWTDuplicationOperationSnapshot& InSnapshot)
{
	FOWTDuplicationOperationSnapshot Snapshot = InSnapshot;
	if (!IsReady())
	{
		return;
	}

	const TWeakObjectPtr<UOWTAttributeEditMode> OperationMode = BoundMode;
	const int32 PreviousSelectionRevision = SelectionRevision;
	const TWeakObjectPtr<AActor> PreviousSelection =
	    OperationMode.IsValid() ? OperationMode->GetSelectedObject() : nullptr;
	if (AActor* Original = Snapshot.SourceActor.Get())
	{
		Snapshot.OriginalObjectId = RegisterActor(*Original).ToString(EGuidFormats::DigitsWithHyphens);
	}
	if (Snapshot.Phase == EOWTDuplicationPhase::Committed)
	{
		if (AActor* Duplicate = Snapshot.DuplicateActor.Get())
		{
			Snapshot.DuplicateObjectId = RegisterActor(*Duplicate, true).ToString(EGuidFormats::DigitsWithHyphens);
		}
	}
	if (OperationMode.IsValid())
	{
		OperationMode->SetOperationObjectIds(Snapshot.OperationId, Snapshot.OriginalObjectId,
		                                     Snapshot.DuplicateObjectId);
	}

	TSharedRef<FJsonObject> Object = MakeSnapshotObject(Snapshot.RequestId, Snapshot.Source);
	OWTAttributeJson::WriteDuplicationSnapshot(*Object, Snapshot);
	Notifications->Publish(TEXT("DuplicateOperationChanged"), OWTAttributeJson::SerializeObject(Object));
	if (!IsReady())
	{
		return;
	}
	if (Snapshot.Phase != EOWTDuplicationPhase::Committed)
	{
		if (Snapshot.Phase == EOWTDuplicationPhase::Failed)
		{
			EmitRejection(Snapshot.RequestId, TEXT("DuplicateFailed"), Snapshot.Error);
		}
		return;
	}
	if (ReportedDuplicateOperations.Contains(Snapshot.OperationId))
	{
		return;
	}
	ReportedDuplicateOperations.Add(Snapshot.OperationId);
	AActor* Duplicate = Snapshot.DuplicateActor.Get();
	if (!IsValid(Duplicate))
	{
		EmitRejection(Snapshot.RequestId, TEXT("DuplicateInvalidated"),
		              TEXT("The duplicate was destroyed before commit notification."));
		return;
	}
	const TWeakObjectPtr<AActor> CreatedActor = Duplicate;
	auto CanSelectDuplicate = [this, OperationMode, PreviousSelection, PreviousSelectionRevision]()
	{
		if (!OperationMode.IsValid())
		{
			return false;
		}
		if (BoundMode != OperationMode)
		{
			return false;
		}
		if (Subsystem->GetAttributeEditMode() != OperationMode.Get())
		{
			return false;
		}
		if (!OperationMode->IsEntered())
		{
			return false;
		}
		if (SelectionRevision != PreviousSelectionRevision)
		{
			return false;
		}
		return OperationMode->GetSelectedObject() == PreviousSelection.Get();
	};
	if (CanSelectDuplicate())
	{
		OperationMode->SetSelectedObject(Duplicate);
	}
	if (!IsReady())
	{
		return;
	}
	if (!CreatedActor.IsValid())
	{
		EmitRejection(Snapshot.RequestId, TEXT("DuplicateInvalidated"),
		              TEXT("The duplicate was destroyed by a selection callback."));
		return;
	}
	++StateRevision;
	RebuildSnapshot();
	Object = MakeSnapshotObject(Snapshot.RequestId, Snapshot.Source);
	Object->SetStringField(TEXT("operationId"), Snapshot.OperationId.ToString(EGuidFormats::DigitsWithHyphens));
	Object->SetStringField(TEXT("originalObjectId"), Snapshot.OriginalObjectId);
	Object->SetStringField(TEXT("duplicateObjectId"), Snapshot.DuplicateObjectId);
	Object->SetStringField(TEXT("phase"), TEXT("Committed"));
	const TArray<FOWTProceduralComponentSnapshot> ProceduralComponents =
	    OperationMode.IsValid() ? OperationMode->GetProceduralComponents() : TArray<FOWTProceduralComponentSnapshot>();
	OWTAttributeJson::WriteProceduralStates(*Object, Snapshot.OperationId, ProceduralComponents);
	Notifications->Publish(TEXT("ObjectDuplicated"), OWTAttributeJson::SerializeObject(Object));
}

void AVTBAttributeEditor::OnProceduralChanged(const FOWTProceduralComponentSnapshot& Snapshot)
{
	if (!IsReady())
	{
		return;
	}

	TSharedRef<FJsonObject> Object = MakeSnapshotObject(FString(), TEXT("Procedural"));
	OWTAttributeJson::WriteProceduralSnapshot(*Object, Snapshot);
	Notifications->Publish(TEXT("ProceduralGenerationChanged"), OWTAttributeJson::SerializeObject(Object));
}

bool AVTBAttributeEditor::IsReady() const
{
	if (IsActorBeingDestroyed())
	{
		return false;
	}
	if (bEndingPlay)
	{
		return false;
	}
	if (!Subsystem.IsValid())
	{
		return false;
	}
	if (Subsystem->GetAttributeEditor() != this)
	{
		return false;
	}
	if (!Notifications)
	{
		return false;
	}
	const UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}
	return !World->bIsTearingDown;
}

bool AVTBAttributeEditor::IsObservedSelectionCurrent(AActor* Actor) const
{
	if (ObservedSelection.Get() != Actor)
	{
		return false;
	}
	if (!Actor)
	{
		return !SelectedId.IsValid();
	}
	return true;
}

FGuid AVTBAttributeEditor::RegisterActor(AActor& Actor, bool bNewObject)
{
	check(StateStore);
	return StateStore->RegisterActor(Actor, bNewObject);
}

void AVTBAttributeEditor::RebuildSnapshot()
{
	FOWTAttributeSnapshot Snapshot;
	Snapshot.EditorId = EditorId.ToString(EGuidFormats::DigitsWithHyphens);
	Snapshot.SelectionRevision = SelectionRevision;
	Snapshot.StateRevision = StateRevision;
	Snapshot.bIsModifying = ActiveOperation.IsValid();
	UVTBOWTEditorSubsystem* EditorSubsystem = Subsystem.Get();
	if (!EditorSubsystem)
	{
		Snapshot.DisabledReason = TEXT("Editor is unavailable.");
		StateStore->Observe(nullptr, FGuid(), MoveTemp(Snapshot));
		return;
	}

	Snapshot.bEditingEnabled = EditorSubsystem->IsEditingEnabled();
	Snapshot.ActiveMode = GetNameSafe(EditorSubsystem->ActiveEditMode);
	Snapshot.GizmoCoordinateSystem =
	    EditorSubsystem->GetCoordinateSystem() == EToolContextCoordinateSystem::Local ? TEXT("Local") : TEXT("World");
	switch (EditorSubsystem->GetTransformGizmoMode())
	{
	case EToolContextTransformGizmoMode::Translation:
		Snapshot.GizmoMode = TEXT("Translation");
		break;
	case EToolContextTransformGizmoMode::Rotation:
		Snapshot.GizmoMode = TEXT("Rotation");
		break;
	case EToolContextTransformGizmoMode::Scale:
		Snapshot.GizmoMode = TEXT("Scale");
		break;
	default:
		Snapshot.GizmoMode = TEXT("Other");
		break;
	}

	StateStore->Observe(EditorSubsystem->SelectedObject.Get(), SelectedId, MoveTemp(Snapshot));
}

FOWTAttributeSnapshot AVTBAttributeEditor::GetSnapshot() const
{
	check(IsInGameThread());
	return StateStore ? StateStore->GetSnapshot() : FOWTAttributeSnapshot();
}

TArray<FOWTEventRecord> AVTBAttributeEditor::GetMonitorEntries(int32 MaximumCount) const
{
	check(IsInGameThread());
	return Notifications ? Notifications->GetRecentEvents(MaximumCount) : TArray<FOWTEventRecord>();
}

int64 AVTBAttributeEditor::GetLatestEventSequence() const
{
	check(IsInGameThread());
	return Notifications ? Notifications->GetLatestSequence() : 0;
}

void AVTBAttributeEditor::NotifySelectionChanged()
{
	check(IsInGameThread());
	if (!IsReady())
	{
		return;
	}
	AActor* Actor = Subsystem->SelectedObject.Get();
	if (!IsObservedSelectionCurrent(Actor))
	{
		ClearOperation();
		ObservedSelection = Actor;
		SelectedId = Actor ? RegisterActor(*Actor) : FGuid();
		++SelectionRevision;
	}
	EmitSnapshot(TEXT("SelectionChanged"));
}

void AVTBAttributeEditor::NotifyEditorStateChanged()
{
	check(IsInGameThread());
	RefreshModeBindings();
	if (!IsReady())
	{
		return;
	}
	EmitSnapshot(TEXT("EditorStateChanged"));
}

void AVTBAttributeEditor::RefreshSelectedTransform()
{
	check(IsInGameThread());
	if (!IsReady())
	{
		return;
	}
	if (bApplyingTransform)
	{
		return;
	}
	AActor* Actor = Subsystem->SelectedObject.Get();
	if (!IsObservedSelectionCurrent(Actor))
	{
		NotifySelectionChanged();
		return;
	}
	if (!Actor)
	{
		return;
	}

	const FOWTAttributeSnapshot Snapshot = GetSnapshot();
	const FTransform ActorTransform = Actor->GetActorTransform();
	if (!ActorTransform.Equals(Snapshot.Transform))
	{
		FinishActiveOperation();
		Subsystem->SynchronizeSelectionGizmo();
		PublishTransform(FString(), TEXT("External"), TEXT("Commit"));
		return;
	}
	RebuildSnapshot();
	const FOWTAttributeSnapshot CurrentSnapshot = GetSnapshot();
	if (Snapshot.bCanEditTransform != CurrentSnapshot.bCanEditTransform)
	{
		EmitSnapshot(TEXT("EditorStateChanged"));
		return;
	}
	if (Snapshot.ObjectName != CurrentSnapshot.ObjectName)
	{
		EmitSnapshot(TEXT("EditorStateChanged"));
	}
}

void AVTBAttributeEditor::BeginGizmoEdit()
{
	check(IsInGameThread());
	if (!IsReady())
	{
		return;
	}
	if (bApplyingTransform)
	{
		return;
	}
	if (ActiveOperation.IsValid())
	{
		Subsystem->TerminateGizmoCapture();
		return;
	}
	AActor* Actor = Subsystem->SelectedObject.Get();
	if (!Actor)
	{
		return;
	}
	ActiveOperation = FGuid::NewGuid();
	OperationActor = Actor;
	OperationStart = Actor->GetActorTransform();
	bGizmoOperation = true;
	PublishTransform(FString(), TEXT("Gizmo"), TEXT("Begin"));
}

void AVTBAttributeEditor::UpdateGizmoEdit()
{
	check(IsInGameThread());
	if (!IsReady())
	{
		return;
	}
	if (bApplyingTransform)
	{
		return;
	}
	if (!bGizmoOperation)
	{
		return;
	}
	AActor* Actor = OperationActor.Get();
	if (!Actor)
	{
		ClearOperation();
		return;
	}
	if (!Actor->GetActorTransform().Equals(GetSnapshot().Transform))
	{
		PublishTransform(FString(), TEXT("Gizmo"), TEXT("Update"));
	}
}

void AVTBAttributeEditor::EndGizmoEdit()
{
	check(IsInGameThread());
	if (!bGizmoOperation)
	{
		return;
	}
	const FGuid FinishedOperation = ActiveOperation;
	ClearOperation();
	if (IsReady())
	{
		PublishTransform(FString(), TEXT("Gizmo"), TEXT("Commit"), FinishedOperation);
	}
}

void AVTBAttributeEditor::ClearOperation()
{
	if (ActiveOperation.IsValid())
	{
		CompletedOperations.Add(ActiveOperation);
	}
	ActiveOperation.Invalidate();
	OperationActor.Reset();
	OperationStart = FTransform::Identity;
	bGizmoOperation = false;
}

void AVTBAttributeEditor::FinishActiveOperation()
{
	check(IsInGameThread());
	if (!ActiveOperation.IsValid())
	{
		return;
	}
	if (bGizmoOperation)
	{
		if (UVTBOWTEditorSubsystem* EditorSubsystem = Subsystem.Get())
		{
			EditorSubsystem->TerminateGizmoCapture();
		}
	}
	if (!ActiveOperation.IsValid())
	{
		return;
	}
	const FGuid FinishedOperation = ActiveOperation;
	ClearOperation();
	if (IsReady())
	{
		PublishTransform(FString(), TEXT("Editor"), TEXT("Commit"), FinishedOperation);
	}
}

void AVTBAttributeEditor::MarkSelectionBaseline()
{
	check(IsInGameThread());
	if (!IsReady())
	{
		return;
	}
	if (!StateStore->MarkBaseline(SelectedId))
	{
		return;
	}
	EmitSnapshot(TEXT("EditorStateChanged"));
}

TSharedRef<FJsonObject> AVTBAttributeEditor::MakeSnapshotObject(const FString& RequestId, const FString& Source) const
{
	return OWTAttributeJson::MakeSnapshotObject(GetSnapshot(), ActiveOperation, RequestId, Source);
}

void AVTBAttributeEditor::EmitSnapshot(FName Event, const FString& RequestId, const FString& Source,
                                       const FString& Phase, FGuid Recipient, FGuid OperationId)
{
	++StateRevision;
	RebuildSnapshot();

	TSharedRef<FJsonObject> Object = MakeSnapshotObject(RequestId, Source);
	if (!Phase.IsEmpty())
	{
		Object->SetStringField(TEXT("phase"), Phase);
	}
	if (OperationId.IsValid())
	{
		Object->SetStringField(TEXT("operationId"), OperationId.ToString(EGuidFormats::DigitsWithHyphens));
	}
	Notifications->Publish(Event, OWTAttributeJson::SerializeObject(Object), Recipient);
}

void AVTBAttributeEditor::PublishTransform(const FString& RequestId, const FString& Source, const FString& Phase,
                                           FGuid OperationId)
{
	EmitSnapshot(TEXT("TransformChanged"), RequestId, Source, Phase, FGuid(), OperationId);
}

void AVTBAttributeEditor::EmitRejection(const FString& RequestId, const FString& Code, const FString& Reason)
{
	TSharedRef<FJsonObject> Object = MakeShared<FJsonObject>();
	Object->SetNumberField(TEXT("schemaVersion"), 1);
	Object->SetStringField(TEXT("editorId"), EditorId.ToString(EGuidFormats::DigitsWithHyphens));
	Object->SetStringField(TEXT("requestId"), RequestId);
	Object->SetStringField(TEXT("source"), TEXT("Editor"));
	Object->SetStringField(TEXT("code"), Code);
	Object->SetStringField(TEXT("reason"), Reason);
	Notifications->Publish(TEXT("RequestRejected"), OWTAttributeJson::SerializeObject(Object));
}

FGuid AVTBAttributeEditor::Subscribe(UObject* Subscriber, const FOWTAttributeEventNative& Callback, bool bSendSnapshot)
{
	check(IsInGameThread());
	if (!IsReady())
	{
		return {};
	}
	const FGuid Handle = Notifications->Subscribe(Subscriber, Callback);
	if (!Handle.IsValid())
	{
		return Handle;
	}
	if (bSendSnapshot)
	{
		RefreshSelectedTransform();
		EmitSnapshot(TEXT("EditorStateChanged"), FString(), TEXT("Editor"), FString(), Handle);
	}
	return Handle;
}

FGuid AVTBAttributeEditor::SubscribeDynamic(UObject* Subscriber, const FOWTAttributeEventDynamic& Callback,
                                            bool bSendSnapshot)
{
	check(IsInGameThread());
	if (!IsReady())
	{
		return {};
	}
	const FGuid Handle = Notifications->SubscribeDynamic(Subscriber, Callback);
	if (!Handle.IsValid())
	{
		return Handle;
	}
	if (bSendSnapshot)
	{
		RefreshSelectedTransform();
		EmitSnapshot(TEXT("EditorStateChanged"), FString(), TEXT("Editor"), FString(), Handle);
	}
	return Handle;
}

bool AVTBAttributeEditor::Unsubscribe(FGuid Handle)
{
	check(IsInGameThread());
	return Notifications ? Notifications->Unsubscribe(Handle) : false;
}

bool AVTBAttributeEditor::ParseSnapshotJson(const FString& Json, FOWTAttributeSnapshot& OutSnapshot)
{
	return OWTAttributeJson::ParseSnapshot(Json, OutSnapshot);
}

TSharedRef<FJsonObject> AVTBAttributeEditor::MakeRequestObject(const FOWTAttributeSnapshot& Expected) const
{
	return OWTAttributeJson::MakeRequestObject(Expected);
}

bool AVTBAttributeEditor::RequestTransformField(const FOWTAttributeSnapshot& Expected, EOWTTransformField Field,
                                                double Value, EOWTTransformEditPhase Phase, FGuid OperationId)
{
	check(IsInGameThread());
	if (!IsReady())
	{
		return false;
	}
	const TCHAR* FieldName = OWTAttributeJson::GetTransformFieldName(Field);
	if (!FieldName)
	{
		EmitRejection(FString(), TEXT("InvalidField"), TEXT("The transform field is not supported."));
		return false;
	}
	const TCHAR* PhaseName = OWTAttributeJson::GetEditPhaseName(Phase);
	if (!PhaseName)
	{
		EmitRejection(FString(), TEXT("InvalidPhase"), TEXT("The transform edit phase is not supported."));
		return false;
	}
	if (!FMath::IsFinite(Value))
	{
		EmitRejection(FString(), TEXT("InvalidValue"), TEXT("The transform value must be finite."));
		return false;
	}
	TSharedRef<FJsonObject> Object = MakeRequestObject(Expected);
	Object->SetStringField(TEXT("operationId"), OperationId.ToString(EGuidFormats::DigitsWithHyphens));
	Object->SetStringField(TEXT("property"), FieldName);
	Object->SetStringField(TEXT("phase"), PhaseName);
	Object->SetStringField(TEXT("space"), TEXT("World"));
	Object->SetNumberField(TEXT("value"), Value);
	return PublishRequest(TEXT("TransformEditRequested"), OWTAttributeJson::SerializeObject(Object));
}

bool AVTBAttributeEditor::RequestDuplicate(const FOWTAttributeSnapshot& Expected)
{
	return PublishRequest(TEXT("DuplicateRequested"), OWTAttributeJson::SerializeObject(MakeRequestObject(Expected)));
}

FGuid AVTBAttributeEditor::BeginDuplicateOperation(const FOWTAttributeSnapshot& Expected,
                                                   const FOWTDuplicationOptions& Options)
{
	check(IsInGameThread());
	if (!IsReady())
	{
		return {};
	}
	if (!OWTAttributeJson::IsFiniteVector(Options.WorldOffset))
	{
		EmitRejection(FString(), TEXT("InvalidOffset"), TEXT("The duplicate offset must be finite."));
		return {};
	}
	if (!StaticEnum<EOWTDuplicationHierarchyScope>()->IsValidEnumValue(static_cast<int64>(Options.HierarchyScope)))
	{
		EmitRejection(FString(), TEXT("InvalidScope"), TEXT("The hierarchy scope is unsupported."));
		return {};
	}
	if (static_cast<int64>(Options.HierarchyScope) >= StaticEnum<EOWTDuplicationHierarchyScope>()->GetMaxEnumValue())
	{
		EmitRejection(FString(), TEXT("InvalidScope"), TEXT("The hierarchy scope is unsupported."));
		return {};
	}
	if (!StaticEnum<EOWTDuplicationGenerationPolicy>()->IsValidEnumValue(static_cast<int64>(Options.GenerationPolicy)))
	{
		EmitRejection(FString(), TEXT("InvalidGenerationPolicy"), TEXT("The generation policy is unsupported."));
		return {};
	}
	if (static_cast<int64>(Options.GenerationPolicy) >=
	    StaticEnum<EOWTDuplicationGenerationPolicy>()->GetMaxEnumValue())
	{
		EmitRejection(FString(), TEXT("InvalidGenerationPolicy"), TEXT("The generation policy is unsupported."));
		return {};
	}
	TSharedRef<FJsonObject> Request = MakeRequestObject(Expected);
	const FString RequestId = Request->GetStringField(TEXT("requestId"));
	Request->SetNumberField(TEXT("schemaVersion"), 2);
	Request->SetObjectField(TEXT("worldOffset"), OWTAttributeJson::MakeVectorObject(Options.WorldOffset));
	Request->SetStringField(TEXT("hierarchyScope"), StaticEnum<EOWTDuplicationHierarchyScope>()->GetNameStringByValue(
	                                                    static_cast<int64>(Options.HierarchyScope)));
	Request->SetStringField(TEXT("generationPolicy"),
	                        StaticEnum<EOWTDuplicationGenerationPolicy>()->GetNameStringByValue(
	                            static_cast<int64>(Options.GenerationPolicy)));
	if (!PublishRequest(TEXT("DuplicateRequested"), OWTAttributeJson::SerializeObject(Request)))
	{
		return {};
	}
	for (const FOWTDuplicationOperationSnapshot& Operation : GetDuplicationOperations())
	{
		if (Operation.RequestId == RequestId)
		{
			return Operation.OperationId;
		}
	}
	return {};
}

bool AVTBAttributeEditor::PublishRequest(FName Event, const FString& Json)
{
	check(IsInGameThread());
	if (!IsReady())
	{
		return false;
	}
	Notifications->RecordEvent(Event, Json, EOWTEventDirection::Inbound);
	TSharedPtr<FJsonObject> Request;
	if (!OWTAttributeJson::ReadObject(Json, Request))
	{
		EmitRejection(FString(), TEXT("InvalidJson"), TEXT("Expected a JSON object of at most 64 KiB."));
		return false;
	}
	FString RequestId;
	FGuid RequestGuid;
	if (!OWTAttributeJson::ReadString(*Request, TEXT("requestId"), RequestId))
	{
		EmitRejection(FString(), TEXT("InvalidRequestId"), TEXT("A requestId GUID is required."));
		return false;
	}
	if (!FGuid::Parse(RequestId, RequestGuid))
	{
		EmitRejection(FString(), TEXT("InvalidRequestId"), TEXT("The requestId is not a valid GUID."));
		return false;
	}
	if (!RequestGuid.IsValid())
	{
		EmitRejection(FString(), TEXT("InvalidRequestId"), TEXT("The requestId is not a valid GUID."));
		return false;
	}
	RequestId = RequestGuid.ToString(EGuidFormats::DigitsWithHyphens);
	if (ProcessedRequestIds.Contains(RequestId))
	{
		return false;
	}
	ProcessedRequestIds.Add(RequestId);
	if (bHandlingRequest)
	{
		EmitRejection(RequestId, TEXT("Busy"), TEXT("An edit request is already being processed."));
		return false;
	}

	TGuardValue<bool> RequestGuard(bHandlingRequest, true);
	RefreshSelectedTransform();
	if (!IsReady())
	{
		return false;
	}
	AActor* Actor = nullptr;
	if (!ValidateRequest(*Request, RequestId, Actor))
	{
		return false;
	}
	if (Event == TEXT("TransformEditRequested"))
	{
		return ProcessTransformRequest(*Request, RequestId, *Actor);
	}
	if (Event == TEXT("DuplicateRequested"))
	{
		return ProcessDuplicateRequest(*Request, RequestId, *Actor);
	}
	EmitRejection(RequestId, TEXT("UnknownEvent"), TEXT("This event is not a supported request."));
	return false;
}

bool AVTBAttributeEditor::ValidateRequest(const FJsonObject& Request, FString& RequestId, AActor*& Actor)
{
	if (!ValidateRequestContext(Request, RequestId))
	{
		return false;
	}

	return ResolveRequestActor(Request, RequestId, Actor);
}

bool AVTBAttributeEditor::ValidateRequestContext(const FJsonObject& Request, const FString& RequestId)
{
	double Version = 0;
	if (!OWTAttributeJson::ReadNumber(Request, TEXT("schemaVersion"), Version))
	{
		EmitRejection(RequestId, TEXT("InvalidSchema"), TEXT("schemaVersion must be a number."));
		return false;
	}
	if (Version != 1)
	{
		if (Version != 2)
		{
			EmitRejection(RequestId, TEXT("UnsupportedSchema"), TEXT("Supported schema versions are 1 and 2."));
			return false;
		}
	}

	FString RequestedEditor;
	FGuid RequestedEditorId;
	if (!OWTAttributeJson::ReadString(Request, TEXT("editorId"), RequestedEditor))
	{
		EmitRejection(RequestId, TEXT("InvalidEditor"), TEXT("An editorId is required."));
		return false;
	}
	if (!FGuid::Parse(RequestedEditor, RequestedEditorId))
	{
		EmitRejection(RequestId, TEXT("InvalidEditor"), TEXT("editorId must be a GUID."));
		return false;
	}
	if (RequestedEditorId != EditorId)
	{
		EmitRejection(RequestId, TEXT("DifferentEditor"), TEXT("The request belongs to another editor session."));
		return false;
	}
	FString Source;
	if (!OWTAttributeJson::ReadString(Request, TEXT("source"), Source))
	{
		EmitRejection(RequestId, TEXT("InvalidSource"), TEXT("A source string is required."));
		return false;
	}
	if (Source.IsEmpty())
	{
		EmitRejection(RequestId, TEXT("InvalidSource"), TEXT("source must not be empty."));
		return false;
	}
	if (Source.Len() > 64)
	{
		EmitRejection(RequestId, TEXT("InvalidSource"), TEXT("source must contain 1 to 64 characters."));
		return false;
	}
	if (!Subsystem->IsEditingEnabled())
	{
		EmitRejection(RequestId, TEXT("EditingDisabled"), TEXT("Editing is disabled."));
		return false;
	}
	return true;
}

bool AVTBAttributeEditor::ResolveRequestActor(const FJsonObject& Request, const FString& RequestId, AActor*& Actor)
{
	int32 RequestedRevision = 0;
	if (!OWTAttributeJson::ReadRevision(Request, TEXT("selectionRevision"), RequestedRevision))
	{
		EmitRejection(RequestId, TEXT("InvalidRevision"), TEXT("selectionRevision must be a nonnegative integer."));
		return false;
	}
	if (RequestedRevision != SelectionRevision)
	{
		EmitRejection(RequestId, TEXT("StaleSelection"), TEXT("The selection changed after this request was created."));
		return false;
	}

	FString RequestedObject;
	FGuid ObjectId;
	if (!OWTAttributeJson::ReadString(Request, TEXT("objectId"), RequestedObject))
	{
		EmitRejection(RequestId, TEXT("InvalidTarget"), TEXT("An objectId is required."));
		return false;
	}
	if (!FGuid::Parse(RequestedObject, ObjectId))
	{
		EmitRejection(RequestId, TEXT("InvalidTarget"), TEXT("objectId must be a GUID."));
		return false;
	}
	if (ObjectId != SelectedId)
	{
		EmitRejection(RequestId, TEXT("StaleSelection"), TEXT("The request does not target the selected actor."));
		return false;
	}
	if (!StateStore->ContainsActor(ObjectId))
	{
		EmitRejection(RequestId, TEXT("UnknownObject"), TEXT("The object is not registered in this editor."));
		return false;
	}

	Actor = StateStore->ResolveActor(ObjectId);
	if (!Actor)
	{
		EmitRejection(RequestId, TEXT("InvalidTarget"), TEXT("The selected actor no longer exists."));
		return false;
	}
	if (Actor->IsActorBeingDestroyed())
	{
		EmitRejection(RequestId, TEXT("InvalidTarget"), TEXT("The selected actor is being destroyed."));
		return false;
	}
	if (Actor->GetWorld() != GetWorld())
	{
		EmitRejection(RequestId, TEXT("DifferentWorld"), TEXT("The target actor belongs to another world."));
		return false;
	}
	if (Subsystem->SelectedObject.Get() != Actor)
	{
		EmitRejection(RequestId, TEXT("StaleSelection"), TEXT("The selected actor changed."));
		return false;
	}
	return true;
}

bool AVTBAttributeEditor::ValidateTransformOperation(FGuid OperationId, const FString& Phase, const FString& RequestId,
                                                     const AActor& Actor)
{
	if (CompletedOperations.Contains(OperationId))
	{
		EmitRejection(RequestId, TEXT("OperationEnded"),
		              TEXT("This operation has already ended. Start a new operation."));
		return false;
	}
	if (bGizmoOperation)
	{
		EmitRejection(RequestId, TEXT("Busy"), TEXT("Finish the gizmo interaction before editing values."));
		return false;
	}
	if (Subsystem->HasGizmoCapture())
	{
		EmitRejection(RequestId, TEXT("Busy"), TEXT("The gizmo has captured the pointer."));
		return false;
	}
	if (ActiveOperation.IsValid())
	{
		if (ActiveOperation != OperationId)
		{
			EmitRejection(RequestId, TEXT("Busy"), TEXT("Another transform operation is active."));
			return false;
		}
		if (OperationActor.Get() != &Actor)
		{
			EmitRejection(RequestId, TEXT("StaleSelection"), TEXT("The active operation has another target."));
			return false;
		}
		if (Phase == TEXT("Begin"))
		{
			EmitRejection(RequestId, TEXT("OperationAlreadyActive"), TEXT("This operation has already begun."));
			return false;
		}
	}
	else
	{
		const bool bRequiresActiveOperation = Phase == TEXT("Update") || Phase == TEXT("Cancel");
		if (bRequiresActiveOperation)
		{
			EmitRejection(RequestId, TEXT("MissingOperation"),
			              TEXT("Begin this operation before updating or cancelling it."));
			return false;
		}
	}

	return true;
}

bool AVTBAttributeEditor::ProcessTransformRequest(const FJsonObject& Request, const FString& RequestId, AActor& Actor)
{
	const FOWTAttributeSnapshot Snapshot = GetSnapshot();
	if (!Snapshot.bCanEditTransform)
	{
		EmitRejection(RequestId, TEXT("TransformDisabled"), Snapshot.DisabledReason);
		return false;
	}

	OWTAttributeJson::FTransformRequest TransformRequest;
	OWTAttributeJson::FRequestError Error;
	if (!OWTAttributeJson::ReadTransformRequest(Request, TransformRequest, Error))
	{
		EmitRejection(RequestId, Error.Code, Error.Reason);
		return false;
	}

	const FGuid OperationId = TransformRequest.OperationId;
	const FString& Phase = TransformRequest.Phase;
	if (!ValidateTransformOperation(OperationId, Phase, RequestId, Actor))
	{
		return false;
	}

	const FString& Source = TransformRequest.Source;
	if (Phase == TEXT("Begin"))
	{
		ActiveOperation = OperationId;
		OperationActor = &Actor;
		OperationStart = Actor.GetActorTransform();
		PublishTransform(RequestId, Source, Phase);
		return true;
	}

	const FTransform PreviousTransform = Actor.GetActorTransform();
	FTransform DesiredTransform = OperationStart;
	if (Phase != TEXT("Cancel"))
	{
		DesiredTransform =
		    OWTAttributeJson::MakeTransformWithField(PreviousTransform, TransformRequest.Field, TransformRequest.Value);
	}
	if (!OWTAttributeJson::IsFiniteTransform(DesiredTransform))
	{
		EmitRejection(RequestId, TEXT("InvalidValue"), TEXT("The requested transform cannot be represented."));
		return false;
	}
	if (!ApplyTransform(Actor, DesiredTransform))
	{
		EmitRejection(RequestId, TEXT("ApplyFailed"), TEXT("The actor rejected the requested transform."));
		return false;
	}
	const bool bEndsOperation = Phase == TEXT("Commit") || Phase == TEXT("Cancel");
	if (bEndsOperation)
	{
		ClearOperation();
		CompletedOperations.Add(OperationId);
		PublishTransform(RequestId, Source, Phase, OperationId);
		return true;
	}
	if (!Actor.GetActorTransform().Equals(PreviousTransform))
	{
		PublishTransform(RequestId, Source, Phase);
	}
	return true;
}

bool AVTBAttributeEditor::ApplyTransform(AActor& Actor, const FTransform& Transform)
{
	TGuardValue<bool> ApplyGuard(bApplyingTransform, true);
	RefreshModeBindings();
	if (!BoundMode.IsValid())
	{
		return false;
	}
	FString Error;
	if (!BoundMode->ApplyTransform(Actor, Transform, Error))
	{
		return false;
	}
	// Movement callbacks can end the editor session or replace the selected actor.
	if (!IsReady())
	{
		return false;
	}
	if (!IsValid(&Actor))
	{
		return false;
	}
	if (Actor.IsActorBeingDestroyed())
	{
		return false;
	}
	if (Subsystem->SelectedObject.Get() != &Actor)
	{
		return false;
	}
	Subsystem->SynchronizeSelectionGizmo();
	return true;
}

bool AVTBAttributeEditor::ProcessDuplicateRequest(const FJsonObject& Request, const FString& RequestId, AActor& Actor)
{
	if (ActiveOperation.IsValid())
	{
		EmitRejection(RequestId, TEXT("Busy"), TEXT("Finish the current transform operation before duplicating."));
		return false;
	}
	if (Subsystem->HasGizmoCapture())
	{
		EmitRejection(RequestId, TEXT("Busy"), TEXT("The gizmo has captured the pointer."));
		return false;
	}
	if (Actor.IsA<AVTBAttributeEditor>())
	{
		EmitRejection(RequestId, TEXT("DuplicateFailed"), TEXT("The attribute editor is an editor service."));
		return false;
	}
	if (Actor.IsA<ACombinedTransformGizmoActor>())
	{
		EmitRejection(RequestId, TEXT("DuplicateFailed"), TEXT("A gizmo actor belongs to the editor service."));
		return false;
	}
	RefreshModeBindings();
	if (!BoundMode.IsValid())
	{
		EmitRejection(RequestId, TEXT("ModeUnavailable"), TEXT("The attribute mode is unavailable."));
		return false;
	}
	FOWTDuplicationOptions Options;
	OWTAttributeJson::FRequestError ParseError;
	if (!OWTAttributeJson::ReadDuplicationOptions(Request, DuplicateWorldOffset, Options, ParseError))
	{
		EmitRejection(RequestId, ParseError.Code, ParseError.Reason);
		return false;
	}

	FString Source;
	Request.TryGetStringField(TEXT("source"), Source);
	FGuid OperationId;
	FString Error;
	if (!BoundMode->BeginDuplicateOperation(&Actor, Options, RequestId, Source, OperationId, Error))
	{
		if (IsReady())
		{
			EmitRejection(RequestId, TEXT("DuplicateFailed"), Error);
		}
		return false;
	}
	return true;
}

void AVTBAttributeEditor::SaveHistory_Implementation()
{
	// TODO: Capture the current attribute state in the history.
}

void AVTBAttributeEditor::SaveHistoryFromTransformContext(FSaveTransformContext Context)
{
	// TODO: Record Context in the transform history.
}

void AVTBAttributeEditor::Undo_Implementation()
{
}

void AVTBAttributeEditor::Redo_Implementation()
{
}
