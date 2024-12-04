// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/WeatherVolume.h"

#include "Utility/CaelumUtilities.h"
#include "Kismet/KismetMathLibrary.h"

#include "Components/SphereComponent.h"

AWeatherVolume::AWeatherVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	WeatherToChangeTo = EWeatherPresets::Sunny;
	ChangeCondition = EChangeCondition::SpecifiedTimeframe;

	Timeframe = FFloatRange(FFloatRangeBound().Inclusive(0.f), FFloatRangeBound().Exclusive(24.f));

	TriggerField = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	TriggerField->SetupAttachment(RootComponent);
}

void AWeatherVolume::OnEnterField(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyindex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player"))
		return;


	switch (ChangeCondition)
	{
		float currentTime;
		case EChangeCondition::SpecifiedTimeframe:
			if (!(Timeframe.HasLowerBound() && Timeframe.HasUpperBound()))
				return;

			currentTime = UCaelumUtilities::GetTimeOfDaySystem(this)->GetCurrentTime();
			if (!UKismetMathLibrary::InRange_FloatFloat(currentTime, Timeframe.GetLowerBoundValue(), Timeframe.GetUpperBoundValue(), true, false))
				return;
			break;
		case EChangeCondition::ContactOnly:
			break;
		default:
			break;
	}

	FString message = "Weather changed!";
	GEngine->AddOnScreenDebugMessage(256, 3.f, FColor::Green, message);
}

void AWeatherVolume::BeginPlay()
{
	TriggerField->OnComponentBeginOverlap.AddDynamic(this, &AWeatherVolume::OnEnterField);
}
