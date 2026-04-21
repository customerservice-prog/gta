#include "AgentBrainComponent.h"

#include "Engine/World.h"

UAgentBrainComponent::UAgentBrainComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAgentBrainComponent::BeginPlay()
{
	Super::BeginPlay();
	ResetEpisodeCounters();
	if (TrainingDirectorActor)
	{
		OnTrainingDirectorBound(TrainingDirectorActor);
	}
}

void UAgentBrainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bTrackSurvivalReward && DeltaTime > KINDA_SMALL_NUMBER)
	{
		AddRewardSignal(EGTARewardSignalID::SurvivalSeconds, DeltaTime);
	}
}

void UAgentBrainComponent::AddRewardSignal(EGTARewardSignalID SignalId, float DeltaValue)
{
	RawEpisodeReward += DeltaValue;

	const float Alpha = FMath::Clamp(EpisodeRewardSmoothing, 0.f, 1.f);
	SmoothedEpisodeReward = FMath::Lerp(SmoothedEpisodeReward, RawEpisodeReward, Alpha);

	(void)SignalId;
}

void UAgentBrainComponent::AddRewardBatch(const TArray<FGTARewardSample>& Samples)
{
	for (const FGTARewardSample& S : Samples)
	{
		AddRewardSignal(S.Signal, S.Value);
	}
}

void UAgentBrainComponent::ResetEpisodeCounters()
{
	RawEpisodeReward = 0.f;
	SmoothedEpisodeReward = 0.f;
	SurvivalAccumulator = 0.f;
}

void UAgentBrainComponent::SetTrainingDirectorActor(AActor* InDirector)
{
	TrainingDirectorActor = InDirector;
	OnTrainingDirectorBound(InDirector);

	if (InDirector)
	{
		UE_LOG(LogTemp, Log, TEXT("AgentBrain: training director bound to '%s'."), *GetNameSafe(InDirector));
	}
}

void UAgentBrainComponent::OnTrainingDirectorBound_Implementation(AActor* Director)
{
}
