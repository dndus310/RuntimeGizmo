#include "Duplication/OWTBuiltinDuplicationProviders.h"

#include "Duplication/OWTDuplicationAdapter.h"
#include "Duplication/OWTDuplicationAdapterProvider.h"
#include "Duplication/OWTPCGDuplicationAdapter.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/UnrealType.h"

namespace OWTDuplication
{
TOptional<bool> ResolveObjectProperty(const UObject& Object, const FProperty& Property)
{
	const FName Name = Property.GetFName();
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
		return false;
	}
	return {};
}

TOptional<bool> ResolveActorProperty(const UObject& Object, const FProperty& Property)
{
	if (Property.GetOwner<UClass>() != AActor::StaticClass())
	{
		return {};
	}
	// AActor's remaining base properties participate in spawn/world lifecycle, not authored state.
	static const TSet<FName> AuthoringProperties = {TEXT("Tags"),
	                                                TEXT("bCanBeDamaged"),
	                                                TEXT("bRelevantForLevelBounds"),
	                                                TEXT("bFindCameraComponentWhenViewTarget"),
	                                                TEXT("bIgnoresOriginShifting"),
	                                                TEXT("bGenerateOverlapEventsDuringLevelStreaming")};
	return AuthoringProperties.Contains(Property.GetFName());
}

TOptional<bool> ResolveComponentProperty(const UObject& Object, const FProperty& Property)
{
	const TOptional<bool> CommonResult = ResolveObjectProperty(Object, Property);
	if (CommonResult.IsSet())
	{
		return CommonResult;
	}
	const FName Name = Property.GetFName();
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
	const UClass* DeclaringClass = Property.GetOwner<UClass>();
	check(DeclaringClass);
	return DeclaringClass->GetOutermost()->GetName() != TEXT("/Script/Engine");
}

TOptional<bool> ResolveSceneProperty(const UObject& Object, const FProperty& Property)
{
	if (Property.GetOwner<UClass>() != USceneComponent::StaticClass())
	{
		return {};
	}
	// SetRelativeTransform restores these together with ComponentToWorld and rotation caches.
	const FName Name = Property.GetFName();
	if (Name == USceneComponent::GetRelativeLocationPropertyName())
	{
		return false;
	}
	if (Name == USceneComponent::GetRelativeRotationPropertyName())
	{
		return false;
	}
	if (Name == USceneComponent::GetRelativeScale3DPropertyName())
	{
		return false;
	}
	return {};
}

TOptional<bool> ResolveMaterialProperty(const UObject& Object, const FProperty& Property)
{
	// Engine-version contract: native renderer resources are rebuilt through MID APIs.
	static const TSet<FName> AuthoringProperties = {TEXT("Parent"),
	                                                TEXT("PhysMaterial"),
	                                                TEXT("PhysicalMaterialMap"),
	                                                TEXT("ScalarParameterValues"),
	                                                TEXT("VectorParameterValues"),
	                                                TEXT("DoubleVectorParameterValues"),
	                                                TEXT("TextureParameterValues"),
	                                                TEXT("FontParameterValues"),
	                                                TEXT("TextureCollectionParameterValues"),
	                                                TEXT("RuntimeVirtualTextureParameterValues"),
	                                                TEXT("SparseVolumeTextureParameterValues")};
	return AuthoringProperties.Contains(Property.GetFName());
}

class FEngineDuplicationProvider final : public IOWTDuplicationAdapterProvider
{
public:
	virtual FName GetProviderId() const override;

	virtual void RegisterAdapters(FOWTDuplicationAdapterRegistry& Registry) const override;
};

FName FEngineDuplicationProvider::GetProviderId() const
{
	return TEXT("OWT.EngineConfiguration");
}

void FEngineDuplicationProvider::RegisterAdapters(FOWTDuplicationAdapterRegistry& Registry) const
{
	FOWTDuplicationPropertyPolicySet& Policies = Registry.GetPropertyPolicies();
	Policies.Register(UObject::StaticClass(), ResolveObjectProperty);
	Policies.Register(AActor::StaticClass(), ResolveActorProperty);
	Policies.Register(UActorComponent::StaticClass(), ResolveComponentProperty);
	Policies.Register(USceneComponent::StaticClass(), ResolveSceneProperty);
	Policies.Register(UMaterialInstanceDynamic::StaticClass(), ResolveMaterialProperty);
}

class FPCGDuplicationProvider final : public IOWTDuplicationAdapterProvider
{
public:
	virtual FName GetProviderId() const override;

	virtual void RegisterAdapters(FOWTDuplicationAdapterRegistry& Registry) const override;
};

FName FPCGDuplicationProvider::GetProviderId() const
{
	return TEXT("OWT.PCG");
}

void FPCGDuplicationProvider::RegisterAdapters(FOWTDuplicationAdapterRegistry& Registry) const
{
	RegisterOWTPCGDuplicationAdapter(Registry);
}
} // namespace OWTDuplication

TUniquePtr<IOWTDuplicationAdapterProvider> CreateOWTEngineDuplicationProvider()
{
	return MakeUnique<OWTDuplication::FEngineDuplicationProvider>();
}

TUniquePtr<IOWTDuplicationAdapterProvider> CreateOWTPCGDuplicationProvider()
{
	return MakeUnique<OWTDuplication::FPCGDuplicationProvider>();
}
