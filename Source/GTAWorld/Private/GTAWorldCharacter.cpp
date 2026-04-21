#include "GTAWorldCharacter.h"

#include "GTAWorldPlayerController.h"
#include "GTADriveableVehicle.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

AGTAWorldCharacter::AGTAWorldCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 320.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	JumpMaxCount = 2;
}

void AGTAWorldCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGTAWorldCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGTAWorldCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AGTAWorldCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AGTAWorldCharacter::LookUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGTAWorldCharacter::OnJump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGTAWorldCharacter::OnFire);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AGTAWorldCharacter::OnInteract);
}

void AGTAWorldCharacter::MoveForward(float Value)
{
	if (!Controller || FMath::IsNearlyZero(Value))
	{
		return;
	}

	const FRotator ControlRot = Controller->GetControlRotation();
	const FRotator YawRot(0, ControlRot.Yaw, 0);
	const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	AddMovementInput(Forward, Value);
}

void AGTAWorldCharacter::MoveRight(float Value)
{
	if (!Controller || FMath::IsNearlyZero(Value))
	{
		return;
	}

	const FRotator ControlRot = Controller->GetControlRotation();
	const FRotator YawRot(0, ControlRot.Yaw, 0);
	const FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
	AddMovementInput(Right, Value);
}

void AGTAWorldCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AGTAWorldCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AGTAWorldCharacter::OnJump()
{
	Jump();
}

void AGTAWorldCharacter::OnFire()
{
	AController* MyController = GetController();
	if (!FollowCamera || !MyController)
	{
		return;
	}

	const FVector Start = FollowCamera->GetComponentLocation();
	const FVector Dir = FollowCamera->GetForwardVector();
	const FVector End = Start + Dir * WeaponRange;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(PlayerFire), false, this);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	if (Hit.bBlockingHit)
	{
		const FVector ShotDir = (Hit.ImpactPoint - Start).GetSafeNormal();
		if (AActor* Target = Hit.GetActor())
		{
			UGameplayStatics::ApplyPointDamage(Target, WeaponDamage, ShotDir, Hit, MyController, this, UDamageType::StaticClass());
		}
	}
}

void AGTAWorldCharacter::OnInteract()
{
	if (AGTAWorldPlayerController* PC = Cast<AGTAWorldPlayerController>(GetController()))
	{
		PC->TryEnterNearbyVehicle(this, InteractTraceDistance);
	}
}
