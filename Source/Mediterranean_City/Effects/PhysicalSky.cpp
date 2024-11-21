#include "PhysicalSky.h"

#include "Kismet/KismetMathLibrary.h"


DEFINE_LOG_CATEGORY(LogPhysicalSky)

namespace aMath
{
	/// <summary>
	/// wraps input into range of 0° - 360°
	/// </summary>
	/// <param name="x"> = input in degrees</param>
	/// <returns>input wrapped in 0° - 360° range</returns>
	static double rev(double x)
	{
		return x - floor(x / 360.0) * 360.0;
	}

	static double sinD(double x)
	{
		return sin(DOUBLE_PI / (180.0) * x);
	}

	static double cosD(double x)
	{
		return cos(DOUBLE_PI / (180.0) * x);
	}

	static double tanD(double x)
	{
		return tan(DOUBLE_PI / (180.0) * x);
	}
}




APhysicalSky::APhysicalSky()
{
	PrimaryActorTick.bCanEverTick = true;

	Latitude = 49.45;
	Longitude = 11.06;
	UTCTimeZone = 2;
	azimuthAngle = 135.17;
	elevationAngle = 5.27;
	localTime = 10.0;
	northOffset = 0.f;

	TimeScaleMultiplier = 1.0;

	bLockLocation = true;
	bListedInSceneOutliner = true;

	SpriteScale = 0.f;

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("Global Post Process"));
	SetRootComponent(PostProcess);
	PostProcess->Mobility = EComponentMobility::Static;

	Skylight = CreateDefaultSubobject<USkyLightComponent>(TEXT("Sky"));
	Skylight->SetupAttachment(PostProcess);
	Skylight->Mobility = EComponentMobility::Movable;

	Sun = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Sun"));
	Sun->SetupAttachment(PostProcess);
	Sun->ForwardShadingPriority = 2;
	Sun->AtmosphereSunLightIndex = 0;
	Sun->SetRelativeRotation(FRotator(-elevationAngle, azimuthAngle + northOffset, 0.0));

	Moon = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Moon"));
	Moon->SetupAttachment(PostProcess);
	Moon->ForwardShadingPriority = 1;
	Moon->AtmosphereSunLightIndex = 1;
	Moon->SetRelativeRotation(FRotator(-26, 35, 0));

	Atmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("Atmosphere"));
	Atmosphere->SetupAttachment(PostProcess);

	Fog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("Fog"));
	Fog->SetupAttachment(PostProcess);

	SkyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sky Sphere"));
	SkyMesh->SetupAttachment(PostProcess);
	SkyMesh->SetRelativeScale3D(FVector(100000.0));

	Compass = CreateEditorOnlyDefaultSubobject<UStaticMeshComponent>(TEXT("Editor Compass"));
	Compass->SetupAttachment(PostProcess);
	Compass->SetRelativeLocationAndRotation(FVector(0, 0, 1500), FRotator(0.0, -90.0 + northOffset, 0.0));
}

bool APhysicalSky::UpdateSky()
{
	UpdateSunPosition();
	UpdateMoonPosition();

	// Update Skysphere Rotation to represent accurate nightsky at location
	SkyMesh->SetRelativeRotation(FRotator(Latitude, 180.0 + northOffset, localTime * 15.0));

	// update compass rotation to match north
	Compass->SetRelativeRotation(FRotator(0.0, -90.0 + northOffset, 0.0));

	return true;
}

void APhysicalSky::ChangeTime(float Amount)
{
	float _newTime = localTime + Amount;

	if (CheckForInvalidTimeDate(_newTime))
		return;

	localTime = _newTime;

	UpdateSky();
	OnTimeSkip.Broadcast(localTime);
}

void APhysicalSky::SkipTime(float Amount)
{
	TimeToSkipTo = localTime + Amount;
	TimeToSkipTo = TimeToSkipTo - floor(TimeToSkipTo / 24.0) * 24.0;
	TimeScaleMultiplier = 60.0;
}

void APhysicalSky::Tick(float DeltaSeconds)
{
	ChangeTime(DeltaSeconds * (1.0 / 60.0) * TimeScaleMultiplier);
	if (UKismetMathLibrary::InRange_FloatFloat(localTime, TimeToSkipTo, MAX_dbl))
	{
		TimeScaleMultiplier = 1.0;
	}
}


void APhysicalSky::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (UpdateSky() == false)
		UE_LOG(LogPhysicalSky, Error, TEXT("Could not update Sky"));
}

