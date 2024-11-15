// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorBase.h"

// Sets default values
ADoorBase::ADoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

   Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
   RootComponent = Root;


   Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
   Door->SetupAttachment(RootComponent);

}

