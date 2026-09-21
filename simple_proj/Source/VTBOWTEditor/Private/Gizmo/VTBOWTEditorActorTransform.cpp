#include "Gizmo/VTBOWTEditorActorTransform.h"

#include "BaseGizmos/TransformProxy.h"
#include "Components/SceneComponent.h"

namespace
{
	struct FActorTransformAttachment
	{
		FTransform RelativeTransform;
		FTransform SocketTransform = FTransform::Identity;
		bool bUsesSocket = false;
		bool bAbsoluteLocation = false;
		bool bAbsoluteRotation = false;
		bool bAbsoluteScale = false;
	};

	FTransform PredictFrameTransform(TConstArrayView<FActorTransformAttachment> Hierarchy, const FTransform& RootTransform)
	{
		FTransform FrameTransform = RootTransform;
		for (int32 Index = Hierarchy.Num() - 1; Index >= 0; --Index)
		{
			const FActorTransformAttachment& Attachment = Hierarchy[Index];
			FTransform ParentTransform = FrameTransform;
			if (Attachment.bUsesSocket)
			{
				ParentTransform = Attachment.SocketTransform * ParentTransform;
			}
			FrameTransform = Attachment.RelativeTransform * ParentTransform;
			if (Attachment.bAbsoluteLocation)
			{
				FrameTransform.CopyTranslation(Attachment.RelativeTransform);
			}
			if (Attachment.bAbsoluteRotation)
			{
				FrameTransform.CopyRotation(Attachment.RelativeTransform);
			}
			if (Attachment.bAbsoluteScale)
			{
				FrameTransform.CopyScale3D(Attachment.RelativeTransform);
			}
		}
		return FrameTransform;
	}
}

namespace UE::VTBOWTEditor
{
	void AddActorTransformTarget(UTransformProxy& Proxy, USceneComponent* RootComponent, USceneComponent* FrameComponent)
	{
		if (RootComponent == FrameComponent)
		{
			Proxy.AddComponent(RootComponent, false);
			return;
		}

		TArray<FActorTransformAttachment> Hierarchy;
		const USceneComponent* Component = FrameComponent;
		while (Component != RootComponent)
		{
			const USceneComponent* Parent = IsValid(Component) ? Component->GetAttachParent() : nullptr;
			if (!IsValid(Parent))
			{
				return;
			}
			FActorTransformAttachment& Attachment = Hierarchy.AddDefaulted_GetRef();
			Attachment.RelativeTransform = Component->GetRelativeTransform();
			Attachment.bUsesSocket = !Component->GetAttachSocketName().IsNone();
			if (Attachment.bUsesSocket)
			{
				Attachment.SocketTransform = Parent->GetSocketTransform(Component->GetAttachSocketName(), RTS_Component);
			}
			Attachment.bAbsoluteLocation = Component->IsUsingAbsoluteLocation();
			Attachment.bAbsoluteRotation = Component->IsUsingAbsoluteRotation();
			Attachment.bAbsoluteScale = Component->IsUsingAbsoluteScale();
			Component = Parent;
		}

		const TWeakObjectPtr<USceneComponent> WeakRoot(RootComponent);
		const TWeakObjectPtr<USceneComponent> WeakFrame(FrameComponent);
		Proxy.AddComponentCustom(RootComponent,
			[WeakFrame]()
			{
				const USceneComponent* Frame = WeakFrame.Get();
				return Frame ? Frame->GetComponentTransform() : FTransform::Identity;
			},
			[WeakRoot, Hierarchy = MoveTemp(Hierarchy)](const FTransform& FrameTransform)
			{
				USceneComponent* Root = WeakRoot.Get();
				if (!Root)
				{
					return;
				}

				FTransform RootTransform = Root->GetComponentTransform();
				FTransform UnitScaleRoot = RootTransform;
				UnitScaleRoot.SetScale3D(FVector::OneVector);
				const FTransform UnitScaleFrame = PredictFrameTransform(Hierarchy, UnitScaleRoot);
				RootTransform.SetScale3D(FrameTransform.GetScale3D()
					* FTransform::GetSafeScaleReciprocal(UnitScaleFrame.GetScale3D()));
				FTransform PredictedFrame = PredictFrameTransform(Hierarchy, RootTransform);
				RootTransform.SetRotation((FrameTransform.GetRotation() * PredictedFrame.GetRotation().Inverse()
					* RootTransform.GetRotation()).GetNormalized());
				PredictedFrame = PredictFrameTransform(Hierarchy, RootTransform);
				RootTransform.AddToTranslation(FrameTransform.GetTranslation() - PredictedFrame.GetTranslation());
				Root->SetWorldTransform(RootTransform);
				Root->InvalidateLightingCacheDetailed(true, false);
			},
			0, false);
	}
}
