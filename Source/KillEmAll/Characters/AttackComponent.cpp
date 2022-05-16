// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/MovementComponent.h"
#include "KillEmAll/Utils.h"

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
	if (bIsPlaying && !bQueueNext)
	{
		bQueueNext = true;
		CurrentAtkIdx = (CurrentAtkIdx + 1) % AttackSequence.Num();
	}
	else if (!bIsPlaying)
	{
		bIsPlaying = true;
		const auto CurrentAttack = AttackSequence[CurrentAtkIdx];
		if (const auto Montage = Utils::NullCheck(CurrentAttack->Montage.Get()))
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
}

void UAttackComponent::OnMontageEnded(UAnimMontage* Montage, bool bIsInterrupted)
{
	bIsPlaying = false;
	if (bQueueNext)
	{
		bQueueNext = false;
		Attack();
	}
	else
	{
		ResetAttackState();
	}
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
					UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *Hit.GetActor()->GetName());
				}
			}

			PrevAttackPointMap.Add(Socket, SocketLoc);
		}
	}
}
