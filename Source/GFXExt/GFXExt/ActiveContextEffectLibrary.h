// Copyright (C) 2024 owoDra

#pragma once

#include "GameplayTagContainer.h"

#include "ActiveContextEffectLibrary.generated.h"

class USoundBase;
class UNiagaraSystem;
class UAudioComponent;
class UNiagaraComponent;
class UContextEffectLibrary;

/**
 * Load status of effects defined in ContextEffectLibrary
 */
UENUM()
enum class EContextEffectsLoadState : uint8
{
	Unloaded = 0,
	Loading = 1,
	Loaded = 2
};


/**
 * Data for Loaded ContextEffect to be registered in ActiveContextEffectLibrary
 */
USTRUCT()
struct FActiveContextEffect
{
	GENERATED_BODY()

	friend class UActiveContextEffectLibrary;

public:
	FActiveContextEffect() {}

public:
	UPROPERTY(VisibleAnywhere)
	FGameplayTagContainer Context;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<USoundBase>> Sounds;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UNiagaraSystem>> NiagaraSystems;

private:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UAudioComponent>> ActiveAudioComponents;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UNiagaraComponent>> ActiveNiagaraComponents;

};


/**
 * Class for loading and managing data defined in ContextEffectLibrary
 */
UCLASS(Transient)
class GFXEXT_API UActiveContextEffectLibrary : public UObject
{
	GENERATED_BODY()
public:
	UActiveContextEffectLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(Transient)
	EContextEffectsLoadState LoadState{ EContextEffectsLoadState::Unloaded };

	UPROPERTY(Transient)
	TArray<FActiveContextEffect> ActiveContextEffects;

public:
	virtual void LoadContextEffectLibrary(const UContextEffectLibrary* Library);
	virtual void UnloadContextEffectLibrary();

protected:
	virtual void OnContextEffectsLoaded(const TArray<FActiveContextEffect>& Effects);


public:
	virtual void PlayEffects(
		FGameplayTagContainer Contexts
		, USceneComponent* AttachToComponent	= nullptr
		, FName AttachPointName					= NAME_None
		, FVector LocationOffset				= FVector()
		, FRotator RotationOffset				= FRotator()
		, EAttachLocation::Type LocationType	= EAttachLocation::KeepRelativeOffset
		, float VolumeMultiplier				= 1.0f
		, float PitchMultiplier					= 1.0f
		, FVector VFXScale						= FVector(1)
	);

protected:
	FActiveContextEffect* FindEffectPtr(const FGameplayTagContainer& Contexts);

};
