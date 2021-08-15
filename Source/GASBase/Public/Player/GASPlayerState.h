// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GASPlayerState.generated.h"

class UGASAttributeSet;
class UGASAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class GASBASE_API AGASPlayerState : public APlayerState , public IAbilitySystemInterface
{
	GENERATED_BODY()
	public:

protected:

	/** Ability System Component. Required to use Gameplay Attributes and Gameplay Abilities. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	UGASAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UGASAttributeSet* AttributeSet;
	
	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle HealthRegenRateChangedDelegateHandle;
	FDelegateHandle XPChangedDelegateHandle;
	FDelegateHandle GoldChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;
	
private:

public:

	// Sets default values for this PlayerState properties
	AGASPlayerState();

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UGASAttributeSet* GetAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = "GASPlayerState|Attributes")
    bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "GASPlayerState|Attributes")
    float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GASPlayerState|Attributes")
    float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GASPlayerState|Attributes")
    float GetHealthRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "GASPlayerState|Attributes")
    float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "GASPlayerState|Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "GASPlayerState|Attributes")
	int32 GetXP() const;

	UFUNCTION(BlueprintCallable, Category = "GASPlayerState|Attributes")
	int32 GetXPBounty() const;

	UFUNCTION(BlueprintCallable, Category = "GASPlayerState|Attributes")
	int32 GetGold() const;

	UFUNCTION(BlueprintCallable, Category = "GASPlayerState|Attributes")
	int32 GetGoldBounty() const;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void HealthRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void XPChanged(const FOnAttributeChangeData& Data);
	virtual void GoldChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);

	// Tag change callbacks
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
private:
};
