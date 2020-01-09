// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Stack.h"
#include "GameFramework/PlayerController.h"
#include "StackPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STACK_API AStackPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AStackPlayerController();

	void ShowResultUI();
	void AddGameScore() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UGameResultWidget> GameResultWidgetClass;

private:
	UPROPERTY()
		class UHUDWidget* HUDWidget;

	UPROPERTY()
		class UGameResultWidget* GameResultWidget;

	UPROPERTY()
		class AStackPlayerState* StackPlayerState;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;

private:
	void ChangeInputMode(bool bGameMode);

};
