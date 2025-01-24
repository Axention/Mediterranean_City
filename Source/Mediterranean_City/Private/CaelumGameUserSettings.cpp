// Fill out your copyright notice in the Description page of Project Settings.


#include "CaelumGameUserSettings.h"


//UCaelumGameUserSettings::UCaelumGameUserSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
//{
//  MusicVolume = 1.f;
//  SFXVolume = 1.f;
//}

UCaelumGameUserSettings* UCaelumGameUserSettings::GetGameSettings()
{
  return Cast<UCaelumGameUserSettings>(GetGameUserSettings());
}

void UCaelumGameUserSettings::SetMusicVolume(float Volume)
{
  MusicVolume = Volume;
}

float UCaelumGameUserSettings::GetMusicVolume() const
{
  return MusicVolume;
}

void UCaelumGameUserSettings::SetSFXVolume(float Volume)
{
  SFXVolume = Volume;
}

float UCaelumGameUserSettings::GetSFXVolume() const
{
  return SFXVolume;
}

