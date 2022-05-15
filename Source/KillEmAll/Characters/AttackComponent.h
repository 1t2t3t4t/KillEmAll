// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

UCLASS(ClassGroup=(Gameplay), meta=(BlueprintSpawnableComponent))
class KILLEMALL_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	TMap<FName, FVector> PrevAttackPointMap;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> AttackSocketNames;

	UPROPERTY(VisibleAnywhere)
	bool bIsAttacking = false;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;
	
	// Sets default values for this component's properties
	UAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void AddAttackSocket(const FName Name);
	
	UFUNCTION(BlueprintCallable)
	virtual void OnAttackStart();
	
	UFUNCTION(BlueprintCallable)
	virtual void OnAttackEnd();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
