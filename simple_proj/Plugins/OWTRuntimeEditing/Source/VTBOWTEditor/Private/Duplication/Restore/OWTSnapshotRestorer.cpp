#include "Duplication/Restore/OWTSnapshotRestorer.h"

#include "Components/ChildActorComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "Duplication/Capture/OWTDuplicationSnapshots.h"
#include "Duplication/OWTDuplicationRequest.h"
#include "Duplication/OWTRuntimeDuplicationParticipant.h"
#include "Duplication/Pipeline/OWTAdapterDispatcher.h"
#include "Duplication/Properties/OWTPropertyValue.h"
#include "Duplication/Restore/OWTRestoreHookComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"

FOWTSnapshotRestorer::FOWTSnapshotRestorer(const FOWTDuplicationContext& InContext, const FOWTRootSnapshot& InSnapshot,
                                           FName InReservedName, bool bInDeferParticipants)
    : Context(InContext), Snapshot(InSnapshot), Result(nullptr), RestoreHook(nullptr), Mapping(InSnapshot.Mapping),
      ReservedName(InReservedName), bDeferParticipants(bInDeferParticipants), bRestored(false), bSucceeded(false)
{
}

AActor* FOWTSnapshotRestorer::Spawn()
{
	const FTransform& Transform = Snapshot.Actors[0]->Transform;
	Result = SpawnDeferred(Transform);
	const bool bSpawned = IsValid(Result);
	// Construction scripts observe the authored root and Instance component values.
	const bool bResolved = bSpawned && ResolveObjects(EResolvePhase::BeforeConstruction);
	if (!bSpawned)
	{
		Context.Error = TEXT("The world could not spawn the duplicate Actor.");
	}
	else if (!bResolved)
	{
		Result->Destroy();
	}

	if (bResolved)
	{
		ApplyCapturedState();
		FinishSpawningWithRestoreHook(Transform);
	}
	const bool bAlive = bResolved && IsValid(Result);
	if (bResolved && !bAlive && Context.Error.IsEmpty())
	{
		Context.Error = TEXT("The duplicate was destroyed during construction or initialization.");
	}
	else if (bAlive && !bSucceeded)
	{
		Result->Destroy();
	}
	return bAlive && bSucceeded ? Result : nullptr;
}

bool FOWTSnapshotRestorer::FinalizeHierarchy(const FOWTObjectMap& CompleteMapping)
{
	Mapping = CompleteMapping;
	ApplyCapturedState();
	return RestoreSceneTransforms() && (!bDeferParticipants || RestoreParticipants());
}

const FOWTObjectMap& FOWTSnapshotRestorer::GetMapping() const
{
	return Mapping;
}

AActor* FOWTSnapshotRestorer::SpawnDeferred(const FTransform& Transform)
{
	AActor& Source = Snapshot.Root;
	FActorSpawnParameters Parameters;
	Parameters.Name = ReservedName;
	Parameters.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Required_ErrorAndReturnNull;
	Parameters.OverrideLevel = Source.GetLevel();
	Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Parameters.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
	Parameters.bDeferConstruction = true;
	AActor* Spawned = Context.World.SpawnActor<AActor>(Source.GetClass(), Transform, Parameters);
	if (IsValid(Spawned))
	{
		Mapping[&Source] = Spawned;
	}
	return Spawned;
}

void FOWTSnapshotRestorer::FinishSpawningWithRestoreHook(const FTransform& Transform)
{
	RestoreHook = NewObject<UOWTRestoreHookComponent>(Result);
	Result->AddInstanceComponent(RestoreHook);
	RestoreHook->SetRestoreCallback([this]()
	{
		RestoreAfterConstruction();
	});
	RestoreHook->RegisterComponent();
	check(!bRestored);
	Result->FinishSpawning(Transform, true, nullptr, ESpawnActorScaleMethod::OverrideRootScale);
	if (IsValid(Result))
	{
		// Worlds without initialized Actors never initialize components, so restoration runs here instead.
		// The callback is consumed on first use, which makes this a no-op after a normal initialization.
		RestoreHook->Restore();
	}
	RestoreHook->SetRestoreCallback(TFunction<void()>());
	RestoreHook->DestroyComponent();
	RestoreHook = nullptr;
}

