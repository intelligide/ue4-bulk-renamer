// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BulkRenamingRuleFactory.h"
#include "AddRule.h"
#include "AddRuleFactory.generated.h"

UCLASS()
class UAddRuleFactory : public UBulkRenamingRuleFactory
{
	GENERATED_BODY()
public:
	inline FText GetDisplayName() override
	{
		return FText::FromString("Add");
	}

	inline UBulkRenamingRule* Create() override
	{
		return NewObject<UAddRule>();
	}
};
