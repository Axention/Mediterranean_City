

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/PostProcessComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/ExponentialHeightFogComponent.h"

#include "PhysicalSky.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPhysicalSky, Log, All)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeskipDelegate, float, newTime);

USTRUCT()
struct FDate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "9999"))
	int Year{ 2000 };
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "12"))
	int Month{ 1 };
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "31"))
	int Day{ 1 };
};


UCLASS(Blueprintable)
class APhysicalSky : public AActor
{
	GENERATED_BODY()
	
public:	
	APhysicalSky();

	bool UpdateSky();

	UFUNCTION(BlueprintPure)
	float GetCurrentTime() const { return localTime; }

public:
	UPROPERTY(BlueprintAssignable)
	FOnTimeskipDelegate OnTimeSkip;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UPostProcessComponent* PostProcess;
	UPROPERTY(EditDefaultsOnly)
	USkyLightComponent* Skylight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDirectionalLightComponent* Sun;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDirectionalLightComponent* Moon;
	UPROPERTY(EditDefaultsOnly)
	USkyAtmosphereComponent* Atmosphere;
	UPROPERTY(EditDefaultsOnly)
	UExponentialHeightFogComponent* Fog;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SkyMesh;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Compass;


	UPROPERTY(EditAnywhere, Category = "Time of Day")
	float Latitude;

	UPROPERTY(EditAnywhere, Category = "Time of Day")
	float Longitude;

	UPROPERTY(EditAnywhere, Category = "Time of Day")
	FDate Date;

	UPROPERTY(EditAnywhere, Category = "Time of Day", meta = (ClampMin = "0.0", ClampMax = "24.0"), BlueprintGetter = GetCurrentTime)
	float localTime;

	UPROPERTY(EditAnywhere, Category = "Time of Day", meta = (ClampMin = "-12", ClampMax = "12"))
	int UTCTimeZone;

	UPROPERTY(EditAnywhere, Category = "Time of Day", meta = (ClampMin = "-180.0", ClampMax = "180.0"))
	float northOffset;



	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

private:
	float elevationAngle;
	float azimuthAngle;

	void UpdateSunPosition();
	void UpdateMoonPosition();
};
