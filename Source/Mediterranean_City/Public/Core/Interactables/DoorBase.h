// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

#include "Core/Interaction/InteractableBase.h"

#include "DoorBase.generated.h"

UENUM(BlueprintType)
enum class EDoorlock : uint8
{
  Unlocked,
  TimeLocked UMETA(ToolTip = "Will automatically open during given timeframe."),
  LockedOneSided UMETA(DisplayName = "one-sided Lock"),
  TimelockedHidden UMETA(DisplayName = "Secret/Hidden & Timelocked"),
  KeyLocked UMETA(Hidden)
};


UCLASS()
class ADoorBase : public AActor, public IInteractionInterface
{
  GENERATED_BODY()

  ADoorBase();

protected:
  virtual void BeginPlay() override;

  virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

  UFUNCTION(BlueprintImplementableEvent)
  void TimeChanged(float newTime);

protected:

  UPROPERTY(EditDefaultsOnly)
  UStaticMeshComponent* Root;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
  UStaticMeshComponent* Door;

  UPROPERTY(EditDefaultsOnly)
  UBoxComponent* TraceFallback;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  EDoorlock DoorlockState;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "DoorlockState == EDoorlock::LockedOneSided", EditConditionHides))
  bool bInvertLockedSide;

#if WITH_EDITOR
  UArrowComponent* UnlockedSideMarker;
#endif // WITH_EDITOR

};
