// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/AbilitySystem/GASAbilitySystemComponent.h"

#include "Pawns/AbilitySystem/Abilities/GASGameplayAbility.h"

UGASAbilitySystemComponent::UGASAbilitySystemComponent()
{
}

void UGASAbilitySystemComponent::ReceiveDamage(UGASAbilitySystemComponent* Source, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(Source, UnmitigatedDamage, MitigatedDamage);
}

void UGASAbilitySystemComponent::AbilityLocalInputPressed(int32 InputID)
{
	// Consume the input if this InputID is overloaded with GenericConfirm/Cancel and the GenericConfim/Cancel callback is bound
	if (IsGenericConfirmInputBound(InputID))
	{
		LocalInputConfirm();
		return;
	}

	if (IsGenericCancelInputBound(InputID))
	{
		LocalInputCancel();
		return;
	}

	// ---------------------------------------------------------

	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.InputID == InputID)
		{
			if (Spec.Ability)
			{
				Spec.InputPressed = true;
				if (Spec.IsActive())
				{
					if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
					{
						ServerSetInputPressed(Spec.Handle);
					}

					AbilitySpecInputPressed(Spec);

					// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
				}
				else
				{
					if (const UGASGameplayAbility* GameplayAbility = Cast<UGASGameplayAbility>(Spec.Ability))
					{
						if (GameplayAbility->bActivateOnInput)
						{
							// Ability is not active, so try to activate it
							TryActivateAbility(Spec.Handle);
						}
					}
				}
			}
		}
	}
}
