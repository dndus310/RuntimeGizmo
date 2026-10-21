#include "Duplication/PCG/OWTPCGReferenceRemapper.h"

#include "Duplication/PCG/OWTPCGAuthoredProperties.h"
#include "Duplication/PCG/OWTPCGConfigurationSnapshot.h"
#include "Duplication/Properties/OWTPropertyFilter.h"
#include "Duplication/Properties/OWTReferenceVisitor.h"
#include "PCGComponent.h"
#include "PCGGraph.h"
#include "UObject/UnrealType.h"

FOWTPCGReferenceRemapper::FOWTPCGReferenceRemapper(TFunction<bool(const UObject&)> InIsManagedObject)
    : IsManagedObject(MoveTemp(InIsManagedObject)), UnsupportedStructs()
{
}

void FOWTPCGReferenceRemapper::VisitCapturedReferences(UPCGComponent& Configuration,
                                                       TFunctionRef<void(UObject*&, bool)> Visitor)
{
	for (TFieldIterator<FProperty> Property(UPCGComponent::StaticClass(), EFieldIteratorFlags::ExcludeSuper); Property; ++Property)
	{
		if (IsReflectedConfigurationProperty(**Property))
		{
			FOWTReferenceVisitor::Visit(**Property, Property->ContainerPtrToValuePtr<void>(&Configuration), Visitor);
		}
	}
	for (UObject* PolicyObject : FOWTPCGConfigurationSnapshot::GetSchedulingPolicyObjects(Configuration.SchedulingPolicy))
	{
		for (TFieldIterator<FProperty> Property(PolicyObject->GetClass()); Property; ++Property)
		{
			if (!Property->HasAnyPropertyFlags(FOWTPropertyFilter::ExcludedStateFlags))
			{
				FOWTReferenceVisitor::Visit(**Property, Property->ContainerPtrToValuePtr<void>(PolicyObject), Visitor);
			}
		}
	}
	for (UPCGGraphInstance* Graph : GetOwnedGraphInstances(Configuration))
	{
		FInstancedPropertyBag& Bag = Graph->ParametersOverrides.Parameters;
		if (const UStruct* Struct = Bag.GetPropertyBagStruct())
		{
			for (TFieldIterator<FProperty> Property(Struct); Property; ++Property)
			{
				FOWTReferenceVisitor::Visit(**Property, Property->ContainerPtrToValuePtr<void>(Bag.GetMutableValue().GetMemory()), Visitor);
			}
		}
	}
	for (auto& Data : Configuration.ToolDataContainer.ToolData)
	{
		if (Data.IsValid())
		{
			for (TFieldIterator<FProperty> Property(Data.GetScriptStruct()); Property; ++Property)
			{
				FOWTReferenceVisitor::Visit(**Property, Property->ContainerPtrToValuePtr<void>(Data.GetMutableMemory()), Visitor);
			}
		}
	}
}

bool FOWTPCGReferenceRemapper::Validate(UPCGComponent& Configuration, const TMap<UObject*, UObject*>& Mapping,
                                        FString& Error)
{
	return VisitConfiguration(Configuration, Mapping, true, Error);
}

bool FOWTPCGReferenceRemapper::Remap(UPCGComponent& Target, const TMap<UObject*, UObject*>& Mapping, FString& Error)
{
	return VisitConfiguration(Target, Mapping, false, Error);
}