bool FOWTSnapshotRestorer::ResolveObjects(EResolvePhase Phase)
{
	// Objects are created inside their duplicated outer, so each pass resolves one more level of nesting.
	bool bResolved = true;
	bool bProgress = true;
	for (int32 Pass = 0; bResolved && bProgress && Pass < Snapshot.Objects.Num(); ++Pass)
	{
		bProgress = false;
		for (const TUniquePtr<FOWTObjectSnapshot>& State : Snapshot.Objects)
		{
			UObject* Original = State->Source.Get();
			UObject* NewOuter = Mapping.FindRef(Original->GetOuter());
			const bool bPending = bResolved && !Original->IsA<AActor>() && !IsValid(Mapping.FindRef(Original)) && IsValid(NewOuter);
			UObject* Destination = nullptr;
			bResolved = bResolved && (!bPending || ResolveObject(*Original, *NewOuter, Phase, Destination));
			if (bPending && Destination)
			{
				Mapping[Original] = Destination;
				bProgress = true;
			}
		}
	}
	return bResolved;
}

bool FOWTSnapshotRestorer::ResolveObject(UObject& Original, UObject& NewOuter, EResolvePhase Phase,
                                         UObject*& OutDestination)
{
	UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(&Original);
	UObject* Existing = Material ? nullptr : FindObjectFast<UObject>(&NewOuter, Original.GetFName());
	const bool bReplacedByConstruction = Existing && (!IsValid(Existing) || Existing->GetClass() != Original.GetClass());
	const UActorComponent* OriginalComponent = Cast<UActorComponent>(&Original);
	const bool bWaitForConstruction = !Material && !Existing && OriginalComponent &&
	                                  Phase == EResolvePhase::BeforeConstruction &&
	                                  OriginalComponent->CreationMethod != EComponentCreationMethod::Instance;

	if (bReplacedByConstruction)
	{
		Context.Error = FString::Printf(TEXT("Construction changed or destroyed subobject %s."), *Original.GetPathName());
	}
	else if (Material)
	{
		OutDestination = ResolveMaterial(*Material, NewOuter);
	}
	else if (Existing)
	{
		OutDestination = Existing;
	}
	else if (!bWaitForConstruction)
	{
		OutDestination = CreateObject(Original, NewOuter);
	}
	return !bReplacedByConstruction;
}

UObject* FOWTSnapshotRestorer::ResolveMaterial(UMaterialInstanceDynamic& Original, UObject& NewOuter) const
{
	// A parent owned by the hierarchy must be duplicated first; external parents stay shared.
	UMaterialInterface* SourceParent = Original.Parent.Get();
	const bool bOwnedParent = Mapping.Contains(SourceParent);
	UMaterialInterface* Parent = bOwnedParent ? Cast<UMaterialInterface>(Mapping.FindRef(SourceParent)) : SourceParent;
	const bool bParentReady = !bOwnedParent || Parent;
	const FName MaterialName = MakeUniqueObjectName(&NewOuter, Original.GetClass(), Original.GetFName());
	return bParentReady ? UMaterialInstanceDynamic::Create(Parent, &NewOuter, MaterialName) : nullptr;
}

UObject* FOWTSnapshotRestorer::CreateObject(UObject& Original, UObject& NewOuter) const
{
	UObject* Created = NewObject<UObject>(&NewOuter, Original.GetClass(), Original.GetFName(), RF_Transient);
	if (UActorComponent* NewComponent = Cast<UActorComponent>(Created))
	{
		NewComponent->GetOwner()->AddInstanceComponent(NewComponent);
		NewComponent->CreationMethod = CastChecked<UActorComponent>(&Original)->CreationMethod;
	}
	return Created;
}

void FOWTSnapshotRestorer::ApplyCapturedState()
{
	for (const TUniquePtr<FOWTObjectSnapshot>& State : Snapshot.Objects)
	{
		UObject* Destination = Mapping.FindRef(State->Source.Get());
		if (IsValid(Destination))
		{
			Context.Adapters.PrepareDestination(*State->Source.Get(), *Destination);
			State->ApplyTo(*Destination, Mapping);
		}
	}
}

bool FOWTSnapshotRestorer::ValidateMapping()
{
	const UObject* Lost = nullptr;
	for (const TPair<UObject*, UObject*>& Pair : Mapping)
	{
		if (!Lost && !IsValid(Pair.Value))
		{
			Lost = Pair.Key;
		}
	}
	if (Lost)
	{
		Context.Error = FString::Printf(TEXT("Could not reconstruct %s."), *Lost->GetPathName());
	}
	return Lost == nullptr;
}

