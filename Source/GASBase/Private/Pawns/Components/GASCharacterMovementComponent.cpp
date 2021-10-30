// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Components/GASCharacterMovementComponent.h"

#include "LogGASBase.h"
#include "Pawns/GASCharacter.h"
#include "Pawns/Components/GASAttributeComponent.h"

UGASCharacterMovementComponent::UGASCharacterMovementComponent()
{
}

float UGASCharacterMovementComponent::GetMaxSpeed() const
{
	if (const UGASAttributeComponent* GASAttributeComponent = Cast<UGASAttributeComponent>(GetOwner()->GetComponentByClass(UGASAttributeComponent::StaticClass())))
	{
		if (!GASAttributeComponent->IsAlive())
		{
			return 0.0f;
		}

		return GASAttributeComponent->GetMoveSpeed();
	}

	FLogGASBase::Warning(FString(__FUNCTION__) + " GASAttributeComponent not found");
	return Super::GetMaxSpeed();
}
