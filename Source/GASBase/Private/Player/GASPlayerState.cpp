// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GASPlayerState.h"

#include "Pawns/GASCharacter.h"
#include "Pawns/AbilitySystem/GASAbilitySystemComponent.h"
#include "Pawns/AbilitySystem/GASAttributeSet.h"

FName AGASPlayerState::AbilitySystemComponentName(TEXT("AbilitySystemComponent"));
FName AGASPlayerState::AttributeSetName(TEXT("AttributeSet"));

AGASPlayerState::AGASPlayerState(const FObjectInitializer& ObjectInitializer)
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>(AbilitySystemComponentName);
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another PPPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	AttributeSet = CreateDefaultSubobject<UGASAttributeSet>(AttributeSetName);

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
	NetUpdateFrequency = 100.0f;

	// Cache tags
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	
}

UAbilitySystemComponent* AGASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGASAttributeSet* AGASPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

bool AGASPlayerState::IsAlive() const
{
	return GetHealth() >= 0.0f;
}

float AGASPlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AGASPlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AGASPlayerState::GetHealthRegenRate() const
{
	return AttributeSet->GetHealthRegenRate();
}

float AGASPlayerState::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

int32 AGASPlayerState::GetCharacterLevel() const
{
	return AttributeSet->GetCharacterLevel();
}

int32 AGASPlayerState::GetXP() const
{
	return AttributeSet->GetXP();
}

int32 AGASPlayerState::GetXPBounty() const
{
	return AttributeSet->GetXPBounty();
}

int32 AGASPlayerState::GetMoney() const
{
	return AttributeSet->GetMoney();
}

int32 AGASPlayerState::GetMoneyBounty() const
{
	return AttributeSet->GetMoneyBounty();
}

void AGASPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		// Attribute change callbacks
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AGASPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &AGASPlayerState::MaxHealthChanged);
		HealthRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthRegenRateAttribute()).AddUObject(this, &AGASPlayerState::HealthRegenRateChanged);
		XPChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetXPAttribute()).AddUObject(this, &AGASPlayerState::XPChanged);
		MoneyChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMoneyAttribute()).AddUObject(this, &AGASPlayerState::MoneyChanged);
		CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCharacterLevelAttribute()).AddUObject(this, &AGASPlayerState::CharacterLevelChanged);

		// Tag change callbacks
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AGASPlayerState::StunTagChanged);
	}
}

void AGASPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;

	// Update the HUD
	// Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint

	// If the player died, handle death
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		if (AGASCharacter* Character = Cast<AGASCharacter>(GetPawn()))
		{
			Character->Die();
		}
	}
}

void AGASPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float MaxHealth = Data.NewValue;

}

void AGASPlayerState::HealthRegenRateChanged(const FOnAttributeChangeData& Data)
{
	float HealthRegenRate = Data.NewValue;

}

void AGASPlayerState::XPChanged(const FOnAttributeChangeData& Data)
{
	float XP = Data.NewValue;

}

void AGASPlayerState::MoneyChanged(const FOnAttributeChangeData& Data)
{
	float Money = Data.NewValue;

}

void AGASPlayerState::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
	float CharacterLevel = Data.NewValue;

}

void AGASPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}
