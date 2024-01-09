// Copyright (C) 2024 owoDra

#pragma once

#include "Logging/LogMacros.h"

GFXEXT_API DECLARE_LOG_CATEGORY_EXTERN(LogGFXE, Log, All);

#if !UE_BUILD_SHIPPING

#define GFXELOG(FormattedText, ...) UE_LOG(LogGFXE, Log, FormattedText, __VA_ARGS__)

#define GFXEENSURE(InExpression) ensure(InExpression)
#define GFXEENSURE_MSG(InExpression, InFormat, ...) ensureMsgf(InExpression, InFormat, __VA_ARGS__)
#define GFXEENSURE_ALWAYS_MSG(InExpression, InFormat, ...) ensureAlwaysMsgf(InExpression, InFormat, __VA_ARGS__)

#define GFXECHECK(InExpression) check(InExpression)
#define GFXECHECK_MSG(InExpression, InFormat, ...) checkf(InExpression, InFormat, __VA_ARGS__)

#else

#define GFXELOG(FormattedText, ...)

#define GFXEENSURE(InExpression) InExpression
#define GFXEENSURE_MSG(InExpression, InFormat, ...) InExpression
#define GFXEENSURE_ALWAYS_MSG(InExpression, InFormat, ...) InExpression

#define GFXECHECK(InExpression) InExpression
#define GFXECHECK_MSG(InExpression, InFormat, ...) InExpression

#endif