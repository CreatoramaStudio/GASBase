// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "GASAttributeComponent.generated.h"

class UGameplayEffect;
class UGASGameplayAbility;
class UGASAbilitySystemComponent;
class UGASAttributeSet;
class UGASAbilityComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPawnDiedDelegate, APawn*, Pawn);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASBASE_API UGASAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "GASAttributeComponent")
	FPawnDiedDelegate OnPawnDied;

	UPROPERTY()
	UGASAbilityComponent* GASAbilityComponent;

	UPROPERTY()
	UGASAttributeSet* AttributeSet;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASAttributeComponent")
	FText CharacterName;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASAttributeComponent")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

protected:

private:

public:

	// Sets default values for this component's properties
	UGASAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "GASAttributeComponent")
	virtual bool IsAlive() const;

	/**
	* Getters for attributes from PPAttributeSetBase
	**/
	
	UFUNCTION(BlueprintCallable, Category = "GASAttributeComponent")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "GASAttributeComponent")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GASAttributeComponent")
	float GetMaxHealth() const;
	
	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "GASAttributeComponent")
	float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "GASAttributeComponent")
	float GetMoveSpeedBaseValue() const;

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "GASAttributeComponent")
	virtual void FinishDying();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	/**
	* Setters for Attributes. Only use these in special cases like Respawning, otherwise use a GE to change Attributes.
	* These change the Attribute's Base Value.
	*/
	virtual void SetHealth(float Health);

	virtual void ResetHealth();

protected:

private:
	
};
