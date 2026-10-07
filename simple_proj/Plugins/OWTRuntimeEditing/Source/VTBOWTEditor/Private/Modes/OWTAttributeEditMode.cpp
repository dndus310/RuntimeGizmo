#include "Modes/OWTAttributeEditMode.h"
#include "BaseGizmos/GizmoViewContext.h"
#include "Context/OWTAttributeEditSessionContext.h"
#include "Context/OWTEditContexts.h"
#include "Context/VTBOWTEditorToolsContext.h"
#include "ContextObjectStore.h"
#include "Duplication/OWTRuntimeActorDuplicator.h"
#include "Duplication/OWTDuplicationRequest.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Extensions/OWTAttributeModeExtension.h"
#include "Features/IModularFeatures.h"
#include "GameFramework/PlayerController.h"
#include "Gizmos/Base/VTBOWTBaseTransformGizmo.h"
#include "Gizmos/Base/VTBOWTAxisPositionGizmo.h"
#include "Gizmos/Base/VTBOWTAxisAngleGizmo.h"
#include "Gizmos/Custom/VTBOWTCustomTransformGizmo.h"
#include "InputRouter.h"
#include "InteractiveGizmoManager.h"
#include "InteractiveToolManager.h"
#include "Rendering/OWTRuntimeToolsViewportBridge.h"
#include "SceneView.h"
#include "Targets/OWTActorToolTarget.h"
#include "ToolTargetManager.h"
#include "Tools/OWTAttributeEditTool.h"
#include "Tools/OWTDuplicateTool.h"
#include "Tools/OWTModeToolBuilder.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorSubsystem.h"

FName OWTToolIds::AttributeEdit()
{
	return TEXT("OWT.AttributeEdit");
}

FName OWTToolIds::Duplicate()
{
	return TEXT("OWT.Duplicate");
}

UOWTAttributeEditMode::UOWTAttributeEditMode()
    : DefaultToolId(OWTToolIds::AttributeEdit()), AdditionalTools(), Subsystem(), ToolsContext(nullptr),
      SessionContext(nullptr), Duplicator(nullptr), Selection(), RegisteredTools(), RegisteredTargets(),
      TargetProviders(), RegisteredServices(), ServiceProviders(), ProviderLeases(), Operations(), Snapshot(),
      SnapSettings(), PendingOptions(), PendingOperation(), PendingSource(),
      CoordinateSystem(EToolContextCoordinateSystem::World), GizmoMode(EToolContextTransformGizmoMode::Translation),
      bInitialized(false), bEntered(false), bChangingTool(false), bRestoreDefault(false), bExecutingDuplicate(false),
      bExiting(false), bShuttingDown(false), bShutdownRequested(false), bDispatchingDuplicate(false)
{
}

UWorld* UOWTAttributeEditMode::GetWorld() const
{
	return Subsystem.IsValid() ? Subsystem->GetWorld() : nullptr;
}

bool UOWTAttributeEditMode::Initialize(UVTBOWTEditorSubsystem& InSubsystem)
{
	check(IsInGameThread());
	if (bInitialized)
	{
		return Subsystem.Get() == &InSubsystem;
	}
	if (GetOuter() != &InSubsystem)
	{
		return false;
	}
	Subsystem = &InSubsystem;
	if (!GetWorld())
	{
		Subsystem.Reset();
		return false;
	}
	Duplicator = NewObject<UOWTRuntimeActorDuplicator>(this);
	if (!Duplicator->Initialize(this))
	{
		Duplicator = nullptr;
		Subsystem.Reset();
		return false;
	}
	Duplicator->OnProceduralChanged.AddUObject(this, &ThisClass::ForwardProcedural);
	if (DefaultToolId.IsNone())
	{
		DefaultToolId = OWTToolIds::AttributeEdit();
	}
	bInitialized = true;
	Snapshot.ModeId = GetClass()->GetFName();
	Snapshot.Lifecycle = TEXT("Initialized");
	NotifyStateChanged();
	return true;
}

bool UOWTAttributeEditMode::Enter()
{
	check(IsInGameThread());
	if (bExiting)
	{
		return false;
	}
	if (bShuttingDown)
	{
		return false;
	}
	if (bShutdownRequested)
	{
		return false;
	}
	if (!bInitialized)
	{
		return false;
	}
	if (bExecutingDuplicate)
	{
		return false;
	}
	if (bEntered)
	{
		return true;
	}
	if (!GetWorld())
	{
		return false;
	}
	if (GetWorld()->bIsTearingDown)
	{
		return false;
	}

	InitializeToolsContext();
	RegisterGizmos();

	bEntered = true;
	Snapshot.Lifecycle = TEXT("Entering");
	ToolsContext->ToolManager->OnToolStarted.AddUObject(this, &ThisClass::OnToolStarted);
	ToolsContext->ToolManager->OnToolEnded.AddUObject(this, &ThisClass::OnToolEnded);

	RegisterBuiltInTools();
	FString Error;
	RegisterTargetFactory(NewObject<UOWTActorToolTargetFactory>(this), TEXT("OWT.Core"), Error);
	RegisterConfiguredTools();
	if (!RegisterExtensions())
	{
		return false;
	}

	Snapshot.Lifecycle = TEXT("Entered");
	bRestoreDefault = true;
	RestoreDefaultTool();
	NotifyStateChanged();
	return bEntered;
}

void UOWTAttributeEditMode::InitializeToolsContext()
{
	ToolsContext = NewObject<UVTBOWTEditorToolsContext>(this);
	ToolsContext->InitializeContext(*Subsystem.Get());
	ToolsContext->ToolManager->ConfigureChangeTrackingMode(EToolChangeTrackingMode::NoChangeTracking);
	ToolsContext->SetSnapSettings(SnapSettings);

	SessionContext = NewObject<UOWTAttributeEditSessionContext>(this);
	SessionContext->Initialize(*this);
	ToolsContext->ContextObjectStore->AddContextObject(SessionContext);
}

