// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/GASCharacter.h"

#include "LogGASBase.h"
#include "AI/GASAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Pawns/GASCharacterMovementComponent.h"
#include "Pawns/AbilitySystem/GASAbilitySystemComponent.h"
#include "Pawns/AbilitySystem/GASAttributeSet.h"
#include "Pawns/AbilitySystem/Abilities/GASGameplayAbility.h"
#include "Player/GASPlayerController.h"
#include "Player/GASPlayerState.h"

// Sets default values
AGASCharacter::AGASCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UGASCharacterMovementComponent>(CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	// Makes sure that the animations play on the Server so that we can use bone and socket transforms
	// to do things like spawning projectiles and other FX.
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));
	
	bAlwaysRelevant = true;

	AIControllerClass = AGASAIController::StaticClass();

	// Cache tags
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

UAbilitySystemComponent * AGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool AGASCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}

int32 AGASCharacter::GetAbilityLevel(EGASAbilityInputID AbilityID) const
{
	return 1;
}

void AGASCharacter::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->CharacterAbilitiesGiven)
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

int32 AGASCharacter::GetCharacterLevel() const
{
	if (AttributeSet.IsValid())
	{
		return static_cast<int32>(AttributeSet->GetCharacterLevel());
	}

	return 0;
}

float AGASCharacter::GetHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}

	return 0.0f;
}

float AGASCharacter::GetMaxHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

float AGASCharacter::GetMoveSpeed() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMoveSpeed();
	}

	return 0.0f;
}

float AGASCharacter::GetMoveSpeedBaseValue() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSet.Get())->GetBaseValue();
	}

	return 0.0f;
}

// Run on Server and all clients
void AGASCharacter::Die()
{
	// Only runs on Server
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAllAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	
	FinishDying();
}

void AGASCharacter::FinishDying()
{
	Destroy();
}

// Called when the game starts or when spawned
void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGASCharacter::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UGASGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID), static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

void AGASCharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		FLogGASBase::Error(FString(__FUNCTION__) +"() Missing DefaultAttributes for " + GetName() + ". Please fill in the character's Blueprint.");
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void AGASCharacter::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (const TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->StartupEffectsApplied = true;
}

void AGASCharacter::SetHealth(float Health)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetHealth(Health);
	}
}

void AGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (AGASPlayerState* GASPlayerState = GetPlayerState<AGASPlayerState>())
	{
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UGASAbilitySystemComponent>(GASPlayerState->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(GASPlayerState, this);

		// Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
		BindASCInput();

		// Set the AttributeSetBase for convenience attribute functions
		AttributeSet = GASPlayerState->GetAttributeSet();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();

		if (AGASPlayerController* PlayerController = Cast<AGASPlayerController>(GetController()))
		{
			
		}

		// Simulated on proxies don't have their PlayerStates yet when BeginPlay is called so we call it again here


		// Respawn specific things that won't affect first possession.

		// Forcibly set the DeadTag count to 0
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());
	}
}

void AGASCharacter::BindASCInput()
{
	if (!bAbilitySystemComponentInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("EGASAbilityInputID"), static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel)));

		bAbilitySystemComponentInputBound = true;
	}
}


