// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Interactables/DoorBase.h"

#include "Utility/CaelumUtilities.h"

// Sets default values
ADoorBase::ADoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

   DoorlockState = EDoorlock::Unlocked;
   bInvertLockedSide = false;

   Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
   RootComponent = Root;

   Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
   Door->SetupAttachment(RootComponent);

   TraceFallback = CreateDefaultSubobject<UBoxComponent>(TEXT("Trace Fallback"));
   TraceFallback->SetupAttachment(RootComponent);

#if WITH_EDITOR
   UnlockedSideMarker = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
   if (UnlockedSideMarker)
   {
      UnlockedSideMarker->SetupAttachment(RootComponent);
      UnlockedSideMarker->SetRelativeLocation(FVector(50, 0, 100));
      UnlockedSideMarker->SetRelativeRotation(FRotator(0, 270, 0));
      UnlockedSideMarker->SetVisibility(DoorlockState == EDoorlock::LockedOneSided);
   }
#endif // WITH_EDITOR
}

void ADoorBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
   Super::PostEditChangeProperty(PropertyChangedEvent);

#if WITH_EDITOR
   if (DoorlockState == EDoorlock::LockedOneSided)
   {
      UnlockedSideMarker->SetVisibility(true);
      UnlockedSideMarker->SetRelativeRotation(FRotator(0, bInvertLockedSide ? 90 : 270, 0));
   }
   else
   {
      UnlockedSideMarker->SetVisibility(false);
   }
#endif // WITH_EDITOR
}

void ADoorBase::BeginPlay()
{
   if (DoorlockState == EDoorlock::TimeLocked)
      UCaelumUtilities::GetTimeOfDaySystem(this)->OnTimeChanged.AddUFunction(this, FName("TimeChanged"));
}