void UOWTAttributeEditMode::RegisterGizmos()
{
	ToolsContext->GizmoManager->RegisterGizmoType(TEXT("OWT.AxisPosition"),
	                                              NewObject<UVTBOWTAxisPositionGizmoBuilder>(ToolsContext));
	ToolsContext->GizmoManager->RegisterGizmoType(TEXT("OWT.AxisAngle"),
	                                              NewObject<UVTBOWTAxisAngleGizmoBuilder>(ToolsContext));
	ToolsContext->GizmoManager->RegisterGizmoType(TEXT("OWT.Transform"),
	                                              NewObject<UVTBOWTBaseTransformGizmoBuilder>(ToolsContext));
	ToolsContext->GizmoManager->RegisterGizmoType(TEXT("OWT.CustomTransform"),
	                                              NewObject<UVTBOWTCustomTransformGizmoBuilder>(ToolsContext));
}

void UOWTAttributeEditMode::RegisterConfiguredTools()
{
	FString Error;
	for (const FOWTToolDescriptor& Descriptor : AdditionalTools)
	{
		FGuid Token;
		if (!RegisterTool(Descriptor, TEXT("OWT.Configuration"), Token, Error))
		{
			UE_LOG(LogTemp, Warning, TEXT("OWT tool registration: %s"), *Error);
		}
	}
}

bool UOWTAttributeEditMode::RegisterExtensions()
{
	TArray<IOWTAttributeModeExtension*> Extensions =
	    IModularFeatures::Get().GetModularFeatureImplementations<IOWTAttributeModeExtension>(
	        IOWTAttributeModeExtension::GetModularFeatureName());
	for (IOWTAttributeModeExtension* Extension : Extensions)
	{
		Extension->RegisterTools(*this);
		if (!bEntered)
		{
			// An extension can synchronously end this mode while it registers its tools.
			return false;
		}
	}
	return true;
}

void UOWTAttributeEditMode::Exit()
{
	check(IsInGameThread());
	if (bExiting)
	{
		return;
	}
	if (!bEntered)
	{
		return;
	}
	{
		TGuardValue<bool> ExitGuard(bExiting, true);
		bEntered = false;
		bRestoreDefault = false;
		Snapshot.Lifecycle = TEXT("Exiting");
		TerminateCapture();
		if (AVTBAttributeEditor* Editor = Subsystem->GetAttributeEditor())
		{
			Editor->FinishActiveOperation();
		}
		if (ToolsContext->ToolManager->HasActiveTool(EToolSide::Left))
		{
			ToolsContext->ToolManager->DeactivateTool(EToolSide::Left, EToolShutdownType::Cancel);
		}
		CancelPendingDuplicate();
		ToolsContext->ToolManager->OnToolStarted.RemoveAll(this);
		ToolsContext->ToolManager->OnToolEnded.RemoveAll(this);
		for (const FOWTRegisteredTool& Tool : RegisteredTools)
		{
			ToolsContext->ToolManager->UnregisterToolType(Tool.Descriptor.ToolId.ToString());
		}
		RegisteredTools.Reset();
		RegisteredTargets.Reset();
		TargetProviders.Reset();
		RegisteredServices.Reset();
		ServiceProviders.Reset();
		ToolsContext->ContextObjectStore->RemoveContextObject(SessionContext);
		ToolsContext->Shutdown();
		ToolsContext = nullptr;
		SessionContext = nullptr;
		CommitSelection(nullptr);
		Snapshot.ActiveToolId = NAME_None;
		Snapshot.Lifecycle = TEXT("Initialized");
		NotifyStateChanged();
	}
	if (bShutdownRequested)
	{
		Shutdown();
	}
}

void UOWTAttributeEditMode::Shutdown()
{
	check(IsInGameThread());
	if (bShuttingDown)
	{
		return;
	}
	bShutdownRequested = true;
	if (bExiting)
	{
		return;
	}
	TGuardValue<bool> ShutdownGuard(bShuttingDown, true);
	Exit();
	if (bExecutingDuplicate)
	{
		Snapshot.Lifecycle = TEXT("ShutdownRequested");
		return;
	}
	if (Duplicator)
	{
		Duplicator->OnProceduralChanged.RemoveAll(this);
		Duplicator->Deinitialize();
		Duplicator = nullptr;
	}
	bInitialized = false;
	Snapshot.Lifecycle = TEXT("Shutdown");
	NotifyStateChanged();
	Subsystem.Reset();
	bShutdownRequested = false;
}

void UOWTAttributeEditMode::Tick(float DeltaTime)
{
	if (!bInitialized)
	{
		return;
	}
	if (Duplicator)
	{
		Duplicator->Tick(DeltaTime);
	}
	if (!bEntered)
	{
		return;
	}
	if (Selection.IsStale())
	{
		SetSelectedObject(nullptr);
	}
	RestoreDefaultTool();
	if (!ToolsContext)
	{
		return;
	}
	UpdateView();
	UVTBOWTEditorToolsContext* Context = ToolsContext;
	Context->ToolManager->Tick(DeltaTime);
	if (ToolsContext != Context)
	{
		return;
	}
	Context->GizmoManager->Tick(DeltaTime);
}

bool UOWTAttributeEditMode::ReceiveEditContext_Implementation(const FInstancedStruct& Context)
{
	return false;
}

