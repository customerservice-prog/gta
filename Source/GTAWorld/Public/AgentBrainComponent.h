#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LearningFunctionDeclarations.h"
#include "AgentBrainComponent.generated.h"

class AActor;

/** Signals for your own reward / analytics stack. */
UENUM(BlueprintType)
enum class EGTARewardSignalID : uint8
{
	SurvivalSeconds UMETA(DisplayName = "Survival Time Delta"),
	DamageDealt UMETA(DisplayName = "Damage Dealt To Targets"),
	DamageTaken UMETA(DisplayName = "Damage Taken"),
	KillOrDisable UMETA(DisplayName = "Kill Or Disable Target"),
	StuckPenalty UMETA(DisplayName = "Stuck / No Progress Penalty"),
	Custom UMETA(DisplayName = "Custom Designer Signal")
};

USTRUCT(BlueprintType)
struct GTAWORLD_API FGTARewardSample
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	EGTARewardSignalID Signal = EGTARewardSignalID::Custom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	float Value = 0.f;
};

/**
 * AgentBrainComponent — reward bookkeeping for your training / telemetry layer.
 * ----------------------------------------------------------------------------
 * FUNCTION DECLARATION:
 * - Accumulate rewards:    AddRewardSignal / AddRewardBatch
 * - Episode lifecycle:     ResetEpisodeCounters
 * - Optional director hook: SetTrainingDirectorActor — e.g. level AGTATrainingDirector
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GTAWORLD_API UAgentBrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAgentBrainComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// #region FunctionDeclaration — Reward
	UFUNCTION(BlueprintCallable, Category = GTA_LEARNING_CATEGORY_REWARD, meta = (ToolTip = "Add a scalar reward contribution for the current policy step or episode."))
	void AddRewardSignal(EGTARewardSignalID SignalId, float DeltaValue);

	UFUNCTION(BlueprintCallable, Category = GTA_LEARNING_CATEGORY_REWARD)
	void AddRewardBatch(const TArray<FGTARewardSample>& Samples);

	UFUNCTION(BlueprintPure, Category = GTA_LEARNING_CATEGORY_REWARD)
	float GetSmoothedEpisodeReward() const { return SmoothedEpisodeReward; }

	UFUNCTION(BlueprintCallable, Category = GTA_LEARNING_CATEGORY_REWARD)
	void ResetEpisodeCounters();
	// #endregion

	// #region FunctionDeclaration — Training director
	UFUNCTION(BlueprintCallable, Category = GTA_LEARNING_CATEGORY_AGENT)
	void SetTrainingDirectorActor(AActor* InDirector);

	UFUNCTION(BlueprintPure, Category = GTA_LEARNING_CATEGORY_AGENT)
	AActor* GetTrainingDirectorActor() const { return TrainingDirectorActor; }
	// #endregion

	UPROPERTY(EditAnywhere, Category = GTA_LEARNING_CATEGORY_REWARD)
	bool bTrackSurvivalReward = true;

	UPROPERTY(EditAnywhere, Category = GTA_LEARNING_CATEGORY_REWARD, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float EpisodeRewardSmoothing = 0.05f;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = GTA_LEARNING_CATEGORY_AGENT)
	void OnTrainingDirectorBound(AActor* Director);
	virtual void OnTrainingDirectorBound_Implementation(AActor* Director);

private:
	UPROPERTY()
	TObjectPtr<AActor> TrainingDirectorActor;

	float RawEpisodeReward = 0.f;
	float SmoothedEpisodeReward = 0.f;
	float SurvivalAccumulator = 0.f;
};
