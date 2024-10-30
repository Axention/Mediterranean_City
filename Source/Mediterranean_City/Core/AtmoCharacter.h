// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AtmoCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UENUM(BlueprintType)
enum EMoveState
{
	MS_Idle,
	MS_Walking,
	MS_Sitting
}; 

UENUM(BlueprintType)
enum EInteractionState
{
	IA_Idle,
	IA_Busy
};


UCLASS()
class AAtmoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAtmoCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	EMoveState GetMoveState() const { return MoveState; }

	UFUNCTION(BlueprintCallable)
	void SetMoveState(EMoveState newState) { MoveState = newState; }

	UFUNCTION(BlueprintPure)
	EInteractionState GetInteractionState() const { return InteractionState; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnRun();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStopRun();

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* CamBoom;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, const UInputAction*> Actions;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* AtmoBaseMappingContext;

	UPROPERTY(BlueprintGetter = GetMoveState, BlueprintSetter = SetMoveState)
	TEnumAsByte<EMoveState> MoveState;

	UPROPERTY(BlueprintGetter = GetInteractionState)
	TEnumAsByte<EInteractionState> InteractionState;
};
