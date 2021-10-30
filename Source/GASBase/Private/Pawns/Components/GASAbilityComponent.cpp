// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Components/GASAbilityComponent.h"

#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"
#include "Pawns/AbilitySystem/GASAbilitySystemComponent.h"
#include "Pawns/AbilitySystem/Abilities/GASGameplayAbility.h"
#include "Pawns/Components/GASAttributeComponent.h"
#include "Player/GASPlayerController.h"
#include "Player/GASPlayerState.h"

// Sets default values for this component's properties
UGASAbilityComponent::UGASAbilityComponent()
{
	
}

UAbilitySystemComponent* UGASAbilityComponent::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

TArray<UGASGameplayAbility*> UGASAbilityComponent::GetAbilities()
{
	TArray<UGASGameplayAbility*> Abilities;
	
	if (GetOwner()->GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || !AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return Abilities;
	}

	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if (Spec.SourceObject == this && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			if (UGASGameplayAbility* Ability = Cast<UGASGameplayAbility>(Spec.Ability))
			{
				Abilities.Add(Ability);
			}
		}
	}

	return Abilities;
}

int32 UGASAbilityComponent::GetAbilityLevel(EGASAbilityInputID AbilityID) const
{
	return 1;
}

void UGASAbilityComponent::RemoveCharacterAbilities()
{
	if (GetOwner()->GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || !AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if (Spec.SourceObject == this && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = false;
}


// Called when the game starts
void UGASAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	GASAttributeComponent = Cast<UGASAttributeComponent>(GetOwner()->GetComponentByClass(UGASAttributeComponent::StaticClass()));
	
}

void UGASAbilityComponent::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetOwner()->GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UGASGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID), static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

void UGASAbilityComponent::AddStartupEffects()
{
	if (GetOwner()->GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (const TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		if (GASAttributeComponent)
		{
			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GASAttributeComponent->GetCharacterLevel(), EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
			}
		}
	}

	AbilitySystemComponent->StartupEffectsApplied = true;
}

void UGASAbilityComponent::OnPlayerState(APawn* Pawn)
{
	if (AGASPlayerState* GASPlayerState = Pawn->GetPlayerState<AGASPlayerState>())
	{
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UGASAbilitySystemComponent>(GASPlayerState->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(GASPlayerState, GetOwner());

		// Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
		BindASCInput(Pawn);

		if (GASAttributeComponent)
		{
			// Set the AttributeSetBase for convenience attribute functions
			GASAttributeComponent->AttributeSet = GASPlayerState->GetAttributeSet();

			// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
			// For now assume possession = spawn/respawn.
			GASAttributeComponent->InitializeAttributes();

			// Forcibly set the DeadTag count to 0
			AbilitySystemComponent->SetTagMapCount(GASAttributeComponent->DeadTag, 0);

			// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
			GASAttributeComponent->ResetHealth();
		}

		if (AGASPlayerController* PlayerController = Cast<AGASPlayerController>(Pawn->GetController()))
		{
			
		}

		// Simulated on proxies don't have their PlayerStates yet when BeginPlay is called so we call it again here


		// Respawn specific things that won't affect first possession.

		
	}
}

void UGASAbilityComponent::BindASCInput(const APawn* Pawn)
{
	if (!bAbilitySystemComponentInputBound && AbilitySystemComponent && IsValid(Pawn->InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(Pawn->InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("EGASAbilityInputID"), static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel)));

		bAbilitySystemComponentInputBound = true;
	}
}

