#pragma once

#include "CoreMinimal.h"

class UScriptStruct;

enum class EOWTStateMonitorChange : uint8
{
	Unchanged,
	Added,
	Modified,
	Removed
};

/** A copied reflection value. No field retains an address into the submitted struct. */
struct OWTSTATEMONITOR_API FOWTStateMonitorNode
{
	TArray<TSharedPtr<FOWTStateMonitorNode>> Children;
	FString Path;
	FString Name;
	FString DisplayName;
	FString TypeName;
	FString Value;
	FString PreviousValue;
	EOWTStateMonitorChange Change = EOWTStateMonitorChange::Unchanged;
	bool bTruncated = false;
};

struct OWTSTATEMONITOR_API FOWTStateMonitorChange
{
	FString Path;
	FString TypeName;
	FString PreviousValue;
	FString Value;
	EOWTStateMonitorChange Change = EOWTStateMonitorChange::Unchanged;
};

struct OWTSTATEMONITOR_API FOWTStateMonitorHistoryEntry
{
	TArray<FOWTStateMonitorChange> Changes;
	uint64 Sequence = 0;
	double TimeSeconds = 0.0;
	int32 OmittedChanges = 0;
};

/** Replace-on-submit source. Previously obtained source pointers remain stable snapshots. */
struct OWTSTATEMONITOR_API FOWTStateMonitorSource
{
	TSharedPtr<FOWTStateMonitorNode> Root;
	TArray<FOWTStateMonitorChange> Changes;
	TArray<TSharedPtr<FOWTStateMonitorHistoryEntry>> History;
	FName SourceId;
	FText Label;
	FString StructType;
	FString CurrentJson;
	uint64 Sequence = 0;
	double TimeSeconds = 0.0;
	int32 OmittedChanges = 0;
	bool bTruncated = false;
};

struct OWTSTATEMONITOR_API FOWTStateMonitorSettings
{
	int32 MaxSources = 32;
	int32 MaxNodesPerSnapshot = 2048;
	int32 MaxDepth = 12;
	int32 MaxHistoryEntries = 100;
	int32 MaxChangesPerEntry = 256;
	int32 MaxTextCharacters = 4096;
	int32 MaxHistoryCharacters = 1024 * 1024;
};

/** Game-thread-only, producer-owned model. Only reflected USTRUCT/UPROPERTY fields are inspected. */
class OWTSTATEMONITOR_API FOWTStateMonitorModel
{
public:
	explicit FOWTStateMonitorModel(const FOWTStateMonitorSettings& InSettings = FOWTStateMonitorSettings());

	bool SubmitSnapshot(FName SourceId, const FText& Label, const UScriptStruct* StructType, const void* StructData,
	                    bool bRecordHistory = true);

	bool RemoveSource(FName SourceId);

	void ClearHistory(FName SourceId);

	void ResetBaseline(FName SourceId);

	void Reset();

	const TArray<TSharedPtr<FOWTStateMonitorSource>>& GetSources() const;

	TSharedPtr<FOWTStateMonitorSource> FindSource(FName SourceId) const;

	uint64 GetRevision() const;

	const FOWTStateMonitorSettings& GetSettings() const;

private:
	FOWTStateMonitorSettings Settings;
	TArray<TSharedPtr<FOWTStateMonitorSource>> Sources;
	uint64 Revision;
	uint64 NextSequence;
};
