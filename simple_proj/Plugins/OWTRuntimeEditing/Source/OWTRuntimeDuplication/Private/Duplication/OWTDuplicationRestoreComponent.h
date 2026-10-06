#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OWTDuplicationRestoreComponent.generated.h"

/** Synchronous bridge between construction and actor initialization. Never retained by the duplicate. */
UCLASS(Transient, NotBlueprintable)
class UOWTDuplicationRestoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOWTDuplicationRestoreComponent();

	virtual void InitializeComponent() override;
	void SetRestoreCallback(TFunction<void()> Callback);
	void Restore();

private:
	TFunction<void()> RestoreCallback;
};
