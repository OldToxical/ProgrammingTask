// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProgrammingTaskCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProgrammingTaskGameMode.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AProgrammingTaskCharacter

AProgrammingTaskCharacter::AProgrammingTaskCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	CharacterState = SkatingAnimationState::Skating;
	SkatingSpeed = 0.f;
	FrictionFactor = 150.f;
	bIsJumping = false;
}

void AProgrammingTaskCharacter::SpeedUp()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		SkatingSpeed = 1100.f;
		CharacterState = SkatingAnimationState::Skating;
	}
}

void AProgrammingTaskCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Get a reference to the game mode instance
	auto Temp = GetWorld()->GetAuthGameMode();
	GM = Cast<AProgrammingTaskGameMode>(Temp);

	TSet<UActorComponent*> Components = GetComponents();
	for (UActorComponent* Component : Components)
	{
		if (Component->ComponentHasTag(FName("Skateboard")))
			SkateboardMesh = Cast<USkeletalMeshComponent>(Component);

		if (Component->ComponentHasTag(FName("SkateboardAttachment")))
			SkateboardAttachmentComponent = Cast<USceneComponent>(Component);
	}
}

void AProgrammingTaskCharacter::Tick(float DeltaTime)
{
	if (SkatingSpeed > 0.f)
		SkatingSpeed -= FrictionFactor * DeltaTime;

	// Get the forward vector of the character
	FVector ForwardVector = GetActorForwardVector();

	// Adjust the velocity direction to match the forward vector
	FVector NewVelocity = ForwardVector * SkatingSpeed;

	// Update the character's velocity
	GetCharacterMovement()->Velocity.X = NewVelocity.X;
	GetCharacterMovement()->Velocity.Y = NewVelocity.Y;
}

void AProgrammingTaskCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AProgrammingTaskCharacter::ExecuteJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProgrammingTaskCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &AProgrammingTaskCharacter::ResetFrictionFactor);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AProgrammingTaskCharacter::ResetFrictionFactor);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProgrammingTaskCharacter::Look);

		// Pause
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AProgrammingTaskCharacter::PauseGame);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProgrammingTaskCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		if (MovementVector.Y > 0.f)
		{
			if (!GetCharacterMovement()->IsFalling() && !bIsJumping)
			{
				CharacterState = SkatingAnimationState::SpeedUp;
				
			}
				
			AddMovementInput(ForwardDirection, MovementVector.Y * 0.001f);
		}
		else if (MovementVector.Y < 0.f)
		{
			FrictionFactor = 800.f;
		}
			
		AddMovementInput(RightDirection, MovementVector.X * 0.001f);
	}
}

void AProgrammingTaskCharacter::ResetFrictionFactor(const FInputActionValue& Value)
{
	FrictionFactor = 150.f;
	CharacterState = SkatingAnimationState::Skating;
}

void AProgrammingTaskCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AProgrammingTaskCharacter::ExecuteJump(const FInputActionValue& Value)
{
	if (IsValid(SkateboardMesh))
	{
		EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;
		EAttachmentRule RotationRule = EAttachmentRule::KeepWorld;
		EAttachmentRule ScaleRule = EAttachmentRule::KeepRelative;

		FAttachmentTransformRules AttachmentRules(LocationRule, RotationRule, ScaleRule, true);
		SkateboardMesh->AttachToComponent(GetMesh(), AttachmentRules, FName("LeftToeBase"));
	}
		

	bIsJumping = true;
	CharacterState = SkatingAnimationState::Jumping;
}

void AProgrammingTaskCharacter::PauseGame(const FInputActionValue& Value)
{
	if (IsValid(GM))
		GM->PauseGame();
}

void AProgrammingTaskCharacter::Landed(const FHitResult& Hit)
{
	if (IsValid(SkateboardMesh) && IsValid(SkateboardAttachmentComponent))
		SkateboardMesh->AttachToComponent(SkateboardAttachmentComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	bIsJumping = false;
	CharacterState = SkatingAnimationState::Skating;
}