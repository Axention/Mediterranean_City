// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Structs/WeatherPresets.h"

#include "WeatherVolume.generated.h"

class USphereComponent;

UENUM()
enum class EChangeCondition : uint8
{
	SpecifiedTimeframe,
	ContactOnly
};

UCLASS()
class AWeatherVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeatherVolume();

	UFUNCTION()
	void OnEnterField(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyindex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditInstanceOnly)
	EChangeCondition ChangeCondition;

	UPROPERTY(EditInstanceOnly)
	EWeatherPresets WeatherToChangeTo;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> TriggerField;

	UPROPERTY(EditInstanceOnly, meta = (EditCondition = "ChangeCondition == EChangeCondition::SpecifiedTimeframe", EditConditionHides, Tooltip="Define the timeframe during which a weather change can be triggered by this volume. Keep between 0 - 24. Inclusive/Exclusive should not be changed!"))
	FFloatRange Timeframe;

};
