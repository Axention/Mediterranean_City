// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DisplayStand.generated.h"

UCLASS()
class ADisplayStand : public AActor
{
  GENERATED_BODY()

public:
  ADisplayStand();

  FTransform GetTransformModifier() const { return TransformModifier; }

protected:
  UPROPERTY(EditAnywhere, meta = (MakeEditWidget))
  FTransform TransformModifier;

};
