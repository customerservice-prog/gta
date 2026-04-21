#pragma once

#include "CoreMinimal.h"

/**
 * Contract between ABaseAgent::AppendTrainingObservation and UGTACustomTrainingPolicy::Evaluate.
 * Change these constants in one place when you evolve your own net / feature extractor.
 */
namespace GTAWorldTraining
{
	inline constexpr int32 DefaultObservationDim = 8;
	/** Policy output: strafe, forward, yaw delta (degrees). */
	inline constexpr int32 DefaultActionDim = 3;
}
