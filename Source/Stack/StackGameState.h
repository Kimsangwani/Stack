// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Stack.h"
#include "GameFramework/GameStateBase.h"
#include "StackGameState.generated.h"

/**
 * 
 */
UCLASS()
class STACK_API AStackGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AStackGameState();

public:
	int32 GetToTalGameScore() const;
	void AddGameScore();
	void SetGameEnd();
	bool IsGamePlaying() const;
	
private:
	UPROPERTY(Transient)
		int32 TotalGameScore;

	UPROPERTY(Transient)
		bool bGamePlaying;
};
