// Copyright (C) 2023 owoDra

#pragma once

#include "Engine/DeveloperSettings.h"

#include "GameplayTagContainer.h"

#include "EffectDeveloperSettings.generated.h"


/**
 * Settings for a loading screen system.
 */
UCLASS(Config = "Game", Defaultconfig, meta = (DisplayName = "Game Effect Extension"))
class GFXEXT_API UEffectDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UEffectDeveloperSettings();

	///////////////////////////////////////////////
	// General
public:
	//
	UPROPERTY(Config, EditAnywhere, Category = "General", meta = (Categories = "SurfaceType"))
	TMap<TEnumAsByte<EPhysicalSurface>, FGameplayTag> SurfaceTypeToContextMap;

};
