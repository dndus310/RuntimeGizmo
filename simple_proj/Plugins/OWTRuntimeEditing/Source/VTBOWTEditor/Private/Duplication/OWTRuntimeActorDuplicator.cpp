#include "Duplication/OWTRuntimeActorDuplicator.h"

#include "Duplication/OWTDuplicationRestoreComponent.h"
#include "Duplication/OWTDuplicationAdapterProvider.h"
#include "Duplication/OWTDuplicationReferences.h"
#include "StructUtils/InstancedStruct.h"
#include "StructUtils/PropertyBag.h"
#include "Duplication/OWTRuntimeDuplicationParticipant.h"
#include "Components/ChildActorComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Brush.h"
#include "Engine/Level.h"
#include "Engine/TimerHandle.h"
#include "Engine/World.h"
#include "Features/IModularFeatures.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Info.h"
#include "GameFramework/Pawn.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Misc/ScopeExit.h"
#include "PhysicsEngine/BodyInstance.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "UObject/StrongObjectPtr.h"
#include "UObject/UObjectHash.h"
#include "UObject/UnrealType.h"

namespace OWTDuplication
{
using FObjectMap = TMap<UObject*, UObject*>;
constexpr EPropertyFlags ExcludedSnapshotProperties = CPF_Transient | CPF_DuplicateTransient |
                                                      CPF_NonPIEDuplicateTransient | CPF_Deprecated | CPF_EditorOnly |
                                                      CPF_SkipSerialization;

bool IsStoredProperty(const FProperty& Property, const UObject& Object,
                      const FOWTDuplicationPropertyPolicySet& Policies)
{
	constexpr EPropertyFlags Excluded = CPF_Transient | CPF_DuplicateTransient | CPF_NonPIEDuplicateTransient |
	                                    CPF_Deprecated | CPF_EditorOnly | CPF_SkipSerialization;
	if (Property.HasAnyPropertyFlags(Excluded))
	{
		return false;
	}
	if (CastField<FDelegateProperty>(&Property))
	{
		return false;
	}
	if (CastField<FMulticastDelegateProperty>(&Property))
	{
		return false;
	}

	const TOptional<bool> Decision = Policies.Resolve(Object, Property);
	return Decision.Get(true);
}
void VisitReferences(FProperty& Property, void* Value, TFunctionRef<void(UObject*&, bool)> Visitor)
{
	for (int32 ArrayIndex = 0; ArrayIndex < Property.ArrayDim; ++ArrayIndex)
	{
		void* Element = static_cast<uint8*>(Value) + Property.GetElementSize() * ArrayIndex;
		if (CastField<FDelegateProperty>(&Property))
		{
			static_cast<FScriptDelegate*>(Element)->Unbind();
			continue;
		}
		if (FMulticastDelegateProperty* Delegate = CastField<FMulticastDelegateProperty>(&Property))
		{
			Delegate->ClearDelegate(nullptr, Element);
			continue;
		}
		if (FObjectPropertyBase* ObjectProperty = CastField<FObjectPropertyBase>(&Property))
		{
			UObject* Object = ObjectProperty->GetObjectPropertyValue(Element);
			UObject* OriginalObject = Object;
			Visitor(Object, Property.HasAnyPropertyFlags(CPF_InstancedReference));
			if (Object != OriginalObject)
			{
				// Unloaded soft references still have a valid path despite resolving to nullptr.
				ObjectProperty->SetObjectPropertyValue(Element, Object);
			}
			continue;
		}
		if (FInterfaceProperty* InterfaceProperty = CastField<FInterfaceProperty>(&Property))
		{
			FScriptInterface* Interface = static_cast<FScriptInterface*>(Element);
			UObject* Object = Interface->GetObject();
			Visitor(Object, false);
			Interface->SetObject(Object);
			Interface->SetInterface(Object ? Object->GetInterfaceAddress(InterfaceProperty->InterfaceClass) : nullptr);
			continue;
		}
		if (FStructProperty* StructProperty = CastField<FStructProperty>(&Property))
		{
			if (StructProperty->Struct == FTimerHandle::StaticStruct())
			{
				// Timer handles identify entries in a shared world manager; copying them aliases the source's timer.
				static_cast<FTimerHandle*>(Element)->Invalidate();
				continue;
			}
			for (TFieldIterator<FProperty> Field(StructProperty->Struct); Field; ++Field)
			{
				void* FieldValue = Field->ContainerPtrToValuePtr<void>(Element);
				if (Field->HasAnyPropertyFlags(ExcludedSnapshotProperties))
				{
					Field->ClearValue(FieldValue);
					continue;
				}
				VisitReferences(**Field, FieldValue, Visitor);
			}
			continue;
		}
		if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(&Property))
		{
			FScriptArrayHelper Array(ArrayProperty, Element);
			for (int32 Index = 0; Index < Array.Num(); ++Index)
			{
				VisitReferences(*ArrayProperty->Inner, Array.GetRawPtr(Index), Visitor);
			}
			continue;
		}
		if (FSetProperty* SetProperty = CastField<FSetProperty>(&Property))
		{
			FScriptSetHelper Set(SetProperty, Element);
			for (int32 Index = 0; Index < Set.GetMaxIndex(); ++Index)
			{
				if (Set.IsValidIndex(Index))
				{
					VisitReferences(*SetProperty->ElementProp, Set.GetElementPtr(Index), Visitor);
				}
			}
			Set.Rehash();
			continue;
		}
		if (FMapProperty* MapProperty = CastField<FMapProperty>(&Property))
		{
			FScriptMapHelper Map(MapProperty, Element);
			for (int32 Index = 0; Index < Map.GetMaxIndex(); ++Index)
			{
				if (!Map.IsValidIndex(Index))
				{
					continue;
				}
				VisitReferences(*MapProperty->KeyProp, Map.GetKeyPtr(Index), Visitor);
				VisitReferences(*MapProperty->ValueProp, Map.GetValuePtr(Index), Visitor);
			}
			Map.Rehash();
		}
	}
}

const UScriptStruct* FindUnsupportedStruct(const FProperty& Property, TSet<const UScriptStruct*>& Visited)
{
	if (const FStructProperty* StructProperty = CastField<FStructProperty>(&Property))
	{
		const UScriptStruct* Struct = StructProperty->Struct;
		if (Struct == FBodyInstance::StaticStruct() || Struct == FConstraintInstance::StaticStruct())
		{
			return Struct;
		}
		if (Struct->IsChildOf(FTickFunction::StaticStruct()))
		{
			return Struct;
		}
		if (Struct == FInstancedStruct::StaticStruct() || Struct == FInstancedPropertyBag::StaticStruct())
		{
			return Struct;
		}
		if (Visited.Contains(Struct))
		{
			return nullptr;
		}
		Visited.Add(Struct);
		for (TFieldIterator<FProperty> Field(Struct); Field; ++Field)
		{
			if (const UScriptStruct* Unsupported = FindUnsupportedStruct(**Field, Visited))
			{
				return Unsupported;
			}
		}
	}
	if (const FArrayProperty* Array = CastField<FArrayProperty>(&Property))
	{
		return FindUnsupportedStruct(*Array->Inner, Visited);
	}
	if (const FSetProperty* Set = CastField<FSetProperty>(&Property))
	{
		return FindUnsupportedStruct(*Set->ElementProp, Visited);
	}
	if (const FMapProperty* Map = CastField<FMapProperty>(&Property))
	{
		if (const UScriptStruct* Unsupported = FindUnsupportedStruct(*Map->KeyProp, Visited))
		{
			return Unsupported;
		}
		return FindUnsupportedStruct(*Map->ValueProp, Visited);
	}
	return nullptr;
}

struct FStoredProperty
{
	FStoredProperty(FProperty& InProperty, const void* Container) : Property(InProperty), Value(nullptr)
	{
		Value = FMemory::Malloc(Property.GetSize(), Property.GetMinAlignment());
		Property.InitializeValue(Value);
		Property.CopyCompleteValue(Value, Property.ContainerPtrToValuePtr<void>(Container));
	}

	~FStoredProperty()
	{
		Property.DestroyValue(Value);
		FMemory::Free(Value);
	}

