#pragma once

#include <corecrt_math.h>

struct FAzimuthialCoords
{
  float azimuth;
  float altitude;

  FAzimuthialCoords()
  {
    azimuth = 0.f;
    altitude = 10.f;
  }

  FAzimuthialCoords(float az, float al) : azimuth(az), altitude(al) {}
};

namespace Astro
{

  static inline float DegToRad(float x)
  {
    return x * 3.1415926535897932f / 180.f;
  }

  static inline float RadToDeg(float x)
  {
    return x * 180.f / 3.1415926535897932f;
  }

  static inline float dsin(float x)
  {
    return sinf(DegToRad(x));
  }

  static inline float dcos(float x)
  {
    return cosf(DegToRad(x));
  }

  /*returns degree input wrapped into 0 - 360 range. (eg. 427.f -> 67.f)*/
  static float dWrap(float x)
  {
    return x - floor(x / 360.0) * 360.0;
  }

  /*wraps "number" into "min to max" range.*/
  static float Overflow(float number, float min, float max)
  {
    return fmod(number - min, max - min + 1) + min;
  }

  // wraps "x" into range [0.0, exclusiveMax[
  static float Overflow(float x, float exclusiveMax)
  {
    return x - floor(x / exclusiveMax) * exclusiveMax;
  }

  /*Converts HourAngle, Declination, latitude & longitude into Azimuthal Coordinates: azimuth & altitude over horizon.*/
  static FAzimuthialCoords GetAzimuthialCoords(float HA, float Decl, float lat, float lon)
  {
    float x = dcos(HA) * dcos(Decl);
    float y = dsin(HA) * dcos(Decl);
    float z = dsin(Decl);

    float xh = x * dsin(lat) - z * dcos(lat);
    float zh = x * dcos(lat) + z * dsin(lat);

    float az = RadToDeg(atan2f(y, xh)) + 180.f;
    float alt = RadToDeg(asinf(zh));

    return FAzimuthialCoords(az, alt);
  }
}