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


public:
  UPROPERTY()
  FOnSkipTimeDelegate OnTimeSkipInteraction;

protected:
  UPROPERTY()
  TObjectPtr<USceneComponent> DefaultRoot;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<USceneComponent> WarpTarget;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UStaticMeshComponent> Mesh;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UBoxComponent> InteractionZone;

  UPROPERTY(EditAnywhere)
  float TimeToSkipTo;

private:
  FTimerHandle Timer;
};