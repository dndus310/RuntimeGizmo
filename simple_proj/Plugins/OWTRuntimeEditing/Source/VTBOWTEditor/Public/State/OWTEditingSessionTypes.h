#pragma once

#include "CoreMinimal.h"
#include "OWTEditingSessionTypes.generated.h"

class AActor;
class UActorComponent;

UENUM(BlueprintType)
enum class EOWTDuplicationPhase : uint8
{
	Accepted,
	Planning,
	Restoring,
	Committed,
	Failed,
	Cancelled,
	CleaningUp
};

UENUM(BlueprintType)
enum class EOWTProceduralState : uint8
{
	NotRequested,
	Scheduled,
	WaitingForGenerationSource,
	Generating,
	Ready,
	Failed,
	Cancelled,
	Cleaned,
	CleaningUp
};

/** Observed mode/tool state. It is independent of Actor attributes and JSON delivery. */
USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTModeSnapshot
{
	GENERATED_BODY()

	FOWTModeSnapshot()
	    : ModeId(NAME_None), ActiveToolId(NAME_None), Lifecycle(NAME_None), DisabledReason(), Revision(0),
	      bEditingEnabled(false), bCanStartTools(false)
	{
	}

	UPROPERTY(BlueprintReadOnly, Category = "OWT|Mode")
	FName ModeId;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Mode")
	FName ActiveToolId;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Mode")
	FName Lifecycle;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Mode")
	FString DisabledReason;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Mode")
	int32 Revision;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Mode")
	bool bEditingEnabled;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Mode")
	bool bCanStartTools;
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTDuplicationOperationSnapshot
{
	GENERATED_BODY()

	FOWTDuplicationOperationSnapshot()
	    : SourceActor(), DuplicateActor(), OperationId(), RequestId(), Source(), OriginalObjectId(),
	      DuplicateObjectId(), HierarchyScope(NAME_None), Phase(EOWTDuplicationPhase::Accepted), Error(), Revision(0)
	{
	}

	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	TWeakObjectPtr<AActor> SourceActor;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	TWeakObjectPtr<AActor> DuplicateActor;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	FGuid OperationId;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	FString RequestId;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	FString Source;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	FString OriginalObjectId;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	FString DuplicateObjectId;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	FName HierarchyScope;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	EOWTDuplicationPhase Phase;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	FString Error;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Duplication")
	int32 Revision;
};

USTRUCT(BlueprintType)
struct VTBOWTEDITOR_API FOWTProceduralComponentSnapshot
{
	GENERATED_BODY()

	FOWTProceduralComponentSnapshot()
	    : Component(), OperationId(), ComponentId(), ComponentName(), GraphPath(), Trigger(),
	      State(EOWTProceduralState::NotRequested), GenerationAttempt(0), Reason(), Revision(0)
	{
	}

	UPROPERTY(BlueprintReadOnly, Category = "OWT|Procedural")
	TWeakObjectPtr<UActorComponent> Component;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Procedural")
	FGuid OperationId;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Procedural")
	FGuid ComponentId;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Procedural")
	FString ComponentName;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Procedural")
	FString GraphPath;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Procedural")
	FString Trigger;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Procedural")
	EOWTProceduralState State;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Procedural")
	int32 GenerationAttempt;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Procedural")
	FString Reason;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Procedural")
	int32 Revision;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOWTModeChanged, const FOWTModeSnapshot&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOWTDuplicationChanged, const FOWTDuplicationOperationSnapshot&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOWTProceduralChanged, const FOWTProceduralComponentSnapshot&);
