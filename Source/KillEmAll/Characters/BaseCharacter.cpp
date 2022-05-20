// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Components/AttackComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackComponent = CreateDefaultSubobject<UAttackComponent>(TEXT("Attack Component"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMovementComponent()->SetPlaneConstraintEnabled(true);
	GetMovementComponent()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);

	HealthComponent->OnDead.BindUObject(this, &ABaseCharacter::OnDead);
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
	AttackComponent->Attack();
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	const auto Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(HurtAnimMontage);
	}

	if (HealthComponent)
	{
		HealthComponent->TakeDamage(DamageAmount);
	}
	
	return Result;
}

void ABaseCharacter::MoveRight(float Value)
{
	AddMovementInput(FVector::ForwardVector, Value * Speed);
}

void ABaseCharacter::Jump()
{
	Super::Jump();
}

void ABaseCharacter::OnDead()
{
	UE_LOG(LogTemp, Display, TEXT("Character %s dead"), *GetName());
	Destroy();
}

