// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/GASCharacterMovementComponent.h"

#include "LogGASBase.h"
#include "Pawns/GASCharacter.h"

UGASCharacterMovementComponent::UGASCharacterMovementComponent()
{
}

float UGASCharacterMovementComponent::GetMaxSpeed() const
{
	AGASCharacter* Owner = Cast<AGASCharacter>(GetOwner());
	
	if (!Owner)
	{
		FLogGASBase::Warning(FString(__FUNCTION__) + " No Character Base Owner");
		return Super::GetMaxSpeed();
	}

	if (!Owner->IsAlive())
	{
		return 0.0f;
	}

	return Owner->GetMoveSpeed();
}