void UOWTAttributeEditMode::RegisterBuiltInTools()
{
	FString Error;
	FGuid Token;
	FOWTToolDescriptor Attribute;
	Attribute.ToolId = OWTToolIds::AttributeEdit();
	Attribute.Label = NSLOCTEXT("OWT", "AttributeTool", "Attributes");
	Attribute.Category = NSLOCTEXT("OWT", "EditCategory", "Edit");
	Attribute.BuilderClass = UOWTAttributeEditToolBuilder::StaticClass();
	RegisterTool(Attribute, TEXT("OWT.Core"), Token, Error);
	FOWTToolDescriptor Duplicate;
	Duplicate.ToolId = OWTToolIds::Duplicate();
	Duplicate.Label = NSLOCTEXT("OWT", "DuplicateTool", "Duplicate");
	Duplicate.Category = Attribute.Category;
	Duplicate.BuilderClass = UOWTDuplicateToolBuilder::StaticClass();
	RegisterTool(Duplicate, TEXT("OWT.Core"), Token, Error);
}

bool UOWTAttributeEditMode::RegisterTool(const FOWTToolDescriptor& Descriptor, FName ProviderId, FGuid& OutToken,
                                         FString& OutError)
{
	check(IsInGameThread());
	OutToken.Invalidate();
	if (!ToolsContext)
	{
		OutError = TEXT("Enter the mode before registering tools.");
		return false;
	}
	if (Descriptor.ToolId.IsNone())
	{
		OutError = TEXT("A stable tool ID is required.");
		return false;
	}
	if (ProviderId.IsNone())
	{
		OutError = TEXT("A provider ID is required.");
		return false;
	}
	if (!Descriptor.BuilderClass)
	{
		OutError = TEXT("A tool builder class is required.");
		return false;
	}
	if (Descriptor.BuilderClass->HasAnyClassFlags(CLASS_Abstract))
	{
		OutError = TEXT("The tool builder must be concrete.");
		return false;
	}
	for (const FOWTRegisteredTool& Registered : RegisteredTools)
	{
		if (Registered.Descriptor.ToolId == Descriptor.ToolId)
		{
			OutError = TEXT("The tool ID is already registered.");
			return false;
		}
	}
	FOWTRegisteredTool Entry;
	Entry.Descriptor = Descriptor;
	Entry.ProviderId = ProviderId;
	Entry.Token = FGuid::NewGuid();
	Entry.Builder = NewObject<UInteractiveToolBuilder>(this, Descriptor.BuilderClass);
	ToolsContext->ToolManager->RegisterToolType(Descriptor.ToolId.ToString(), Entry.Builder);
	OutToken = Entry.Token;
	RegisteredTools.Add(Entry);
	ProviderLeases.FindOrAdd(ProviderId).Add(Entry.Builder);
	NotifyStateChanged();
	return true;
}

bool UOWTAttributeEditMode::RegisterTargetFactory(UToolTargetFactory* Factory, FName ProviderId, FString& OutError)
{
	if (!ToolsContext)
	{
		OutError = TEXT("The mode is not entered.");
		return false;
	}
	if (!IsValid(Factory))
	{
		OutError = TEXT("A target factory is required.");
		return false;
	}
	if (Factory->GetTypedOuter<UOWTAttributeEditMode>() != this)
	{
		OutError = TEXT("The factory must belong to this mode.");
		return false;
	}
	if (RegisteredTargets.Contains(Factory))
	{
		OutError = TEXT("The factory is already registered.");
		return false;
	}
	RegisteredTargets.Add(Factory);
	TargetProviders.Add(Factory, ProviderId);
	ProviderLeases.FindOrAdd(ProviderId).Add(Factory);
	ToolsContext->TargetManager->AddTargetFactory(Factory);
	return true;
}

bool UOWTAttributeEditMode::RegisterContextObject(UObject* Service, FName ProviderId, FString& OutError)
{
	if (!ToolsContext)
	{
		OutError = TEXT("The mode is not entered.");
		return false;
	}
	if (!IsValid(Service))
	{
		OutError = TEXT("A service object is required.");
		return false;
	}
	if (ProviderId.IsNone())
	{
		OutError = TEXT("A provider ID is required.");
		return false;
	}
	if (Service->GetTypedOuter<UOWTAttributeEditMode>() != this)
	{
		OutError = TEXT("The service must belong to this mode.");
		return false;
	}
	if (ToolsContext->ContextObjectStore->FindContextByClass(Service->GetClass()))
	{
		OutError = TEXT("A service of this type is already registered.");
		return false;
	}
	if (!ToolsContext->ContextObjectStore->AddContextObject(Service))
	{
		OutError = TEXT("Context registration failed.");
		return false;
	}
	RegisteredServices.Add(Service);
	ServiceProviders.Add(Service, ProviderId);
	ProviderLeases.FindOrAdd(ProviderId).Add(Service);
	return true;
}

bool UOWTAttributeEditMode::CanUnloadProvider(FName ProviderId) const
{
	if (ProviderId == TEXT("OWT.Core"))
	{
		return false;
	}
	const TArray<TWeakObjectPtr<UObject>>* Leases = ProviderLeases.Find(ProviderId);
	if (!Leases)
	{
		return true;
	}
	for (const TWeakObjectPtr<UObject>& Lease : *Leases)
	{
		if (Lease.IsValid())
		{
			return false;
		}
	}
	return true;
}

