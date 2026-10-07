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
class UOWTAttributeEditMode;
class UOWTAttributeEditTool;

UCLASS(Config = Game)
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

	UFUNCTION(BlueprintPure, Category = "OWT|Editing")
	AVTBAttributeEditor* GetAttributeEditor() const;

	bool InitializeToolsContext();
	void ShowSelectionGizmo();
	void ToggleEditing();
	void SynchronizeSelectionGizmo();
	void ShutdownToolsContext();
	void HideSelectionGizmo();
	void TerminateGizmoCapture();
	void RegisterAttributeEditor(AVTBAttributeEditor* Editor);

	void SetSelectedObject(AActor* Actor);
	void SetCoordinateSystem(EToolContextCoordinateSystem System);
	void SetTransformGizmoMode(EToolContextTransformGizmoMode Mode);

	bool IsEditingEnabled() const;
	bool HasGizmoCapture() const;
	UVTBOWTEditorToolsContext* GetToolsContext() const;
	UCombinedTransformGizmo* GetTransformGizmo() const;
	UTransformProxy* GetTransformProxy() const;
	EToolContextCoordinateSystem GetCoordinateSystem() const;
	EToolContextTransformGizmoMode GetTransformGizmoMode() const;
	UOWTAttributeEditMode* GetAttributeEditMode() const;

private:
	void InitializeDefaultMode();
	void RegisterSystemContextHandlers();
	void NotifyEditorStateChanged();
	bool RouteGizmoPointer(const FOWTGizmoPointerContext& Pointer);
	UOWTAttributeEditTool* FindAttributeTool() const;

public:
	UPROPERTY(Config, EditAnywhere, Category = "OWT|Editing")
	TSubclassOf<UOWTAttributeEditMode> DefaultModeClass;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "OWT|Editing")
	TObjectPtr<UObject> ActiveEditMode;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "OWT|Editing")
	TWeakObjectPtr<AActor> SelectedObject;

private:
	TWeakObjectPtr<AVTBAttributeEditor> AttributeEditor;
	TMap<const UScriptStruct*, TFunction<bool(const FInstancedStruct&)>> SystemContextHandlers;
};
