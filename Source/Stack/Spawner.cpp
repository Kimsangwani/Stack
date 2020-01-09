// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "Tile.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/InputSettings.h"
#include "RemainderTile.h"
#include "StackGameModeBase.h"
#include "StackPlayerController.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	LeftSpawner = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftSpawner"));
	RightSpawner = CreateDefaultSubobject<UArrowComponent>(TEXT("RightSpawner"));
	Background = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Background"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BACKGROUND(TEXT("/Game/Resources/Background.Background"));
	if (SM_BACKGROUND.Succeeded())
	{
		Background->SetStaticMesh(SM_BACKGROUND.Object);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CURVE(TEXT("/Game/Blueprints/ColorCurve.ColorCurve"));
	if (CURVE.Succeeded())
	{
		FloatCurve = CURVE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MI_BACKGROUND(TEXT("/Game/Resources/MI_Background.MI_Background"));
	if (MI_BACKGROUND.Succeeded())
	{
		ColourationBrush = MI_BACKGROUND.Object;
	}

	RootComponent = Root;
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);;
	LeftSpawner->SetupAttachment(RootComponent);
	RightSpawner->SetupAttachment(RootComponent);
	Background->SetupAttachment(Camera);

	SpringArm->TargetArmLength = 500.0f;
	SpringArm->SetRelativeRotation(FRotator(-30.0f, 45.0f, 0.0f));
	SpringArm->bDoCollisionTest = false;
	Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
	LeftSpawner->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	RightSpawner->SetRelativeLocation(FVector(0.0f, 100.0f, 0.0f));
	RightSpawner->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	Background->SetRelativeLocation(FVector(700.0f, 0.0f, -510.0f));
	Background->SetRelativeRotation(FRotator(-90.0f, 160.0f, 10.0f));
	Background->SetWorldScale3D(FVector(18.0f, 18.0f, 1.0f));

	TileClass = ATile::StaticClass();
	RemainderTileClass = ARemainderTile::StaticClass();

	MoveSpeed = 200.0f;
	InitStackNumber = 15;
	StackZSize = 10.0f;
	Tolerance = 3.0f;
	ColorH = 175.0f;
	ColorG = 0.5f;
	ColorV = 1.0f;

	bCanMove = true;
	bIsLeftSpawn = true;
	bCanSpawn = false;
	bEndGame = false;
	
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	if (FloatCurve != NULL)
	{
		ColorTimeline = NewObject<UTimelineComponent>(this, FName("TimelineAnimation"));
		ColorTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript; 
		this->BlueprintCreatedComponents.Add(ColorTimeline); 
		ColorTimeline->SetNetAddressable();	

		//ColorTimeline->SetPropertySetObject(this);

		ColorTimeline->SetLooping(false);
		ColorTimeline->SetTimelineLength(5.0f);
		//ColorTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		//ColorTimeline->SetPlaybackPosition(0.0f, false);

		InterpFunction.BindUFunction(this, FName{ TEXT("TimelineCallback") });
		//TimelineFinished.BindUFunction(this, FName{ TEXT("TimelineFinishedCallback") });
		ColorTimeline->AddInterpFloat(FloatCurve, InterpFunction);
		//ColorTimeline->SetTimelineFinishedFunc(TimelineFinished);

		ColorTimeline->RegisterComponent();
	}
	
	Brush = UMaterialInstanceDynamic::Create(Background->GetStaticMesh()->GetMaterial(0), this);
	CHECK(nullptr != Brush);

	CHECK(nullptr != TileClass);
	for (int32 i = 0; i < InitStackNumber - 1; i++)
	{
		FVector Position = RootComponent->GetComponentLocation();
		ATile* Tile = GetWorld()->SpawnActor<ATile>(TileClass, Position, FRotator::ZeroRotator);
		SetColor(Tile);
		RootComponent->AddWorldOffset(FVector(0.0f, 0.0f, StackZSize));
	}

	FVector Position = RootComponent->GetComponentLocation();
	ATile* Tile = GetWorld()->SpawnActor<ATile>(TileClass, Position, FRotator::ZeroRotator);
	SetColor(Tile);
	RootComponent->AddWorldOffset(FVector(0.0f, 0.0f, StackZSize));
	PreviousTile = Tile;

	PreviousColor = FVector(ColorR, ColorG, ColorB);
	ChangeBackgroundColor();

	SpawnTile();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTile && bCanMove)
	{
		MoveTile(CurrentTile, DeltaTime, MoveSpeed);
	}

	if (ColorTimeline != NULL)
	{
		ColorTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
	}

	if (OffTrack() && !bEndGame)
	{
		EndGame();
	}
}

// Called to bind functionality to input
void ASpawner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASpawner::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASpawner::TouchStopped);
}

void ASpawner::TimelineCallback(float Val)
{
	FVector Color = FMath::Lerp(PreviousColor, FVector(ColorR, ColorG, ColorB), Val);
	Brush->SetVectorParameterValue(FName("Param"), FLinearColor(Color.X, Color.Y, Color.Z, 1.0f));
	Background->SetMaterial(0, Brush);
}

