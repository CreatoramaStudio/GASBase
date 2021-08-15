// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GASPlayerController.h"

#include "AbilitySystemComponent.h"
#include "Player/GASPlayerState.h"

AGASPlayerController::AGASPlayerController()
{
	
}

void AGASPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AGASPlayerState* MyPlayerState = GetPlayerState<AGASPlayerState>())
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		MyPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MyPlayerState, InPawn);
	}
}