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

protected:
  virtual void BeginPlay() override;

private:
  UFUNCTION()
  void OnEnterField(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyindex, bool bFromSweep, const FHitResult& SweepResult);

  UFUNCTION()
  void OnExitField(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyindex);

  UFUNCTION()
  void OnInteract();

  bool SendWeatherChange();

  // End Functions ---
  //
  // Begin Members ---

protected:
  /*When can this Volume trigger a Weather change.*/
  UPROPERTY(EditInstanceOnly)
  EChangeCondition ChangeCondition;

  /*What weather to change to if condition is met.*/
  UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
  UWeatherPreset* WeatherToChangeTo;

  /*Zone to consider for interaction. (Only Player has to be in this zone. Targets can be outside.)*/
  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<USphereComponent> TriggerField;

  /*Timeframe during which the weather change can be triggered.*/
  UPROPERTY(EditInstanceOnly, meta = (EditCondition = "ChangeCondition == EChangeCondition::SpecifiedTimeframe", EditConditionHides))
  FFloatRange Timeframe;

  /*The Object which has to be interacted with while inside the zone to trigger the weather change.*/
  UPROPERTY(EditInstanceOnly, meta = (EditCondition = "ChangeCondition == EChangeCondition::InteractionTriggered", EditConditionHides, MustImplement = "/Script/Mediterranean_City.InteractionInterface"))
  TObjectPtr<AInteractable> InteractionTarget;



};
