// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Interactables/TimeskipBench.h"

#include "Utility/CaelumUtilities.h"
#include "Core/Character/AtmoCharacter.h"
#include "Core/Character/ActionAnimComponent.h"
#include "Effects/SkySystem.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "MotionWarpingComponent.h"


ATimeskipBench::ATimeskipBench()
{
	PrimaryActorTick.bCanEverTick = false;

	TimeToSkipTo = 12.f;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = DefaultRoot;
	DefaultRoot->SetMobility(EComponentMobility::Static);

	InteractionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Interactable"));
	InteractionZone->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	WarpTarget = CreateDefaultSubobject<USceneComponent>(TEXT("Target"));
	WarpTarget->SetupAttachment(RootComponent);

}

void ATimeskipBench::Interact_Implementation(AAtmoCharacter* Character)
{
	FTimerManagerTimerParameters timerParams = { .bLoop = false, .bMaxOncePerFrame = false, .FirstDelay = -1.f };

	switch (Character->GetMoveState())
	{
	case EMoveState::MS_Sitting:
		if (GetWorld()->GetTimerManager().TimerExists(Timer))
			return;

		OnTimeSkipInteraction.ExecuteIfBound(TimeToSkipTo);

		GetWorld()->GetTimerManager().SetTimer(Timer, TimeToSkipTo + 1.f, timerParams);
		break;
	default:
		if (UKismetMathLibrary::Dot_VectorVector(Character->GetActorForwardVector(), GetActorForwardVector()) > -0.25)
			return;
		Character->GetMotionWarper()->AddOrUpdateWarpTargetFromComponent(FName("SeatTarget"), WarpTarget, NAME_None, false);
		Character->GetActionAnimComponent()->SitDown();
		OnInteractionDelegate.ExecuteIfBound();
		break;
	}
}

void ATimeskipBench::SetHighlight_Implementation(bool newState)
{
	Mesh->SetRenderCustomDepth(newState);
}

void ATimeskipBench::BeginPlay()
{
	Super::BeginPlay();
	
	if (UCaelumUtilities::GetTimeOfDaySystem(GetWorld()) == nullptr)
		UE_LOG(LogTemp, Error, TEXT("ToD is nullptr"));

	OnTimeSkipInteraction.BindUFunction(UCaelumUtilities::GetTimeOfDaySystem(GetWorld()), FName("SkipTime"));
}


