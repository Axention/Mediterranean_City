// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "InteractionInterface.h"

#include "AtmoCharacter.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxInteractionDistance = 150.f;

	SearchObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);
}

void UInteractionComponent::TryInteraction(FVector CameraPosition, FVector CameraForward)
{
	FHitResult hit;

	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), CameraPosition, CameraPosition + CameraForward * MaxInteractionDistance, 5.f, SearchObjectTypes, false,
		TArray<AActor*>::TArray(), EDrawDebugTrace::None, hit, true))
	{
		if (hit.GetActor()->Implements<UInteractionInterface>())
		{
			IInteractionInterface::Execute_Interact(hit.GetActor(), Cast<AAtmoCharacter>(GetOwner()));
		}
	}
}

