#include "VTBOWTEditorModeSubsystem.h"

#include "Context/VTBOWTEditorToolsContext.h"
#include "Context/IVTBOWTEditorInput.h"
#include "Context/IVTBOWTEditorSceneState.h"
#include "Context/IVTBOWTEditorViewport.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "Gizmo/VTBOWTEditorRepositionalGizmo.h"
#include "VTBOWTEditorGizmoManager.h"

void UVTBOWTEditorModeSubsystem::CreateToolsContext()
{
	UVTBOWTEditorToolsContext* Context = NewObject<UVTBOWTEditorToolsContext>(this);

	Context->SetCreateGizmoManagerFunc([](const UInteractiveToolsContext::FContextInitInfo& ContextInfo)
	{
		UVTBOWTEditorGizmoManager* Manager = NewObject<UVTBOWTEditorGizmoManager>(ContextInfo.ToolsContext);
		Manager->Initialize(ContextInfo.QueriesAPI, ContextInfo.TransactionsAPI, ContextInfo.InputRouter);
		Manager->RegisterDefaultGizmos();

		return Manager;
	});

	EditorToolsContext = Context;
}

void UVTBOWTEditorModeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CreateToolsContext();

	EditorToolsContext->InitializeContext(GetWorld());
	EditorToolsContext->OnSelectionChangeRequested.BindUObject(this, &ThisClass::HandleSelectionChange);
}

void UVTBOWTEditorModeSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if (!SelectionSource.IsValid())
	{
		if (AGameModeBase* GameModeBase = InWorld.GetAuthGameMode())
		{
			BindSelectionSource(GameModeBase);
		}
	}
}

UVTBOWTEditorGizmoManager* UVTBOWTEditorModeSubsystem::GetGizmoManager() const
{
	return EditorToolsContext ? Cast<UVTBOWTEditorGizmoManager>(EditorToolsContext->GizmoManager) : nullptr;
}

bool UVTBOWTEditorModeSubsystem::BindSelectionSource(UObject* Source)
{
	IVTBOWTEditorSelectionSource* Next = IsValid(Source) ? Cast<IVTBOWTEditorSelectionSource>(Source) : nullptr;
	if (!IsValid(EditorToolsContext) || (Source && !Next))
	{
		return false;
	}
	if (SelectionSource.Get() == Source && (Source || !SelectionChangedHandle.IsValid()))
	{
		return true;
	}
	if (IVTBOWTEditorSelectionSource* Previous = Cast<IVTBOWTEditorSelectionSource>(SelectionSource.Get()))
	{
		Previous->OnSelectionChanged().Remove(SelectionChangedHandle);
	}
	SelectionChangedHandle.Reset();
	SelectionSource = Source;
	if (Next)
	{
		SelectionChangedHandle = Next->OnSelectionChanged().AddUObject(this, &ThisClass::OnSelectionChanged);
	}
	OnSelectionChanged();
	return true;
}

void UVTBOWTEditorModeSubsystem::OnSelectionChanged()
{
	FVTBOWTActorSelection Actors;
	USceneComponent* Frame = nullptr;
	if (IVTBOWTEditorSelectionSource* Source = Cast<IVTBOWTEditorSelectionSource>(SelectionSource.Get()))
	{
		Source->GetSelection(Actors);
		Frame = Source->GetSelectionFrame();
	}
	ReceiveSelection(Actors, Frame);
}

bool UVTBOWTEditorModeSubsystem::HandleSelectionChange(const FSelectedObjectsChangeList& Change)
{
	IVTBOWTEditorSelectionSource* Source = Cast<IVTBOWTEditorSelectionSource>(SelectionSource.Get());
	return Source && Source->ApplySelectionChange(Change);
}

void UVTBOWTEditorModeSubsystem::ReceiveSelection(const FVTBOWTActorSelection& Actors, USceneComponent* FrameComponent)
{
	if (!EditorToolsContext || !EditorToolsContext->IsRuntimeReady())
	{
		return;
	}
	Selection = FVTBOWTTransformSelection{Actors, FrameComponent};
	ApplyPendingSelection();
}

