// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/AbilitySystem/GASAbilitySystemComponent.h"

UGASAbilitySystemComponent::UGASAbilitySystemComponent()
{
}

void UGASAbilitySystemComponent::ReceiveDamage(UGASAbilitySystemComponent* Source, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(Source, UnmitigatedDamage, MitigatedDamage);
}
