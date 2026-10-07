#include "VTBOWTEditorSubsystem.h"

#include "Context/OWTEditContexts.h"
#include "Modes/VTBOWTObjectEditMode.h"
#include "Tools/OWTAttributeEditTool.h"
#include "Gizmos/Base/VTBOWTBaseTransformGizmo.h"
#include "VTBAttributeEditor.h"

UVTBOWTEditorSubsystem::UVTBOWTEditorSubsystem()
    : DefaultModeClass(UVTBOWTObjectEditMode::StaticClass()), ActiveEditMode(nullptr), SelectedObject(),
      AttributeEditor(), SystemContextHandlers()
{
}

void UVTBOWTEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	InitializeDefaultMode();
	RegisterSystemContextHandlers();
}

void UVTBOWTEditorSubsystem::InitializeDefaultMode()
{
	UClass* ModeClass = DefaultModeClass ? DefaultModeClass.Get() : UVTBOWTObjectEditMode::StaticClass();
	UOWTAttributeEditMode* Mode = NewObject<UOWTAttributeEditMode>(this, ModeClass);
	ActiveEditMode = Mode;
	Mode->Initialize(*this);
}

void UVTBOWTEditorSubsystem::RegisterSystemContextHandlers()
{
	SystemContextHandlers.Add(FOWTToggleEditingContext::StaticStruct(),
	                          [this](const FInstancedStruct&)
	                          {
		                          ToggleEditing();
		                          return true;
	                          });
	SystemContextHandlers.Add(FOWTGizmoPointerContext::StaticStruct(),
	                          [this](const FInstancedStruct& Context)
	                          {
		                          return RouteGizmoPointer(Context.Get<FOWTGizmoPointerContext>());
	                          });
}

void UVTBOWTEditorSubsystem::Deinitialize()
{
	if (UOWTAttributeEditMode* Mode = GetAttributeEditMode())
	{
		Mode->Shutdown();
	}
	SelectedObject.Reset();
	AttributeEditor.Reset();
	SystemContextHandlers.Empty();
	ActiveEditMode = nullptr;
	Super::Deinitialize();
}

void UVTBOWTEditorSubsystem::OnWorldEndPlay(UWorld& InWorld)
{
	if (UOWTAttributeEditMode* Mode = GetAttributeEditMode())
	{
		Mode->Shutdown();
	}
	SelectedObject.Reset();
	Super::OnWorldEndPlay(InWorld);
}

void UVTBOWTEditorSubsystem::Tick(float DeltaTime)
{
	if (UOWTAttributeEditMode* Mode = GetAttributeEditMode())
	{
		Mode->Tick(DeltaTime);
	}
	if (AVTBAttributeEditor* Editor = AttributeEditor.Get())
	{
		Editor->RefreshSelectedTransform();
	}
}

TStatId UVTBOWTEditorSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UVTBOWTEditorSubsystem, STATGROUP_Tickables);
}

bool UVTBOWTEditorSubsystem::ReceiveEditContext_Implementation(const FInstancedStruct& Context)
{
	check(IsInGameThread());
	if (!Context.IsValid())
	{
		return false;
	}
	if (const auto* Handler = SystemContextHandlers.Find(Context.GetScriptStruct()))
	{
		return (*Handler)(Context);
	}
	if (!IsEditingEnabled())
	{
		return false;
	}
	if (!IsValid(ActiveEditMode))
	{
		return false;
	}
	return IOWTEditContextReceiver::Execute_ReceiveEditContext(ActiveEditMode, Context);
}

bool UVTBOWTEditorSubsystem::SetActiveEditMode(UObject* InMode)
{
	check(IsInGameThread());
	UOWTAttributeEditMode* NextMode = Cast<UOWTAttributeEditMode>(InMode);
	if (!IsValid(NextMode))
	{
		return false;
	}
	if (NextMode->GetOuter() != this)
	{
		return false;
	}
	if (NextMode == GetAttributeEditMode())
	{
		return true;
	}
	const bool bWasEditing = IsEditingEnabled();
	if (!NextMode->Initialize(*this))
	{
		return false;
	}
	if (UOWTAttributeEditMode* OldMode = GetAttributeEditMode())
	{
		OldMode->Shutdown();
	}
	ActiveEditMode = NextMode;
	SelectedObject.Reset();
	NotifyEditorStateChanged();
	if (bWasEditing)
	{
		return NextMode->Enter();
	}
	return true;
}

UOWTAttributeEditMode* UVTBOWTEditorSubsystem::GetAttributeEditMode() const
{
	return Cast<UOWTAttributeEditMode>(ActiveEditMode);
}

bool UVTBOWTEditorSubsystem::InitializeToolsContext()
{
	UOWTAttributeEditMode* Mode = GetAttributeEditMode();
	return Mode ? Mode->Enter() : false;
}

void UVTBOWTEditorSubsystem::ShutdownToolsContext()
{
	if (UOWTAttributeEditMode* Mode = GetAttributeEditMode())
	{
		Mode->Exit();
	}
}

void UVTBOWTEditorSubsystem::ToggleEditing()
{
	if (UOWTAttributeEditMode* Mode = GetAttributeEditMode())
	{
		if (Mode->IsEntered())
		{
			Mode->Exit();
		}
		else
		{
			Mode->Enter();
		}
	}
}

