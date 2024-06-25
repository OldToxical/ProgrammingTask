// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProgrammingTaskGameMode.h"
#include "ProgrammingTaskCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MainUserWidget.h"

AProgrammingTaskGameMode::AProgrammingTaskGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	Score = 0.f;
	ActiveWidgetIndex = 0;
}

void AProgrammingTaskGameMode::AddWidgets(TArray<class UUserWidget*> Widgets_)
{
	Widgets = Widgets_;

	Cast<UMainUserWidget>(Widgets[0])->UpdateScore(Score);
	SwitchWidget(0);
}

void AProgrammingTaskGameMode::PauseGame()
{
	APlayerController* const PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));

	if (IsValid(PlayerController))
	{
		if (!PlayerController->IsPaused())
		{
			SwitchWidget(1);
			PlayerController->SetPause(true);
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;
		}
		else
		{
			SwitchWidget(0);
			PlayerController->SetPause(false);
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false;
		}
	}
}

void AProgrammingTaskGameMode::UpdateScore(float NewScore)
{
	Score = NewScore;
	Cast<UMainUserWidget>(Widgets[0])->UpdateScore(Score);
}

void AProgrammingTaskGameMode::SwitchWidget(int WidgetIndex)
{
	if (Widgets.Num() > 0)
	{
		Widgets[ActiveWidgetIndex]->RemoveFromParent();
		Widgets[WidgetIndex]->AddToViewport();
		ActiveWidgetIndex = WidgetIndex;
	}
}