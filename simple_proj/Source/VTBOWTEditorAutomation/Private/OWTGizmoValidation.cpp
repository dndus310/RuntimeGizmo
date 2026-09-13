#include "OWTGizmoValidation.h"

#include "VTBOWTSpectator.h"
#include "VTBOWTEditorSubsystem.h"
#include "Context/OWTEditContexts.h"
#include "BaseGizmos/CombinedTransformGizmo.h"
#include "BaseGizmos/GizmoBaseComponent.h"
#include "BaseGizmos/GizmoCircleComponent.h"
#include "BaseGizmos/ViewAdjustedStaticMeshGizmoComponent.h"
#include "BaseGizmos/GizmoRenderingUtil.h"
#include "BaseGizmos/ViewBasedTransformAdjusters.h"
#include "BaseGizmos/HitTargets.h"
#include "BaseGizmos/TransformProxy.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "Context/VTBOWTEditorToolsContext.h"
#include "Gizmos/Base/VTBOWTBaseTransformGizmo.h"
#include "Gizmos/Custom/VTBOWTCustomTransformGizmo.h"
#include "Gizmos/Custom/VTBOWTTransformGizmoActor.h"
#include "InteractiveGizmoManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PawnMovementComponent.h"

namespace
{
struct FOWTValidationActionInstance : FInputActionInstance
{
	FOWTValidationActionInstance(const UInputAction* Action, FInputActionValue InValue) : FInputActionInstance(Action)
	{
		Value = InValue;
		TriggerEvent = ETriggerEvent::Triggered;
	}
};

bool ExecuteCameraInput(UEnhancedInputComponent& Input, const UInputAction* Action, ETriggerEvent EventType,
                        const FInputActionValue& Value)
{
	int32 Executed = 0;
	for (const auto& Binding : Input.GetActionEventBindings())
	{
		if (Binding->GetAction() != Action || Binding->GetTriggerEvent() != EventType)
		{
			continue;
		}
		Binding->Execute(FOWTValidationActionInstance(Action, Value));
		++Executed;
	}
	return Executed == 1;
}

bool SendPointer(AVTBOWTSpectator& Pawn, const FVector& Origin, const FVector& Direction, bool bPressed, bool bDown,
                 bool bReleased)
{
	FOWTGizmoPointerContext Pointer;
	Pointer.RayOrigin = Origin;
	Pointer.RayDirection = Direction;
	Pointer.bPressed = bPressed;
	Pointer.bDown = bDown;
	Pointer.bReleased = bReleased;
	return Pawn.SendEditContext(FInstancedStruct::Make(Pointer));
}

bool Drag(AVTBOWTSpectator& Pawn, UVTBOWTEditorSubsystem& Hub, const FVector& Start, const FVector& End,
          const FVector& Direction)
{
	bool bOK = SendPointer(Pawn, Start, Direction, true, true, false);
	bOK &= Hub.HasGizmoCapture();
	bOK &= SendPointer(Pawn, End, Direction, false, true, false);
	bOK &= SendPointer(Pawn, End, Direction, false, false, true);
	bOK &= !Hub.HasGizmoCapture();
	return bOK;
}

bool ValidateTranslation(AVTBOWTSpectator& Pawn, UVTBOWTEditorSubsystem& Hub, AActor& Target)
{
	FOWTGizmoSnapSettings Settings;
	bool bOK = Hub.SetGizmoSnapSettings(Settings);
	Hub.SetTransformGizmoMode(EToolContextTransformGizmoMode::Translation);
	PrepareOWTGizmoHeadlessHandles(*Hub.GetTransformGizmo()->GetGizmoActor());
	Hub.GetTransformGizmo()->SetNewGizmoTransform(FTransform::Identity);
	bOK &= Drag(Pawn, Hub, FVector(50, -100, 0), FVector(73, -100, 0), FVector::YAxisVector);
	bOK &= Target.GetActorLocation().Equals(FVector(20, 0, 0), 0.01);
	UE_LOG(LogTemp, Display, TEXT("OWT translation positive: %d %s"), bOK, *Target.GetActorLocation().ToString());

	Hub.GetTransformGizmo()->SetNewGizmoTransform(FTransform::Identity);
	bOK &= Drag(Pawn, Hub, FVector(50, -100, 0), FVector(27, -100, 0), FVector::YAxisVector);
	bOK &= Target.GetActorLocation().Equals(FVector(-20, 0, 0), 0.01);
	UE_LOG(LogTemp, Display, TEXT("OWT translation negative: %d %s"), bOK, *Target.GetActorLocation().ToString());

	Settings.TranslationStep = 25;
	bOK &= Hub.SetGizmoSnapSettings(Settings);
	Hub.GetTransformGizmo()->SetNewGizmoTransform(FTransform::Identity);
	bOK &= Drag(Pawn, Hub, FVector(50, -100, 0), FVector(88, -100, 0), FVector::YAxisVector);
	bOK &= Target.GetActorLocation().Equals(FVector(50, 0, 0), 0.01);
	UE_LOG(LogTemp, Display, TEXT("OWT translation step 25: %d %s"), bOK, *Target.GetActorLocation().ToString());

	Settings.bTranslationEnabled = false;
	bOK &= Hub.SetGizmoSnapSettings(Settings);
	Hub.GetTransformGizmo()->SetNewGizmoTransform(FTransform::Identity);
	bOK &= Drag(Pawn, Hub, FVector(50, -100, 0), FVector(73, -100, 0), FVector::YAxisVector);
	bOK &= Target.GetActorLocation().Equals(FVector(23, 0, 0), 0.01);
	UE_LOG(LogTemp, Display, TEXT("OWT translation unsnapped: %d %s"), bOK, *Target.GetActorLocation().ToString());
	UE_LOG(LogTemp, Display, TEXT("OWT translation snapping: %d"), bOK);
	return bOK;
}

bool ValidateRotation(AVTBOWTSpectator& Pawn, UVTBOWTEditorSubsystem& Hub, AActor& Target)
{
	bool bOK = Hub.SetGizmoSnapSettings(FOWTGizmoSnapSettings());
	Hub.SetTransformGizmoMode(EToolContextTransformGizmoMode::Rotation);
	ACombinedTransformGizmoActor* Actor = Hub.GetTransformGizmo()->GetGizmoActor();
	const TArray<UPrimitiveComponent*> Handles = {Actor->RotateX, Actor->RotateY, Actor->RotateZ};
	const FVector Axes[] = {FVector::XAxisVector, FVector::YAxisVector, FVector::ZAxisVector};
	PrepareOWTGizmoHeadlessHandles(*Actor);
	const UStaticMesh* CircleMesh = CastChecked<UStaticMeshComponent>(Actor->RotateX)->GetStaticMesh();
	const double Radius = CircleMesh->GetBoundingBox().Max.Y - 1.0;
	const double StartCoordinate = Radius / FMath::Sqrt(2.0);

	for (int32 AxisIndex = 0; AxisIndex < 3; ++AxisIndex)
	{
		Hub.GetTransformGizmo()->SetNewGizmoTransform(FTransform::Identity);
		const FVector Axis = Axes[AxisIndex];
		const FVector StartPoint =
		    Handles[AxisIndex]->GetComponentTransform().TransformPosition(FVector(0, StartCoordinate, StartCoordinate));
		const FVector Start = StartPoint - Axis * 100;
		const FVector End = FQuat(Axis, FMath::DegreesToRadians(22.0)).RotateVector(StartPoint) - Axis * 100;
		bOK &= SendPointer(Pawn, Start, Axis, true, true, false);
		bOK &= Hub.HasGizmoCapture();
		Hub.Tick(0.016f);
		for (int32 OtherIndex = 0; OtherIndex < 3; ++OtherIndex)
		{
			bOK &= Handles[OtherIndex]->IsVisible() == (OtherIndex == AxisIndex);
			if (OtherIndex == AxisIndex)
			{
				continue;
			}
			// The same runtime hit target used by click/hover behaviors must reject hidden axes.
			UGizmoComponentHitTarget* HitTarget = NewObject<UGizmoComponentHitTarget>();
			HitTarget->Component = Handles[OtherIndex];
			FVector OtherStart(StartCoordinate);
			OtherStart[OtherIndex] = -100;
			bOK &= !HitTarget->IsHit(FInputDeviceRay(FRay(OtherStart, Axes[OtherIndex]))).bHit;
		}
		bOK &= SendPointer(Pawn, End, Axis, false, true, false);
		bOK &= Target.GetActorQuat().Equals(FQuat(Axis, FMath::DegreesToRadians(15.0)), 0.001);
		bOK &= SendPointer(Pawn, End, Axis, false, false, true);
		bOK &= !Hub.HasGizmoCapture();
		bOK &= Actor->RotateX->IsVisible() && Actor->RotateY->IsVisible() && Actor->RotateZ->IsVisible();
	}

	// Changing modes during capture must restore the focus state before applying the new mode's visibility.
	Hub.GetTransformGizmo()->SetNewGizmoTransform(FTransform::Identity);
	bOK &= SendPointer(Pawn, FVector(StartCoordinate, StartCoordinate, -100), FVector::ZAxisVector, true, true, false);
	Hub.SetTransformGizmoMode(EToolContextTransformGizmoMode::Translation);
	bOK &= !Hub.HasGizmoCapture() && !Actor->RotateX->IsVisible() && !Actor->RotateY->IsVisible() &&
	       !Actor->RotateZ->IsVisible();
	Hub.SetTransformGizmoMode(EToolContextTransformGizmoMode::Rotation);
	bOK &= Actor->RotateX->IsVisible() && Actor->RotateY->IsVisible() && Actor->RotateZ->IsVisible();
	UE_LOG(LogTemp, Display, TEXT("OWT rotation snapping/isolation/restoration: %d"), bOK);
	return bOK;
}

bool ValidateScale(AVTBOWTSpectator& Pawn, UVTBOWTEditorSubsystem& Hub, AActor& Target)
{
	FOWTGizmoSnapSettings Settings;
	bool bOK = Hub.SetGizmoSnapSettings(Settings);
	Hub.SetTransformGizmoMode(EToolContextTransformGizmoMode::Scale);
	PrepareOWTGizmoHeadlessHandles(*Hub.GetTransformGizmo()->GetGizmoActor());
	Hub.GetTransformGizmo()->SetNewGizmoTransform(FTransform::Identity);
	// The engine's axis scale source maps 7 units to +0.14 scale, snapped to +0.1.
	bOK &= Drag(Pawn, Hub, FVector(80, -100, 0), FVector(87, -100, 0), FVector::YAxisVector);
	bOK &= Target.GetActorScale3D().Equals(FVector(1.1, 1, 1), 0.001);

	Settings.ScaleStep = 0.25;
	bOK &= Hub.SetGizmoSnapSettings(Settings);
	Hub.GetTransformGizmo()->SetNewGizmoTransform(FTransform::Identity);
	bOK &= Drag(Pawn, Hub, FVector(80, -100, 0), FVector(87, -100, 0), FVector::YAxisVector);
	bOK &= Target.GetActorScale3D().Equals(FVector(1.25, 1, 1), 0.001);

	Settings.bScaleEnabled = false;
	bOK &= Hub.SetGizmoSnapSettings(Settings);
	Hub.GetTransformGizmo()->SetNewGizmoTransform(FTransform::Identity);
	bOK &= Drag(Pawn, Hub, FVector(80, -100, 0), FVector(87, -100, 0), FVector::YAxisVector);
	bOK &= Target.GetActorScale3D().Equals(FVector(1.14, 1, 1), 0.001);
	UE_LOG(LogTemp, Display, TEXT("OWT scale snapping: %d"), bOK);
	return bOK;
}
} // namespace

