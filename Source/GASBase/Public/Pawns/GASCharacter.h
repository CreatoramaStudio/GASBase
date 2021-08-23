// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Data/GASEnums.h"
#include "GASCharacter.generated.h"

class UGameplayEffect;
class UGASGameplayAbility;
class UGASAbilitySystemComponent;
class UGASAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AGASCharacter*, Character);

UCLASS()
class GASBASE_API AGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "GASCharacter")
	FCharacterDiedDelegate OnCharacterDied;
	
protected:

	// Instead of TWeakObjectPtrs, you could just have UPROPERTY() hard references or no references at all and just call
	// GetAbilitySystem() and make a GetAttributeSetBase() that can read from the PlayerState or from child classes.
	// Just make sure you test if the pointer is valid before using.
	// I opted for TWeakObjectPtrs because I didn't want a shared hard reference here and I didn't want an extra function call of getting
	// the ASC/AttributeSet from the PlayerState or child classes every time I referenced them in this base class.

	TWeakObjectPtr<UGASAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UGASAttributeSet> AttributeSet;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASCharacter")
	FText CharacterName;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASCharacter|Abilities")
	TArray<TSubclassOf<UGASGameplayAbility>> CharacterAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASCharacter|Abilities")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASCharacter|Abilities")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	bool bAbilitySystemComponentInputBound = false;

private:

public:

	// Sets default values for this character's properties
	AGASCharacter(const FObjectInitializer& ObjectInitializer);

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "GASCharacter")
    virtual bool IsAlive() const;

	// Switch on AbilityID to return individual ability levels. Hardcoded to 1 for every ability in this project.
	UFUNCTION(BlueprintCallable, Category = "GASCharacter")
    virtual int32 GetAbilityLevel(EGASAbilityInputID AbilityID) const;

	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();

	/**
	* Getters for attributes from PPAttributeSetBase
	**/
	
	UFUNCTION(BlueprintCallable, Category = "GASCharacter|Attributes")
    int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "GASCharacter|Attributes")
    float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GASCharacter|Attributes")
    float GetMaxHealth() const;
	
	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "GASCharacter|Attributes")
    float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "GASCharacter|Attributes")
    float GetMoveSpeedBaseValue() const;

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "GASCharacter")
    virtual void FinishDying();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	virtual void AddStartupEffects();


	/**
	* Setters for Attributes. Only use these in special cases like Respawning, otherwise use a GE to change Attributes.
	* These change the Attribute's Base Value.
	*/
	virtual void SetHealth(float Health);


	// Client only
	virtual void OnRep_PlayerState() override;

	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput();
private:
	
};
