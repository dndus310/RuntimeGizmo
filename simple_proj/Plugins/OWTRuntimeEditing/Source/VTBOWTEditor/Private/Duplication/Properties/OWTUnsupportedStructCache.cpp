#include "Duplication/Properties/OWTUnsupportedStructCache.h"

#include "Engine/EngineBaseTypes.h"
#include "PhysicsEngine/BodyInstance.h"
#include "PhysicsEngine/ConstraintInstance.h"
#include "StructUtils/InstancedStruct.h"
#include "StructUtils/PropertyBag.h"
#include "UObject/UnrealType.h"

FOWTUnsupportedStructCache::FOWTUnsupportedStructCache() : Results(), Visiting()
{
}

const UScriptStruct* FOWTUnsupportedStructCache::Find(const FProperty& Property)
{
	const FStructProperty* StructProperty = CastField<FStructProperty>(&Property);
	const FArrayProperty* Array = CastField<FArrayProperty>(&Property);
	const FSetProperty* Set = CastField<FSetProperty>(&Property);
	const FMapProperty* Map = CastField<FMapProperty>(&Property);

	const UScriptStruct* Unsupported = nullptr;
	if (StructProperty)
	{
		Unsupported = FindInStruct(*StructProperty->Struct);
	}
	else if (Array)
	{
		Unsupported = Find(*Array->Inner);
	}
	else if (Set)
	{
		Unsupported = Find(*Set->ElementProp);
	}
	else if (Map)
	{
		Unsupported = Find(*Map->KeyProp);
		Unsupported = Unsupported ? Unsupported : Find(*Map->ValueProp);
	}
	return Unsupported;
}

const UScriptStruct* FOWTUnsupportedStructCache::FindInStruct(const UScriptStruct& Struct)
{
	const bool bNativeHandle = IsNativeHandleStruct(Struct);
	const UScriptStruct* const* Cached = Results.Find(&Struct);
	// A recursive struct is judged by its outermost visit.
	const bool bNeedsInspection = !bNativeHandle && !Cached && !Visiting.Contains(&Struct);

	const UScriptStruct* Unsupported = nullptr;
	if (bNativeHandle)
	{
		Unsupported = &Struct;
	}
	else if (Cached)
	{
		Unsupported = *Cached;
	}
	else if (bNeedsInspection)
	{
		Visiting.Add(&Struct);
		for (TFieldIterator<FProperty> Field(&Struct); Field && !Unsupported; ++Field)
		{
			Unsupported = Find(**Field);
		}
		Visiting.Remove(&Struct);
		Results.Add(&Struct, Unsupported);
	}
	return Unsupported;
}

bool FOWTUnsupportedStructCache::IsNativeHandleStruct(const UScriptStruct& Struct)
{
	const bool bPhysicsHandle = &Struct == FBodyInstance::StaticStruct() || &Struct == FConstraintInstance::StaticStruct();
	const bool bTickHandle = Struct.IsChildOf(FTickFunction::StaticStruct());
	const bool bOpaqueStruct = &Struct == FInstancedStruct::StaticStruct() || &Struct == FInstancedPropertyBag::StaticStruct();
	return bPhysicsHandle || bTickHandle || bOpaqueStruct;
}
