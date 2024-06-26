#include "PauseWidget.h"
#include "ProgrammingTaskGameMode.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

UPauseWidget::UPauseWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsInitialized = false;
}

void UPauseWidget::NativeConstruct()
{
	if (!bIsInitialized)
	{
		// Get a reference to the game mode instance
		auto Temp = GetWorld()->GetAuthGameMode();
		GM = Cast<AProgrammingTaskGameMode>(Temp);

		// Initialize Buttons
		if (IsValid(ResumeButton))
			ResumeButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClick_ResumeButton);

		if (IsValid(QuitButton))
			QuitButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClick_QuitButton);

		if (IsValid(RestartButton))
			RestartButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClick_RestartButton);

		bIsInitialized = true;
	}
}

void UPauseWidget::OnClick_ResumeButton()
{
	if (IsValid(GM))
		GM->PauseGame();
}

void UPauseWidget::OnClick_RestartButton()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Showcase");
}

void UPauseWidget::OnClick_QuitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}