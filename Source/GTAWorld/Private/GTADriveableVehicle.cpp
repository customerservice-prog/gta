#include "GTADriveableVehicle.h"

#include "GTAWorldCharacter.h"
#include "ChaosVehicleMovementComponent.h"
#include "GameFramework/PlayerController.h"

AGTADriveableVehicle::AGTADriveableVehicle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGTADriveableVehicle::SetSavedOnFootCharacter(AGTAWorldCharacter* InCharacter)
{
	SavedOnFootCharacter = InCharacter;
}

void AGTADriveableVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGTADriveableVehicle::Throttle);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGTADriveableVehicle::Steering);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AGTADriveableVehicle::OnExitInteract);
}

void AGTADriveableVehicle::Throttle(float Value)
{
	if (UChaosVehicleMovementComponent* ChaosMove = Cast<UChaosVehicleMovementComponent>(GetVehicleMovementComponent()))
	{
		ChaosMove->SetThrottleInput(Value);
	}
}

void AGTADriveableVehicle::Steering(float Value)
{
	if (UChaosVehicleMovementComponent* ChaosMove = Cast<UChaosVehicleMovementComponent>(GetVehicleMovementComponent()))
	{
		ChaosMove->SetSteeringInput(Value);
	}
}

void AGTADriveableVehicle::OnExitInteract()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}

	if (AGTAWorldCharacter* Char = SavedOnFootCharacter.Get())
	{
		PC->Possess(Char);
	}
}
