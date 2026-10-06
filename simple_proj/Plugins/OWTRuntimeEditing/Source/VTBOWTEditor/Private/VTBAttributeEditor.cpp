#include "VTBAttributeEditor.h"

#include "Components/SceneComponent.h"
#include "BaseGizmos/CombinedTransformGizmo.h"
#include "State/OWTAttributeStateStore.h"
#include "Dom/JsonObject.h"
#include "Duplication/OWTRuntimeActorDuplicator.h"
#include "Engine/World.h"
#include "Events/OWTNotificationCenter.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "VTBOWTEditorSubsystem.h"

namespace
{
const TCHAR* TransformFields[] = {TEXT("Location.X"),    TEXT("Location.Y"),     TEXT("Location.Z"),
                                  TEXT("Rotation.Roll"), TEXT("Rotation.Pitch"), TEXT("Rotation.Yaw"),
                                  TEXT("Scale.X"),       TEXT("Scale.Y"),        TEXT("Scale.Z")};
const TCHAR* EditPhases[] = {TEXT("Begin"), TEXT("Update"), TEXT("Commit"), TEXT("Cancel")};

FString SerializeObject(const TSharedRef<FJsonObject>& Object)
{
	FString Json;
	FJsonSerializer::Serialize(Object, TJsonWriterFactory<>::Create(&Json));
	return Json;
}

bool ReadObject(const FString& Json, TSharedPtr<FJsonObject>& Object)
{
	if (Json.Len() > 65536)
	{
		return false;
	}
	return FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Json), Object);
}

bool ReadString(const FJsonObject& Object, const TCHAR* Field, FString& Value)
{
	if (!Object.HasTypedField<EJson::String>(Field))
	{
		return false;
	}
	return Object.TryGetStringField(Field, Value);
}

bool ReadNumber(const FJsonObject& Object, const TCHAR* Field, double& Value)
{
	if (!Object.HasTypedField<EJson::Number>(Field))
	{
		return false;
	}
	if (!Object.TryGetNumberField(Field, Value))
	{
		return false;
	}
	return FMath::IsFinite(Value);
}

bool ReadBoolean(const FJsonObject& Object, const TCHAR* Field, bool& Value)
{
	if (!Object.HasTypedField<EJson::Boolean>(Field))
	{
		return false;
	}
	return Object.TryGetBoolField(Field, Value);
}

bool ReadRevision(const FJsonObject& Object, const TCHAR* Field, int32& Value)
{
	double Number = 0;
	if (!ReadNumber(Object, Field, Number))
	{
		return false;
	}
	if (Number < 0)
	{
		return false;
	}
	if (Number > MAX_int32)
	{
		return false;
	}
	Value = static_cast<int32>(Number);
	return Number == static_cast<double>(Value);
}

bool IsFiniteVector(const FVector& Vector)
{
	if (!FMath::IsFinite(Vector.X))
	{
		return false;
	}
	if (!FMath::IsFinite(Vector.Y))
	{
		return false;
	}
	return FMath::IsFinite(Vector.Z);
}

bool IsFiniteTransform(const FTransform& Transform)
{
	if (!IsFiniteVector(Transform.GetLocation()))
	{
		return false;
	}
	if (!IsFiniteVector(Transform.GetScale3D()))
	{
		return false;
	}
	return !Transform.GetRotation().ContainsNaN();
}

TSharedRef<FJsonObject> VectorObject(const FVector& Value)
{
	TSharedRef<FJsonObject> Object = MakeShared<FJsonObject>();
	Object->SetNumberField(TEXT("x"), Value.X);
	Object->SetNumberField(TEXT("y"), Value.Y);
	Object->SetNumberField(TEXT("z"), Value.Z);
	return Object;
}

bool ReadVector(const FJsonObject& Object, const TCHAR* Field, FVector& Value)
{
	const TSharedPtr<FJsonObject>* Vector = nullptr;
	if (!Object.TryGetObjectField(Field, Vector))
	{
		return false;
	}
	if (!ReadNumber(**Vector, TEXT("x"), Value.X))
	{
		return false;
	}
	if (!ReadNumber(**Vector, TEXT("y"), Value.Y))
	{
		return false;
	}
	return ReadNumber(**Vector, TEXT("z"), Value.Z);
}

