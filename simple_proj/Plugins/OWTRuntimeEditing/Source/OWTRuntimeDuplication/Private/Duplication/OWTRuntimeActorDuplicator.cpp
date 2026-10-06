#include "Duplication/OWTRuntimeActorDuplicator.h"

#include "Duplication/OWTDuplicationRestoreComponent.h"
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
#include "GameFramework/Controller.h"
#include "GameFramework/Info.h"
#include "GameFramework/Pawn.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PhysicsEngine/BodyInstance.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "UObject/StrongObjectPtr.h"
#include "UObject/UObjectHash.h"
#include "UObject/UnrealType.h"

namespace
{
using FObjectMap = TMap<UObject*, UObject*>;
constexpr EPropertyFlags ExcludedSnapshotProperties = CPF_Transient | CPF_DuplicateTransient |
                                                      CPF_NonPIEDuplicateTransient | CPF_Deprecated | CPF_EditorOnly |
                                                      CPF_SkipSerialization;

bool IsStoredProperty(const FProperty& Property, const UObject& Object)
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

	const FName Name = Property.GetFName();
	if (Object.IsA<UMaterialInstanceDynamic>())
	{
		// MID native renderer resources are recreated by its public API, never copied reflectively.
		return Name == TEXT("Parent") || Name == TEXT("PhysMaterial") || Name == TEXT("PhysicalMaterialMap") ||
		       Name == TEXT("ScalarParameterValues") || Name == TEXT("VectorParameterValues") ||
		       Name == TEXT("DoubleVectorParameterValues") || Name == TEXT("TextureParameterValues") ||
		       Name == TEXT("FontParameterValues") || Name == TEXT("TextureCollectionParameterValues") ||
		       Name == TEXT("RuntimeVirtualTextureParameterValues") ||
		       Name == TEXT("SparseVolumeTextureParameterValues");
	}
	if (Name == TEXT("PrimaryActorTick"))
	{
		return false;
	}
	if (Name == TEXT("PrimaryComponentTick"))
	{
		return false;
	}
	if (Name == TEXT("BodyInstance"))
	{
		// Physics handles are native members. Only its reflected configuration is copied separately.
		return false;
	}

	const UClass* DeclaringClass = Property.GetOwner<UClass>();
	if (DeclaringClass == USceneComponent::StaticClass())
	{
		// Transform setters must update ComponentToWorld and rotation caches together.
		if (Name == TEXT("RelativeLocation"))
		{
			return false;
		}
		if (Name == TEXT("RelativeRotation"))
		{
			return false;
		}
		if (Name == TEXT("RelativeScale3D"))
		{
			return false;
		}
	}
	if (DeclaringClass == AActor::StaticClass())
	{
		return Name == TEXT("Tags") || Name == TEXT("bCanBeDamaged") || Name == TEXT("bRelevantForLevelBounds") ||
		       Name == TEXT("bFindCameraComponentWhenViewTarget") || Name == TEXT("bIgnoresOriginShifting") ||
		       Name == TEXT("bGenerateOverlapEventsDuringLevelStreaming");
	}
	if (Object.IsA<UActorComponent>())
	{
		if (Name == TEXT("CreationMethod"))
		{
			return false;
		}
		if (Name == TEXT("ComponentTags"))
		{
			return true;
		}
		if (Property.HasAnyPropertyFlags(CPF_Edit))
		{
			return true;
		}
		return DeclaringClass->GetOutermost()->GetName() != TEXT("/Script/Engine");
	}

