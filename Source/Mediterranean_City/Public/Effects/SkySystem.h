// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Utility/AstroMath.h"
#include "Components/TimelineComponent.h"

#include "SkySystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSkySystem, Log, All);

class UPostProcessComponent;
class UCaelumSkyLightComponent;
class USkyAtmosphereComponent;
class UVolumetricCloudComponent;
class UDirectionalLightComponent;
class UExponentialHeightFogComponent;
class UCurveFloat;
class UWeatherPreset;
class UNiagaraComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimeChangedDelegate, float);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSunsetDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSunriseDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangingWeather, float, BlendAlpha, bool, Raining);


USTRUCT()
struct FLocationInfo
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, meta = (ClampMin = "-90.0", ClampMax = "90.0"))
  float Latitude;
  UPROPERTY(EditAnywhere, meta = (ClampMin = "-180.0", ClampMax = "180.0"))
  float Longitude;

  UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "9999"))
  int Year;
  UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "12"))
  int Month;
  UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "31"))
  int Day;

  UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "23.999"))
  float LocalTime;
  UPROPERTY(EditAnywhere, meta = (ClampMin = "-12", ClampMax = "12"))
  int UTC_TimeZone;

  UPROPERTY(EditAnywhere, meta = (ClampMin = "-180.0", ClampMax = "180.0"))
  float NorthOffset;

  FLocationInfo()
  {
    Latitude = 36.39f;
    Longitude = 25.46f;

    Year = 2024;
    Month = 6;
    Day = 18;

    LocalTime = 10.f;
    UTC_TimeZone = 3;

    NorthOffset = 0.f;
  }

  int GetDayOfYear()
  {
    return FDateTime(Year, Month, Day).GetDayOfYear();
  }
};


UCLASS()
class ASkySystem : public AActor
{
  GENERATED_BODY()

public:
  ASkySystem();

  UFUNCTION()
  void SkipTime(float newTime);

  UFUNCTION(BlueprintCallable)
  void ChangeWeather(UWeatherPreset* newWeather);

  UFUNCTION(BlueprintCallable, BlueprintPure)
  float GetCurrentTime() const { return SimData.LocalTime; }

  UFUNCTION(BlueprintCallable, BlueprintPure)
  float GetSunAltitude() const { return SunCoords.altitude; }

  bool IsSkipOnCooldown() const { return TimeskipRemaining > 0.f; }

  bool IsWeatherReady() const { return (bBlendingWeather == 0) && (InternalRandomTickTotalCooldown <= (RandomTickCooldown - 30.f)); }

protected:
  virtual void BeginPlay() override;

  virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

  virtual void Tick(float DeltaSeconds) override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  UFUNCTION(BlueprintImplementableEvent)
  void Sunrise();

  UFUNCTION(BlueprintImplementableEvent)
  void Sunset();

private:
  void TickTime(float DeltaSeconds);

  void ChangeTime(float Amount);

  void UpdateSimulationTimeDate(float newTime);

  void CalculatePlanetaryPositions();

  void UpdateLighting();

  void TickWeather(float deltaSeconds);

  void RndWeatherEvent();

  void UpdateWeatherValues();

  UFUNCTION()
  void BlendWeather(float Value);

  UFUNCTION()
  void OnWeatherBlendFin();

  // End Functions ---
  //
  // Begin Members ---

public:
  FOnTimeChangedDelegate OnTimeChanged;

  UPROPERTY(BlueprintAssignable)
  FOnSunsetDelegate OnSunset;

  UPROPERTY(BlueprintAssignable)
  FOnSunriseDelegate OnSunrise;

  UPROPERTY(BlueprintAssignable)
  FOnChangingWeather OnChangingWeather;

protected:
  /*Container for all time relevant information.*/
  UPROPERTY(EditAnywhere, Category = "Simulation")
  FLocationInfo SimData;

  /*Realtime-Speed Multiplier for the internal Time-Simulation.
  1x >> Realtime
  60x >> 1sec = 1min
  etc...*/
  UPROPERTY(EditAnywhere, Category = "Simulation", meta = (ClampMin = "0.0"))
  float SimulationSpeed;

  /*The Intensity of SunIntensityFalloff at 1.*/
  UPROPERTY(EditDefaultsOnly, Category = "Lighting", meta = (ClampMin = "0.0"))
  float SunLux;

  /*Normalized Function for modulating sun intensity based on Sun altitude.*/
  UPROPERTY(EditDefaultsOnly, Category = "Lighting", AdvancedDisplay)
  TObjectPtr<UCurveFloat> SunIntensityFalloff;

  /*Interval in Seconds: How often a random weather event can happen. (Only acts as a "Tick". Event is decided by chance.)*/
  UPROPERTY(EditAnywhere, Category = "Weather")
  float RandomTickCooldown;

  /*Chance to switch weather after RandomTickCooldown-Seconds have passed*/
  UPROPERTY(EditAnywhere, Category = "Weather")
  float RandomWeatherChance;

  /*Holds information about the current Weather-State*/
  UPROPERTY(EditAnywhere, Category = "Weather")
  TObjectPtr<UWeatherPreset> CurrentWeather;

  /*List of Weather Presets the Random Weather Event can use.*/
  UPROPERTY(EditDefaultsOnly, Category = "Weather", BlueprintReadOnly)
  TArray<UWeatherPreset*> DefaultWeatherPresets;

  /*Fallback for a Function used to blend between Weather-States. (X: time, length - Y: Blend Alpha)*/
  UPROPERTY(EditDefaultsOnly, Category = "Weather", AdvancedDisplay)
  TObjectPtr<UCurveFloat> DefaultWeatherBlend;

  /*MPC to get the world reference for. Used to write information to clouds and puddles.*/
  UPROPERTY(EditDefaultsOnly, Category = "Weather", AdvancedDisplay)
  UMaterialParameterCollection* WeatherParameterCollection;

  UPROPERTY(EditDefaultsOnly, Category = "Weather")
  float PuddleFadeTime;

  /*Normalized Easing Function for Timeskips (eg. smooth blend-in & out)*/
  UPROPERTY(EditDefaultsOnly, Category = "Other")
  TObjectPtr<UCurveFloat> TimeSkipEase;

  /*Defines how long the time-skipping interaction takes.*/
  UPROPERTY(EditAnywhere, Category = "Other")
  float TimeSkipDuration;

  // ----- Components Begin
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
  TObjectPtr<UPostProcessComponent> PostProcess;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UCaelumSkyLightComponent> SkyLight;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UDirectionalLightComponent> Sun;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UDirectionalLightComponent> Moon;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<USkyAtmosphereComponent> Atmosphere;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UVolumetricCloudComponent> VolumetricClouds;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UExponentialHeightFogComponent> Fog;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UStaticMeshComponent> SkySphere;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<UNiagaraComponent> RainParticles;
  // ----- Components End

private:
  float TimeskipRemaining;
  float InternalTimeskipSpeed;
  float InternalTotalTimeskip;
  float InternalTimeMultiplier;
  FTimerHandle TimeSkipHandle;

  FAzimuthialCoords SunCoords;
  FAzimuthialCoords MoonCoords;

  float InternalRandomTickTotalCooldown;
  FTimeline WeatherTimeline;
  UWeatherPreset* PreviousWeather;
  UMaterialParameterCollectionInstance* WeatherParams;
  uint8 bBlendingWeather : 1;
  float PuddleAmountInternalSnap;
  float InternalWetnessSnap;

  FTimerHandle PuddleHandle;
  void ResetPuddles();
};
