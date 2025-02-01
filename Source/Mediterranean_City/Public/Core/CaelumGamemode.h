// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "CaelumGamemode.generated.h"

class ASkySystem;

UCLASS()
class ACaelumGamemode : public AGameModeBase
{
  GENERATED_BODY()

public:
  UPROPERTY(BlueprintReadWrite)
  ASkySystem* ToDSystem = nullptr;

  virtual void StartPlay() override;
};