	void Apply(void* Container, const FObjectMap& Mapping) const
	{
		void* Destination = Property.ContainerPtrToValuePtr<void>(Container);
		Property.CopyCompleteValue(Destination, Value);
		VisitReferences(Property, Destination,
		                [&Mapping](UObject*& Reference, bool)
		                {
			                if (UObject* const* Replacement = Mapping.Find(Reference))
			                {
				                Reference = *Replacement;
			                }
		                });
	}

	FProperty& Property;
	void* Value;
};

struct FObjectState
{
	FObjectState(UObject& InSource, const TArray<TUniquePtr<IOWTDuplicationAdapter>>& Adapters,
	             const FOWTDuplicationPropertyPolicySet& Policies, FString& Error)
	    : Source(&InSource), MaterialSnapshot(nullptr), Properties()
	{
		if (UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(&InSource))
		{
			MaterialSnapshot.Reset(UMaterialInstanceDynamic::Create(Material->Parent, GetTransientPackage()));
			MaterialSnapshot->RenamedTextures = Material->RenamedTextures;
		}
		for (TFieldIterator<FProperty> Property(InSource.GetClass()); Property; ++Property)
		{
			if (!IsStoredProperty(**Property, InSource, Policies))
			{
				continue;
			}
			bool bHandled = false;
			for (const auto& Adapter : Adapters)
			{
				bool bApplicable = Adapter->AcceptsObject(InSource);
				bApplicable |= Adapter->OwnsObject(InSource);
				if (!bApplicable)
				{
					continue;
				}
				if (Adapter->OwnsProperty(InSource, **Property))
				{
					if (Adapter->IsAuxiliary())
					{
						Error = TEXT("AdapterOwnershipConflict: auxiliary policies cannot own properties.");
						return;
					}
					if (bHandled)
					{
						Error = FString::Printf(TEXT("AdapterOwnershipConflict: multiple primary adapters claim %s."),
						                        *Property->GetPathName());
						return;
					}
					bHandled = true;
				}
			}
			if (bHandled)
			{
				continue;
			}
			TSet<const UScriptStruct*> Visited;
			if (const UScriptStruct* Unsupported = FindUnsupportedStruct(**Property, Visited))
			{
				Error = FString::Printf(
				    TEXT("Property %s.%s contains native struct %s which requires an explicit configuration adapter."),
				    *InSource.GetPathName(), *Property->GetName(), *Unsupported->GetName());
				return;
			}
			Properties.Add(MakeUnique<FStoredProperty>(**Property, &InSource));
		}
	}

	void Apply(UObject& Destination, const FObjectMap& Mapping) const
	{
		check(Destination.GetClass() == Source->GetClass());
		if (MaterialSnapshot.IsValid())
		{
			ApplyMaterial(*CastChecked<UMaterialInstanceDynamic>(&Destination), Mapping);
			return;
		}
		for (const TUniquePtr<FStoredProperty>& Property : Properties)
		{
			Property->Apply(&Destination, Mapping);
		}
	}

	void ApplyMaterial(UMaterialInstanceDynamic& Destination, const FObjectMap& Mapping) const
	{
		for (const TUniquePtr<FStoredProperty>& Property : Properties)
		{
			Property->Apply(MaterialSnapshot.Get(), Mapping);
		}
		Destination.CopyParameterOverrides(MaterialSnapshot.Get());
		Destination.PhysMaterial = MaterialSnapshot->PhysMaterial;
		for (int32 Index = 0; Index < UE_ARRAY_COUNT(Destination.PhysicalMaterialMap); ++Index)
		{
			Destination.PhysicalMaterialMap[Index] = MaterialSnapshot->PhysicalMaterialMap[Index];
		}
		Destination.RenamedTextures = MaterialSnapshot->RenamedTextures;
		for (const FTextureCollectionParameterValue& Parameter : MaterialSnapshot->TextureCollectionParameterValues)
		{
			Destination.SetTextureCollectionParameterValueByInfo(Parameter.ParameterInfo, Parameter.ParameterValue);
		}
		for (const FRuntimeVirtualTextureParameterValue& Parameter :
		     MaterialSnapshot->RuntimeVirtualTextureParameterValues)
		{
			Destination.SetRuntimeVirtualTextureParameterValueByInfo(Parameter.ParameterInfo, Parameter.ParameterValue);
		}
		for (const FSparseVolumeTextureParameterValue& Parameter : MaterialSnapshot->SparseVolumeTextureParameterValues)
		{
			Destination.SetSparseVolumeTextureParameterValue(Parameter.ParameterInfo.Name, Parameter.ParameterValue);
		}
	}

	TStrongObjectPtr<UObject> Source;
	TStrongObjectPtr<UMaterialInstanceDynamic> MaterialSnapshot;
	TArray<TUniquePtr<FStoredProperty>> Properties;
};

struct FComponentState
{
	explicit FComponentState(UActorComponent& Component)
	    : Source(&Component), ChildActor(nullptr), Parent(nullptr), Socket(NAME_None),
	      RelativeTransform(FTransform::Identity), WorldTransform(FTransform::Identity), BodyProperties(),
	      InstanceTransforms(), InstanceCustomData(), LinearVelocity(FVector::ZeroVector),
	      AngularVelocity(FVector::ZeroVector), NumCustomDataFloats(0),
	      TickInterval(Component.PrimaryComponentTick.TickInterval), bRegistered(Component.IsRegistered()),
	      bActive(Component.IsActive()), bTickEnabled(Component.IsComponentTickEnabled()), bSimulating(false),
	      bAwake(false)
	{
		if (UChildActorComponent* Child = Cast<UChildActorComponent>(&Component))
		{
			ChildActor = Child->GetChildActor();
		}
		if (USceneComponent* Scene = Cast<USceneComponent>(&Component))
		{
			Parent = Scene->GetAttachParent();
			Socket = Scene->GetAttachSocketName();
			RelativeTransform = Scene->GetRelativeTransform();
			WorldTransform = Scene->GetComponentTransform();
		}
		if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(&Component))
		{
			bSimulating = Primitive->IsSimulatingPhysics();
			if (bSimulating)
			{
				LinearVelocity = Primitive->GetPhysicsLinearVelocity();
				AngularVelocity = Primitive->GetPhysicsAngularVelocityInRadians();
				bAwake = Primitive->IsAnyRigidBodyAwake();
			}
			for (TFieldIterator<FProperty> Property(FBodyInstance::StaticStruct()); Property; ++Property)
			{
				if (!Property->HasAnyPropertyFlags(CPF_Edit))
				{
					continue;
				}
				if (Property->HasAnyPropertyFlags(CPF_Transient | CPF_Deprecated))
				{
					continue;
				}
				BodyProperties.Add(MakeUnique<FStoredProperty>(**Property, &Primitive->BodyInstance));
			}
		}
		if (UInstancedStaticMeshComponent* Instances = Cast<UInstancedStaticMeshComponent>(&Component))
		{
			for (int32 Index = 0; Index < Instances->GetInstanceCount(); ++Index)
			{
				FTransform Transform;
				Instances->GetInstanceTransform(Index, Transform, false);
				InstanceTransforms.Add(Transform);
			}
			NumCustomDataFloats = Instances->NumCustomDataFloats;
			InstanceCustomData = Instances->PerInstanceSMCustomData;
		}
	}

	TStrongObjectPtr<UActorComponent> Source;
	AActor* ChildActor;
	USceneComponent* Parent;
	FName Socket;
	FTransform RelativeTransform;
	FTransform WorldTransform;
	TArray<TUniquePtr<FStoredProperty>> BodyProperties;
	TArray<FTransform> InstanceTransforms;
	TArray<float> InstanceCustomData;
	FVector LinearVelocity;
	FVector AngularVelocity;
	int32 NumCustomDataFloats;
	float TickInterval;
	bool bRegistered;
	bool bActive;
	bool bTickEnabled;
	bool bSimulating;
	bool bAwake;
};

