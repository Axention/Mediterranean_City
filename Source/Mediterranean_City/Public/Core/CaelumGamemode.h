// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "CaelumGamemode.generated.h"

class ASkySystem;

/**
 *
 */
UCLASS()
class MEDITERRANEAN_CITY_API ACaelumGamemode : public AGameModeBase
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  ASkySystem* ToDSystem = nullptr;

  virtual void StartPlay() override;
};
