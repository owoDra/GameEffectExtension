// Copyright (C) 2023 owoDra

#pragma once

#include "UObject/Interface.h"

#include "ContextEffectTypes.h"

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
	void ContextEffectAction(FContextEffectGenericParameter Param);

 };

