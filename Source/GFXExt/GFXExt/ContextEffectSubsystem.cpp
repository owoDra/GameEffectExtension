// Copyright (C) 2023 owoDra


#include "ContextEffectSubsystem.h"

#include "ContextEffectLibrary.h"
#include "EffectDeveloperSettings.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ContextEffectSubsystem)


void UContextEffectSubsystem::SpawnContextEffect(
	const AActor* SpawningActor
	, USceneComponent* AttachToComponent
	, const FName AttachPoint
	, const FVector LocationOffset
	, const FRotator RotationOffset
	, FGameplayTag Effect
	, FGameplayTagContainer Contexts
	, TArray<UAudioComponent*>& OutAudio
	, TArray<UNiagaraComponent*>& OutNiagara
	, FVector VFXScale
	, float AudioVolume
	, float AudioPitch)
{
	// First determine if this Actor has a matching Set of Libraries

	if (auto* FoundEffectsLibrariesSet{ ActiveActorEffectsMap.Find(SpawningActor) })
	{
		const auto& LibrarySet{ FoundEffectsLibrariesSet->Set };

		TArray<USoundBase*> TotalSounds;
		TArray<UNiagaraSystem*> TotalNiagaraSystems;

		// Collect effects from ContextEffectLibrary

		for (const auto& EffectLibrary : LibrarySet)
		{
			// If the Effect Library is not valid, skip.

			if (!EffectLibrary)
			{
				continue;
			}

			// If not already loaded, load it.

			if (EffectLibrary->GetContextEffectLibraryLoadState() == EContextEffectLibraryLoadState::Unloaded)
			{
				EffectLibrary->LoadEffects();
			}

			// If already loaded, get the effect of the specified effect tag

			if (EffectLibrary->GetContextEffectLibraryLoadState() == EContextEffectLibraryLoadState::Loaded)
			{
				TArray<USoundBase*> Sounds;
				TArray<UNiagaraSystem*> NiagaraSystems;
				EffectLibrary->GetEffects(Effect, Contexts, Sounds, NiagaraSystems);

				// Append to accumulating array

				TotalSounds.Append(Sounds);
				TotalNiagaraSystems.Append(NiagaraSystems);
			}
		}

		// Spawn Sounds Attached, add Audio Component to List of ACs

		for (const auto& Sound : TotalSounds)
		{
			auto* AudioComponent
			{
				UGameplayStatics::SpawnSoundAttached(
					Sound,
					AttachToComponent,
					AttachPoint,
					LocationOffset,
					RotationOffset,
					EAttachLocation::KeepRelativeOffset,
					false,
					AudioVolume,
					AudioPitch,
					0.0f,
					nullptr,
					nullptr,
					true)
			};

			OutAudio.Add(AudioComponent);
		}

		// Spawn Niagara Systems Attached, add Niagara Component to List of NCs
		
		for (const auto& NiagaraSystem : TotalNiagaraSystems)
		{
			
			auto* NiagaraComponent
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(
					NiagaraSystem,
					AttachToComponent,
					AttachPoint,
					LocationOffset,
					RotationOffset,
					VFXScale,
					EAttachLocation::KeepRelativeOffset,
					true,
					ENCPoolMethod::None,
					true,
					true)
			};

			OutNiagara.Add(NiagaraComponent);
		}
	}
}


bool UContextEffectSubsystem::GetContextFromSurfaceType(TEnumAsByte<EPhysicalSurface> PhysicalSurface, FGameplayTag& Context)
{
	const auto* DevSettings{ GetDefault<UEffectDeveloperSettings>() };

	if (const auto* GameplayTagPtr{ DevSettings->SurfaceTypeToContextMap.Find(PhysicalSurface) })
	{
		Context = *GameplayTagPtr;
	}

	return Context.IsValid();
}

void UContextEffectSubsystem::LoadAndAddContextEffectLibraries(AActor* OwningActor, TSet<TSoftObjectPtr<UContextEffectLibrary>> ContextEffectLibraries)
{
	// Early out if Owning Actor is invalid or if the associated Libraries is 0 (or less)

	if (OwningActor == nullptr || ContextEffectLibraries.Num() <= 0)
	{
		return;
	}

	// Load Library Assets from Soft Obj refs

	FActorCntextEffectLibrariesEntry EffectLibrariesSet;

	for (const auto& ContextEffectSoftObj : ContextEffectLibraries)
	{
		if (auto* EffectLibrary{ ContextEffectSoftObj.LoadSynchronous() })
		{
			EffectLibrary->LoadEffects();

			EffectLibrariesSet.Set.Add(EffectLibrary);
		}
	}

	// Update Active Actor Effects Map

	ActiveActorEffectsMap.Emplace(OwningActor, EffectLibrariesSet);
}

void UContextEffectSubsystem::UnloadAndRemoveContextEffectLibraries(AActor* OwningActor)
{
	// Early out if Owning Actor is invalid

	if (!OwningActor)
	{
		return;
	}

	// Remove ref from Active Actor/Effects Set Map

	ActiveActorEffectsMap.Remove(OwningActor);
}
