#if WITH_DEV_AUTOMATION_TESTS

#include "RuntimeEditor/Context/VTBEditorInteractiveToolsContext.h"
#include "RuntimeEditor/Gizmo/VTBEditorTransformGizmo.h"
#include "BaseGizmos/AxisAngleGizmo.h"
#include "BaseGizmos/AxisPositionGizmo.h"
#include "BaseGizmos/GizmoViewContext.h"
#include "BaseGizmos/HitTargets.h"
#include "BaseGizmos/ViewBasedTransformAdjusters.h"
#include "BaseGizmos/ViewAdjustedStaticMeshGizmoComponent.h"
#include "Components/SceneComponent.h"
#include "ContextObjectStore.h"
#include "Engine/World.h"
#include "InputRouter.h"
#include "InteractiveGizmoManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "Math/OrthoMatrix.h"
#include "SceneView.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBEditorGizmoLifecycleTest,
	"VTB.RuntimeGizmo.BuilderCompositionAndLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBEditorGizmoLifecycleTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	UVTBEditorInteractiveToolsContext* Context = NewObject<UVTBEditorInteractiveToolsContext>(World);
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		World->DestroyWorld(false);
	};
	if (!TestTrue(TEXT("Runtime context initializes without an editor context"), Context->InitializeRuntime(World)))
	{
		return false;
	}

	UVTBEditorTransformGizmoBuilder* Builder = NewObject<UVTBEditorTransformGizmoBuilder>(Context->GizmoManager);
	Builder->EnabledElements = ETransformGizmoSubElements::TranslateAxisX | ETransformGizmoSubElements::RotateAxisZ;
	Context->GizmoManager->RegisterGizmoType(UVTBEditorTransformGizmoBuilder::BuilderIdentifier, Builder);
	UVTBEditorTransformGizmo* Gizmo = Cast<UVTBEditorTransformGizmo>(Context->GizmoManager->CreateGizmo(
		UVTBEditorTransformGizmoBuilder::BuilderIdentifier, TEXT("LifecycleTest"), Context));
	if (!TestNotNull(TEXT("Registered builder creates our runtime gizmo"), Gizmo))
	{
		return false;
	}
	ACombinedTransformGizmoActor* GizmoActor = Gizmo->GetGizmoActor();
	if (!TestNotNull(TEXT("Factory creates the stock visual actor"), GizmoActor))
	{
		return false;
	}
	TestNotNull(TEXT("Requested translation handle is created"), GizmoActor->TranslateX.Get());
	TestNotNull(TEXT("Requested rotation handle is created"), GizmoActor->RotateZ.Get());
	TestNull(TEXT("Unrequested handles cannot become hit targets"), GizmoActor->TranslateY.Get());
	TestNull(TEXT("Unrequested scale handles are absent"), GizmoActor->UniformScale.Get());
	TestFalse(TEXT("Visual actor is never replicated"), GizmoActor->GetIsReplicated());
	if (const UViewAdjustedStaticMeshGizmoComponent* Arrow = Cast<UViewAdjustedStaticMeshGizmoComponent>(GizmoActor->TranslateX))
	{
		TestTrue(TEXT("Stock visual handle uses an ITF mesh"), IsValid(Arrow->GetStaticMesh()));
		TestTrue(TEXT("Component is registered for runtime rendering"), Arrow->IsRegistered());
	}

	AActor* TargetActor = World->SpawnActor<AActor>();
	USceneComponent* Root = NewObject<USceneComponent>(TargetActor);
	TargetActor->SetRootComponent(Root);
	Root->SetMobility(EComponentMobility::Movable);
	Root->RegisterComponent();
	UTransformProxy* Proxy = NewObject<UTransformProxy>(Context);
	Proxy->AddComponent(Root);
	Gizmo->SetActiveTarget(Proxy, Context->GizmoManager);
	Gizmo->SetVisibility(true);
	TestEqual(TEXT("Combined gizmo binds the transform proxy"), Gizmo->ActiveTarget.Get(), Proxy);
	TestFalse(TEXT("Bound gizmo can be displayed in a game world"), GizmoActor->IsHidden());
	TestTrue(TEXT("Manager owns the registered instance"), Context->GizmoManager->FindGizmoByInstanceIdentifier(TEXT("LifecycleTest")) == Gizmo);
	TestTrue(TEXT("Manager destroys the complete gizmo tree"), Context->GizmoManager->DestroyGizmo(Gizmo));
	TestNull(TEXT("Destroyed instance is removed from manager"), Context->GizmoManager->FindGizmoByInstanceIdentifier(TEXT("LifecycleTest")));
	TestFalse(TEXT("Destroying gizmo destroys its visual actor"), IsValid(GizmoActor));
	TestTrue(TEXT("Destroying gizmo preserves the edited actor"), IsValid(TargetActor));
	TestNull(TEXT("Shutdown releases active proxy reference"), Gizmo->ActiveTarget.Get());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBEditorGizmoBehaviorTest,
	"VTB.RuntimeGizmo.CapturePolicyAndTermination",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBEditorGizmoBehaviorTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	UVTBEditorInteractiveToolsContext* Context = NewObject<UVTBEditorInteractiveToolsContext>(World);
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		World->DestroyWorld(false);
	};
	if (!TestTrue(TEXT("Runtime context initializes"), Context->InitializeRuntime(World)))
	{
		return false;
	}
	UVTBEditorTransformGizmoBuilder* Builder = NewObject<UVTBEditorTransformGizmoBuilder>(Context->GizmoManager);
	Builder->EnabledElements = ETransformGizmoSubElements::TranslateAxisX;
	Context->GizmoManager->RegisterGizmoType(UVTBEditorTransformGizmoBuilder::BuilderIdentifier, Builder);
	UVTBEditorTransformGizmo* Gizmo = Cast<UVTBEditorTransformGizmo>(Context->GizmoManager->CreateGizmo(
		UVTBEditorTransformGizmoBuilder::BuilderIdentifier));
	if (!TestNotNull(TEXT("Runtime combined gizmo is created"), Gizmo))
	{
		return false;
	}
	AActor* TargetActor = World->SpawnActor<AActor>();
	USceneComponent* Root = NewObject<USceneComponent>(TargetActor);
	TargetActor->SetRootComponent(Root);
	Root->SetMobility(EComponentMobility::Movable);
	Root->RegisterComponent();
	UTransformProxy* Proxy = NewObject<UTransformProxy>(Context);
	Proxy->AddComponent(Root);
	Gizmo->SetActiveTarget(Proxy, Context->GizmoManager);
	const TArray<UInteractiveGizmo*> Children = Context->GizmoManager->FindAllGizmosOfType(
		UInteractiveGizmoManager::DefaultAxisPositionBuilderIdentifier);
	UAxisPositionGizmo* Axis = Children.Num() == 1 ? Cast<UAxisPositionGizmo>(Children[0]) : nullptr;
	if (!TestNotNull(TEXT("Combined gizmo creates one stock axis child"), Axis))
	{
		return false;
	}
	// Isolate capture policy from camera geometry: a deterministic hit still uses the real
	// engine behavior, router, axis interaction and ForceEndCapture implementation.
	UGizmoLambdaHitTarget* HitTarget = NewObject<UGizmoLambdaHitTarget>(Axis);
	HitTarget->IsHitFunction = [](const FInputDeviceRay&) { return FInputRayHit(10.0); };
	Axis->HitTarget = HitTarget;
	FInputDeviceState Press;
	Press.InputDevice = EInputDevices::Mouse;
	Press.Mouse.Left.SetStates(true, true, false);
	Press.Mouse.WorldRay = FRay(FVector(100, 100, 100), FVector(-1, -1, -1).GetSafeNormal());
	Press.bAltKeyDown = true;
	Context->InputRouter->PostInputEvent(Press);
	TestFalse(TEXT("Alt camera gesture does not capture the gizmo"), Context->InputRouter->HasActiveMouseCapture());
	TestFalse(TEXT("Rejected input does not begin a transform"), Axis->bInInteraction);

	Press.bAltKeyDown = false;
	Context->InputRouter->PostInputEvent(Press);
	TestTrue(TEXT("Plain left drag is captured by ITF"), Context->InputRouter->HasActiveMouseCapture());
	TestTrue(TEXT("Accepted input begins axis interaction"), Axis->bInInteraction);
	Context->InputRouter->ForceTerminateAll();
	TestFalse(TEXT("Focus-loss termination releases mouse capture"), Context->InputRouter->HasActiveMouseCapture());
	TestFalse(TEXT("Force termination reaches the sub-gizmo"), Axis->bInInteraction);

	Context->InputRouter->PostInputEvent(Press);
	TestTrue(TEXT("A new drag can start after termination"), Context->InputRouter->HasActiveMouseCapture());
	Press.Mouse.Left.SetStates(false, false, true);
	Context->InputRouter->PostInputEvent(Press);
	TestFalse(TEXT("Mouse release ends capture"), Context->InputRouter->HasActiveMouseCapture());
	TestFalse(TEXT("Mouse release ends interaction"), Axis->bInInteraction);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBEditorGizmoAnalyticDragTest,
	"VTB.RuntimeGizmo.AnalyticHitDragUndoAndCancel",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBEditorGizmoAnalyticDragTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	UVTBEditorInteractiveToolsContext* Context = NewObject<UVTBEditorInteractiveToolsContext>(World);
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		World->DestroyWorld(false);
	};
	if (!TestTrue(TEXT("Runtime context initializes"), Context->InitializeRuntime(World)))
	{
		return false;
	}

	// Orthographic camera looking along +Z: 1000 world units cover 1000 pixels.
	// This exercises the real component's camera-dependent hit path without a viewport window.
	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(nullptr, World->Scene, FEngineShowFlags(ESFIM_Game)));
	FSceneViewInitOptions ViewOptions;
	ViewOptions.ViewFamily = &ViewFamily;
	ViewOptions.SetViewRectangle(FIntRect(0, 0, 1000, 1000));
	ViewOptions.ViewOrigin = FVector(0, 0, -500);
	ViewOptions.ViewLocation = ViewOptions.ViewOrigin;
	ViewOptions.ViewRotationMatrix = FMatrix::Identity;
	ViewOptions.ProjectionMatrix = FReversedZOrthoMatrix(500.0, 500.0, 1.0 / 10000.0, 0.0);
	const FSceneView View(ViewOptions);
	UGizmoViewContext* ViewContext = Context->ContextObjectStore
		? Context->ContextObjectStore->FindContext<UGizmoViewContext>() : nullptr;
	if (!TestNotNull(TEXT("Stock gizmo view context"), ViewContext))
	{
		return false;
	}
	ViewContext->ResetFromSceneView(View);
	Context->SetCoordinateSystem(EToolContextCoordinateSystem::World);
	Context->SetGizmoMode(EToolContextTransformGizmoMode::Translation);

	UVTBEditorTransformGizmoBuilder* Builder = NewObject<UVTBEditorTransformGizmoBuilder>(Context->GizmoManager);
	Builder->EnabledElements = ETransformGizmoSubElements::TranslateAxisX;
	Context->GizmoManager->RegisterGizmoType(UVTBEditorTransformGizmoBuilder::BuilderIdentifier, Builder);
	UVTBEditorTransformGizmo* Gizmo = Cast<UVTBEditorTransformGizmo>(Context->GizmoManager->CreateGizmo(
		UVTBEditorTransformGizmoBuilder::BuilderIdentifier));
	if (!TestNotNull(TEXT("Custom gizmo is created"), Gizmo))
	{
		return false;
	}
	AActor* Actor = World->SpawnActor<AActor>();
	USceneComponent* Root = NewObject<USceneComponent>(Actor);
	Actor->SetRootComponent(Root);
	Root->SetMobility(EComponentMobility::Movable);
	Root->RegisterComponent();
	UTransformProxy* Proxy = NewObject<UTransformProxy>(Context);
	Proxy->AddComponent(Root);
	Gizmo->SetActiveTarget(Proxy, Context->GizmoManager);
	Gizmo->bSnapToWorldGrid = false;
	Context->TickRuntime(0.0f);
	Gizmo->SetVisibility(false);
	// UE 5.7 binds hover on the sub-gizmo hit target, bypassing the old combined-builder callback.
	const TArray<UInteractiveGizmo*> Children = Context->GizmoManager->FindAllGizmosOfType(
		UInteractiveGizmoManager::DefaultAxisPositionBuilderIdentifier);
	UAxisPositionGizmo* Axis = Children.Num() == 1 ? Cast<UAxisPositionGizmo>(Children[0]) : nullptr;
	UGizmoComponentHitTarget* HitTarget = Axis ? Cast<UGizmoComponentHitTarget>(Axis->HitTarget.GetObject()) : nullptr;
	if (!TestNotNull(TEXT("Stock analytic hit target"), HitTarget)) { return false; }
	TSharedRef<int32> HoverUpdates = MakeShared<int32>(0);
	HitTarget->UpdateHoverFunction = [HoverUpdates, OriginalHover = HitTarget->UpdateHoverFunction](bool bHovering)
	{
		++*HoverUpdates;
		if (OriginalHover) { OriginalHover(bHovering); }
	};

	const auto MakePointer = [](double X, bool bPressed, bool bDown, bool bReleased)
	{
		FInputDeviceState Input;
		Input.InputDevice = EInputDevices::Mouse;
		Input.Mouse.Position2D = FVector2D(500.0 + X, 500.0);
		Input.Mouse.WorldRay = FRay(FVector(X, 0, -500), FVector::ZAxisVector);
		Input.Mouse.Left.SetStates(bPressed, bDown, bReleased);
		return Input;
	};
	Context->InputRouter->PostInputEvent(MakePointer(40.0, true, true, false));
	TestFalse(TEXT("Hidden visual actor cannot capture"), Context->InputRouter->HasActiveMouseCapture());
	Gizmo->SetVisibility(true);
	Context->InputRouter->PostInputEvent(MakePointer(40.0, false, false, false));
	TestEqual(TEXT("One stock input event updates hover once without a separate hover dispatch"), *HoverUpdates, 1);
	TestFalse(TEXT("Hover alone does not start a drag"), Context->InputRouter->HasActiveMouseCapture());
	Context->InputRouter->PostInputEvent(MakePointer(40.0, true, true, false));
	if (!TestTrue(TEXT("Visible analytic arrow captures despite disabled physics collision"), Context->InputRouter->HasActiveMouseCapture()))
	{
		return false;
	}
	Context->InputRouter->PostInputEvent(MakePointer(75.0, false, true, false));
	TestTrue(TEXT("Dragging the arrow moves the actor along X"), Actor->GetActorLocation().Equals(FVector(35, 0, 0), 0.01));
	Context->InputRouter->PostInputEvent(MakePointer(75.0, false, false, true));
	TestFalse(TEXT("Release ends analytic drag capture"), Context->InputRouter->HasActiveMouseCapture());
	TestTrue(TEXT("Released drag is undoable"), Context->CanUndo());
	TestTrue(TEXT("Runtime undo succeeds"), Context->Undo());
	TestTrue(TEXT("Undo restores edited component"), Actor->GetActorLocation().IsNearlyZero(0.01));
	TestTrue(TEXT("Undo restores gizmo location"), Gizmo->GetGizmoTransform().GetLocation().IsNearlyZero(0.01));

	Context->InputRouter->PostInputEvent(MakePointer(40.0, true, true, false));
	TestTrue(TEXT("Restored arrow can capture again"), Context->InputRouter->HasActiveMouseCapture());
	Context->InputRouter->PostInputEvent(MakePointer(65.0, false, true, false));
	TestTrue(TEXT("Second drag changes the transform"), Actor->GetActorLocation().Equals(FVector(25, 0, 0), 0.01));
	Context->CancelActiveInteraction();
	TestFalse(TEXT("Cancel releases capture"), Context->InputRouter->HasActiveMouseCapture());
	TestTrue(TEXT("Cancel restores component to pre-drag transform"), Actor->GetActorLocation().IsNearlyZero(0.01));
	TestTrue(TEXT("Cancel restores gizmo to pre-drag transform"), Gizmo->GetGizmoTransform().GetLocation().IsNearlyZero(0.01));
	TestFalse(TEXT("Cancelled drag does not add undo history"), Context->CanUndo());
	TestTrue(TEXT("Cancelled drag preserves prior redo history"), Context->CanRedo());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBEditorGizmoModeVisibilityTest,
	"VTB.RuntimeGizmo.ModeVisibilityAndRotationAxisFocus",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBEditorGizmoModeVisibilityTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	UVTBEditorInteractiveToolsContext* Context = NewObject<UVTBEditorInteractiveToolsContext>(World);
	ON_SCOPE_EXIT
	{
		Context->Shutdown();
		World->DestroyWorld(false);
	};
	if (!TestTrue(TEXT("Runtime context initializes"), Context->InitializeRuntime(World)))
	{
		return false;
	}

	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(nullptr, World->Scene, FEngineShowFlags(ESFIM_Game)));
	FSceneViewInitOptions ViewOptions;
	ViewOptions.ViewFamily = &ViewFamily;
	ViewOptions.SetViewRectangle(FIntRect(0, 0, 1000, 1000));
	ViewOptions.ViewOrigin = FVector(0, 0, -500);
	ViewOptions.ViewLocation = ViewOptions.ViewOrigin;
	ViewOptions.ViewRotationMatrix = FMatrix::Identity;
	ViewOptions.ProjectionMatrix = FReversedZOrthoMatrix(500.0, 500.0, 1.0 / 10000.0, 0.0);
	const FSceneView View(ViewOptions);
	UGizmoViewContext* ViewContext = Context->ContextObjectStore
		? Context->ContextObjectStore->FindContext<UGizmoViewContext>() : nullptr;
	if (!TestNotNull(TEXT("Stock gizmo view context"), ViewContext))
	{
		return false;
	}
	ViewContext->ResetFromSceneView(View);

	UVTBEditorTransformGizmoBuilder* Builder = NewObject<UVTBEditorTransformGizmoBuilder>(Context->GizmoManager);
	Builder->EnabledElements = ETransformGizmoSubElements::TranslateAllAxes
		| ETransformGizmoSubElements::RotateAllAxes
		| ETransformGizmoSubElements::ScaleAllAxes
		| ETransformGizmoSubElements::ScaleUniform;
	Context->GizmoManager->RegisterGizmoType(UVTBEditorTransformGizmoBuilder::BuilderIdentifier, Builder);
	UVTBEditorTransformGizmo* Gizmo = Cast<UVTBEditorTransformGizmo>(Context->GizmoManager->CreateGizmo(
		UVTBEditorTransformGizmoBuilder::BuilderIdentifier));
	if (!TestNotNull(TEXT("Custom gizmo is created"), Gizmo))
	{
		return false;
	}

	AActor* Actor = World->SpawnActor<AActor>();
	USceneComponent* Root = NewObject<USceneComponent>(Actor);
	Actor->SetRootComponent(Root);
	Root->SetMobility(EComponentMobility::Movable);
	Root->RegisterComponent();
	Root->SetWorldRotation(FRotator(0.0, 45.0, 0.0));
	UTransformProxy* Proxy = NewObject<UTransformProxy>(Context);
	Proxy->AddComponent(Root);
	Gizmo->SetActiveTarget(Proxy, Context->GizmoManager);
	Gizmo->SetVisibility(true);

	ACombinedTransformGizmoActor* GizmoActor = Gizmo->GetGizmoActor();
	if (!TestNotNull(TEXT("Stock visual actor"), GizmoActor))
	{
		return false;
	}

	Context->SetGizmoMode(EToolContextTransformGizmoMode::Translation);
	Gizmo->Tick(0.0f);
	TestTrue(TEXT("Translation mode shows translation handles"), GizmoActor->TranslateX->IsVisible());
	TestFalse(TEXT("Translation mode hides rotation handles"), GizmoActor->RotateX->IsVisible());
	TestFalse(TEXT("Translation mode hides scale handles"), GizmoActor->UniformScale->IsVisible());

	Context->SetGizmoMode(EToolContextTransformGizmoMode::Rotation);
	Gizmo->Tick(0.0f);
	TestFalse(TEXT("Rotation mode hides translation handles"), GizmoActor->TranslateX->IsVisible());
	TestTrue(TEXT("Rotation mode shows X rotation"), GizmoActor->RotateX->IsVisible());
	TestTrue(TEXT("Rotation mode shows Y rotation"), GizmoActor->RotateY->IsVisible());
	TestTrue(TEXT("Rotation mode shows Z rotation"), GizmoActor->RotateZ->IsVisible());
	if (GizmoActor->RotationSphere)
	{
		TestTrue(TEXT("Rotation mode shows the editor-style outer ring"), GizmoActor->RotationSphere->IsVisible());
	}
	TestFalse(TEXT("Rotation mode hides scale handles"), GizmoActor->UniformScale->IsVisible());

	Context->SetGizmoMode(EToolContextTransformGizmoMode::Scale);
	Gizmo->Tick(0.0f);
	TestFalse(TEXT("Scale mode hides translation handles"), GizmoActor->TranslateX->IsVisible());
	TestFalse(TEXT("Scale mode hides rotation handles"), GizmoActor->RotateX->IsVisible());
	TestTrue(TEXT("Scale mode shows scale handles"), GizmoActor->UniformScale->IsVisible());

	static const FName FullScaleAxisMeshName(TEXT("GizmoBoxArrowHandle"));
	UViewAdjustedStaticMeshGizmoComponent* FullScaleX = nullptr;
	TArray<UViewAdjustedStaticMeshGizmoComponent*> ViewAdjustedComponents;
	GizmoActor->GetComponents(ViewAdjustedComponents);
	for (UViewAdjustedStaticMeshGizmoComponent* Component : ViewAdjustedComponents)
	{
		const UStaticMesh* Mesh = Component ? Component->GetStaticMesh() : nullptr;
		if (Mesh && Mesh->GetFName() == FullScaleAxisMeshName)
		{
			FullScaleX = Component;
			break;
		}
	}
	if (!TestNotNull(TEXT("Scale mode uses the stock full X axis mesh"), FullScaleX))
	{
		return false;
	}
	TestTrue(TEXT("Scale mode shows a full axis scale handle"), FullScaleX->IsVisible());
	if (!TestTrue(TEXT("Full X scale axis has a view adjuster"), FullScaleX->GetTransformAdjuster().IsValid()))
	{
		return false;
	}

	Context->SetCoordinateSystem(EToolContextCoordinateSystem::Local);
	Gizmo->Tick(0.0f);
	const FVector LocalScaleAxis = FullScaleX->GetTransformAdjuster()
		->GetAdjustedComponentToWorld(*ViewContext, FullScaleX->GetComponentTransform())
		.GetRotation()
		.GetAxisX();

	Context->SetCoordinateSystem(EToolContextCoordinateSystem::World);
	Gizmo->Tick(0.0f);
	const FVector WorldScaleAxis = FullScaleX->GetTransformAdjuster()
		->GetAdjustedComponentToWorld(*ViewContext, FullScaleX->GetComponentTransform())
		.GetRotation()
		.GetAxisX();
	TestFalse(TEXT("Full X scale axis follows Local/World coordinate changes"),
		LocalScaleAxis.Equals(WorldScaleAxis, 0.01));

	Context->SetGizmoMode(EToolContextTransformGizmoMode::Rotation);
	Gizmo->Tick(0.0f);
	const TArray<UInteractiveGizmo*> RotationGizmos = Context->GizmoManager->FindAllGizmosOfType(
		UInteractiveGizmoManager::DefaultAxisAngleBuilderIdentifier);
	UAxisAngleGizmo* ActiveRotation = RotationGizmos.IsEmpty() ? nullptr : Cast<UAxisAngleGizmo>(RotationGizmos[0]);
	if (!TestNotNull(TEXT("Stock rotation child gizmo"), ActiveRotation))
	{
		return false;
	}

	ActiveRotation->bInInteraction = true;
	Gizmo->Tick(0.0f);
	int32 VisibleRotationAxes = 0;
	for (UPrimitiveComponent* Component : { GizmoActor->RotateX.Get(), GizmoActor->RotateY.Get(), GizmoActor->RotateZ.Get() })
	{
		if (Component->IsVisible())
		{
			++VisibleRotationAxes;
		}
	}
	TestEqual(TEXT("Active rotation drag isolates one axis"), VisibleRotationAxes, 1);
	if (GizmoActor->RotationSphere)
	{
		TestFalse(TEXT("Active rotation drag hides the outer ring"), GizmoActor->RotationSphere->IsVisible());
	}

	ActiveRotation->bInInteraction = false;
	Gizmo->Tick(0.0f);
	TestTrue(TEXT("Ending rotation restores X axis"), GizmoActor->RotateX->IsVisible());
	TestTrue(TEXT("Ending rotation restores Y axis"), GizmoActor->RotateY->IsVisible());
	TestTrue(TEXT("Ending rotation restores Z axis"), GizmoActor->RotateZ->IsVisible());
	if (GizmoActor->RotationSphere)
	{
		TestTrue(TEXT("Ending rotation restores the outer ring"), GizmoActor->RotationSphere->IsVisible());
	}
	return true;
}

#endif