bool UOWTAttributeEditMode::UnregisterProvider(FName ProviderId, FString& OutError)
{
	check(IsInGameThread());
	if (!ToolsContext)
	{
		OutError = TEXT("The mode is not entered.");
		return false;
	}
	if (ProviderId == TEXT("OWT.Core"))
	{
		OutError = TEXT("Core services remain registered for the entered mode.");
		return false;
	}
	if (bChangingTool)
	{
		OutError = TEXT("A tool transition is already in progress.");
		return false;
	}
	TGuardValue<bool> Transition(bChangingTool, true);
	UVTBOWTEditorToolsContext* OriginalContext = ToolsContext;
	for (const FOWTRegisteredTool& Entry : RegisteredTools)
	{
		if (Entry.ProviderId != ProviderId)
		{
			continue;
		}
		if (Entry.Descriptor.ToolId != Snapshot.ActiveToolId)
		{
			continue;
		}
		ToolsContext->ToolManager->DeactivateTool(EToolSide::Left, EToolShutdownType::Cancel);
		break;
	}
	if (ToolsContext != OriginalContext)
	{
		return true;
	}
	for (int32 Index = RegisteredTools.Num() - 1; Index >= 0; --Index)
	{
		if (RegisteredTools[Index].ProviderId != ProviderId)
		{
			continue;
		}
		ToolsContext->ToolManager->UnregisterToolType(RegisteredTools[Index].Descriptor.ToolId.ToString());
		RegisteredTools.RemoveAt(Index);
	}
	ToolsContext->TargetManager->RemoveTargetFactoriesByPredicate(
	    [this, ProviderId](UToolTargetFactory* Factory)
	    {
		    const FName* Owner = TargetProviders.Find(Factory);
		    return Owner ? *Owner == ProviderId : false;
	    });
	for (int32 Index = RegisteredTargets.Num() - 1; Index >= 0; --Index)
	{
		const FName* Owner = TargetProviders.Find(RegisteredTargets[Index]);
		if (!Owner)
		{
			continue;
		}
		if (*Owner != ProviderId)
		{
			continue;
		}
		TargetProviders.Remove(RegisteredTargets[Index]);
		RegisteredTargets.RemoveAt(Index);
	}
	for (int32 Index = RegisteredServices.Num() - 1; Index >= 0; --Index)
	{
		const FName* Owner = ServiceProviders.Find(RegisteredServices[Index]);
		if (!Owner)
		{
			continue;
		}
		if (*Owner != ProviderId)
		{
			continue;
		}
		ToolsContext->ContextObjectStore->RemoveContextObject(RegisteredServices[Index]);
		ServiceProviders.Remove(RegisteredServices[Index]);
		RegisteredServices.RemoveAt(Index);
	}
	NotifyStateChanged();
	return true;
}

TArray<FOWTToolAvailability> UOWTAttributeEditMode::GetAvailableTools() const
{
	TArray<FOWTToolAvailability> Result;
	for (const FOWTRegisteredTool& Registered : RegisteredTools)
	{
		FOWTToolAvailability Entry;
		Entry.ToolId = Registered.Descriptor.ToolId;
		Entry.Label = Registered.Descriptor.Label;
		Entry.Category = Registered.Descriptor.Category;
		Entry.bActive = Snapshot.ActiveToolId == Entry.ToolId;
		FString Reason;
		if (!bEntered)
		{
			Reason = TEXT("Editing is disabled.");
		}
		else if (HasPendingDuplicate())
		{
			Reason = TEXT("A duplicate operation is pending.");
		}
		else if (Registered.Descriptor.bRequiresHistory)
		{
			Reason = TEXT("This tool requires a runtime history provider.");
		}
		else if (Registered.Descriptor.bRequiresMeshRendering)
		{
			Reason = TEXT("This viewport supports overlay primitives and HUD, not material mesh rendering.");
		}
		else
		{
			FToolBuilderState State;
			ToolsContext->ToolManager->GetContextQueriesAPI()->GetCurrentSelectionState(State);
			Entry.bEnabled = Registered.Builder->CanBuildTool(State);
			if (!Entry.bEnabled)
			{
				Reason = TEXT("The current selection does not support this tool.");
			}
		}
		Entry.Reason = FText::FromString(Reason);
		Result.Add(Entry);
	}
	return Result;
}

bool UOWTAttributeEditMode::RequestToolStart(FName ToolId, FString& OutError)
{
	for (const FOWTRegisteredTool& Entry : RegisteredTools)
	{
		if (Entry.Descriptor.ToolId != ToolId)
		{
			continue;
		}
		if (UOWTModeToolBuilder* Builder = Cast<UOWTModeToolBuilder>(Entry.Builder))
		{
			return Builder->RequestStart(*this, ToolId, OutError);
		}
		return StartTool(ToolId, OutError);
	}
	OutError = TEXT("Unknown tool ID.");
	return false;
}

