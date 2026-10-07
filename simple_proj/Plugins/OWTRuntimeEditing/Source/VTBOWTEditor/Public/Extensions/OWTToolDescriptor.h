#pragma once

#include "CoreMinimal.h"
#include "InteractiveToolBuilder.h"
#include "OWTToolDescriptor.generated.h"

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTToolDescriptor
{
	GENERATED_BODY()
	FOWTToolDescriptor()
	    : ToolId(NAME_None), Label(), Category(), Description(), BuilderClass(nullptr), bRequiresHistory(false),
	      bRequiresMeshRendering(false)
	{
	}
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ToolId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Label;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Category;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UInteractiveToolBuilder> BuilderClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bRequiresHistory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bRequiresMeshRendering;
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTToolAvailability
{
	GENERATED_BODY()
	FOWTToolAvailability() : ToolId(NAME_None), Label(), Category(), Reason(), bEnabled(false), bActive(false)
	{
	}
	UPROPERTY(BlueprintReadOnly)
	FName ToolId;
	UPROPERTY(BlueprintReadOnly)
	FText Label;
	UPROPERTY(BlueprintReadOnly)
	FText Category;
	UPROPERTY(BlueprintReadOnly)
	FText Reason;
	UPROPERTY(BlueprintReadOnly)
	bool bEnabled;
	UPROPERTY(BlueprintReadOnly)
	bool bActive;
};

namespace OWTToolIds
{
VTBOWTEDITOR_API FName AttributeEdit();
VTBOWTEDITOR_API FName Duplicate();
} // namespace OWTToolIds
