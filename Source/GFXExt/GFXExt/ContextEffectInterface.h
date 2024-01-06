// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Interface.h"

#include "GameplayTagContainer.h"

#include "ContextEffectInterface.generated.h"


 UINTERFACE(Blueprintable)
 class GFXEXT_API UContextEffectInterface : public UInterface
 {
	 GENERATED_BODY()
 };
 
 class GFXEXT_API IContextEffectInterface : public IInterface
 {
	 GENERATED_BODY()

 public:
 	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayEffects(
		FGameplayTag EffectTag
		, FGameplayTagContainer Contexts
		, USceneComponent* AttachToComponent	= nullptr
		, FName AttachPointName					= NAME_None
		, FVector LocationOffset				= FVector(0.0, 0.0, 0.0)
		, FRotator RotationOffset				= FRotator(0.0, 0.0, 0.0)
		, EAttachLocation::Type LocationType	= EAttachLocation::KeepRelativeOffset
		, float VolumeMultiplier				= 1.0f
		, float PitchMultiplier					= 1.0f
		, FVector VFXScale						= FVector(1.0, 1.0, 1.0)
	);

 };

