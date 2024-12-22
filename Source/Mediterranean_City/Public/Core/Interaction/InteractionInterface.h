// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "InteractionInterface.generated.h"

class AAtmoCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
  GENERATED_BODY()
};

/**
 *
 */
class MEDITERRANEAN_CITY_API IInteractionInterface
{
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintNativeEvent)
  void Interact(AAtmoCharacter* Character);

  UFUNCTION(BlueprintNativeEvent)
  void SetHighlight(bool newState);
};