bool ValidateActor(AActor* Source, UWorld& World, bool bChildActor, FString& Error)
{
	if (!IsValid(Source))
	{
		Error = TEXT("The source Actor is no longer valid.");
		return false;
	}
	if (Source->IsActorBeingDestroyed())
	{
		Error = TEXT("The source Actor is being destroyed.");
		return false;
	}
	if (Source->HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		Error = TEXT("Only world Actor instances can be duplicated.");
		return false;
	}
	if (Source->GetWorld() != &World)
	{
		Error = TEXT("The source Actor belongs to another world.");
		return false;
	}
	if (Source->GetActorTransform().ContainsNaN())
	{
		Error = TEXT("The source Actor transform must be finite.");
		return false;
	}
	if (Source->IsA<AInfo>())
	{
		Error = TEXT("World management Actors require an application-specific duplication policy.");
		return false;
	}
	if (Source->IsA<AController>())
	{
		Error = TEXT("Controller Actors require an application-specific duplication policy.");
		return false;
	}
	if (Source->IsA<ABrush>())
	{
		Error = TEXT("BSP/volume Actors require an application-specific duplication policy.");
		return false;
	}
	if (const APawn* Pawn = Cast<APawn>(Source))
	{
		if (Pawn->GetController())
		{
			Error = TEXT("Unpossess the Pawn before duplicating its configuration.");
			return false;
		}
		if (Pawn->AutoPossessPlayer != EAutoReceiveInput::Disabled)
		{
			Error = TEXT("Disable automatic player possession before duplicating the Pawn.");
			return false;
		}
		if (Pawn->AutoPossessAI != EAutoPossessAI::Disabled)
		{
			Error = TEXT("Disable automatic AI possession before duplicating the Pawn.");
			return false;
		}
	}
	if (Source->IsChildActor() && !bChildActor)
	{
		Error = TEXT("Duplicate the owning Actor to include a ChildActorComponent-managed Actor.");
		return false;
	}

	TInlineComponentArray<UActorComponent*> Components(Source);
	for (UActorComponent* Component : Components)
	{
		if (Component->IsA<UPhysicsConstraintComponent>())
		{
			Error = TEXT("PhysicsConstraintComponent needs an explicit adapter to rebuild native constraint handles.");
			return false;
		}
		if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Component))
		{
			if (Primitive->IsSimulatingPhysics())
			{
				if (!Primitive->IsA<UStaticMeshComponent>())
				{
					Error = TEXT("Simulated components other than StaticMesh require a custom physics adapter.");
					return false;
				}
				if (Primitive->IsA<UInstancedStaticMeshComponent>())
				{
					Error = TEXT("Simulated instanced meshes require a custom physics adapter.");
					return false;
				}
				if (Primitive->IsWelded())
				{
					Error = TEXT("Simulated welded bodies require a custom physics adapter.");
					return false;
				}
			}
		}
	}
	return true;
}

void SplitActorName(const FString& Name, FString& Family, int64& Suffix)
{
	Family = Name;
	Suffix = 0;
	int32 Separator = INDEX_NONE;
	if (!Name.FindLastChar(TEXT('_'), Separator) || Separator == Name.Len() - 1)
	{
		return;
	}
	const FString Number = Name.Mid(Separator + 1);
	for (TCHAR Character : Number)
	{
		if (!FChar::IsDigit(Character))
		{
			return;
		}
	}
	Family = Name.Left(Separator);
	Suffix = FCString::Atoi64(*Number);
}

FName AllocateActorName(const AActor& Source, TMap<FString, int64>& Counters, FString& Error)
{
	FString Family;
	int64 LargestSuffix;
	SplitActorName(Source.GetName(), Family, LargestSuffix);
	const FString CounterKey = Source.GetLevel()->GetPathName() + TEXT("|") + Family.ToLower();
	LargestSuffix = FMath::Max(LargestSuffix, Counters.FindRef(CounterKey));
	ForEachObjectWithOuter(
	    Source.GetLevel(),
	    [&Family, &LargestSuffix](UObject* Object)
	    {
		    FString CandidateFamily;
		    int64 CandidateSuffix;
		    SplitActorName(Object->GetName(), CandidateFamily, CandidateSuffix);
		    if (CandidateFamily.Equals(Family, ESearchCase::IgnoreCase))
		    {
			    LargestSuffix = FMath::Max(LargestSuffix, CandidateSuffix);
		    }
	    },
	    false);
	if (LargestSuffix >= MAX_int32 - 1)
	{
		Error = TEXT("The source name family has exhausted its numeric suffix range.");
		return NAME_None;
	}
	Counters.Add(CounterKey, LargestSuffix + 1);
	return FName(*FString::Printf(TEXT("%s_%lld"), *Family, LargestSuffix + 1));
}

struct FActorState
{
	explicit FActorState(AActor& Actor, const FVector& Offset)
	    : Source(&Actor), ParentComponent(Actor.GetParentComponent()), Transform(Actor.GetActorTransform()),
	      TickInterval(Actor.PrimaryActorTick.TickInterval), bHidden(Actor.IsHidden()),
	      bCollision(Actor.GetActorEnableCollision()), bTickEnabled(Actor.IsActorTickEnabled())
	{
		Transform.AddToTranslation(Offset);
	}

	TStrongObjectPtr<AActor> Source;
	UChildActorComponent* ParentComponent;
	FTransform Transform;
	float TickInterval;
	bool bHidden;
	bool bCollision;
	bool bTickEnabled;
};

class FDuplicateOperation
{
public:
	FDuplicateOperation(AActor& InSource, const FVector& InOffset, TMap<FString, int64>& InCounters,
	                    TArray<TUniquePtr<IOWTDuplicationAdapter>>& InAdapters,
	                    const FOWTDuplicationPropertyPolicySet& InPolicies, bool bInDeferParticipants,
	                    FString& InError);

	bool Capture();
	bool ReserveName();
	AActor* Spawn();
	bool FinalizeHierarchy(const FObjectMap& CompleteMapping);

	const FObjectMap& GetMapping() const;

private:
	bool CaptureActor(AActor& Actor, bool bChild);
	void CaptureOwnedObject(UObject* Object, bool bInstanced);
	bool ValidateMaterial(const UMaterialInstanceDynamic& Material);
	bool ValidateMapping();

	void ResolveObjects(bool bAfterConstruction);
	void ApplyObjects();
	void RestoreAfterConstruction();
	void RestoreParticipants();
	bool RestoreActor(const FActorState& State);
	bool RestoreSceneTransform(const FComponentState& State, USceneComponent& Scene);
	bool RestoreSceneTransforms();
	bool RestoreComponents(const FActorState& ActorState, AActor& Destination);

private:
	AActor& Source;
	AActor* Result;
	UOWTDuplicationRestoreComponent* RestoreComponent;
	FVector Offset;
	TMap<FString, int64>& Counters;
	FString& Error;
	TArray<TUniquePtr<IOWTDuplicationAdapter>>& Adapters;
	const FOWTDuplicationPropertyPolicySet& Policies;
	TArray<TUniquePtr<FActorState>> Actors;
	TArray<TUniquePtr<FObjectState>> Objects;
	TArray<TUniquePtr<FComponentState>> Components;
	FObjectMap Mapping;
	TSet<UObject*> ExplicitInstancedObjects;
	FName ReservedName;
	bool bDeferParticipants;
	bool bRestored;
	bool bSucceeded;
};

FDuplicateOperation::FDuplicateOperation(AActor& InSource, const FVector& InOffset, TMap<FString, int64>& InCounters,
                                         TArray<TUniquePtr<IOWTDuplicationAdapter>>& InAdapters,
                                         const FOWTDuplicationPropertyPolicySet& InPolicies, bool bInDeferParticipants,
                                         FString& InError)
    : Source(InSource), Result(nullptr), RestoreComponent(nullptr), Offset(InOffset), Counters(InCounters),
      Error(InError), Adapters(InAdapters), Policies(InPolicies), Actors(), Objects(), Components(), Mapping(),
      ExplicitInstancedObjects(), ReservedName(NAME_None), bDeferParticipants(bInDeferParticipants), bRestored(false),
      bSucceeded(false)
{
}

