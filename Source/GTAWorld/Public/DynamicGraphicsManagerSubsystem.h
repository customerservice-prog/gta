#pragma once

#include "CoreMinimal.h"
#include "Subsystems/TickableWorldSubsystem.h"
#include "DynamicGraphicsManagerSubsystem.generated.h"

/**
 * Dynamic graphics: tracks frame budget and nudges scalability.
 * ----------------------------------------------------------------------------
 * FUNCTION DECLARATION:
 * - Tick policy: internal Tick adjusts scalability from smoothed frame time.
 * Neural Rendering bridges belong here once you link engine/plugin symbols for your SKU.
 */
UCLASS()
class GTAWORLD_API UDynamicGraphicsManagerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Tick(float DeltaSeconds) override;
	virtual TStatId GetStatId() const override;
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
	virtual bool IsTickableInEditor() const override { return false; }

	/** Milliseconds above which we step quality down one notch. */
	UPROPERTY(EditAnywhere, Category = "Graphics|Budget")
	float HighFrameTimeMs = 22.f;

	/** Milliseconds below which we may step quality up (hysteresis). */
	UPROPERTY(EditAnywhere, Category = "Graphics|Budget")
	float LowFrameTimeMs = 14.f;

	UPROPERTY(EditAnywhere, Category = "Graphics|Budget")
	float EvaluationIntervalSeconds = 1.f;

protected:
	float SmoothedFrameMs = 16.7f;
	float SecondsSinceEval = 0.f;
	/** Mirrors view-distance bucket (0–3) as a lightweight LOD / streaming lever. */
	int32 LastOverallLevel = 3;
};
