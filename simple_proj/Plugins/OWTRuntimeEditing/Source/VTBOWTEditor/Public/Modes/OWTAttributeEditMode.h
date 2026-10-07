#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OWTEditContextReceiver.h"
#include "Extensions/OWTToolDescriptor.h"
#include "State/OWTEditingSessionTypes.h"
#include "ToolContextInterfaces.h"
#include "Context/OWTGizmoSnapSettings.h"
#include "OWTAttributeEditMode.generated.h"

class UVTBOWTEditorSubsystem;
class UVTBOWTEditorToolsContext;
class UOWTAttributeEditSessionContext;
class UOWTAttributeEditTool;
class UOWTDuplicateTool;
class UOWTRuntimeActorDuplicator;
class UInteractiveTool;
class UInteractiveToolManager;
class UToolTargetFactory;
class UCanvas;
struct FOWTDuplicationOptions;
struct FOWTGizmoPointerContext;

USTRUCT()
struct FOWTRegisteredTool
{
	GENERATED_BODY()

	FOWTRegisteredTool() : Descriptor(), Builder(nullptr), ProviderId(NAME_None), Token()
	{
	}

	UPROPERTY()
	FOWTToolDescriptor Descriptor;

	UPROPERTY()
	TObjectPtr<UInteractiveToolBuilder> Builder;

	UPROPERTY()
	FName ProviderId;

	UPROPERTY()
	FGuid Token;
};

UCLASS(BlueprintType, Blueprintable)
class VTBOWTEDITOR_API UOWTAttributeEditMode : public UObject, public IOWTEditContextReceiver
{
	GENERATED_BODY()

public:
	UOWTAttributeEditMode();

	virtual UWorld* GetWorld() const override;
	virtual bool ReceiveEditContext_Implementation(const FInstancedStruct& Context) override;

	bool Initialize(UVTBOWTEditorSubsystem& InSubsystem);
	bool Enter();
	void Tick(float DeltaTime);
	void Exit();
	void Shutdown();

	bool RegisterTool(const FOWTToolDescriptor& Descriptor, FName ProviderId, FGuid& OutToken, FString& OutError);
	bool RegisterTargetFactory(UToolTargetFactory* Factory, FName ProviderId, FString& OutError);
	bool RegisterContextObject(UObject* Service, FName ProviderId, FString& OutError);
	bool UnregisterProvider(FName ProviderId, FString& OutError);
	bool CanUnloadProvider(FName ProviderId) const;

	bool StartTool(FName ToolId, FString& OutError);
	bool RequestToolStart(FName ToolId, FString& OutError);
	bool EndTool(bool bAccept, FString& OutError);
	bool CanAcceptActiveTool() const;
	bool CanCancelActiveTool() const;
	bool ApplyTransform(AActor& Actor, const FTransform& Transform, FString& OutError);

	bool BeginDuplicateOperation(AActor* Actor, const FOWTDuplicationOptions& Options, const FString& RequestId,
	                             const FString& Source, FGuid& OutOperationId, FString& OutError);
	void ExecutePendingDuplicate(UOWTDuplicateTool& Tool);
	void CancelPendingDuplicate();
	bool HasPendingDuplicate() const;
	void SetOperationObjectIds(const FGuid& OperationId, const FString& OriginalId, const FString& DuplicateId);

	void CommitSelection(AActor* Actor);
	void ShowSelectionGizmo();
	void SynchronizeSelectionGizmo();
	void HideSelectionGizmo();
	bool RoutePointer(const FOWTGizmoPointerContext& Pointer);
	bool HasCapture() const;
	void TerminateCapture();
	void RenderTools(UCanvas* Canvas, APlayerController* Controller);
	void NotifyStateChanged();

	bool IsEntered() const
	{
		return bEntered;
	}

	bool IsInitialized() const
	{
		return bInitialized;
	}

	UVTBOWTEditorSubsystem* GetSubsystem() const
	{
		return Subsystem.Get();
	}

	UVTBOWTEditorToolsContext* GetToolsContext() const
	{
		return ToolsContext;
	}