void APhysicalSky::UpdateSunPosition()
{
	float LSTM = 15 * UTCTimeZone;

	int dayOfYear = FDateTime(Date.Year, Date.Month, Date.Day).GetDayOfYear();

	float B = (360.0 / 365.0) * (dayOfYear - 81);
	float EoT = (9.87 * UKismetMathLibrary::DegSin(2 * B)) - (7.53 * UKismetMathLibrary::DegCos(B)) - (1.5 * UKismetMathLibrary::DegSin(B));
	float TC = (4 * (Longitude - LSTM)) + EoT;
	float LST = localTime + (TC / 60.0);

	float declinationAngle = -23.45 * UKismetMathLibrary::DegCos((360.0 / 365.0) * (dayOfYear + 10));
	float hourAngle = 15.0 * (LST - 12.0);

	elevationAngle = UKismetMathLibrary::DegAsin(UKismetMathLibrary::DegSin(declinationAngle) * UKismetMathLibrary::DegSin(Latitude) + UKismetMathLibrary::DegCos(declinationAngle) * UKismetMathLibrary::DegCos(Latitude) * UKismetMathLibrary::DegCos(hourAngle));

	azimuthAngle = UKismetMathLibrary::DegAcos((UKismetMathLibrary::DegSin(declinationAngle) * UKismetMathLibrary::DegCos(Latitude) - UKismetMathLibrary::DegCos(declinationAngle) * UKismetMathLibrary::DegSin(Latitude) * UKismetMathLibrary::DegCos(hourAngle)) / UKismetMathLibrary::DegCos(elevationAngle));
	if (hourAngle >= 0)
		azimuthAngle = 360 - azimuthAngle;

	Sun->SetRelativeRotation(FRotator(-elevationAngle, azimuthAngle + northOffset, 0.0));

	// optimized lighting calculations based on visibility
	if (-elevationAngle >= 2.0)
	{
		if(Sun->CastDynamicShadows == true)
			Sun->CastDynamicShadows = false;
		Sun->LightingChannels.bChannel0 = false;
		Sun->bCastCloudShadows = false;
	}
	else
	{
		if(Sun->CastDynamicShadows == false)
			Sun->CastDynamicShadows = true;
		Sun->LightingChannels.bChannel0 = true;
		Sun->bCastCloudShadows = true;
	}
}