bool UOWTAttributeEditMode::StartTool(FName ToolId, FString& OutError)
{
	if (!bEntered)
	{
		OutError = TEXT("Editing is disabled.");
		return false;
	}
	if (!ToolsContext)
	{
		OutError = TEXT("The tools context is unavailable.");
		return false;
	}
	if (bChangingTool)
	{
		OutError = TEXT("A tool transition is already in progress.");
		return false;
	}
	if (bExecutingDuplicate)
	{
		OutError = TEXT("Hierarchy restoration is in progress.");
		return false;
	}
	const FOWTRegisteredTool* Entry = RegisteredTools.FindByPredicate(
	    [ToolId](const FOWTRegisteredTool& Item)
	    {
		    return Item.Descriptor.ToolId == ToolId;
	    });
	if (!Entry)
	{
		OutError = TEXT("Unknown tool ID.");
		return false;
	}
	if (Entry->Descriptor.bRequiresHistory)
	{
		OutError = TEXT("No runtime history provider is installed.");
		return false;
	}
	if (Entry->Descriptor.bRequiresMeshRendering)
	{
		OutError = TEXT("This viewport does not support material mesh drawing.");
		return false;
	}
	if (Snapshot.ActiveToolId == ToolId)
	{
		return true;
	}
	UInteractiveToolManager* Manager = ToolsContext->ToolManager;
	if (UInteractiveTool* ActiveTool = Manager->GetActiveTool(EToolSide::Left))
	{
		if (ActiveTool->HasAccept())
		{
			OutError = TEXT("Accept or cancel the current tool before starting another.");
			return false;
		}
	}
	if (!Manager->CanActivateTool(EToolSide::Left, ToolId.ToString()))
	{
		OutError = TEXT("The tool cannot be built for this selection.");
		return false;
	}
	TGuardValue<bool> Transition(bChangingTool, true);
	TerminateCapture();
	if (Manager->HasActiveTool(EToolSide::Left))
	{
		const EToolShutdownType ShutdownType =
		    Manager->CanCancelActiveTool(EToolSide::Left) ? EToolShutdownType::Cancel : EToolShutdownType::Completed;
		Manager->DeactivateTool(EToolSide::Left, ShutdownType);
	}
	if (!bEntered)
	{
		OutError = TEXT("The mode exited during tool shutdown.");
		return false;
	}
	if (!Manager->SelectActiveToolType(EToolSide::Left, ToolId.ToString()))
	{
		OutError = TEXT("Tool selection failed.");
		bRestoreDefault = true;
		return false;
	}
	if (!Manager->ActivateTool(EToolSide::Left))
	{
		OutError = TEXT("Tool construction or setup failed.");
		bRestoreDefault = true;
		return false;
	}
	if (!Manager->HasActiveTool(EToolSide::Left))
	{
		OutError = TEXT("The tool ended during setup.");
		bRestoreDefault = true;
		return false;
	}
	bRestoreDefault = false;
	return true;
}

bool UOWTAttributeEditMode::EndTool(bool bAccept, FString& OutError)
{
	if (!ToolsContext)
	{
		OutError = TEXT("The tools context is unavailable.");
		return false;
	}
	UInteractiveToolManager* Manager = ToolsContext->ToolManager;
	if (!Manager->HasActiveTool(EToolSide::Left))
	{
		OutError = TEXT("No tool is active.");
		return false;
	}
	if (bAccept)
	{
		if (!CanAcceptActiveTool())
		{
			OutError = TEXT("The active tool cannot be accepted.");
			return false;
		}
	}
	else
	{
		if (!CanCancelActiveTool())
		{
			OutError = TEXT("The active tool cannot be cancelled.");
			return false;
		}
	}
	Manager->DeactivateTool(EToolSide::Left, bAccept ? EToolShutdownType::Accept : EToolShutdownType::Cancel);
	return true;
}

bool UOWTAttributeEditMode::CanAcceptActiveTool() const
{
	return ToolsContext ? ToolsContext->ToolManager->CanAcceptActiveTool(EToolSide::Left) : false;
}

bool UOWTAttributeEditMode::CanCancelActiveTool() const
{
	return ToolsContext ? ToolsContext->ToolManager->CanCancelActiveTool(EToolSide::Left) : false;
}

void UOWTAttributeEditMode::OnToolStarted(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	Snapshot.ActiveToolId = FName(*Manager->GetActiveToolName(EToolSide::Left));
	for (const FOWTRegisteredTool& Entry : RegisteredTools)
	{
		if (Entry.Descriptor.ToolId != Snapshot.ActiveToolId)
		{
			continue;
		}
		ProviderLeases.FindOrAdd(Entry.ProviderId).Add(Tool);
		break;
	}
	Snapshot.DisabledReason.Reset();
	NotifyStateChanged();
}

void UOWTAttributeEditMode::OnToolEnded(UInteractiveToolManager* Manager, UInteractiveTool* Tool)
{
	Snapshot.ActiveToolId = NAME_None;
	bRestoreDefault = bEntered;
	NotifyStateChanged();
}

void UOWTAttributeEditMode::RestoreDefaultTool()
{
	if (!bRestoreDefault)
	{
		return;
	}
	if (!bEntered)
	{
		return;
	}
	if (bChangingTool)
	{
		return;
	}
	if (!ToolsContext)
	{
		return;
	}
	if (ToolsContext->ToolManager->HasActiveTool(EToolSide::Left))
	{
		bRestoreDefault = false;
		return;
	}
	bRestoreDefault = false;
	FString Error;
	if (!StartTool(DefaultToolId, Error))
	{
		Snapshot.DisabledReason = Error;
		NotifyStateChanged();
	}
}

UOWTAttributeEditTool* UOWTAttributeEditMode::GetAttributeTool() const
{
	return ToolsContext ? Cast<UOWTAttributeEditTool>(ToolsContext->ToolManager->GetActiveTool(EToolSide::Left))
	                    : nullptr;
}

bool UOWTAttributeEditMode::ApplyTransform(AActor& Actor, const FTransform& Transform, FString& OutError)
{
	UOWTAttributeEditTool* Tool = GetAttributeTool();
	if (!Tool)
	{
		OutError = TEXT("The attribute tool is not active.");
		return false;
	}
	return Tool->ApplyTransform(Actor, Transform, OutError);
}

