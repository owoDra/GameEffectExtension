// Copyright (C) 2023 owoDra

#pragma once

#include "GameplayTagContainer.h"

#include "ContextEffectLibrary.generated.h"

class UNiagaraSystem;
class USoundBase;


/**
 * Load status of ContextEffectLibrary
 */
UENUM()
enum class EContextEffectLibraryLoadState : uint8 
{
	Unloaded = 0,
	Loading = 1,
	Loaded = 2
};


/**
 * Effect tags and effect mapping data for ContextEffect to be registered in ContextEffectLibrary
 */
USTRUCT(BlueprintType)
struct GFXEXT_API FContextEffect
{
	GENERATED_BODY()
public:
	FContextEffect() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer Context;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowedClasses = "/Script/Engine.SoundBase, /Script/Niagara.NiagaraSystem"))
	TArray<FSoftObjectPath> Effects;

};


/**
 * Effect tag and effect object reference mapping data for ContextEffect registered in the loaded ContextEffectLibrary
 */
USTRUCT(BlueprintType)
struct GFXEXT_API FActiveContextEffect
{
	GENERATED_BODY()
public:
	FActiveContextEffect() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer Context;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<USoundBase>> Sounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UNiagaraSystem>> NiagaraSystems;

};


/**
 * Data class for linking, reading, and referencing effect tags and effects
 */
UCLASS(BlueprintType)
class GFXEXT_API UContextEffectLibrary : public UObject
{
	GENERATED_BODY()
public:
	UContextEffectLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Context Effect")
	TArray<FContextEffect> ContextEffects;

protected:
	UPROPERTY(Transient)
	TArray<FActiveContextEffect> ActiveContextEffects;

	UPROPERTY(Transient)
	EContextEffectLibraryLoadState EffectsLoadState{ EContextEffectLibraryLoadState::Unloaded };

public:
	UFUNCTION(BlueprintCallable)
	void GetEffects(
		const FGameplayTag Effect,
		const FGameplayTagContainer Context,
		TArray<USoundBase*>& OutSounds,
		TArray<UNiagaraSystem*>& OutNiagaraSystems);

	UFUNCTION(BlueprintCallable)
	void LoadEffects();

	EContextEffectLibraryLoadState GetContextEffectLibraryLoadState() const { return EffectsLoadState; }

protected:
	void LoadEffectsInternal();

	void ContextEffectLibraryLoadingComplete(const TArray<FActiveContextEffect>& NewActiveContextEffect);

};
