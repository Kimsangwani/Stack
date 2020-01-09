// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Stack.h"
#include "GameFramework/Pawn.h"
#include "Components/TimelineComponent.h"
#include "Spawner.generated.h"

UCLASS()
class STACK_API ASpawner : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, Category = Tile, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class ATile> TileClass;

	UPROPERTY(EditAnywhere, Category = RemainderTile, meta = (AllowPrivateAccess = true))
		TSubclassOf<class ARemainderTile> RemainderTileClass;

	UPROPERTY(VisibleAnywhere, Category = Tile)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Spawner)
		UArrowComponent* LeftSpawner;

	UPROPERTY(VisibleAnywhere, Category = Spawner)
		UArrowComponent* RightSpawner;

	UPROPERTY(VisibleAnywhere, Category = Background)
		UStaticMeshComponent* Background;

	ATile* CurrentTile;
	ATile* PreviousTile;

	bool bCanMove;
	bool bIsLeftSpawn;
	bool bCanSpawn;
	bool bEndGame;

	float MoveSpeed;
	float StackZSize;
	float Tolerance;

	float ColorH;
	float ColorS;
	float ColorV;

	float ColorR;
	float ColorG;
	float ColorB;

	FVector PreviousColor;

	int32 InitStackNumber;

	FVector2D PreviousTouchLocation;
	FVector2D NewTouchLocation;

	UPROPERTY()
		class UTimelineComponent* ColorTimeline;

	UPROPERTY()
		UCurveFloat* FloatCurve;

	UPROPERTY()
		UMaterialInstanceDynamic* Brush;
	UPROPERTY()
		UMaterialInterface* ColourationBrush;

	FOnTimelineFloat InterpFunction{};
	//FOnTimelineEvent TimelineFinished{};

private:
	void SpawnTile();
	void PlaceTile();
	void MoveTile(class ATile* CurrentTile, float DeltaTime, float Speed);

	void CalculateIntersection(class ATile* CurrentTile, class ATile* PreviousTile);
	void SpawnRemainderTile(class ATile* CurrentTile, class ATile* PreviousTile, float RemainderScaleValue);

	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void GetColor();
	void SetColor(class ATile* Tile);
	void HSVtoRGB(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV);
	void ChangeBackgroundColor();

	UFUNCTION()
		void TimelineCallback(float Val);

	UFUNCTION()
		void TimelineFinishedCallback();

	void PlayTimeline();

	bool OffTrack();
	void EndGame();
};