void FOWTSnapshotRestorer::RestoreAfterConstruction()
{
	bRestored = true;
	const bool bAlive = IsValid(Result);
	if (!bAlive)
	{
		Context.Error = TEXT("Construction destroyed the duplicate Actor.");
	}
	check(!bAlive || !Result->HasActorBegunPlay());

	bool bActorsRestored = bAlive;
	for (const TUniquePtr<FOWTActorSnapshot>& Actor : Snapshot.Actors)
	{
		bActorsRestored = bActorsRestored && RestoreActor(*Actor);
	}
	const bool bHierarchyRestored = bActorsRestored && ValidateMapping();
	if (bHierarchyRestored)
	{
		// Cross-child references can only be remapped after every child Actor has been recreated.
		ApplyCapturedState();
	}
	// Component registration/initialization callbacks may have changed transforms after their first application.
	const bool bTransformsRestored = bHierarchyRestored && RestoreSceneTransforms();
	if (bAlive && !bTransformsRestored)
	{
		Result->Destroy();
	}

	const bool bAdaptersRestored = bTransformsRestored && Context.Adapters.Restore(Mapping, Context.Error);
	if (bAdaptersRestored)
	{
		// Adapters add mappings for the objects they rebuilt; references to those objects are translated now.
		ApplyCapturedState();
	}
	bSucceeded = bAdaptersRestored && (bDeferParticipants || RestoreParticipants());
}

bool FOWTSnapshotRestorer::RestoreActor(const FOWTActorSnapshot& State)
{
	const bool bChildResolved = !State.ParentComponent || ResolveChildActor(State);
	AActor* Destination = bChildResolved ? CastChecked<AActor>(Mapping[State.Source.Get()]) : nullptr;
	check(!Destination || !Destination->HasActorBegunPlay());
	const bool bComponentsReplaced = bChildResolved && ResolveObjects(EResolvePhase::AfterConstruction) &&
	                                 ReplaceConstructedComponents(State, *Destination);
	if (bComponentsReplaced)
	{
		// Construction overwrote retained components with class defaults; captured values win.
		ApplyCapturedState();
	}

	const bool bConfigured = bComponentsReplaced && RestoreComponents(State, *Destination);
	const bool bAlive = bConfigured && IsValid(Destination);
	if (bConfigured && !bAlive)
	{
		Context.Error = TEXT("Component restoration destroyed a duplicate Actor.");
	}
	if (bAlive)
	{
		RestoreActorFlags(State, *Destination);
	}
	return bAlive;
}

bool FOWTSnapshotRestorer::ResolveChildActor(const FOWTActorSnapshot& State)
{
	AActor* Original = State.Source.Get();
	UChildActorComponent* Parent = Cast<UChildActorComponent>(Mapping.FindRef(State.ParentComponent));
	if (Parent && !Parent->GetChildActor())
	{
		Parent->CreateChildActor();
	}
	AActor* Child = Parent ? Parent->GetChildActor() : nullptr;
	const bool bMatches = IsValid(Child) && Child->GetClass() == Original->GetClass();

	if (!Parent)
	{
		Context.Error = TEXT("Could not reconstruct a ChildActorComponent parent.");
	}
	else if (!bMatches)
	{
		Context.Error = FString::Printf(TEXT("Could not reconstruct child Actor %s."), *Original->GetPathName());
	}
	if (bMatches)
	{
		Mapping[Original] = Child;
	}
	return bMatches;
}

bool FOWTSnapshotRestorer::ReplaceConstructedComponents(const FOWTActorSnapshot& State, AActor& Destination)
{
	TSet<UActorComponent*> Retained;
	bool bAllRetained = true;
	for (const TUniquePtr<FOWTComponentSnapshot>& ComponentState : Snapshot.Components)
	{
		const bool bOwned = IsOwnedBy(*ComponentState, State);
		UActorComponent* Component = bAllRetained && bOwned ? Cast<UActorComponent>(Mapping.FindRef(ComponentState->Source.Get())) : nullptr;
		bAllRetained = bAllRetained && (!bOwned || IsValid(Component));
		if (bAllRetained && bOwned)
		{
			Retained.Add(Component);
			// CAC unregistration destroys the temporary child. Children are resolved after this parent is restored.
			Component->UnregisterComponent();
		}
	}
	if (!bAllRetained)
	{
		Context.Error = TEXT("Could not reconstruct a hierarchy component.");
	}

	if (bAllRetained)
	{
		// Components construction created without a captured source would otherwise survive as extra state.
		TInlineComponentArray<UActorComponent*> Constructed(&Destination);
		for (UActorComponent* Component : Constructed)
		{
			if (Component != RestoreHook && !Retained.Contains(Component))
			{
				Component->DestroyComponent();
			}
		}
	}
	return bAllRetained;
}

