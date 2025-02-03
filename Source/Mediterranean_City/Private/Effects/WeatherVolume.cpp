// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/WeatherVolume.h"

#include "Core/Interaction/InteractableBase.h"

#include "Utility/CaelumUtilities.h"
#include "Kismet/KismetMathLibrary.h"

#include "Components/SphereComponent.h"

AWeatherVolume::AWeatherVolume()
{
  PrimaryActorTick.bCanEverTick = false;

  WeatherToChangeTo = nullptr;
  ChangeCondition = EChangeCondition::SpecifiedTimeframe;

  Timeframe = FFloatRange(FFloatRangeBound().Inclusive(0.f), FFloatRangeBound().Exclusive(24.f));

  TriggerField = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
  TriggerField->SetupAttachment(RootComponent);
}

void AWeatherVolume::BeginPlay()
{
  TriggerField->OnComponentBeginOverlap.AddDynamic(this, &AWeatherVolume::OnEnterField);
  TriggerField->OnComponentEndOverlap.AddDynamic(this, &AWeatherVolume::OnExitField);
}

void AWeatherVolume::OnEnterField(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyindex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (!OtherActor->ActorHasTag("Player"))
    return;

  switch (ChangeCondition) {

  case EChangeCondition::SpecifiedTimeframe:
  {
    if (!(Timeframe.HasLowerBound() && Timeframe.HasUpperBound())) {
      UE_LOG(LogTemp, Error, TEXT("Timeframe has no Bounds! (Upper or Lower)"));
      return;
    }

    if (!UKismetMathLibrary::InRange_FloatFloat(UCaelumUtilities::GetTimeOfDaySystem(this)->GetCurrentTime(), Timeframe.GetLowerBoundValue(), Timeframe.GetUpperBoundValue(), true, false))
      return;

    SendWeatherChange();
    return;

    break;
  }
  case EChangeCondition::InteractionTriggered:
  {
    if (InteractionTarget != nullptr)
      InteractionTarget->OnInteractionDelegate.BindUFunction(this, FName("OnInteract"));
    return;
    break;
  }
  default:
    SendWeatherChange();
    break;
  }
}

void AWeatherVolume::OnExitField(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyindex)
{
  if (!OtherActor->ActorHasTag("Player"))
    return;

  if (InteractionTarget != nullptr)
    InteractionTarget->OnInteractionDelegate.Unbind();
}

void AWeatherVolume::OnInteract()
{
  if (SendWeatherChange())
    InteractionTarget->OnInteractionDelegate.Unbind();
}

bool AWeatherVolume::SendWeatherChange()
{
  if (!UCaelumUtilities::GetTimeOfDaySystem(this)->IsWeatherReady())
    return false;

  UCaelumUtilities::GetTimeOfDaySystem(this)->ChangeWeather(WeatherToChangeTo);
  GEngine->AddOnScreenDebugMessage(888, 4.f, FColor::Emerald, FString("Weather Changing via Event"));
  return true;
}
