#include "Duplication/OWTDuplicationRestoreComponent.h"

UOWTDuplicationRestoreComponent::UOWTDuplicationRestoreComponent() : RestoreCallback()
{
	bWantsInitializeComponent = true;
	bAutoActivate = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UOWTDuplicationRestoreComponent::InitializeComponent()
{
	Super::InitializeComponent();
	Restore();
}

void UOWTDuplicationRestoreComponent::SetRestoreCallback(TFunction<void()> Callback)
{
	RestoreCallback = MoveTemp(Callback);
}

void UOWTDuplicationRestoreComponent::Restore()
{
	// Clear the stored callback before invoking it. Component registration can reenter initialization.
	TFunction<void()> Callback = MoveTemp(RestoreCallback);
	if (Callback)
	{
		Callback();
	}
}
