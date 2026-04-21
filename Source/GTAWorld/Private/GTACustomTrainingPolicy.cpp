#include "GTACustomTrainingPolicy.h"

void UGTACustomTrainingPolicy::EvaluatePolicy_Implementation(const TArray<float>& Observation, TArray<float>& OutActions) const
{
	OutActions.Reset();
	OutActions.SetNum(GTAWorldTraining::DefaultActionDim);

	const float Dist = Observation.IsValidIndex(2) ? Observation[2] : 0.f;
	const float DirX = Observation.IsValidIndex(3) ? Observation[3] : 0.f;
	const float DirY = Observation.IsValidIndex(4) ? Observation[4] : 0.f;

	float Strafe = FMath::Clamp(DirX, -1.f, 1.f);
	float Forward = FMath::Clamp(DirY, -1.f, 1.f);
	float Yaw = FMath::Clamp(DirX * 45.f, -90.f, 90.f);

	if (Dist <= 0.05f)
	{
		Strafe = Forward = Yaw = 0.f;
	}

	if (bNormalizeActions)
	{
		const FVector2D M(Strafe, Forward);
		if (M.Size() > 1.f)
		{
			const FVector2D N = M.GetSafeNormal();
			Strafe = N.X;
			Forward = N.Y;
		}
	}

	OutActions[0] = Strafe;
	OutActions[1] = Forward;
	OutActions[2] = Yaw;
}
