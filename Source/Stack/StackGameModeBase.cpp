// Fill out your copyright notice in the Description page of Project Settings.

#include "StackGameModeBase.h"
#include "Spawner.h"
#include "StackPlayerController.h"
#include "StackGameState.h"
#include "StackPlayerState.h"

AStackGameModeBase::AStackGameModeBase()
{
	DefaultPawnClass = ASpawner::StaticClass();
	PlayerControllerClass = AStackPlayerController::StaticClass();
	GameStateClass = AStackGameState::StaticClass();
	PlayerStateClass = AStackPlayerState::StaticClass();
}

void AStackGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StackGameState = Cast<AStackGameState>(GameState);
}

void AStackGameModeBase::AddScore()
{
	auto StackPlayerController = Cast<AStackPlayerController>(GetWorld()->GetFirstPlayerController());
	StackPlayerController->AddGameScore();

	StackGameState->AddGameScore();
}

int32 AStackGameModeBase::GetScore() const
{
	return StackGameState->GetToTalGameScore();
}