bool FindField(const FString& Name, EOWTTransformField& Field)
{
	for (int32 Index = 0; Index < UE_ARRAY_COUNT(TransformFields); ++Index)
	{
		if (Name == TransformFields[Index])
		{
			Field = static_cast<EOWTTransformField>(Index);
			return true;
		}
	}
	return false;
}

FTransform WithField(const FTransform& Original, EOWTTransformField Field, double Value)
{
	FVector Location = Original.GetLocation();
	FRotator Rotation = Original.Rotator();
	FVector Scale = Original.GetScale3D();
	switch (Field)
	{
	case EOWTTransformField::LocationX:
		Location.X = Value;
		break;
	case EOWTTransformField::LocationY:
		Location.Y = Value;
		break;
	case EOWTTransformField::LocationZ:
		Location.Z = Value;
		break;
	case EOWTTransformField::RotationRoll:
		Rotation.Roll = Value;
		break;
	case EOWTTransformField::RotationPitch:
		Rotation.Pitch = Value;
		break;
	case EOWTTransformField::RotationYaw:
		Rotation.Yaw = Value;
		break;
	case EOWTTransformField::ScaleX:
		Scale.X = Value;
		break;
	case EOWTTransformField::ScaleY:
		Scale.Y = Value;
		break;
	case EOWTTransformField::ScaleZ:
		Scale.Z = Value;
		break;
	}
	return FTransform(Rotation.Quaternion(), Location, Scale);
}
} // namespace

