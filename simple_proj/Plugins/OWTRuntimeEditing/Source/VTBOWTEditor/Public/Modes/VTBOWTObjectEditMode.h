#pragma once

#include "CoreMinimal.h"
#include "Context/OWTEditContexts.h"
#include "Interfaces/OWTEditContextReceiver.h"
#include "VTBOWTObjectEditMode.generated.h"

struct FOWTResolvedContextHandler;

USTRUCT()
struct FOWTContextHandlerBinding
{
	GENERATED_BODY()

public:
	FOWTContextHandlerBinding() : Receiver(), FunctionName(NAME_None)
	{
	}
	FOWTContextHandlerBinding(UObject* InReceiver, FName InFunctionName)
	    : Receiver(InReceiver), FunctionName(InFunctionName)
	{
	}

public:
	UPROPERTY()
	TWeakObjectPtr<UObject> Receiver;

	UPROPERTY()
	FName FunctionName;
};

UCLASS(BlueprintType, Blueprintable)
class VTBOWTEDITOR_API UVTBOWTObjectEditMode : public UObject, public IOWTEditContextReceiver
{
	GENERATED_BODY()

public:
	UVTBOWTObjectEditMode();
	virtual bool ReceiveEditContext_Implementation(const FInstancedStruct& Context) override;

	// Runs once on first dispatch, after BP construction. Call parent when overriding.
	UFUNCTION(BlueprintNativeEvent, Category = "OWT|Editing")
	void InitializeContextBindings();

	// Exactly one typed input parameter, no return value. Rebinding replaces the handler.
	UFUNCTION(BlueprintCallable, Category = "OWT|Editing")
	bool BindContextHandler(UScriptStruct* ContextType, UObject* Receiver, FName FunctionName);

	UFUNCTION(BlueprintNativeEvent, Category = "OWT|Editing")
	void SelectObject(const FOWTSelectObjectContext& Context);

	UFUNCTION(BlueprintNativeEvent, Category = "OWT|Editing")
	void Undo(const FOWTUndoContext& Context);

	UFUNCTION(BlueprintNativeEvent, Category = "OWT|Editing")
	void Redo(const FOWTRedoContext& Context);

	UFUNCTION(BlueprintNativeEvent, Category = "OWT|Editing")
	void ToggleCoordinateSystem(const FOWTToggleCoordinateSystemContext& Context);

	UFUNCTION(BlueprintNativeEvent, Category = "OWT|Editing")
	void ToggleTransformSpline(const FOWTToggleTransformSplineContext& Context);

	UFUNCTION(BlueprintNativeEvent, Category = "OWT|Editing")
	void DuplicateSelection(const FOWTDuplicateSelectionContext& Context);

	UFUNCTION(BlueprintNativeEvent, Category = "OWT|Editing")
	void HideSelectionGizmo(const FOWTHideSelectionGizmoContext& Context);

	UFUNCTION(BlueprintCallable, Category = "OWT|Editing")
	bool UnbindContextHandler(UScriptStruct* ContextType);

	UFUNCTION(BlueprintNativeEvent, Category = "OWT|Editing")
	void SetTranslation(const FOWTSetTranslationContext& Context);

	UFUNCTION(BlueprintNativeEvent, Category = "OWT|Editing")
	void SetRotation(const FOWTSetRotationContext& Context);

	UFUNCTION(BlueprintNativeEvent, Category = "OWT|Editing")
	void SetScale(const FOWTSetScaleContext& Context);

	template <typename TContext>
	bool BindContext(UObject* Receiver, FName FunctionName)
	{
		return BindContextHandler(TContext::StaticStruct(), Receiver, FunctionName);
	}

private:
	void EnsureContextBindings();
	FOWTResolvedContextHandler ResolveContextHandler(const UScriptStruct* ContextType) const;
	void InvokeContextHandler(const FInstancedStruct& Context, const FOWTResolvedContextHandler& Handler) const;

private:
	UPROPERTY(Transient)
	TMap<TObjectPtr<UScriptStruct>, FOWTContextHandlerBinding> ContextHandlers;
	bool bContextBindingsInitialized;
};
