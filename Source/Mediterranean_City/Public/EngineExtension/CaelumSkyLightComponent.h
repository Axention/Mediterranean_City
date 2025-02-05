// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkyLightComponent.h"
#include "CaelumSkyLightComponent.generated.h"

class UCurveFloat;
class UCurveLinearColor;

UCLASS()
class UCaelumSkyLightComponent : public USkyLightComponent
{
  GENERATED_BODY()

  UCaelumSkyLightComponent(const FObjectInitializer& ObjectInitializer);

public:
  void UpdateLighting(float SunAltitude);

protected:
  UPROPERTY(EditAnywhere, Category = "Non-Lumen Adjustments")
  TObjectPtr<UCurveFloat> IntensityCurve;

  UPROPERTY(EditAnywhere, Category = "Non-Lumen Adjustments")
  TObjectPtr<UCurveLinearColor> ColorCurve;

  UPROPERTY(EditAnywhere, Category = "Lumen")
  float LumenIntensity;

  UPROPERTY(EditAnywhere, Category = "Lumen")
  FColor LumenColor;
};
