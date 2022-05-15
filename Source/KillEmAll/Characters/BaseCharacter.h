// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class KILLEMALL_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	float Speed = 100.f;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* PunchAnim;

	UPROPERTY(EditDefaultsOnly)
	class UAttackComponent* AttackComponent;

public:
	static FName LeftHandSocketName;
	
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void Attack();
	
private:
	void MoveRight(float Value);
	void Jump();
};
