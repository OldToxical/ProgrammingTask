#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

UCLASS()
class PROGRAMMINGTASK_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPauseWidget(const FObjectInitializer& ObjectInitializer);
	~UPauseWidget() = default;

	virtual void NativeConstruct() override;


protected:
	UFUNCTION()
	void OnClick_ResumeButton();
	UFUNCTION()
	void OnClick_RestartButton();
	UFUNCTION()
	void OnClick_QuitButton();

	// Initialization properties
	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	bool bIsInitialized;
	class AProgrammingTaskGameMode* GM;
};
