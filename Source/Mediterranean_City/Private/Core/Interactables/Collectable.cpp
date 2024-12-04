// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Interactables/Collectable.h"

#include "Core/Interactables/DisplayStand.h"

#include "Kismet/KismetMathLibrary.h"

#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY(LogCollectibles);

ACollectable::ACollectable()
{
	PrimaryActorTick.bCanEverTick = false;

	DisplayStand = nullptr;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Default Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item"));
	InteractionField = CreateDefaultSubobject<UBoxComponent>(TEXT("Interactable Field"));

	RootComponent = Root;
	Mesh->SetupAttachment(RootComponent);
	InteractionField->SetupAttachment(RootComponent);

	Root->Mobility = EComponentMobility::Static;
	Mesh->Mobility = EComponentMobility::Static;
	InteractionField->Mobility = EComponentMobility::Static;

}

void ACollectable::Interact_Implementation(AAtmoCharacter* Character)
{
	if (!DisplayStand)
		UE_LOG(LogCollectibles, Error, TEXT("No Displaystand referenced for %s!"), *GetDebugName(this));

	FVector newLoc = DisplayStand->GetActorLocation() + DisplayStand->GetTransformModifier().GetLocation();
	FRotator newRot = DisplayStand->GetActorRotation() + DisplayStand->GetTransformModifier().GetRotation().Rotator();

	Mesh->SetWorldLocationAndRotation(newLoc, newRot.Quaternion());

	InteractionField->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACollectable::SetHighlight_Implementation(bool newState)
{
	Mesh->SetRenderCustomDepth(newState);
}