bool UOWTAttributeEditMode::BeginDuplicateOperation(AActor* Actor, const FOWTDuplicationOptions& Options,
                                                    const FString& RequestId, const FString& Source,
                                                    FGuid& OutOperationId, FString& OutError)
{
	OutOperationId.Invalidate();
	if (bDispatchingDuplicate)
	{
		OutError = TEXT("Busy: retry duplication after the current completion callback returns.");
		return false;
	}
	if (!bEntered)
	{
		OutError = TEXT("Editing is disabled.");
		return false;
	}
	if (!IsValid(Actor))
	{
		OutError = TEXT("A live actor is required.");
		return false;
	}
	if (Actor->GetWorld() != GetWorld())
	{
		OutError = TEXT("The source belongs to another world.");
		return false;
	}
	if (Selection.Get() != Actor)
	{
		OutError = TEXT("The source is no longer selected.");
		return false;
	}
	if (HasPendingDuplicate())
	{
		OutError = TEXT("A duplicate operation is already pending.");
		return false;
	}
	if (bExecutingDuplicate)
	{
		OutError = TEXT("A duplicate operation is restoring its hierarchy.");
		return false;
	}
	for (const FOWTDuplicationOperationSnapshot& Previous : Operations)
	{
		if (RequestId.IsEmpty())
		{
			break;
		}
		if (Previous.RequestId == RequestId)
		{
			OutError = TEXT("The request ID was already processed.");
			return false;
		}
	}
	PendingOperation = FGuid::NewGuid();
	PendingSource = Actor;
	PendingOptions = MakeShared<FOWTDuplicationOptions>(Options);
	const TWeakObjectPtr<UOWTAttributeEditMode> WeakMode(this);
	const FGuid ExpectedOperation = PendingOperation;
	const TWeakObjectPtr<AActor> ExpectedSource(Actor);
	PendingOptions->IsCancellationRequested = [WeakMode, ExpectedOperation, ExpectedSource]()
	{
		if (!WeakMode.IsValid())
		{
			return true;
		}
		if (!WeakMode->IsEntered())
		{
			return true;
		}
		if (WeakMode->PendingOperation != ExpectedOperation)
		{
			return true;
		}
		if (!ExpectedSource.IsValid())
		{
			return true;
		}
		if (ExpectedSource->IsActorBeingDestroyed())
		{
			return true;
		}
		if (WeakMode->Selection != ExpectedSource)
		{
			return true;
		}
		return false;
	};
	PendingOptions->OnAuthoredCommitted = [WeakMode, ExpectedOperation](AActor* Result)
	{
		if (!WeakMode.IsValid())
		{
			return;
		}
		WeakMode->PendingOperation.Invalidate();
		WeakMode->PendingOptions.Reset();
		WeakMode->PendingSource.Reset();
		WeakMode->UpdateOperation(ExpectedOperation, EOWTDuplicationPhase::Committed, FString(), Result);
	};
	FOWTDuplicationOperationSnapshot Operation;
	Operation.SourceActor = Actor;
	Operation.OperationId = PendingOperation;
	Operation.RequestId = RequestId;
	Operation.Source = Source;
	Operation.HierarchyScope = FName(
	    *StaticEnum<EOWTDuplicationHierarchyScope>()->GetNameStringByValue(static_cast<int64>(Options.HierarchyScope)));
	Operation.Revision = 1;
	Operations.Add(Operation);
	if (Operations.Num() > 256)
	{
		Operations.RemoveAt(0);
	}
	OutOperationId = PendingOperation;
	const FGuid AcceptedId = PendingOperation;
	OnDuplicationChanged.Broadcast(Operation);
	if (!bEntered)
	{
		CancelPendingDuplicate();
		OutError = TEXT("The mode exited while accepting the request.");
		return false;
	}
	if (PendingOperation != AcceptedId)
	{
		OutError = TEXT("The request was cancelled while being accepted.");
		return false;
	}
	if (!PendingSource.IsValid())
	{
		CancelPendingDuplicate();
		OutError = TEXT("The source was destroyed while accepting the request.");
		return false;
	}
	if (Selection != PendingSource)
	{
		CancelPendingDuplicate();
		OutError = TEXT("The selection changed while accepting the request.");
		return false;
	}
	if (!StartTool(OWTToolIds::Duplicate(), OutError))
	{
		const FGuid FailedOperation = PendingOperation;
		PendingOperation.Invalidate();
		PendingOptions.Reset();
		PendingSource.Reset();
		UpdateOperation(FailedOperation, EOWTDuplicationPhase::Failed, OutError);
		return false;
	}
	return true;
}

bool UOWTAttributeEditMode::HasPendingDuplicate() const
{
	return PendingOperation.IsValid();
}

void UOWTAttributeEditMode::SetOperationObjectIds(const FGuid& OperationId, const FString& OriginalId,
                                                  const FString& DuplicateId)
{
	for (FOWTDuplicationOperationSnapshot& Operation : Operations)
	{
		if (Operation.OperationId != OperationId)
		{
			continue;
		}
		Operation.OriginalObjectId = OriginalId;
		Operation.DuplicateObjectId = DuplicateId;
		return;
	}
}

