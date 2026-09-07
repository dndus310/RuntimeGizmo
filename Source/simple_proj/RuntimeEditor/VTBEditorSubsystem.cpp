#include "VTBEditorSubsystem.h"

#include "Context/VTBEditorInteractiveToolsContext.h"
#include "Gizmo/VTBEditorTransformGizmo.h"
#include "Selection/VTBEditorTargetSelection.h"
#include "VTBEditorGameMode.h"

#include "BaseGizmos/AxisAngleGizmo.h"
#include "BaseGizmos/CombinedTransformGizmo.h"
#include "BaseGizmos/GizmoBaseComponent.h"
#include "BaseGizmos/GizmoRenderingUtil.h"
#include "BaseGizmos/HitTargets.h"
#include "BaseGizmos/ViewAdjustedStaticMeshGizmoComponent.h"
#include "Components/MeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "InteractiveGizmoManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "SceneTypes.h"
#include "ToolContextInterfaces.h"

UVTBEditorSubsystem::UVTBEditorSubsystem()
	: TransformGizmoSource(EVTBEditorTransformGizmoSource::DefaultITF)
	, Phase(EVTBEditorSubsystemPhase::Ready)
{
}

bool UVTBEditorSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}

	const UWorld* World = Cast<UWorld>(Outer);
	if (!World)
	{
		return false;
	}

	return World->GetNetMode() != NM_DedicatedServer;
}

void UVTBEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ToolsContext = NewObject<UVTBEditorInteractiveToolsContext>(this);
	if (!ToolsContext->InitializeRuntime(GetWorld()))
	{
		ToolsContext = nullptr;
		return;
	}

	TargetAdapter = NewObject<UVTBEditorTargetSelection>(this);
	EnsureCustomGizmoBuilder();
	ToolsContext->SetGizmoMode(EToolContextTransformGizmoMode::Translation);
}

void UVTBEditorSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	if (SelectionSource.IsValid())
	{
		return;
	}

	AGameModeBase* GameMode = InWorld.GetAuthGameMode();
	if (!GameMode)
	{
		return;
	}

	BindSelectionSource(GameMode);
}

void UVTBEditorSubsystem::Deinitialize()
{
	ExitRuntime();
	Super::Deinitialize();
}

void UVTBEditorSubsystem::ExitRuntime()
{
	if (Phase == EVTBEditorSubsystemPhase::Exiting)
	{
		return;
	}
	Phase = EVTBEditorSubsystemPhase::Exiting;
	if (IVTBSelectionSource* Selection = Cast<IVTBSelectionSource>(SelectionSource.Get()))
	{
		Selection->OnSelectionChanged().Remove(SelectionChangedHandle);
	}
	SelectionChangedHandle.Reset();
	SelectionSource.Reset();
	if (ToolsContext)
	{
		ToolsContext->CancelActiveInteraction();
		ToolsContext->GizmoManager->DestroyAllGizmosByOwner(this);
		ToolsContext->Shutdown();
	}
	TransformGizmo = nullptr;
	GizmoBuilder = nullptr;
	TargetAdapter = nullptr;
	ToolsContext = nullptr;
	PendingSelection.Reset();
}

void UVTBEditorSubsystem::SetTransformGizmoSource(EVTBEditorTransformGizmoSource Source)
{
	if (TransformGizmoSource == Source)
	{
		return;
	}

	TransformGizmoSource = Source;
	EnsureCustomGizmoBuilder();
	if (!ToolsContext)
	{
		return;
	}

	ToolsContext->CancelActiveInteraction();
	ToolsContext->GizmoManager->DestroyAllGizmosByOwner(this);
	TransformGizmo = nullptr;
	if (TargetAdapter && Phase == EVTBEditorSubsystemPhase::Ready)
	{
		RefreshGizmoTarget();
	}
}

bool UVTBEditorSubsystem::BindSelectionSource(UObject* Source)
{
	if (!ToolsContext)
	{
		return false;
	}
	if (Phase == EVTBEditorSubsystemPhase::Exiting)
	{
		return false;
	}

	IVTBSelectionSource* Selection = IsValid(Source) ? Cast<IVTBSelectionSource>(Source) : nullptr;
	if (Source)
	{
		if (!Selection)
		{
			return false;
		}
	}

	if (IVTBSelectionSource* Previous = Cast<IVTBSelectionSource>(SelectionSource.Get()))
	{
		Previous->OnSelectionChanged().Remove(SelectionChangedHandle);
	}
	SelectionChangedHandle.Reset();
	SelectionSource = Source;
	if (Selection)
	{
		SelectionChangedHandle = Selection->OnSelectionChanged().AddUObject(this, &ThisClass::OnSelectionChanged);
	}
	OnSelectionChanged();
	return true;
}

