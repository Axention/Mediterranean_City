// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeskipBench.h"

#include "Kismet/GameplayStatics.h"

#include "PhysicalSky.h"


ATimeskipBench::ATimeskipBench()
{
	PrimaryActorTick.bCanEverTick = false;

	TimeAmountToSkip = 3.f;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = DefaultRoot;
	DefaultRoot->SetMobility(EComponentMobility::Static);

	InteractionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Interactable"));
	InteractionZone->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

}

void ATimeskipBench::Interact_Implementation()
{
	if (!GetWorld()->GetTimerManager().TimerExists(Timer))
	{
		OnTimeSkipInteraction.ExecuteIfBound(TimeAmountToSkip);

		FTimerManagerTimerParameters timerParams = { .bLoop = false, .bMaxOncePerFrame = false, .FirstDelay = -1.f };

		GetWorld()->GetTimerManager().SetTimer(Timer, TimeAmountToSkip + 1.f, timerParams);
	}	
}

void ATimeskipBench::BeginPlay()
{
	Super::BeginPlay();
	
	OnTimeSkipInteraction.BindUFunction(UGameplayStatics::GetActorOfClass(GetWorld(), APhysicalSky::StaticClass()), FName("SkipTime"));
}


