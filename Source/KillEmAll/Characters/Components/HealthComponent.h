// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class KILLEMALL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	DECLARE_DELEGATE(FOnDead);
	
	UPROPERTY(EditDefaultsOnly)
	float Health = 100.f;

public:
	FOnDead OnDead;
	
	UHealthComponent();

	float GetHealth() const
	{
		return Health;
	}
	
	void TakeDamage(const float Damage);
};
