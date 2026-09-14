#include "Context/VTBOWTEditorToolsContext.h"

#include "Camera/PlayerCameraManager.h"
#include "Components/SceneComponent.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "InteractiveToolChange.h"
#include "Materials/Material.h"
#include "VTBOWTEditorSubsystem.h"
#include "Context/VTBOWTSceneSnappingManager.h"
#include "ContextObjectStore.h"
#include "InputRouter.h"

namespace
{
class FOWTToolsQueries : public IToolsContextQueriesAPI
{
public:
	FOWTToolsQueries(UVTBOWTEditorToolsContext& InContext, UVTBOWTEditorSubsystem& InOwner)
	    : Context(InContext), Owner(InOwner)
	{
	}

	virtual UWorld* GetCurrentEditingWorld() const override
	{
		return Owner.GetWorld();
	}

	virtual void GetCurrentSelectionState(FToolBuilderState& State) const override
	{
		State = FToolBuilderState();
		State.World = Owner.GetWorld();
		State.ToolManager = Context.ToolManager;
		State.GizmoManager = Context.GizmoManager;
		State.TargetManager = Context.TargetManager;
		if (AActor* Actor = Owner.SelectedObject.Get())
		{
			State.SelectedActors.Add(Actor);
			if (USceneComponent* Root = Actor->GetRootComponent())
			{
				State.SelectedComponents.Add(Root);
			}
		}
	}

	virtual void GetCurrentViewState(FViewCameraState& State) const override
	{
		APlayerController* Controller = Owner.GetWorld()->GetFirstPlayerController();
		APlayerCameraManager* Camera = Controller ? Controller->PlayerCameraManager.Get() : nullptr;
		if (!Camera)
		{
			return;
		}

		const FMinimalViewInfo& View = Camera->GetCameraCacheView();
		State.Position = View.Location;
		State.Orientation = View.Rotation.Quaternion();
		State.HorizontalFOVDegrees = View.FOV;
		State.AspectRatio = View.AspectRatio;
		State.OrthoWorldCoordinateWidth = View.OrthoWidth;
		State.bIsOrthographic = View.ProjectionMode == ECameraProjectionMode::Orthographic;
	}

	virtual EToolContextCoordinateSystem GetCurrentCoordinateSystem() const override
	{
		return Owner.GetCoordinateSystem();
	}

	virtual EToolContextTransformGizmoMode GetCurrentTransformGizmoMode() const override
	{
		return Owner.GetTransformGizmoMode();
	}

	virtual FToolContextSnappingConfiguration GetCurrentSnappingSettings() const override
	{
		const FOWTGizmoSnapSettings Settings = Context.GetSnapSettings();
		FToolContextSnappingConfiguration Configuration;
		Configuration.bEnablePositionGridSnapping = Settings.bTranslationEnabled;
		Configuration.PositionGridDimensions = FVector(Settings.TranslationStep);
		Configuration.bEnableRotationGridSnapping = Settings.bRotationEnabled;
		Configuration.RotationGridAngles =
		    FRotator(Settings.RotationStepDegrees, Settings.RotationStepDegrees, Settings.RotationStepDegrees);
		Configuration.bEnableScaleGridSnapping = Settings.bScaleEnabled;
		Configuration.ScaleGridSize = Settings.ScaleStep;
		return Configuration;
	}

	virtual UMaterialInterface* GetStandardMaterial(EStandardToolContextMaterials Type) const override
	{
		return UMaterial::GetDefaultMaterial(MD_Surface);
	}

	virtual FViewport* GetHoveredViewport() const override
	{
		UGameViewportClient* Viewport = Owner.GetWorld()->GetGameViewport();
		return Viewport ? Viewport->Viewport : nullptr;
	}

	virtual FViewport* GetFocusedViewport() const override
	{
		return GetHoveredViewport();
	}

private:
	UVTBOWTEditorToolsContext& Context;
	UVTBOWTEditorSubsystem& Owner;
};

// Runtime history is a separate service. These hooks deliberately do not use GEditor transactions.
class FOWTToolsTransactions : public IToolsContextTransactionsAPI
{
public:
	virtual void DisplayMessage(const FText& Message, EToolMessageLevel Level) override
	{
		UE_LOG(LogTemp, Display, TEXT("OWT Tools: %s"), *Message.ToString());
	}

	virtual void PostInvalidation() override
	{
		// Game viewports redraw every frame.
	}

	virtual void BeginUndoTransaction(const FText& Description) override
	{
	}

	virtual void EndUndoTransaction() override
	{
	}

	virtual void AppendChange(UObject* Target, TUniquePtr<FToolCommandChange> Change, const FText& Description) override
	{
		// History integration is not implemented yet; do not advertise Undo support.
	}

	virtual bool RequestSelectionChange(const FSelectedObjectsChangeList& Change) override
	{
		// Selection enters through the edit-context interface.
		return false;
	}
};
} // namespace

UVTBOWTEditorToolsContext::UVTBOWTEditorToolsContext() : SnapSettings(), Queries(), Transactions(), bInitialized(false)
{
}

void UVTBOWTEditorToolsContext::InitializeContext(UVTBOWTEditorSubsystem& Subsystem)
{
	check(IsInGameThread());
	if (bInitialized)
	{
		return;
	}

	Queries = MakeUnique<FOWTToolsQueries>(*this, Subsystem);
	Transactions = MakeUnique<FOWTToolsTransactions>();
	Super::Initialize(Queries.Get(), Transactions.Get());
	ContextObjectStore->AddContextObject(NewObject<UVTBOWTSceneSnappingManager>(this));
	bInitialized = true;
}

void UVTBOWTEditorToolsContext::Shutdown()
{
	check(IsInGameThread());
	if (!bInitialized)
	{
		return;
	}

	// Managers may still call the APIs while shutting down.
	Super::Shutdown();
	Transactions.Reset();
	Queries.Reset();
	bInitialized = false;
}

bool UVTBOWTEditorToolsContext::IsInitialized() const
{
	return bInitialized;
}

bool UVTBOWTEditorToolsContext::SetSnapSettings(const FOWTGizmoSnapSettings& Settings)
{
	check(IsInGameThread());
	if (!FMath::IsFinite(Settings.TranslationStep) || Settings.TranslationStep <= UE_SMALL_NUMBER ||
	    !FMath::IsFinite(Settings.RotationStepDegrees) || Settings.RotationStepDegrees <= UE_SMALL_NUMBER ||
	    !FMath::IsFinite(Settings.ScaleStep) || Settings.ScaleStep <= UE_SMALL_NUMBER)
	{
		return false;
	}

	// Finish the current interaction before changing its quantization rule.
	if (bInitialized)
	{
		InputRouter->ForceTerminateAll();
	}
	SnapSettings = Settings;
	return true;
}

FOWTGizmoSnapSettings UVTBOWTEditorToolsContext::GetSnapSettings() const
{
	return SnapSettings;
}
