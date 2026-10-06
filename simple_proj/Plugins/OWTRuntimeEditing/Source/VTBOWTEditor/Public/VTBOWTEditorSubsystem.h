// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Interfaces/OWTEditContextReceiver.h"
#include "ToolContextInterfaces.h"
#include "Context/OWTGizmoSnapSettings.h"
#include "VTBOWTEditorSubsystem.generated.h"

struct FOWTGizmoPointerContext;
class AActor;
class USceneComponent;
class UVTBOWTEditorToolsContext;
class UCombinedTransformGizmo;
class UVTBOWTBaseTransformGizmo;
class UTransformProxy;
class AVTBAttributeEditor;

UCLASS()
class VTBOWTEDITOR_API UVTBOWTEditorSubsystem : public UTickableWorldSubsystem, public IOWTEditContextReceiver
{
	GENERATED_BODY()

public:
	UVTBOWTEditorSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void OnWorldEndPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual bool ReceiveEditContext_Implementation(const FInstancedStruct& Context) override;
	virtual TStatId GetStatId() const override;

	UFUNCTION(BlueprintCallable, Category = "OWT|Editing")
	bool SetActiveEditMode(UObject* Mode);

	UFUNCTION(BlueprintCallable, Category = "OWT|Snapping")
	bool SetGizmoSnapSettings(const FOWTGizmoSnapSettings& Settings);

	UFUNCTION(BlueprintPure, Category = "OWT|Snapping")
	FOWTGizmoSnapSettings GetGizmoSnapSettings() const;

	bool InitializeToolsContext();
	void ShowSelectionGizmo();
	void ToggleEditing();
	bool IsEditingEnabled() const;
	bool HasGizmoCapture() const;
	void ShutdownToolsContext();
	void HideSelectionGizmo();
	void SetSelectedObject(AActor* Actor);
	void SetCoordinateSystem(EToolContextCoordinateSystem System);
	void SetTransformGizmoMode(EToolContextTransformGizmoMode Mode);
	void RegisterAttributeEditor(AVTBAttributeEditor* Editor);
	void SynchronizeSelectionGizmo();
	void TerminateGizmoCapture();

	UFUNCTION(BlueprintPure, Category = "OWT|Editing")
	AVTBAttributeEditor* GetAttributeEditor() const;

	UVTBOWTEditorToolsContext* GetToolsContext() const;
	UCombinedTransformGizmo* GetTransformGizmo() const;
	UTransformProxy* GetTransformProxy() const;
	EToolContextCoordinateSystem GetCoordinateSystem() const;
	EToolContextTransformGizmoMode GetTransformGizmoMode() const;

private:
	void UpdateGizmoView();
	void NotifyEditorStateChanged();
	void OnGizmoEditStarted(UTransformProxy* Proxy);
	void OnGizmoTransformChanged(UTransformProxy* Proxy, FTransform Transform);
	void OnGizmoEditEnded(UTransformProxy* Proxy);
	bool RouteGizmoPointer(const FOWTGizmoPointerContext& Pointer);

public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "OWT|Editing")
	TObjectPtr<UObject> ActiveEditMode;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "OWT|Editing")
	TWeakObjectPtr<AActor> SelectedObject;

private:
	UPROPERTY(Transient)
	TObjectPtr<UVTBOWTEditorToolsContext> ToolsContext;

	UPROPERTY(Transient)
	TObjectPtr<UVTBOWTBaseTransformGizmo> TransformGizmo;

	TWeakObjectPtr<AVTBAttributeEditor> AttributeEditor;
	TWeakObjectPtr<USceneComponent> GizmoTarget;
	TMap<const UScriptStruct*, TFunction<bool(const FInstancedStruct&)>> SystemContextHandlers;
	EToolContextCoordinateSystem CoordinateSystem;
	EToolContextTransformGizmoMode TransformGizmoMode;
	bool bEditingEnabled;
};
