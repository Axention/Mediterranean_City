// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Character/ActionAnimComponent.h"

#include "Components/CapsuleComponent.h"
#include "Core/Character/AtmoCharacter.h"


UActionAnimComponent::UActionAnimComponent()
{
  PrimaryComponentTick.bCanEverTick = false;

  SitDownMontage = nullptr;
  StandUpMontage = nullptr;
  Player = nullptr;
}

void UActionAnimComponent::BeginPlay()
{
  Super::BeginPlay();

  Player = CastChecked<AAtmoCharacter>(GetOwner());
}

// Begin Actions ---

void UActionAnimComponent::SitDown()
{
  Player->SetMoveState(EMoveState::MS_Sitting);

  Player->bUseControllerRotationYaw = false;
  Player->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
  Player->GetMesh()->GetAnimInstance()->Montage_Play(SitDownMontage);
}

void UActionAnimComponent::StandUp()
{
  Player->GetMesh()->GetAnimInstance()->Montage_Play(StandUpMontage);
  Player->GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &UActionAnimComponent::OnMontageBlendOut);
}

void UActionAnimComponent::OnMontageBlendOut(UAnimMontage* Montage, bool interrupted)
{
  Player->SetMoveState(EMoveState::MS_Idle);

  Player->bUseControllerRotationYaw = true;
  Player->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

  Player->GetMesh()->GetAnimInstance()->OnMontageBlendingOut.RemoveDynamic(this, &UActionAnimComponent::OnMontageBlendOut);
}

// End Actions ---