// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AttackAnimData.generated.h"

/**
 * 
 */
UCLASS()
class KILLEMALL_API UAttackAnimData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UAnimMontage> Montage;

	UPROPERTY(EditDefaultsOnly)
	bool bLockMovement = false;

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> AttackSocketNames;
};
