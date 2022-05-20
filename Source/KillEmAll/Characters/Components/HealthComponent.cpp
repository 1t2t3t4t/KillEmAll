// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;
}

void UHealthComponent::TakeDamage(const float Damage)
{
	const float AdjustedDamage = Damage > Health ? Health : Damage;
	Health -= AdjustedDamage;
	if (Health <= 0)
	{
		OnDead.Execute();
	}
}
