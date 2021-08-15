// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Pawns/AbilitySystem/GASAttributeSet.h"
#include "DamageExecutionCalculation.generated.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct PPDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	PPDamageStatics()
	{
		// Snapshot happens at time of GESpec creation

		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.

		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASAttributeSet, Damage, Source, true);
	}
};

static const PPDamageStatics& DamageStatics()
{
	static PPDamageStatics DStatics;
	return DStatics;
}

/**
* 
*/
UCLASS()
class GASBASE_API UDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	public:

	protected:

	private:

	public:
	
	UDamageExecutionCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	protected:

	private:
};
