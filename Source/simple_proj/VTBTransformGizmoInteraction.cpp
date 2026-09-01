// Copyright Epic Games, Inc. All Rights Reserved.

#include "VTBTransformGizmoInteraction.h"

#include "BaseGizmos/CombinedTransformGizmo.h"
#include "BaseGizmos/TransformGizmoUtil.h"
#include "BaseGizmos/TransformProxy.h"
#include "BaseGizmos/ViewAdjustedStaticMeshGizmoComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/Engine.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolsContext.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialParameters.h"
#include "VTBTransformTarget.h"

namespace
{
	const FName VTBAlwaysOnTopGizmoTag(TEXT("VTBAlwaysOnTopGizmoMaterial"));
	const FName GizmoColorParameterName(TEXT("GizmoColor"));
	const TCHAR* AlwaysOnTopGizmoMaterialPath = TEXT("/Engine/InteractiveToolsFramework/Materials/GizmoComponentMaterial_NotDimmed");
	constexpr double RotationAxisIsolationMinAngleDegrees = 0.25;
	constexpr double RotationAxisIsolationDotThreshold = 0.94;

	void SetComponentVisibility(UPrimitiveComponent* Component, bool bVisible)
	{
		if (IsValid(Component))
		{
			Component->SetVisibility(bVisible);
		}
	}

	bool IsGizmoComponentHovered(UPrimitiveComponent* Component)
	{
		if (UViewAdjustedStaticMeshGizmoComponent* MeshGizmoComponent = Cast<UViewAdjustedStaticMeshGizmoComponent>(Component))
		{
			return MeshGizmoComponent->IsBeingHovered();
		}

		return false;
	}
}

void UVTBTransformGizmoInteraction::Initialize(
	UInteractiveToolsContext* InToolsContext,
	TUniqueFunction<void(bool)> TransformEditStateChangedCallbackIn)
{
	ToolsContext = InToolsContext;
	TransformEditStateChangedCallback = TransformEditStateChangedCallbackIn
		? MoveTemp(TransformEditStateChangedCallbackIn)
		: TUniqueFunction<void(bool)>([](bool) {});
}

void UVTBTransformGizmoInteraction::Shutdown()
{
	ClearSelection();
	ToolsContext = nullptr;
	TransformEditStateChangedCallback = [](bool) {};
}

bool UVTBTransformGizmoInteraction::SelectTarget(
	AActor* TargetActor,
	bool bRequireTransformTargetInterface,
	const FVTBTransformGizmoSettings& Settings)
{
	if (!TargetActor)
	{
		ClearSelection();
		return true;
	}

	if (TargetActor == SelectedActor.Get())
	{
		return true;
	}

	if (!ToolsContext || !ToolsContext->GizmoManager)
	{
		return false;
	}

	USceneComponent* TransformComponent = ResolveTransformComponent(TargetActor, bRequireTransformTargetInterface);
	if (!TransformComponent)
	{
		return false;
	}

	ClearSelection();

	SelectedActor = TargetActor;
	SelectedComponent = TransformComponent;

	if (UPrimitiveComponent* SelectedPrimitive = Cast<UPrimitiveComponent>(SelectedComponent.Get()))
	{
		ApplySelectedOutline(SelectedPrimitive);
	}

	NotifySelectionChanged(SelectedActor.Get(), true);

	ActiveProxy = NewObject<UTransformProxy>(this, TEXT("VTBTransformProxy"));
	ActiveProxy->AddComponent(SelectedComponent.Get(), false);
	ActiveProxy->OnBeginTransformEdit.AddUObject(this, &UVTBTransformGizmoInteraction::HandleBeginTransformEdit);
	ActiveProxy->OnEndTransformEdit.AddUObject(this, &UVTBTransformGizmoInteraction::HandleEndTransformEdit);
	ActiveProxy->OnTransformChanged.AddUObject(this, &UVTBTransformGizmoInteraction::HandleProxyTransformChanged);

	EnsureTransformGizmo(Settings);

	if (!ActiveGizmo)
	{
		ClearSelection();
		return false;
	}

	ActiveGizmo->SetActiveTarget(ActiveProxy.Get(), ToolsContext->GizmoManager);
	SyncGizmoSettings(Settings);
	ActiveGizmo->SetVisibility(true);
	ConfigureGizmoRenderPriority();
	return true;
}