void UVTBEditorSubsystem::OnSelectionChanged()
{
	TArray<TWeakObjectPtr<AActor>> Actors;
	if (IVTBSelectionSource* Selection = Cast<IVTBSelectionSource>(SelectionSource.Get()))
	{
		Selection->GetSelectionSnapshot(Actors);
	}
	ReceiveSelection(Actors);
}

void UVTBEditorSubsystem::ReceiveSelection(const TArray<TWeakObjectPtr<AActor>>& Actors)
{
	if (!ToolsContext)
	{
		return;
	}
	if (Phase == EVTBEditorSubsystemPhase::Exiting)
	{
		return;
	}

	PendingSelection = Actors;
	RefreshRuntime();
}

bool UVTBEditorSubsystem::IsInteractionLocked() const
{
	if (Phase == EVTBEditorSubsystemPhase::Refreshing)
	{
		return true;
	}
	if (!ToolsContext)
	{
		return false;
	}
	return ToolsContext->IsCancellingInteraction();
}

void UVTBEditorSubsystem::RefreshRuntime()
{
	if (!ToolsContext)
	{
		return;
	}
	if (!TargetAdapter)
	{
		return;
	}
	if (TransformGizmoSource == EVTBEditorTransformGizmoSource::CustomVTB && !GizmoBuilder)
	{
		return;
	}
	if (Phase != EVTBEditorSubsystemPhase::Ready)
	{
		return;
	}

	if (ToolsContext->IsCancellingInteraction())
	{
		return;
	}

	TGuardValue<EVTBEditorSubsystemPhase> RefreshScope(Phase, EVTBEditorSubsystemPhase::Refreshing);
	TOptional<TArray<TWeakObjectPtr<AActor>>> Selection = MoveTemp(PendingSelection);
	PendingSelection.Reset();

	const bool bHasSelectionRequest = Selection.IsSet();
	const bool bTargetsChanged = bHasSelectionRequest ? false : TargetAdapter->RefreshSelection();
	if (!bHasSelectionRequest)
	{
		if (!bTargetsChanged)
		{
			return;
		}
	}

	// Consume requests before cancellation. Requests from its callbacks remain for the next tick.
	ToolsContext->CancelActiveInteraction();
	if (bHasSelectionRequest)
	{
		TargetAdapter->SetSelection(GetWorld(), Selection.GetValue());
	}
	else if (bTargetsChanged)
	{
		TargetAdapter->RebuildFromCurrentTransforms(); // Use the transforms restored by cancellation.
	}
	RefreshGizmoTarget();
}

void UVTBEditorSubsystem::RefreshGizmoTarget()
{
	TArray<AActor*> Actors;
	TargetAdapter->GetSelectedActors(Actors);
	ToolsContext->SetSelection(Actors);
	UTransformProxy* Proxy = TargetAdapter->GetTransformProxy();
	bool bHasTarget = Proxy != nullptr;
	EToolContextTransformGizmoMode GizmoMode = EToolContextTransformGizmoMode::Translation;
	IToolsContextQueriesAPI* Queries = ToolsContext->GizmoManager->GetContextQueriesAPI();
	if (Queries)
	{
		GizmoMode = Queries->GetCurrentTransformGizmoMode();
	}
	if (GizmoMode == EToolContextTransformGizmoMode::NoGizmo)
	{
		bHasTarget = false;
	}

	const bool bUseCustomGizmo = TransformGizmoSource == EVTBEditorTransformGizmoSource::CustomVTB;
	bool bRebuildGizmo = TransformGizmo ? TransformGizmo->IsA<UVTBEditorTransformGizmo>() != bUseCustomGizmo : false;
	if (bUseCustomGizmo)
	{
		EnsureCustomGizmoBuilder();
		if (!GizmoBuilder)
		{
			return;
		}

		ETransformGizmoSubElements Elements = ETransformGizmoSubElements::TranslateAllAxes
			| ETransformGizmoSubElements::TranslateAllPlanes
			| ETransformGizmoSubElements::RotateAllAxes
			| ETransformGizmoSubElements::ScaleUniform;
		if (Actors.Num() == 1)
		{
			Elements |= ETransformGizmoSubElements::ScaleAllAxes;
			Elements |= ETransformGizmoSubElements::ScaleAllPlanes;
		}

		bRebuildGizmo |= GizmoBuilder->EnabledElements != Elements;
		if (bHasTarget)
		{
			GizmoBuilder->EnabledElements = Elements;
		}
	}

	if (TransformGizmo)
	{
		ACombinedTransformGizmoActor* GizmoActor = TransformGizmo->GetGizmoActor();
		if (!bHasTarget)
		{
			ToolsContext->GizmoManager->DestroyAllGizmosByOwner(this);
			TransformGizmo = nullptr;
		}
		else if (!IsValid(GizmoActor))
		{
			ToolsContext->GizmoManager->DestroyAllGizmosByOwner(this);
			TransformGizmo = nullptr;
		}
		else if (bRebuildGizmo)
		{
			ToolsContext->GizmoManager->DestroyAllGizmosByOwner(this);
			TransformGizmo = nullptr;
		}
	}

	if (!bHasTarget)
	{
		return;
	}
	if (!TransformGizmo)
	{
		TransformGizmo = bUseCustomGizmo
			? Cast<UCombinedTransformGizmo>(ToolsContext->GizmoManager->CreateGizmo(
				UVTBEditorTransformGizmoBuilder::BuilderIdentifier, TEXT("VTB.RuntimeTransform"), this))
			: ToolsContext->GizmoManager->Create3AxisTransformGizmo(this, TEXT("VTB.RuntimeTransform"));
		if (TransformGizmo)
		{
			TransformGizmo->SetIsNonUniformScaleAllowedFunction([]()
			{
				return true;
			});
			ApplyGizmoRendering();
			TransformGizmo->SetVisibility(false);
		}
	}
	if (TransformGizmo)
	{
		if (TransformGizmo->ActiveTarget != Proxy)
		{
			TransformGizmo->SetActiveTarget(Proxy, ToolsContext->GizmoManager);
		}
	}
}

