// Fill out your copyright notice in the Description page of Project Settings.


#include "GameResultWidget.h"
#include "StackGameState.h"
#include "StackPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UGameResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UGameResultWidget::OnReturnToTitleClicked);
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryGameButton)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &UGameResultWidget::OnRetryGameClicked);
	}

	CHECK(CurrentGameState.IsValid());

	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	CHECK(nullptr != TotalScore);
	TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetToTalGameScore())));
}

void UGameResultWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

void UGameResultWidget::OnRetryGameClicked()
{
	auto StackPlayerController = Cast<AStackPlayerController>(GetOwningPlayer());
	CHECK(nullptr != StackPlayerController);
	StackPlayerController->RestartLevel();
}

void UGameResultWidget::BindGameState(class AStackGameState* GameState)
{
	CHECK(nullptr != GameState);
	CurrentGameState = GameState;
}