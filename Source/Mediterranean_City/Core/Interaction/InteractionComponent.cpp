// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "InteractionInterface.h"

#include "AtmoCharacter.h"

#include "Kismet/KismetSystemLibrary.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxInteractionDistance = 150.f;

	SearchObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);
}

void UInteractionComponent::TryInteraction(FVector CameraPosition, FVector CameraForward)
{
	if (!bFoundInteractable)
		return;

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

void UInteractionComponent::SeekInteractible(FVector CamPos, FVector CamFwd)
{
	FHitResult hit;

	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), CamPos, CamPos + CamFwd * MaxInteractionDistance, 5.f, ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>::TArray(), EDrawDebugTrace::None, hit, true))
	{
		if (hit.GetActor() == lastHitActor)
			return;

		if (hit.GetActor()->Implements<UInteractionInterface>())
		{
			IInteractionInterface::Execute_SetHighlight(hit.GetActor(), true);
			bFoundInteractable = true;
		}

		if (lastHitActor)
		{
			if (lastHitActor->Implements<UInteractionInterface>())
			{
				IInteractionInterface::Execute_SetHighlight(lastHitActor, false);
				bFoundInteractable = false;
			}
		}

		lastHitActor = hit.GetActor();
		return;
	}

	ResetSeeking();
}

void UInteractionComponent::ResetSeeking()
{
	if (lastHitActor)
	{
		if (lastHitActor->Implements<UInteractionInterface>())
		{
			IInteractionInterface::Execute_SetHighlight(lastHitActor, false);
			bFoundInteractable = false;
		}

		lastHitActor = nullptr;
	}
}



