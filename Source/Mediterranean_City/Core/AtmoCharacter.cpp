// Fill out your copyright notice in the Description page of Project Settings.


#include "AtmoCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

#include "Interaction/InteractionComponent.h"

#include "../Effects/PhysicalSky.h"


// Sets default values
AAtmoCharacter::AAtmoCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CamBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CamBoom->SetupAttachment(GetMesh(), "head");

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CamBoom);

	IAComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));

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

// Called when the game starts or when spawned
void AAtmoCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTimeSkipInteraction.BindUFunction(UGameplayStatics::GetActorOfClass(GetWorld(), APhysicalSky::StaticClass()), FName("SkipTime"));
}

void AAtmoCharacter::SkipTime(float Amount)
{
	OnTimeSkipInteraction.Execute(Amount);
}

void AAtmoCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D inputVector = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), inputVector.X);
	AddMovementInput(UKismetMathLibrary::GetRightVector(GetControlRotation()), inputVector.Y);
}

void AAtmoCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D inputVector = Value.Get<FVector2D>();

	AddControllerYawInput(inputVector.X);
	AddControllerPitchInput(inputVector.Y);

	FRotator rot = GetControlRotation();
	rot.Pitch = UKismetMathLibrary::ClampAngle(rot.Pitch, -60.0, 50.0);
	GetController()->SetControlRotation(rot);
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


