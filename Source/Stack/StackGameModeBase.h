// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Stack.h"
#include "GameFramework/GameModeBase.h"
#include "StackGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class STACK_API AStackGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStackGameModeBase();
	virtual void PostInitializeComponents() override;

	void AddScore();
	int32 GetScore() const;

private:
	UPROPERTY()
		class AStackGameState* StackGameState;
};