bool UVTBOWTEditorSubsystem::IsEditingEnabled() const
{
	const UOWTAttributeEditMode* Mode = GetAttributeEditMode();
	return Mode ? Mode->IsEntered() : false;
}

void UVTBOWTEditorSubsystem::ShowSelectionGizmo()
{
	if (UOWTAttributeEditMode* Mode = GetAttributeEditMode())
	{
		Mode->ShowSelectionGizmo();
	}
}

void UVTBOWTEditorSubsystem::HideSelectionGizmo()
{
	if (UOWTAttributeEditMode* Mode = GetAttributeEditMode())
	{
		Mode->HideSelectionGizmo();
	}
}

void UVTBOWTEditorSubsystem::SetSelectedObject(AActor* Actor)
{
	if (UOWTAttributeEditMode* Mode = GetAttributeEditMode())
	{
		Mode->SetSelectedObject(Actor);
	}
}

void UVTBOWTEditorSubsystem::SetCoordinateSystem(EToolContextCoordinateSystem System)
{
	if (UOWTAttributeEditMode* Mode = GetAttributeEditMode())
	{
		Mode->SetCoordinateSystem(System);
	}
}

void UVTBOWTEditorSubsystem::SetTransformGizmoMode(EToolContextTransformGizmoMode Value)
{
	if (UOWTAttributeEditMode* Mode = GetAttributeEditMode())
	{
		Mode->SetTransformGizmoMode(Value);
	}
}

bool UVTBOWTEditorSubsystem::SetGizmoSnapSettings(const FOWTGizmoSnapSettings& Settings)
{
	UOWTAttributeEditMode* Mode = GetAttributeEditMode();
	return Mode ? Mode->SetSnapSettings(Settings) : false;
}

FOWTGizmoSnapSettings UVTBOWTEditorSubsystem::GetGizmoSnapSettings() const
{
	const UOWTAttributeEditMode* Mode = GetAttributeEditMode();
	return Mode ? Mode->GetSnapSettings() : FOWTGizmoSnapSettings();
}

void UVTBOWTEditorSubsystem::SynchronizeSelectionGizmo()
{
	if (UOWTAttributeEditMode* Mode = GetAttributeEditMode())
	{
		Mode->SynchronizeSelectionGizmo();
	}
}

void UVTBOWTEditorSubsystem::TerminateGizmoCapture()
{
	if (UOWTAttributeEditMode* Mode = GetAttributeEditMode())
	{
		Mode->TerminateCapture();
	}
}

bool UVTBOWTEditorSubsystem::HasGizmoCapture() const
{
	const UOWTAttributeEditMode* Mode = GetAttributeEditMode();
	return Mode ? Mode->HasCapture() : false;
}

UVTBOWTEditorToolsContext* UVTBOWTEditorSubsystem::GetToolsContext() const
{
	const UOWTAttributeEditMode* Mode = GetAttributeEditMode();
	return Mode ? Mode->GetToolsContext() : nullptr;
}

UCombinedTransformGizmo* UVTBOWTEditorSubsystem::GetTransformGizmo() const
{
	UOWTAttributeEditTool* Tool = FindAttributeTool();
	return Tool ? Tool->GetGizmo() : nullptr;
}

UTransformProxy* UVTBOWTEditorSubsystem::GetTransformProxy() const
{
	UOWTAttributeEditTool* Tool = FindAttributeTool();
	return Tool ? Tool->GetTransformProxy() : nullptr;
}

EToolContextCoordinateSystem UVTBOWTEditorSubsystem::GetCoordinateSystem() const
{
	const UOWTAttributeEditMode* Mode = GetAttributeEditMode();
	return Mode ? Mode->GetCoordinateSystem() : EToolContextCoordinateSystem::World;
}

EToolContextTransformGizmoMode UVTBOWTEditorSubsystem::GetTransformGizmoMode() const
{
	const UOWTAttributeEditMode* Mode = GetAttributeEditMode();
	return Mode ? Mode->GetTransformGizmoMode() : EToolContextTransformGizmoMode::Translation;
}

AVTBAttributeEditor* UVTBOWTEditorSubsystem::GetAttributeEditor() const
{
	return AttributeEditor.Get();
}

void UVTBOWTEditorSubsystem::RegisterAttributeEditor(AVTBAttributeEditor* Editor)
{
	check(IsInGameThread());
	if (Editor)
	{
		checkf(Editor->GetWorld() == GetWorld(), TEXT("AttributeEditor must belong to this world."));
	}
	AttributeEditor = Editor;
}

void UVTBOWTEditorSubsystem::NotifyEditorStateChanged()
{
	if (AVTBAttributeEditor* Editor = AttributeEditor.Get())
	{
		Editor->NotifyEditorStateChanged();
	}
}

bool UVTBOWTEditorSubsystem::RouteGizmoPointer(const FOWTGizmoPointerContext& Pointer)
{
	UOWTAttributeEditMode* Mode = GetAttributeEditMode();
	return Mode ? Mode->RoutePointer(Pointer) : false;
}

UOWTAttributeEditTool* UVTBOWTEditorSubsystem::FindAttributeTool() const
{
	const UOWTAttributeEditMode* Mode = GetAttributeEditMode();
	if (!Mode)
	{
		return nullptr;
	}

	return Mode->GetAttributeTool();
}
