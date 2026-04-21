#pragma once

#include "CoreMinimal.h"
#include "Subsystems/TickableWorldSubsystem.h"
#include "WorldDirectorSubsystem.generated.h"

/** Normalized fun/challenge proxies collected from gameplay. */
USTRUCT(BlueprintType)
struct GTAWORLD_API FWorldDirectorMetrics
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metrics")
	float DamageTakenPerMinute = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metrics")
	float PlayerKillsLastFiveMin = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metrics")
	float ObjectiveProgress01 = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Metrics")
	float AverageEngagement01 = 0.f;
};

/** Tunables World Director applies to keep pacing in band. */
USTRUCT(BlueprintType)
struct GTAWORLD_API FWorldFeelState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feel")
	float TimeDilation = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feel", meta = (ClampMin = "-2000", ClampMax = "2000"))
	float TargetGravityZ = -980.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feel", meta = (ClampMin = "0", ClampMax = "150000"))
	float SunIntensity = 100000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feel", meta = (ClampMin = "0", ClampMax = "10"))
	float FogDensity = 0.02f;
};

/**
 * WorldDirectorSubsystem — high-level “fun / challenge” loop.
 * ----------------------------------------------------------------------------
 * FUNCTION DECLARATION:
 * - Ingest metrics:  PushPlayerMetricsSnapshot
 * - Apply output:     ApplyFeelState (or internal tick policy)
 */
UCLASS()
class GTAWORLD_API UWorldDirectorSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual void Tick(float DeltaSeconds) override;
	virtual TStatId GetStatId() const override;
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
	virtual bool IsTickableInEditor() const override { return false; }

	UFUNCTION(BlueprintCallable, Category = "WorldDirector")
	void PushPlayerMetricsSnapshot(const FWorldDirectorMetrics& Snapshot);

	UFUNCTION(BlueprintCallable, Category = "WorldDirector")
	void ApplyFeelState(const FWorldFeelState& State);

	/** Minimum seconds between heavy environment updates (lighting / fog). */
	UPROPERTY(EditAnywhere, Category = "WorldDirector")
	float MinSecondsBetweenEnvironmentTouches = 2.f;

protected:
	void TickAdjust(float DeltaSeconds);

	FWorldDirectorMetrics LastMetrics;
	FWorldFeelState CurrentFeel;
	float SecondsSinceEnvTouch = 0.f;
};