void UVTBOWTEditorModeSubsystem::ApplyPendingSelection()
{
	UVTBOWTEditorToolsContext* Context = EditorToolsContext;
	if (!Context)
	{
		return;
	}
	Context->RunContextUpdate([&]
	{
		UVTBOWTEditorGizmoManager* Manager = GetGizmoManager();
		if (!Manager)
		{
			return;
		}
		TOptional<FVTBOWTTransformSelection> Request = MoveTemp(Selection);
		Selection.Reset();
		UVTBOWTEditorRepositionalGizmo* PreviousGizmo = Manager->GetSelectionGizmo();
		TWeakObjectPtr<USceneComponent> PreviousFrame = PreviousGizmo ? PreviousGizmo->GetSelectionFrame() : nullptr;
		const bool bApplied = Manager->SynchronizeSelection(Request, [Context]
		{
			Context->GetInput().CancelActiveInteraction();
			return Context->IsRuntimeReady();
		});
		if (bApplied && Context->IsRuntimeReady())
		{
			TArray<AActor*> Actors;
			Manager->GetSelection(Actors);
			UVTBOWTEditorRepositionalGizmo* Gizmo = Manager->GetSelectionGizmo();
			USceneComponent* Frame = Gizmo ? Gizmo->GetSelectionFrame() : nullptr;
			TArray<UActorComponent*> Components;
			for (AActor* Actor : Actors)
			{
				Components.Add(Frame && Frame->GetOwner() == Actor ? Frame : Actor->GetRootComponent());
			}
			Context->GetSceneState().SetSelection(Actors, Components);
			if (Frame && Frame != PreviousFrame.Get())
			{
				Context->GetSceneState().SetCoordinateSystem(EToolContextCoordinateSystem::Local);
			}
		}
		if (!bApplied && Request.IsSet() && !Selection.IsSet() && Context->IsRuntimeReady())
		{
			Selection = MoveTemp(Request);
		}
	});
}

void UVTBOWTEditorModeSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UVTBOWTEditorToolsContext* Context = EditorToolsContext;
	if (!Context || !Context->IsRuntimeReady())
	{
		return;
	}
	if (SelectionChangedHandle.IsValid() && !SelectionSource.IsValid())
	{
		BindSelectionSource(nullptr);
	}
	ApplyPendingSelection();
	if (!Context->IsRuntimeReady())
	{
		return;
	}
	APlayerController* LocalController = nullptr;
	UWorld* EditingWorld = Context->GetEditingWorld();
	if (IsValid(EditingWorld))
	{
		for (FConstPlayerControllerIterator It = EditingWorld->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerController* Candidate = It->Get();
			if (IsValid(Candidate) && Candidate->IsLocalController())
			{
				LocalController = Candidate;
				break;
			}
		}
	}
	const bool bHasView = Context->GetViewport().UpdateView(LocalController);
	if (!bHasView)
	{
		Context->GetInput().CancelActiveInteraction();
	}
	Context->TickRuntime(DeltaTime);
	if (Context->IsRuntimeReady())
	{
		if (UVTBOWTEditorGizmoManager* Manager = GetGizmoManager())
		{
			Manager->UpdateSelectionVisibility(bHasView);
		}
	}
}

void UVTBOWTEditorModeSubsystem::Deinitialize()
{
	if (IVTBOWTEditorSelectionSource* Source = Cast<IVTBOWTEditorSelectionSource>(SelectionSource.Get()))
	{
		Source->OnSelectionChanged().Remove(SelectionChangedHandle);
	}

	SelectionChangedHandle.Reset();
	SelectionSource.Reset();
	Selection.Reset();

	if (EditorToolsContext)
	{
		EditorToolsContext->OnSelectionChangeRequested.Unbind();
		EditorToolsContext->Shutdown();
	}

	EditorToolsContext = nullptr;

	Super::Deinitialize();
}

TStatId UVTBOWTEditorModeSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UVTBOWTEditorModeSubsystem, STATGROUP_Tickables);
}

UWorld* UVTBOWTEditorModeSubsystem::GetTickableGameObjectWorld() const
{
	return GetWorld();
}

UVTBOWTEditorToolsContext* UVTBOWTEditorModeSubsystem::GetToolsContext()
{
	if (!EditorToolsContext)
	{
		CreateToolsContext();
	}

	return EditorToolsContext;
}
