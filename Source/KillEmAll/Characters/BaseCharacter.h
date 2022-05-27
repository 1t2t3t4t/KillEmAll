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
	class UAttackComponent* AttackComponent;

	UPROPERTY(EditDefaultsOnly)
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HurtAnimMontage;

public:
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
	UFUNCTION(BlueprintCallable)
	virtual void Attack();
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
private:
	void MoveRight(float Value);
	void Jump();

	UFUNCTION()
	void OnDead();
};
