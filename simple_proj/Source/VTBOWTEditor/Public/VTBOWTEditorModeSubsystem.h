#pragma once

#include "CoreMinimal.h"
#include "Selection/VTBOWTEditorSelectionSource.h"
#include "Subsystems/WorldSubsystem.h"
#include "VTBOWTEditorModeSubsystem.generated.h"

class UVTBOWTEditorToolsContext;
class UVTBOWTEditorGizmoManager;

UCLASS()
class VTBOWTEDITOR_API UVTBOWTEditorModeSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual UWorld* GetTickableGameObjectWorld() const override;

	UVTBOWTEditorToolsContext* GetToolsContext();
	UVTBOWTEditorGizmoManager* GetGizmoManager() const;
	bool BindSelectionSource(UObject* Source);
	void ReceiveSelection(const FVTBOWTActorSelection& Actors, USceneComponent* FrameComponent = nullptr);

private:
	void CreateToolsContext();
	void ApplyPendingSelection();
	void OnSelectionChanged();
	bool HandleSelectionChange(const FSelectedObjectsChangeList& Change);

private:
	UPROPERTY()
	TObjectPtr<UVTBOWTEditorToolsContext> EditorToolsContext;

	TWeakObjectPtr<UObject> SelectionSource;
	FDelegateHandle SelectionChangedHandle;
	TOptional<FVTBOWTTransformSelection> Selection;
};
