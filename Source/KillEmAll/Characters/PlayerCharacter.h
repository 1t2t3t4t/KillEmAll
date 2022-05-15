// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class KILLEMALL_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* Camera;
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Attack() override;
};