void APhysicalSky::UpdateMoonPosition()
{
	{
		using namespace aMath;

		// own implementation of formulas from stjarnhimlen.se

		double UT = localTime - UTCTimeZone; // universal time, UTC, GMT

		double d = 367 * Date.Year - 7 * (Date.Year + (Date.Month + 9) / 12) / 4 - 3 * ((Date.Year + (Date.Month - 9) / 7) / 100 + 1) / 4 + 275 * Date.Month / 9 + Date.Day - 730515;
		d = d + UT / 24.0;

		double N = 125.1228 - 0.0529538083 * d;	// Long asc. node
		double i = 5.1454;								// inclination
		double w = 318.0634 + 0.1643573223 * d;	// Arg. of perigee
		double a = 60.2666;								// Mean distance
		double e = 0.054900;								// eccentricity
		double M = 115.3654 + 13.0649929509 * d;	// Mean anomaly
		N = rev(N);
		w = rev(w);
		M = rev(M);

		// perturbation correctors
		double Ms = rev(356.0470 + 0.9856002585 * d);	// Sun's mean anomaly
		double wS = rev(282.9404 + 0.0000470935 * d);	// Sun's longitude of periphelon
		double Ls = rev(wS + Ms);								// Sun's mean longitude

		double Lm = rev(N + w + M);			// Moon's mean longitude
		double Mm = M;								// Moon's mean anomaly
		double D = rev(Lm - Ls);				// Moon's mean elongation
		double F = rev(Lm - N);					// Moon's argument of latitude

		double _evection = -1.274 * sinD(Mm - 2 * D);
		double _variation = 0.658 * sinD(2 * D);
		double _yearlyEquation = -0.186 * sinD(Ms);

		double _pLon = _evection + _variation + _yearlyEquation;
		double _pLat = -0.173 * sinD(F - 2 * D);


		double E0 = M + (180.0 / PI) * e * sinD(M) * (1 + e * cosD(M));
		double E1 = E0 - (E0 - (180.0 / PI) * e * sinD(E0) - M) / (1 - e * cosD(E0));
		for (int iter = 0; iter < 5; iter++)
		{
			if (abs(E0 - E1) < 0.005)
				break;

			E0 = E1;
			E1 = E0 - (E0 - (180.0 / PI) * e * sinD(E0) - M) / (1 - e * cosD(E0));
		}

		double E = E1; // eccentric anomaly

		double x = a * (cosD(E) - e);					// rect X coord in lunar orbit plane
		double y = a * sqrt(1 - e * e) * sinD(E);	// rect Y coord in lunar orbit plane

		double r = sqrt(x * x + y * y);				// distance (in earth radii)
		double v = rev(atan2(y, x) * 180.0 / PI);	// true anomaly

		double xeclip = r * (cosD(N) * cosD(v + w) - sinD(N) * sinD(v + w) * cosD(i));
		double yeclip = r * (sinD(N) * cosD(v + w) + cosD(N) * sinD(v + w) * cosD(i));
		double zeclip = r * sinD(v + w) * sinD(i);

		double _long = rev(atan2(yeclip, xeclip) * 180.0 / PI + _pLon);
		double _lat = atan2(zeclip, sqrt(xeclip * xeclip + yeclip * yeclip)) * 180.0 / PI + _pLat;
		r = sqrt(xeclip * xeclip + yeclip * yeclip + zeclip * zeclip);

		double oblecl = 23.4393 - 0.0000003563 * d; // obliquity of the eliptic

		xeclip = cosD(_long) * cosD(_lat);
		yeclip = sinD(_long) * cosD(_lat);
		zeclip = sinD(_lat);

		double xequat = xeclip;
		double yequat = yeclip * cosD(oblecl) - zeclip * sinD(oblecl);
		double zequat = yeclip * sinD(oblecl) + zeclip * cosD(oblecl);

		double RA = rev(atan2(yequat, xequat) * 180.0 / PI);
		double Decl = atan2(zequat, sqrt(xequat * xequat + yequat * yequat)) * 180.0 / PI;

		double GMST0DEG = rev(Ls + 180.0);
		double SIDTIMEDEG = GMST0DEG + (UT * 15.0) + Longitude;

		double HA = SIDTIMEDEG - RA;

		double _x = cosD(HA) * cosD(Decl);
		double _y = sinD(HA) * cosD(Decl);
		double _z = sinD(Decl);
		double xhor = (_x * sinD(Latitude)) - (_z * cosD(Latitude));
		double yhor = _y;
		double zhor = (_x * cosD(Latitude)) + (_z * sinD(Latitude));


		double azimuth = atan2(yhor, xhor) * 180.0 / PI + 180.0;
		double altitude = asin(zhor) * 180.0 / PI;

		Moon->SetRelativeRotation(FRotator(-altitude, azimuth + northOffset, 0.0));

	// optimized lighting calculations based on visibility
		if (-elevationAngle >= 2.0)
		{
			if (-altitude >= 2.0)
			{
				if (Moon->CastDynamicShadows == true)
					Moon->CastDynamicShadows = false;
				Moon->bCastCloudShadows = false;
			}
			else
			{
				if (Moon->CastDynamicShadows == false)
					Moon->CastDynamicShadows = true;
				Moon->LightingChannels.bChannel0 = true;
				Moon->bCastCloudShadows = true;
			}
		}
		else
		{
			Moon->CastDynamicShadows = false;
			Moon->LightingChannels.bChannel0 = false;
			Moon->bCastCloudShadows = false;
		}

	}
}

bool APhysicalSky::CheckForInvalidTimeDate(float newTime)
{
	if (newTime >= 24.0)
		Date.Day += 1;

	if (Date.Day > FDateTime::DaysInMonth(Date.Year, Date.Month))
	{
		Date.Day = 1;
		Date.Month += 1;
		if (Date.Month > 12)
		{
			Date.Month = 1;
			Date.Year = fminf(Date.Year + 1, 9999);
		}
	}

	newTime = (newTime - floor(newTime / 24.f) * 24.f);
	float fminutes = fmod(newTime, 1.0) * 60.0;
	float fseconds = fmod(fminutes, 1.0) * 60.0;

	if (!FDateTime::Validate(Date.Year, Date.Month, Date.Day, floor(newTime), floor(fminutes), floor(fseconds), floor(fmod(fseconds, 1.0) * 1000.0)))
	{
		UE_LOG(LogPhysicalSky, Error, TEXT("New Date / Time is not valid!"));
		return true;
	}

	return false;
}







