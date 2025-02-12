// Fill out your copyright notice in the Description page of Project Settings.


#include "Cinematics/CinematicBench.h"

#include "Core/Character/AtmoCharacter.h"
#include "Components/BoxComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


ACinematicBench::ACinematicBench()
{
  PrimaryActorTick.bCanEverTick = false;

  DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
  RootComponent = DefaultRoot;
  DefaultRoot->SetMobility(EComponentMobility::Static);

  InteractionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Interactable"));
  InteractionZone->SetupAttachment(RootComponent);

  Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
  Mesh->SetupAttachment(RootComponent);

  WarpTarget = CreateDefaultSubobject<USceneComponent>(TEXT("Target"));
  WarpTarget->SetupAttachment(RootComponent);

  isSitting = 0;
  character = nullptr;
}


void ACinematicBench::Interact_Implementation(AAtmoCharacter* Character)
{
  if (isSitting == 1) return;

  isSitting = 1;

  if (UKismetMathLibrary::Dot_VectorVector(Character->GetActorForwardVector(), GetActorForwardVector()) > -0.25)
    return;
  Character->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ACinematicBench::OnFullySitting);
  Character->SitDown(WarpTarget, true);
  Character->EnableLookOnly();
  character = Character;
}

void ACinematicBench::SetHighlight_Implementation(bool newState)
{
  Mesh->SetRenderCustomDepth(newState);
}

void ACinematicBench::OnFullySitting(UAnimMontage* Montage, bool bInterrupted)
{
  StartCredits();
}


