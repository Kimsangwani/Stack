// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Stack.h"
#include "Blueprint/UserWidget.h"
#include "GameTitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class STACK_API UGameTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnStartGameClicked();

	UFUNCTION()
		void OnExitGameClicked();

protected:
	UPROPERTY()
		class UButton* StartGameButton;

	UPROPERTY()
		class UButton* ExitGameButton;
};
