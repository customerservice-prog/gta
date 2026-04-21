#include "GTAWorldGameMode.h"

#include "GTAWorldCharacter.h"
#include "GTAWorldPlayerController.h"

AGTAWorldGameMode::AGTAWorldGameMode()
{
	DefaultPawnClass = AGTAWorldCharacter::StaticClass();
	PlayerControllerClass = AGTAWorldPlayerController::StaticClass();
}
