#pragma once

#include "CoreMinimal.h"

class AActor;

/** Allocates "<Family>_<N>" names above every existing sibling and every name this session handed out. */
class FOWTActorNameAllocator
{
public:
	explicit FOWTActorNameAllocator(TMap<FString, int64>& InCounters);
	/** Returns NAME_None with Error set when the family's numeric suffix range is exhausted. */
	FName Allocate(const AActor& Source, FString& Error);

private:
	static void SplitName(const FString& Name, FString& Family, int64& Suffix);
	static int64 FindLargestSuffix(const AActor& Source, const FString& Family);

private:
	/** Keyed by level path and lower-case family. Owned by the duplicator so names never repeat in a session. */
	TMap<FString, int64>& Counters;
};
