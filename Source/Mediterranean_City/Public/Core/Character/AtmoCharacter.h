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

class UInteractionComponent;
class UActionAnimComponent;

class UMotionWarpingComponent;

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	MS_Idle,
	MS_Walking,
	MS_Sitting
}; 

UENUM(BlueprintType)
enum class EInteractionState : uint8
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

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure)
	EMoveState GetMoveState() const { return MoveState; }

	UFUNCTION(BlueprintCallable)
	void SetMoveState(EMoveState newState) { MoveState = newState; }

	UFUNCTION(BlueprintPure)
	EInteractionState GetInteractionState() const { return InteractionState; }

	UActionAnimComponent* GetActionAnimComponent() const { return ActionAnimComponent; }

	UMotionWarpingComponent* GetMotionWarper() const { return MotionWarper; }

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
	void Interact(const FInputActionValue& Value);

	void ClampRotation();

protected:
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* CamBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInteractionComponent* IAComponent;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, const UInputAction*> Actions;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* AtmoBaseMappingContext;

	UPROPERTY(BlueprintGetter = GetMoveState, BlueprintSetter = SetMoveState)
	EMoveState MoveState;

	UPROPERTY(BlueprintGetter = GetInteractionState)
	EInteractionState InteractionState;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UActionAnimComponent> ActionAnimComponent;

	UPROPERTY()
	TObjectPtr<UMotionWarpingComponent> MotionWarper;
};
