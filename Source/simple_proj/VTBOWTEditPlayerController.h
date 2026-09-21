#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VTBOWTEditPlayerController.generated.h"

UCLASS()
class AVTBCorePlayerController : public APlayerController
{
	GENERATED_BODY()
};

UCLASS()
class SIMPLE_PROJ_API AVTBOWTEditPlayerController : public AVTBCorePlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void UndoRuntimeEdit();
	void RedoRuntimeEdit();
};