void UVTBTransformGizmoInteraction::ClearSelection()
{
	if (ActiveGizmo && ActiveGizmo->ActiveTarget)
	{
		ActiveGizmo->ClearActiveTarget();
	}

	SetTransformEditInProgress(false);
	DestroyActiveGizmo();
	ClearSelectedOutline();
	NotifySelectionChanged(SelectedActor.Get(), false);

	SelectedActor = nullptr;
	SelectedComponent = nullptr;
	ActiveProxy = nullptr;
	bHasPendingSettings = false;
	bRotationAxisIsolationActive = false;
	IsolatedRotationAxis = EAxis::None;
	ResetTransformEditState();
}

void UVTBTransformGizmoInteraction::SyncGizmoSettings(const FVTBTransformGizmoSettings& Settings)
{
	if (bTransformEditInProgress)
	{
		PendingSettings = Settings;
		bHasPendingSettings = true;
		return;
	}

	ActiveSettings = Settings;
	ApplyActiveGizmoSettings();
}

void UVTBTransformGizmoInteraction::ApplyActiveGizmoSettings()
{
	if (!ActiveGizmo)
	{
		return;
	}

	ActiveGizmo->CurrentCoordinateSystem = ActiveSettings.CoordinateSystem;
	ActiveGizmo->ActiveGizmoMode = ActiveSettings.TransformMode;
	ActiveGizmo->Tick(0.0f);
	ResetRotationAxisVisibility();
	ConfigureGizmoRenderPriority();
}

void UVTBTransformGizmoInteraction::DestroyActiveGizmo()
{
	if (ToolsContext && ToolsContext->GizmoManager)
	{
		ToolsContext->GizmoManager->DestroyAllGizmosByOwner(this);
	}

	ActiveGizmo = nullptr;
}

void UVTBTransformGizmoInteraction::EnsureTransformGizmo(const FVTBTransformGizmoSettings& Settings)
{
	if (ActiveGizmo || !ToolsContext || !ToolsContext->GizmoManager)
	{
		SyncGizmoSettings(Settings);
		return;
	}

	ActiveGizmo = UE::TransformGizmoUtil::Create3AxisTransformGizmo(
		ToolsContext->GizmoManager,
		this,
		TEXT("VTBTransformGizmo"));

	if (!ActiveGizmo)
	{
		return;
	}

	ActiveGizmo->bUseContextGizmoMode = true;
	ActiveGizmo->bUseContextCoordinateSystem = true;
	ActiveGizmo->bSnapToWorldGrid = true;
	ActiveGizmo->bSnapToWorldRotGrid = true;
	ActiveGizmo->bSnapToScaleGrid = true;
	ActiveGizmo->SetDisallowNegativeScaling(true);
	ActiveGizmo->SetVisibility(false);
	SyncGizmoSettings(Settings);
}

void UVTBTransformGizmoInteraction::ConfigureGizmoRenderPriority() const
{
	if (!IsValid(ActiveGizmo) || !IsValid(ActiveGizmo->GetGizmoActor()))
	{
		return;
	}

	TArray<UPrimitiveComponent*> GizmoPrimitives;
	ActiveGizmo->GetGizmoActor()->GetComponents<UPrimitiveComponent>(GizmoPrimitives, true);

	for (UPrimitiveComponent* Primitive : GizmoPrimitives)
	{
		if (!IsValid(Primitive))
		{
			continue;
		}

		bool bRenderStateChanged = false;
		if (Primitive->DepthPriorityGroup != SDPG_Foreground)
		{
			Primitive->DepthPriorityGroup = SDPG_Foreground;
			bRenderStateChanged = true;
		}

		if (!Primitive->bUseEditorCompositing)
		{
			Primitive->bUseEditorCompositing = true;
			bRenderStateChanged = true;
		}

		if (!Primitive->bRenderCustomDepth)
		{
			Primitive->bRenderCustomDepth = true;
			bRenderStateChanged = true;
		}

		if (Primitive->TranslucencySortPriority < GizmoTranslucentSortPriority)
		{
			Primitive->TranslucencySortPriority = GizmoTranslucentSortPriority;
			bRenderStateChanged = true;
		}

		ApplyAlwaysOnTopGizmoMaterial(Primitive);

		if (bRenderStateChanged && Primitive->IsRegistered())
		{
			Primitive->MarkRenderStateDirty();
		}
	}
}

