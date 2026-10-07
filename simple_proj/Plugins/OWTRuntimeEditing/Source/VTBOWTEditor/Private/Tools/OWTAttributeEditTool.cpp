#include "Tools/OWTAttributeEditTool.h"
#include "BaseBehaviors/SingleClickBehavior.h"
#include "BaseGizmos/TransformProxy.h"
#include "Components/SceneComponent.h"
#include "Context/OWTAttributeEditSessionContext.h"
#include "Context/VTBOWTEditorToolsContext.h"
#include "ContextObjectStore.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Gizmos/Base/VTBOWTBaseTransformGizmo.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolManager.h"
#include "Modes/OWTAttributeEditMode.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorSubsystem.h"

namespace
{
UOWTAttributeEditMode* ResolveAttributeMode(const FToolBuilderState& State)
{
	if (!State.ToolManager)
	{
		return nullptr;
	}
	UOWTAttributeEditSessionContext* Session =
	    State.ToolManager->GetContextObjectStore()->FindContext<UOWTAttributeEditSessionContext>();
	return Session ? Session->GetMode() : nullptr;
}

AVTBAttributeEditor* FindEditor(UOWTAttributeEditMode* Mode)
{
	if (!Mode)
	{
		return nullptr;
	}
	UVTBOWTEditorSubsystem* Subsystem = Mode->GetSubsystem();
	return Subsystem ? Subsystem->GetAttributeEditor() : nullptr;
}
} // namespace

UOWTAttributeEditTool::UOWTAttributeEditTool()
    : Mode(), TransformGizmo(nullptr), Properties(nullptr), GizmoTarget(), bHidden(false)
{
}

void UOWTAttributeEditTool::Initialize(UOWTAttributeEditMode& InMode)
{
	Mode = &InMode;
}

void UOWTAttributeEditTool::Setup()
{
	Super::Setup();
	Properties = NewObject<UOWTTransformProperties>(this);
	AddToolPropertySource(Properties);
	InitializeSelectionBehavior();

	ShowGizmo();
	ObserveTransform();
}

void UOWTAttributeEditTool::InitializeSelectionBehavior()
{
	USingleClickInputBehavior* ClickBehavior = NewObject<USingleClickInputBehavior>(this);
	ClickBehavior->Initialize(this);
	// Gizmo captures take precedence over the broad actor selection ray.
	ClickBehavior->SetDefaultPriority(FInputCapturePriority(150));
	AddInputBehavior(ClickBehavior);
}

void UOWTAttributeEditTool::Shutdown(EToolShutdownType ShutdownType)
{
	HideGizmo();
	Properties = nullptr;
	Super::Shutdown(ShutdownType);
}

void UOWTAttributeEditTool::OnTick(float DeltaTime)
{
	if (!Mode.IsValid())
	{
		return;
	}

	AActor* Actor = Mode->GetSelectedObject();
	USceneComponent* RootComponent = Actor ? Actor->GetRootComponent() : nullptr;
	if (GizmoTarget.Get() != RootComponent)
	{
		const bool bWasHidden = bHidden;
		HideGizmo();
		if (!bWasHidden)
		{
			ShowGizmo();
		}
	}

	ObserveTransform();
}

void UOWTAttributeEditTool::ObserveTransform()
{
	if (!Properties)
	{
		return;
	}
	if (!Mode.IsValid())
	{
		return;
	}
	AActor* Actor = Mode->GetSelectedObject();
	if (!IsValid(Actor))
	{
		return;
	}
	const FTransform Transform = Actor->GetActorTransform();
	Properties->Location = Transform.GetLocation();
	Properties->Rotation = Transform.Rotator();
	Properties->Scale = Transform.GetScale3D();
}

