// Copyright (C) 2023 owoDra

#include "ContextEffectLibraryFactory.h"

#include "ContextEffectLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ContextEffectLibraryFactory)


UContextEffectLibraryFactory::UContextEffectLibraryFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UContextEffectLibrary::StaticClass();

	bCreateNew = true;
	bEditorImport = false;
	bEditAfterNew = true;
}


UObject* UContextEffectLibraryFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	auto* ContextEffectLibrary{ NewObject<UContextEffectLibrary>(InParent, Name, Flags) };

	return ContextEffectLibrary;
}
