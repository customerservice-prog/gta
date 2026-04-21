#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GTAWorldTrainingLayout.h"
#include "GTATrainingDirector.generated.h"

class ABaseAgent;
class UGTACustomTrainingPolicy;

/**
 * Level-placed actor that runs YOUR training tick: gather obs → policy → ApplyPolicyActionVector.
 * No external ML plugins required — swap UGTACustomTrainingPolicy for your implementation.
 */
UCLASS()
class GTAWORLD_API AGTATrainingDirector : public AActor
{
	GENERATED_BODY()

public:
	AGTATrainingDirector();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Training")
	void RegisterAgent(ABaseAgent* Agent);

	UFUNCTION(BlueprintCallable, Category = "Training")
	void UnregisterAgent(ABaseAgent* Agent);

	UFUNCTION(BlueprintCallable, Category = "Training")
	void ClearAgents();

	UPROPERTY(EditAnywhere, Category = "Training", meta = (ClampMin = "0.01"))
	float StepSeconds = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Training")
	TSubclassOf<UGTACustomTrainingPolicy> PolicyClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Training")
	TObjectPtr<UGTACustomTrainingPolicy> Policy;

	UPROPERTY(EditAnywhere, Category = "Training")
	int32 PlayerIndexForObservation = 0;

protected:
	virtual void BeginPlay() override;

private:
	void StepOnce();

	UPROPERTY()
	TArray<TObjectPtr<ABaseAgent>> Agents;

	float Accumulator = 0.f;
};
