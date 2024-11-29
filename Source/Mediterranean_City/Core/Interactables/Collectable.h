// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InteractionInterface.h"

#include "Collectable.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCollectibles, Log, Warning);


class UBoxComponent;
class ADisplayStand;

UCLASS()
class ACollectable : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ACollectable();

	virtual void Interact_Implementation(AAtmoCharacter* Character) override;

protected:

private:


public:

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> InteractionField;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<ADisplayStand> DisplayStand;

};
