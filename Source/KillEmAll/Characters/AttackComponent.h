// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KillEmAll/AnimData/AttackAnimData.h"
#include "AttackComponent.generated.h"

UCLASS(ClassGroup=(Gameplay), meta=(BlueprintSpawnableComponent))
class KILLEMALL_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	TMap<FName, FVector> PrevAttackPointMap;

	UPROPERTY(VisibleAnywhere)
	bool bIsAttacking = false; // For line tracing

	UPROPERTY(EditDefaultsOnly)
	TArray<UAttackAnimData*> AttackSequence;

	UPROPERTY(EditDefaultsOnly)
	float ComboCancelTime = 0.1;

	UPROPERTY()
	TArray<FName> AttackSocketNames;

	UPROPERTY()
	UAnimInstance* AnimInstance;

	UPROPERTY()
	class UMovementComponent* MovementComponent;
	
	uint32 CurrentAtkIdx = 0;

	// For attack sequence playing
	bool bIsPlaying = false;
	bool bQueueNext = false;

	FTimerHandle SequenceCancelHandler;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;
	
	// Sets default values for this component's properties
	UAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void ResetAttackState();

public:
	void AddAttackSocket(const FName Name);
	void Attack();
	
	UFUNCTION(BlueprintCallable)
	virtual void OnAttackStart();
	
	UFUNCTION(BlueprintCallable)
	virtual void OnAttackEnd();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void OnHit(const FHitResult& HitResult);

private:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bIsInterrupted);

	UFUNCTION()
	void CancelQueueSequence();
};