bool FOWTPCGReferenceRemapper::VisitConfiguration(UPCGComponent& Component, const TMap<UObject*, UObject*>& Mapping,
                                                  bool bValidateOnly, FString& Error)
{
	bool bVisited = true;
	for (TFieldIterator<FProperty> Property(UPCGComponent::StaticClass(), EFieldIteratorFlags::ExcludeSuper); Property && bVisited; ++Property)
	{
		bVisited = !IsReflectedConfigurationProperty(**Property) ||
		           VisitProperty(**Property, Property->ContainerPtrToValuePtr<void>(&Component), Mapping, bValidateOnly, Error);
	}
	for (UObject* PolicyObject : FOWTPCGConfigurationSnapshot::GetSchedulingPolicyObjects(Component.SchedulingPolicy))
	{
		bVisited = bVisited && VisitStruct(*PolicyObject->GetClass(), PolicyObject, Mapping, bValidateOnly, Error);
	}
	for (UPCGGraphInstance* Graph : GetOwnedGraphInstances(Component))
	{
		FInstancedPropertyBag& Bag = Graph->ParametersOverrides.Parameters;
		const UScriptStruct* Struct = Bag.GetPropertyBagStruct();
		bVisited = bVisited && (!Struct || VisitStruct(*Struct, Bag.GetMutableValue().GetMemory(), Mapping, bValidateOnly, Error));
	}
	for (auto& Data : Component.ToolDataContainer.ToolData)
	{
		bVisited = bVisited && (!Data.IsValid() || VisitStruct(*Data.GetScriptStruct(), Data.GetMutableMemory(), Mapping, bValidateOnly, Error));
	}
	return bVisited;
}

bool FOWTPCGReferenceRemapper::VisitStruct(const UStruct& Struct, void* Memory, const TMap<UObject*, UObject*>& Mapping,
                                           bool bValidateOnly, FString& Error)
{
	bool bVisited = true;
	for (TFieldIterator<FProperty> Property(&Struct); Property && bVisited; ++Property)
	{
		bVisited = Property->HasAnyPropertyFlags(FOWTPropertyFilter::ExcludedStateFlags) ||
		           VisitProperty(**Property, Property->ContainerPtrToValuePtr<void>(Memory), Mapping, bValidateOnly, Error);
	}
	return bVisited;
}

bool FOWTPCGReferenceRemapper::VisitProperty(FProperty& Property, void* Value, const TMap<UObject*, UObject*>& Mapping,
                                             bool bValidateOnly, FString& Error)
{
	const UScriptStruct* Unsupported = UnsupportedStructs.Find(Property);
	if (Unsupported)
	{
		Error = FString::Printf(TEXT("PCG parameter %s contains unsupported native state %s."), *Property.GetPathName(),
		                        *Unsupported->GetName());
	}

	bool bVisited = Unsupported == nullptr;
	if (bVisited)
	{
		FOWTReferenceVisitor::Visit(Property, Value, [this, &Mapping, bValidateOnly, &Error, &Property, &bVisited](UObject*& Reference, bool)
		{
			const bool bManaged = bVisited && Reference && IsManagedObject(*Reference);
			UObject* const* Replacement = bVisited && Reference && !bManaged && !bValidateOnly ? Mapping.Find(Reference) : nullptr;
			if (bManaged)
			{
				Error = FString::Printf(TEXT("PCG parameter %s references managed output %s and needs a "
				                             "logical output remapping adapter."),
				                        *Property.GetPathName(), *Reference->GetPathName());
				bVisited = false;
			}
			else if (Replacement)
			{
				Reference = *Replacement;
			}
		});
	}
	return bVisited;
}

TArray<UPCGGraphInstance*> FOWTPCGReferenceRemapper::GetOwnedGraphInstances(UPCGComponent& Component)
{
	TArray<UPCGGraphInstance*> Instances;
	for (UPCGGraphInstance* Graph = Component.GetGraphInstance(); Graph && Graph->IsIn(&Component); Graph = Cast<UPCGGraphInstance>(Graph->Graph))
	{
		Instances.Add(Graph);
	}
	return Instances;
}

bool FOWTPCGReferenceRemapper::IsReflectedConfigurationProperty(const FProperty& Property)
{
	// The tool container exposes opaque FInstancedStruct values through its own runtime API.
	const FStructProperty* StructProperty = CastField<FStructProperty>(&Property);
	const bool bToolContainer = StructProperty && StructProperty->Struct == FPCGInteractiveToolDataContainer::StaticStruct();
	return FOWTPCGAuthoredProperties::IsAuthored(Property) && !bToolContainer;
}