	// Actor-derived user fields and instanced UObject values are state, whereas AActor's base fields are lifecycle.
	return true;
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
		if (Struct->GetFName() == TEXT("InstancedStruct") || Struct->GetFName() == TEXT("InstancedPropertyBag"))
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
	FObjectState(UObject& InSource, FString& Error) : Source(&InSource), MaterialSnapshot(nullptr), Properties()
	{
		if (UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(&InSource))
		{
			MaterialSnapshot.Reset(UMaterialInstanceDynamic::Create(Material->Parent, GetTransientPackage()));
			MaterialSnapshot->RenamedTextures = Material->RenamedTextures;
		}
		for (TFieldIterator<FProperty> Property(InSource.GetClass()); Property; ++Property)
		{
			if (!IsStoredProperty(**Property, InSource))
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
	FDuplicateOperation(AActor& InSource, const FVector& InOffset, TMap<FString, int64>& InCounters, FString& InError)
	    : Source(InSource), Result(nullptr), RestoreComponent(nullptr), Offset(InOffset), Counters(InCounters),
	      Error(InError), Actors(), Objects(), Components(), Mapping(), ExplicitInstancedObjects(), bRestored(false),
	      bSucceeded(false)
	{
	}

	bool Capture()
	{
		if (!CaptureActor(Source, false))
		{
			return false;
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

	AActor* Spawn()
	{
		FActorSpawnParameters Parameters;
		Parameters.Name = AllocateActorName(Source, Counters, Error);
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

private:
	bool CaptureActor(AActor& Actor, bool bChild)
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
		Objects.Add(MakeUnique<FObjectState>(Actor, Error));
		if (!Error.IsEmpty())
		{
			return false;
		}
		Mapping.Add(&Actor, nullptr);
		TInlineComponentArray<UActorComponent*> SourceComponents(&Actor);
		for (UActorComponent* Component : SourceComponents)
		{
			Objects.Add(MakeUnique<FObjectState>(*Component, Error));
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

	void CaptureOwnedObject(UObject* Object, bool bInstanced)
	{
		if (!Object)
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
		Objects.Add(MakeUnique<FObjectState>(*Object, Error));
	}

	bool ValidateMaterial(const UMaterialInstanceDynamic& Material)
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

	void ResolveObjects(bool bAfterConstruction)
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
					const FName MaterialName =
					    MakeUniqueObjectName(NewOuter, Original->GetClass(), Original->GetFName());
					Mapping[Original] = UMaterialInstanceDynamic::Create(Parent, NewOuter, MaterialName);
					bProgress = true;
					continue;
				}
				UObject* Destination = FindObjectFast<UObject>(NewOuter, Original->GetFName());
				if (Destination && (!IsValid(Destination) || Destination->GetClass() != Original->GetClass()))
				{
					Error = FString::Printf(TEXT("Construction changed or destroyed subobject %s."),
					                        *Original->GetPathName());
					return;
				}
				if (!Destination)
				{
					if (OriginalComponent && !bAfterConstruction &&
					    OriginalComponent->CreationMethod != EComponentCreationMethod::Instance)
					{
						continue;
					}
					Destination =
					    NewObject<UObject>(NewOuter, Original->GetClass(), Original->GetFName(), RF_Transient);
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

	void ApplyObjects()
	{
		for (const TUniquePtr<FObjectState>& State : Objects)
		{
			if (UObject* Destination = Mapping.FindRef(State->Source.Get()))
			{
				if (IsValid(Destination))
				{
					State->Apply(*Destination, Mapping);
				}
			}
		}
	}

	bool ValidateMapping()
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

	void RestoreAfterConstruction()
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
		bSucceeded = true;
	}

	bool RestoreActor(const FActorState& State)
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

	bool RestoreSceneTransform(const FComponentState& State, USceneComponent& Scene)
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

	bool RestoreSceneTransforms()
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

	bool RestoreComponents(const FActorState& ActorState, AActor& Destination)
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

	AActor& Source;
	AActor* Result;
	UOWTDuplicationRestoreComponent* RestoreComponent;
	FVector Offset;
	TMap<FString, int64>& Counters;
	FString& Error;
	TArray<TUniquePtr<FActorState>> Actors;
	TArray<TUniquePtr<FObjectState>> Objects;
	TArray<TUniquePtr<FComponentState>> Components;
	FObjectMap Mapping;
	TSet<UObject*> ExplicitInstancedObjects;
	bool bRestored;
	bool bSucceeded;
};
} // namespace

UOWTRuntimeActorDuplicator::UOWTRuntimeActorDuplicator() : OwningObject(), NameCounters(), bDuplicating(false)
{
}

UWorld* UOWTRuntimeActorDuplicator::GetWorld() const
{
	UObject* Owner = OwningObject.Get();
	return Owner ? Owner->GetWorld() : nullptr;
}

bool UOWTRuntimeActorDuplicator::Initialize(UObject* Owner)
{
	if (bDuplicating)
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
	return true;
}

AActor* UOWTRuntimeActorDuplicator::DuplicateActor(AActor* Source, const FVector& WorldOffset, FString& OutError)
{
	check(IsInGameThread());
	OutError.Reset();
	UWorld* World = GetWorld();
	if (!World || World->bIsTearingDown)
	{
		OutError = TEXT("Runtime duplication requires an initialized owner with a live world.");
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
	if (WorldOffset.ContainsNaN())
	{
		OutError = TEXT("The duplication offset must be finite.");
		return nullptr;
	}
	if (!ValidateActor(Source, *World, false, OutError))
	{
		return nullptr;
	}

	TGuardValue<bool> Guard(bDuplicating, true);
	FDuplicateOperation Operation(*Source, WorldOffset, NameCounters, OutError);
	if (!Operation.Capture())
	{
		return nullptr;
	}
	AActor* Duplicate = Operation.Spawn();
	if (Duplicate && GetWorld() != World)
	{
		Duplicate->Destroy();
		OutError = TEXT("The duplicator owner was released during duplication.");
		return nullptr;
	}
	return Duplicate;
}

void UOWTRuntimeActorDuplicator::Deinitialize()
{
	OwningObject.Reset();
	NameCounters.Reset();
}