bool ValidateOWTGizmoSnapping(AVTBOWTSpectator& Pawn, UVTBOWTEditorSubsystem& Hub, AActor& Target)
{
	ACombinedTransformGizmoActor* Actor = Hub.GetTransformGizmo()->GetGizmoActor();
	PrepareOWTGizmoHeadlessHandles(*Actor);
	Hub.SetCoordinateSystem(EToolContextCoordinateSystem::World);
	bool bOK = ValidateTranslation(Pawn, Hub, Target);
	bOK &= ValidateRotation(Pawn, Hub, Target);
	bOK &= ValidateScale(Pawn, Hub, Target);
	FOWTGizmoSnapSettings InvalidSettings;
	InvalidSettings.TranslationStep = 0;
	bOK &= !Hub.SetGizmoSnapSettings(InvalidSettings);
	bOK &= Hub.SetGizmoSnapSettings(FOWTGizmoSnapSettings());
	Hub.GetTransformGizmo()->SetNewGizmoTransform(FTransform::Identity);
	return bOK;
}

bool ValidateOWTCameraNavigation(AVTBOWTSpectator& Pawn, UVTBOWTEditorSubsystem& Hub)
{
	UWorld* World = Pawn.GetWorld();
	AVTBOWTSpectator* Camera = World->SpawnActor<AVTBOWTSpectator>(Pawn.GetClass());
	APlayerController* Controller = World->SpawnActor<APlayerController>();
	Controller->Player = NewObject<ULocalPlayer>(GEngine);
	Controller->PlayerInput = NewObject<UEnhancedPlayerInput>(Controller);
	Controller->Possess(Camera);
	UEnhancedInputComponent* Input = NewObject<UEnhancedInputComponent>(Camera);
	Camera->SetupPlayerInputComponent(Input);
	bool bOK = Camera->IsLocallyControlled();
	const FInputActionValue Move(FVector2D(0, 1));
	const FInputActionValue Look(FVector2D(10, 5));

	// WASD and look events do nothing without RMB.
	bOK &= ExecuteCameraInput(*Input, Camera->CameraMoveAction, ETriggerEvent::Triggered, Move);
	bOK &= ExecuteCameraInput(*Input, Camera->CameraLookAction, ETriggerEvent::Triggered, Look);
	bOK &= Camera->GetPendingMovementInputVector().IsNearlyZero() && Controller->RotationInput.IsNearlyZero();
	bOK &= ExecuteCameraInput(*Input, Camera->CameraNavigateAction, ETriggerEvent::Started, FInputActionValue(true));
	bOK &= Camera->IsCameraNavigating();
	bOK &= ExecuteCameraInput(*Input, Camera->CameraMoveAction, ETriggerEvent::Triggered, Move);
	bOK &= ExecuteCameraInput(*Input, Camera->CameraLookAction, ETriggerEvent::Triggered, Look);
	bOK &= !Camera->GetPendingMovementInputVector().IsNearlyZero() && !Controller->RotationInput.IsNearlyZero();
	bOK &= !Camera->SendEditContext(FInstancedStruct::Make<FOWTSetTranslationContext>());
	Camera->GetMovementComponent()->Velocity = FVector(100, 0, 0);
	bOK &= ExecuteCameraInput(*Input, Camera->CameraNavigateAction, ETriggerEvent::Completed, FInputActionValue(false));
	bOK &= !Camera->IsCameraNavigating() && Camera->GetMovementComponent()->Velocity.IsNearlyZero() &&
	       Camera->GetPendingMovementInputVector().IsNearlyZero() && Controller->RotationInput.IsNearlyZero();

	Hub.SetTransformGizmoMode(EToolContextTransformGizmoMode::Translation);
	PrepareOWTGizmoHeadlessHandles(*Hub.GetTransformGizmo()->GetGizmoActor());
	bOK &= SendPointer(Pawn, FVector(50, -100, 0), FVector::YAxisVector, true, true, false);
	bOK &= Hub.HasGizmoCapture();
	bOK &= ExecuteCameraInput(*Input, Camera->CameraNavigateAction, ETriggerEvent::Started, FInputActionValue(true));
	bOK &= ExecuteCameraInput(*Input, Camera->CameraMoveAction, ETriggerEvent::Triggered, Move);
	bOK &= ExecuteCameraInput(*Input, Camera->CameraLookAction, ETriggerEvent::Triggered, Look);
	bOK &= !Camera->IsCameraNavigating() && Camera->GetPendingMovementInputVector().IsNearlyZero() &&
	       Controller->RotationInput.IsNearlyZero();
	bOK &= SendPointer(Pawn, FVector(50, -100, 0), FVector::YAxisVector, false, false, true);

	// Check the actual IMC modifiers, including negative/backward input.
	for (const FEnhancedActionKeyMapping& Mapping : Camera->MappingContext->GetMappings())
	{
		if (Mapping.Action != Camera->CameraMoveAction)
		{
			continue;
		}
		FInputActionValue Value(FVector2D(1, 0));
		for (const UInputModifier* Modifier : Mapping.Modifiers)
		{
			Value = Modifier->ModifyRaw(nullptr, Value, 0.016f);
		}
		const FVector2D Expected = Mapping.Key == EKeys::W   ? FVector2D(0, 1)
		                           : Mapping.Key == EKeys::S ? FVector2D(0, -1)
		                           : Mapping.Key == EKeys::A ? FVector2D(-1, 0)
		                                                     : FVector2D(1, 0);
		bOK &= Value.Get<FVector2D>().Equals(Expected);
	}
	Controller->UnPossess();
	Camera->Destroy();
	Controller->Destroy();
	UE_LOG(LogTemp, Display, TEXT("OWT camera RMB/WASD/look/exclusive gizmo capture: %d"), bOK);
	return bOK;
}

