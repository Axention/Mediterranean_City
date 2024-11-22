// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeskipBench.h"

#include "Kismet/GameplayStatics.h"

#include "AtmoCharacter.h"
#include "ActionAnimComponent.h"
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

void ATimeskipBench::Interact_Implementation(AAtmoCharacter* Character)
{
	FTimerManagerTimerParameters timerParams = { .bLoop = false, .bMaxOncePerFrame = false, .FirstDelay = -1.f };

	switch (Character->GetMoveState())
	{
	case MS_Sitting:
		if (GetWorld()->GetTimerManager().TimerExists(Timer))
			return;

		OnTimeSkipInteraction.ExecuteIfBound(TimeAmountToSkip);

		GetWorld()->GetTimerManager().SetTimer(Timer, TimeAmountToSkip + 1.f, timerParams);
		break;
	default:
		Character->GetActionAnimComponent()->SitDown();
		break;
	}
}

void ATimeskipBench::BeginPlay()
{
	Super::BeginPlay();
	
	OnTimeSkipInteraction.BindUFunction(UGameplayStatics::GetActorOfClass(GetWorld(), APhysicalSky::StaticClass()), FName("SkipTime"));
}