bool FDuplicateOperation::Capture()
{
	if (!CaptureActor(Source, false))
	{
		return false;
	}
	for (const auto& Adapter : Adapters)
	{
		if (!Adapter->HasCapturedSubjects())
		{
			continue;
		}
		Adapter->VisitCapturedReferences(
		    [this](UObject*& Reference, bool bInstanced)
		    {
			    CaptureOwnedObject(Reference, bInstanced);
		    });
		if (!Error.IsEmpty())
		{
			return false;
		}
	}
	// The array grows when reflected state reaches another owned instanced UObject.
	for (int32 Index = 0; Index < Objects.Num(); ++Index)
	{
		FObjectState& State = *Objects[Index];
		for (const TUniquePtr<FStoredProperty>& Property : State.Properties)
		{
			VisitReferences(Property->Property, Property->Value,
			                [this](UObject*& Reference, bool bInstanced)
			                {
				                CaptureOwnedObject(Reference, bInstanced);
			                });
			if (!Error.IsEmpty())
			{
				return false;
			}
		}
	}
	for (const TUniquePtr<FObjectState>& State : Objects)
	{
		UObject* Object = State->Source.Get();
		if (Object->IsA<AActor>())
		{
			continue;
		}
		if (Object->IsA<UActorComponent>())
		{
			continue;
		}
		if (Object->IsA<UMaterialInstanceDynamic>())
		{
			continue;
		}
		if (ExplicitInstancedObjects.Contains(Object))
		{
			continue;
		}
		if (!Object->GetClass()->HasAnyClassFlags(CLASS_DefaultToInstanced))
		{
			Error = FString::Printf(TEXT("Owned object %s must be an instanced UObject to duplicate safely."),
			                        *Object->GetPathName());
			return false;
		}
	}
	return true;
}

AActor* FDuplicateOperation::Spawn()
{
	FActorSpawnParameters Parameters;
	Parameters.Name = ReservedName;
	if (!Error.IsEmpty())
	{
		return nullptr;
	}
	Parameters.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Required_ErrorAndReturnNull;
	Parameters.OverrideLevel = Source.GetLevel();
	Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Parameters.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
	Parameters.bDeferConstruction = true;
	const FTransform& Transform = Actors[0]->Transform;
	Result = Source.GetWorld()->SpawnActor<AActor>(Source.GetClass(), Transform, Parameters);
	if (!IsValid(Result))
	{
		Error = TEXT("The world could not spawn the duplicate Actor.");
		return nullptr;
	}
	Mapping[&Source] = Result;
	ResolveObjects(false);
	if (!Error.IsEmpty())
	{
		Result->Destroy();
		return nullptr;
	}
	ApplyObjects();

	RestoreComponent = NewObject<UOWTDuplicationRestoreComponent>(Result);
	Result->AddInstanceComponent(RestoreComponent);
	RestoreComponent->SetRestoreCallback(
	    [this]()
	    {
		    RestoreAfterConstruction();
	    });
	RestoreComponent->RegisterComponent();
	check(!bRestored);
	Result->FinishSpawning(Transform, true, nullptr, ESpawnActorScaleMethod::OverrideRootScale);
	if (IsValid(Result))
	{
		// Worlds without initialized Actors cannot have dispatched BeginPlay yet.
		RestoreComponent->Restore();
	}
	RestoreComponent->SetRestoreCallback(TFunction<void()>());
	RestoreComponent->DestroyComponent();
	RestoreComponent = nullptr;
	if (!IsValid(Result))
	{
		if (Error.IsEmpty())
		{
			Error = TEXT("The duplicate was destroyed during construction or initialization.");
		}
		return nullptr;
	}
	if (!bSucceeded)
	{
		Result->Destroy();
		return nullptr;
	}
	return Result;
}

bool FDuplicateOperation::CaptureActor(AActor& Actor, bool bChild)
{
	if (!ValidateActor(&Actor, *Source.GetWorld(), bChild, Error))
	{
		return false;
	}
	if (Mapping.Contains(&Actor))
	{
		Error = TEXT("The ChildActorComponent hierarchy contains a cycle.");
		return false;
	}
	Actors.Add(MakeUnique<FActorState>(Actor, Offset));
	Objects.Add(MakeUnique<FObjectState>(Actor, Adapters, Policies, Error));
	if (!Error.IsEmpty())
	{
		return false;
	}
	Mapping.Add(&Actor, nullptr);
	TInlineComponentArray<UActorComponent*> SourceComponents(&Actor);
	for (UActorComponent* Component : SourceComponents)
	{
		bool bManaged = false;
		for (const auto& Adapter : Adapters)
		{
			bManaged |= Adapter->IsManagedObject(*Component);
		}
		if (bManaged)
		{
			continue;
		}
		if (USceneComponent* Scene = Cast<USceneComponent>(Component))
		{
			if (USceneComponent* Parent = Scene->GetAttachParent())
			{
				for (const auto& Adapter : Adapters)
				{
					if (Adapter->IsManagedObject(*Parent))
					{
						Error = FString::Printf(TEXT("Authored component %s is attached to managed output %s; a "
						                             "logical attachment adapter is required."),
						                        *Scene->GetPathName(), *Parent->GetPathName());
						return false;
					}
				}
			}
		}
		Objects.Add(MakeUnique<FObjectState>(*Component, Adapters, Policies, Error));
		if (!Error.IsEmpty())
		{
			return false;
		}
		Components.Add(MakeUnique<FComponentState>(*Component));
		Mapping.Add(Component, nullptr);
	}
	for (UActorComponent* Component : SourceComponents)
	{
		if (UChildActorComponent* Child = Cast<UChildActorComponent>(Component))
		{
			if (AActor* ChildActor = Child->GetChildActor())
			{
				if (!CaptureActor(*ChildActor, true))
				{
					return false;
				}
			}
		}
	}
	return true;
}

void FDuplicateOperation::CaptureOwnedObject(UObject* Object, bool bInstanced)
{
	if (!Object)
	{
		return;
	}
	bool bAdapterOwned = false;
	for (const auto& Adapter : Adapters)
	{
		if (Adapter->IsManagedObject(*Object))
		{
			Error = FString::Printf(
			    TEXT("Authored property references managed output %s; a logical output adapter is required."),
			    *Object->GetPathName());
			return;
		}
		if (Adapter->OwnsObject(*Object))
		{
			if (Adapter->IsAuxiliary())
			{
				Error = TEXT("AdapterOwnershipConflict: auxiliary policies cannot own objects.");
				return;
			}
			if (bAdapterOwned)
			{
				Error = FString::Printf(TEXT("AdapterOwnershipConflict: multiple primary adapters claim %s."),
				                        *Object->GetPathName());
				return;
			}
			bAdapterOwned = true;
		}
	}
	if (bAdapterOwned)
	{
		return;
	}
	if (bInstanced)
	{
		ExplicitInstancedObjects.Add(Object);
	}
	if (Mapping.Contains(Object))
	{
		return;
	}
	bool bOwned = false;
	for (const TUniquePtr<FActorState>& Actor : Actors)
	{
		bOwned |= Object->IsIn(Actor->Source.Get());
	}
	if (!bOwned)
	{
		return;
	}
	if (Object->IsA<UActorComponent>() || Object->IsA<AActor>())
	{
		Error = TEXT("The hierarchy references an owned Actor/component outside its component collections.");
		return;
	}
	if (UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(Object))
	{
		if (!ValidateMaterial(*Material))
		{
			return;
		}
	}
	UObject* Outer = Object->GetOuter();
	if (!Mapping.Contains(Outer))
	{
		CaptureOwnedObject(Outer, false);
	}
	Mapping.Add(Object, nullptr);
	Objects.Add(MakeUnique<FObjectState>(*Object, Adapters, Policies, Error));
}

bool FDuplicateOperation::ValidateMaterial(const UMaterialInstanceDynamic& Material)
{
	if (Material.GetClass() != UMaterialInstanceDynamic::StaticClass())
	{
		Error = TEXT("Custom MID subclasses require an application-specific material adapter.");
		return false;
	}
	if (!Material.ParameterCollectionParameterValues.IsEmpty())
	{
		Error = TEXT("MID parameter collection overrides require a custom material adapter.");
		return false;
	}
	if (!Material.UserSceneTextureOverrides.IsEmpty())
	{
		Error = TEXT("MID user scene texture overrides require a custom material adapter.");
		return false;
	}
	if (Material.NaniteOverrideMaterial.GetOverrideMaterial())
	{
		Error = TEXT("Explicit MID Nanite overrides require a custom material adapter.");
		return false;
	}
	for (const FSparseVolumeTextureParameterValue& Parameter : Material.SparseVolumeTextureParameterValues)
	{
		if (Parameter.ParameterInfo.Association != EMaterialParameterAssociation::GlobalParameter)
		{
			Error = TEXT("Layered sparse-volume MID parameters require a custom material adapter.");
			return false;
		}
	}
	return true;
}

