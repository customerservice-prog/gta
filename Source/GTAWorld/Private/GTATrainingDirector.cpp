#include "GTATrainingDirector.h"

#include "AgentBrainComponent.h"
#include "BaseAgent.h"
#include "GTACustomTrainingPolicy.h"

AGTATrainingDirector::AGTATrainingDirector()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGTATrainingDirector::BeginPlay()
{
	Super::BeginPlay();

	if (!Policy && PolicyClass)
	{
		Policy = NewObject<UGTACustomTrainingPolicy>(this, PolicyClass);
	}
	else if (!Policy)
	{
		Policy = NewObject<UGTACustomTrainingPolicy>(this, UGTACustomTrainingPolicy::StaticClass());
	}
}

void AGTATrainingDirector::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Accumulator += DeltaSeconds;
	if (Accumulator >= StepSeconds)
	{
		Accumulator = 0.f;
		StepOnce();
	}
}

void AGTATrainingDirector::RegisterAgent(ABaseAgent* Agent)
{
	if (!Agent || Agents.Contains(Agent))
	{
		return;
	}
	Agents.Add(Agent);
	if (Agent->AgentBrain)
	{
		Agent->AgentBrain->SetTrainingDirectorActor(this);
	}
}

void AGTATrainingDirector::UnregisterAgent(ABaseAgent* Agent)
{
	if (Agent && Agent->AgentBrain && Agent->AgentBrain->GetTrainingDirectorActor() == this)
	{
		Agent->AgentBrain->SetTrainingDirectorActor(nullptr);
	}
	Agents.Remove(Agent);
}

void AGTATrainingDirector::ClearAgents()
{
	for (ABaseAgent* Agent : Agents)
	{
		if (Agent && Agent->AgentBrain && Agent->AgentBrain->GetTrainingDirectorActor() == this)
		{
			Agent->AgentBrain->SetTrainingDirectorActor(nullptr);
		}
	}
	Agents.Reset();
}

void AGTATrainingDirector::StepOnce()
{
	if (!Policy)
	{
		return;
	}

	for (ABaseAgent* Agent : Agents)
	{
		if (!IsValid(Agent))
		{
			continue;
		}

		TArray<float> Obs;
		Agent->AppendTrainingObservation(Obs, PlayerIndexForObservation);
		TArray<float> Actions;
		Policy->EvaluatePolicy(Obs, Actions);
		Agent->ApplyPolicyActionVector(Actions);
	}
}
