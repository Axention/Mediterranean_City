#pragma once

#include "CoreMinimal.h"

#include "Core/Interaction/InteractionInterface.h"

#include "InteractableBase.generated.h"

DECLARE_DELEGATE(FOnInteractionDelegate);

UCLASS()
class AInteractable : public AActor, public IInteractionInterface
{
   GENERATED_BODY()

public:
   AInteractable();

   FOnInteractionDelegate OnInteractionDelegate;
};
