// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "InteractionComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MEDITERRANEAN_CITY_API UInteractionComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  UInteractionComponent();

  UFUNCTION(BlueprintCallable)
  void TryInteraction(FVector CameraPosition, FVector CameraForward);

  void SeekInteractible(FVector CamPos, FVector CamFwd);

  void ResetSeeking();

protected:
  UPROPERTY(EditDefaultsOnly)
  float MaxInteractionDistance;

  UPROPERTY(EditAnywhere)
  TArray<TEnumAsByte<EObjectTypeQuery>> SearchObjectTypes;

private:
  TObjectPtr<AActor> lastHitActor;

  bool bFoundInteractable{ false };
};
