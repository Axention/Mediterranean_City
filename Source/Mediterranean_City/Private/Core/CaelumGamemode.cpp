// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CaelumGamemode.h"

#include "Effects/SkySystem.h"

#include "Kismet/GameplayStatics.h"

void ACaelumGamemode::StartPlay()
{
   ToDSystem = Cast<ASkySystem>(UGameplayStatics::GetActorOfClass(this, ASkySystem::StaticClass()));

   Super::StartPlay();
}
