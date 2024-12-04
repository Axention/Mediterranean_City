// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

#include "Core/Interaction/InteractionInterface.h"

#include "DoorBase.generated.h"

UENUM(BlueprintType)
enum EDoorlock
{
	Unlocked,
	TimeLocked,
	LockedOneSided,
	KeyLocked
};


UCLASS()
class ADoorBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
	ADoorBase();

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

protected:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Door;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* TraceFallback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EDoorlock> DoorlockState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "DoorlockState == EDoorlock::LockedOneSided", EditConditionHides))
	bool bInvertLockedSide;

	UArrowComponent* UnlockedSideMarker;




};
