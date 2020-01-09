// Fill out your copyright notice in the Description page of Project Settings.


#include "StackPlayerState.h"

AStackPlayerState::AStackPlayerState()
{
	GameScore = 0;
}

int32 AStackPlayerState::GetGameScore() const
{
	return GameScore;
}

void AStackPlayerState::AddGameScore()
{
	GameScore++;
	OnPlayerStateChanged.Broadcast();
}
