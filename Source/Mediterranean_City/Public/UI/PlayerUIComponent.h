// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerUIComponent.generated.h"


UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class UPlayerUIComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  UPlayerUIComponent();

  UFUNCTION(BlueprintImplementableEvent)
  void ToggleBenchUI();

  UFUNCTION(BlueprintImplementableEvent)
  void UpdateBenchUI(int newTime);
};
