#include "Obstacle.h"
#include "Components/BoxComponent.h"
#include "ProgrammingTaskGameMode.h"
#include "ProgrammingTaskCharacter.h"

AObstacle::AObstacle()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	Reward = 100.f;
	bIsInfinite = false;
	GM = nullptr;
}

void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OverlapBegin);

	// Get a reference to the game mode instance
	auto Temp = GetWorld()->GetAuthGameMode();
	GM = Cast<AProgrammingTaskGameMode>(Temp);
}

void AObstacle::OverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& result)
{
	if (otherActor->IsA(AProgrammingTaskCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, otherActor->GetName());

		if (IsValid(GM))
			GM->UpdateScore(GM->GetScore() + Reward);

		if (!bIsInfinite)
			Destroy();
	}
}