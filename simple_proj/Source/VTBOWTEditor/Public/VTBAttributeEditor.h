#pragma once

#include "CoreMinimal.h"
#include "Context/SaveTransformContext.h"
#include "GameFramework/Actor.h"
#include "VTBAttributeEditor.generated.h"

UCLASS(Blueprintable)
class VTBOWTEDITOR_API AVTBAttributeEditor : public AActor
{
	GENERATED_BODY()

public:
	virtual void SaveHistory_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "History")
	void SaveHistory();

	UFUNCTION(BlueprintCallable, Category = "History")
	void SaveHistoryFromTransformContext(FSaveTransformContext Context);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "History")
	void Undo();

	UFUNCTION(BlueprintNativeEvent, Category = "History")
	void Redo();
};