void UOWTAttributeEditTool::OnPropertyModified(UObject* PropertySet, FProperty* Property)
{
	if (PropertySet != Properties)
	{
		return;
	}
	if (!Mode.IsValid())
	{
		return;
	}
	AActor* Actor = Mode->GetSelectedObject();
	if (!IsValid(Actor))
	{
		return;
	}
	FString Error;
	AVTBAttributeEditor* Editor = FindEditor(Mode.Get());
	if (Editor)
	{
		Editor->BeginGizmoEdit();
	}
	ApplyTransform(*Actor, FTransform(Properties->Rotation, Properties->Location, Properties->Scale), Error);
	Editor = FindEditor(Mode.Get());
	if (Editor)
	{
		Editor->UpdateGizmoEdit();
		Editor->EndGizmoEdit();
	}
}

bool UOWTAttributeEditTool::ApplyTransform(AActor& Actor, const FTransform& Transform, FString& OutError)
{
	if (!Mode.IsValid())
	{
		OutError = TEXT("The editing mode has ended.");
		return false;
	}
	if (Mode->GetSelectedObject() != &Actor)
	{
		OutError = TEXT("The selected actor changed.");
		return false;
	}
	if (Actor.IsActorBeingDestroyed())
	{
		OutError = TEXT("The actor is being destroyed.");
		return false;
	}
	if (Transform.ContainsNaN())
	{
		OutError = TEXT("The transform must be finite.");
		return false;
	}
	USceneComponent* Root = Actor.GetRootComponent();
	if (!IsValid(Root))
	{
		OutError = TEXT("The actor has no transform component.");
		return false;
	}
	if (Root->Mobility != EComponentMobility::Movable)
	{
		OutError = TEXT("The root component must be movable.");
		return false;
	}
	Actor.SetActorTransform(Transform, false, nullptr, ETeleportType::TeleportPhysics);
	if (!Mode.IsValid())
	{
		return false;
	}
	if (Actor.IsActorBeingDestroyed())
	{
		return false;
	}
	if (Mode->GetSelectedObject() != &Actor)
	{
		return false;
	}
	SynchronizeGizmo();
	ObserveTransform();
	return true;
}

bool UOWTAttributeEditTool::SelectActor(AActor* Actor)
{
	if (!Mode.IsValid())
	{
		return false;
	}
	HideGizmo();
	bHidden = false;
	Mode->CommitSelection(Actor);
	if (!Mode.IsValid())
	{
		return false;
	}
	ShowGizmo();
	ObserveTransform();
	return true;
}

FInputRayHit UOWTAttributeEditTool::IsHitByClick(const FInputDeviceRay& ClickPos)
{
	if (!Mode.IsValid())
	{
		return FInputRayHit();
	}
	if (AVTBAttributeEditor* Editor = FindEditor(Mode.Get()))
	{
		if (Editor->GetSnapshot().bIsModifying)
		{
			return FInputRayHit();
		}
	}
	return FInputRayHit(TNumericLimits<double>::Max());
}

void UOWTAttributeEditTool::OnClicked(const FInputDeviceRay& ClickPos)
{
	if (!Mode.IsValid())
	{
		return;
	}
	UWorld* World = Mode->GetWorld();
	if (!World)
	{
		return;
	}

	FHitResult Hit;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(OWTAttributeSelection), true);
	if (AVTBAttributeEditor* Editor = FindEditor(Mode.Get()))
	{
		QueryParams.AddIgnoredActor(Editor);
	}
	if (APlayerController* Controller = World->GetFirstPlayerController())
	{
		if (APawn* Pawn = Controller->GetPawn())
		{
			QueryParams.AddIgnoredActor(Pawn);
		}
	}

	const FVector TraceStart = ClickPos.WorldRay.Origin;
	const FVector TraceEnd = TraceStart + ClickPos.WorldRay.Direction * 1000000.0;
	World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		if (HitActor->IsA<ACombinedTransformGizmoActor>())
		{
			return;
		}
	}
	SelectActor(HitActor);
}

