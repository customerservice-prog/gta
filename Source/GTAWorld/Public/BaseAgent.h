#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GTAWorldTrainingLayout.h"
#include "LearningFunctionDeclarations.h"
#include "BaseAgent.generated.h"

class UAgentBrainComponent;

/**
 * BaseAgent — RL-capable NPC body.
 * ----------------------------------------------------------------------------
 * FUNCTION DECLARATION:
 * - Locomotion from policy:  ApplyBrainLocomotionStep
 * - Combat feedback:        ReportDamageDealt / ReportDamageTaken (feeds AgentBrain rewards)
 * - Episode boundary:        ResetLearningEpisode
 *
 * Pair with `AGTATrainingDirector` + `UGTACustomTrainingPolicy` for your own training loop (no third-party ML plugin required).
 */
UCLASS(Abstract)
class GTAWORLD_API ABaseAgent : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseAgent();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

	// #region FunctionDeclaration — Policy hooks
	/** Apply normalized locomotion intent from RL (-1..1 per axis), scaled by AgentMaxWalkSpeed. */
	UFUNCTION(BlueprintCallable, Category = GTA_LEARNING_CATEGORY_AGENT, meta = (ToolTip = "Normalized locomotion from your policy or scripts."))
	void ApplyBrainLocomotionStep(FVector2D MoveAxes, float YawDeltaDegrees);

	/** Pack `GTAWorldTraining::DefaultObservationDim` floats for your policy (override in Blueprint via Append Training Observation if you expose an event, or subclass). */
	UFUNCTION(BlueprintCallable, Category = "Training")
	void AppendTrainingObservation(TArray<float>& Out, int32 PlayerIndex = 0) const;

	/** Expects `GTAWorldTraining::DefaultActionDim` floats: strafe, forward, yaw (deg). */
	UFUNCTION(BlueprintCallable, Category = "Training")
	void ApplyPolicyActionVector(const TArray<float>& Actions);

	UFUNCTION(BlueprintCallable, Category = GTA_LEARNING_CATEGORY_AGENT)
	void ReportDamageDealt(float Amount);

	UFUNCTION(BlueprintCallable, Category = GTA_LEARNING_CATEGORY_AGENT)
	void ReportDamageTaken(float Amount);

	UFUNCTION(BlueprintCallable, Category = GTA_LEARNING_CATEGORY_AGENT)
	void ResetLearningEpisode();
	// #endregion

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Learning")
	TObjectPtr<UAgentBrainComponent> AgentBrain;

	/** Upper bound used when mapping policy outputs to movement. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	float AgentMaxWalkSpeed = 450.f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float StuckSpeedThreshold = 20.f;

	float TimeNearStationary = 0.f;
};
