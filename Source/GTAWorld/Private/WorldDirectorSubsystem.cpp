#include "WorldDirectorSubsystem.h"

#include "Components/ExponentialHeightFogComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/WorldSettings.h"

void UWorldDirectorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UWorldDirectorSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UWorldDirectorSubsystem::Tick(float DeltaSeconds)
{
	TickAdjust(DeltaSeconds);
}

TStatId UWorldDirectorSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UWorldDirectorSubsystem, STATGROUP_Tickables);
}

void UWorldDirectorSubsystem::TickAdjust(float DeltaSeconds)
{
	SecondsSinceEnvTouch += DeltaSeconds;

	FWorldFeelState Adjusted = CurrentFeel;

	const float Stress = FMath::Clamp(LastMetrics.DamageTakenPerMinute / 300.f, 0.f, 1.f);
	const float Mastery = FMath::Clamp(LastMetrics.PlayerKillsLastFiveMin / 10.f, 0.f, 1.f);

	Adjusted.TimeDilation = FMath::Clamp(1.f + 0.15f * (Stress - Mastery), 0.85f, 1.1f);
	Adjusted.TargetGravityZ = FMath::Lerp(-880.f, -1100.f, Mastery);
	Adjusted.SunIntensity = FMath::Lerp(75000.f, 125000.f, 1.f - Stress * 0.5f);
	Adjusted.FogDensity = FMath::Lerp(0.01f, 0.045f, Stress);

	if (SecondsSinceEnvTouch >= MinSecondsBetweenEnvironmentTouches)
	{
		ApplyFeelState(Adjusted);
		SecondsSinceEnvTouch = 0.f;
	}
	else if (UWorld* World = GetWorld())
	{
		World->GetWorldSettings()->TimeDilation = Adjusted.TimeDilation;
	}
}

void UWorldDirectorSubsystem::PushPlayerMetricsSnapshot(const FWorldDirectorMetrics& Snapshot)
{
	LastMetrics = Snapshot;
}

void UWorldDirectorSubsystem::ApplyFeelState(const FWorldFeelState& State)
{
	CurrentFeel = State;

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	World->GetWorldSettings()->TimeDilation = State.TimeDilation;

	// Gravity is applied through CharacterMovement scales (portable across engine versions).
	const float ReferenceGravityZ = -980.f;
	const float GravityScale = State.TargetGravityZ / ReferenceGravityZ;
	for (TActorIterator<ACharacter> ChIt(World); ChIt; ++ChIt)
	{
		if (UCharacterMovementComponent* Move = ChIt->GetCharacterMovement())
		{
			Move->GravityScale = FMath::Max(GravityScale, 0.01f);
		}
	}

	for (TActorIterator<ADirectionalLight> It(World); It; ++It)
	{
		if (UDirectionalLightComponent* L = It->GetDirectionalLightComponent())
		{
			L->SetIntensity(State.SunIntensity);
		}
	}

	for (TActorIterator<AExponentialHeightFog> It(World); It; ++It)
	{
		if (UExponentialHeightFogComponent* Fog = It->FindComponentByClass<UExponentialHeightFogComponent>())
		{
			Fog->SetFogDensity(State.FogDensity);
			break;
		}
	}
}
