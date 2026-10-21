#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OWTRestoreHookComponent.generated.h"

/** Synchronous bridge between construction and actor initialization. Never retained by the duplicate. */
UCLASS(Transient, NotBlueprintable)
class UOWTRestoreHookComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOWTRestoreHookComponent();
	virtual void InitializeComponent() override;
	void SetRestoreCallback(TFunction<void()> Callback);
	void Restore();

private:
	TFunction<void()> RestoreCallback;
};
