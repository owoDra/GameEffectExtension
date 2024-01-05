// Copyright (C) 2023 owoDra

#pragma once

#include "GameplayTagContainer.h"

#include "ContextEffectTypes.generated.h"

class USceneComponent;


/**
 * Generic parameters by action for ContextEffect
 */
USTRUCT(BlueprintType)
struct FContextEffectGenericParameter
{
	GENERATED_BODY()
public:
	FContextEffectGenericParameter() {}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag EffectTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTagContainer Contexts;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector LocationOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator RotationOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHitSuccess;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FHitResult HitResult;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Bone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TWeakObjectPtr<USceneComponent> Component;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector VFXScale{ FVector::OneVector };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AudioVolume{ 1.0f };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AudioPitch{ 1.0f };

};