AVTBAttributeEditor::AVTBAttributeEditor()
    : Notifications(nullptr), StateStore(nullptr), Duplicator(nullptr), DuplicateWorldOffset(100.0, 0.0, 0.0),
      Subsystem(), ObservedSelection(), OperationActor(), ProcessedRequestIds(), CompletedOperations(),
      OperationStart(FTransform::Identity), EditorId(), SelectedId(), ActiveOperation(), SelectionRevision(0),
      StateRevision(0), bGizmoOperation(false), bApplyingTransform(false), bHandlingRequest(false), bEndingPlay(false)
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
	if (Notifications)
	{
		Notifications->Shutdown();
	}
	if (UVTBOWTEditorSubsystem* Hub = Subsystem.Get())
	{
		if (Hub->GetAttributeEditor() == this)
		{
			Hub->RegisterAttributeEditor(nullptr);
		}
	}
	Subsystem.Reset();
	if (StateStore)
	{
		StateStore->Reset();
	}
	if (Duplicator)
	{
		Duplicator->Deinitialize();
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

	Subsystem = InSubsystem;
	EditorId = FGuid::NewGuid();
	Notifications = NewObject<UOWTNotificationCenter>(this);
	const bool bEventsInitialized = Notifications->Initialize(this);
	check(bEventsInitialized);
	StateStore = NewObject<UOWTAttributeStateStore>(this);
	Duplicator = NewObject<UOWTRuntimeActorDuplicator>(this);
	const bool bDuplicatorInitialized = Duplicator->Initialize(this);
	check(bDuplicatorInitialized);
	InSubsystem->RegisterAttributeEditor(this);
	NotifySelectionChanged();
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
	UVTBOWTEditorSubsystem* Hub = Subsystem.Get();
	if (!Hub)
	{
		Snapshot.DisabledReason = TEXT("Editor is unavailable.");
		StateStore->Observe(nullptr, FGuid(), MoveTemp(Snapshot));
		return;
	}

	Snapshot.bEditingEnabled = Hub->IsEditingEnabled();
	Snapshot.ActiveMode = GetNameSafe(Hub->ActiveEditMode);
	Snapshot.GizmoCoordinateSystem =
	    Hub->GetCoordinateSystem() == EToolContextCoordinateSystem::Local ? TEXT("Local") : TEXT("World");
	switch (Hub->GetTransformGizmoMode())
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

	StateStore->Observe(Hub->SelectedObject.Get(), SelectedId, MoveTemp(Snapshot));
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
	const FTransform Actual = Actor->GetActorTransform();
	if (!Actual.Equals(Snapshot.Transform))
	{
		FinishActiveOperation();
		Subsystem->SynchronizeSelectionGizmo();
		PublishTransform(FString(), TEXT("External"), TEXT("Commit"));
		return;
	}
	RebuildSnapshot();
	const FOWTAttributeSnapshot Current = GetSnapshot();
	if (Snapshot.bCanEditTransform != Current.bCanEditTransform)
	{
		EmitSnapshot(TEXT("EditorStateChanged"));
		return;
	}
	if (Snapshot.ObjectName != Current.ObjectName)
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
		if (UVTBOWTEditorSubsystem* Hub = Subsystem.Get())
		{
			Hub->TerminateGizmoCapture();
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
	const FOWTAttributeSnapshot Snapshot = GetSnapshot();
	TSharedRef<FJsonObject> Object = MakeShared<FJsonObject>();
	Object->SetNumberField(TEXT("schemaVersion"), 1);
	Object->SetStringField(TEXT("editorId"), Snapshot.EditorId);
	Object->SetStringField(TEXT("requestId"), RequestId);
	Object->SetStringField(TEXT("source"), Source);
	Object->SetStringField(TEXT("objectId"), Snapshot.ObjectId);
	Object->SetStringField(TEXT("objectName"), Snapshot.ObjectName);
	Object->SetStringField(TEXT("objectClass"), Snapshot.ObjectClass);
	Object->SetStringField(TEXT("disabledReason"), Snapshot.DisabledReason);
	Object->SetStringField(TEXT("activeMode"), Snapshot.ActiveMode);
	Object->SetStringField(TEXT("gizmoMode"), Snapshot.GizmoMode);
	Object->SetStringField(TEXT("gizmoCoordinateSystem"), Snapshot.GizmoCoordinateSystem);
	Object->SetStringField(TEXT("space"), TEXT("World"));
	Object->SetNumberField(TEXT("selectionRevision"), Snapshot.SelectionRevision);
	Object->SetNumberField(TEXT("stateRevision"), Snapshot.StateRevision);
	Object->SetBoolField(TEXT("editingEnabled"), Snapshot.bEditingEnabled);
	Object->SetBoolField(TEXT("hasSelection"), Snapshot.bHasSelection);
	Object->SetBoolField(TEXT("canEditTransform"), Snapshot.bCanEditTransform);
	Object->SetBoolField(TEXT("isModifying"), Snapshot.bIsModifying);
	Object->SetBoolField(TEXT("hasChanges"), Snapshot.bHasChanges);
	Object->SetStringField(TEXT("operationId"), ActiveOperation.IsValid()
	                                                ? ActiveOperation.ToString(EGuidFormats::DigitsWithHyphens)
	                                                : FString());
	if (Snapshot.bHasSelection && IsFiniteTransform(Snapshot.Transform))
	{
		TSharedRef<FJsonObject> Transform = MakeShared<FJsonObject>();
		Transform->SetObjectField(TEXT("location"), VectorObject(Snapshot.Transform.GetLocation()));
		Transform->SetObjectField(TEXT("scale"), VectorObject(Snapshot.Transform.GetScale3D()));
		TSharedRef<FJsonObject> Rotation = MakeShared<FJsonObject>();
		const FRotator Angles = Snapshot.Transform.Rotator();
		Rotation->SetNumberField(TEXT("roll"), Angles.Roll);
		Rotation->SetNumberField(TEXT("pitch"), Angles.Pitch);
		Rotation->SetNumberField(TEXT("yaw"), Angles.Yaw);
		Transform->SetObjectField(TEXT("rotation"), Rotation);
		Object->SetObjectField(TEXT("transform"), Transform);
	}
	else
	{
		Object->SetField(TEXT("transform"), MakeShared<FJsonValueNull>());
	}
	return Object;
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
	Notifications->Publish(Event, SerializeObject(Object), Recipient);
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
	Notifications->Publish(TEXT("RequestRejected"), SerializeObject(Object));
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
	TSharedPtr<FJsonObject> Object;
	if (!ReadObject(Json, Object))
	{
		return false;
	}
	double Schema = 0;
	if (!ReadNumber(*Object, TEXT("schemaVersion"), Schema))
	{
		return false;
	}
	if (Schema != 1)
	{
		return false;
	}

	FOWTAttributeSnapshot Parsed;
	if (!ReadString(*Object, TEXT("editorId"), Parsed.EditorId))
	{
		return false;
	}
	if (!ReadRevision(*Object, TEXT("selectionRevision"), Parsed.SelectionRevision))
	{
		return false;
	}
	if (!ReadRevision(*Object, TEXT("stateRevision"), Parsed.StateRevision))
	{
		return false;
	}
	if (!ReadBoolean(*Object, TEXT("editingEnabled"), Parsed.bEditingEnabled))
	{
		return false;
	}
	if (!ReadBoolean(*Object, TEXT("hasSelection"), Parsed.bHasSelection))
	{
		return false;
	}
	if (!ReadBoolean(*Object, TEXT("canEditTransform"), Parsed.bCanEditTransform))
	{
		return false;
	}
	if (!ReadBoolean(*Object, TEXT("isModifying"), Parsed.bIsModifying))
	{
		return false;
	}
	if (!ReadBoolean(*Object, TEXT("hasChanges"), Parsed.bHasChanges))
	{
		return false;
	}
	if (!ReadString(*Object, TEXT("objectId"), Parsed.ObjectId))
	{
		return false;
	}
	if (!ReadString(*Object, TEXT("objectName"), Parsed.ObjectName))
	{
		return false;
	}
	if (!ReadString(*Object, TEXT("objectClass"), Parsed.ObjectClass))
	{
		return false;
	}
	if (!ReadString(*Object, TEXT("disabledReason"), Parsed.DisabledReason))
	{
		return false;
	}
	if (!ReadString(*Object, TEXT("activeMode"), Parsed.ActiveMode))
	{
		return false;
	}
	if (!ReadString(*Object, TEXT("gizmoMode"), Parsed.GizmoMode))
	{
		return false;
	}
	if (!ReadString(*Object, TEXT("gizmoCoordinateSystem"), Parsed.GizmoCoordinateSystem))
	{
		return false;
	}
	const TSharedPtr<FJsonObject>* Transform = nullptr;
	if (Parsed.bHasSelection && Object->TryGetObjectField(TEXT("transform"), Transform))
	{
		FVector Location;
		FVector Scale;
		FRotator Rotation;
		if (!ReadVector(**Transform, TEXT("location"), Location))
		{
			return false;
		}
		if (!ReadVector(**Transform, TEXT("scale"), Scale))
		{
			return false;
		}
		const TSharedPtr<FJsonObject>* Angles = nullptr;
		if (!(*Transform)->TryGetObjectField(TEXT("rotation"), Angles))
		{
			return false;
		}
		if (!ReadNumber(**Angles, TEXT("roll"), Rotation.Roll))
		{
			return false;
		}
		if (!ReadNumber(**Angles, TEXT("pitch"), Rotation.Pitch))
		{
			return false;
		}
		if (!ReadNumber(**Angles, TEXT("yaw"), Rotation.Yaw))
		{
			return false;
		}
		Parsed.Transform = FTransform(Rotation, Location, Scale);
	}
	else if (Parsed.bCanEditTransform)
	{
		return false;
	}
	OutSnapshot = MoveTemp(Parsed);
	return true;
}

TSharedRef<FJsonObject> AVTBAttributeEditor::MakeRequestObject(const FOWTAttributeSnapshot& Expected) const
{
	TSharedRef<FJsonObject> Object = MakeShared<FJsonObject>();
	Object->SetNumberField(TEXT("schemaVersion"), 1);
	Object->SetStringField(TEXT("editorId"), Expected.EditorId);
	Object->SetStringField(TEXT("objectId"), Expected.ObjectId);
	Object->SetNumberField(TEXT("selectionRevision"), Expected.SelectionRevision);
	Object->SetStringField(TEXT("requestId"), FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens));
	Object->SetStringField(TEXT("source"), TEXT("DetailsView"));
	return Object;
}

