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
class UPlayerUIComponent;

class UMotionWarpingComponent;


UENUM(BlueprintType)
enum class EMoveState : uint8
{
  MS_Idle,
  MS_Walking,
  MS_Sitting
};


UCLASS()
class AAtmoCharacter : public ACharacter
{
  GENERATED_BODY()

public:
  AAtmoCharacter();

  UFUNCTION(BlueprintPure)
  EMoveState GetMoveState() const { return MoveState; }

  UFUNCTION(BlueprintCallable)
  void SetMoveState(EMoveState newState) { MoveState = newState; }

  int8 GetTimeToSkipTo() const { return TimeToSkipTo; }

  UActionAnimComponent* GetActionAnimComponent() const { return ActionAnimComponent; }

  UMotionWarpingComponent* GetMotionWarper() const { return MotionWarper; }

  void SitDown(const USceneComponent* Target);

protected:
  virtual void BeginPlay() override;

  virtual void Tick(float DeltaSeconds) override;

  virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

  UFUNCTION(BlueprintImplementableEvent)
  void OnRun();

  UFUNCTION(BlueprintImplementableEvent)
  void OnStopRun();

private:
  void Move(const FInputActionValue& Value);

  void Look(const FInputActionValue& Value);

  void Run(const FInputActionValue& Value);

  void Interact(const FInputActionValue& Value);

  void StandUp(const FInputActionValue& Value);

  void ChangeTimeToSkipTo(const FInputActionValue& Value);

  void ClampRotation();

  // End Functions ---
  //
  // Begin Members ---

protected:
  /*Default Mapping Context for Movement.*/
  UPROPERTY(EditDefaultsOnly, Category = "Input|Base")
  UInputMappingContext* AtmoBaseMappingContext;

  /*Map of Actions referenced by BaseMappingContext. Used to bind C++ Functions to these Actions.*/
  UPROPERTY(EditDefaultsOnly, Category = "Input|Base")
  TMap<FString, const UInputAction*> Actions;

  /*Mapping Context only used when sitting on a bench.*/
  UPROPERTY(EditDefaultsOnly, Category = "Input|Bench")
  TObjectPtr<UInputMappingContext> BenchMappingContext;

  /*Map of Actions referenced by BenchMappingContext. Used to bind C++ Functions to these Actions.*/
  UPROPERTY(EditDefaultsOnly, Category = "Input|Bench")
  TMap<FString, const UInputAction*> BenchActions;

  /*Internal State of Movement. (eg. Idle, Walking, Sitting)*/
  UPROPERTY(BlueprintGetter = GetMoveState, BlueprintSetter = SetMoveState)
  EMoveState MoveState;

  /*Holds the time the player wants to skip to.*/
  UPROPERTY(BlueprintReadOnly)
  int TimeToSkipTo;

  // Variables End ---
  //
  // Components Begin ---

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UCameraComponent> Camera;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<USpringArmComponent> CamBoom;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UActionAnimComponent> ActionAnimComponent;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UPlayerUIComponent> UIComponent;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TObjectPtr<UInteractionComponent> IAComponent;

  UPROPERTY()
  TObjectPtr<UMotionWarpingComponent> MotionWarper;

  // Components End ---
};
