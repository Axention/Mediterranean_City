// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Curves/CurveFloat.h"

#include "WeatherPreset.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class UWeatherPreset : public UDataAsset
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  TObjectPtr<UCurveFloat> BlendCurve;

  UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
  float CloudCoverage;

  UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
  float Percipitation;

  UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
  float Detail;

  UPROPERTY(EditAnywhere)
  uint8 bHasRain : 1;

  UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
  float FogDensity;

  UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "10.0"))
  float FogExtinction;

  UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
  float MieAbsorptionScale;
};
