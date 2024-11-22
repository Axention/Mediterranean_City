// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ActionAnimComponent.h"

#include "AtmoCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


UActionAnimComponent::UActionAnimComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SitDownMontage = nullptr;
	StandUpMontage = nullptr;
	Player = nullptr;
}

void UActionAnimComponent::SitDown()
{
	Player->SetMoveState(MS_Sitting);

	Player->bUseControllerRotationYaw = false;
	Player->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	Player->GetMesh()->GetAnimInstance()->Montage_Play(SitDownMontage);
}

void UActionAnimComponent::StandUp()
{
	Player->GetMesh()->GetAnimInstance()->Montage_Play(StandUpMontage);
	Player->GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &UActionAnimComponent::OnMontageBlendOut);
}

void UActionAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = CastChecked<AAtmoCharacter>(GetOwner());
}

void UActionAnimComponent::OnMontageBlendOut(UAnimMontage* Montage, bool interrupted)
{
	Player->SetMoveState(MS_Idle);

	Player->bUseControllerRotationYaw = true;
	Player->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

	Player->GetMesh()->GetAnimInstance()->OnMontageBlendingOut.RemoveDynamic(this, &UActionAnimComponent::OnMontageBlendOut);
}
