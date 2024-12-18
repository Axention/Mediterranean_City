// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/SkySystem.h"

#include "Components/PostProcessComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/VolumetricCloudComponent.h"

#include "Curves/CurveFloat.h"

#include "Kismet/KismetMathLibrary.h"

ASkySystem::ASkySystem()
{
	PrimaryActorTick.bCanEverTick = true;

	SimData = FLocationInfo();
	SimulationSpeed = 1.f;

	SunCoords = FAzimuthialCoords();
	MoonCoords = FAzimuthialCoords();

	SunLux = 120000.f;
	FastForwardTimeMultiplier = 3.f;
	TimeskipRemaining = -1.f;

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("Global PostProcess"));
	RootComponent = PostProcess;

	Sun = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Sun"));
	Sun->SetupAttachment(RootComponent);
	Sun->ForwardShadingPriority = 2;
	Sun->AtmosphereSunLightIndex = 0;

	Moon = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Moon"));
	Moon->SetupAttachment(RootComponent);
	Moon->ForwardShadingPriority = 1;
	Moon->AtmosphereSunLightIndex = 1;

	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("Skylight"));
	SkyLight->SetupAttachment(RootComponent);

	VolumetricClouds = CreateDefaultSubobject<UVolumetricCloudComponent>(TEXT("Clouds"));
	VolumetricClouds->SetupAttachment(RootComponent);

	Fog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("Fog"));
	Fog->SetupAttachment(RootComponent);

	Atmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("Atmosphere"));
	Atmosphere->SetupAttachment(RootComponent);

	SkySphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkySphere"));
	SkySphere->SetupAttachment(RootComponent);

}

void ASkySystem::SkipTime(float newTime)
{
	TimeskipRemaining = (newTime <= SimData.LocalTime) ? (24.f - (SimData.LocalTime - newTime)) : (newTime - SimData.LocalTime);
}

void ASkySystem::CalculatePlanetaryPositions()
{
	using namespace Astro;
	// implementation based on https://stjarnhimlen.se/comp/ppcomp.html

	float UT = SimData.LocalTime - SimData.UTC_TimeZone;

	float d = 367 * SimData.Year - 7 * (SimData.Year + (SimData.Month + 9) / 12) / 4 - 3 * ((SimData.Year + (SimData.Month - 9) / 7) / 100 + 1) / 4 + 275 * SimData.Month / 9 + SimData.Day - 730515;
	d = d + UT / 24.0;

	float ecl = 23.4393 - 0.0000003563 * d;

	// ----- Begin Sun 
	const float NS = 0.f;
	const float iS = 0.f;
	const float wS = dWrap(282.9404 + 0.0000470935 * d);
	const float aS = 1.f;
	const float eS = 0.016709 - 0.000000001151 * d;
	const float MS = dWrap(356.0470 + 0.9856002585 * d);

	float ES = MS + eS * (180.f / UE_PI) * dsin(MS) * (1.0 + eS * dcos(MS));

	float xvS = dcos(ES) - eS;
	float yvS = sqrtf(1.f - eS * eS) * dsin(ES);

	float vS = RadToDeg(atan2f(yvS, xvS));
	float rS = sqrtf(xvS * xvS + yvS * yvS);

	float lonsun = vS + wS;

	float xS = rS * dcos(lonsun);
	float yS = rS * dsin(lonsun);

	float xeS = xS;
	float yeS = yS * dcos(ecl);
	float zeS = yS * dsin(ecl);

	float RAS = dWrap(RadToDeg(atan2f(yeS, xeS)));
	float DecS = dWrap(RadToDeg(atan2f(zeS, sqrtf(xeS * xeS + yeS * yeS))));

	float Ls = dWrap(MS + wS);
	float GMST0 = dWrap(Ls + 180.f);
	float GMST = dWrap(GMST0 + dWrap(UT * 15.f));
	float LST = dWrap(GMST + SimData.Longitude);

	float HAS = dWrap(LST - RAS);

	SunCoords = GetAzimuthialCoords(HAS, DecS, SimData.Latitude, SimData.Longitude);
	// ----- End Sun 


	// ----- Begin Moon 
	const float NM = dWrap(125.1228 - 0.0529538083 * d);
	const float iM = 5.1454;
	const float wM = dWrap(318.0634 + 0.1643573223 * d);
	const float aM = 60.2666;
	const float eM = 0.054900;
	const float MM = dWrap(115.3654 + 13.0649929509 * d);

	float E = MM + RadToDeg(eM) * dsin(MM) * (1.0 + eM * dcos(MM));

	float xvM = aM * (dcos(E) - eM);
	float yvM = aM * (sqrtf(1.0 - eM * eM) * dsin(E));

	float vM = RadToDeg(atan2f(yvM, xvM));
	float rM = sqrtf(xvM * xvM + yvM * yvM);

	float xhM = rM * (dcos(NM) * dcos(vM + wM) - dsin(NM) * dsin(vM + wM) * dcos(iM));
	float yhM = rM * (dsin(NM) * dcos(vM + wM) + dcos(NM) * dsin(vM + wM) * dcos(iM));
	float zhM = rM * (dsin(vM + wM) * dsin(iM));

	// perturbation correction
	float lonecl = RadToDeg(atan2f(yhM, xhM));
	float latecl = RadToDeg(atan2f(zhM, sqrtf(xhM * xhM + yhM * yhM)));

	float LM = dWrap(MM + wM + NM);
	float D = dWrap(LM - Ls);
	float F = dWrap(LM - NM);
	float corrlon = -1.274f * dsin(MM - 2 * D) + 0.658 * dsin(2 * D) - 0.186 * dsin(MS);
	float corrlat = -0.173 * dsin(F - 2 * D);

	lonecl = dWrap(lonecl + corrlon);
	latecl = latecl + corrlat;

	xhM = dcos(lonecl) * dcos(latecl);
	yhM = dsin(lonecl) * dcos(latecl);
	zhM = dsin(latecl);
	// corrections end

	float xeM = xhM;
	float yeM = yhM * dcos(ecl) - zhM * dsin(ecl);
	float zeM = yhM * dsin(ecl) + zhM * dcos(ecl);

	float RAM = dWrap(RadToDeg(atan2f(yeM, xeM)));
	float DecM = RadToDeg(atan2f(zeM, sqrtf(xeM * xeM + yeM * yeM)));

	float HAM = dWrap(LST - RAM);

	MoonCoords = GetAzimuthialCoords(HAM, DecM, SimData.Latitude, SimData.Longitude);
	// ----- End Moon
}