bool FOWTSnapshotRestorer::RestoreComponents(const FOWTActorSnapshot& ActorState, AActor& Destination)
{
	AActor* Original = ActorState.Source.Get();
	USceneComponent* Root = Cast<USceneComponent>(Mapping.FindRef(Original->GetRootComponent()));
	Destination.SetRootComponent(Root);
	bool bConfigured = true;
	for (const TUniquePtr<FOWTComponentSnapshot>& State : Snapshot.Components)
	{
		const bool bOwned = IsOwnedBy(*State, ActorState);
		bConfigured = bConfigured && (!bOwned || ApplyComponentConfiguration(*State, *CastChecked<UActorComponent>(Mapping[State->Source.Get()])));
	}
	if (bConfigured && Root)
	{
		Root->SetWorldTransform(ActorState.Transform, false, nullptr, ETeleportType::TeleportPhysics);
	}

	// Registration and activation run user callbacks, so they start only after every component is configured.
	bool bActivated = bConfigured;
	for (const TUniquePtr<FOWTComponentSnapshot>& State : Snapshot.Components)
	{
		const bool bOwned = IsOwnedBy(*State, ActorState);
		bActivated = bActivated && (!bOwned || ActivateComponent(*State, *CastChecked<UActorComponent>(Mapping[State->Source.Get()]), Destination));
	}
	return bActivated;
}

bool FOWTSnapshotRestorer::ApplyComponentConfiguration(const FOWTComponentSnapshot& State, UActorComponent& Component)
{
	UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(&Component);
	USceneComponent* Scene = Cast<USceneComponent>(&Component);
	UInstancedStaticMeshComponent* Instances = Cast<UInstancedStaticMeshComponent>(&Component);
	if (Primitive)
	{
		for (const TUniquePtr<FOWTPropertyValue>& Property : State.BodyProperties)
		{
			Property->ApplyTo(&Primitive->BodyInstance, Mapping);
		}
	}
	bool bConfigured = true;
	if (Scene)
	{
		Scene->UpdateComponentToWorld();
		bConfigured = RestoreSceneTransform(State, *Scene);
	}
	if (bConfigured && Instances)
	{
		Instances->ClearInstances();
		Instances->SetNumCustomDataFloats(State.NumCustomDataFloats);
		for (const FTransform& Transform : State.InstanceTransforms)
		{
			Instances->AddInstance(Transform, false);
		}
		Instances->PerInstanceSMCustomData = State.InstanceCustomData;
	}
	return bConfigured;
}

bool FOWTSnapshotRestorer::ActivateComponent(const FOWTComponentSnapshot& State, UActorComponent& Component,
                                             AActor& Destination)
{
	bool bRegistered = true;
	if (State.bRegistered)
	{
		Component.RegisterComponent();
		const bool bSurvived = IsValid(&Destination) && IsValid(&Component);
		bRegistered = bSurvived && Component.IsRegistered();
		if (!bSurvived)
		{
			Context.Error = TEXT("Component registration destroyed a duplicate object.");
		}
		else if (!bRegistered)
		{
			Context.Error = TEXT("A duplicate component callback canceled its required registration.");
		}
		const bool bNeedsInitialization = bRegistered && Context.World.AreActorsInitialized() &&
		                                  Component.bWantsInitializeComponent && !Component.HasBeenInitialized();
		if (bNeedsInitialization)
		{
			Component.InitializeComponent();
		}
	}

	if (bRegistered)
	{
		UChildActorComponent* Child = Cast<UChildActorComponent>(&Component);
		if (Child && !State.ChildActor)
		{
			Child->DestroyChildActor();
		}
		if (State.bSimulating)
		{
			UPrimitiveComponent& Primitive = *CastChecked<UPrimitiveComponent>(&Component);
			Primitive.SetSimulatePhysics(true);
			Primitive.SetPhysicsLinearVelocity(State.LinearVelocity);
			Primitive.SetPhysicsAngularVelocityInRadians(State.AngularVelocity);
			if (State.bAwake)
			{
				Primitive.WakeAllRigidBodies();
			}
			else
			{
				Primitive.PutAllRigidBodiesToSleep();
			}
		}
		Component.SetActive(State.bActive);
		Component.SetComponentTickInterval(State.TickInterval);
		Component.SetComponentTickEnabled(State.bTickEnabled);
	}
	const bool bActivated = bRegistered && IsValid(&Destination) && IsValid(&Component);
	if (bRegistered && !bActivated)
	{
		Context.Error = TEXT("Component activation destroyed a duplicate object.");
	}
	return bActivated;
}

void FOWTSnapshotRestorer::RestoreActorFlags(const FOWTActorSnapshot& State, AActor& Destination)
{
	Destination.SetOwner(Cast<AActor>(Mapping.FindRef(State.Source->GetOwner())));
	Destination.SetInstigator(nullptr);
	Destination.SetActorHiddenInGame(State.bHidden);
	Destination.SetActorEnableCollision(State.bCollision);
	Destination.SetActorTickInterval(State.TickInterval);
	Destination.SetActorTickEnabled(State.bTickEnabled);
}

