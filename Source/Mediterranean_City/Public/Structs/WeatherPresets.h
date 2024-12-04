#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EWeatherPresets : uint8
{
	Sunny UMETA(DisplayName = "Sunny"),
	Cloudy UMETA(DisplayName = "Cloudy"),
	Overcast UMETA(DisplayName = "Overcast"),
	Rainy UMETA(DisplayName = "Rainy"),
	Foggy UMETA(DisplayName = "Foggy")
};
