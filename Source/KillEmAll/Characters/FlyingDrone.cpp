// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingDrone.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFlyingDrone::AFlyingDrone()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
}

// Called when the game starts or when spawned
void AFlyingDrone::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	FloatingPawnMovement->SetUpdatedComponent(RootComponent);
}

void AFlyingDrone::RecalibateRotation()
{
	if (bIsAdjusting)
	{
		return;
	}
	
	const FVector PlayerDir = PlayerPawn->GetActorLocation() - GetActorLocation();
	const float Distance = PlayerDir.Size();
	if (Distance <= ReAdjustRange)
	{
		bIsAdjusting = true;
		FTimerHandle AdjustHandle;
		GetWorld()->GetTimerManager().SetTimer(AdjustHandle, this, &AFlyingDrone::DisableAdjust, ReAdjustDuration);
	}
	const FRotator NewRot = FMath::Lerp(GetActorRotation(), PlayerDir.Rotation(), TurnRate * GetWorld()->GetDeltaSeconds());
	SetActorRotation(NewRot);
}

void AFlyingDrone::DisableAdjust()
{
	bIsAdjusting = false;
}

// Called every frame
void AFlyingDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RecalibateRotation();

	AddMovementInput(GetActorForwardVector(), FlyingSpeed);
}

// Called to bind functionality to input
void AFlyingDrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

