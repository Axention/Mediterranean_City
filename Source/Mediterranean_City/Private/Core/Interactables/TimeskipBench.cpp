// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Interactables/TimeskipBench.h"

#include "Utility/CaelumUtilities.h"
#include "Core/Character/AtmoCharacter.h"
#include "Effects/SkySystem.h"

#include "Kismet/KismetMathLibrary.h"


ATimeskipBench::ATimeskipBench()
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

}

void ATimeskipBench::BeginPlay()
{
  Super::BeginPlay();

  OnTimeSkipInteraction.BindUFunction(UCaelumUtilities::GetTimeOfDaySystem(GetWorld()), FName("SkipTime"));
}

void ATimeskipBench::Interact_Implementation(AAtmoCharacter* Character)
{
  ASkySystem* sky = UCaelumUtilities::GetTimeOfDaySystem(this);
  switch (Character->GetMoveState()) {
  case EMoveState::MS_Sitting:
    if (sky->IsSkipOnCooldown())
      return;
    if (Character->GetTimeToSkipTo() != (int8)sky->GetCurrentTime())
      OnTimeSkipInteraction.ExecuteIfBound(Character->GetTimeToSkipTo());
    break;

  default:
    if (UKismetMathLibrary::Dot_VectorVector(Character->GetActorForwardVector(), GetActorForwardVector()) > -0.25)
      return;
    Character->SitDown(WarpTarget, false);
    OnInteractionDelegate.ExecuteIfBound();
    break;
  }
}

void ATimeskipBench::SetHighlight_Implementation(bool newState)
{
  Mesh->SetRenderCustomDepth(newState);
}