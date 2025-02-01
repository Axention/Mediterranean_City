// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "InteractionComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UInteractionComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  UInteractionComponent();

  void TryInteraction(FVector CameraPosition, FVector CameraForward);

  void SeekInteractible(FVector CamPos, FVector CamFwd);

  void ResetSeeking();

  // End Functions ---
  //
  // Begin Members ---

protected:
  /*How far to trace for interactable objects*/
  UPROPERTY(EditDefaultsOnly)
  float MaxInteractionDistance;

  /*Filter for Object Types to search for. (Defaults to "Interactables")*/
  UPROPERTY(EditAnywhere)
  TArray<TEnumAsByte<EObjectTypeQuery>> SearchObjectTypes;

private:
  TObjectPtr<AActor> lastHitActor;

  bool bFoundInteractable{ false };
};