	UOWTAttributeEditTool* GetAttributeTool() const;
	TArray<FOWTToolAvailability> GetAvailableTools() const;
	TArray<FOWTProceduralComponentSnapshot> GetProceduralComponents() const;

	FOWTModeSnapshot GetSnapshot() const
	{
		return Snapshot;
	}

	TArray<FOWTDuplicationOperationSnapshot> GetDuplicationOperations() const
	{
		return Operations;
	}

	bool SetSelectedObject(AActor* Actor);

	AActor* GetSelectedObject() const
	{
		return Selection.Get();
	}

	void SetCoordinateSystem(EToolContextCoordinateSystem System);

	EToolContextCoordinateSystem GetCoordinateSystem() const
	{
		return CoordinateSystem;
	}

	void SetTransformGizmoMode(EToolContextTransformGizmoMode Mode);

	EToolContextTransformGizmoMode GetTransformGizmoMode() const
	{
		return GizmoMode;
	}

	bool SetSnapSettings(const FOWTGizmoSnapSettings& Settings);

	FOWTGizmoSnapSettings GetSnapSettings() const
	{
		return SnapSettings;
	}

private:
	void InitializeToolsContext();
	void RegisterGizmos();
	void RegisterBuiltInTools();
	void RegisterConfiguredTools();
	bool RegisterExtensions();
	void RestoreDefaultTool();

	void UpdateView();
	void UpdateOperation(const FGuid& OperationId, EOWTDuplicationPhase Phase, const FString& Error = FString(),
	                     AActor* Result = nullptr);
	void ForwardProcedural(const FOWTProceduralComponentSnapshot& State);

	void OnToolStarted(UInteractiveToolManager* Manager, UInteractiveTool* Tool);
	void OnToolEnded(UInteractiveToolManager* Manager, UInteractiveTool* Tool);

public:
	FOWTModeChanged OnModeChanged;
	FOWTDuplicationChanged OnDuplicationChanged;
	FOWTProceduralChanged OnProceduralChanged;

	UPROPERTY(EditDefaultsOnly, Category = "OWT|Tools")
	FName DefaultToolId;

	UPROPERTY(EditDefaultsOnly, Category = "OWT|Tools")
	TArray<FOWTToolDescriptor> AdditionalTools;

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UVTBOWTEditorSubsystem> Subsystem;
	UPROPERTY(Transient)
	TObjectPtr<UVTBOWTEditorToolsContext> ToolsContext;
	UPROPERTY(Transient)
	TObjectPtr<UOWTAttributeEditSessionContext> SessionContext;
	UPROPERTY(Transient)
	TObjectPtr<UOWTRuntimeActorDuplicator> Duplicator;
	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> Selection;
	UPROPERTY(Transient)
	TArray<FOWTRegisteredTool> RegisteredTools;
	UPROPERTY(Transient)
	TArray<TObjectPtr<UToolTargetFactory>> RegisteredTargets;
	TMap<TWeakObjectPtr<UToolTargetFactory>, FName> TargetProviders;
	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> RegisteredServices;
	TMap<TWeakObjectPtr<UObject>, FName> ServiceProviders;
	TMap<FName, TArray<TWeakObjectPtr<UObject>>> ProviderLeases;
	UPROPERTY(Transient)
	TArray<FOWTDuplicationOperationSnapshot> Operations;
	UPROPERTY(Transient)
	FOWTModeSnapshot Snapshot;
	UPROPERTY(Transient)
	FOWTGizmoSnapSettings SnapSettings;
	TSharedPtr<FOWTDuplicationOptions> PendingOptions;
	FGuid PendingOperation;
	TWeakObjectPtr<AActor> PendingSource;
	EToolContextCoordinateSystem CoordinateSystem;
	EToolContextTransformGizmoMode GizmoMode;
	bool bInitialized;
	bool bEntered;
	bool bChangingTool;
	bool bRestoreDefault;
	bool bExecutingDuplicate;
	bool bExiting;
	bool bShuttingDown;
	bool bShutdownRequested;
	bool bDispatchingDuplicate;
};
