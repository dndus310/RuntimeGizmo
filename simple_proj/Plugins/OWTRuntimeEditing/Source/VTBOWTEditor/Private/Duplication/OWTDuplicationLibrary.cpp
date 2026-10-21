#include "Duplication/OWTDuplicationLibrary.h"

#include "Duplication/OWTDuplicationSubsystem.h"
#include "Engine/World.h"

static const TCHAR* const MissingSubsystemError = TEXT("The source Actor is not in a game world that supports runtime duplication.");

FOWTDuplicationResult UOWTDuplicationLibrary::DuplicateActor(AActor* Source, const FOWTDuplicationOptions& Options)
{
	UOWTDuplicationSubsystem* Subsystem = GetSubsystem(Source);
	FOWTDuplicationResult Result;
	if (Subsystem)
	{
		Result = Subsystem->DuplicateActor(Source, Options);
	}
	else
	{
		Result.Error = MissingSubsystemError;
	}
	return Result;
}

FOWTDuplicationResult UOWTDuplicationLibrary::DuplicateActorAtOffset(AActor* Source, FVector WorldOffset)
{
	FOWTDuplicationOptions Options;
	Options.WorldOffset = WorldOffset;
	return DuplicateActor(Source, Options);
}

bool UOWTDuplicationLibrary::CanDuplicateActor(AActor* Source, const FOWTDuplicationOptions& Options,
                                               FString& OutError)
{
	UOWTDuplicationSubsystem* Subsystem = GetSubsystem(Source);
	const bool bCanDuplicate = Subsystem && Subsystem->CanDuplicateActor(Source, Options, OutError);
	if (!Subsystem)
	{
		OutError = MissingSubsystemError;
	}
	return bCanDuplicate;
}

TArray<FOWTProceduralComponentSnapshot> UOWTDuplicationLibrary::GetProceduralComponents(
    const UObject* WorldContextObject)
{
	const UOWTDuplicationSubsystem* Subsystem = GetSubsystem(WorldContextObject);
	return Subsystem ? Subsystem->GetProceduralComponents() : TArray<FOWTProceduralComponentSnapshot>();
}

UOWTDuplicationSubsystem* UOWTDuplicationLibrary::GetSubsystem(const UObject* WorldContextObject)
{
	const UWorld* World = IsValid(WorldContextObject) ? WorldContextObject->GetWorld() : nullptr;
	return World ? World->GetSubsystem<UOWTDuplicationSubsystem>() : nullptr;
}