void UOWTAttributeEditMode::ExecutePendingDuplicate(UOWTDuplicateTool& Tool)
{
	TGuardValue<bool> DispatchGuard(bDispatchingDuplicate, true);
	auto EndCancelledTool = [this, &Tool]()
	{
		if (!ToolsContext)
		{
			return;
		}
		UInteractiveToolManager* Manager = ToolsContext->ToolManager;
		if (Manager->GetActiveTool(EToolSide::Left) != &Tool)
		{
			return;
		}
		Manager->PostActiveToolShutdownRequest(&Tool, EToolShutdownType::Cancel);
	};
	if (!HasPendingDuplicate())
	{
		EndCancelledTool();
		return;
	}
	const FGuid OperationId = PendingOperation;
	const TSharedPtr<FOWTDuplicationOptions> Options = PendingOptions;
	const TWeakObjectPtr<AActor> Source = PendingSource;
	auto CanContinue = [this, OperationId, Options, Source, &Tool]()
	{
		if (!bEntered)
		{
			return false;
		}
		if (PendingOperation != OperationId)
		{
			return false;
		}
		if (PendingOptions != Options)
		{
			return false;
		}
		if (!Source.IsValid())
		{
			return false;
		}
		if (Source->IsActorBeingDestroyed())
		{
			return false;
		}
		if (Selection != Source)
		{
			return false;
		}
		if (!ToolsContext)
		{
			return false;
		}
		if (ToolsContext->ToolManager->GetActiveTool(EToolSide::Left) != &Tool)
		{
			return false;
		}
		return true;
	};
	UpdateOperation(OperationId, EOWTDuplicationPhase::Planning);
	if (!CanContinue())
	{
		CancelPendingDuplicate();
		EndCancelledTool();
		return;
	}
	FString Error;
	UpdateOperation(OperationId, EOWTDuplicationPhase::Restoring);
	if (!CanContinue())
	{
		CancelPendingDuplicate();
		EndCancelledTool();
		return;
	}
	AActor* Duplicate;
	{
		TGuardValue<bool> Executing(bExecutingDuplicate, true);
		Duplicate = Duplicator->DuplicateActorWithOptions(Source.Get(), *Options, OperationId, Error);
	}
	const FOWTDuplicationOperationSnapshot* Observed = Operations.FindByPredicate(
	    [OperationId](const FOWTDuplicationOperationSnapshot& Item)
	    {
		    return Item.OperationId == OperationId;
	    });
	const bool bCommitted = Observed ? Observed->Phase == EOWTDuplicationPhase::Committed : false;
	const bool bCancelled = !CanContinue();
	PendingOperation.Invalidate();
	PendingOptions.Reset();
	PendingSource.Reset();
	if (bCommitted)
	{
		// The service announced commit before scheduling generation. Later mode exit
		// cannot turn an authored result into a cancelled provisional operation.
	}
	else if (bCancelled)
	{
		UpdateOperation(OperationId, EOWTDuplicationPhase::Cancelled, Error);
	}
	else if (IsValid(Duplicate))
	{
		UpdateOperation(OperationId, EOWTDuplicationPhase::Committed, FString(), Duplicate);
	}
	else
	{
		UpdateOperation(OperationId, EOWTDuplicationPhase::Failed, Error);
	}
	if (Snapshot.Lifecycle == TEXT("ShutdownRequested"))
	{
		Shutdown();
		return;
	}
	if (!ToolsContext)
	{
		return;
	}
	UInteractiveToolManager* Manager = ToolsContext->ToolManager;
	if (Manager->GetActiveTool(EToolSide::Left) != &Tool)
	{
		return;
	}
	Manager->PostActiveToolShutdownRequest(&Tool, Duplicate ? EToolShutdownType::Completed : EToolShutdownType::Cancel);
}

void UOWTAttributeEditMode::CancelPendingDuplicate()
{
	if (!HasPendingDuplicate())
	{
		return;
	}
	// Synchronous restore owns its stack; it must finish before its result can be released.
	const FGuid OperationId = PendingOperation;
	PendingOperation.Invalidate();
	PendingOptions.Reset();
	PendingSource.Reset();
	if (bExecutingDuplicate)
	{
		UpdateOperation(OperationId, EOWTDuplicationPhase::CleaningUp,
		                TEXT("Cancellation was requested during hierarchy restoration."));
		return;
	}
	UpdateOperation(OperationId, EOWTDuplicationPhase::Cancelled,
	                TEXT("The duplicate operation was cancelled before commit."));
}

void UOWTAttributeEditMode::UpdateOperation(const FGuid& OperationId, EOWTDuplicationPhase Phase, const FString& Error,
                                            AActor* Result)
{
	for (FOWTDuplicationOperationSnapshot& Operation : Operations)
	{
		if (Operation.OperationId != OperationId)
		{
			continue;
		}
		Operation.Phase = Phase;
		Operation.Error = Error;
		Operation.DuplicateActor = Result;
		++Operation.Revision;
		const FOWTDuplicationOperationSnapshot Copy = Operation;
		TGuardValue<bool> DispatchGuard(bDispatchingDuplicate, true);
		OnDuplicationChanged.Broadcast(Copy);
		return;
	}
}

TArray<FOWTProceduralComponentSnapshot> UOWTAttributeEditMode::GetProceduralComponents() const
{
	return Duplicator ? Duplicator->GetProceduralComponents() : TArray<FOWTProceduralComponentSnapshot>();
}

void UOWTAttributeEditMode::ForwardProcedural(const FOWTProceduralComponentSnapshot& State)
{
	OnProceduralChanged.Broadcast(State);
}

bool UOWTAttributeEditMode::SetSelectedObject(AActor* Actor)
{
	if (Actor)
	{
		if (!IsValid(Actor))
		{
			return false;
		}
		if (Actor->IsActorBeingDestroyed())
		{
			return false;
		}
		if (Actor->GetWorld() != GetWorld())
		{
			return false;
		}
	}
	if (UOWTAttributeEditTool* Tool = GetAttributeTool())
	{
		return Tool->SelectActor(Actor);
	}
	CommitSelection(Actor);
	return true;
}

void UOWTAttributeEditMode::CommitSelection(AActor* Actor)
{
	if (Selection.Get() == Actor)
	{
		if (!Selection.IsStale())
		{
			return;
		}
	}
	if (HasPendingDuplicate())
	{
		CancelPendingDuplicate();
	}
	Selection = Actor;
	if (!Subsystem.IsValid())
	{
		return;
	}
	if (Subsystem->GetAttributeEditMode() != this)
	{
		return;
	}
	Subsystem->SelectedObject = Selection;
	if (AVTBAttributeEditor* Editor = Subsystem->GetAttributeEditor())
	{
		Editor->NotifySelectionChanged();
	}
	NotifyStateChanged();
}

void UOWTAttributeEditMode::ShowSelectionGizmo()
{
	if (UOWTAttributeEditTool* Tool = GetAttributeTool())
	{
		Tool->ShowGizmo();
	}
}

void UOWTAttributeEditMode::HideSelectionGizmo()
{
	if (UOWTAttributeEditTool* Tool = GetAttributeTool())
	{
		Tool->HideGizmo();
	}
}

