// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VTBTransformTarget.generated.h"

class USceneComponent;

UINTERFACE(BlueprintType)
class SIMPLE_PROJ_API UVTBTransformTarget : public UInterface
{
	GENERATED_BODY()
};

class SIMPLE_PROJ_API IVTBTransformTarget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VTB Transform Gizmo")
	bool CanTransform() const;
	virtual bool CanTransform_Implementation() const { return true; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VTB Transform Gizmo")
	USceneComponent* GetTransformComponent() const;
	virtual USceneComponent* GetTransformComponent_Implementation() const { return nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "VTB Transform Gizmo")
	void NotifySelectionChanged(bool bSelected);
	virtual void NotifySelectionChanged_Implementation(bool bSelected) {}
};
