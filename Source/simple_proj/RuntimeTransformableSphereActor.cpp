// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeTransformableSphereActor.h"

#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/EngineTypes.h"
#include "Engine/StaticMesh.h"
#include "RuntimeGizmoPlayerController.h"
#include "RuntimeTransformGizmoComponent.h"
#include "UObject/ConstructorHelpers.h"

ARuntimeTransformableSphereActor::ARuntimeTransformableSphereActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SetRootComponent(SphereMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereAsset.Succeeded())
	{
		SphereMesh->SetStaticMesh(SphereAsset.Object);
	}

	SphereMesh->SetMobility(EComponentMobility::Movable);
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereMesh->SetCollisionObjectType(ECC_WorldDynamic);
	SphereMesh->SetCollisionResponseToAllChannels(ECR_Block);
	SphereMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	SphereMesh->SetGenerateOverlapEvents(false);
	SphereMesh->SetRenderCustomDepth(false);
	SphereMesh->SetCustomDepthStencilValue(HoverStencilValue);

	SphereMesh->OnBeginCursorOver.AddDynamic(this, &ARuntimeTransformableSphereActor::HandleBeginCursorOver);
	SphereMesh->OnEndCursorOver.AddDynamic(this, &ARuntimeTransformableSphereActor::HandleEndCursorOver);
	SphereMesh->OnClicked.AddDynamic(this, &ARuntimeTransformableSphereActor::HandleClicked);
}

void ARuntimeTransformableSphereActor::BeginPlay()
{
	Super::BeginPlay();
	UpdateHighlightState();
}

void ARuntimeTransformableSphereActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bHovered && !bSelectedByRuntimeGizmo)
	{
		return;
	}

	const FColor OutlineColor = bSelectedByRuntimeGizmo ? FColor(255, 210, 64) : FColor(64, 210, 255);
	const float OutlineRadius = SphereMesh ? SphereMesh->Bounds.SphereRadius * 1.04f : 64.0f;
	const FVector OutlineCenter = SphereMesh ? SphereMesh->Bounds.Origin : GetActorLocation();
	DrawDebugSphere(GetWorld(), OutlineCenter, OutlineRadius, 48, OutlineColor, false, 0.0f, SDPG_Foreground, bSelectedByRuntimeGizmo ? 3.0f : 2.0f);
}

void ARuntimeTransformableSphereActor::SetSelectedByRuntimeGizmo(bool bSelected)
{
	bSelectedByRuntimeGizmo = bSelected;
	UpdateHighlightState();
}

USceneComponent* ARuntimeTransformableSphereActor::GetRuntimeTransformComponent_Implementation() const
{
	return SphereMesh;
}

void ARuntimeTransformableSphereActor::HandleBeginCursorOver(UPrimitiveComponent* TouchedComponent)
{
	bHovered = true;
	UpdateHighlightState();
}

void ARuntimeTransformableSphereActor::HandleEndCursorOver(UPrimitiveComponent* TouchedComponent)
{
	bHovered = false;
	UpdateHighlightState();
}

void ARuntimeTransformableSphereActor::HandleClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (ButtonPressed != EKeys::LeftMouseButton)
	{
		return;
	}

	APlayerController* PlayerController = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (!PlayerController)
	{
		return;
	}

	URuntimeTransformGizmoComponent* RuntimeGizmo = nullptr;
	if (ARuntimeGizmoPlayerController* RuntimePlayerController = Cast<ARuntimeGizmoPlayerController>(PlayerController))
	{
		RuntimeGizmo = RuntimePlayerController->GetRuntimeTransformGizmoComponent();
	}
	else
	{
		RuntimeGizmo = PlayerController->FindComponentByClass<URuntimeTransformGizmoComponent>();
	}

	if (RuntimeGizmo && !RuntimeGizmo->IsGizmoInteracting())
	{
		RuntimeGizmo->SelectActor(this);
	}
}

void ARuntimeTransformableSphereActor::UpdateHighlightState()
{
	if (!SphereMesh)
	{
		return;
	}

	const bool bShouldHighlight = bHovered || bSelectedByRuntimeGizmo;
	SphereMesh->SetRenderCustomDepth(bShouldHighlight);
	SphereMesh->SetCustomDepthStencilValue(bSelectedByRuntimeGizmo ? SelectedStencilValue : HoverStencilValue);
}
