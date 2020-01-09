// Fill out your copyright notice in the Description page of Project Settings.


#include "RemainderTile.h"

ARemainderTile::ARemainderTile()
{
	Tile->SetSimulatePhysics(true);
	Tile->SetCollisionProfileName(TEXT("BlockAll"));
}

void ARemainderTile::BeginPlay()
{
	Super::BeginPlay();

	
}