void FDuplicateOperation::ResolveObjects(bool bAfterConstruction)
{
	for (int32 Pass = 0; Pass < Objects.Num(); ++Pass)
	{
		bool bProgress = false;
		for (const TUniquePtr<FObjectState>& State : Objects)
		{
			UObject* Original = State->Source.Get();
			if (Original->IsA<AActor>() || IsValid(Mapping.FindRef(Original)))
			{
				continue;
			}
			UObject* NewOuter = Mapping.FindRef(Original->GetOuter());
			if (!IsValid(NewOuter))
			{
				continue;
			}
			UActorComponent* OriginalComponent = Cast<UActorComponent>(Original);
			if (UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(Original))
			{
				UMaterialInterface* Parent = Material->Parent;
				if (Mapping.Contains(Parent))
				{
					Parent = Cast<UMaterialInterface>(Mapping.FindRef(Parent));
					if (!Parent)
					{
						continue;
					}
				}
				const FName MaterialName = MakeUniqueObjectName(NewOuter, Original->GetClass(), Original->GetFName());
				Mapping[Original] = UMaterialInstanceDynamic::Create(Parent, NewOuter, MaterialName);
				bProgress = true;
				continue;
			}
			UObject* Destination = FindObjectFast<UObject>(NewOuter, Original->GetFName());
			if (Destination && (!IsValid(Destination) || Destination->GetClass() != Original->GetClass()))
			{
				Error =
				    FString::Printf(TEXT("Construction changed or destroyed subobject %s."), *Original->GetPathName());
				return;
			}
			if (!Destination)
			{
				if (OriginalComponent && !bAfterConstruction &&
				    OriginalComponent->CreationMethod != EComponentCreationMethod::Instance)
				{
					continue;
				}
				Destination = NewObject<UObject>(NewOuter, Original->GetClass(), Original->GetFName(), RF_Transient);
				if (UActorComponent* NewComponent = Cast<UActorComponent>(Destination))
				{
					NewComponent->GetOwner()->AddInstanceComponent(NewComponent);
					NewComponent->CreationMethod = OriginalComponent->CreationMethod;
				}
			}
			Mapping[Original] = Destination;
			bProgress = true;
		}
		if (!bProgress)
		{
			break;
		}
	}
}

void FDuplicateOperation::ApplyObjects()
{
	for (const TUniquePtr<FObjectState>& State : Objects)
	{
		if (UObject* Destination = Mapping.FindRef(State->Source.Get()))
		{
			if (IsValid(Destination))
			{
				for (const auto& Adapter : Adapters)
				{
					bool bApplicable = Adapter->AcceptsObject(*Destination);
					bApplicable |= Adapter->OwnsObject(*State->Source.Get());
					if (bApplicable)
					{
						Adapter->PrepareDestination(*Destination);
					}
				}
				State->Apply(*Destination, Mapping);
			}
		}
	}
}

bool FDuplicateOperation::ValidateMapping()
{
	for (const TPair<UObject*, UObject*>& Pair : Mapping)
	{
		if (!IsValid(Pair.Value))
		{
			Error = FString::Printf(TEXT("Could not reconstruct %s."), *Pair.Key->GetPathName());
			return false;
		}
	}
	return true;
}

void FDuplicateOperation::RestoreAfterConstruction()
{
	bRestored = true;
	if (!IsValid(Result))
	{
		Error = TEXT("Construction destroyed the duplicate Actor.");
		return;
	}
	check(!Result->HasActorBegunPlay());
	for (const TUniquePtr<FActorState>& Actor : Actors)
	{
		if (!RestoreActor(*Actor))
		{
			Result->Destroy();
			return;
		}
	}
	if (!ValidateMapping())
	{
		Result->Destroy();
		return;
	}
	// Cross-child references can only be remapped after every child Actor has been recreated.
	ApplyObjects();
	// Component registration/initialization callbacks may have changed transforms after their first application.
	if (!RestoreSceneTransforms())
	{
		Result->Destroy();
		return;
	}
	for (const auto& Adapter : Adapters)
	{
		if (!Adapter->HasCapturedSubjects())
		{
			continue;
		}
		if (!Adapter->Restore(Mapping, Error))
		{
			return;
		}
	}
	ApplyObjects();
	if (!bDeferParticipants)
	{
		RestoreParticipants();
		if (!Error.IsEmpty())
		{
			return;
		}
	}
	bSucceeded = true;
}

void FDuplicateOperation::RestoreParticipants()
{
	for (const TUniquePtr<FObjectState>& State : Objects)
	{
		UObject* Destination = Mapping.FindRef(State->Source.Get());
		if (Destination->GetClass()->ImplementsInterface(UOWTRuntimeDuplicationParticipant::StaticClass()))
		{
			if (!IOWTRuntimeDuplicationParticipant::Execute_RestoreRuntimeDuplicateState(
			        Destination, State->Source.Get(), Mapping, Error))
			{
				if (Error.IsEmpty())
				{
					Error = FString::Printf(TEXT("Custom duplication restoration rejected %s."),
					                        *Destination->GetPathName());
				}
				Result->Destroy();
				return;
			}
			if (!ValidateMapping())
			{
				Result->Destroy();
				return;
			}
		}
	}
}

bool FDuplicateOperation::RestoreActor(const FActorState& State)
{
	AActor* Original = State.Source.Get();
	if (State.ParentComponent)
	{
		UChildActorComponent* Parent = Cast<UChildActorComponent>(Mapping.FindRef(State.ParentComponent));
		if (!Parent)
		{
			Error = TEXT("Could not reconstruct a ChildActorComponent parent.");
			return false;
		}
		if (!Parent->GetChildActor())
		{
			Parent->CreateChildActor();
		}
		AActor* Child = Parent->GetChildActor();
		if (!IsValid(Child) || Child->GetClass() != Original->GetClass())
		{
			Error = FString::Printf(TEXT("Could not reconstruct child Actor %s."), *Original->GetPathName());
			return false;
		}
		Mapping[Original] = Child;
	}
	AActor* Destination = CastChecked<AActor>(Mapping[Original]);
	check(!Destination->HasActorBegunPlay());
	ResolveObjects(true);
	if (!Error.IsEmpty())
	{
		return false;
	}
	TSet<UActorComponent*> Retained;
	for (const TUniquePtr<FComponentState>& ComponentState : Components)
	{
		if (ComponentState->Source->GetOwner() != Original)
		{
			continue;
		}
		UActorComponent* Component = Cast<UActorComponent>(Mapping.FindRef(ComponentState->Source.Get()));
		if (!IsValid(Component))
		{
			Error = TEXT("Could not reconstruct a hierarchy component.");
			return false;
		}
		Retained.Add(Component);
		// CAC unregistration destroys the temporary child. Children are resolved after this parent is restored.
		Component->UnregisterComponent();
	}
	TInlineComponentArray<UActorComponent*> Constructed(Destination);
	for (UActorComponent* Component : Constructed)
	{
		if (Component != RestoreComponent && !Retained.Contains(Component))
		{
			Component->DestroyComponent();
		}
	}
	ApplyObjects();
	if (!RestoreComponents(State, *Destination))
	{
		return false;
	}
	if (!IsValid(Destination))
	{
		Error = TEXT("Component restoration destroyed a duplicate Actor.");
		return false;
	}
	Destination->SetOwner(Cast<AActor>(Mapping.FindRef(Original->GetOwner())));
	Destination->SetInstigator(nullptr);
	Destination->SetActorHiddenInGame(State.bHidden);
	Destination->SetActorEnableCollision(State.bCollision);
	Destination->SetActorTickInterval(State.TickInterval);
	Destination->SetActorTickEnabled(State.bTickEnabled);
	return true;
}

