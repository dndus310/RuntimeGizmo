#pragma once

#include "CoreMinimal.h"
#include "Events/OWTEventTypes.h"
#include "OWTAttributeTypes.generated.h"

UENUM(BlueprintType)
enum class EOWTTransformField : uint8
{
	LocationX,
	LocationY,
	LocationZ,
	RotationRoll,
	RotationPitch,
	RotationYaw,
	ScaleX,
	ScaleY,
	ScaleZ
};

UENUM(BlueprintType)
enum class EOWTTransformEditPhase : uint8
{
	Begin,
	Update,
	Commit,
	Cancel
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTAttributeSnapshot
{
	GENERATED_BODY()

	FOWTAttributeSnapshot();

	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	FString EditorId;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	FString ObjectId;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	FString ObjectName;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	FString ObjectClass;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	FString DisabledReason;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	FString ActiveMode;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	FString GizmoMode;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	FString GizmoCoordinateSystem;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	int32 SelectionRevision;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	int32 StateRevision;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	bool bEditingEnabled;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	bool bHasSelection;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	bool bCanEditTransform;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	bool bIsModifying;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	bool bHasChanges;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Attributes")
	FTransform Transform;
};
