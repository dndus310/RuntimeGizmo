#include "Gizmos/Custom/VTBOWTTransformGizmoActor.h"

#include "BaseGizmos/GizmoArrowComponent.h"
#include "BaseGizmos/GizmoBoxComponent.h"
#include "BaseGizmos/GizmoCircleComponent.h"
#include "Engine/World.h"

void AVTBOWTTransformGizmoActor::InitializeHandles(UGizmoViewContext& ViewContext)
{
	CreateTranslationHandles(ViewContext);
	CreateRotationHandles(ViewContext);
	CreateScaleHandles(ViewContext);
}

void AVTBOWTTransformGizmoActor::CreateTranslationHandles(UGizmoViewContext& ViewContext)
{
	TranslateX = AddDefaultArrowComponent(GetWorld(), this, &ViewContext, FLinearColor::Red, FVector::XAxisVector);
	TranslateY = AddDefaultArrowComponent(GetWorld(), this, &ViewContext, FLinearColor::Green, FVector::YAxisVector);
	TranslateZ = AddDefaultArrowComponent(GetWorld(), this, &ViewContext, FLinearColor::Blue, FVector::ZAxisVector);
}

void AVTBOWTTransformGizmoActor::CreateRotationHandles(UGizmoViewContext& ViewContext)
{
	RotateX = AddDefaultCircleComponent(GetWorld(), this, &ViewContext, FLinearColor::Red, FVector::XAxisVector, 80.f);
	RotateY =
	    AddDefaultCircleComponent(GetWorld(), this, &ViewContext, FLinearColor::Green, FVector::YAxisVector, 80.f);
	RotateZ = AddDefaultCircleComponent(GetWorld(), this, &ViewContext, FLinearColor::Blue, FVector::ZAxisVector, 80.f);
}

void AVTBOWTTransformGizmoActor::CreateScaleHandles(UGizmoViewContext& ViewContext)
{
	AxisScaleX = AddDefaultBoxComponent(GetWorld(), this, &ViewContext, FLinearColor::Red, FVector(80, 0, 0));
	AxisScaleY = AddDefaultBoxComponent(GetWorld(), this, &ViewContext, FLinearColor::Green, FVector(0, 80, 0));
	AxisScaleZ = AddDefaultBoxComponent(GetWorld(), this, &ViewContext, FLinearColor::Blue, FVector(0, 0, 80));
}

FVTBOWTTransformGizmoActorFactory::FVTBOWTTransformGizmoActorFactory(UGizmoViewContext& ViewContext)
    : FCombinedTransformGizmoActorFactory(&ViewContext)
{
}

ACombinedTransformGizmoActor* FVTBOWTTransformGizmoActorFactory::CreateNewGizmoActor(UWorld* World) const
{
	check(World && GizmoViewContext);
	AVTBOWTTransformGizmoActor* Actor = World->SpawnActor<AVTBOWTTransformGizmoActor>();
	if (!ensureMsgf(Actor, TEXT("OWT gizmo actor creation failed.")))
	{
		return nullptr;
	}
	Actor->InitializeHandles(*GizmoViewContext);
	return Actor;
}
