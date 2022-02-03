// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Components/ActorComponent.h"
#include "Data/GASEnums.h"
#include "GASAbilityComponent.generated.h"

class UGameplayEffect;
class UGASGameplayAbility;
class UGASAbilitySystemComponent;
class UGASAttributeSet;
class UGASAttributeComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASBASE_API UGASAbilityComponent : public UActorComponent, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UGASAttributeComponent* GASAttributeComponent;

	UPROPERTY()
	UGASAbilitySystemComponent* AbilitySystemComponent;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASAbilityComponent")
	TArray<TSubclassOf<UGASGameplayAbility>> CharacterAbilities;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASAbilityComponent")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	bool bAbilitySystemComponentInputBound = false;

protected:

private:

public:

	// Sets default values for this component's properties
	UGASAbilityComponent();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = "GASAbilityComponent")
	virtual TArray<UGASGameplayAbility*> GetAbilities();

	// Switch on AbilityID to return individual ability levels. Hardcoded to 1 for every ability in this project.
	UFUNCTION(BlueprintCallable, Category = "GASAbilityComponent")
	virtual int32 GetAbilityLevel(EGASAbilityInputID AbilityID) const;

	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();
	
	// Called when the game starts
	virtual void BeginPlay() override;

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	virtual void AddStartupEffects();

	// Client only
	virtual void OnPlayerState(APawn* Pawn);

	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repeated to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repeated before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput(const APawn* Pawn,const FString InputEnumName = "EGASAbilityInputID",const FString ConfirmTargetInputName = "ConfirmTarget",const FString CancelTargetInputName = "CancelTarget",const int32 ConfirmTargetInputID = static_cast<int32>(EGASAbilityInputID::Confirm),const int32 CancelTargetInputID = static_cast<int32>(EGASAbilityInputID::Cancel));

protected:

private:	
};
