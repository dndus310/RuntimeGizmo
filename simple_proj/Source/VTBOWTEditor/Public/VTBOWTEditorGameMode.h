// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UObject/Interface.h"
#include "VTBOWTEditorGameMode.generated.h"

class AVTBAttributeEditor;
class UWorld;

UINTERFACE(BlueprintType, Blueprintable)
class VTBOWTEDITOR_API UVTBOWTEditorGameModeProvider : public UInterface
{
	GENERATED_BODY()
};

class VTBOWTEDITOR_API IVTBOWTEditorGameModeProvider
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	AVTBAttributeEditor* GetAttributeEditor();
};

UCLASS()
class VTBOWTEDITOR_API AVTBOWTEditorGameMode : public AGameMode, public IVTBOWTEditorGameModeProvider
{
	GENERATED_BODY()

public:
	AVTBOWTEditorGameMode();

	virtual AVTBAttributeEditor* GetAttributeEditor_Implementation() override;

private:
	AVTBAttributeEditor* SpawnAttributeEditor(UWorld& World);
	AVTBAttributeEditor* FindAttributeEditor(UWorld& World) const;

private:
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "Attribute Editor",
	          meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AVTBAttributeEditor> AttributeEditor;
};
