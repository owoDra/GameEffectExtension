// Copyright (C) 2023 owoDra

#include "ContextEffectLibrary.h"

#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ContextEffectLibrary)


UContextEffectLibrary::UContextEffectLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UContextEffectLibrary::GetEffects(
	const FGameplayTag Effect, 
	const FGameplayTagContainer Context, 
	TArray<USoundBase*>& OutSounds, 
	TArray<UNiagaraSystem*>& OutNiagaraSystems)
{
	// Make sure Effect is valid and Library is loaded

	if (Effect.IsValid() && Context.IsValid() && EffectsLoadState == EContextEffectLibraryLoadState::Loaded)
	{
		for (const auto& ActiveContextEffect : ActiveContextEffects)
		{
			// Make sure the Effect is an exact Tag Match and ensure the Context has all tags in the Effect (and neither or both are empty)

			if (Effect.MatchesTagExact(ActiveContextEffect.EffectTag)
				&& Context.HasAllExact(ActiveContextEffect.Context)
				&& (ActiveContextEffect.Context.IsEmpty() == Context.IsEmpty()))
			{
				// Get all Matching Sounds and Niagara Systems

				OutSounds.Append(ActiveContextEffect.Sounds);
				OutNiagaraSystems.Append(ActiveContextEffect.NiagaraSystems);
			}
		}
	}
}


void UContextEffectLibrary::LoadEffects()
{
	// Load Effects into Library if not currently loading

	if (EffectsLoadState != EContextEffectLibraryLoadState::Loading)
	{
		// Set load state to loading

		EffectsLoadState = EContextEffectLibraryLoadState::Loading;

		// Clear out any old Active Effects

		ActiveContextEffects.Empty();

		// Call internal loading function

		LoadEffectsInternal();
	}
}

void UContextEffectLibrary::LoadEffectsInternal()
{
	// @TODO: Add Async Loading for Libraries

	TArray<FActiveContextEffect> NewActiveContextEffectArray;

	for (const auto& ContextEffect : ContextEffects)
	{
		if (ContextEffect.EffectTag.IsValid() && ContextEffect.Context.IsValid())
		{
			auto& NewActiveContextEffect{ NewActiveContextEffectArray.AddDefaulted_GetRef() };
			NewActiveContextEffect.EffectTag = ContextEffect.EffectTag;
			NewActiveContextEffect.Context = ContextEffect.Context;

			// Try to load and add Effects to New Active Context Effects

			for (const auto& Effect : ContextEffect.Effects)
			{
				if (auto* Object{ Effect.TryLoad() })
				{
					if (auto* SoundBase{ Cast<USoundBase>(Object) })
					{
						NewActiveContextEffect.Sounds.Add(SoundBase);
					}
					else if (auto* NiagaraSystem{ Cast<UNiagaraSystem>(Object) })
					{
						NewActiveContextEffect.NiagaraSystems.Add(NiagaraSystem);
					}
				}
			}
		}
	}

	// @TODO: Call Load Complete after Async Load

	ContextEffectLibraryLoadingComplete(NewActiveContextEffectArray);
}

void UContextEffectLibrary::ContextEffectLibraryLoadingComplete(const TArray<FActiveContextEffect>& NewActiveContextEffect)
{
	EffectsLoadState = EContextEffectLibraryLoadState::Loaded;

	ActiveContextEffects.Append(NewActiveContextEffect);
}
