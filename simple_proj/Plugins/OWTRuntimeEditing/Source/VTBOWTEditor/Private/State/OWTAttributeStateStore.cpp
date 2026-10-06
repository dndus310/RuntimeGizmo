#include "State/OWTAttributeStateStore.h"

#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"

FGuid UOWTAttributeStateStore::RegisterActor(AActor& Actor, bool bNewObject)
{
	check(IsInGameThread());
	for (auto It = Actors.CreateIterator(); It; ++It)
	{
		if (!It.Value().Actor.IsValid())
		{
			It.RemoveCurrent();
			continue;
		}
		if (It.Value().Actor.Get() == &Actor)
		{
			return It.Key();
		}
	}

	const FGuid Id = FGuid::NewGuid();
	FOWTRegisteredAttributeActor Record;
	Record.Actor = &Actor;
	Record.Baseline = Actor.GetActorTransform();
	Record.bNewObject = bNewObject;
	Actors.Add(Id, MoveTemp(Record));
	return Id;
}

AActor* UOWTAttributeStateStore::ResolveActor(FGuid Id) const
{
	check(IsInGameThread());
	const FOWTRegisteredAttributeActor* Record = Actors.Find(Id);
	return Record ? Record->Actor.Get() : nullptr;
}

bool UOWTAttributeStateStore::ContainsActor(FGuid Id) const
{
	check(IsInGameThread());
	return Actors.Contains(Id);
}

void UOWTAttributeStateStore::Observe(AActor* Actor, FGuid Id, FOWTAttributeSnapshot Context)
{
	check(IsInGameThread());
	Snapshot = MoveTemp(Context);
	if (!IsValid(Actor))
	{
		if (Snapshot.DisabledReason.IsEmpty())
		{
			Snapshot.DisabledReason =
			    Snapshot.bEditingEnabled ? TEXT("No actor selected.") : TEXT("Editing is disabled.");
		}
		return;
	}
	if (Actor->IsActorBeingDestroyed())
	{
		Snapshot.DisabledReason = TEXT("The selected actor is being destroyed.");
		return;
	}

	Snapshot.bHasSelection = true;
	Snapshot.ObjectId = Id.ToString(EGuidFormats::DigitsWithHyphens);
	Snapshot.ObjectName = Actor->GetName();
	Snapshot.ObjectClass = Actor->GetClass()->GetPathName();
	Snapshot.Transform = Actor->GetActorTransform();
	if (const FOWTRegisteredAttributeActor* Record = Actors.Find(Id))
	{
		Snapshot.bHasChanges = Record->bNewObject;
		if (!Snapshot.Transform.Equals(Record->Baseline))
		{
			Snapshot.bHasChanges = true;
		}
	}
	if (!Snapshot.bEditingEnabled)
	{
		Snapshot.DisabledReason = TEXT("Editing is disabled.");
		return;
	}
	USceneComponent* Root = Actor->GetRootComponent();
	if (!Root)
	{
		Snapshot.DisabledReason = TEXT("The selected actor has no root component.");
		return;
	}
	if (Root->Mobility != EComponentMobility::Movable)
	{
		Snapshot.DisabledReason = TEXT("The selected actor root is not Movable.");
		return;
	}
	if (Snapshot.Transform.ContainsNaN())
	{
		Snapshot.DisabledReason = TEXT("The actor transform contains an invalid number.");
		return;
	}
	Snapshot.bCanEditTransform = true;
}

bool UOWTAttributeStateStore::MarkBaseline(FGuid Id)
{
	check(IsInGameThread());
	FOWTRegisteredAttributeActor* Record = Actors.Find(Id);
	if (!Record)
	{
		return false;
	}
	AActor* Actor = Record->Actor.Get();
	if (!Actor)
	{
		return false;
	}
	Record->Baseline = Actor->GetActorTransform();
	Record->bNewObject = false;
	return true;
}

void UOWTAttributeStateStore::Reset()
{
	check(IsInGameThread());
	Actors.Empty();
	Snapshot = FOWTAttributeSnapshot();
}

FOWTAttributeSnapshot UOWTAttributeStateStore::GetSnapshot() const
{
	check(IsInGameThread());
	return Snapshot;
}
