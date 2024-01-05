// Copyright (C) 2023 owoDra

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "GameplayTagContainer.h"

#include "ContextEffectSubsystem.generated.h"

enum EPhysicalSurface : int;
class UContextEffectLibrary;
class UNiagaraComponent;
class UAudioComponent;
class USceneComponent;


/**
 * List data of registered ContextEffectLibrary for an actor
 */
USTRUCT()
struct GFXEXT_API FActorCntextEffectLibrariesEntry
{
	GENERATED_BODY()
public:
	FActorCntextEffectLibrariesEntry() {}

public:
	UPROPERTY(EditAnywhere)
	TSet<TObjectPtr<UContextEffectLibrary>> Set;

};


/**
 * Subsystem class that manages ContextEffectLibrary for each actor
 * 
 * Tips:
 *	This makes it possible to generate different effects for each actor for the same EffectTag.
 */
UCLASS()
class GFXEXT_API UContextEffectSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UContextEffectSubsystem() {}

protected:
	//
	// List data of registered ContextEffectLibrary for each actor
	//
	UPROPERTY(Transient)
	TMap<TObjectPtr<AActor>, FActorCntextEffectLibrariesEntry> ActiveActorEffectsMap;
	
public:
	UFUNCTION(BlueprintCallable, Category = "ContextEffect")
	void SpawnContextEffect(
		const AActor* SpawningActor
		, USceneComponent* AttachToComponent
		, const FName AttachPoint
		, const FVector LocationOffset
		, const FRotator RotationOffset
		, FGameplayTag Effect
		, FGameplayTagContainer Contexts
		, TArray<UAudioComponent*>& OutAudio
		, TArray<UNiagaraComponent*>& OutNiagara
		, FVector VFXScale = FVector(1)
		, float AudioVolume = 1
		, float AudioPitch = 1);

	UFUNCTION(BlueprintCallable, Category = "ContextEffect")
	bool GetContextFromSurfaceType(TEnumAsByte<EPhysicalSurface> PhysicalSurface, FGameplayTag& Context);

	UFUNCTION(BlueprintCallable, Category = "ContextEffect")
	void LoadAndAddContextEffectLibraries(AActor* OwningActor, TSet<TSoftObjectPtr<UContextEffectLibrary>> ContextEffectLibraries);

	UFUNCTION(BlueprintCallable, Category = "ContextEffect")
	void UnloadAndRemoveContextEffectLibraries(AActor* OwningActor);

};
