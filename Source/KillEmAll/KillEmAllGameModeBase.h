// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KillEmAllGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class KILLEMALL_API AKillEmAllGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WinWidgetClass;

	UPROPERTY(VisibleAnywhere)
	UUserWidget* WinWidget;
	
	UFUNCTION(BlueprintCallable)
	void EnteredWinTrigger();
};
