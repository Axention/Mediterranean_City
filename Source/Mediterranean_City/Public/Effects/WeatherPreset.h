// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Curves/CurveFloat.h"

#include "WeatherPreset.generated.h"


UCLASS(BlueprintType)
class UWeatherPreset : public UDataAsset
{
  GENERATED_BODY()

public:
  /*Curve controlling how this weather preset gets blended in. Also controls how long the transition takes.*/
  UPROPERTY(EditAnywhere)
  TObjectPtr<UCurveFloat> BlendCurve;

  /*Control the Amount of Sky covered by Clouds. (0: No Clouds - 1: Fully Covered)*/
  UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
  float CloudCoverage;

  /*Globally Lerps Cloud Type toward cumulonimbus clouds.*/
  UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
  float Percipitation;

  /*Enables Rain Effects & Puddles*/
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  uint8 bHasRain : 1;

  UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
  float FogDensity;

  UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "10.0"))
  float FogExtinction;

  UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
  float MieAbsorptionScale;

  /*Maximum View Samples the volumetric clouds can render for this weather. (Lower samples dramatically increase performance at no visual loss when weather is heavily clouded.)*/
  UPROPERTY(EditAnywhere, meta = (ClampMin = "16.0", ClampMax = "256.0"), Category = "Optimizations")
  float MaxSamples{ 256.f };
};
