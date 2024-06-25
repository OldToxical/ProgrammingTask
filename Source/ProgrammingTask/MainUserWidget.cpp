#include "MainUserWidget.h"
#include "Components/TextBlock.h"
#include "ProgrammingTaskGameMode.h"

UMainUserWidget::UMainUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UMainUserWidget::NativeConstruct()
{
}

void UMainUserWidget::UpdateScore(float Score)
{
	if (IsValid(ScoreValueTextBlock))
		ScoreValueTextBlock->SetText(FText::FromString(FString::SanitizeFloat(Score)));
}