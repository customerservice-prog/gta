#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GTAWorldCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

/**
 * Third-person on-foot character — camera, locomotion, shoot, interact.
 */
UCLASS()
class GTAWORLD_API AGTAWorldCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGTAWorldCharacter();

	UFUNCTION(BlueprintPure, Category = "Camera")
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void OnJump();
	void OnFire();
	void OnInteract();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float WeaponRange = 10000.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float WeaponDamage = 28.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float InteractTraceDistance = 650.f;
};
