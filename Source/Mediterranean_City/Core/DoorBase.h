// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interaction/InteractionInterface.h"

#include "DoorBase.generated.h"

UCLASS()
class ADoorBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
	ADoorBase();

protected:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Door;

};
