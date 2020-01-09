// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Stack.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class STACK_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindPlayerState(class AStackPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateScore();

private:
	UPROPERTY()
		class UTextBlock* Score;

	TWeakObjectPtr<class AStackPlayerState> CurrentPlayerState;
};
