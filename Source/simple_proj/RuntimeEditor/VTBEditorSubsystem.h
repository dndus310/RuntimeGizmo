#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VTBEditorSubsystem.generated.h"

class AActor;
class UVTBEditorInteractiveToolsContext;

/** One local editing session per Game/PIE world. GameMode communicates through IVTBSelectionSource. */
UCLASS()
class SIMPLE_PROJ_API UVTBEditorSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UVTBEditorSubsystem();

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	// Calls made during an interaction callback apply on the next tick.
	bool BindSelectionSource(UObject* Source); // nullptr unbinds and clears selection.
	void ReceiveSelection(const TArray<TWeakObjectPtr<AActor>>& Actors);
	// Runtime input layers use the Context-owned input/capture API.
	UVTBEditorInteractiveToolsContext* GetRuntimeContext() const { return ToolsContext.Get(); }

private:
	void ExitRuntime();
	void ApplyPendingSelection();
	void OnSelectionChanged();

	UPROPERTY(Transient)
	TObjectPtr<UVTBEditorInteractiveToolsContext> ToolsContext;

	TWeakObjectPtr<UObject> SelectionSource;
	FDelegateHandle SelectionChangedHandle;
	TOptional<TArray<TWeakObjectPtr<AActor>>> PendingSelection;
};
