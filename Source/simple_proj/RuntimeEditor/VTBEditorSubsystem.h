#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VTBEditorSubsystem.generated.h"

class AActor;
class UCombinedTransformGizmo;
class UVTBEditorInteractiveToolsContext;
class UVTBEditorTargetSelection;
class UVTBEditorTransformGizmoBuilder;

UENUM(BlueprintType)
enum class EVTBEditorTransformGizmoSource : uint8
{
	DefaultITF,
	CustomVTB
};

enum class EVTBEditorSubsystemPhase : uint8
{
	Ready,
	Refreshing,
	Exiting
};

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

	UFUNCTION(BlueprintCallable, Category = "VTB Editor|Gizmo")
	void SetTransformGizmoSource(EVTBEditorTransformGizmoSource Source);

	// Calls made during an interaction callback apply on the next tick.
	bool BindSelectionSource(UObject* Source); // nullptr unbinds and clears selection.
	void ReceiveSelection(const TArray<TWeakObjectPtr<AActor>>& Actors);

private:
	friend class AVTBEditorSpectatorPawn;

	void ExitRuntime();
	void RefreshRuntime();
	void RefreshGizmoTarget();
	void UpdateGizmoVisibility(bool bHasView);
	void ApplyGizmoRendering();
	void EnsureCustomGizmoBuilder();
	bool UpdateView();
	void OnSelectionChanged();
	bool IsInteractionLocked() const;

	UPROPERTY(Transient)
	TObjectPtr<UVTBEditorInteractiveToolsContext> ToolsContext;

	UPROPERTY(Transient)
	TObjectPtr<UVTBEditorTargetSelection> TargetAdapter;

	UPROPERTY(Transient)
	TObjectPtr<UVTBEditorTransformGizmoBuilder> GizmoBuilder;

	UPROPERTY(Transient)
	TObjectPtr<UCombinedTransformGizmo> TransformGizmo;

	UPROPERTY(EditAnywhere, Category = "VTB Editor|Gizmo")
	EVTBEditorTransformGizmoSource TransformGizmoSource;

	TWeakObjectPtr<UObject> SelectionSource;
	FDelegateHandle SelectionChangedHandle;
	TOptional<TArray<TWeakObjectPtr<AActor>>> PendingSelection;
	EVTBEditorSubsystemPhase Phase;
};
