#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUserWidget.generated.h"

UCLASS()
class PROGRAMMINGTASK_API UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainUserWidget(const FObjectInitializer& ObjectInitializer);
	~UMainUserWidget() = default;

	virtual void NativeConstruct() override;

	// Setters
	void UpdateScore(float Score);
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreValueTextBlock;
};
