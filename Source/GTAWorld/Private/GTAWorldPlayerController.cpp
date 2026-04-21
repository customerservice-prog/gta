#include "GTAWorldPlayerController.h"

#include "GTADriveableVehicle.h"
#include "GTAWorldCharacter.h"
#include "WorldDirectorSubsystem.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/DamageType.h"

AGTAWorldPlayerController::AGTAWorldPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = false;
}

void AGTAWorldPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	PushWorldDirectorTelemetry();
}

void AGTAWorldPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (CachedPawnForDamage)
	{
		CachedPawnForDamage->OnTakeAnyDamage.RemoveDynamic(this, &AGTAWorldPlayerController::HandlePawnDamage);
		CachedPawnForDamage = nullptr;
	}

	if (InPawn)
	{
		InPawn->OnTakeAnyDamage.AddDynamic(this, &AGTAWorldPlayerController::HandlePawnDamage);
		CachedPawnForDamage = InPawn;
	}
}

void AGTAWorldPlayerController::OnUnPossess()
{
	if (CachedPawnForDamage)
	{
		CachedPawnForDamage->OnTakeAnyDamage.RemoveDynamic(this, &AGTAWorldPlayerController::HandlePawnDamage);
		CachedPawnForDamage = nullptr;
	}
	Super::OnUnPossess();
}

void AGTAWorldPlayerController::TryEnterNearbyVehicle(AGTAWorldCharacter* FromCharacter, float TraceDistance)
{
	if (!FromCharacter || !GetWorld())
	{
		return;
	}

	UCameraComponent* Cam = FromCharacter->GetFollowCamera();
	if (!Cam)
	{
		return;
	}

	const FVector Start = Cam->GetComponentLocation();
	const FVector End = Start + Cam->GetForwardVector() * TraceDistance;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(VehicleInteract), false, FromCharacter);
	Params.AddIgnoredActor(FromCharacter);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		if (AGTADriveableVehicle* Vehicle = Cast<AGTADriveableVehicle>(Hit.GetActor()))
		{
			Vehicle->SetSavedOnFootCharacter(FromCharacter);
			Possess(Vehicle);
		}
	}
}

void AGTAWorldPlayerController::HandlePawnDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage > 0.f)
	{
		SmoothedDamagePerSecond = FMath::Lerp(SmoothedDamagePerSecond, Damage, 0.35f);
	}
}

void AGTAWorldPlayerController::PushWorldDirectorTelemetry()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UWorldDirectorSubsystem* Director = World->GetSubsystem<UWorldDirectorSubsystem>();
	if (!Director)
	{
		return;
	}

	DirectorAccumSeconds += GetWorld()->GetDeltaSeconds();
	if (DirectorAccumSeconds < 1.f)
	{
		return;
	}
	DirectorAccumSeconds = 0.f;

	FWorldDirectorMetrics Metrics;
	Metrics.DamageTakenPerMinute = SmoothedDamagePerSecond * 60.f;

	const float KillEw = KillMilestoneAccumulator;
	Metrics.PlayerKillsLastFiveMin = FMath::Clamp(KillEw, 0.f, 50.f);

	Director->PushPlayerMetricsSnapshot(Metrics);
}
