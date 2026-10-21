#include "Duplication/OWTDuplicationSubsystem.h"

#include "Duplication/OWTRuntimeActorDuplicator.h"
#include "Engine/World.h"

UOWTDuplicationSubsystem::UOWTDuplicationSubsystem() : Duplicator(nullptr)
{
}

void UOWTDuplicationSubsystem::Deinitialize()
{
	if (Duplicator)
	{
		Duplicator->Deinitialize();
		Duplicator = nullptr;
	}
	Super::Deinitialize();
}

void UOWTDuplicationSubsystem::Tick(float DeltaTime)
{
	if (Duplicator)
	{
		Duplicator->Tick(DeltaTime);
	}
}

TStatId UOWTDuplicationSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UOWTDuplicationSubsystem, STATGROUP_Tickables);
}

FOWTDuplicationResult UOWTDuplicationSubsystem::DuplicateActor(AActor* Source, const FOWTDuplicationOptions& Options)
{
	UOWTRuntimeActorDuplicator* SessionDuplicator = GetDuplicator();
	FOWTDuplicationResult Result;
	if (SessionDuplicator)
	{
		Result = SessionDuplicator->Duplicate(Source, Options, FGuid::NewGuid());
	}
	else
	{
		Result.Error = TEXT("Runtime duplication requires a live game world.");
	}
	return Result;
}

bool UOWTDuplicationSubsystem::CanDuplicateActor(AActor* Source, const FOWTDuplicationOptions& Options,
                                                 FString& OutError)
{
	UOWTRuntimeActorDuplicator* SessionDuplicator = GetDuplicator();
	const bool bCanDuplicate = SessionDuplicator && SessionDuplicator->CanDuplicate(Source, Options, OutError);
	if (!SessionDuplicator)
	{
		OutError = TEXT("Runtime duplication requires a live game world.");
	}
	return bCanDuplicate;
}

TArray<FOWTProceduralComponentSnapshot> UOWTDuplicationSubsystem::GetProceduralComponents() const
{
	return Duplicator ? Duplicator->GetProceduralComponents() : TArray<FOWTProceduralComponentSnapshot>();
}

UOWTRuntimeActorDuplicator* UOWTDuplicationSubsystem::GetDuplicator()
{
	// Created lazily so providers registered by modules loaded after world start are still installed.
	if (!Duplicator)
	{
		UOWTRuntimeActorDuplicator* Created = NewObject<UOWTRuntimeActorDuplicator>(this);
		if (Created->Initialize(this))
		{
			Duplicator = Created;
		}
	}
	return Duplicator;
}

bool UOWTDuplicationSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}
