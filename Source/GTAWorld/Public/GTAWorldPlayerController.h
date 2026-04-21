#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GTAWorldPlayerController.generated.h"

class AGTAWorldCharacter;
class UDamageType;

/**
 * FUNCTION DECLARATION:
 * - Vehicle entry:      TryEnterNearbyVehicle
 * - Director telemetry: PushWorldDirectorMetrics (called from tick)
 */
UCLASS()
class GTAWORLD_API AGTAWorldPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGTAWorldPlayerController();

	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void TryEnterNearbyVehicle(AGTAWorldCharacter* FromCharacter, float TraceDistance);

protected:
	UFUNCTION()
	void HandlePawnDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void PushWorldDirectorTelemetry();

	UPROPERTY()
	TObjectPtr<APawn> CachedPawnForDamage;

	float DirectorAccumSeconds = 0.f;
	float SmoothedDamagePerSecond = 0.f;
	float KillMilestoneAccumulator = 0.f;
};
