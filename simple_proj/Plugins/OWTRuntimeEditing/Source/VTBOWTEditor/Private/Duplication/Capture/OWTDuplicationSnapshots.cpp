#include "Duplication/Capture/OWTDuplicationSnapshots.h"

#include "Components/ChildActorComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "Duplication/Pipeline/OWTAdapterDispatcher.h"
#include "Duplication/Properties/OWTPropertyFilter.h"
#include "Duplication/Properties/OWTPropertyValue.h"
#include "Duplication/Properties/OWTUnsupportedStructCache.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PhysicsEngine/BodyInstance.h"
#include "UObject/UnrealType.h"

TUniquePtr<FOWTObjectSnapshot> FOWTObjectSnapshot::Capture(UObject& Source, const FOWTDuplicationContext& Context)
{
	TUniquePtr<FOWTObjectSnapshot> Snapshot(new FOWTObjectSnapshot(Source));
	bool bCaptured = true;
	for (TFieldIterator<FProperty> Property(Source.GetClass()); Property && bCaptured; ++Property)
	{
		bool bAdapterOwned = false;
		const bool bCopied = FOWTPropertyFilter::IsCopied(**Property, Source, Context.PropertyPolicies);
		bCaptured = !bCopied || Context.Adapters.ResolvePropertyOwner(Source, **Property, bAdapterOwned, Context.Error);
		const UScriptStruct* Unsupported = bCopied && bCaptured && !bAdapterOwned ? Context.UnsupportedStructs.Find(**Property) : nullptr;
		if (Unsupported)
		{
			Context.Error = FString::Printf(
			    TEXT("Property %s.%s contains native struct %s which requires an explicit configuration adapter."),
			    *Source.GetPathName(), *Property->GetName(), *Unsupported->GetName());
			bCaptured = false;
		}
		else if (bCopied && bCaptured && !bAdapterOwned)
		{
			Snapshot->Properties.Add(MakeUnique<FOWTPropertyValue>(**Property, &Source));
		}
	}
	return bCaptured ? MoveTemp(Snapshot) : nullptr;
}

FOWTObjectSnapshot::FOWTObjectSnapshot(UObject& InSource) : Source(&InSource), MaterialSnapshot(nullptr), Properties()
{
	if (UMaterialInstanceDynamic* Material = Cast<UMaterialInstanceDynamic>(&InSource))
	{
		MaterialSnapshot.Reset(UMaterialInstanceDynamic::Create(Material->Parent, GetTransientPackage()));
		MaterialSnapshot->RenamedTextures = Material->RenamedTextures;
	}
}

FOWTObjectSnapshot::~FOWTObjectSnapshot() = default;

void FOWTObjectSnapshot::ApplyTo(UObject& Destination, const FOWTObjectMap& Mapping) const
{
	check(Destination.GetClass() == Source->GetClass());
	if (MaterialSnapshot.IsValid())
	{
		ApplyToMaterial(*CastChecked<UMaterialInstanceDynamic>(&Destination), Mapping);
	}
	else
	{
		for (const TUniquePtr<FOWTPropertyValue>& Property : Properties)
		{
			Property->ApplyTo(&Destination, Mapping);
		}
	}
}

void FOWTObjectSnapshot::ApplyToMaterial(UMaterialInstanceDynamic& Destination, const FOWTObjectMap& Mapping) const
{
	for (const TUniquePtr<FOWTPropertyValue>& Property : Properties)
	{
		Property->ApplyTo(MaterialSnapshot.Get(), Mapping);
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

FOWTComponentSnapshot::FOWTComponentSnapshot(UActorComponent& Component)
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
		// Only editable body settings are configuration; the remaining fields mirror the live physics body.
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
			BodyProperties.Add(MakeUnique<FOWTPropertyValue>(**Property, &Primitive->BodyInstance));
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

FOWTComponentSnapshot::~FOWTComponentSnapshot() = default;

FOWTActorSnapshot::FOWTActorSnapshot(AActor& Actor, const FVector& Offset)
    : Source(&Actor), ParentComponent(Actor.GetParentComponent()), Transform(Actor.GetActorTransform()),
      TickInterval(Actor.PrimaryActorTick.TickInterval), bHidden(Actor.IsHidden()),
      bCollision(Actor.GetActorEnableCollision()), bTickEnabled(Actor.IsActorTickEnabled())
{
	Transform.AddToTranslation(Offset);
}

FOWTRootSnapshot::FOWTRootSnapshot(AActor& InRoot) : Root(InRoot), Actors(), Objects(), Components(), Mapping()
{
}

FOWTRootSnapshot::~FOWTRootSnapshot() = default;