void UVTBEditorSubsystem::UpdateGizmoVisibility(bool bHasView)
{
	if (!TransformGizmo)
	{
		return;
	}

	TransformGizmo->SetVisibility(bHasView);
	if (!bHasView)
	{
		return;
	}

	ACombinedTransformGizmoActor* GizmoActor = TransformGizmo->GetGizmoActor();
	IToolsContextQueriesAPI* Queries = ToolsContext && ToolsContext->GizmoManager
		? ToolsContext->GizmoManager->GetContextQueriesAPI() : nullptr;
	if (!IsValid(GizmoActor) || !Queries)
	{
		return;
	}

	const EToolContextTransformGizmoMode GizmoMode = Queries->GetCurrentTransformGizmoMode();
	const EToolContextCoordinateSystem CoordinateSystem = Queries->GetCurrentCoordinateSystem();
	static const FName FullScaleAxisMeshName(TEXT("GizmoBoxArrowHandle"));
	TArray<UViewAdjustedStaticMeshGizmoComponent*> ViewAdjustedComponents;
	GizmoActor->GetComponents(ViewAdjustedComponents);
	for (UViewAdjustedStaticMeshGizmoComponent* Component : ViewAdjustedComponents)
	{
		const UStaticMesh* Mesh = Component ? Component->GetStaticMesh() : nullptr;
		if (Mesh && Mesh->GetFName() == FullScaleAxisMeshName)
		{
			Component->UpdateWorldLocalState(CoordinateSystem == EToolContextCoordinateSystem::World);
		}
	}

	const bool bRotationMode = GizmoMode == EToolContextTransformGizmoMode::Rotation
		|| GizmoMode == EToolContextTransformGizmoMode::Combined;
	if (!bRotationMode)
	{
		return;
	}

	UPrimitiveComponent* ActiveRotationComponent = nullptr;
	const TArray<UInteractiveGizmo*> RotationGizmos = ToolsContext->GizmoManager->FindAllGizmosOfType(
		UInteractiveGizmoManager::DefaultAxisAngleBuilderIdentifier);
	for (UInteractiveGizmo* ChildGizmo : RotationGizmos)
	{
		const UAxisAngleGizmo* RotationGizmo = Cast<UAxisAngleGizmo>(ChildGizmo);
		if (!RotationGizmo || !RotationGizmo->bInInteraction)
		{
			continue;
		}

		const UGizmoComponentHitTarget* HitTarget = Cast<UGizmoComponentHitTarget>(RotationGizmo->HitTarget.GetObject());
		UPrimitiveComponent* Component = HitTarget ? HitTarget->Component.Get() : nullptr;
		if (Component == GizmoActor->RotateX || Component == GizmoActor->RotateY || Component == GizmoActor->RotateZ)
		{
			ActiveRotationComponent = Component;
			break;
		}
	}

	UPrimitiveComponent* RotationComponents[] = { GizmoActor->RotateX.Get(), GizmoActor->RotateY.Get(), GizmoActor->RotateZ.Get() };
	for (UPrimitiveComponent* Component : RotationComponents)
	{
		if (Component)
		{
			Component->SetVisibility(!ActiveRotationComponent || Component == ActiveRotationComponent);
		}
	}
	if (GizmoActor->RotationSphere)
	{
		GizmoActor->RotationSphere->SetVisibility(!ActiveRotationComponent);
	}
}

