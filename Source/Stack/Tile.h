// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Stack.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class STACK_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Tile)
		UStaticMeshComponent* Tile;

	UPROPERTY()
		UMaterialInstanceDynamic* Brush;
	UPROPERTY()
		UMaterialInterface* ColourationBrush;
public:

	void SetPhysics();
	void ChangeMaterials(float R, float G, float B);
	FVector GetMeshZSize();
};
