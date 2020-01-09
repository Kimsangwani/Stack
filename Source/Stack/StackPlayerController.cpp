// Fill out your copyright notice in the Description page of Project Settings.


#include "StackPlayerController.h"
#include "HUDWidget.h"
#include "GameResultWidget.h"
#include "StackGameState.h"
#include "StackPlayerState.h"


AStackPlayerController::AStackPlayerController()
{
	bEnableClickEvents = true;
	bShowMouseCursor = true;

	static ConstructorHelpers::FClassFinder<UHUDWidget> UI_HUD_C(TEXT("/Game/Blueprints/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UGameResultWidget> UI_GameResult_C(TEXT("/Game/Blueprints/UI/UI_Result.UI_Result_C"));
	if (UI_GameResult_C.Succeeded())
	{
		GameResultWidgetClass = UI_GameResult_C.Class;
	}
}

void AStackPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ChangeInputMode(true);

	HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
	CHECK(nullptr != HUDWidget);
	HUDWidget->AddToViewport();

	GameResultWidget = CreateWidget<UGameResultWidget>(this, GameResultWidgetClass);
	CHECK(nullptr != GameResultWidgetClass);

	StackPlayerState = Cast<AStackPlayerState>(PlayerState);
	CHECK(nullptr != StackPlayerState);
	HUDWidget->BindPlayerState(StackPlayerState);
	StackPlayerState->OnPlayerStateChanged.Broadcast();

}

void AStackPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void AStackPlayerController::ShowResultUI()
{
	auto StackGameState = Cast<AStackGameState>(UGameplayStatics::GetGameState(this));
	CHECK(nullptr != StackGameState);
	GameResultWidget->BindGameState(StackGameState);

	GameResultWidget->AddToViewport();
	ChangeInputMode(false);
}

void AStackPlayerController::AddGameScore() const
{
	StackPlayerState->AddGameScore();
}
