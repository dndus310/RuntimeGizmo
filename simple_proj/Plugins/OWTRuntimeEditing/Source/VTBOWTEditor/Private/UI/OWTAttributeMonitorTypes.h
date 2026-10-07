#pragma once

#include "CoreMinimal.h"
#include "Events/OWTAttributeTypes.h"
#include "Extensions/OWTToolDescriptor.h"
#include "State/OWTEditingSessionTypes.h"
#include "OWTAttributeMonitorTypes.generated.h"

/** Runtime editor adapter data. The reusable viewer has no knowledge of these fields. */
USTRUCT()
struct FOWTAttributeMonitorSnapshot
{
	GENERATED_BODY()

	UPROPERTY()
	FOWTAttributeSnapshot Selection;

	UPROPERTY()
	FOWTModeSnapshot Mode;

	UPROPERTY()
	TArray<FOWTToolAvailability> Tools;

	UPROPERTY()
	TArray<FOWTDuplicationOperationSnapshot> DuplicationOperations;

	UPROPERTY()
	TArray<FOWTProceduralComponentSnapshot> ProceduralComponents;
};

/** Transport evidence is a separate source and never supplies the current Actor state. */
USTRUCT()
struct FOWTAttributeMonitorEvents
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FOWTEventRecord> Journal;
};
