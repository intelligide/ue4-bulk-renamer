// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BulkRenamingRuleFactory.h"
#include "ReplaceRule.h"
#include "ReplaceRuleFactory.generated.h"

UCLASS()
class UReplaceRuleFactory : public UBulkRenamingRuleFactory
{
	GENERATED_BODY()
public:
	inline FText GetDisplayName() override
	{
		return FText::FromString("Replace");
	}

	inline UBulkRenamingRule* Create() override
	{
		return NewObject<UReplaceRule>();
	}
};