void ASkySystem::UpdateLighting()
{
	Sun->SetRelativeRotation(FRotator(-SunCoords.altitude, SunCoords.azimuth, 0.0));
	Moon->SetRelativeRotation(FRotator(-MoonCoords.altitude, MoonCoords.azimuth, 0.0));

	SkySphere->SetRelativeRotation(FRotator(SimData.Latitude, 180.0 + SimData.NorthOffset, SimData.LocalTime * 15.f));

	const float SunNormalizedTwilight = (UKismetMathLibrary::FClamp(SunCoords.altitude, -18.0, 6.0) + 18.f) / 24.f;
	if (SunIntensityFalloff)
		Sun->SetIntensity(SunLux * SunIntensityFalloff->GetFloatValue(SunNormalizedTwilight));


	// ----- Begin Lighting Optimization
	if (SunCoords.altitude < -2.0 && Sun->CastDynamicShadows == true) // Sunset -> Night
	{
		Sun->CastDynamicShadows = false;
		Sun->LightingChannels.bChannel0 = false;
		Sun->bCastCloudShadows = false;
		Sun->MarkRenderStateDirty();

	}
	else if(SunCoords.altitude >= -2.0 && Sun->CastDynamicShadows == false)
	{
		Sun->CastDynamicShadows = true;
		Sun->LightingChannels.bChannel0 = true;
		Sun->bCastCloudShadows = true;
		Sun->MarkRenderStateDirty();

		Moon->CastDynamicShadows = false;
		Moon->LightingChannels.bChannel0 = false;
		Moon->bCastCloudShadows = false;
		Moon->MarkRenderStateDirty();
	}

	if (MoonCoords.altitude < -2.0 && Moon->CastDynamicShadows == true)
	{
		Moon->CastDynamicShadows = false;
		Moon->LightingChannels.bChannel0 = false;
		Moon->bCastCloudShadows = false;
		Moon->MarkRenderStateDirty();
	}
	else if (MoonCoords.altitude >= -2.0 && Moon->CastDynamicShadows == false && SunCoords.altitude < -2.0)
	{
		Moon->CastDynamicShadows = true;
		Moon->LightingChannels.bChannel0 = true;
		Moon->bCastCloudShadows = true;
		Moon->MarkRenderStateDirty();
	}
	// ----- End Lighting Optimization
}

void ASkySystem::ChangeTime(float Amount)
{
	float newLocalTime = SimData.LocalTime + Amount;

	UpdateSimulationTimeDate(newLocalTime);

	SimData.LocalTime = Astro::Overflow(newLocalTime, 24.f);

	OnTimeChanged.Broadcast(SimData.LocalTime);

	CalculatePlanetaryPositions();
	UpdateLighting();
}

void ASkySystem::UpdateSimulationTimeDate(float newTime)
{
	if (newTime >= 24.f)
		SimData.Day += 1;

	if (SimData.Day > FDateTime::DaysInMonth(SimData.Year, SimData.Month))
	{
		SimData.Day = 1;
		SimData.Month += 1;
		if (SimData.Month > 12)
		{
			SimData.Month = 1;
			SimData.Year = fminf(SimData.Year + 1, 2100);
			// upper limit of 2100 since planetary calculations
			// are only decently accurate for ca. 1900 - 2100
		}
	}
}

void ASkySystem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	CalculatePlanetaryPositions();
	UpdateLighting();
}

void ASkySystem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float TimeDelta = DeltaSeconds * SimulationSpeed * (24.0 / 86400.0);

	if (TimeskipRemaining > -1.f)
		TimeskipRemaining = TimeskipRemaining - TimeDelta * FastForwardTimeMultiplier;

	ChangeTime(TimeDelta * (TimeskipRemaining <= 0.f ? 1.f : FastForwardTimeMultiplier));
}


