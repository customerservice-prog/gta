#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GTAWorldGameMode.generated.h"

/**
 * Root game mode — defaults for on-foot third-person; vehicles reuse same PC class.
 */
UCLASS()
class GTAWORLD_API AGTAWorldGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGTAWorldGameMode();
};
