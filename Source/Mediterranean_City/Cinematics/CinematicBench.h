// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Interaction/InteractableBase.h"

#include "CinematicBench.generated.h"

class UBoxComponent;
class AAtmoCharacter;

UCLASS()
class ACinematicBench : public AInteractable
{
  GENERATED_BODY()

public:
  ACinematicBench();

  virtual void Interact_Implementation(AAtmoCharacter* Character) override;

  virtual void SetHighlight_Implementation(bool newState) override;

  UFUNCTION()
  void OnFullySitting(UAnimMontage* Montage, bool bInterrupted);

  UFUNCTION(BlueprintImplementableEvent)
  void StartCredits();

protected:
  UPROPERTY()
  TObjectPtr<USceneComponent> DefaultRoot;

  /*The target to move to during the sit-down montage.*/
  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<USceneComponent> WarpTarget;

  /*Static Mesh of this Bench.*/
  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UStaticMeshComponent> Mesh;

  /*Buffer Zone for Interaction traces.*/
  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UBoxComponent> InteractionZone;

private:
  uint8 isSitting : 1;

  AAtmoCharacter* character;
};
