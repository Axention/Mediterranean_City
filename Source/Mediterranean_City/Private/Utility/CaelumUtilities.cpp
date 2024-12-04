// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CaelumUtilities.h"
#include "Core/CaelumGamemode.h"

#include "Kismet/GameplayStatics.h"

APhysicalSky* UCaelumUtilities::GetTimeOfDaySystem(UObject* WorldContextObject)
{
   return Cast<ACaelumGamemode>(UGameplayStatics::GetGameMode(WorldContextObject))->ToDSystem;
}
