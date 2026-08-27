// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RuntimeTransformTarget.generated.h"

class USceneComponent;

UINTERFACE(BlueprintType)
class SIMPLE_PROJ_API URuntimeTransformTarget : public UInterface
{
	GENERATED_BODY()
};

class SIMPLE_PROJ_API IRuntimeTransformTarget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Runtime Transform Gizmo")
	USceneComponent* GetRuntimeTransformComponent() const;
};
