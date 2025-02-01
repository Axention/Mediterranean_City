// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CaelumGameInstance.generated.h"

class UUserWidget;

UCLASS()
class MEDITERRANEAN_CITY_API UCaelumGameInstance : public UGameInstance
{
  GENERATED_BODY()

public:
  virtual void Init() override;

  UFUNCTION()
  virtual void BeginLoadingScreen(const FString& MapName);

  UFUNCTION()
  virtual void EndLoadingScreen(UWorld* InLoadedWorld);

protected:
  /*Widget Class to construct the Loading Screen SWidget from.*/
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Screen")
  TSubclassOf<UUserWidget> LoadingScreenWidget;

  /*Minimum time the loading screen is shown.*/
  UPROPERTY(EditAnywhere, Category = "Loading Screen")
  float MinimumLoadTime{ 1.f };


private:
  TSharedPtr<SWidget> LoadingSWidgetPtr;
};
