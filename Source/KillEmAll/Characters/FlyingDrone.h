// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlyingDrone.generated.h"

UCLASS()
class KILLEMALL_API AFlyingDrone : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	class UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY(VisibleAnywhere)
	APawn* PlayerPawn;

	UPROPERTY(EditDefaultsOnly)
	float FlyingSpeed = 15.f;

	UPROPERTY(EditDefaultsOnly)
	float TurnRate = 2.f;

	UPROPERTY(EditDefaultsOnly)
	float ReAdjustRange = 200.f;

	UPROPERTY(EditDefaultsOnly)
	float ReAdjustDuration = 3.f;

	bool bIsAdjusting = false;
	
public:
	// Sets default values for this pawn's properties
	AFlyingDrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void RecalibateRotation();
	void DisableAdjust();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
