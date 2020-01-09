// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Stack.h"
#include "GameFramework/PlayerState.h"
#include "StackPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);
/**
 * 
 */
UCLASS()
class STACK_API AStackPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AStackPlayerState();

	int32 GetGameScore() const;
	void AddGameScore();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

protected:
	UPROPERTY(Transient)
	int32 GameScore;
};