void PrepareOWTGizmoHeadlessHandles(ACombinedTransformGizmoActor& Actor)
{
	// Keep the real mesh and hit target; only remove view-dependent transforms in headless tests.
	TInlineComponentArray<UPrimitiveComponent*> Components(&Actor);
	for (UPrimitiveComponent* Component : Components)
	{
		if (UViewAdjustedStaticMeshGizmoComponent* Mesh = Cast<UViewAdjustedStaticMeshGizmoComponent>(Component))
		{
			using FAdjuster = UE::GizmoRenderingUtil::FSubGizmoTransformAdjuster;
			FAdjuster::FSettings Settings;
			Settings.bKeepConstantViewSize = false;
			Settings.bMirrorBasedOnOctant = false;
			StaticCastSharedPtr<FAdjuster>(Mesh->GetTransformAdjuster())->SetSettings(Settings);
			Mesh->SetRenderVisibilityFunction(nullptr);
		}
		else if (UGizmoBaseComponent* Legacy = Cast<UGizmoBaseComponent>(Component))
		{
			Legacy->SetGizmoViewContext(nullptr);
		}
	}
}

bool ValidateOWTBaseGizmoMaterials(UVTBOWTEditorSubsystem& Hub)
{
	ACombinedTransformGizmoActor* Actor = Hub.GetTransformGizmo()->GetGizmoActor();
	UMaterialInterface* Parent = LoadObject<UMaterialInterface>(
	    nullptr, TEXT("/Game/VTBOWT/Materials/MI_OWTGizmo_NotOccluded.MI_OWTGizmo_NotOccluded"));
	bool bOK = Hub.GetTransformGizmo()->GetClass() == UVTBOWTBaseTransformGizmo::StaticClass();
	bOK &= Actor->GetClass() == ACombinedTransformGizmoActor::StaticClass();
	bOK &= Actor->TranslateXY && Actor->TranslateYZ && Actor->TranslateXZ && Actor->PlaneScaleXY &&
	       Actor->PlaneScaleYZ && Actor->PlaneScaleXZ && Actor->UniformScale;
	bool bOccluded = true;
	FGuid Expression;
	bOK &= Parent &&
	       Parent->GetStaticSwitchParameterValue(FMaterialParameterInfo(TEXT("OccludeByCustomDepth")), bOccluded,
	                                             Expression) &&
	       !bOccluded;
	TInlineComponentArray<UViewAdjustedStaticMeshGizmoComponent*> Components(Actor);
	bOK &= Components.Num() == 22;
	for (UViewAdjustedStaticMeshGizmoComponent* Component : Components)
	{
		for (int32 Slot = 0; Slot < Component->GetNumMaterials(); ++Slot)
		{
			const UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(Component->GetMaterial(Slot));
			bOK &= MID && MID->Parent == Parent;
		}
		if (UMaterialInterface* Hover = Component->GetHoverOverrideMaterial())
		{
			const UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(Hover);
			bOK &= MID && MID->Parent == Parent;
		}
	}
	FLinearColor Color;
	bOK &=
	    Actor->TranslateX->GetMaterial(0)->GetVectorParameterValue(FMaterialParameterInfo(TEXT("GizmoColor")), Color);
	bOK &= Color.Equals(UE::GizmoRenderingUtil::GetDefaultAxisColor(EAxis::X));
	UE_LOG(LogTemp, Display, TEXT("OWT default handles/MIDs/static switch/colors: %d (mesh components %d)"), bOK,
	       Components.Num());
	return bOK;
}

