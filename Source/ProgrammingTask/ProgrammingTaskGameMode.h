#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProgrammingTaskGameMode.generated.h"

UCLASS(minimalapi)
class AProgrammingTaskGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProgrammingTaskGameMode();

	// Functions
	UFUNCTION(BlueprintCallable)
	void AddWidgets(TArray<class UUserWidget*> Widgets_);
	void PauseGame();

	// Getters
	float GetScore() { return Score; }	

	// Setters
	void UpdateScore(float NewScore);

private:
	void SwitchWidget(int WidgetIndex);

	UPROPERTY(VisibleAnywhere)
	float Score;
	UPROPERTY(VisibleAnywhere)
	TArray<class UUserWidget*> Widgets;
	UPROPERTY(VisibleAnywhere)
	int ActiveWidgetIndex;
};