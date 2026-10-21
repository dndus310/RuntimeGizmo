#include "Duplication/PCG/OWTPCGConfigurationSnapshot.h"

#include "Duplication/PCG/OWTPCGAuthoredProperties.h"
#include "PCGComponent.h"
#include "PCGGraph.h"
#include "RuntimeGen/SchedulingPolicies/PCGSchedulingPolicyBase.h"
#include "Subsystems/PCGSubsystem.h"
#include "UObject/UObjectHash.h"

FOWTPCGConfigurationSnapshot::FOWTPCGConfigurationSnapshot(UPCGComponent& Component)
    : Source(&Component), Configuration(NewObject<UPCGComponent>(GetTransientPackage())), PolicyCopies(),
      bWasGenerated(Component.bGenerated), bActivated(Component.bActivated)
{
	// A runtime trigger may have been changed from native code before the source creates its policy.
	// Capture the public configuration without the engine's source-policy ensure or any source mutation.
	// The engine restore API also overwrites inherited tags, so capture them alongside the base schema.
	Configuration->ComponentTags = Component.ComponentTags;
	FOWTPCGAuthoredProperties::Copy(*UPCGComponent::StaticClass(), &Component, Configuration.Get());

	// Instanced graph and scheduling policy have ownership and refresh contracts beyond reflected values.
	Configuration->GetGraphInstance()->SetGraph(Component.GetGraphInstance()->Graph);
	Configuration->GetGraphInstance()->CopyParameterOverrides(Component.GetGraphInstance());
	Configuration->SetSchedulingPolicyClass(Component.SchedulingPolicyClass);
	if (Component.SchedulingPolicy)
	{
		Configuration->SchedulingPolicy = CopySchedulingPolicy(*Component.SchedulingPolicy, *Configuration, PolicyCopies);
	}

	// Commit restores activation only after authored objects and references are ready.
	Configuration->bActivated = false;
	if (UPCGSubsystem* Subsystem = Component.GetSubsystem())
	{
		Subsystem->ForAllRegisteredLocalComponents(&Component, [this](UPCGComponent* Local)
		{
			bWasGenerated |= Local && Local->bGenerated;
		});
	}
}

UPCGSchedulingPolicyBase* FOWTPCGConfigurationSnapshot::CopySchedulingPolicy(UPCGSchedulingPolicyBase& Policy,
                                                                             UPCGComponent& Owner,
                                                                             TMap<UObject*, UObject*>& Copies)
{
	FObjectDuplicationParameters Parameters(&Policy, &Owner);
	Parameters.DestName = MakeUniqueObjectName(&Owner, Policy.GetClass());
	Parameters.CreatedObjects = &Copies;
	Parameters.FlagMask &= ~RF_Standalone;
	return CastChecked<UPCGSchedulingPolicyBase>(StaticDuplicateObjectEx(Parameters));
}

TArray<UObject*> FOWTPCGConfigurationSnapshot::GetSchedulingPolicyObjects(UPCGSchedulingPolicyBase* Policy)
{
	TArray<UObject*> Objects;
	if (Policy)
	{
		Objects.Add(Policy);
		GetObjectsWithOuter(Policy, Objects, true);
	}
	return Objects;
}