void UVTBTransformGizmoInteraction::ApplyAlwaysOnTopGizmoMaterial(UPrimitiveComponent* Primitive) const
{
	if (!Primitive || Primitive->ComponentHasTag(VTBAlwaysOnTopGizmoTag))
	{
		return;
	}

	UMaterialInterface* AlwaysOnTopBaseMaterial = LoadObject<UMaterialInterface>(nullptr, AlwaysOnTopGizmoMaterialPath);
	if (!AlwaysOnTopBaseMaterial)
	{
		return;
	}

	const FLinearColor GizmoColor = ResolveGizmoColor(Primitive);
	UMaterialInstanceDynamic* AlwaysOnTopMaterial = UMaterialInstanceDynamic::Create(AlwaysOnTopBaseMaterial, Primitive);
	if (!AlwaysOnTopMaterial)
	{
		return;
	}

	AlwaysOnTopMaterial->SetVectorParameterValue(GizmoColorParameterName, GizmoColor);

	if (UViewAdjustedStaticMeshGizmoComponent* MeshGizmoComponent = Cast<UViewAdjustedStaticMeshGizmoComponent>(Primitive))
	{
		MeshGizmoComponent->SetAllMaterials(AlwaysOnTopMaterial);

		if (UMaterialInstanceDynamic* HoverMaterial = UMaterialInstanceDynamic::Create(AlwaysOnTopBaseMaterial, Primitive))
		{
			HoverMaterial->SetVectorParameterValue(GizmoColorParameterName, FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
			MeshGizmoComponent->SetHoverOverrideMaterial(HoverMaterial);
		}
	}
	else
	{
		for (int32 MaterialIndex = 0; MaterialIndex < Primitive->GetNumMaterials(); ++MaterialIndex)
		{
			Primitive->SetMaterial(MaterialIndex, AlwaysOnTopMaterial);
		}
	}

	Primitive->ComponentTags.Add(VTBAlwaysOnTopGizmoTag);
}

FLinearColor UVTBTransformGizmoInteraction::ResolveGizmoColor(UPrimitiveComponent* Primitive) const
{
	if (Primitive)
	{
		if (UMaterialInterface* ExistingMaterial = Primitive->GetMaterial(0))
		{
			FLinearColor ExistingColor = FLinearColor::White;
			if (ExistingMaterial->GetVectorParameterValue(FMaterialParameterInfo(GizmoColorParameterName), ExistingColor))
			{
				return ExistingColor;
			}
		}
	}

	return FLinearColor::White;
}

USceneComponent* UVTBTransformGizmoInteraction::ResolveTransformComponent(AActor* TargetActor, bool bRequireTransformTargetInterface) const
{
	if (!TargetActor)
	{
		return nullptr;
	}

	const bool bImplementsTransformTarget = TargetActor->GetClass()->ImplementsInterface(UVTBTransformTarget::StaticClass());
	if (!bImplementsTransformTarget)
	{
		return bRequireTransformTargetInterface ? nullptr : TargetActor->GetRootComponent();
	}

	if (!IVTBTransformTarget::Execute_CanTransform(TargetActor))
	{
		return nullptr;
	}

	if (USceneComponent* InterfaceComponent = IVTBTransformTarget::Execute_GetTransformComponent(TargetActor))
	{
		return InterfaceComponent;
	}

	return TargetActor->GetRootComponent();
}

void UVTBTransformGizmoInteraction::NotifySelectionChanged(AActor* TargetActor, bool bSelected) const
{
	if (TargetActor && TargetActor->GetClass()->ImplementsInterface(UVTBTransformTarget::StaticClass()))
	{
		IVTBTransformTarget::Execute_NotifySelectionChanged(TargetActor, bSelected);
	}
}

void UVTBTransformGizmoInteraction::ApplySelectedOutline(UPrimitiveComponent* PrimitiveComponent)
{
	ClearSelectedOutline();

	if (!PrimitiveComponent)
	{
		return;
	}

	SelectedOutlineComponent = PrimitiveComponent;
	bPreviousRenderCustomDepth = PrimitiveComponent->bRenderCustomDepth;
	PreviousCustomDepthStencilValue = PrimitiveComponent->CustomDepthStencilValue;

	PrimitiveComponent->SetRenderCustomDepth(true);
	PrimitiveComponent->SetCustomDepthStencilValue(SelectedOutlineStencilValue);
}

void UVTBTransformGizmoInteraction::ClearSelectedOutline()
{
	if (UPrimitiveComponent* PrimitiveComponent = SelectedOutlineComponent.Get())
	{
		PrimitiveComponent->SetRenderCustomDepth(bPreviousRenderCustomDepth);
		PrimitiveComponent->SetCustomDepthStencilValue(PreviousCustomDepthStencilValue);
	}

	SelectedOutlineComponent.Reset();
	bPreviousRenderCustomDepth = false;
	PreviousCustomDepthStencilValue = 0;
}

void UVTBTransformGizmoInteraction::HandleBeginTransformEdit(UTransformProxy* Proxy)
{
	if (Proxy == ActiveProxy.Get())
	{
		TransformEditState.InitialTransform = Proxy->GetTransform();
		TransformEditState.LastDisplayedRotationDeltaDegrees = 0.0;
		TransformEditState.bHasInitialTransform = true;
		SetTransformEditInProgress(true);

		const EAxis::Type HoveredRotationAxis = GetHoveredRotationAxis();
		if (HoveredRotationAxis != EAxis::None)
		{
			SetRotationAxisIsolation(HoveredRotationAxis);
		}
	}
}

void UVTBTransformGizmoInteraction::HandleEndTransformEdit(UTransformProxy* Proxy)
{
	if (Proxy == ActiveProxy.Get())
	{
		UpdateRotationDeltaDisplay(Proxy->GetTransform(), true);
		SetTransformEditInProgress(false);
		ResetRotationAxisVisibility();
		ResetTransformEditState();

		if (bHasPendingSettings)
		{
			const FVTBTransformGizmoSettings SettingsToApply = PendingSettings;
			bHasPendingSettings = false;
			SyncGizmoSettings(SettingsToApply);
		}
	}
}

void UVTBTransformGizmoInteraction::HandleProxyTransformChanged(UTransformProxy* Proxy, FTransform NewTransform)
{
	if (!SelectedActor)
	{
		return;
	}

	const FVector Location = SelectedActor->GetActorLocation();
	const FRotator Rotation = SelectedActor->GetActorRotation();
	const FVector Scale = SelectedActor->GetActorScale3D();
	UE_LOG(LogTemp, Verbose, TEXT("VTB gizmo changed %s | Location=%s Rotation=%s Scale=%s"),
		*SelectedActor->GetName(),
		*Location.ToCompactString(),
		*Rotation.ToCompactString(),
		*Scale.ToCompactString());

	UpdateRotationAxisIsolation(NewTransform);
	UpdateRotationDeltaDisplay(NewTransform, false);
}

void UVTBTransformGizmoInteraction::SetTransformEditInProgress(bool bInProgress)
{
	if (bTransformEditInProgress == bInProgress)
	{
		return;
	}

	bTransformEditInProgress = bInProgress;
	TransformEditStateChangedCallback(bTransformEditInProgress);
}

void UVTBTransformGizmoInteraction::ResetTransformEditState()
{
	TransformEditState = FTransformEditState();
}

void UVTBTransformGizmoInteraction::UpdateRotationDeltaDisplay(const FTransform& NewTransform, bool bFinalMessage)
{
	if (!ActiveSettings.Feedback.bShowRotationDelta ||
		ActiveSettings.TransformMode != EToolContextTransformGizmoMode::Rotation ||
		!TransformEditState.bHasInitialTransform)
	{
		return;
	}

	const FQuat InitialRotation = TransformEditState.InitialTransform.GetRotation().GetNormalized();
	const FQuat CurrentRotation = NewTransform.GetRotation().GetNormalized();
	FQuat DeltaRotation = CurrentRotation * InitialRotation.Inverse();
	DeltaRotation.Normalize();

	double AngleRadians = DeltaRotation.GetAngle();
	if (AngleRadians > PI)
	{
		AngleRadians = (2.0 * PI) - AngleRadians;
	}

	const double AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
	if (!bFinalMessage && FMath::Abs(AngleDegrees - TransformEditState.LastDisplayedRotationDeltaDegrees) < 0.1)
	{
		return;
	}

	TransformEditState.LastDisplayedRotationDeltaDegrees = AngleDegrees;

	if (GEngine)
	{
		const uint64 MessageKey = reinterpret_cast<uint64>(this) + RotationDeltaMessageKeyOffset;
		const float DisplayTime = bFinalMessage ? 1.25f : 0.05f;
		GEngine->AddOnScreenDebugMessage(
			MessageKey,
			DisplayTime,
			FColor::Yellow,
			FString::Printf(TEXT("Rotation: %.1f deg"), AngleDegrees));
	}
}

void UVTBTransformGizmoInteraction::UpdateRotationAxisIsolation(const FTransform& NewTransform)
{
	if (bRotationAxisIsolationActive ||
		ActiveSettings.TransformMode != EToolContextTransformGizmoMode::Rotation)
	{
		return;
	}

	EAxis::Type RotationAxis = EAxis::None;
	if (TryDetectRotationAxis(NewTransform, RotationAxis))
	{
		SetRotationAxisIsolation(RotationAxis);
	}
}

bool UVTBTransformGizmoInteraction::TryDetectRotationAxis(const FTransform& NewTransform, EAxis::Type& AxisOut) const
{
	AxisOut = EAxis::None;

	if (!TransformEditState.bHasInitialTransform)
	{
		return false;
	}

	const FQuat InitialRotation = TransformEditState.InitialTransform.GetRotation().GetNormalized();
	const FQuat CurrentRotation = NewTransform.GetRotation().GetNormalized();
	FQuat DeltaRotation = CurrentRotation * InitialRotation.Inverse();
	DeltaRotation.Normalize();

	double AngleRadians = DeltaRotation.GetAngle();
	if (AngleRadians > PI)
	{
		AngleRadians = (2.0 * PI) - AngleRadians;
	}

	if (FMath::RadiansToDegrees(AngleRadians) < RotationAxisIsolationMinAngleDegrees)
	{
		return false;
	}

	const FVector RotationAxis = DeltaRotation.GetRotationAxis().GetSafeNormal();
	if (RotationAxis.IsNearlyZero())
	{
		return false;
	}

	const bool bUseLocalAxes = ActiveSettings.CoordinateSystem == EToolContextCoordinateSystem::Local;
	const FVector CandidateAxes[] =
	{
		bUseLocalAxes ? InitialRotation.RotateVector(FVector::XAxisVector) : FVector::XAxisVector,
		bUseLocalAxes ? InitialRotation.RotateVector(FVector::YAxisVector) : FVector::YAxisVector,
		bUseLocalAxes ? InitialRotation.RotateVector(FVector::ZAxisVector) : FVector::ZAxisVector
	};

	double BestDot = 0.0;
	int32 BestAxisIndex = INDEX_NONE;
	for (int32 AxisIndex = 0; AxisIndex < UE_ARRAY_COUNT(CandidateAxes); ++AxisIndex)
	{
		const double AxisDot = FMath::Abs(FVector::DotProduct(RotationAxis, CandidateAxes[AxisIndex].GetSafeNormal()));
		if (AxisDot > BestDot)
		{
			BestDot = AxisDot;
			BestAxisIndex = AxisIndex;
		}
	}

	if (BestAxisIndex == INDEX_NONE || BestDot < RotationAxisIsolationDotThreshold)
	{
		return false;
	}

	switch (BestAxisIndex)
	{
	case 0:
		AxisOut = EAxis::X;
		return true;
	case 1:
		AxisOut = EAxis::Y;
		return true;
	case 2:
		AxisOut = EAxis::Z;
		return true;
	default:
		return false;
	}
}

EAxis::Type UVTBTransformGizmoInteraction::GetHoveredRotationAxis() const
{
	if (ActiveSettings.TransformMode != EToolContextTransformGizmoMode::Rotation ||
		!IsValid(ActiveGizmo) ||
		!IsValid(ActiveGizmo->GetGizmoActor()))
	{
		return EAxis::None;
	}

	ACombinedTransformGizmoActor* GizmoActor = ActiveGizmo->GetGizmoActor();
	if (IsGizmoComponentHovered(GizmoActor->RotateX))
	{
		return EAxis::X;
	}
	if (IsGizmoComponentHovered(GizmoActor->RotateY))
	{
		return EAxis::Y;
	}
	if (IsGizmoComponentHovered(GizmoActor->RotateZ))
	{
		return EAxis::Z;
	}

	return EAxis::None;
}

void UVTBTransformGizmoInteraction::SetRotationAxisIsolation(EAxis::Type VisibleAxis)
{
	if (!IsValid(ActiveGizmo) ||
		!IsValid(ActiveGizmo->GetGizmoActor()) ||
		ActiveSettings.TransformMode != EToolContextTransformGizmoMode::Rotation)
	{
		return;
	}

	ACombinedTransformGizmoActor* GizmoActor = ActiveGizmo->GetGizmoActor();
	SetComponentVisibility(GizmoActor->RotateX, VisibleAxis == EAxis::X);
	SetComponentVisibility(GizmoActor->RotateY, VisibleAxis == EAxis::Y);
	SetComponentVisibility(GizmoActor->RotateZ, VisibleAxis == EAxis::Z);
	SetComponentVisibility(GizmoActor->RotationSphere, false);
	SetComponentVisibility(GizmoActor->FreeRotateHandle, false);

	bRotationAxisIsolationActive = true;
	IsolatedRotationAxis = VisibleAxis;
}

void UVTBTransformGizmoInteraction::ResetRotationAxisVisibility()
{
	if (!IsValid(ActiveGizmo) || !IsValid(ActiveGizmo->GetGizmoActor()))
	{
		bRotationAxisIsolationActive = false;
		IsolatedRotationAxis = EAxis::None;
		return;
	}

	ACombinedTransformGizmoActor* GizmoActor = ActiveGizmo->GetGizmoActor();
	const bool bShowRotationAxes = ActiveSettings.TransformMode == EToolContextTransformGizmoMode::Rotation ||
		ActiveSettings.TransformMode == EToolContextTransformGizmoMode::Combined;
	SetComponentVisibility(GizmoActor->RotateX, bShowRotationAxes);
	SetComponentVisibility(GizmoActor->RotateY, bShowRotationAxes);
	SetComponentVisibility(GizmoActor->RotateZ, bShowRotationAxes);
	SetComponentVisibility(GizmoActor->RotationSphere, bShowRotationAxes);
	SetComponentVisibility(GizmoActor->FreeRotateHandle, ActiveSettings.TransformMode == EToolContextTransformGizmoMode::Rotation);

	bRotationAxisIsolationActive = false;
	IsolatedRotationAxis = EAxis::None;
}
