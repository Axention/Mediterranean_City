// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "Core/Interaction/InteractableBase.h"

#include "TimeskipBench.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSkipTimeDelegate, float, Amount);

UCLASS()
class ATimeskipBench : public AInteractable
{
  GENERATED_BODY()

public:
  ATimeskipBench();

  virtual void Interact_Implementation(AAtmoCharacter* Character) override;

  virtual void SetHighlight_Implementation(bool newState) override;

protected:
  virtual void BeginPlay() override;

  // End Functions ---
  //
  // Begin Functions ---

public:
  /*Delegate called when sitting and interacting again.*/
  FOnSkipTimeDelegate OnTimeSkipInteraction;

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

};