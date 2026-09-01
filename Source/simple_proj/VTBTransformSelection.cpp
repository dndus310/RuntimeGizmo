// Copyright Epic Games, Inc. All Rights Reserved.

#include "VTBTransformSelection.h"

#include "BaseBehaviors/SingleClickBehavior.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "VTBTransformTarget.h"

void UVTBTransformSelection::Initialize(
	APlayerController* InPlayerController,
	TUniqueFunction<bool()> CanChangeSelectionCallbackIn,
	TUniqueFunction<void(AActor*)> SelectTargetCallbackIn,
	ECollisionChannel InTraceChannel,
	double InTraceDistance,
	bool bInRequireTransformTargetInterface,
	bool bInClearSelectionOnBackgroundClick)
{
	PlayerController = InPlayerController;
	CanChangeSelectionCallback = MoveTemp(CanChangeSelectionCallbackIn);
	SelectTargetCallback = MoveTemp(SelectTargetCallbackIn);
	TraceChannel = InTraceChannel;
	TraceDistance = InTraceDistance;
	bRequireTransformTargetInterface = bInRequireTransformTargetInterface;
	bClearSelectionOnBackgroundClick = bInClearSelectionOnBackgroundClick;

	ClickBehavior = NewObject<USingleClickInputBehavior>(this);
	ClickBehavior->Initialize(this);

	BehaviorSet = NewObject<UInputBehaviorSet>(this);
	BehaviorSet->Add(ClickBehavior, this);
}

void UVTBTransformSelection::Shutdown()
{
	if (BehaviorSet)
	{
		BehaviorSet->RemoveAll();
	}

	ClickBehavior = nullptr;
	BehaviorSet = nullptr;
	PlayerController.Reset();
	CanChangeSelectionCallback = []() { return true; };
	SelectTargetCallback = [](AActor*) {};
}

FInputRayHit UVTBTransformSelection::IsHitByClick(const FInputDeviceRay& ClickPos)
{
	if (!CanChangeSelectionCallback())
	{
		return FInputRayHit();
	}

	FHitResult Hit;
	if (AActor* TargetActor = FindTransformTarget(ClickPos, Hit))
	{
		FInputRayHit RayHit(Hit.Distance);
		RayHit.SetHitObject(TargetActor);
		return RayHit;
	}

	return bClearSelectionOnBackgroundClick ? FInputRayHit(TNumericLimits<float>::Max()) : FInputRayHit();
}

void UVTBTransformSelection::OnClicked(const FInputDeviceRay& ClickPos)
{
	if (!CanChangeSelectionCallback())
	{
		return;
	}

	FHitResult Hit;
	AActor* TargetActor = FindTransformTarget(ClickPos, Hit);
	if (TargetActor || bClearSelectionOnBackgroundClick)
	{
		SelectTargetCallback(TargetActor);
	}
}

AActor* UVTBTransformSelection::FindTransformTarget(const FInputDeviceRay& ClickPos, FHitResult& HitOut) const
{
	UWorld* World = PlayerController.IsValid() ? PlayerController->GetWorld() : GetWorld();
	if (!World)
	{
		return nullptr;
	}

	const FVector TraceStart = ClickPos.WorldRay.Origin;
	const FVector TraceEnd = TraceStart + ClickPos.WorldRay.Direction.GetSafeNormal() * TraceDistance;

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(VTBTransformSelectionInteraction), true);
	if (APlayerController* Controller = PlayerController.Get())
	{
		if (APawn* Pawn = Controller->GetPawn())
		{
			QueryParams.AddIgnoredActor(Pawn);
		}

		if (AActor* ViewTarget = Controller->GetViewTarget())
		{
			QueryParams.AddIgnoredActor(ViewTarget);
		}
	}

	if (!World->LineTraceSingleByChannel(HitOut, TraceStart, TraceEnd, TraceChannel, QueryParams))
	{
		return nullptr;
	}

	AActor* HitActor = HitOut.GetActor();
	return IsSelectableTransformTarget(HitActor) ? HitActor : nullptr;
}

bool UVTBTransformSelection::IsSelectableTransformTarget(AActor* Actor) const
{
	if (!Actor)
	{
		return false;
	}

	const bool bImplementsTransformTarget = Actor->GetClass()->ImplementsInterface(UVTBTransformTarget::StaticClass());
	if (bImplementsTransformTarget)
	{
		return IVTBTransformTarget::Execute_CanTransform(Actor);
	}

	return !bRequireTransformTargetInterface && Actor->GetRootComponent() != nullptr;
}
