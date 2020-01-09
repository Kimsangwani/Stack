// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "Components/TextBlock.h"
#include "StackPlayerState.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Score = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtScore")));
	CHECK(nullptr != Score);
}

void UHUDWidget::BindPlayerState(class AStackPlayerState* PlayerState)
{
	CHECK(nullptr != PlayerState);
	CurrentPlayerState = PlayerState;
	PlayerState->OnPlayerStateChanged.AddUObject(this, &UHUDWidget::UpdateScore);
}

void UHUDWidget::UpdateScore()
{
	CHECK(CurrentPlayerState.IsValid());

	Score->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));
}