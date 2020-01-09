// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "RemainderTile.generated.h"

/**
 * 
 */
UCLASS()
class STACK_API ARemainderTile : public ATile
{
	GENERATED_BODY()
	
public:
	ARemainderTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
