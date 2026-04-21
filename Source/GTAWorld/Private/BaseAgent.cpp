#include "BaseAgent.h"

#include "AgentBrainComponent.h"
#include "GTAWorldTrainingLayout.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseAgent::ABaseAgent()
{
	PrimaryActorTick.bCanEverTick = true;

	AgentBrain = CreateDefaultSubobject<UAgentBrainComponent>(TEXT("AgentBrain"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = AgentMaxWalkSpeed;
}

void ABaseAgent::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseAgent::AppendTrainingObservation(TArray<float>& Out, int32 PlayerIndex) const
{
	Out.Reset();
	Out.Reserve(GTAWorldTraining::DefaultObservationDim);

	const FVector V = GetVelocity();
	Out.Add(V.X / 800.f);
	Out.Add(V.Y / 800.f);

	if (const UWorld* World = GetWorld())
	{
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, PlayerIndex))
		{
			const FVector Loc = GetActorLocation();
			const FVector PLoc = PlayerPawn->GetActorLocation();
			const FVector Delta = PLoc - Loc;
			const float Dist2D = FVector::Dist2D(Loc, PLoc);
			Out.Add(FMath::Clamp(Dist2D / 5000.f, 0.f, 1.f));

			const FVector2D Planar(Delta.X, Delta.Y);
			const FVector2D N = Planar.GetSafeNormal();
			Out.Add(N.X);
			Out.Add(N.Y);

			const float Face = FVector::DotProduct(GetActorForwardVector(), Delta.GetSafeNormal());
			Out.Add(Face);
		}
		else
		{
			Out.Add(0.f);
			Out.Add(0.f);
			Out.Add(0.f);
			Out.Add(0.f);
		}
	}
	else
	{
		while (Out.Num() < 6)
		{
			Out.Add(0.f);
		}
	}

	while (Out.Num() < GTAWorldTraining::DefaultObservationDim)
	{
		Out.Add(0.f);
	}

	Out.SetNum(GTAWorldTraining::DefaultObservationDim);
}

void ABaseAgent::ApplyPolicyActionVector(const TArray<float>& Actions)
{
	if (Actions.Num() < GTAWorldTraining::DefaultActionDim)
	{
		return;
	}
	ApplyBrainLocomotionStep(FVector2D(Actions[0], Actions[1]), Actions[2]);
}

void ABaseAgent::ApplyBrainLocomotionStep(FVector2D MoveAxes, float YawDeltaDegrees)
{
	MoveAxes = MoveAxes.ClampAxes(-1.f, 1.f);

	AddMovementInput(GetActorForwardVector(), MoveAxes.Y);
	AddMovementInput(GetActorRightVector(), MoveAxes.X);

	if (!FMath::IsNearlyZero(YawDeltaDegrees))
	{
		AddActorWorldRotation(FRotator(0.f, YawDeltaDegrees, 0.f));
	}

	const float Vel = GetVelocity().Size2D();
	if (Vel < StuckSpeedThreshold)
	{
		TimeNearStationary += GetWorld()->GetDeltaSeconds();
		if (TimeNearStationary > 2.f)
		{
			if (AgentBrain)
			{
				AgentBrain->AddRewardSignal(EGTARewardSignalID::StuckPenalty, -0.25f);
			}
			TimeNearStationary = 0.f;
		}
	}
	else
	{
		TimeNearStationary = 0.f;
	}
}

void ABaseAgent::ReportDamageDealt(float Amount)
{
	if (AgentBrain && Amount > 0.f)
	{
		AgentBrain->AddRewardSignal(EGTARewardSignalID::DamageDealt, Amount * 0.01f);
	}
}

void ABaseAgent::ReportDamageTaken(float Amount)
{
	if (AgentBrain && Amount > 0.f)
	{
		AgentBrain->AddRewardSignal(EGTARewardSignalID::DamageTaken, -Amount * 0.01f);
	}
}

void ABaseAgent::ResetLearningEpisode()
{
	if (AgentBrain)
	{
		AgentBrain->ResetEpisodeCounters();
	}
}

float ABaseAgent::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Actual = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ReportDamageTaken(Actual);
	return Actual;
}

void ABaseAgent::FellOutOfWorld(const UDamageType& dmgType)
{
	if (AgentBrain)
	{
		AgentBrain->AddRewardSignal(EGTARewardSignalID::StuckPenalty, -5.f);
	}
	Super::FellOutOfWorld(dmgType);
}
