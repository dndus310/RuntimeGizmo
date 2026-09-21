#pragma once

#include "CoreMinimal.h"
#include "Context/IVTBOWTEditorContextExecution.h"
#include "InteractiveToolsContext.h"
#include "ToolContextInterfaces.h"
#include "VTBOWTEditorToolsContext.generated.h"

class FVTBOWTEditorToolsContextRenderImpl;
class FVTBOWTEditorToolsContextQueriesImpl;
class FVTBOWTEditorToolsContextTransactionImpl;
class FVTBOWTEditorToolsContextInput;
class FVTBOWTEditorToolsContextViewport;
class FVTBOWTEditorSceneState;
class IVTBOWTEditorInput;
class IVTBOWTEditorViewport;
class IVTBOWTEditorSceneState;
class IVTBOWTEditorUndoRedo;
class IVTBOWTEditorTransactionHistory;

DECLARE_DELEGATE_RetVal_OneParam(bool, FVTBOWTToolsSelectionChangeRequest, const FSelectedObjectsChangeList&);

UINTERFACE()
class VTBOWTEDITOR_API UVTBOWTEditorToolsContextProvider : public UInterface
{
	GENERATED_BODY()
};

class VTBOWTEDITOR_API IVTBOWTEditorToolsContextProvider
{
	GENERATED_BODY()

public:
	virtual IToolsContextRenderAPI* GetContextRenderAPI() = 0;
	virtual IToolsContextQueriesAPI* GetContextQueriesAPI() = 0;
	virtual IToolsContextTransactionsAPI* GetContextTransactionAPI() = 0;
};

UCLASS()
class VTBOWTEDITOR_API UVTBOWTEditorToolsContext : public UInteractiveToolsContext,
	public IVTBOWTEditorToolsContextProvider, public IVTBOWTEditorContextExecution
{
	GENERATED_BODY()

public:
	UVTBOWTEditorToolsContext();
	UVTBOWTEditorToolsContext(FVTableHelper& Helper);
	virtual ~UVTBOWTEditorToolsContext() override;

	virtual void Initialize(IToolsContextQueriesAPI* InQueriesAPI = nullptr, IToolsContextTransactionsAPI* InTransactionsAPI = nullptr) override;
	bool InitializeContext(UWorld* InWorld);
	virtual void Shutdown() override;
	virtual void BeginDestroy() override;

	virtual IToolsContextRenderAPI* GetContextRenderAPI() override;
	virtual IToolsContextQueriesAPI* GetContextQueriesAPI() override;
	virtual IToolsContextTransactionsAPI* GetContextTransactionAPI() override;

	virtual bool IsRuntimeReady() const override;
	UWorld* GetEditingWorld() const;
	void TickRuntime(float DeltaTime);
	virtual bool RunContextUpdate(TFunctionRef<void()> Action) override;

	IVTBOWTEditorInput& GetInput() const;
	IVTBOWTEditorViewport& GetViewport() const;
	IVTBOWTEditorSceneState& GetSceneState() const;
	IVTBOWTEditorUndoRedo& GetUndoRedo() const;
	FVTBOWTToolsSelectionChangeRequest OnSelectionChangeRequested;

private:
	friend class FVTBOWTEditorToolsContextInput;
	friend class FVTBOWTEditorToolsContextViewport;
	friend class FVTBOWTEditorSceneState;

	void InitializeInternal(UWorld* InWorld, IToolsContextQueriesAPI* InQueriesAPI, IToolsContextTransactionsAPI* InTransactionsAPI);
	void RunGuardedContextUpdate(TFunctionRef<void()> Action);
	void FinishPendingShutdown();
	void UpdateRenderView(const FSceneView* View, FPrimitiveDrawInterface* PDI);
	void ResetRenderView();

	TUniquePtr<IVTBOWTEditorTransactionHistory> TransactionHistory;
	TUniquePtr<IVTBOWTEditorSceneState> SceneState;
	TUniquePtr<FVTBOWTEditorToolsContextRenderImpl> ContextRenderAPI;
	TUniquePtr<FVTBOWTEditorToolsContextQueriesImpl> ContextQueriesAPI;
	TUniquePtr<FVTBOWTEditorToolsContextTransactionImpl> ContextTransactionAPI;
	TUniquePtr<FVTBOWTEditorToolsContextInput> ContextInput;
	TUniquePtr<FVTBOWTEditorToolsContextViewport> ContextViewport;
	bool bUpdating = false;
	bool bShutdownRequested = false;
};