bool AVTBAttributeEditor::RequestTransformField(const FOWTAttributeSnapshot& Expected, EOWTTransformField Field,
                                                double Value, EOWTTransformEditPhase Phase, FGuid OperationId)
{
	check(IsInGameThread());
	if (!IsReady())
	{
		return false;
	}
	if (static_cast<uint8>(Field) >= UE_ARRAY_COUNT(TransformFields))
	{
		EmitRejection(FString(), TEXT("InvalidField"), TEXT("The transform field is not supported."));
		return false;
	}
	if (static_cast<uint8>(Phase) >= UE_ARRAY_COUNT(EditPhases))
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
	Object->SetStringField(TEXT("property"), TransformFields[static_cast<uint8>(Field)]);
	Object->SetStringField(TEXT("phase"), EditPhases[static_cast<uint8>(Phase)]);
	Object->SetStringField(TEXT("space"), TEXT("World"));
	Object->SetNumberField(TEXT("value"), Value);
	return PublishRequest(TEXT("TransformEditRequested"), SerializeObject(Object));
}

bool AVTBAttributeEditor::RequestDuplicate(const FOWTAttributeSnapshot& Expected)
{
	return PublishRequest(TEXT("DuplicateRequested"), SerializeObject(MakeRequestObject(Expected)));
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
	if (!ReadObject(Json, Request))
	{
		EmitRejection(FString(), TEXT("InvalidJson"), TEXT("Expected a JSON object of at most 64 KiB."));
		return false;
	}
	FString RequestId;
	FGuid RequestGuid;
	if (!ReadString(*Request, TEXT("requestId"), RequestId))
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
	double Version = 0;
	if (!ReadNumber(Request, TEXT("schemaVersion"), Version))
	{
		EmitRejection(RequestId, TEXT("InvalidSchema"), TEXT("schemaVersion must be a number."));
		return false;
	}
	if (Version != 1)
	{
		EmitRejection(RequestId, TEXT("UnsupportedSchema"), TEXT("Only schemaVersion 1 is supported."));
		return false;
	}
	FString RequestedEditor;
	FGuid RequestedEditorId;
	if (!ReadString(Request, TEXT("editorId"), RequestedEditor))
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
	if (!ReadString(Request, TEXT("source"), Source))
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
	int32 RequestedRevision = 0;
	if (!ReadRevision(Request, TEXT("selectionRevision"), RequestedRevision))
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
	if (!ReadString(Request, TEXT("objectId"), RequestedObject))
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

bool AVTBAttributeEditor::ProcessTransformRequest(const FJsonObject& Request, const FString& RequestId, AActor& Actor)
{
	const FOWTAttributeSnapshot Snapshot = GetSnapshot();
	if (!Snapshot.bCanEditTransform)
	{
		EmitRejection(RequestId, TEXT("TransformDisabled"), Snapshot.DisabledReason);
		return false;
	}
	FString Space;
	if (!ReadString(Request, TEXT("space"), Space))
	{
		EmitRejection(RequestId, TEXT("InvalidSpace"), TEXT("A transform space is required."));
		return false;
	}
	if (Space != TEXT("World"))
	{
		EmitRejection(RequestId, TEXT("UnsupportedSpace"), TEXT("Only World transform edits are supported."));
		return false;
	}
	FString Property;
	EOWTTransformField Field;
	if (!ReadString(Request, TEXT("property"), Property))
	{
		EmitRejection(RequestId, TEXT("InvalidField"), TEXT("A transform property is required."));
		return false;
	}
	if (!FindField(Property, Field))
	{
		EmitRejection(RequestId, TEXT("InvalidField"), TEXT("The transform property is not supported."));
		return false;
	}
	double Value = 0;
	if (!ReadNumber(Request, TEXT("value"), Value))
	{
		EmitRejection(RequestId, TEXT("InvalidValue"), TEXT("The transform value must be a finite JSON number."));
		return false;
	}
	FString Phase;
	if (!ReadString(Request, TEXT("phase"), Phase))
	{
		EmitRejection(RequestId, TEXT("InvalidPhase"), TEXT("An edit phase is required."));
		return false;
	}
	const bool bKnownPhase = MakeArrayView(EditPhases)
	                             .ContainsByPredicate(
	                                 [&Phase](const TCHAR* KnownPhase)
	                                 {
		                                 return Phase == KnownPhase;
	                                 });
	if (!bKnownPhase)
	{
		EmitRejection(RequestId, TEXT("InvalidPhase"), TEXT("Use Begin, Update, Commit or Cancel."));
		return false;
	}
	FString OperationText;
	FGuid OperationId;
	if (!ReadString(Request, TEXT("operationId"), OperationText))
	{
		EmitRejection(RequestId, TEXT("InvalidOperation"), TEXT("An operationId GUID is required."));
		return false;
	}
	if (!FGuid::Parse(OperationText, OperationId))
	{
		EmitRejection(RequestId, TEXT("InvalidOperation"), TEXT("operationId must be a valid GUID."));
		return false;
	}
	if (!OperationId.IsValid())
	{
		EmitRejection(RequestId, TEXT("InvalidOperation"), TEXT("operationId must be a valid GUID."));
		return false;
	}
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
		if (Phase == TEXT("Update") || Phase == TEXT("Cancel"))
		{
			EmitRejection(RequestId, TEXT("MissingOperation"),
			              TEXT("Begin this operation before updating or cancelling it."));
			return false;
		}
	}

	FString Source;
	Request.TryGetStringField(TEXT("source"), Source);
	if (Phase == TEXT("Begin"))
	{
		ActiveOperation = OperationId;
		OperationActor = &Actor;
		OperationStart = Actor.GetActorTransform();
		PublishTransform(RequestId, Source, Phase);
		return true;
	}

	const FTransform Previous = Actor.GetActorTransform();
	const FTransform Desired = Phase == TEXT("Cancel") ? OperationStart : WithField(Previous, Field, Value);
	if (!IsFiniteTransform(Desired))
	{
		EmitRejection(RequestId, TEXT("InvalidValue"), TEXT("The requested transform cannot be represented."));
		return false;
	}
	if (!ApplyTransform(Actor, Desired))
	{
		EmitRejection(RequestId, TEXT("ApplyFailed"), TEXT("The actor rejected the requested transform."));
		return false;
	}
	if (Phase == TEXT("Commit") || Phase == TEXT("Cancel"))
	{
		ClearOperation();
		CompletedOperations.Add(OperationId);
		PublishTransform(RequestId, Source, Phase, OperationId);
		return true;
	}
	if (!Actor.GetActorTransform().Equals(Previous))
	{
		PublishTransform(RequestId, Source, Phase);
	}
	return true;
}