bool FOWTSnapshotRestorer::RestoreSceneTransform(const FOWTComponentSnapshot& State, USceneComponent& Scene)
{
	USceneComponent* Parent = Cast<USceneComponent>(Mapping.FindRef(State.Parent));
	const FName Socket = Parent ? State.Socket : NAME_None;
	const bool bNeedsReattach = Scene.GetAttachParent() != Parent || Scene.GetAttachSocketName() != Socket;
	bool bAttached = true;
	if (bNeedsReattach)
	{
		Scene.DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		if (Scene.IsRegistered())
		{
			bAttached = !Parent || Scene.AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform, Socket);
		}
		else
		{
			Scene.SetupAttachment(Parent, Socket);
		}
	}
	if (!bAttached)
	{
		Context.Error = TEXT("A duplicate scene component rejected its captured attachment.");
	}

	if (bAttached)
	{
		// Components attached to something outside the hierarchy keep their world placement plus the offset.
		FTransform Transform = State.RelativeTransform;
		if (!Parent)
		{
			Transform = State.WorldTransform;
			Transform.AddToTranslation(Context.Options.WorldOffset);
		}
		Scene.SetRelativeTransform(Transform, false, nullptr, ETeleportType::TeleportPhysics);
	}
	const bool bTransformRestored = bAttached && IsValid(&Scene);
	if (bAttached && !bTransformRestored)
	{
		Context.Error = TEXT("A duplicate scene component was destroyed by its transform callback.");
	}
	return bTransformRestored;
}

bool FOWTSnapshotRestorer::RestoreSceneTransforms()
{
	bool bTransformsRestored = true;
	for (const TUniquePtr<FOWTActorSnapshot>& Actor : Snapshot.Actors)
	{
		for (const TUniquePtr<FOWTComponentSnapshot>& State : Snapshot.Components)
		{
			USceneComponent* Scene = IsOwnedBy(*State, *Actor) ? Cast<USceneComponent>(Mapping.FindRef(State->Source.Get())) : nullptr;
			const bool bRestorable = bTransformsRestored && Scene;
			// An earlier transform callback in this loop may have destroyed this component.
			if (bRestorable && !IsValid(Scene))
			{
				Context.Error = FString::Printf(TEXT("Could not reconstruct %s."), *State->Source->GetPathName());
				bTransformsRestored = false;
			}
			else if (bRestorable)
			{
				bTransformsRestored = RestoreSceneTransform(*State, *Scene);
			}
		}
		bTransformsRestored = bTransformsRestored && ValidateMapping();
		AActor* Destination = bTransformsRestored ? CastChecked<AActor>(Mapping[Actor->Source.Get()]) : nullptr;
		USceneComponent* Root = Destination ? Destination->GetRootComponent() : nullptr;
		if (Root)
		{
			Root->SetWorldTransform(Actor->Transform, false, nullptr, ETeleportType::TeleportPhysics);
			bTransformsRestored = ValidateMapping();
		}
	}
	return bTransformsRestored;
}

bool FOWTSnapshotRestorer::RestoreParticipants()
{
	bool bParticipantsRestored = true;
	for (const TUniquePtr<FOWTObjectSnapshot>& State : Snapshot.Objects)
	{
		UObject* Destination = Mapping.FindRef(State->Source.Get());
		const bool bParticipant = bParticipantsRestored &&
		                          Destination->GetClass()->ImplementsInterface(UOWTRuntimeDuplicationParticipant::StaticClass());
		if (bParticipant)
		{
			const bool bAccepted = IOWTRuntimeDuplicationParticipant::Execute_RestoreRuntimeDuplicateState(
			    Destination, State->Source.Get(), Mapping, Context.Error);
			if (!bAccepted && Context.Error.IsEmpty())
			{
				Context.Error = FString::Printf(TEXT("Custom duplication restoration rejected %s."), *Destination->GetPathName());
			}
			// Participants are user code and may destroy any duplicate object.
			bParticipantsRestored = bAccepted && ValidateMapping();
		}
	}
	if (!bParticipantsRestored)
	{
		Result->Destroy();
	}
	return bParticipantsRestored;
}

bool FOWTSnapshotRestorer::IsOwnedBy(const FOWTComponentSnapshot& State, const FOWTActorSnapshot& ActorState) const
{
	return State.Source->GetOwner() == ActorState.Source.Get();
}
