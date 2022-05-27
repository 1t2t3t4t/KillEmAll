// Copyright Epic Games, Inc. All Rights Reserved.


#include "KillEmAllGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AKillEmAllGameModeBase::EnteredWinTrigger()
{
	const auto Controller = UGameplayStatics::GetPlayerController(this, 0);
	Controller->GetPawn()->DisableInput(Controller);

	Controller->SetShowMouseCursor(true);
	WinWidget = CreateWidget(Controller, WinWidgetClass);
	WinWidget->AddToViewport();
}