bool ValidateOWTCustomGizmo(AVTBOWTSpectator& Pawn, UVTBOWTEditorSubsystem& Hub, AActor& Target)
{
	Hub.HideSelectionGizmo();
	UInteractiveGizmoManager* Manager = Hub.GetToolsContext()->GizmoManager;
	UVTBOWTCustomTransformGizmo* Gizmo =
	    Cast<UVTBOWTCustomTransformGizmo>(Manager->CreateGizmo(TEXT("OWT.CustomTransform"), FString(), &Target));
	if (!Gizmo)
	{
		return false;
	}

	Gizmo->SetTargetComponent(*Target.GetRootComponent());
	Gizmo->SetNewGizmoTransform(FTransform::Identity);
	Hub.SetTransformGizmoMode(EToolContextTransformGizmoMode::Rotation);
	Gizmo->Tick(0.f);
	ACombinedTransformGizmoActor* Actor = Gizmo->GetGizmoActor();
	PrepareOWTGizmoHeadlessHandles(*Actor);
	bool bOK = Actor->IsA<AVTBOWTTransformGizmoActor>() && !Actor->TranslateXY && !Actor->UniformScale;
	const double Coordinate = 80.0 / FMath::Sqrt(2.0);
	bOK &= SendPointer(Pawn, FVector(Coordinate, Coordinate, -100), FVector::ZAxisVector, true, true, false);
	bOK &= Hub.HasGizmoCapture() && !Actor->RotateX->IsVisible() && !Actor->RotateY->IsVisible();
	bOK &= SendPointer(Pawn, FVector(Coordinate, Coordinate, -100), FVector::ZAxisVector, false, false, true);
	bOK &= !Hub.HasGizmoCapture() && Actor->RotateX->IsVisible() && Actor->RotateY->IsVisible();
	TWeakObjectPtr<AActor> PreviousActor = Actor;
	Manager->DestroyGizmo(Gizmo);
	bOK &= !PreviousActor.IsValid();
	Hub.ShowSelectionGizmo();
	bOK &= Hub.GetTransformGizmo() && Hub.GetTransformGizmo()->GetClass() == UVTBOWTBaseTransformGizmo::StaticClass();
	UE_LOG(LogTemp, Display, TEXT("OWT custom gizmo creation/isolation/cleanup/default restoration: %d"), bOK);
	return bOK;
}
