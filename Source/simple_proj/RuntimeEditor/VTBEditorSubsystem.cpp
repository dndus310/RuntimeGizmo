#include "VTBEditorSubsystem.h"

#include "Context/VTBEditorInteractiveToolsContext.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Selection/VTBSelectionSource.h"

UVTBEditorSubsystem::UVTBEditorSubsystem()
{
}

bool UVTBEditorSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}

	const UWorld* World = Cast<UWorld>(Outer);
	if (!ensureMsgf(World != nullptr, TEXT("UVTBEditorSubsystem requires a UWorld outer.")))
	{
		return false;
	}

	return World->IsGameWorld() && World->GetNetMode() != NM_DedicatedServer;
}

void UVTBEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ToolsContext = NewObject<UVTBEditorInteractiveToolsContext>(this);
	if (!ensureMsgf(IsValid(ToolsContext), TEXT("Failed to create the runtime tools context.")))
	{
		return;
	}

	if (!ensureMsgf(ToolsContext->InitializeRuntime(GetWorld()), TEXT("Failed to initialize the runtime tools context.")))
	{
		ToolsContext = nullptr;
		return;
	}
}

void UVTBEditorSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	if (SelectionSource.IsValid())
	{
		return;
	}

	AGameModeBase* GameMode = InWorld.GetAuthGameMode();
	if (!IsValid(GameMode))
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
	IVTBSelectionSource* Selection = Cast<IVTBSelectionSource>(SelectionSource.Get());
	if (Selection)
	{
		Selection->OnSelectionChanged().Remove(SelectionChangedHandle);
	}
	SelectionChangedHandle.Reset();
	SelectionSource.Reset();
	if (IsValid(ToolsContext))
	{
		ToolsContext->Shutdown();
	}
	ToolsContext = nullptr;
	PendingSelection.Reset();
}

bool UVTBEditorSubsystem::BindSelectionSource(UObject* Source)
{
	if (!IsValid(ToolsContext))
	{
		return false;
	}

	if (SelectionSource.Get() == Source)
	{
		return true;
	}

	IVTBSelectionSource* Selection = IsValid(Source) ? Cast<IVTBSelectionSource>(Source) : nullptr;
	if (!ensureMsgf(Source == nullptr || Selection != nullptr, TEXT("Selection source must implement IVTBSelectionSource.")))
	{
		return false;
	}

	IVTBSelectionSource* Previous = Cast<IVTBSelectionSource>(SelectionSource.Get());
	if (Previous)
	{
		Previous->OnSelectionChanged().Remove(SelectionChangedHandle);
	}
	SelectionChangedHandle.Reset();
	SelectionSource = Source;
	if (Selection != nullptr)
	{
		SelectionChangedHandle = Selection->OnSelectionChanged().AddUObject(this, &ThisClass::OnSelectionChanged);
	}
	OnSelectionChanged();
	return true;
}

void UVTBEditorSubsystem::OnSelectionChanged()
{
	TArray<TWeakObjectPtr<AActor>> Actors;
	IVTBSelectionSource* Selection = Cast<IVTBSelectionSource>(SelectionSource.Get());
	if (Selection)
	{
		Selection->GetSelectionSnapshot(Actors);
	}
	ReceiveSelection(Actors);
}

void UVTBEditorSubsystem::ReceiveSelection(const TArray<TWeakObjectPtr<AActor>>& Actors)
{
	if (!IsValid(ToolsContext) || !ToolsContext->IsRuntimeReady())
	{
		return;
	}

	PendingSelection = Actors;
	ApplyPendingSelection();
}

void UVTBEditorSubsystem::ApplyPendingSelection()
{
	UVTBEditorInteractiveToolsContext* Context = ToolsContext;
	if (!IsValid(Context))
	{
		return;
	}

	TOptional<TArray<TWeakObjectPtr<AActor>>> Selection = MoveTemp(PendingSelection);
	PendingSelection.Reset();
	if (!Context->ApplyTransformGizmoState(this, Selection) && Selection.IsSet())
	{
		PendingSelection = MoveTemp(Selection);
	}
}

void UVTBEditorSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UVTBEditorInteractiveToolsContext* Context = ToolsContext;
	if (!IsValid(Context) || !Context->IsRuntimeReady())
	{
		return;
	}

	if (SelectionChangedHandle.IsValid() && !SelectionSource.IsValid())
	{
		BindSelectionSource(nullptr);
	}

	ApplyPendingSelection();
	// Gizmo callbacks can enqueue work or tear down the runtime context.
	if (!IsValid(Context) || !Context->IsRuntimeReady())
	{
		return;
	}

	const bool bHasView = Context->UpdateView();
	if (!bHasView)
	{
		Context->CancelActiveInteraction();
	}

	Context->TickRuntime(DeltaTime);
	Context->UpdateTransformGizmoVisibility(bHasView);
}

TStatId UVTBEditorSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UVTBEditorSubsystem, STATGROUP_Tickables);
}
