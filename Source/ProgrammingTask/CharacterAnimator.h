#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ProgrammingTaskCharacter.h"
#include "CharacterAnimator.generated.h"

UCLASS()
class PROGRAMMINGTASK_API UCharacterAnimator : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCharacterAnimator();
	~UCharacterAnimator() = default;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Animation Properties")
	SkatingAnimationState AnimState;
};
