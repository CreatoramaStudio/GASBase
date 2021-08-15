// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GASCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASBASE_API UGASCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	public:

	protected:

	private:

	public:
	
	UGASCharacterMovementComponent();
	
	virtual float GetMaxSpeed() const override;

	protected:

	private:
};
