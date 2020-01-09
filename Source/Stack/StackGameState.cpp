// Fill out your copyright notice in the Description page of Project Settings.


#include "StackGameState.h"

AStackGameState::AStackGameState()
{
	TotalGameScore = 0;
	bGamePlaying = true;
}

int32 AStackGameState::GetToTalGameScore() const
{
	return TotalGameScore;
}

void AStackGameState::AddGameScore()
{
	TotalGameScore++;
}

void AStackGameState::SetGameEnd()
{
	bGamePlaying = false;
}

bool AStackGameState::IsGamePlaying() const
{
	return bGamePlaying;
}