// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GASAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UGASAbilitySystemComponent*, Source, float, UnmitigatedDamage, float, MitigatedDamage);

/**
* 
*/
UCLASS()
class GASBASE_API UGASAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:

	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamage;

protected:

private:

public:
	UGASAbilitySystemComponent();
	
	// Called from PPDamageExecCalculation. Broadcasts on ReceivedDamage whenever this ASC receives damage.
	virtual void ReceiveDamage(UGASAbilitySystemComponent* Source, float UnmitigatedDamage, float MitigatedDamage);

	virtual void AbilityLocalInputPressed(int32 InputID) override;

protected:

private:
};
