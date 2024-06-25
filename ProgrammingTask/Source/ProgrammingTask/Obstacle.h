#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class PROGRAMMINGTASK_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& result);

	UPROPERTY(EditAnywhere)
	class UBoxComponent* Collider;
	UPROPERTY(EditAnywhere)
	float Reward;
	UPROPERTY(EditAnywhere)
	bool bIsInfinite;
	
	class AProgrammingTaskGameMode* GM;
};