void UVTBEditorSubsystem::ApplyGizmoRendering()
{
	ACombinedTransformGizmoActor* GizmoActor = TransformGizmo ? TransformGizmo->GetGizmoActor() : nullptr;
	if (!IsValid(GizmoActor))
	{
		return;
	}

	UMaterialInterface* Material = LoadObject<UMaterialInterface>(
		nullptr, TEXT("/Engine/InteractiveToolsFramework/Materials/GizmoComponentMaterial_NotDimmed"));
	TSet<UPrimitiveComponent*> StyledComponents;
	const FLinearColor HoverColor = FLinearColor::White;
	const auto MaterialInstance = [Material](UObject* Outer, const FLinearColor& Color)
	{
		if (!Material)
		{
			return static_cast<UMaterialInstanceDynamic*>(nullptr);
		}

		UMaterialInstanceDynamic* Instance = UMaterialInstanceDynamic::Create(Material, Outer);
		if (Instance)
		{
			Instance->SetVectorParameterValue(TEXT("GizmoColor"), Color);
			Instance->SetScalarParameterValue(TEXT("OccludeByCustomDepth"), 0.0f);
		}
		return Instance;
	};
	const auto ReadGizmoColor = [](UMeshComponent* MeshComponent, FLinearColor& Color)
	{
		if (!MeshComponent)
		{
			return false;
		}

		const FHashedMaterialParameterInfo GizmoColorParameter(TEXT("GizmoColor"));
		for (int32 Index = 0; Index < MeshComponent->GetNumMaterials(); ++Index)
		{
			UMaterialInterface* ExistingMaterial = MeshComponent->GetMaterial(Index);
			if (ExistingMaterial && ExistingMaterial->GetVectorParameterValue(GizmoColorParameter, Color, false))
			{
				return true;
			}
		}
		return false;
	};
	TFunction<void(UPrimitiveComponent*, const FLinearColor&, const FLinearColor&)> ApplyComponent;
	ApplyComponent = [MaterialInstance, &ApplyComponent, &StyledComponents](UPrimitiveComponent* Component, const FLinearColor& Color, const FLinearColor& Hover)
	{
		if (!Component)
		{
			return;
		}
		if (StyledComponents.Contains(Component))
		{
			return;
		}
		StyledComponents.Add(Component);

		Component->SetRenderCustomDepth(false);
		Component->DepthPriorityGroup = SDPG_Foreground;
		Component->TranslucencySortPriority = UE::GizmoRenderingUtil::GIZMO_TRANSLUCENCY_SORT_PRIORITY;
		if (UGizmoBaseComponent* GizmoComponent = Cast<UGizmoBaseComponent>(Component))
		{
			GizmoComponent->Color = Color;
			GizmoComponent->NotifyExternalPropertyUpdates();
		}

		UMaterialInstanceDynamic* Instance = MaterialInstance(Component, Color);
		UMaterialInstanceDynamic* HoverInstance = MaterialInstance(Component, Hover);
		if (UViewAdjustedStaticMeshGizmoComponent* ViewMeshComponent = Cast<UViewAdjustedStaticMeshGizmoComponent>(Component))
		{
			if (Instance)
			{
				ViewMeshComponent->SetAllMaterials(Instance);
			}
			if (HoverInstance)
			{
				ViewMeshComponent->SetHoverOverrideMaterial(HoverInstance);
			}
		}
		else if (UMeshComponent* StandardMeshComponent = Cast<UMeshComponent>(Component))
		{
			if (Instance)
			{
				const int32 MaterialCount = FMath::Max(1, StandardMeshComponent->GetNumMaterials());
				for (int32 Index = 0; Index < MaterialCount; ++Index)
				{
					StandardMeshComponent->SetMaterial(Index, Instance);
				}
			}
		}

		if (USceneComponent* SceneComponent = Cast<USceneComponent>(Component))
		{
			for (USceneComponent* Child : SceneComponent->GetAttachChildren())
			{
				ApplyComponent(Cast<UPrimitiveComponent>(Child), Color, Hover);
			}
		}
	};

	const FLinearColor XColor = UE::GizmoRenderingUtil::GetDefaultAxisColor(EAxis::X);
	const FLinearColor YColor = UE::GizmoRenderingUtil::GetDefaultAxisColor(EAxis::Y);
	const FLinearColor ZColor = UE::GizmoRenderingUtil::GetDefaultAxisColor(EAxis::Z);
	const FLinearColor CenterColor(0.7f, 0.7f, 0.7f, 1.0f);
	const FLinearColor FreeRotateColor(0.5f, 0.5f, 0.5f, 0.15f);

	FLinearColor XRotateColor = XColor;
	FLinearColor YRotateColor = YColor;
	FLinearColor ZRotateColor = ZColor;
	XRotateColor.A = 0.75f;
	YRotateColor.A = 0.75f;
	ZRotateColor.A = 0.75f;

	ApplyComponent(GizmoActor->TranslateX, XColor, HoverColor);
	ApplyComponent(GizmoActor->RotateX, XRotateColor, HoverColor);
	ApplyComponent(GizmoActor->AxisScaleX, XColor, HoverColor);
	ApplyComponent(GizmoActor->TranslateYZ, XColor, HoverColor);
	ApplyComponent(GizmoActor->PlaneScaleYZ, XColor, HoverColor);
	ApplyComponent(GizmoActor->TranslateY, YColor, HoverColor);
	ApplyComponent(GizmoActor->RotateY, YRotateColor, HoverColor);
	ApplyComponent(GizmoActor->AxisScaleY, YColor, HoverColor);
	ApplyComponent(GizmoActor->TranslateXZ, YColor, HoverColor);
	ApplyComponent(GizmoActor->PlaneScaleXZ, YColor, HoverColor);
	ApplyComponent(GizmoActor->TranslateZ, ZColor, HoverColor);
	ApplyComponent(GizmoActor->RotateZ, ZRotateColor, HoverColor);
	ApplyComponent(GizmoActor->AxisScaleZ, ZColor, HoverColor);
	ApplyComponent(GizmoActor->TranslateXY, ZColor, HoverColor);
	ApplyComponent(GizmoActor->PlaneScaleXY, ZColor, HoverColor);
	ApplyComponent(GizmoActor->UniformScale, CenterColor, HoverColor);
	ApplyComponent(GizmoActor->RotationSphere, CenterColor, HoverColor);
	ApplyComponent(GizmoActor->FreeRotateHandle, FreeRotateColor, HoverColor);
	ApplyComponent(GizmoActor->FreeTranslateHandle, CenterColor, HoverColor);

	TArray<UPrimitiveComponent*> Components;
	GizmoActor->GetComponents(Components);
	for (UPrimitiveComponent* Component : Components)
	{
		if (!Component)
		{
			continue;
		}

		Component->SetRenderCustomDepth(false);
		if (StyledComponents.Contains(Component))
		{
			continue;
		}

		FLinearColor ExistingColor;
		if (ReadGizmoColor(Cast<UMeshComponent>(Component), ExistingColor))
		{
			ApplyComponent(Component, ExistingColor, HoverColor);
		}
	}
}

