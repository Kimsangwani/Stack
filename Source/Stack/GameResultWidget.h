// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Stack.h"
#include "Blueprint/UserWidget.h"
#include "GameResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class STACK_API UGameResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindGameState(class AStackGameState* GameState);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnReturnToTitleClicked();

	UFUNCTION()
		void OnRetryGameClicked();

private:
	TWeakObjectPtr<class AStackGameState> CurrentGameState;

protected:
	UPROPERTY()
		class UButton* ReturnToTitleButton;

	UPROPERTY()
		class UButton* RetryGameButton;
};
