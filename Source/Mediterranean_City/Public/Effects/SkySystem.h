// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Utility/AstroMath.h"

#include "SkySystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSkySystem, Log, All);

class UPostProcessComponent;
class USkyLightComponent;
class USkyAtmosphereComponent;
class UVolumetricCloudComponent;
class UDirectionalLightComponent;
class UExponentialHeightFogComponent;
class UCurveFloat;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimeChangedDelegate, float);


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
};


UCLASS()
class ASkySystem : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkySystem();

	UFUNCTION()
	void SkipTime(float newTime);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurrentTime() const { return SimData.LocalTime; }

protected:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void Tick(float DeltaSeconds) override;

	void CalculatePlanetaryPositions();

	void UpdateLighting();

	void ChangeTime(float Amount);

private:
	void UpdateSimulationTimeDate(float newTime);


public:
	FOnTimeChangedDelegate OnTimeChanged;

protected:
	UPROPERTY(EditAnywhere, Category = "Simulation")
	FLocationInfo SimData;

	/* 1x -> Realtime, 60x -> 1s = 1min */
	UPROPERTY(EditAnywhere, Category = "Simulation", meta = (ClampMin = "0.0"))
	float SimulationSpeed;

	UPROPERTY(EditAnywhere, Category = "Lighting", meta = (ClampMin = "0.0"))
	float SunLux;

	UPROPERTY(EditDefaultsOnly, Category = "Lighting", AdvancedDisplay)
	TObjectPtr<UCurveFloat> SunIntensityFalloff;

	UPROPERTY(EditDefaultsOnly, Category = "Lighting")
	float FastForwardTimeMultiplier;

	// ----- Components Begin
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPostProcessComponent> PostProcess;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkyLightComponent> SkyLight;

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
	// ----- Components End


private:
	float TimeskipRemaining;

	FAzimuthialCoords SunCoords;

	FAzimuthialCoords MoonCoords;
};
