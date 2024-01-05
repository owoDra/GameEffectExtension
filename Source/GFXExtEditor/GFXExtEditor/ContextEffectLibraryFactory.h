// Copyright (C) 2023 owoDra

#pragma once

#include "Factories/Factory.h"

#include "ContextEffectLibraryFactory.generated.h"

class FFeedbackContext;
class UClass;
class UObject;


UCLASS(hidecategories = Object, MinimalAPI)
class UContextEffectLibraryFactory : public UFactory
{
	GENERATED_BODY()
public:
	UContextEffectLibraryFactory(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

	virtual bool ShouldShowInNewMenu() const override { return true; }

};
