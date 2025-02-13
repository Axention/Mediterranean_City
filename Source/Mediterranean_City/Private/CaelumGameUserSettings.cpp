// Fill out your copyright notice in the Description page of Project Settings.


#include "CaelumGameUserSettings.h"


UCaelumGameUserSettings::UCaelumGameUserSettings()
{
  MusicVolume = 0.5;
  SFXVolume = 0.5;
}

UCaelumGameUserSettings* UCaelumGameUserSettings::GetGameSettings()
{
  return CastChecked<UCaelumGameUserSettings>(GetGameUserSettings());
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

