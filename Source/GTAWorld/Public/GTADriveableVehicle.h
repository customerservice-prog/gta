#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "GTADriveableVehicle.generated.h"

class AGTAWorldCharacter;

/** Chaos-driven vehicle pawn with enter/exit hand-off to AGTAWorldCharacter. */
UCLASS()
class GTAWORLD_API AGTADriveableVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	AGTADriveableVehicle();

	void SetSavedOnFootCharacter(AGTAWorldCharacter* InCharacter);

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Throttle(float Value);
	void Steering(float Value);
	void OnExitInteract();

	UPROPERTY()
	TObjectPtr<AGTAWorldCharacter> SavedOnFootCharacter;
};
