#pragma once

#include "CoreMinimal.h"
#include "OWTEditContexts.generated.h"

class AActor;

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTSelectObjectContext
{
	GENERATED_BODY()

public:
	FOWTSelectObjectContext() : SelectedObject()
	{
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OWT|Editing")
	TWeakObjectPtr<AActor> SelectedObject;
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTUndoContext
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTRedoContext
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTToggleCoordinateSystemContext
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTToggleTransformSplineContext
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTSetTranslationContext
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTSetRotationContext
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTSetScaleContext
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTHideSelectionGizmoContext
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTDuplicateSelectionContext
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTToggleEditingContext
{
	GENERATED_BODY()
};

// Semantic pointer state. No input action, mapping context or key identifier crosses the boundary.
USTRUCT()
struct VTBOWTEDITOR_API FOWTGizmoPointerContext
{
	GENERATED_BODY()

public:
	FOWTGizmoPointerContext()
	    : RayOrigin(FVector::ZeroVector), RayDirection(FVector::ForwardVector), ScreenPosition(FVector2D::ZeroVector),
	      bPressed(false), bDown(false), bReleased(false)
	{
	}

public:
	UPROPERTY()
	FVector RayOrigin;
	UPROPERTY()
	FVector RayDirection;
	UPROPERTY()
	FVector2D ScreenPosition;
	UPROPERTY()
	bool bPressed;
	UPROPERTY()
	bool bDown;
	UPROPERTY()
	bool bReleased;
};
