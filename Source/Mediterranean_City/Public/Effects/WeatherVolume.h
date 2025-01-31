// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Effects/WeatherPreset.h"

#include "WeatherVolume.generated.h"

class USphereComponent;
class ATimeskipBench;
class AInteractable;

UENUM()
enum class EChangeCondition : uint8
{
  SpecifiedTimeframe,
  InteractionTriggered,
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

  UFUNCTION()
  void OnExitField(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyindex);

protected:
  virtual void BeginPlay() override;

  UFUNCTION()
  void OnInteract();

  bool SendWeatherChange();

protected:
  UPROPERTY(EditInstanceOnly)
  EChangeCondition ChangeCondition;

  UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
  UWeatherPreset* WeatherToChangeTo;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<USphereComponent> TriggerField;

  UPROPERTY(EditInstanceOnly, meta = (EditCondition = "ChangeCondition == EChangeCondition::SpecifiedTimeframe", EditConditionHides))
  FFloatRange Timeframe;

  UPROPERTY(EditInstanceOnly, meta = (EditCondition = "ChangeCondition == EChangeCondition::InteractionTriggered", EditConditionHides, MustImplement = "InteractionInterface"))
  TObjectPtr<AInteractable> InteractionTarget;



};