void UOWTAttributeEditTool::ShowGizmo()
{
	if (!Mode.IsValid())
	{
		return;
	}
	UVTBOWTEditorToolsContext* Context = Mode->GetToolsContext();
	if (!Context)
	{
		return;
	}
	AActor* Actor = Mode->GetSelectedObject();
	USceneComponent* Root = Actor ? Actor->GetRootComponent() : nullptr;
	if (!IsValid(Root))
	{
		return;
	}
	if (Root->Mobility != EComponentMobility::Movable)
	{
		return;
	}
	if (TransformGizmo)
	{
		if (GizmoTarget.Get() == Root)
		{
			return;
		}
		HideGizmo();
	}
	bHidden = false;
	TransformGizmo =
	    Cast<UVTBOWTBaseTransformGizmo>(Context->GizmoManager->CreateGizmo(TEXT("OWT.Transform"), FString(), this));
	if (!ensure(TransformGizmo))
	{
		return;
	}
	TransformGizmo->SetTargetComponent(*Root);
	UTransformProxy* Proxy = TransformGizmo->GetTransformProxy();
	check(Proxy);
	Proxy->OnBeginTransformEdit.AddUObject(this, &ThisClass::OnGizmoEditStarted);
	Proxy->OnTransformChanged.AddUObject(this, &ThisClass::OnGizmoTransformChanged);
	Proxy->OnEndTransformEdit.AddUObject(this, &ThisClass::OnGizmoEditEnded);
	GizmoTarget = Root;
	TransformGizmo->Tick(0.f);
}

void UOWTAttributeEditTool::HideGizmo()
{
	bHidden = true;
	if (!Mode.IsValid())
	{
		return;
	}
	if (AVTBAttributeEditor* Editor = FindEditor(Mode.Get()))
	{
		Editor->FinishActiveOperation();
	}
	if (!TransformGizmo)
	{
		return;
	}
	Mode->TerminateCapture();
	if (UTransformProxy* Proxy = GetTransformProxy())
	{
		Proxy->OnBeginTransformEdit.RemoveAll(this);
		Proxy->OnTransformChanged.RemoveAll(this);
		Proxy->OnEndTransformEdit.RemoveAll(this);
	}
	if (UVTBOWTEditorToolsContext* Context = Mode->GetToolsContext())
	{
		Context->GizmoManager->DestroyAllGizmosByOwner(this);
	}
	TransformGizmo = nullptr;
	GizmoTarget.Reset();
}

void UOWTAttributeEditTool::SynchronizeGizmo()
{
	if (!Mode.IsValid())
	{
		return;
	}
	if (Mode->HasCapture())
	{
		return;
	}
	if (!TransformGizmo)
	{
		return;
	}
	USceneComponent* Root = GizmoTarget.Get();
	if (!IsValid(Root))
	{
		return;
	}
	TransformGizmo->ReinitializeGizmoTransform(Root->GetComponentTransform());
}

UTransformProxy* UOWTAttributeEditTool::GetTransformProxy() const
{
	return TransformGizmo ? TransformGizmo->GetTransformProxy() : nullptr;
}

void UOWTAttributeEditTool::OnGizmoEditStarted(UTransformProxy* Proxy)
{
	if (AVTBAttributeEditor* Editor = FindEditor(Mode.Get()))
	{
		Editor->BeginGizmoEdit();
	}
}

void UOWTAttributeEditTool::OnGizmoTransformChanged(UTransformProxy* Proxy, FTransform Transform)
{
	if (AVTBAttributeEditor* Editor = FindEditor(Mode.Get()))
	{
		Editor->UpdateGizmoEdit();
	}
}

void UOWTAttributeEditTool::OnGizmoEditEnded(UTransformProxy* Proxy)
{
	if (AVTBAttributeEditor* Editor = FindEditor(Mode.Get()))
	{
		Editor->EndGizmoEdit();
	}
}

bool UOWTAttributeEditToolBuilder::CanBuildTool(const FToolBuilderState& State) const
{
	UOWTAttributeEditMode* Mode = ResolveAttributeMode(State);
	return Mode ? Mode->IsEntered() : false;
}

UInteractiveTool* UOWTAttributeEditToolBuilder::BuildTool(const FToolBuilderState& State) const
{
	if (!CanBuildTool(State))
	{
		return nullptr;
	}
	UOWTAttributeEditTool* Tool = NewObject<UOWTAttributeEditTool>(State.ToolManager);
	Tool->Initialize(*ResolveAttributeMode(State));
	return Tool;
}
