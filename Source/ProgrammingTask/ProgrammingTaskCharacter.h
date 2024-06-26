// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ProgrammingTaskCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UENUM(Blueprintable)
enum class SkatingAnimationState : uint8
{
	Skating,
	SpeedUp,
	Jumping
};

UCLASS(config=Game)
class AProgrammingTaskCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

public:
	AProgrammingTaskCharacter();
	
	UFUNCTION(BlueprintCallable)
	void SpeedUp();

	UFUNCTION(BlueprintCallable)
	void EndJumpAnimation();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for stopping movement input */
	void ResetFrictionFactor(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	/** Called for jump input */
	void ExecuteJump(const FInputActionValue& Value);

	/** Called for pause input */
	void PauseGame(const FInputActionValue& Value);

	// Called when landed
	virtual void Landed(const FHitResult& Hit) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkateboardMesh;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SkateboardAttachmentComponent;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns CharacterState **/
	SkatingAnimationState* GetCharacterState() { return &CharacterState; }

private:
	class AProgrammingTaskGameMode* GM;
	SkatingAnimationState CharacterState;
	float SkatingSpeed;
	float FrictionFactor;
	bool bIsJumping;
	bool bIsJumpingAnimationFinished;
};

