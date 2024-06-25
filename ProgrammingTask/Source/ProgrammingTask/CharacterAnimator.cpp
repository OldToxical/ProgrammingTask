#include "CharacterAnimator.h"

UCharacterAnimator::UCharacterAnimator()
{
}

void UCharacterAnimator::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	AActor* OwningActor = GetOwningActor();

	if (IsValid(OwningActor))
		if (AProgrammingTaskCharacter* Character = Cast<AProgrammingTaskCharacter>(OwningActor))
			AnimState = *Character->GetCharacterState();
}