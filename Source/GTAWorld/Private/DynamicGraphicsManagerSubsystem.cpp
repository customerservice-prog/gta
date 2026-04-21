#include "DynamicGraphicsManagerSubsystem.h"

#include "GameFramework/GameUserSettings.h"

void UDynamicGraphicsManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (UGameUserSettings* S = UGameUserSettings::GetGameUserSettings())
	{
		LastOverallLevel = S->GetViewDistanceQuality();
	}
}

void UDynamicGraphicsManagerSubsystem::Tick(float DeltaSeconds)
{
	const float FrameMs = DeltaSeconds * 1000.f;
	SmoothedFrameMs = FMath::Lerp(SmoothedFrameMs, FrameMs, 0.08f);
	SecondsSinceEval += DeltaSeconds;

	if (SecondsSinceEval < EvaluationIntervalSeconds)
	{
		return;
	}
	SecondsSinceEval = 0.f;

	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	if (!Settings)
	{
		return;
	}

	int32 Vd = Settings->GetViewDistanceQuality();
	if (SmoothedFrameMs > HighFrameTimeMs)
	{
		Vd = FMath::Max(Vd - 1, 0);
	}
	else if (SmoothedFrameMs < LowFrameTimeMs)
	{
		Vd = FMath::Min(Vd + 1, 3);
	}

	if (Vd != LastOverallLevel)
	{
		LastOverallLevel = Vd;
		Settings->SetViewDistanceQuality(Vd);
		Settings->ApplySettings(false);
	}
}

TStatId UDynamicGraphicsManagerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDynamicGraphicsManagerSubsystem, STATGROUP_Tickables);
}
