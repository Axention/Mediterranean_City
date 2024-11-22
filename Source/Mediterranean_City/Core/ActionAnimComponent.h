// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionAnimComponent.generated.h"

class AAtmoCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEDITERRANEAN_CITY_API UActionAnimComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UActionAnimComponent();

	void SitDown();
	void StandUp();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMontageBlendOut(UAnimMontage* Montage, bool interrupted);

private:

public:

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> SitDownMontage;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> StandUpMontage;

private:
	TObjectPtr<AAtmoCharacter> Player;
		
};
