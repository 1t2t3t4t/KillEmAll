// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/MovementComponent.h"
#include "KillEmAll/Utils.h"
#include "Kismet/GameplayStatics.h"

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner())
	{
		if (USkeletalMeshComponent* Comp = Cast<USkeletalMeshComponent>(
			GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass())))
		{
			SkeletalMeshComponent = Comp;
			AnimInstance = Utils::NullCheck(SkeletalMeshComponent->GetAnimInstance());
			AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &UAttackComponent::OnMontageEnded);
		}
		if (UMovementComponent* Comp = Cast<UMovementComponent>(
			GetOwner()->GetComponentByClass(UMovementComponent::StaticClass())))
		{
			MovementComponent = Comp;
		}
	}
}

void UAttackComponent::ResetAttackState()
{
	GetWorld()->GetTimerManager().ClearTimer(SequenceCancelHandler);
	CurrentAtkIdx = 0;
	bQueueNext = false;
	bIsPlaying = false;
	bIsAttacking = false;
	if (MovementComponent)
	{
		MovementComponent->SetActive(true);
	}
}

void UAttackComponent::AddAttackSocket(const FName Name)
{
	AttackSocketNames.Push(Name);
}

void UAttackComponent::Attack()
{
	GetWorld()->GetTimerManager().ClearTimer(SequenceCancelHandler);
	GetWorld()->GetTimerManager().SetTimer(SequenceCancelHandler, this, &UAttackComponent::CancelQueueSequence,
	                                       ComboCancelTime);

	if (bIsPlaying && !bQueueNext)
	{
		bQueueNext = true;
	}
	else if (!bIsPlaying)
	{
		bIsPlaying = true;
		const auto CurrentAttack = AttackSequence[CurrentAtkIdx];
		if (const auto Montage = Utils::NullCheck(CurrentAttack->Montage.LoadSynchronous()))
		{
			AnimInstance->Montage_Play(Montage);
			AttackSocketNames = CurrentAttack->AttackSocketNames;
			if (MovementComponent)
			{
				MovementComponent->SetActive(!CurrentAttack->bLockMovement);
			}
		}
	}
}

void UAttackComponent::OnAttackStart()
{
	bIsAttacking = true;
}

void UAttackComponent::OnAttackEnd()
{
	bIsAttacking = false;
	PrevAttackPointMap.Reset();
	HitActors.Reset();

	if (bQueueNext)
	{
		GetWorld()->GetTimerManager().ClearTimer(SequenceCancelHandler);
		bIsPlaying = false;
		bQueueNext = false;
		CurrentAtkIdx = (CurrentAtkIdx + 1) % AttackSequence.Num();
		Attack();
	}
}

void UAttackComponent::OnMontageEnded(UAnimMontage* Montage, bool bIsInterrupted)
{
	if (!bIsInterrupted)
	{
		bIsPlaying = false;
		ResetAttackState();
	}
}

void UAttackComponent::CancelQueueSequence()
{
	bQueueNext = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear queue"));
}

void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsAttacking && SkeletalMeshComponent)
	{
		for (auto Socket : AttackSocketNames)
		{
			if (!SkeletalMeshComponent->DoesSocketExist(Socket))
			{
				UE_LOG(LogTemp, Warning, TEXT("Socket %s does not exist."), *Socket.ToString());
				continue;
			}
			auto SocketLoc = SkeletalMeshComponent->GetSocketLocation(Socket);
			if (PrevAttackPointMap.Contains(Socket))
			{
				auto PrevPoint = PrevAttackPointMap[Socket];

				FHitResult Hit;

				FCollisionObjectQueryParams Params(ECC_Pawn);

				FCollisionQueryParams QueryParams;
				QueryParams.AddIgnoredActor(GetOwner());

				const auto bIsHit = GetWorld()->LineTraceSingleByObjectType(
					Hit, PrevPoint, SocketLoc, Params, QueryParams);
				DrawDebugLine(GetWorld(), PrevPoint, SocketLoc, FColor::Red, false, 5);

				if (bIsHit)
				{
					OnHit(Hit);
				}
			}

			PrevAttackPointMap.Add(Socket, SocketLoc);
		}
	}
}

void UAttackComponent::OnHit(const FHitResult& HitResult)
{
	if (AActor* HitActor = HitResult.GetActor())
	{
		if (HitActors.Contains(HitActor))
		{
			return;
		}
		HitActors.AddUnique(HitActor);
		const auto CurrentAttack = AttackSequence[CurrentAtkIdx];
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *HitActor->GetName());
		UGameplayStatics::ApplyDamage(HitActor, CurrentAttack->Damage, GetOwner()->GetInstigatorController(),
		                              GetOwner(), UDamageType::StaticClass());
	}
}