void ASpawner::TimelineFinishedCallback()
{
}

void ASpawner::PlayTimeline()
{
	if (ColorTimeline != NULL)
	{
		ColorTimeline->PlayFromStart();
	}
}

void ASpawner::SpawnTile()
{
	PreviousColor = FVector(ColorR, ColorG, ColorB);

	if (bIsLeftSpawn)
	{
		ATile* LeftTile = GetWorld()->SpawnActor<ATile>(TileClass, FVector::ZeroVector, FRotator::ZeroRotator);
		FVector LeftLocation = FVector(LeftSpawner->GetComponentLocation().X * 2, PreviousTile->GetActorLocation().Y, LeftSpawner->GetComponentLocation().Z);
		LeftTile->SetActorLocation(LeftLocation);
		LeftTile->SetActorScale3D(PreviousTile->GetActorScale3D());
		CurrentTile = LeftTile;
	}
	else
	{
		ATile* RightTile = GetWorld()->SpawnActor<ATile>(TileClass, FVector::ZeroVector, FRotator::ZeroRotator);
		FVector RightLocation = FVector(PreviousTile->GetActorLocation().X, RightSpawner->GetComponentLocation().Y * 2, RightSpawner->GetComponentLocation().Z);
		RightTile->SetActorLocation(RightLocation);
		RightTile->SetActorScale3D(PreviousTile->GetActorScale3D());
		CurrentTile = RightTile;
	}

	SetColor(CurrentTile);
	ChangeBackgroundColor();
	bCanMove = true;
}

void ASpawner::PlaceTile()
{
	bCanMove = false;

	if (CurrentTile->IsOverlappingActor(PreviousTile))
	{
		RootComponent->AddWorldOffset(FVector(0.0f, 0.0f, StackZSize));
		CalculateIntersection(CurrentTile, PreviousTile);
		PreviousTile = CurrentTile;
		bIsLeftSpawn = !bIsLeftSpawn;
		SpawnTile();

		auto StackGameMode = Cast<AStackGameModeBase>(GetWorld()->GetAuthGameMode());
		CHECK(nullptr != StackGameMode);
		StackGameMode->AddScore();
	}
	else
	{
		EndGame();
	}
}

void ASpawner::MoveTile(ATile* CurrentTile, float DeltaTime, float Speed)
{
	FVector Location;
	if (bIsLeftSpawn)
	{
		Location = FVector(-DeltaTime * Speed, 0.0f, 0.0f);
	}
	else
	{
		Location = FVector(0.0f, -DeltaTime * Speed, 0.0f);
	}

	CurrentTile->AddActorWorldOffset(Location);
}

void ASpawner::CalculateIntersection(ATile* CurrentTile, ATile* PreviousTile)
{
	float RemainderScaleValue = 0;

	if (bIsLeftSpawn)
	{
		float CurrentTileLocationX;
		float PreviousTileLocationX;
		float PreviousTileScaleX;

		CurrentTileLocationX = CurrentTile->GetActorLocation().X;
		PreviousTileLocationX = PreviousTile->GetActorLocation().X;
		PreviousTileScaleX = PreviousTile->GetActorScale().X;

		if (FMath::Abs(CurrentTileLocationX - PreviousTileLocationX) <= Tolerance)
		{
			CurrentTile->SetActorLocation(FVector(PreviousTile->GetActorLocation().X, CurrentTile->GetActorLocation().Y, CurrentTile->GetActorLocation().Z));
		}
		else
		{
			CurrentTile->SetActorScale3D(FVector(PreviousTileScaleX - FMath::Abs((CurrentTileLocationX - PreviousTileLocationX) / 100), CurrentTile->GetActorScale3D().Y, CurrentTile->GetActorScale3D().Z));
			CurrentTile->SetActorLocation(FVector((CurrentTileLocationX + PreviousTileLocationX) / 2, CurrentTile->GetActorLocation().Y, CurrentTile->GetActorLocation().Z));
			RemainderScaleValue = (CurrentTileLocationX - PreviousTileLocationX) / 100;
			SpawnRemainderTile(CurrentTile, PreviousTile, RemainderScaleValue);
		}	
	}
	else
	{
		float CurrentTileLocationY;
		float PreviousTileLocationY;
		float PreviousTileScaleY;

		CurrentTileLocationY = CurrentTile->GetActorLocation().Y;
		PreviousTileLocationY = PreviousTile->GetActorLocation().Y;
		PreviousTileScaleY = PreviousTile->GetActorScale().Y;

		if (FMath::Abs(CurrentTileLocationY - PreviousTileLocationY) <= Tolerance)
		{
			CurrentTile->SetActorLocation(FVector(CurrentTile->GetActorLocation().X, PreviousTile->GetActorLocation().Y, CurrentTile->GetActorLocation().Z));
		}
		else
		{
			CurrentTile->SetActorScale3D(FVector(CurrentTile->GetActorScale3D().X, PreviousTileScaleY - FMath::Abs((CurrentTileLocationY - PreviousTileLocationY) / 100), CurrentTile->GetActorScale3D().Z));
			CurrentTile->SetActorLocation(FVector(CurrentTile->GetActorLocation().X, (CurrentTileLocationY + PreviousTileLocationY) / 2, CurrentTile->GetActorLocation().Z));
			RemainderScaleValue = (CurrentTileLocationY - PreviousTileLocationY) / 100;
			SpawnRemainderTile(CurrentTile, PreviousTile, RemainderScaleValue);
		}
	}
}

