#include "Duplication/Restore/OWTActorNameAllocator.h"

#include "Algo/AllOf.h"
#include "Engine/Level.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectHash.h"

FOWTActorNameAllocator::FOWTActorNameAllocator(TMap<FString, int64>& InCounters) : Counters(InCounters)
{
}

FName FOWTActorNameAllocator::Allocate(const AActor& Source, FString& Error)
{
	FString Family;
	int64 SourceSuffix;
	SplitName(Source.GetName(), Family, SourceSuffix);
	const FString CounterKey = Source.GetLevel()->GetPathName() + TEXT("|") + Family.ToLower();
	const int64 LargestSuffix =
	    FMath::Max3(SourceSuffix, Counters.FindRef(CounterKey), FindLargestSuffix(Source, Family));
	const bool bSuffixAvailable = LargestSuffix < MAX_int32 - 1;

	FName Name = NAME_None;
	if (bSuffixAvailable)
	{
		Counters.Add(CounterKey, LargestSuffix + 1);
		Name = FName(*FString::Printf(TEXT("%s_%lld"), *Family, LargestSuffix + 1));
	}
	else
	{
		Error = TEXT("The source name family has exhausted its numeric suffix range.");
	}
	return Name;
}

void FOWTActorNameAllocator::SplitName(const FString& Name, FString& Family, int64& Suffix)
{
	int32 Separator = INDEX_NONE;
	const bool bHasSeparator = Name.FindLastChar(TEXT('_'), Separator) && Separator < Name.Len() - 1;
	const FString Number = bHasSeparator ? Name.Mid(Separator + 1) : FString();
	const bool bNumericSuffix = bHasSeparator && Algo::AllOf(Number, [](TCHAR Character)
	{
		return FChar::IsDigit(Character);
	});

	Family = bNumericSuffix ? Name.Left(Separator) : Name;
	Suffix = bNumericSuffix ? FCString::Atoi64(*Number) : 0;
}

int64 FOWTActorNameAllocator::FindLargestSuffix(const AActor& Source, const FString& Family)
{
	int64 LargestSuffix = 0;
	ForEachObjectWithOuter(Source.GetLevel(), [&Family, &LargestSuffix](UObject* Object)
	{
		FString CandidateFamily;
		int64 CandidateSuffix;
		SplitName(Object->GetName(), CandidateFamily, CandidateSuffix);
		if (CandidateFamily.Equals(Family, ESearchCase::IgnoreCase))
		{
			LargestSuffix = FMath::Max(LargestSuffix, CandidateSuffix);
		}
	}, false);
	return LargestSuffix;
}
