#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StructUtils/InstancedStruct.h"
#include "OWTEditContextReceiver.generated.h"

UINTERFACE(BlueprintType, Blueprintable)
class VTBOWTEDITOR_API UOWTEditContextReceiver : public UInterface
{
	GENERATED_BODY()
};

class VTBOWTEDITOR_API IOWTEditContextReceiver
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "OWT|Editing")
	bool ReceiveEditContext(const FInstancedStruct& Context);
};
