#include "AttributeEditor/OWTAttributeJson.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

namespace OWTAttributeJson
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

TSharedRef<FJsonObject> MakeVectorObject(const FVector& Value)
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

FTransform MakeTransformWithField(const FTransform& Original, EOWTTransformField Field, double Value)
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

const TCHAR* GetTransformFieldName(EOWTTransformField Field)
{
	const uint8 Index = static_cast<uint8>(Field);
	if (Index >= UE_ARRAY_COUNT(TransformFields))
	{
		return nullptr;
	}
	return TransformFields[Index];
}

const TCHAR* GetEditPhaseName(EOWTTransformEditPhase Phase)
{
	const uint8 Index = static_cast<uint8>(Phase);
	if (Index >= UE_ARRAY_COUNT(EditPhases))
	{
		return nullptr;
	}
	return EditPhases[Index];
}

TSharedRef<FJsonObject> MakeSnapshotObject(const FOWTAttributeSnapshot& Snapshot, FGuid ActiveOperation,
                                           const FString& RequestId, const FString& Source)
{
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
		Transform->SetObjectField(TEXT("location"), MakeVectorObject(Snapshot.Transform.GetLocation()));
		Transform->SetObjectField(TEXT("scale"), MakeVectorObject(Snapshot.Transform.GetScale3D()));
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

TSharedRef<FJsonObject> MakeRequestObject(const FOWTAttributeSnapshot& Expected)
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

bool ParseSnapshot(const FString& Json, FOWTAttributeSnapshot& OutSnapshot)
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

bool ReadTransformRequest(const FJsonObject& Request, FTransformRequest& OutRequest, FRequestError& Error)
{
	FString Space;
	if (!ReadString(Request, TEXT("space"), Space))
	{
		Error = FRequestError(TEXT("InvalidSpace"), TEXT("A transform space is required."));
		return false;
	}
	if (Space != TEXT("World"))
	{
		Error = FRequestError(TEXT("UnsupportedSpace"), TEXT("Only World transform edits are supported."));
		return false;
	}
	FString Property;
	EOWTTransformField Field;
	if (!ReadString(Request, TEXT("property"), Property))
	{
		Error = FRequestError(TEXT("InvalidField"), TEXT("A transform property is required."));
		return false;
	}
	if (!FindField(Property, Field))
	{
		Error = FRequestError(TEXT("InvalidField"), TEXT("The transform property is not supported."));
		return false;
	}
	double Value = 0;
	if (!ReadNumber(Request, TEXT("value"), Value))
	{
		Error = FRequestError(TEXT("InvalidValue"), TEXT("The transform value must be a finite JSON number."));
		return false;
	}
	FString Phase;
	if (!ReadString(Request, TEXT("phase"), Phase))
	{
		Error = FRequestError(TEXT("InvalidPhase"), TEXT("An edit phase is required."));
		return false;
	}
	bool bKnownPhase = false;
	for (const TCHAR* KnownPhase : EditPhases)
	{
		if (Phase == KnownPhase)
		{
			bKnownPhase = true;
			break;
		}
	}
	if (!bKnownPhase)
	{
		Error = FRequestError(TEXT("InvalidPhase"), TEXT("Use Begin, Update, Commit or Cancel."));
		return false;
	}
	FString OperationText;
	FGuid OperationId;
	if (!ReadString(Request, TEXT("operationId"), OperationText))
	{
		Error = FRequestError(TEXT("InvalidOperation"), TEXT("An operationId GUID is required."));
		return false;
	}
	if (!FGuid::Parse(OperationText, OperationId))
	{
		Error = FRequestError(TEXT("InvalidOperation"), TEXT("operationId must be a valid GUID."));
		return false;
	}
	if (!OperationId.IsValid())
	{
		Error = FRequestError(TEXT("InvalidOperation"), TEXT("operationId must be a valid GUID."));
		return false;
	}
	OutRequest.Field = Field;
	OutRequest.Value = Value;
	OutRequest.Phase = MoveTemp(Phase);
	OutRequest.OperationId = OperationId;
	Request.TryGetStringField(TEXT("source"), OutRequest.Source);
	return true;
}

bool ReadDuplicationOptions(const FJsonObject& Request, const FVector& DefaultWorldOffset,
                            FOWTDuplicationOptions& Options, FRequestError& Error)
{
	Options.WorldOffset = DefaultWorldOffset;
	double Version = 1;
	Request.TryGetNumberField(TEXT("schemaVersion"), Version);
	if (Version == 2)
	{
		FString Scope;
		if (!ReadString(Request, TEXT("hierarchyScope"), Scope))
		{
			Error = FRequestError(TEXT("InvalidScope"), TEXT("A hierarchyScope is required in schema 2."));
			return false;
		}
		const int64 ScopeValue = StaticEnum<EOWTDuplicationHierarchyScope>()->GetValueByNameString(Scope);
		if (ScopeValue == INDEX_NONE)
		{
			Error = FRequestError(TEXT("InvalidScope"), TEXT("The hierarchyScope is unsupported."));
			return false;
		}
		if (ScopeValue >= StaticEnum<EOWTDuplicationHierarchyScope>()->GetMaxEnumValue())
		{
			Error = FRequestError(TEXT("InvalidScope"), TEXT("The hierarchyScope is unsupported."));
			return false;
		}
		Options.HierarchyScope = static_cast<EOWTDuplicationHierarchyScope>(ScopeValue);
		FString Policy;
		if (!ReadString(Request, TEXT("generationPolicy"), Policy))
		{
			Error = FRequestError(TEXT("InvalidGenerationPolicy"), TEXT("A generationPolicy is required in schema 2."));
			return false;
		}
		const int64 PolicyValue = StaticEnum<EOWTDuplicationGenerationPolicy>()->GetValueByNameString(Policy);
		if (PolicyValue == INDEX_NONE)
		{
			Error = FRequestError(TEXT("InvalidGenerationPolicy"), TEXT("The generationPolicy is unsupported."));
			return false;
		}
		if (PolicyValue >= StaticEnum<EOWTDuplicationGenerationPolicy>()->GetMaxEnumValue())
		{
			Error = FRequestError(TEXT("InvalidGenerationPolicy"), TEXT("The generationPolicy is unsupported."));
			return false;
		}
		Options.GenerationPolicy = static_cast<EOWTDuplicationGenerationPolicy>(PolicyValue);
		const TSharedPtr<FJsonObject>* Offset = nullptr;
		if (!Request.TryGetObjectField(TEXT("worldOffset"), Offset))
		{
			Error = FRequestError(TEXT("InvalidOffset"), TEXT("A worldOffset vector is required in schema 2."));
			return false;
		}
		if (!ReadNumber(**Offset, TEXT("x"), Options.WorldOffset.X))
		{
			Error = FRequestError(TEXT("InvalidOffset"), TEXT("worldOffset.x must be finite."));
			return false;
		}
		if (!ReadNumber(**Offset, TEXT("y"), Options.WorldOffset.Y))
		{
			Error = FRequestError(TEXT("InvalidOffset"), TEXT("worldOffset.y must be finite."));
			return false;
		}
		if (!ReadNumber(**Offset, TEXT("z"), Options.WorldOffset.Z))
		{
			Error = FRequestError(TEXT("InvalidOffset"), TEXT("worldOffset.z must be finite."));
			return false;
		}
	}
	else
	{
		if (Request.HasField(TEXT("hierarchyScope")))
		{
			Error = FRequestError(TEXT("SchemaRequired"), TEXT("Duplication policies require schemaVersion 2."));
			return false;
		}
		if (Request.HasField(TEXT("generationPolicy")))
		{
			Error = FRequestError(TEXT("SchemaRequired"), TEXT("Duplication policies require schemaVersion 2."));
			return false;
		}
		if (Request.HasField(TEXT("worldOffset")))
		{
			Error = FRequestError(TEXT("SchemaRequired"), TEXT("Duplication offsets require schemaVersion 2."));
			return false;
		}
	}
	return true;
}

void WriteModeSnapshot(FJsonObject& Object, const FOWTModeSnapshot& Snapshot)
{
	Object.SetStringField(TEXT("modeId"), Snapshot.ModeId.ToString());
	Object.SetStringField(TEXT("toolId"), Snapshot.ActiveToolId.ToString());
	Object.SetStringField(TEXT("lifecycle"), Snapshot.Lifecycle.ToString());
	Object.SetStringField(TEXT("reason"), Snapshot.DisabledReason);
	Object.SetNumberField(TEXT("modeRevision"), Snapshot.Revision);
}

void WriteDuplicationSnapshot(FJsonObject& Object, const FOWTDuplicationOperationSnapshot& Snapshot)
{
	Object.SetStringField(TEXT("operationId"), Snapshot.OperationId.ToString(EGuidFormats::DigitsWithHyphens));
	Object.SetStringField(TEXT("originalObjectId"), Snapshot.OriginalObjectId);
	Object.SetStringField(TEXT("duplicateObjectId"), Snapshot.DuplicateObjectId);
	Object.SetStringField(TEXT("phase"),
	                      StaticEnum<EOWTDuplicationPhase>()->GetNameStringByValue(static_cast<int64>(Snapshot.Phase)));
	Object.SetStringField(TEXT("reason"), Snapshot.Error);
	Object.SetStringField(TEXT("hierarchyScope"), Snapshot.HierarchyScope.ToString());
}

void WriteProceduralStates(FJsonObject& Object, FGuid OperationId,
                           const TArray<FOWTProceduralComponentSnapshot>& ProceduralComponents)
{
	TArray<TSharedPtr<FJsonValue>> States;
	for (const FOWTProceduralComponentSnapshot& Component : ProceduralComponents)
	{
		if (Component.OperationId != OperationId)
		{
			continue;
		}
		TSharedRef<FJsonObject> State = MakeShared<FJsonObject>();
		State->SetStringField(TEXT("componentId"), Component.ComponentId.ToString(EGuidFormats::DigitsWithHyphens));
		State->SetStringField(TEXT("state"), StaticEnum<EOWTProceduralState>()->GetNameStringByValue(
		                                         static_cast<int64>(Component.State)));
		States.Add(MakeShared<FJsonValueObject>(State));
	}
	Object.SetArrayField(TEXT("proceduralStates"), States);
}

void WriteProceduralSnapshot(FJsonObject& Object, const FOWTProceduralComponentSnapshot& Snapshot)
{
	Object.SetStringField(TEXT("operationId"), Snapshot.OperationId.ToString(EGuidFormats::DigitsWithHyphens));
	Object.SetStringField(TEXT("componentId"), Snapshot.ComponentId.ToString(EGuidFormats::DigitsWithHyphens));
	Object.SetStringField(TEXT("componentName"), Snapshot.ComponentName);
	Object.SetStringField(TEXT("graph"), Snapshot.GraphPath);
	Object.SetStringField(TEXT("trigger"), Snapshot.Trigger);
	Object.SetStringField(TEXT("state"),
	                      StaticEnum<EOWTProceduralState>()->GetNameStringByValue(static_cast<int64>(Snapshot.State)));
	Object.SetNumberField(TEXT("generationAttempt"), Snapshot.GenerationAttempt);
	Object.SetStringField(TEXT("reason"), Snapshot.Reason);
}
} // namespace OWTAttributeJson