void ASpawner::SpawnRemainderTile(ATile* CurrentTile, ATile* PreviousTile, float RemainderScaleValue)
{
	if (bIsLeftSpawn)
	{
		ARemainderTile* RemainderTile = GetWorld()->SpawnActor<ARemainderTile>(RemainderTileClass, FVector::ZeroVector, FRotator::ZeroRotator);
		RemainderTile->SetActorScale3D(FVector(FMath::Abs(RemainderScaleValue), CurrentTile->GetActorScale3D().Y, CurrentTile->GetActorScale3D().Z));

		FVector Origin;
		FVector BoxExtent;
		PreviousTile->GetActorBounds(false, Origin, BoxExtent);
		float RemmainderLocationX = CurrentTile->GetActorLocation().X + (BoxExtent.X * FMath::Sign(RemainderScaleValue));
		RemainderTile->SetActorLocation(FVector(RemmainderLocationX, CurrentTile->GetActorLocation().Y, CurrentTile->GetActorLocation().Z));
		RemainderTile->ChangeMaterials(ColorR, ColorG, ColorB);
	}
	else
	{
		ARemainderTile* RemainderTile = GetWorld()->SpawnActor<ARemainderTile>(RemainderTileClass, FVector::ZeroVector, FRotator::ZeroRotator);
		RemainderTile->SetActorScale3D(FVector(CurrentTile->GetActorScale3D().X, FMath::Abs(RemainderScaleValue), CurrentTile->GetActorScale3D().Z));

		FVector Origin;
		FVector BoxExtent;
		PreviousTile->GetActorBounds(false, Origin, BoxExtent);
		float RemmainderLocatoinY = CurrentTile->GetActorLocation().Y + (BoxExtent.Y * FMath::Sign(RemainderScaleValue));
		RemainderTile->SetActorLocation(FVector(CurrentTile->GetActorLocation().Y, RemmainderLocatoinY, CurrentTile->GetActorLocation().Z));
		RemainderTile->ChangeMaterials(ColorR, ColorG, ColorB);
	}
}

void ASpawner::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	PlaceTile();
}

void ASpawner::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{

}

void ASpawner::GetColor()
{
	HSVtoRGB(ColorR, ColorG, ColorB, ColorH, ColorS, ColorV);
}

void ASpawner::SetColor(ATile* Tile)
{
	ColorH += 3;
	if (ColorV > 0.8)
	{
		ColorS -= 0.02;
		if (ColorS < 0.7)
		{
			ColorS = 0.8f;
			ColorV = 0.2f;
			ColorH = FMath::RandRange(0.0, 389);
		}
	}
	else
	{
		ColorV += 0.05;
	}

	GetColor();
	Tile->ChangeMaterials(ColorR, ColorG, ColorB);
}

bool ASpawner::OffTrack()
{
	if(CurrentTile->GetActorLocation().X + (PreviousTile->GetMeshZSize().X * 2) + Tolerance < PreviousTile->GetActorLocation().X ||
		CurrentTile->GetActorLocation().Y + (PreviousTile->GetMeshZSize().Y * 2) + Tolerance < PreviousTile->GetActorLocation().Y)
	{
		return true;
	}
	return false;
}

void ASpawner::EndGame()
{
	AStackPlayerController* StackPlayerController = Cast<AStackPlayerController>(GetController());
	CHECK(nullptr != StackPlayerController);
	StackPlayerController->ShowResultUI();
	CurrentTile->SetPhysics();
	bEndGame = true;
}

void ASpawner::HSVtoRGB(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV)
{
	float fC = fV * fS;
	float fHPrime = fmod(fH / 60.0, 6);
	float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
	float fM = fV - fC;

	if (0 <= fHPrime && fHPrime < 1) {
		fR = fC;
		fG = fX;
		fB = 0;
	}
	else if (1 <= fHPrime && fHPrime < 2) {
		fR = fX;
		fG = fC;
		fB = 0;
	}
	else if (2 <= fHPrime && fHPrime < 3) {
		fR = 0;
		fG = fC;
		fB = fX;
	}
	else if (3 <= fHPrime && fHPrime < 4) {
		fR = 0;
		fG = fX;
		fB = fC;
	}
	else if (4 <= fHPrime && fHPrime < 5) {
		fR = fX;
		fG = 0;
		fB = fC;
	}
	else if (5 <= fHPrime && fHPrime < 6) {
		fR = fC;
		fG = 0;
		fB = fX;
	}
	else {
		fR = 0;
		fG = 0;
		fB = 0;
	}

	fR += fM;
	fG += fM;
	fB += fM;
}

void ASpawner::ChangeBackgroundColor()
{
	PlayTimeline();
}

