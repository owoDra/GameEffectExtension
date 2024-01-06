// Copyright (C) 2023 owoDra

#include "ActiveContextEffectLibrary.h"

#include "ContextEffectLibrary.h"

#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ActiveContextEffectLibrary)


UActiveContextEffectLibrary::UActiveContextEffectLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UActiveContextEffectLibrary::LoadContextEffectLibrary(const UContextEffectLibrary* Library)
{
	check(Library);
	check(LoadState == EContextEffectsLoadState::Unloaded);

	// Set load state to "Loading"

	LoadState = EContextEffectsLoadState::Loading;

	// Load context effects

	TArray<FActiveContextEffect> LoadedEffects;

	for (const auto& ContextEffect : Library->ContextEffects)
	{
		auto& NewActiveEffect{ LoadedEffects.AddDefaulted_GetRef() };
		NewActiveEffect.Context = ContextEffect.Context;
	
		// Load and add effect objects
			
		for (const auto& EffectObjectPath : ContextEffect.Effects)
		{
			auto* EffectObject{ EffectObjectPath.TryLoad() };
	
			if (auto* SoundObject{ Cast<USoundBase>(EffectObject) })
			{
				NewActiveEffect.Sounds.Add(SoundObject);
			}
			else if (auto* NiagaraObject{ Cast<UNiagaraSystem>(EffectObject) })
			{
				NewActiveEffect.NiagaraSystems.Add(NiagaraObject);
			}
		}
	}

	OnContextEffectsLoaded(LoadedEffects);
}

void UActiveContextEffectLibrary::UnloadContextEffectLibrary()
{
	for (auto& Effect : ActiveContextEffects)
	{
		for (const auto& AudioComponent : Effect.ActiveAudioComponents)
		{
			if (AudioComponent)
			{
				AudioComponent->Deactivate();
				AudioComponent->DestroyComponent();
			}
		}

		Effect.ActiveAudioComponents.Empty();

		for (const auto& NiagaraComponent : Effect.ActiveNiagaraComponents)
		{
			if (NiagaraComponent)
			{
				NiagaraComponent->Deactivate();
				NiagaraComponent->DestroyComponent();
			}
		}

		Effect.ActiveNiagaraComponents.Empty();
	}

	ActiveContextEffects.Empty();

	LoadState = EContextEffectsLoadState::Unloaded;
}

void UActiveContextEffectLibrary::OnContextEffectsLoaded(const TArray<FActiveContextEffect>& Effects)
{
	check(LoadState == EContextEffectsLoadState::Loading);

	LoadState = EContextEffectsLoadState::Loaded;

	ActiveContextEffects = Effects;
}


void UActiveContextEffectLibrary::PlayEffects(
	FGameplayTagContainer Contexts
	, USceneComponent* AttachToComponent
	, FName AttachPointName
	, FVector LocationOffset
	, FRotator RotationOffset
	, EAttachLocation::Type LocationType
	, float VolumeMultiplier
	, float PitchMultiplier
	, FVector VFXScale)
{
	if (!ensure(LoadState == EContextEffectsLoadState::Loaded))
	{
		return;
	}

	// Find ContextEffect

	if (auto* FoundEffect{ FindEffectPtr(Contexts) })
	{
		int32 Index{ 0 };

		// Play Sound. If there is already an AudioComponent, reuse it; if not, create a new one.

		const auto& Sounds{ FoundEffect->Sounds };
		auto& AudioComponents{ FoundEffect->ActiveAudioComponents };

		for (const auto& Sound : Sounds)
		{
			// Run only when Sound is valid.

			if (Sound)
			{
				auto bShouldCreateNew{ true };

				// If Component already exists. Reuse it.

				if (AudioComponents.IsValidIndex(Index))
				{
					if (auto AudioComponent{ AudioComponents[Index] })
					{
						AudioComponent->SetSound(Sound);

						bShouldCreateNew = false;
					}
				}

				// If no Component. Newly spawned

				if (bShouldCreateNew)
				{
					auto* AudioComponent
					{
						UGameplayStatics::SpawnSoundAttached(
							Sound
							, AttachToComponent
							, AttachPointName
							, LocationOffset
							, RotationOffset
							, LocationType
							, true
							, VolumeMultiplier
							, PitchMultiplier
							, 0.0f
							, nullptr
							, nullptr
							, false
						)
					};

					// Store created components

					AudioComponents.EmplaceAt(Index, AudioComponent);
				}
			}

			Index++;
		}

		Index = 0;

		// Play Niagara system. If there is already an NiagaraComponent, reuse it; if not, create a new one.

		const auto& NiagaraSystems{ FoundEffect->NiagaraSystems };
		auto& NiagaraComponents{ FoundEffect->ActiveNiagaraComponents };

		for (const auto& NiagaraSystem : NiagaraSystems)
		{
			// Run only when Niagara system is valid.

			if (NiagaraSystem)
			{
				auto bShouldCreateNew{ true };

				// If Component already exists. Reuse it.

				if (NiagaraComponents.IsValidIndex(Index))
				{
					if (auto NiagaraComponent{ NiagaraComponents[Index] })
					{
						NiagaraComponent->SetAsset(NiagaraSystem);

						bShouldCreateNew = false;
					}
				}

				// If no Component. Newly spawned

				if (bShouldCreateNew)
				{
					auto* NiagaraComponent
					{
						UNiagaraFunctionLibrary::SpawnSystemAttached(
							NiagaraSystem
							, AttachToComponent
							, AttachPointName
							, LocationOffset
							, RotationOffset
							, VFXScale
							, LocationType
							, false
							, ENCPoolMethod::None
						)
					};

					// Store created components

					NiagaraComponents.EmplaceAt(Index, NiagaraComponent);
				}
			}

			Index++;
		}
	}
}

FActiveContextEffect* UActiveContextEffectLibrary::FindEffectPtr(const FGameplayTagContainer& Contexts)
{
	for (auto& Effect : ActiveContextEffects)
	{
		if (Effect.Context.HasAllExact(Contexts))
		{
			return &Effect;
		}
	}

	return nullptr;
}