void UOWTAttributeEditMode::SynchronizeSelectionGizmo()
{
	if (UOWTAttributeEditTool* Tool = GetAttributeTool())
	{
		Tool->SynchronizeGizmo();
	}
}

bool UOWTAttributeEditMode::HasCapture() const
{
	return ToolsContext ? ToolsContext->InputRouter->HasActiveMouseCapture() : false;
}

void UOWTAttributeEditMode::TerminateCapture()
{
	if (ToolsContext)
	{
		ToolsContext->InputRouter->ForceTerminateAll();
	}
}

bool UOWTAttributeEditMode::RoutePointer(const FOWTGizmoPointerContext& Pointer)
{
	if (!bEntered)
	{
		return false;
	}
	if (!ToolsContext)
	{
		return false;
	}
	UpdateView();
	FInputDeviceState Input;
	Input.InputDevice = EInputDevices::Mouse;
	Input.Mouse.WorldRay = FRay(Pointer.RayOrigin, Pointer.RayDirection);
	Input.Mouse.Position2D = Pointer.ScreenPosition;
	Input.Mouse.Left.SetStates(Pointer.bPressed, Pointer.bDown, Pointer.bReleased);
	bool bButtonEvent = Pointer.bPressed;
	if (Pointer.bDown)
	{
		bButtonEvent = true;
	}
	if (Pointer.bReleased)
	{
		bButtonEvent = true;
	}
	if (bButtonEvent)
	{
		ToolsContext->InputRouter->PostInputEvent(Input);
	}
	else
	{
		ToolsContext->InputRouter->PostHoverInputEvent(Input);
	}
	return true;
}

void UOWTAttributeEditMode::SetCoordinateSystem(EToolContextCoordinateSystem System)
{
	TerminateCapture();
	if (Subsystem.IsValid())
	{
		if (AVTBAttributeEditor* Editor = Subsystem->GetAttributeEditor())
		{
			Editor->FinishActiveOperation();
		}
	}
	CoordinateSystem = System;
	NotifyStateChanged();
}

void UOWTAttributeEditMode::SetTransformGizmoMode(EToolContextTransformGizmoMode Mode)
{
	TerminateCapture();
	if (Subsystem.IsValid())
	{
		if (AVTBAttributeEditor* Editor = Subsystem->GetAttributeEditor())
		{
			Editor->FinishActiveOperation();
		}
	}
	GizmoMode = Mode;
	if (UOWTAttributeEditTool* Tool = GetAttributeTool())
	{
		if (Tool->GetGizmo())
		{
			Tool->GetGizmo()->Tick(0.f);
		}
	}
	NotifyStateChanged();
}

bool UOWTAttributeEditMode::SetSnapSettings(const FOWTGizmoSnapSettings& Settings)
{
	if (!FMath::IsFinite(Settings.TranslationStep))
	{
		return false;
	}
	if (Settings.TranslationStep <= UE_SMALL_NUMBER)
	{
		return false;
	}
	if (!FMath::IsFinite(Settings.RotationStepDegrees))
	{
		return false;
	}
	if (Settings.RotationStepDegrees <= UE_SMALL_NUMBER)
	{
		return false;
	}
	if (!FMath::IsFinite(Settings.ScaleStep))
	{
		return false;
	}
	if (Settings.ScaleStep <= UE_SMALL_NUMBER)
	{
		return false;
	}
	if (ToolsContext)
	{
		if (!ToolsContext->SetSnapSettings(Settings))
		{
			return false;
		}
	}
	SnapSettings = Settings;
	return true;
}

void UOWTAttributeEditMode::UpdateView()
{
	if (!ToolsContext)
	{
		return;
	}
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	ULocalPlayer* Player = Controller ? Controller->GetLocalPlayer() : nullptr;
	UGameViewportClient* Viewport = Player ? Player->ViewportClient.Get() : nullptr;
	if (!Viewport)
	{
		return;
	}
	if (!Viewport->Viewport)
	{
		return;
	}
	FSceneViewFamilyContext Family(
	    FSceneViewFamily::ConstructionValues(Viewport->Viewport, GetWorld()->Scene, Viewport->EngineShowFlags)
	        .SetRealtimeUpdate(true));
	FVector Location;
	FRotator Rotation;
	const FSceneView* View = Player->CalcSceneView(&Family, Location, Rotation, Viewport->Viewport);
	if (!View)
	{
		return;
	}
	if (UGizmoViewContext* ViewContext = ToolsContext->ContextObjectStore->FindContext<UGizmoViewContext>())
	{
		ViewContext->ResetFromSceneView(*View);
	}
}

void UOWTAttributeEditMode::RenderTools(UCanvas* Canvas, APlayerController* Controller)
{
	if (!bEntered)
	{
		return;
	}
	if (!ToolsContext)
	{
		return;
	}
	OWTRuntimeToolsViewportBridge::Draw(*ToolsContext, Canvas, Controller, GetWorld());
}

void UOWTAttributeEditMode::NotifyStateChanged()
{
	if (Snapshot.Lifecycle == TEXT("Entering"))
	{
		return;
	}
	Snapshot.bEditingEnabled = bEntered;
	Snapshot.bCanStartTools = bEntered;
	if (HasPendingDuplicate())
	{
		Snapshot.bCanStartTools = false;
	}
	++Snapshot.Revision;
	const FOWTModeSnapshot Copy = Snapshot;
	OnModeChanged.Broadcast(Copy);
	if (Subsystem.IsValid())
	{
		if (AVTBAttributeEditor* Editor = Subsystem->GetAttributeEditor())
		{
			Editor->NotifyEditorStateChanged();
		}
	}
}
