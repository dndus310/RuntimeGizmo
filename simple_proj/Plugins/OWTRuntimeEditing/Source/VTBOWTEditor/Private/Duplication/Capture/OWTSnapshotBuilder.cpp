#include "Duplication/Capture/OWTSnapshotBuilder.h"

#include "Components/ChildActorComponent.h"
#include "Components/SceneComponent.h"
#include "Duplication/Capture/OWTDuplicationSnapshots.h"
#include "Duplication/Capture/OWTSourceValidator.h"
#include "Duplication/OWTDuplicationRequest.h"
#include "Duplication/Pipeline/OWTAdapterDispatcher.h"
#include "Duplication/Properties/OWTPropertyValue.h"
#include "Duplication/Properties/OWTReferenceVisitor.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"

FOWTSnapshotBuilder::FOWTSnapshotBuilder(const FOWTDuplicationContext& InContext, FOWTRootSnapshot& InSnapshot)
    : Context(InContext), Snapshot(InSnapshot), ExplicitInstancedObjects()
{
}

bool FOWTSnapshotBuilder::Capture()
{
	return CaptureActor(Snapshot.Root) && CaptureAdapterReferences() && CaptureReachableObjects() &&
	       ValidateOwnedObjects();
}

bool FOWTSnapshotBuilder::CaptureActor(AActor& Actor)
{
	// The hierarchy collector already validated every Actor reachable through ChildActorComponents.
	checkf(!Snapshot.Mapping.Contains(&Actor), TEXT("ChildActorComponent hierarchies cannot contain cycles."));
	Snapshot.Actors.Add(MakeUnique<FOWTActorSnapshot>(Actor, Context.Options.WorldOffset));
	TUniquePtr<FOWTObjectSnapshot> ActorState = FOWTObjectSnapshot::Capture(Actor, Context);
	bool bCaptured = ActorState.IsValid();
	if (bCaptured)
	{
		Snapshot.Objects.Add(MoveTemp(ActorState));
		Snapshot.Mapping.Add(&Actor, nullptr);
	}

	TInlineComponentArray<UActorComponent*> SourceComponents(&Actor);
	for (UActorComponent* Component : SourceComponents)
	{
		bCaptured = bCaptured && (Context.Adapters.IsManagedObject(*Component) || CaptureComponent(*Component));
	}
	for (UActorComponent* Component : SourceComponents)
	{
		UChildActorComponent* Child = Cast<UChildActorComponent>(Component);
		AActor* ChildActor = Child ? Child->GetChildActor() : nullptr;
		bCaptured = bCaptured && (!ChildActor || CaptureActor(*ChildActor));
	}
	return bCaptured;
}

bool FOWTSnapshotBuilder::CaptureComponent(UActorComponent& Component)
{
	const USceneComponent* Scene = Cast<USceneComponent>(&Component);
	const USceneComponent* Parent = Scene ? Scene->GetAttachParent() : nullptr;
	const bool bAttachedToManagedOutput = Parent && Context.Adapters.IsManagedObject(*Parent);
	if (bAttachedToManagedOutput)
	{
		Context.Error = FString::Printf(TEXT("Authored component %s is attached to managed output %s; a "
		                                     "logical attachment adapter is required."),
		                                *Scene->GetPathName(), *Parent->GetPathName());
	}

	TUniquePtr<FOWTObjectSnapshot> ComponentState =
	    bAttachedToManagedOutput ? nullptr : FOWTObjectSnapshot::Capture(Component, Context);
	const bool bCaptured = ComponentState.IsValid();
	if (bCaptured)
	{
		Snapshot.Objects.Add(MoveTemp(ComponentState));
		Snapshot.Components.Add(MakeUnique<FOWTComponentSnapshot>(Component));
		Snapshot.Mapping.Add(&Component, nullptr);
	}
	return bCaptured;
}

bool FOWTSnapshotBuilder::CaptureAdapterReferences()
{
	return Context.Adapters.VisitCapturedReferences([this](UObject*& Reference, bool bInstanced)
	{
		return CaptureReference(Reference, bInstanced);
	});
}

