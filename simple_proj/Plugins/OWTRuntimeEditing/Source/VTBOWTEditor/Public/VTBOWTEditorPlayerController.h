// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VTBOWTEditorPlayerController.generated.h"

class UOWTAttributeDetailsWidget;

UCLASS()
class VTBOWTEDITOR_API AVTBOWTEditorPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVTBOWTEditorPlayerController();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category = "OWT|UI")
	void ToggleEventMonitor();

	UFUNCTION(BlueprintPure, Category = "OWT|UI")
	bool IsAttributeUIBlockingInput() const;

	UFUNCTION(BlueprintPure, Category = "OWT|UI")
	UOWTAttributeDetailsWidget* GetAttributeDetailsWidget() const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OWT|UI")
	TSubclassOf<UOWTAttributeDetailsWidget> AttributeDetailsClass;

private:
	UPROPERTY(Transient)
	TObjectPtr<UOWTAttributeDetailsWidget> AttributeDetails;
};
