#include "Duplication/Restore/OWTRestoreHookComponent.h"

UOWTRestoreHookComponent::UOWTRestoreHookComponent() : RestoreCallback()
{
	bWantsInitializeComponent = true;
	bAutoActivate = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UOWTRestoreHookComponent::InitializeComponent()
{
	Super::InitializeComponent();
	Restore();
}

void UOWTRestoreHookComponent::SetRestoreCallback(TFunction<void()> Callback)
{
	RestoreCallback = MoveTemp(Callback);
}

void UOWTRestoreHookComponent::Restore()
{
	// Clear the stored callback before invoking it. Component registration can reenter initialization.
	TFunction<void()> Callback = MoveTemp(RestoreCallback);
	if (Callback)
	{
		Callback();
	}
}
