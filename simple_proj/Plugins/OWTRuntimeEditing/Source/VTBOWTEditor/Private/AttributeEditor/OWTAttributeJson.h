#pragma once

#include "CoreMinimal.h"
#include "Duplication/OWTDuplicationRequest.h"
#include "Events/OWTAttributeTypes.h"
#include "State/OWTEditingSessionTypes.h"

class FJsonObject;

// The JSON boundary has no actor ownership or event dispatch responsibilities.
namespace OWTAttributeJson
{
struct FRequestError
{
	FRequestError() = default;
	FRequestError(const TCHAR* InCode, const TCHAR* InReason) : Code(InCode), Reason(InReason)
	{
	}

	FString Code;
	FString Reason;
};

struct FTransformRequest
{
	FTransformRequest() : Source(), Phase(), OperationId(), Field(EOWTTransformField::LocationX), Value(0)
	{
	}

	FString Source;
	FString Phase;
	FGuid OperationId;
	EOWTTransformField Field;
	double Value;
};

FString SerializeObject(const TSharedRef<FJsonObject>& Object);
bool ReadObject(const FString& Json, TSharedPtr<FJsonObject>& Object);
bool ReadString(const FJsonObject& Object, const TCHAR* Field, FString& Value);
bool ReadNumber(const FJsonObject& Object, const TCHAR* Field, double& Value);
bool ReadRevision(const FJsonObject& Object, const TCHAR* Field, int32& Value);

TSharedRef<FJsonObject> MakeSnapshotObject(const FOWTAttributeSnapshot& Snapshot, FGuid ActiveOperation,
                                           const FString& RequestId, const FString& Source);
TSharedRef<FJsonObject> MakeRequestObject(const FOWTAttributeSnapshot& Expected);
TSharedRef<FJsonObject> MakeVectorObject(const FVector& Value);
bool ParseSnapshot(const FString& Json, FOWTAttributeSnapshot& OutSnapshot);
bool ReadTransformRequest(const FJsonObject& Request, FTransformRequest& OutRequest, FRequestError& Error);
bool ReadDuplicationOptions(const FJsonObject& Request, const FVector& DefaultWorldOffset,
                            FOWTDuplicationOptions& Options, FRequestError& Error);

void WriteModeSnapshot(FJsonObject& Object, const FOWTModeSnapshot& Snapshot);
void WriteDuplicationSnapshot(FJsonObject& Object, const FOWTDuplicationOperationSnapshot& Snapshot);
void WriteProceduralSnapshot(FJsonObject& Object, const FOWTProceduralComponentSnapshot& Snapshot);
void WriteProceduralStates(FJsonObject& Object, FGuid OperationId,
                           const TArray<FOWTProceduralComponentSnapshot>& ProceduralComponents);

const TCHAR* GetTransformFieldName(EOWTTransformField Field);
const TCHAR* GetEditPhaseName(EOWTTransformEditPhase Phase);
bool IsFiniteVector(const FVector& Vector);
bool IsFiniteTransform(const FTransform& Transform);
FTransform MakeTransformWithField(const FTransform& Original, EOWTTransformField Field, double Value);
} // namespace OWTAttributeJson
