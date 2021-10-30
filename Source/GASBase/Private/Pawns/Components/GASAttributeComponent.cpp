// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Components/GASAttributeComponent.h"

#include "LogGASBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Pawns/AbilitySystem/GASAttributeSet.h"
#include "Pawns/Components/GASAbilityComponent.h"

// Sets default values for this component's properties
UGASAttributeComponent::UGASAttributeComponent()
{
	// Cache tags
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));
}

bool UGASAttributeComponent::IsAlive() const
{
	return GetHealth() > 0.0f;
}

int32 UGASAttributeComponent::GetCharacterLevel() const
{
	if (AttributeSet)
	{
		return static_cast<int32>(AttributeSet->GetCharacterLevel());
	}

	return 0;
}

float UGASAttributeComponent::GetHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetHealth();
	}

	return 0.0f;
}

float UGASAttributeComponent::GetMaxHealth() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

float UGASAttributeComponent::GetMoveSpeed() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMoveSpeed();
	}

	return 0.0f;
}

float UGASAttributeComponent::GetMoveSpeedBaseValue() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSet)->GetBaseValue();
	}

	return 0.0f;
}

void UGASAttributeComponent::Die()
{
	if (GASAbilityComponent)
	{
		// Only runs on Server
		GASAbilityComponent->RemoveCharacterAbilities();

		if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Character->GetCharacterMovement()->GravityScale = 0;
			Character->GetCharacterMovement()->Velocity = FVector(0);
		}
		
	
		if (GASAbilityComponent->GetAbilitySystemComponent())
		{
			GASAbilityComponent->GetAbilitySystemComponent()->CancelAllAbilities();

			FGameplayTagContainer EffectTagsToRemove;
			EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
			int32 NumEffectsRemoved = GASAbilityComponent->GetAbilitySystemComponent()->RemoveActiveEffectsWithTags(EffectTagsToRemove);

			GASAbilityComponent->GetAbilitySystemComponent()->AddLooseGameplayTag(DeadTag);
		}
	}

	OnPawnDied.Broadcast(Cast<APawn>(GetOwner()));
	
	FinishDying();
}

void UGASAttributeComponent::FinishDying()
{
	GetOwner()->Destroy();
}


// Called when the game starts
void UGASAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	GASAbilityComponent = Cast<UGASAbilityComponent>(GetOwner()->GetComponentByClass(UGASAbilityComponent::StaticClass()));
	
}

void UGASAttributeComponent::InitializeAttributes()
{
	if (!GASAbilityComponent->GetAbilitySystemComponent())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		FLogGASBase::Error(FString(__FUNCTION__) +"() Missing DefaultAttributes for " + GetName() + ". Please fill in the character's Blueprint.");
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = GASAbilityComponent->GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle NewHandle = GASAbilityComponent->GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = GASAbilityComponent->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), GASAbilityComponent->GetAbilitySystemComponent());
	}
}

void UGASAttributeComponent::SetHealth(float Health)
{
	if (AttributeSet)
	{
		AttributeSet->SetHealth(Health);
	}
}

void UGASAttributeComponent::ResetHealth()
{
	SetHealth(GetMaxHealth());
}

