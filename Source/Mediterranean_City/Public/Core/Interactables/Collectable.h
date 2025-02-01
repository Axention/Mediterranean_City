// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Core/Interaction/InteractableBase.h"

#include "Collectable.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCollectibles, Log, Warning);

class UBoxComponent;
class ADisplayStand;

UCLASS()
class ACollectable : public AInteractable
{
  GENERATED_BODY()

public:
  ACollectable();

  virtual void Interact_Implementation(AAtmoCharacter* Character) override;

  virtual void SetHighlight_Implementation(bool newState) override;

  // End Functions ---
  //
  // Begin Members ---

protected:
  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<USceneComponent> Root;

  /*Static Mesh for this Collectable.*/
  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UStaticMeshComponent> Mesh;

  /*Buffer Zone for Interaction Traces. Enables Interaction with the object even if not looking exactly at the meshes collision shape.*/
  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UBoxComponent> InteractionField;

  /*Reference to a DisplayStand Actor. Used as the place to display the collectable after it has been picked up.*/
  UPROPERTY(EditInstanceOnly)
  TObjectPtr<ADisplayStand> DisplayStand;

};
