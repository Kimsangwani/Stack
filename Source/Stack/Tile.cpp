// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile"));
	Tile->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_TILE(TEXT("/Game/Resources/Block.Block"));
	if (SM_TILE.Succeeded())
	{
		Tile->SetStaticMesh(SM_TILE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MI_BlOCK(TEXT("/Game/Resources/MI_Block.MI_Block"));
	if (MI_BlOCK.Succeeded())
	{
		ColourationBrush = MI_BlOCK.Object;
	}

	Tile->SetCollisionProfileName(TEXT("Block"));
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	Brush = UMaterialInstanceDynamic::Create(Tile->GetStaticMesh()->GetMaterial(0), this);
	CHECK(nullptr != Brush);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::SetPhysics()
{
	Tile->SetSimulatePhysics(true);
}

void ATile::ChangeMaterials(float R, float G, float B)
{
	Brush->SetVectorParameterValue(FName("Param"), FLinearColor(R, G, B, 1));
	Tile->SetMaterial(0, Brush);
}

FVector ATile::GetMeshZSize()
{
	FVector BoxExtent = Tile->CalcBounds(Tile->GetComponentTransform()).BoxExtent;

	return BoxExtent;
}