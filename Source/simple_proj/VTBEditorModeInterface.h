#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VTBEditorModeInterface.generated.h"

UENUM(BlueprintType)
enum class EActiveGizmoMode : uint8
{
	Transform = 0 UMETA(DisplayName = "Transform"),
	Spline UMETA(DisplayName = "Spline"),
	Count UMETA(Hidden)
};

/** Commands handled by the authoritative GameMode. */
UINTERFACE(BlueprintType)
class SIMPLE_PROJ_API UVTBOWTEditorModeControl : public UInterface
{
	GENERATED_BODY()
};

class SIMPLE_PROJ_API IVTBOWTEditorModeControl
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VTB Editor|Mode")
	void SetEditorState(bool bNewState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VTB Editor|Mode")
	void SetActiveGizmoMode(EActiveGizmoMode NewGizmoMode);
};

/** Read the latest state from GameState without depending on its concrete class. */
UINTERFACE(BlueprintType)
class SIMPLE_PROJ_API UVTBOWTEditorModeState : public UInterface
{
	GENERATED_BODY()
};

class SIMPLE_PROJ_API IVTBOWTEditorModeState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VTB Editor|Mode")
	bool GetEditorState() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "VTB Editor|Mode")
	EActiveGizmoMode GetActiveGizmoMode() const;
};
