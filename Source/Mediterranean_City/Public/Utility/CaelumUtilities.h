// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Effects/PhysicalSky.h"

#include "CaelumUtilities.generated.h"

/**
 * 
 */
UCLASS()
class MEDITERRANEAN_CITY_API UCaelumUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static APhysicalSky* GetTimeOfDaySystem(UObject* WorldContextObject);
};
