// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterController.generated.h"

UCLASS()
class KILLEMALL_API ACharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACharacterController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
