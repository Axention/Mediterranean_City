// Fill out your copyright notice in the Description page of Project Settings.


#include "EngineExtension/CaelumSkyLightComponent.h"

#include "Curves/CurveFloat.h"
#include "Curves/CurveLinearColor.h"

#include "Utility/CaelumUtilities.h"

UCaelumSkyLightComponent::UCaelumSkyLightComponent(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{
  IntensityCurve = nullptr;
  ColorCurve = nullptr;

  LumenIntensity = 1;
  LumenColor = FColor::White;
}

void UCaelumSkyLightComponent::UpdateLighting(float SunAltitude)
{
  static const auto LumenVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Lumen.DiffuseIndirect.Allow"));
  int32 usingLumen = LumenVar->GetInt();
  if (usingLumen == 1) {
    if (Intensity != LumenIntensity) SetIntensity(LumenIntensity);
    if (LightColor != LumenColor) SetLightColor(LumenColor);
    return;
  }


  if (IntensityCurve != nullptr)
    SetIntensity(IntensityCurve->GetFloatValue(SunAltitude));

  if (ColorCurve != nullptr)
    SetLightColor(ColorCurve->GetLinearColorValue(SunAltitude).ToFColor(false));
}
