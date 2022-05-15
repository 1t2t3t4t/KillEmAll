// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "AttackComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

FName ABaseCharacter::LeftHandSocketName = TEXT("LeftHandSocket");

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack Component"));
	AttackComponent->AddAttackSocket(LeftHandSocketName);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMovementComponent()->SetPlaneConstraintEnabled(true);
	GetMovementComponent()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABaseCharacter::Attack);
}

void ABaseCharacter::Attack()
{
	GetMesh()->GetAnimInstance()->Montage_Play(PunchAnim);
}

void ABaseCharacter::MoveRight(float Value)
{
	AddMovementInput(FVector::ForwardVector, Value * Speed);
}

void ABaseCharacter::Jump()
{
	Super::Jump();
}

