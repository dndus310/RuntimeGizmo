#if WITH_DEV_AUTOMATION_TESTS
#include "Tests/OWTModeTestTypes.h"
#include "Misc/AutomationTest.h"
#include "Context/OWTAttributeEditSessionContext.h"
#include "Context/OWTEditContexts.h"
#include "Context/VTBOWTEditorToolsContext.h"
#include "ContextObjectStore.h"
#include "Duplication/OWTDuplicationRequest.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/Canvas.h"
#include "Engine/TextureRenderTarget2D.h"
#include "CanvasTypes.h"
#include "GameFramework/PlayerController.h"
#include "RenderingThread.h"
#include "RHI.h"
#include "SceneView.h"
#include "InteractiveToolManager.h"
#include "Modes/OWTAttributeEditMode.h"
#include "Targets/OWTActorToolTarget.h"
#include "ToolTargetManager.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorSubsystem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTModeContractTest, "OWT.Runtime.ModeLifecycle",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTModeContractTest::RunTest(const FString& Parameters)
{
	const UWorld::InitializationValues Values = UWorld::InitializationValues()
	                                                .AllowAudioPlayback(false)
	                                                .CreatePhysicsScene(true)
	                                                .CreateNavigation(false)
	                                                .CreateAISystem(false)
	                                                .ShouldSimulatePhysics(false);
	UWorld* World =
	    UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, &Values);
	if (!TestNotNull(TEXT("World"), World))
	{
		return false;
	}
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	UVTBOWTEditorSubsystem* Hub = World->GetSubsystem<UVTBOWTEditorSubsystem>();
	AVTBAttributeEditor* Editor = World->SpawnActor<AVTBAttributeEditor>();
	Editor->BindSubsystem(Hub);
	UOWTAttributeEditMode* Mode = Hub->GetAttributeEditMode();
	TestTrue(TEXT("World initializes the mode without activating tools"), Mode->IsInitialized());
	TestFalse(TEXT("Editing starts off"), Mode->IsEntered());
	for (int32 Iteration = 0; Iteration < 3; ++Iteration)
	{
		TestTrue(TEXT("Mode enters"), Mode->Enter());
		TestEqual(TEXT("Default tool actually active"), Mode->GetSnapshot().ActiveToolId, OWTToolIds::AttributeEdit());
		TestEqual(TEXT("No registration accumulation"), Mode->GetAvailableTools().Num(), 2);
		UOWTAttributeEditSessionContext* Session =
		    Mode->GetToolsContext()->ContextObjectStore->FindContext<UOWTAttributeEditSessionContext>();
		TestTrue(TEXT("Context provides exact owning session"), Session->GetMode() == Mode);
		Mode->Exit();
		TestNull(TEXT("Exit releases tools context"), Mode->GetToolsContext());
		TestTrue(TEXT("Exit keeps session initialized"), Mode->IsInitialized());
	}
	Mode->Enter();
	FString Error;
	FGuid Token;
	FOWTToolDescriptor Descriptor;
	Descriptor.ToolId = TEXT("Contract.Tool");
	Descriptor.Label = FText::FromString(TEXT("Contract tool"));
	Descriptor.BuilderClass = UOWTModeTestBuilder::StaticClass();
	TestTrue(TEXT("External descriptor registration"),
	         Mode->RegisterTool(Descriptor, TEXT("Contract.Provider"), Token, Error));
	TestFalse(TEXT("Duplicate ID rejected"), Mode->RegisterTool(Descriptor, TEXT("Other"), Token, Error));
	TestTrue(TEXT("Registered tool starts through ToolManager"), Mode->StartTool(Descriptor.ToolId, Error));
	UOWTModeTestTool* Tool =
	    Cast<UOWTModeTestTool>(Mode->GetToolsContext()->ToolManager->GetActiveTool(EToolSide::Left));
	TestNotNull(TEXT("Builder created requested tool"), Tool);
	FOWTGizmoPointerContext Pointer;
	Pointer.RayOrigin = FVector::ZeroVector;
	Pointer.RayDirection = FVector::ForwardVector;
	Pointer.bPressed = true;
	Pointer.bDown = true;
	Mode->RoutePointer(Pointer);
	TestTrue(TEXT("Tool input behavior owns capture"), Mode->HasCapture());
	Pointer.bPressed = false;
	Pointer.bDown = false;
	Pointer.bReleased = true;
	Mode->RoutePointer(Pointer);
	if (Tool)
	{
		TestEqual(TEXT("InputRouter delivers click to active tool"), Tool->ClickCount, 1);
	}
	if (!GUsingNullRHI)
	{
		UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>();
		RenderTarget->InitAutoFormat(256, 256);
		FTextureRenderTargetResource* Resource = RenderTarget->GameThread_GetRenderTargetResource();
		FCanvas DrawCanvas(Resource, nullptr, World, World->GetFeatureLevel());
		FSceneViewFamilyContext Family(
		    FSceneViewFamily::ConstructionValues(Resource, World->Scene, FEngineShowFlags(ESFIM_Game)));
		FSceneViewInitOptions ViewOptions;
		ViewOptions.ViewFamily = &Family;
		ViewOptions.SetViewRectangle(FIntRect(0, 0, 256, 256));
		ViewOptions.ViewOrigin = FVector::ZeroVector;
		ViewOptions.ViewRotationMatrix = FMatrix::Identity;
		ViewOptions.ProjectionMatrix = FMatrix::Identity;
		FSceneView View(ViewOptions);
		UCanvas* Canvas = NewObject<UCanvas>();
		Canvas->Init(256, 256, &View, &DrawCanvas);
		APlayerController* Controller = World->SpawnActor<APlayerController>();
		Mode->RenderTools(Canvas, Controller);
		if (Tool)
		{
			TestEqual(TEXT("Runtime bridge calls Tool Render with PDI"), Tool->RenderCount, 1);
			TestEqual(TEXT("Runtime bridge calls Tool DrawHUD"), Tool->HUDCount, 1);
		}
		DrawCanvas.Flush_GameThread();
		FlushRenderingCommands();
	}
	else
	{
		AddInfo(TEXT("Render-target drawing requires an RHI; lifecycle/input/registry checks continue under NullRHI."));
	}
	TestTrue(TEXT("Active tool acceptance"), Mode->CanAcceptActiveTool());
	TestTrue(TEXT("Active tool ends"), Mode->EndTool(true, Error));
	Mode->Tick(0.f);
	TestEqual(TEXT("Default tool restored on later tick"), Mode->GetSnapshot().ActiveToolId,
	          OWTToolIds::AttributeEdit());
	const TArray<TSubclassOf<UInteractiveToolBuilder>> FailureBuilders = {UOWTModeRejectBuilder::StaticClass(),
	                                                                      UOWTModeNullBuilder::StaticClass(),
	                                                                      UOWTModeSetupCancelBuilder::StaticClass()};
	for (int32 Index = 0; Index < FailureBuilders.Num(); ++Index)
	{
		Descriptor.ToolId = FName(*FString::Printf(TEXT("Contract.Failure%d"), Index));
		Descriptor.BuilderClass = FailureBuilders[Index];
		Mode->RegisterTool(Descriptor, TEXT("Contract.Provider"), Token, Error);
		TestFalse(TEXT("Rejected/null/setup-cancel tool never reports activation success"),
		          Mode->StartTool(Descriptor.ToolId, Error));
		Mode->Tick(0.f);
		TestEqual(TEXT("Failure restores or retains default tool"), Mode->GetSnapshot().ActiveToolId,
		          OWTToolIds::AttributeEdit());
	}
	Descriptor.ToolId = TEXT("Contract.History");
	Descriptor.BuilderClass = UOWTModeTestBuilder::StaticClass();
	Descriptor.bRequiresHistory = true;
	Mode->RegisterTool(Descriptor, TEXT("Contract.Provider"), Token, Error);
	TestFalse(TEXT("History requirement is enforced"), Mode->StartTool(Descriptor.ToolId, Error));
	AActor* Actor = World->SpawnActor<AActor>();
	UToolTarget* Target = Mode->GetToolsContext()->TargetManager->BuildTarget(Actor, FToolTargetTypeRequirements());
	TestTrue(TEXT("Registered actor factory builds a rootless target"), Cast<UOWTActorToolTarget>(Target) != nullptr);
	TestTrue(TEXT("Provider removes its tools"), Mode->UnregisterProvider(TEXT("Contract.Provider"), Error));
	TestFalse(TEXT("Provider cannot unload while its retired UObject instances remain alive"),
	          Mode->CanUnloadProvider(TEXT("Contract.Provider")));
	TestEqual(TEXT("Only default providers remain"), Mode->GetAvailableTools().Num(), 2);
	APlayerController* UnsupportedSource = World->SpawnActor<APlayerController>();
	TestNotNull(TEXT("Concrete unsupported source fixture"), UnsupportedSource);
	Mode->SetSelectedObject(UnsupportedSource);
	FOWTDuplicationOptions RetryOptions;
	FGuid FailureId;
	bool bSawFailure = false;
	bool bNestedRetryAccepted = true;
	const FDelegateHandle FailureCallback = Mode->OnDuplicationChanged.AddLambda(
	    [this, Mode, Actor, &RetryOptions, &bSawFailure,
	     &bNestedRetryAccepted](const FOWTDuplicationOperationSnapshot& State)
	    {
		    if (State.Phase != EOWTDuplicationPhase::Failed)
		    {
			    return;
		    }
		    if (bSawFailure)
		    {
			    return;
		    }
		    bSawFailure = true;
		    TestFalse(TEXT("Terminal failure clears pending state before notification"), Mode->HasPendingDuplicate());
		    Mode->SetSelectedObject(Actor);
		    FGuid NestedId;
		    FString NestedError;
		    bNestedRetryAccepted = Mode->BeginDuplicateOperation(Actor, RetryOptions, TEXT("Contract.NestedRetry"),
		                                                         TEXT("Contract"), NestedId, NestedError);
		    TestTrue(TEXT("Completion callback receives an explicit busy reason"), NestedError.Contains(TEXT("Busy")));
	    });
	TestTrue(TEXT("Unsupported source request reaches deferred preflight"),
	         Mode->BeginDuplicateOperation(UnsupportedSource, RetryOptions, TEXT("Contract.Failure"), TEXT("Contract"),
	                                       FailureId, Error));
	Mode->Tick(0.f);
	TestTrue(TEXT("Deferred preflight reports failure"), bSawFailure);
	TestFalse(TEXT("A completion callback cannot attach a retry to the retiring duplicate tool"), bNestedRetryAccepted);
	Mode->OnDuplicationChanged.Remove(FailureCallback);
	Mode->Tick(0.f);
	FGuid RetryId;
	TestTrue(TEXT("A retry starts after the retiring tool is cleaned up"),
	         Mode->BeginDuplicateOperation(Actor, RetryOptions, TEXT("Contract.LaterRetry"), TEXT("Contract"), RetryId,
	                                       Error));
	Mode->Tick(0.f);
	const TArray<FOWTDuplicationOperationSnapshot> RetryOperations = Mode->GetDuplicationOperations();
	const FOWTDuplicationOperationSnapshot* RetryOperation = RetryOperations.FindByPredicate(
	    [RetryId](const FOWTDuplicationOperationSnapshot& State)
	    {
		    return State.OperationId == RetryId;
	    });
	if (TestNotNull(TEXT("Retry operation is retained"), RetryOperation))
	{
		TestTrue(TEXT("The later retry reaches authored commit"),
		         RetryOperation->Phase == EOWTDuplicationPhase::Committed);
	}
	Mode->Exit();
	TestFalse(TEXT("Off mode rejects start"), Mode->StartTool(OWTToolIds::AttributeEdit(), Error));
	Mode->Enter();
	bool bExitCallbackRan = false;
	const FDelegateHandle ExitCallback = Mode->OnModeChanged.AddLambda(
	    [this, Mode, &bExitCallbackRan](const FOWTModeSnapshot& State)
	    {
		    if (State.Lifecycle != TEXT("Exiting"))
		    {
			    return;
		    }
		    if (bExitCallbackRan)
		    {
			    return;
		    }
		    bExitCallbackRan = true;
		    TestFalse(TEXT("A shutdown callback cannot re-enter an exiting mode"), Mode->Enter());
		    Mode->Shutdown();
	    });
	Mode->Exit();
	Mode->OnModeChanged.Remove(ExitCallback);
	TestTrue(TEXT("Tool-end state callback ran inside Exit"), bExitCallbackRan);
	TestFalse(TEXT("Deferred reentrant shutdown ends the session"), Mode->IsInitialized());
	TestNull(TEXT("Deferred shutdown releases Context once"), Mode->GetToolsContext());
	TestNull(TEXT("Deferred shutdown releases Subsystem reference"), Mode->GetSubsystem());
	TestEqual(TEXT("Exit cannot overwrite terminal Shutdown state"), Mode->GetSnapshot().Lifecycle,
	          FName(TEXT("Shutdown")));
	World->DestroyWorld(false);
	GEngine->DestroyWorldContext(World);
	return true;
}
#endif