void UVTBEditorSubsystem::EnsureCustomGizmoBuilder()
{
	if (TransformGizmoSource != EVTBEditorTransformGizmoSource::CustomVTB || GizmoBuilder || !ToolsContext)
	{
		return;
	}

	GizmoBuilder = NewObject<UVTBEditorTransformGizmoBuilder>(ToolsContext->GizmoManager);
	ToolsContext->GizmoManager->RegisterGizmoType(UVTBEditorTransformGizmoBuilder::BuilderIdentifier, GizmoBuilder);
}

bool UVTBEditorSubsystem::UpdateView()
{
	APlayerController* Controller = nullptr;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* Candidate = It->Get();
		if (!Candidate)
		{
			continue;
		}
		if (!Candidate->IsLocalController())
		{
			continue;
		}
		Controller = Candidate;
		break;
	}
	return ToolsContext->UpdateView(Controller);
}

void UVTBEditorSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!ToolsContext)
	{
		return;
	}
	if (IsInteractionLocked())
	{
		return;
	}
	if (SelectionChangedHandle.IsValid())
	{
		if (!SelectionSource.IsValid())
		{
			BindSelectionSource(nullptr);
		}
	}
	RefreshRuntime();
	const bool bHasView = UpdateView();
	if (!bHasView)
	{
		ToolsContext->CancelActiveInteraction();
	}
	TGuardValue<EVTBEditorSubsystemPhase> RefreshScope(Phase, EVTBEditorSubsystemPhase::Refreshing);
	ToolsContext->TickRuntime(DeltaTime);
	UpdateGizmoVisibility(bHasView);
}

TStatId UVTBEditorSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UVTBEditorSubsystem, STATGROUP_Tickables);
}
