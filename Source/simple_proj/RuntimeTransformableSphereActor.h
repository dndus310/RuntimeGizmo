// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeTransformTarget.h"
#include "RuntimeTransformableSphereActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class SIMPLE_PROJ_API ARuntimeTransformableSphereActor : public AActor, public IRuntimeTransformTarget
{
	GENERATED_BODY()

public:
	ARuntimeTransformableSphereActor();

	void SetSelectedByRuntimeGizmo(bool bSelected);
	virtual USceneComponent* GetRuntimeTransformComponent_Implementation() const override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UFUNCTION()
	void HandleBeginCursorOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void HandleEndCursorOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void HandleClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	void UpdateHighlightState();

	UPROPERTY(VisibleAnywhere, Category = "Runtime Transform Gizmo")
	TObjectPtr<UStaticMeshComponent> SphereMesh;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Highlight")
	int32 HoverStencilValue = 1;

	UPROPERTY(EditAnywhere, Category = "Runtime Transform Gizmo|Highlight")
	int32 SelectedStencilValue = 2;

	bool bHovered = false;
	bool bSelectedByRuntimeGizmo = false;
};
