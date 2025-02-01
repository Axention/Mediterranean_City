// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionAnimComponent.generated.h"

class AAtmoCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UActionAnimComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  UActionAnimComponent();

  void SitDown();

  void StandUp();

protected:
  virtual void BeginPlay() override;

private:
  UFUNCTION()
  void OnMontageBlendOut(UAnimMontage* Montage, bool interrupted);

  // End Functions ---
  //
  // Begin Members ---

protected:
  UPROPERTY(EditDefaultsOnly, Category = "Montages")
  TObjectPtr<UAnimMontage> SitDownMontage;

  UPROPERTY(EditDefaultsOnly, Category = "Montages")
  TObjectPtr<UAnimMontage> StandUpMontage;

private:
  TObjectPtr<AAtmoCharacter> Player;

};
