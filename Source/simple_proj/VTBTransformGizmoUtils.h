// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VTBTransformGizmoUtils.generated.h"

class UVTBInteractiveToolsSubsystem;

UCLASS()
class SIMPLE_PROJ_API UVTBTransformGizmoUtils final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "VTB Runtime Tools", meta = (WorldContext = "WorldContextObject"))
	static bool SelectTransformTarget(UObject* WorldContextObject, AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "VTB Runtime Tools", meta = (WorldContext = "WorldContextObject"))
	static void ClearSelection(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "VTB Runtime Tools", meta = (WorldContext = "WorldContextObject"))
	static bool ShouldBlockViewportCameraInput(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "VTB Runtime Tools", meta = (WorldContext = "WorldContextObject"))
	static UVTBInteractiveToolsSubsystem* GetSubsystem(UObject* WorldContextObject);
};
