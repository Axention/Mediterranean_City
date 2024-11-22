// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ActionAnimComponent.h"

#include "AtmoCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"


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
	Player->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_None;
}

void UActionAnimComponent::StandUp()
{
	Player->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking; // only for debug purposes while no montages are implemented!
	Player->SetMoveState(MS_Idle);
}

void UActionAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = CastChecked<AAtmoCharacter>(GetOwner());
}