bool FOWTSnapshotBuilder::CaptureReachableObjects()
{
	// The array grows when reflected state reaches another owned instanced UObject.
	bool bCaptured = true;
	for (int32 Index = 0; bCaptured && Index < Snapshot.Objects.Num(); ++Index)
	{
		const FOWTObjectSnapshot& State = *Snapshot.Objects[Index];
		for (const TUniquePtr<FOWTPropertyValue>& Property : State.Properties)
		{
			FOWTReferenceVisitor::Visit(Property->GetProperty(), Property->GetValue(), [this, &bCaptured](UObject*& Reference, bool bInstanced)
			{
				bCaptured = bCaptured && CaptureReference(Reference, bInstanced);
			});
		}
	}
	return bCaptured;
}

bool FOWTSnapshotBuilder::ValidateOwnedObjects() const
{
	// Components and Actors are owned through their collections; MIDs are rebuilt through their own API.
	const TUniquePtr<FOWTObjectSnapshot>* Unowned = Snapshot.Objects.FindByPredicate([this](const TUniquePtr<FOWTObjectSnapshot>& State)
	{
		const UObject* Object = State->Source.Get();
		const bool bCollectionMember = Object->IsA<AActor>() || Object->IsA<UActorComponent>() || Object->IsA<UMaterialInstanceDynamic>();
		const bool bInstanced = ExplicitInstancedObjects.Contains(Object) || Object->GetClass()->HasAnyClassFlags(CLASS_DefaultToInstanced);
		return !bCollectionMember && !bInstanced;
	});
	if (Unowned)
	{
		Context.Error = FString::Printf(TEXT("Owned object %s must be an instanced UObject to duplicate safely."),
		                                *(*Unowned)->Source->GetPathName());
	}
	return Unowned == nullptr;
}

bool FOWTSnapshotBuilder::CaptureReference(UObject* Reference, bool bInstanced)
{
	const bool bManaged = Reference && Context.Adapters.IsManagedObject(*Reference);
	if (bManaged)
	{
		Context.Error =
		    FString::Printf(TEXT("Authored property references managed output %s; a logical output adapter is required."),
		                    *Reference->GetPathName());
	}

	bool bAdapterOwned = false;
	const bool bResolved = Reference && !bManaged && Context.Adapters.ResolveObjectOwner(*Reference, bAdapterOwned, Context.Error);
	const bool bCoreCopies = bResolved && !bAdapterOwned;
	if (bCoreCopies && bInstanced)
	{
		ExplicitInstancedObjects.Add(Reference);
	}
	// References already captured or outside the hierarchy need no further work.
	const bool bNewOwnedObject = bCoreCopies && !Snapshot.Mapping.Contains(Reference) && IsInCapturedActor(*Reference);
	const bool bCaptured = !Reference || (bResolved && (!bNewOwnedObject || CaptureOwnedObject(*Reference)));
	return bCaptured;
}

bool FOWTSnapshotBuilder::CaptureOwnedObject(UObject& Object)
{
	const bool bOutsideCollections = Object.IsA<UActorComponent>() || Object.IsA<AActor>();
	if (bOutsideCollections)
	{
		Context.Error = TEXT("The hierarchy references an owned Actor/component outside its component collections.");
	}
	const UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(&Object);
	const bool bSupportedMaterial = !bOutsideCollections && (!Material || FOWTSourceValidator::ValidateMaterial(*Material, Context.Error));
	// Outers are captured first so restoration can create each object inside its duplicated outer.
	UObject* Outer = Object.GetOuter();
	const bool bOuterCaptured = bSupportedMaterial && (Snapshot.Mapping.Contains(Outer) || CaptureReference(Outer, false));

	TUniquePtr<FOWTObjectSnapshot> State;
	if (bOuterCaptured)
	{
		Snapshot.Mapping.Add(&Object, nullptr);
		State = FOWTObjectSnapshot::Capture(Object, Context);
	}
	const bool bCaptured = State.IsValid();
	if (bCaptured)
	{
		Snapshot.Objects.Add(MoveTemp(State));
	}
	return bCaptured;
}

bool FOWTSnapshotBuilder::IsInCapturedActor(const UObject& Object) const
{
	return Snapshot.Actors.ContainsByPredicate([&Object](const TUniquePtr<FOWTActorSnapshot>& Actor)
	{
		return Object.IsIn(Actor->Source.Get());
	});
}
