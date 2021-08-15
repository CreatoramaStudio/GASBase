// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/AbilitySystem/GASAbilitySystemGlobals.h"

UGASAbilitySystemGlobals::UGASAbilitySystemGlobals()
{

}

void UGASAbilitySystemGlobals::InitGlobalTags()
{
	Super::InitGlobalTags();

	DeadTag = FGameplayTag::RequestGameplayTag("State.Dead");
	KnockedDownTag = FGameplayTag::RequestGameplayTag("State.KnockedDown");
}
