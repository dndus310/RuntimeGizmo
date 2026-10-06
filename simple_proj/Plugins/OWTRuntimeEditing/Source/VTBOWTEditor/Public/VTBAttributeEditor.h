#pragma once

#include "CoreMinimal.h"
#include "Context/SaveTransformContext.h"
#include "Events/OWTAttributeTypes.h"
#include "GameFramework/Actor.h"
#include "VTBAttributeEditor.generated.h"

class FJsonObject;
class UOWTNotificationCenter;
class UOWTAttributeStateStore;
class UOWTRuntimeActorDuplicator;
class UVTBOWTEditorSubsystem;

UCLASS(Blueprintable)
class VTBOWTEDITOR_API AVTBAttributeEditor : public AActor
{
	GENERATED_BODY()

public:
	AVTBAttributeEditor();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SaveHistory_Implementation();

	UFUNCTION(BlueprintCallable, Category = "OWT|Attributes")
	FGuid SubscribeDynamic(UObject* Subscriber, const FOWTAttributeEventDynamic& Callback, bool bSendSnapshot = true);
	UFUNCTION(BlueprintCallable, Category = "OWT|Attributes")
	bool Unsubscribe(FGuid Handle);
	UFUNCTION(BlueprintCallable, Category = "OWT|Attributes")
	bool PublishRequest(FName Event, const FString& Json);
	UFUNCTION(BlueprintCallable, Category = "OWT|Attributes")
	bool RequestTransformField(const FOWTAttributeSnapshot& Expected, EOWTTransformField Field, double Value,
	                           EOWTTransformEditPhase Phase, FGuid OperationId);
	UFUNCTION(BlueprintCallable, Category = "OWT|Attributes")
	bool RequestDuplicate(const FOWTAttributeSnapshot& Expected);
	UFUNCTION(BlueprintCallable, Category = "OWT|Attributes")
	void MarkSelectionBaseline();
	UFUNCTION(BlueprintPure, Category = "OWT|Attributes")
	FOWTAttributeSnapshot GetSnapshot() const;
	UFUNCTION(BlueprintPure, Category = "OWT|Monitor")
	TArray<FOWTEventRecord> GetMonitorEntries(int32 MaximumCount = 256) const;
	UFUNCTION(BlueprintPure, Category = "OWT|Monitor")
	int64 GetLatestEventSequence() const;
	UFUNCTION(BlueprintCallable, Category = "OWT|Attributes")
	static bool ParseSnapshotJson(const FString& Json, FOWTAttributeSnapshot& OutSnapshot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "History")
	void SaveHistory();

	UFUNCTION(BlueprintCallable, Category = "History")
	void SaveHistoryFromTransformContext(FSaveTransformContext Context);

	void BindSubsystem(UVTBOWTEditorSubsystem* InSubsystem);
	void NotifyEditorStateChanged();
	void NotifySelectionChanged();
	void BeginGizmoEdit();
	void UpdateGizmoEdit();
	void EndGizmoEdit();
	void RefreshSelectedTransform();
	void FinishActiveOperation();
	FGuid Subscribe(UObject* Subscriber, const FOWTAttributeEventNative& Callback, bool bSendSnapshot = true);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "History")
	void Undo();

	UFUNCTION(BlueprintNativeEvent, Category = "History")
	void Redo();

private:
	bool IsReady() const;
	bool IsObservedSelectionCurrent(AActor* Actor) const;
	FGuid RegisterActor(AActor& Actor, bool bNewObject = false);
	void RebuildSnapshot();
	void EmitSnapshot(FName Event, const FString& RequestId = FString(), const FString& Source = TEXT("Editor"),
	                  const FString& Phase = FString(), FGuid Recipient = FGuid(), FGuid OperationId = FGuid());
	void EmitRejection(const FString& RequestId, const FString& Code, const FString& Reason);
	bool ValidateRequest(const FJsonObject& Request, FString& RequestId, AActor*& Actor);
	bool ProcessTransformRequest(const FJsonObject& Request, const FString& RequestId, AActor& Actor);
	bool ProcessDuplicateRequest(const FJsonObject& Request, const FString& RequestId, AActor& Actor);
	bool ApplyTransform(AActor& Actor, const FTransform& Transform);
	void ClearOperation();
	void PublishTransform(const FString& RequestId, const FString& Source, const FString& Phase,
	                      FGuid OperationId = FGuid());
	TSharedRef<FJsonObject> MakeSnapshotObject(const FString& RequestId, const FString& Source) const;
	TSharedRef<FJsonObject> MakeRequestObject(const FOWTAttributeSnapshot& Expected) const;

	UPROPERTY(Transient)
	TObjectPtr<UOWTNotificationCenter> Notifications;
	UPROPERTY(Transient)
	TObjectPtr<UOWTAttributeStateStore> StateStore;
	UPROPERTY(Transient)
	TObjectPtr<UOWTRuntimeActorDuplicator> Duplicator;
	UPROPERTY(EditAnywhere, Category = "OWT|Attributes")
	FVector DuplicateWorldOffset;

	TWeakObjectPtr<UVTBOWTEditorSubsystem> Subsystem;
	TWeakObjectPtr<AActor> ObservedSelection;
	TWeakObjectPtr<AActor> OperationActor;
	TSet<FString> ProcessedRequestIds;
	TSet<FGuid> CompletedOperations;
	FTransform OperationStart;
	FGuid EditorId;
	FGuid SelectedId;
	FGuid ActiveOperation;
	int32 SelectionRevision;
	int32 StateRevision;
	bool bGizmoOperation;
	bool bApplyingTransform;
	bool bHandlingRequest;
	bool bEndingPlay;
};