bool AVTBAttributeEditor::ApplyTransform(AActor& Actor, const FTransform& Transform)
{
	TGuardValue<bool> ApplyGuard(bApplyingTransform, true);
	if (!Actor.GetActorTransform().Equals(Transform))
	{
		if (!Actor.SetActorTransform(Transform, false, nullptr, ETeleportType::TeleportPhysics))
		{
			return false;
		}
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
	const FString OriginalId = SelectedId.ToString(EGuidFormats::DigitsWithHyphens);
	FString Error;
	AActor* Duplicate = Duplicator->DuplicateActor(&Actor, DuplicateWorldOffset, Error);
	if (!Duplicate)
	{
		EmitRejection(RequestId, TEXT("DuplicateFailed"), Error);
		return false;
	}
	const FGuid DuplicateId = RegisterActor(*Duplicate, true);
	const TWeakObjectPtr<AActor> CreatedActor = Duplicate;
	Subsystem->SetSelectedObject(Duplicate);
	if (!IsReady())
	{
		return false;
	}
	if (!CreatedActor.IsValid())
	{
		EmitRejection(RequestId, TEXT("DuplicateInvalidated"),
		              TEXT("The new actor was destroyed by a selection callback."));
		return false;
	}
	// SetSelectedObject owns the selection notification. Re-read the final state for this result.
	++StateRevision;
	RebuildSnapshot();
	FString Source;
	Request.TryGetStringField(TEXT("source"), Source);
	TSharedRef<FJsonObject> Object = MakeSnapshotObject(RequestId, Source);
	Object->SetStringField(TEXT("originalObjectId"), OriginalId);
	Object->SetStringField(TEXT("duplicateObjectId"), DuplicateId.ToString(EGuidFormats::DigitsWithHyphens));
	Notifications->Publish(TEXT("ObjectDuplicated"), SerializeObject(Object));
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