bool FDuplicateOperation::RestoreSceneTransform(const FComponentState& State, USceneComponent& Scene)
{
	USceneComponent* Parent = Cast<USceneComponent>(Mapping.FindRef(State.Parent));
	const FName Socket = Parent ? State.Socket : NAME_None;
	if (Scene.GetAttachParent() != Parent || Scene.GetAttachSocketName() != Socket)
	{
		Scene.DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		if (Scene.IsRegistered())
		{
			if (Parent)
			{
				if (!Scene.AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform, Socket))
				{
					Error = TEXT("A duplicate scene component rejected its captured attachment.");
					return false;
				}
			}
		}
		else
		{
			Scene.SetupAttachment(Parent, Socket);
		}
	}
	FTransform Transform = State.RelativeTransform;
	if (!Parent)
	{
		Transform = State.WorldTransform;
		Transform.AddToTranslation(Offset);
	}
	Scene.SetRelativeTransform(Transform, false, nullptr, ETeleportType::TeleportPhysics);
	if (!IsValid(&Scene))
	{
		Error = TEXT("A duplicate scene component was destroyed by its transform callback.");
		return false;
	}
	return true;
}

bool FDuplicateOperation::RestoreSceneTransforms()
{
	for (const TUniquePtr<FActorState>& Actor : Actors)
	{
		for (const TUniquePtr<FComponentState>& State : Components)
		{
			if (State->Source->GetOwner() != Actor->Source.Get())
			{
				continue;
			}
			if (USceneComponent* Scene = Cast<USceneComponent>(Mapping.FindRef(State->Source.Get())))
			{
				if (!RestoreSceneTransform(*State, *Scene))
				{
					return false;
				}
				if (!ValidateMapping())
				{
					return false;
				}
			}
		}
		AActor* Destination = CastChecked<AActor>(Mapping[Actor->Source.Get()]);
		if (USceneComponent* Root = Destination->GetRootComponent())
		{
			Root->SetWorldTransform(Actor->Transform, false, nullptr, ETeleportType::TeleportPhysics);
			if (!ValidateMapping())
			{
				return false;
			}
		}
	}
	return true;
}

bool FDuplicateOperation::RestoreComponents(const FActorState& ActorState, AActor& Destination)
{
	AActor* Original = ActorState.Source.Get();
	USceneComponent* Root = Cast<USceneComponent>(Mapping.FindRef(Original->GetRootComponent()));
	Destination.SetRootComponent(Root);
	for (const TUniquePtr<FComponentState>& State : Components)
	{
		if (State->Source->GetOwner() != Original)
		{
			continue;
		}
		UActorComponent* Component = CastChecked<UActorComponent>(Mapping[State->Source.Get()]);
		if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Component))
		{
			for (const TUniquePtr<FStoredProperty>& Property : State->BodyProperties)
			{
				Property->Apply(&Primitive->BodyInstance, Mapping);
			}
		}
		if (USceneComponent* Scene = Cast<USceneComponent>(Component))
		{
			Scene->UpdateComponentToWorld();
			if (!RestoreSceneTransform(*State, *Scene))
			{
				return false;
			}
		}
		if (UInstancedStaticMeshComponent* Instances = Cast<UInstancedStaticMeshComponent>(Component))
		{
			Instances->ClearInstances();
			Instances->SetNumCustomDataFloats(State->NumCustomDataFloats);
			for (const FTransform& Transform : State->InstanceTransforms)
			{
				Instances->AddInstance(Transform, false);
			}
			Instances->PerInstanceSMCustomData = State->InstanceCustomData;
		}
	}
	if (Root)
	{
		Root->SetWorldTransform(ActorState.Transform, false, nullptr, ETeleportType::TeleportPhysics);
	}
	for (const TUniquePtr<FComponentState>& State : Components)
	{
		if (State->Source->GetOwner() != Original)
		{
			continue;
		}
		UActorComponent* Component = CastChecked<UActorComponent>(Mapping[State->Source.Get()]);
		if (State->bRegistered)
		{
			Component->RegisterComponent();
			if (!IsValid(&Destination) || !IsValid(Component))
			{
				Error = TEXT("Component registration destroyed a duplicate object.");
				return false;
			}
			if (!Component->IsRegistered())
			{
				Error = TEXT("A duplicate component callback canceled its required registration.");
				return false;
			}
			if (Source.GetWorld()->AreActorsInitialized() && Component->bWantsInitializeComponent &&
			    !Component->HasBeenInitialized())
			{
				Component->InitializeComponent();
			}
		}
		if (UChildActorComponent* Child = Cast<UChildActorComponent>(Component))
		{
			if (!State->ChildActor)
			{
				Child->DestroyChildActor();
			}
		}
		if (State->bSimulating)
		{
			UPrimitiveComponent* Primitive = CastChecked<UPrimitiveComponent>(Component);
			Primitive->SetSimulatePhysics(true);
			Primitive->SetPhysicsLinearVelocity(State->LinearVelocity);
			Primitive->SetPhysicsAngularVelocityInRadians(State->AngularVelocity);
			if (State->bAwake)
			{
				Primitive->WakeAllRigidBodies();
			}
			else
			{
				Primitive->PutAllRigidBodiesToSleep();
			}
		}
		Component->SetActive(State->bActive);
		Component->SetComponentTickInterval(State->TickInterval);
		Component->SetComponentTickEnabled(State->bTickEnabled);
		if (!IsValid(&Destination) || !IsValid(Component))
		{
			Error = TEXT("Component activation destroyed a duplicate object.");
			return false;
		}
	}
	return true;
}

const FObjectMap& FDuplicateOperation::GetMapping() const
{
	return Mapping;
}

bool FDuplicateOperation::ReserveName()
{
	ReservedName = AllocateActorName(Source, Counters, Error);
	return Error.IsEmpty();
}

bool FDuplicateOperation::FinalizeHierarchy(const FObjectMap& CompleteMapping)
{
	Mapping = CompleteMapping;
	ApplyObjects();
	if (!RestoreSceneTransforms())
	{
		return false;
	}
	if (bDeferParticipants)
	{
		RestoreParticipants();
	}
	return Error.IsEmpty();
}

/** Collects authored sources before any destination is spawned. Adapter-owned outputs are excluded. */
class FAuthoredActorHierarchy
{
public:
	FAuthoredActorHierarchy(AActor& InSource, UWorld& InWorld, EOWTDuplicationHierarchyScope InScope,
	                        const TArray<TUniquePtr<IOWTDuplicationAdapter>>& InAdapters, FString& InError);

	bool Collect();

	bool CaptureAdapters();

	const TArray<AActor*>& GetActors() const;

	TArray<AActor*> GetRootActors() const;

private:
	bool CollectActor(AActor* Actor, bool bManagedChild);

	bool CollectAttachedActors(AActor& Actor);

	bool ValidateManagedDescendants(AActor& ManagedActor);

	bool DispatchAdapters(AActor& Actor, bool bCapture);

	bool IsManagedObject(const UObject& Object) const;

private:
	AActor& Source;
	UWorld& World;
	const TArray<TUniquePtr<IOWTDuplicationAdapter>>& Adapters;
	FString& Error;
	TArray<AActor*> Actors;
	TSet<AActor*> Visited;
	TSet<AActor*> Visiting;
	EOWTDuplicationHierarchyScope Scope;
};

FAuthoredActorHierarchy::FAuthoredActorHierarchy(AActor& InSource, UWorld& InWorld,
                                                 EOWTDuplicationHierarchyScope InScope,
                                                 const TArray<TUniquePtr<IOWTDuplicationAdapter>>& InAdapters,
                                                 FString& InError)
    : Source(InSource), World(InWorld), Adapters(InAdapters), Error(InError), Actors(), Visited(), Visiting(),
      Scope(InScope)
{
}

bool FAuthoredActorHierarchy::Collect()
{
	return CollectActor(&Source, false);
}

bool FAuthoredActorHierarchy::CaptureAdapters()
{
	for (AActor* Actor : Actors)
	{
		if (!DispatchAdapters(*Actor, true))
		{
			return false;
		}
	}
	return true;
}

const TArray<AActor*>& FAuthoredActorHierarchy::GetActors() const
{
	return Actors;
}

TArray<AActor*> FAuthoredActorHierarchy::GetRootActors() const
{
	TArray<AActor*> Roots;
	for (AActor* Actor : Actors)
	{
		UChildActorComponent* Parent = Actor->GetParentComponent();
		if (Parent)
		{
			if (Visited.Contains(Parent->GetOwner()))
			{
				continue;
			}
		}
		Roots.Add(Actor);
	}
	return Roots;
}

