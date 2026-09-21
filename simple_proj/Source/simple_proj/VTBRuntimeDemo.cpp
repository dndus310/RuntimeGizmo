#include "VTBRuntimeDemo.h"

#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Context/VTBOWTEditorToolsContext.h"
#include "Context/IVTBOWTEditorInput.h"
#include "Context/IVTBOWTEditorUndoRedo.h"
#include "Context/IVTBOWTEditorViewport.h"
#include "Engine/Canvas.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "VTBOWTEditorModeSubsystem.h"

AVTBRuntimeDemo::AVTBRuntimeDemo()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVTBRuntimeDemo::BeginPlay()
{
	Super::BeginPlay();
	UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (!Cube)
	{
		return;
	}
	for (int32 Index = 0; Index < 3; ++Index)
	{
		FActorSpawnParameters Params;
		Params.Name = FName(*FString::Printf(TEXT("RuntimeDemo_%s"), Index == 0 ? TEXT("CubeA")
			: Index == 1 ? TEXT("CubeB") : TEXT("Floor")));
		if (Index < 2)
		{
			AActor* Actor = GetWorld()->SpawnActor<AActor>(Params);
			USplineComponent* Spline = NewObject<USplineComponent>(Actor, TEXT("SplineRoot"));
			Spline->SetMobility(EComponentMobility::Movable);
			Actor->SetRootComponent(Spline);
			Actor->AddInstanceComponent(Spline);
			Spline->RegisterComponent();
			Actor->SetActorLocation(FVector(0, Index == 0 ? -150 : 150, 1030));
			Actor->Tags.Add(TEXT("VTBRuntimeDemo"));

			const auto AddMesh = [Actor, Spline, Cube](FName Name, const FTransform& RelativeTransform)
			{
				UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(Actor, Name);
				Mesh->SetMobility(EComponentMobility::Movable);
				Mesh->SetupAttachment(Spline);
				Mesh->SetRelativeTransform(RelativeTransform);
				Mesh->SetStaticMesh(Cube);
				Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
				Actor->AddInstanceComponent(Mesh);
				Mesh->RegisterComponent();
			};
			AddMesh(TEXT("TargetMesh"), FTransform(FRotator(10, Index == 0 ? 25 : -30, 5),
				FVector(45, 0, 35), FVector(1.2)));
			AddMesh(TEXT("SiblingMarker"), FTransform(FRotator::ZeroRotator,
				FVector(-80, 0, -30), FVector(0.3, 0.6, 0.3)));
		}
		else
		{
			AStaticMeshActor* Actor = GetWorld()->SpawnActor<AStaticMeshActor>(Params);
			UStaticMeshComponent* Mesh = Actor->GetStaticMeshComponent();
			Mesh->SetMobility(EComponentMobility::Movable);
			Mesh->SetStaticMesh(Cube);
			Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
			Actor->SetActorLocation(FVector(0, 0, 970));
			Actor->SetActorScale3D(FVector(12, 12, .1));
			Mesh->SetMobility(EComponentMobility::Static);
		}
	}
}

void AVTBRuntimeDemo::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!bPositionedCamera && PC && PC->GetPawn())
	{
		const FVector Eye(-680, -800, 1550);
		PC->GetPawn()->SetActorLocation(Eye);
		PC->SetControlRotation((FVector(0, 0, 1050) - Eye).Rotation());
		bPositionedCamera = true;
		SetActorTickEnabled(false);
	}
}

void AVTBRuntimeDemoHUD::DrawHUD()
{
	Super::DrawHUD();
	if (!Canvas)
	{
		return;
	}
	DrawRect(FLinearColor(0.01f, 0.015f, 0.025f, 0.85f), 12, 12, 1040, 254);
	DrawText(TEXT("COMPONENT FRAME DEMO | click a large cube or its small sibling marker"), FLinearColor::White, 24, 20);
	DrawText(TEXT("W / E / R : Move / Rotate / Scale   Ctrl+Z / Ctrl+Y : Undo / Redo"), FLinearColor::White, 24, 42);
	DrawText(TEXT("Ctrl+~ : Local / World   Esc : Clear   RMB + WASD/QE : Fly camera"), FLinearColor::White, 24, 64);
	UVTBOWTEditorModeSubsystem* Runtime = GetWorld()->GetSubsystem<UVTBOWTEditorModeSubsystem>();
	UVTBOWTEditorToolsContext* Context = Runtime ? Runtime->GetToolsContext() : nullptr;
	DrawText(FString::Printf(TEXT("Context: %s   Render: %llu   Capture: %s   Undo: %s   Redo: %s"),
		Context && Context->IsRuntimeReady() ? TEXT("Ready") : TEXT("Missing"),
		Context ? Context->GetViewport().GetRenderCallCount() : 0,
		Context && Context->GetInput().HasActiveMouseCapture() ? TEXT("Yes") : TEXT("No"),
		Context && Context->GetUndoRedo().CanUndo() ? TEXT("Yes") : TEXT("No"),
		Context && Context->GetUndoRedo().CanRedo() ? TEXT("Yes") : TEXT("No")), FLinearColor::Yellow, 24, 86);
	float Y = 110;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (!It->ActorHasTag(TEXT("VTBRuntimeDemo")))
		{
			continue;
		}
		DrawText(FString::Printf(TEXT("%s  Pos %s  Rot %s  Scale %s"), *It->GetName(),
			*It->GetActorLocation().ToCompactString(), *It->GetActorRotation().ToCompactString(),
			*It->GetActorScale3D().ToCompactString()), FLinearColor::White, 24, Y);
		Y += 22;
	}
	FToolBuilderState Selection;
	if (IToolsContextQueriesAPI* Queries = Context ? Context->GetContextQueriesAPI() : nullptr)
	{
		Queries->GetCurrentSelectionState(Selection);
	}
	USceneComponent* Frame = Selection.SelectedComponents.IsEmpty()
		? nullptr : Cast<USceneComponent>(Selection.SelectedComponents[0]);
	DrawText(FString::Printf(TEXT("Selected frame: %s"), Frame ? *FString::Printf(TEXT("%s.%s"),
		*Frame->GetOwner()->GetName(), *Frame->GetName()) : TEXT("None")), FLinearColor::Yellow, 24, 158);
	if (Frame)
	{
		DrawText(FString::Printf(TEXT("World     Pos %s  Rot %s  Scale %s"),
			*Frame->GetComponentLocation().ToCompactString(), *Frame->GetComponentRotation().ToCompactString(),
			*Frame->GetComponentScale().ToCompactString()), FLinearColor::White, 24, 180);
		DrawText(FString::Printf(TEXT("Relative  Pos %s  Rot %s  Scale %s"),
			*Frame->GetRelativeLocation().ToCompactString(), *Frame->GetRelativeRotation().ToCompactString(),
			*Frame->GetRelativeScale3D().ToCompactString()), FLinearColor::White, 24, 202);
	}
	DrawText(TEXT("Drag changes the actor and both meshes; the selected component's Relative TRS stays fixed."),
		FLinearColor::White, 24, 232);
}
