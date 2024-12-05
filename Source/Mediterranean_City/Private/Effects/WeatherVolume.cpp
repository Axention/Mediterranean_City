// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/WeatherVolume.h"

#include "Core/Interaction/InteractableBase.h"

#include "Utility/CaelumUtilities.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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

	// TODO: check for weather cooldown

	switch (ChangeCondition)
	{

		case EChangeCondition::SpecifiedTimeframe:
		{
			if (!(Timeframe.HasLowerBound() && Timeframe.HasUpperBound()))
			{
				UE_LOG(LogTemp, Error, TEXT("Timeframe has no Bounds! (Upper or Lower)"));
				return;
			}

			if (!UKismetMathLibrary::InRange_FloatFloat(UCaelumUtilities::GetTimeOfDaySystem(this)->GetCurrentTime(), Timeframe.GetLowerBoundValue(), Timeframe.GetUpperBoundValue(), true, false))
				return;

			// UCaelumUtilities::GetTimeOfDaySystem(this)->GetWeatherSystem()->TryWeatherChange(WeatherToChangeTo); 
			// TODO: Implement base Weather System for communication

			GEngine->AddOnScreenDebugMessage(256, 3.f, FColor::Green, "Weather changed via Timeframe!");
			return;

			break;
		}
		case EChangeCondition::InteractionTriggered:
		{
			InteractionTarget->OnInteractionDelegate.BindUFunction(this, FName("OnInteract"));
			return;
			break;
		}
		default:
			GEngine->AddOnScreenDebugMessage(256, 3.f, FColor::Green, "Weather changed via default!");
			break;
	}
}

void AWeatherVolume::OnExitField(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyindex)
{
	if (!OtherActor->ActorHasTag("Player"))
		return;

	InteractionTarget->OnInteractionDelegate.Unbind();
}

void AWeatherVolume::BeginPlay()
{
	TriggerField->OnComponentBeginOverlap.AddDynamic(this, &AWeatherVolume::OnEnterField);
	TriggerField->OnComponentEndOverlap.AddDynamic(this, &AWeatherVolume::OnExitField);
}

void AWeatherVolume::OnInteract()
{
	GEngine->AddOnScreenDebugMessage(256, 3.f, FColor::Green, "Weather changed via Interaction!");
	InteractionTarget->OnInteractionDelegate.Unbind();
}
