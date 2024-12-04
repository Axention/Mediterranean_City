// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Character/AtmoCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

#include "Core/Interaction/InteractionComponent.h"
#include "Core/Character/ActionAnimComponent.h"

#include "MotionWarpingComponent.h"

AAtmoCharacter::AAtmoCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CamBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CamBoom->SetupAttachment(GetMesh(), "head");

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CamBoom);

	IAComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));

	ActionAnimComponent = CreateDefaultSubobject<UActionAnimComponent>(TEXT("Action Animation Component"));

	MotionWarper = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping Component"));

}

void AAtmoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(AtmoBaseMappingContext, 0);
		}
	}


	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (Input)
	{
		Input->BindAction(Actions["Move"], ETriggerEvent::Triggered, this, &AAtmoCharacter::Move);

		Input->BindAction(Actions["Look"], ETriggerEvent::Triggered, this, &AAtmoCharacter::Look);

		Input->BindAction(Actions["Run"], ETriggerEvent::Triggered, this, &AAtmoCharacter::Run);
		Input->BindAction(Actions["Run"], ETriggerEvent::Completed, this, &AAtmoCharacter::Run);

		Input->BindAction(Actions["Interact"], ETriggerEvent::Triggered, this, &AAtmoCharacter::Interact);
	}
	
}

void AAtmoCharacter::Tick(float DeltaSeconds)
{
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		ClampRotation();
	}

	if (GetMoveState() != MS_Sitting)
		IAComponent->SeekInteractible(Camera->GetComponentLocation(), Camera->GetForwardVector());
	else
		IAComponent->ResetSeeking();
}

void AAtmoCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAtmoCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D inputVector = Value.Get<FVector2D>();

	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
		return;

	if (GetMoveState() == MS_Sitting)
		GetActionAnimComponent()->StandUp();

	AddMovementInput(GetActorForwardVector(), inputVector.X);
	AddMovementInput(UKismetMathLibrary::GetRightVector(GetControlRotation()), inputVector.Y);
}

void AAtmoCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D inputVector = Value.Get<FVector2D>();

	AddControllerYawInput(inputVector.X);
	AddControllerPitchInput(inputVector.Y);

	ClampRotation();
}

void AAtmoCharacter::Run(const FInputActionValue& Value)
{
	bool bRun = Value.Get<bool>();

	if (UKismetMathLibrary::InverseTransformDirection(GetTransform(), GetPendingMovementInputVector()).X < 0.0)
		bRun = false;

	if (bRun)
		OnRun();
	else
		OnStopRun();
}

void AAtmoCharacter::Interact(const FInputActionValue& Value)
{
	IAComponent->TryInteraction(GetMesh()->GetSocketLocation(FName("head")), GetControlRotation().Vector());
}

void AAtmoCharacter::ClampRotation()
{
	FRotator rot = GetControlRotation();
	rot.Pitch = UKismetMathLibrary::ClampAngle(rot.Pitch, -60.0, 50.0);
	rot.Yaw = UKismetMathLibrary::ClampAngle(rot.Yaw, GetActorForwardVector().Rotation().Yaw - 50.0, GetActorForwardVector().Rotation().Yaw + 50.0);
	
	GetController()->SetControlRotation(rot);
}