bool FAuthoredActorHierarchy::CollectActor(AActor* Actor, bool bManagedChild)
{
	if (Visiting.Contains(Actor))
	{
		Error = TEXT("The authored hierarchy contains an attachment cycle.");
		return false;
	}
	if (Visited.Contains(Actor))
	{
		return true;
	}
	if (!ValidateActor(Actor, World, bManagedChild, Error))
	{
		return false;
	}
	if (Actor->GetLevel() != Source.GetLevel())
	{
		Error = TEXT("An authored descendant belongs to another level.");
		return false;
	}
	if (IsManagedObject(*Actor))
	{
		Error =
		    FString::Printf(TEXT("%s is a managed output, not an authored duplication source."), *Actor->GetPathName());
		return false;
	}
	if (!DispatchAdapters(*Actor, false))
	{
		return false;
	}

	Visiting.Add(Actor);
	Visited.Add(Actor);
	Actors.Add(Actor);
	TInlineComponentArray<UChildActorComponent*> Children(Actor);
	for (UChildActorComponent* Child : Children)
	{
		AActor* ChildActor = Child->GetChildActor();
		if (!ChildActor)
		{
			continue;
		}
		if (!CollectActor(ChildActor, true))
		{
			return false;
		}
	}
	if (Scope == EOWTDuplicationHierarchyScope::AuthoredHierarchy)
	{
		if (!CollectAttachedActors(*Actor))
		{
			return false;
		}
	}
	Visiting.Remove(Actor);
	return true;
}

bool FAuthoredActorHierarchy::CollectAttachedActors(AActor& Actor)
{
	TArray<AActor*> AttachedActors;
	Actor.GetAttachedActors(AttachedActors, true, false);
	for (AActor* Child : AttachedActors)
	{
		bool bManaged = false;
		for (const auto& Adapter : Adapters)
		{
			bManaged |= Adapter->IsManagedObject(*Child);
		}
		if (bManaged)
		{
			if (!ValidateManagedDescendants(*Child))
			{
				return false;
			}
			continue;
		}
		if (!CollectActor(Child, Child->IsChildActor()))
		{
			return false;
		}
	}
	return true;
}

bool FAuthoredActorHierarchy::ValidateManagedDescendants(AActor& ManagedActor)
{
	TArray<AActor*> Descendants;
	ManagedActor.GetAttachedActors(Descendants, true, true);
	for (AActor* Descendant : Descendants)
	{
		bool bManaged = false;
		for (const auto& Adapter : Adapters)
		{
			bManaged |= Adapter->IsManagedObject(*Descendant);
		}
		if (!bManaged)
		{
			Error = TEXT("An authored Actor is attached beneath a managed output; a logical "
			             "attachment adapter is required.");
			return false;
		}
	}
	return true;
}

bool FAuthoredActorHierarchy::DispatchAdapters(AActor& Actor, bool bCapture)
{
	TArray<UObject*> Objects;
	Objects.Add(&Actor);
	TInlineComponentArray<UActorComponent*> Components(&Actor);
	for (UActorComponent* Component : Components)
	{
		if (!IsManagedObject(*Component))
		{
			Objects.Add(Component);
		}
	}

	for (UObject* Object : Objects)
	{
		for (const auto& Adapter : Adapters)
		{
			if (!Adapter->AcceptsObject(*Object))
			{
				continue;
			}
			bool bSucceeded;
			if (bCapture)
			{
				bSucceeded = Adapter->CaptureSubject(*Object, Error);
			}
			else
			{
				bSucceeded = Adapter->ValidateObject(*Object, Error);
			}
			if (!bSucceeded)
			{
				return false;
			}
		}
	}
	return true;
}

bool FAuthoredActorHierarchy::IsManagedObject(const UObject& Object) const
{
	for (const auto& Adapter : Adapters)
	{
		if (Adapter->IsManagedObject(Object))
		{
			return true;
		}
	}
	return false;
}
} // namespace OWTDuplication

using namespace OWTDuplication;

UOWTRuntimeActorDuplicator::UOWTRuntimeActorDuplicator()
    : OwningObject(), NameCounters(), AdapterRegistry(), CommittedAdapters(), bDuplicating(false), bDispatching(false)
{
}

UWorld* UOWTRuntimeActorDuplicator::GetWorld() const
{
	UObject* Owner = OwningObject.Get();
	return Owner ? Owner->GetWorld() : nullptr;
}

bool UOWTRuntimeActorDuplicator::Initialize(UObject* Owner)
{
	check(IsInGameThread());
	if (bDuplicating)
	{
		return false;
	}
	if (bDispatching)
	{
		return false;
	}
	if (!IsValid(Owner))
	{
		return false;
	}
	if (Owner != GetOuter())
	{
		return false;
	}
	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		return false;
	}
	if (World->bIsTearingDown)
	{
		return false;
	}
	OwningObject = Owner;
	{
		TGuardValue<bool> InstallingProviders(bDuplicating, true);
		RegisterAdapterProviders();
	}
	if (!OwningObject.IsValid())
	{
		Deinitialize();
		return false;
	}
	if (GetWorld() != World)
	{
		Deinitialize();
		return false;
	}
	if (!IsValid(World))
	{
		Deinitialize();
		return false;
	}
	if (World->bIsTearingDown)
	{
		Deinitialize();
		return false;
	}
	return true;
}

void UOWTRuntimeActorDuplicator::RegisterAdapterProviders()
{
	TArray<IOWTDuplicationAdapterProvider*> Providers =
	    IModularFeatures::Get().GetModularFeatureImplementations<IOWTDuplicationAdapterProvider>(
	        IOWTDuplicationAdapterProvider::GetFeatureName());
	Providers.Sort(
	    [](const IOWTDuplicationAdapterProvider& Left, const IOWTDuplicationAdapterProvider& Right)
	    {
		    return Left.GetProviderId().LexicalLess(Right.GetProviderId());
	    });
	for (const IOWTDuplicationAdapterProvider* Provider : Providers)
	{
		Provider->RegisterAdapters(AdapterRegistry);
	}
}

AActor* UOWTRuntimeActorDuplicator::DuplicateActor(AActor* Source, const FVector& WorldOffset, FString& OutError)
{
	FOWTDuplicationOptions Options;
	Options.WorldOffset = WorldOffset;
	Options.HierarchyScope = EOWTDuplicationHierarchyScope::ActorAndManagedChildren;
	return DuplicateActorWithOptions(Source, Options, FGuid::NewGuid(), OutError);
}

