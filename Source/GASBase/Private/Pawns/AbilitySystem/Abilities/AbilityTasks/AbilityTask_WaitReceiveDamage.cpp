// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/AbilitySystem/Abilities/AbilityTasks/AbilityTask_WaitReceiveDamage.h"

#include "Pawns/AbilitySystem/GASAbilitySystemComponent.h"

UAbilityTask_WaitReceiveDamage::UAbilityTask_WaitReceiveDamage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TriggerOnce = false;
}

UAbilityTask_WaitReceiveDamage* UAbilityTask_WaitReceiveDamage::WaitReceiveDamage(UGameplayAbility* OwningAbility, bool InTriggerOnce)
{
	UAbilityTask_WaitReceiveDamage* MyObj = NewAbilityTask<UAbilityTask_WaitReceiveDamage>(OwningAbility);
	MyObj->TriggerOnce = InTriggerOnce;
	return MyObj;
}

void UAbilityTask_WaitReceiveDamage::Activate()
{
	if (UGASAbilitySystemComponent* GASASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent))
	{
		GASASC->ReceivedDamage.AddDynamic(this, &UAbilityTask_WaitReceiveDamage::OnDamageReceived);
	}
}

void UAbilityTask_WaitReceiveDamage::OnDestroy(bool AbilityIsEnding)
{

	if (UGASAbilitySystemComponent* GASASC = Cast<UGASAbilitySystemComponent>(AbilitySystemComponent))
	{
		GASASC->ReceivedDamage.RemoveDynamic(this, &UAbilityTask_WaitReceiveDamage::OnDamageReceived);
	}

	Super::OnDestroy(AbilityIsEnding);
}

void UAbilityTask_WaitReceiveDamage::OnDamageReceived(UGASAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
	}

	if (TriggerOnce)
	{
		EndTask();
	}
}
