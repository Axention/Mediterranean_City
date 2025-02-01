// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CaelumGameUserSettings.generated.h"


UCLASS()
class MEDITERRANEAN_CITY_API UCaelumGameUserSettings : public UGameUserSettings
{
  GENERATED_BODY()

public:

  UFUNCTION(BlueprintCallable)
  static UCaelumGameUserSettings* GetGameSettings();

  UFUNCTION(BlueprintCallable)
  void SetMusicVolume(float Volume);

  UFUNCTION(BlueprintPure)
  float GetMusicVolume() const;

  UFUNCTION(BlueprintCallable)
  void SetSFXVolume(float Volume);

  UFUNCTION(BlueprintPure)
  float GetSFXVolume() const;

protected:

  UPROPERTY(Config)
  float MusicVolume;

  UPROPERTY(Config)
  float SFXVolume;
};
