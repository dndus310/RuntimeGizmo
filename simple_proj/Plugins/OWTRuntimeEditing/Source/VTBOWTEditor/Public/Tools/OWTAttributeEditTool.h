#pragma once

#include "CoreMinimal.h"
#include "InteractiveTool.h"
#include "InteractiveToolBuilder.h"
#include "InputBehaviorSet.h"
#include "BaseBehaviors/BehaviorTargetInterfaces.h"
#include "OWTAttributeEditTool.generated.h"

class UOWTAttributeEditMode;
class UTransformProxy;
class UVTBOWTBaseTransformGizmo;

UCLASS(Transient)
class VTBOWTEDITOR_API UOWTTransformProperties : public UInteractiveToolPropertySet
{
	GENERATED_BODY()

public:
	UOWTTransformProperties()
	    : Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), Scale(FVector::OneVector)
	{
	}

	UPROPERTY(EditAnywhere, Category = "Transform")
	FVector Location;

	UPROPERTY(EditAnywhere, Category = "Transform")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, Category = "Transform")
	FVector Scale;
};

UCLASS(Transient)
class VTBOWTEDITOR_API UOWTAttributeEditTool : public UInteractiveTool, public IClickBehaviorTarget
{
	GENERATED_BODY()

public:
	UOWTAttributeEditTool();

	virtual void Setup() override;
	virtual void OnTick(float DeltaTime) override;
	virtual void Shutdown(EToolShutdownType ShutdownType) override;
	virtual void OnPropertyModified(UObject* PropertySet, FProperty* Property) override;
	virtual FInputRayHit IsHitByClick(const FInputDeviceRay& ClickPos) override;
	virtual void OnClicked(const FInputDeviceRay& ClickPos) override;

	virtual bool HasAccept() const override
	{
		return false;
	}

	virtual bool HasCancel() const override
	{
		return false;
	}

	void Initialize(UOWTAttributeEditMode& InMode);
	bool ApplyTransform(AActor& Actor, const FTransform& Transform, FString& OutError);
	bool SelectActor(AActor* Actor);

	void ShowGizmo();
	void SynchronizeGizmo();
	void HideGizmo();

	UVTBOWTBaseTransformGizmo* GetGizmo() const
	{
		return TransformGizmo;
	}

	UTransformProxy* GetTransformProxy() const;

private:
	void InitializeSelectionBehavior();
	void ObserveTransform();

	void OnGizmoEditStarted(UTransformProxy* Proxy);
	void OnGizmoTransformChanged(UTransformProxy* Proxy, FTransform Transform);
	void OnGizmoEditEnded(UTransformProxy* Proxy);

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UOWTAttributeEditMode> Mode;

	UPROPERTY(Transient)
	TObjectPtr<UVTBOWTBaseTransformGizmo> TransformGizmo;

	UPROPERTY(Transient)
	TObjectPtr<UOWTTransformProperties> Properties;

	TWeakObjectPtr<USceneComponent> GizmoTarget;
	bool bHidden;
};

UCLASS(Transient)
class VTBOWTEDITOR_API UOWTAttributeEditToolBuilder : public UInteractiveToolBuilder
{
	GENERATED_BODY()

public:
	virtual bool CanBuildTool(const FToolBuilderState& State) const override;
	virtual UInteractiveTool* BuildTool(const FToolBuilderState& State) const override;
};