AActor* UOWTRuntimeActorDuplicator::DuplicateActorWithOptions(AActor* Source, const FOWTDuplicationOptions& Options,
                                                              const FGuid& OperationId, FString& OutError)
{
	check(IsInGameThread());
	OutError.Reset();
	UWorld* World = GetWorld();
	if (!World)
	{
		OutError = TEXT("Runtime duplication requires an initialized owner with a live world.");
		return nullptr;
	}
	if (World->bIsTearingDown)
	{
		OutError = TEXT("The duplication world is shutting down.");
		return nullptr;
	}
	if (bDuplicating)
	{
		OutError = TEXT("A duplication callback cannot reenter the same duplicator.");
		return nullptr;
	}
	if (World->GetNetMode() != NM_Standalone)
	{
		OutError = TEXT("Runtime duplication currently supports standalone worlds.");
		return nullptr;
	}
	if (bDispatching)
	{
		OutError = TEXT("A procedural notification cannot reenter duplication synchronously.");
		return nullptr;
	}
	if (Options.WorldOffset.ContainsNaN())
	{
		OutError = TEXT("The duplication offset must be finite.");
		return nullptr;
	}
	if (!ValidateActor(Source, *World, false, OutError))
	{
		return nullptr;
	}

	TGuardValue<bool> Guard(bDuplicating, true);
	ON_SCOPE_EXIT
	{
		if (!OwningObject.IsValid())
		{
			bDuplicating = false;
			Deinitialize();
		}
	};
	// Factories may update the owner registry; this operation retains one consistent configuration.
	const FOWTDuplicationAdapterRegistry OperationRegistry = AdapterRegistry;
	TArray<TUniquePtr<IOWTDuplicationAdapter>> Adapters = OperationRegistry.CreateAdapters();
	for (const auto& Adapter : Adapters)
	{
		if (!Adapter->Initialize(*World, OutError))
		{
			return nullptr;
		}
	}
	FAuthoredActorHierarchy Hierarchy(*Source, *World, Options.HierarchyScope, Adapters, OutError);
	if (!Hierarchy.Collect())
	{
		return nullptr;
	}
	if (!Hierarchy.CaptureAdapters())
	{
		return nullptr;
	}

	const TArray<AActor*>& AuthoredActors = Hierarchy.GetActors();
	const TArray<AActor*> Roots = Hierarchy.GetRootActors();
	TArray<TUniquePtr<FDuplicateOperation>> Operations;
	for (AActor* Root : Roots)
	{
		TUniquePtr<FDuplicateOperation> Operation =
		    MakeUnique<FDuplicateOperation>(*Root, Options.WorldOffset, NameCounters, Adapters,
		                                    OperationRegistry.GetPropertyPolicies(), Roots.Num() > 1, OutError);
		if (!Operation->Capture())
		{
			return nullptr;
		}
		Operations.Add(MoveTemp(Operation));
	}
	FObjectMap CompleteMapping;
	for (const auto& Operation : Operations)
	{
		if (!Operation->ReserveName())
		{
			return nullptr;
		}
		CompleteMapping.Append(Operation->GetMapping());
	}
	for (const auto& Adapter : Adapters)
	{
		if (!Adapter->HasCapturedSubjects())
		{
			continue;
		}
		if (!Adapter->ValidateReferences(CompleteMapping, OutError))
		{
			return nullptr;
		}
	}
	TArray<TWeakObjectPtr<AActor>> CreatedRoots;
	auto Rollback = [&]()
	{
		for (const auto& Adapter : Adapters)
		{
			if (Adapter->HasCapturedSubjects())
			{
				Adapter->Rollback(CompleteMapping);
			}
		}
		for (const TWeakObjectPtr<AActor>& Root : CreatedRoots)
		{
			if (AActor* Actor = Root.Get())
			{
				Actor->Destroy();
			}
		}
	};
	for (const auto& Operation : Operations)
	{
		AActor* Duplicate = Operation->Spawn();
		CompleteMapping.Append(Operation->GetMapping());
		if (!Duplicate)
		{
			Rollback();
			return nullptr;
		}
		CreatedRoots.Add(Duplicate);
		for (AActor* Original : AuthoredActors)
		{
			if (!IsValid(Original))
			{
				OutError = TEXT("A source Actor was destroyed by a construction or restoration callback.");
				Rollback();
				return nullptr;
			}
			if (Original->IsActorBeingDestroyed())
			{
				OutError = TEXT("A source Actor is being destroyed after a user callback.");
				Rollback();
				return nullptr;
			}
		}
		if (Options.IsCancellationRequested)
		{
			if (Options.IsCancellationRequested())
			{
				OutError = TEXT("Cancelled: the editing operation ended during construction or restoration.");
				Rollback();
				return nullptr;
			}
		}
		if (GetWorld() != World)
		{
			OutError = TEXT("The duplicator owner was released during duplication.");
			Rollback();
			return nullptr;
		}
	}
	if (Roots.Num() > 1)
	{
		for (const auto& Adapter : Adapters)
		{
			if (!Adapter->HasCapturedSubjects())
			{
				continue;
			}
			if (!Adapter->Restore(CompleteMapping, OutError))
			{
				Rollback();
				return nullptr;
			}
		}
		for (const auto& Operation : Operations)
		{
			if (!Operation->FinalizeHierarchy(CompleteMapping))
			{
				Rollback();
				return nullptr;
			}
			CompleteMapping.Append(Operation->GetMapping());
		}
	}
	AActor* Result = Cast<AActor>(CompleteMapping.FindRef(Source));
	if (Options.IsCancellationRequested)
	{
		if (Options.IsCancellationRequested())
		{
			OutError = TEXT("Cancelled: the editing operation ended before authored commit.");
			Rollback();
			return nullptr;
		}
	}
	if (!IsValid(Result))
	{
		OutError = TEXT("The final authored duplicate is no longer valid.");
		Rollback();
		return nullptr;
	}
	CommitAdapters(Adapters, CompleteMapping, *Result, Options, OperationId);
	return Result;
}

void UOWTRuntimeActorDuplicator::CommitAdapters(TArray<TUniquePtr<IOWTDuplicationAdapter>>& Adapters,
                                                const TMap<UObject*, UObject*>& CompleteMapping, AActor& Result,
                                                const FOWTDuplicationOptions& Options, const FGuid& OperationId)
{
	// Install observations before announcing authored commit; activation can now report subsequent changes.
	const int32 FirstCommittedAdapter = CommittedAdapters.Num();
	for (auto& Adapter : Adapters)
	{
		if (!Adapter->HasCapturedSubjects())
		{
			continue;
		}
		Adapter->OnProceduralChanged.AddUObject(this, &UOWTRuntimeActorDuplicator::ForwardProceduralChanged);
		IOWTDuplicationAdapter* Preparing = Adapter.Get();
		CommittedAdapters.Add(MoveTemp(Adapter));
		Preparing->PrepareCommit(OperationId, Options, CompleteMapping);
	}
	if (Options.OnAuthoredCommitted)
	{
		Options.OnAuthoredCommitted(&Result);
	}
	for (int32 Index = FirstCommittedAdapter; Index < CommittedAdapters.Num(); ++Index)
	{
		CommittedAdapters[Index]->Commit(OperationId, Options, CompleteMapping);
	}
	for (int32 Index = CommittedAdapters.Num() - 1; Index >= FirstCommittedAdapter; --Index)
	{
		if (CommittedAdapters[Index]->GetProceduralComponents().IsEmpty())
		{
			CommittedAdapters.RemoveAt(Index);
		}
	}
}

void UOWTRuntimeActorDuplicator::ForwardProceduralChanged(const FOWTProceduralComponentSnapshot& Snapshot)
{
	TGuardValue<bool> Guard(bDispatching, true);
	OnProceduralChanged.Broadcast(Snapshot);
}

void UOWTRuntimeActorDuplicator::Tick(float DeltaTime)
{
	if (bDuplicating)
	{
		return;
	}
	if (bDispatching)
	{
		return;
	}
	if (!OwningObject.IsValid())
	{
		Deinitialize();
		return;
	}
	TGuardValue<bool> Guard(bDispatching, true);
	for (const auto& Adapter : CommittedAdapters)
	{
		Adapter->Tick(DeltaTime);
		if (!OwningObject.IsValid())
		{
			break;
		}
	}
	for (int32 Index = CommittedAdapters.Num() - 1; Index >= 0; --Index)
	{
		bool bHasLiveComponents = false;
		for (const FOWTProceduralComponentSnapshot& Snapshot : CommittedAdapters[Index]->GetProceduralComponents())
		{
			if (Snapshot.Component.IsValid())
			{
				bHasLiveComponents = true;
				break;
			}
		}
		if (!bHasLiveComponents)
		{
			CommittedAdapters.RemoveAt(Index);
		}
	}
}

TArray<FOWTProceduralComponentSnapshot> UOWTRuntimeActorDuplicator::GetProceduralComponents() const
{
	TArray<FOWTProceduralComponentSnapshot> Result;
	for (const auto& Adapter : CommittedAdapters)
	{
		Result.Append(Adapter->GetProceduralComponents());
	}
	return Result;
}

FOWTDuplicationAdapterRegistry& UOWTRuntimeActorDuplicator::GetAdapterRegistry()
{
	return AdapterRegistry;
}

void UOWTRuntimeActorDuplicator::Deinitialize()
{
	OwningObject.Reset();
	if (bDuplicating)
	{
		return;
	}
	if (bDispatching)
	{
		return;
	}
	for (const auto& Adapter : CommittedAdapters)
	{
		Adapter->Shutdown();
	}
	CommittedAdapters.Reset();
	NameCounters.Reset();
}

void UOWTRuntimeActorDuplicator::BeginDestroy()
{
	Deinitialize();
	Super::BeginDestroy();
}
