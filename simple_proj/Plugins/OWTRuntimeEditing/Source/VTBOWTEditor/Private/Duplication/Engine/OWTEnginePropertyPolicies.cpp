#include "Duplication/Engine/OWTEnginePropertyPolicies.h"

#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Duplication/OWTDuplicationPropertyPolicy.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/UnrealType.h"

void FOWTEnginePropertyPolicies::Register(FOWTDuplicationPropertyPolicySet& Policies)
{
	Policies.Register(UObject::StaticClass(), ResolveObjectProperty);
	Policies.Register(AActor::StaticClass(), ResolveActorProperty);
	Policies.Register(UActorComponent::StaticClass(), ResolveComponentProperty);
	Policies.Register(USceneComponent::StaticClass(), ResolveSceneProperty);
	Policies.Register(UMaterialInstanceDynamic::StaticClass(), ResolveMaterialProperty);
}

TOptional<bool> FOWTEnginePropertyPolicies::ResolveObjectProperty(const UObject& Object, const FProperty& Property)
{
	// Tick functions and body instances are restored through engine APIs, not copied as values.
	static const TSet<FName> LifecycleProperties = {TEXT("PrimaryActorTick"), TEXT("PrimaryComponentTick"),
	                                                TEXT("BodyInstance")};
	return LifecycleProperties.Contains(Property.GetFName()) ? TOptional<bool>(false) : TOptional<bool>();
}

TOptional<bool> FOWTEnginePropertyPolicies::ResolveActorProperty(const UObject& Object, const FProperty& Property)
{
	// AActor's remaining base properties participate in spawn/world lifecycle, not authored state.
	static const TSet<FName> AuthoringProperties = {TEXT("Tags"),
	                                                TEXT("bCanBeDamaged"),
	                                                TEXT("bRelevantForLevelBounds"),
	                                                TEXT("bFindCameraComponentWhenViewTarget"),
	                                                TEXT("bIgnoresOriginShifting"),
	                                                TEXT("bGenerateOverlapEventsDuringLevelStreaming")};
	const bool bDeclaredByActor = Property.GetOwner<UClass>() == AActor::StaticClass();
	return bDeclaredByActor ? TOptional<bool>(AuthoringProperties.Contains(Property.GetFName())) : TOptional<bool>();
}

TOptional<bool> FOWTEnginePropertyPolicies::ResolveComponentProperty(const UObject& Object, const FProperty& Property)
{
	const TOptional<bool> CommonDecision = ResolveObjectProperty(Object, Property);
	const FName Name = Property.GetFName();
	const UClass* DeclaringClass = Property.GetOwner<UClass>();
	check(DeclaringClass);
	// Hidden engine fields are runtime bookkeeping; hidden fields of project classes are user state.
	const bool bProjectProperty = DeclaringClass->GetOutermost()->GetName() != TEXT("/Script/Engine");

	TOptional<bool> Decision = CommonDecision;
	if (!CommonDecision.IsSet())
	{
		const bool bCreationMethod = Name == TEXT("CreationMethod");
		const bool bAuthored = Name == TEXT("ComponentTags") || Property.HasAnyPropertyFlags(CPF_Edit) || bProjectProperty;
		Decision = !bCreationMethod && bAuthored;
	}
	return Decision;
}

TOptional<bool> FOWTEnginePropertyPolicies::ResolveSceneProperty(const UObject& Object, const FProperty& Property)
{
	// SetRelativeTransform restores these together with ComponentToWorld and rotation caches.
	const FName Name = Property.GetFName();
	const bool bDeclaredByScene = Property.GetOwner<UClass>() == USceneComponent::StaticClass();
	const bool bRelativeTransform = Name == USceneComponent::GetRelativeLocationPropertyName() ||
	                                Name == USceneComponent::GetRelativeRotationPropertyName() ||
	                                Name == USceneComponent::GetRelativeScale3DPropertyName();
	return bDeclaredByScene && bRelativeTransform ? TOptional<bool>(false) : TOptional<bool>();
}

TOptional<bool> FOWTEnginePropertyPolicies::ResolveMaterialProperty(const UObject& Object, const FProperty& Property)
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
