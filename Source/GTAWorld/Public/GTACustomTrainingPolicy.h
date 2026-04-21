#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GTAWorldTrainingLayout.h"
#include "GTACustomTrainingPolicy.generated.h"

/**
 * YOUR policy object — override EvaluatePolicy in C++ or a Blueprint child.
 * Replace the default heuristic with ONNX, your own small NN, or scripted tactics.
 */
UCLASS(Blueprintable)
class GTAWORLD_API UGTACustomTrainingPolicy : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Training")
	bool bNormalizeActions = true;

	UFUNCTION(BlueprintNativeEvent, Category = "Training")
	void EvaluatePolicy(const TArray<float>& Observation, TArray<float>& OutActions) const;
};
