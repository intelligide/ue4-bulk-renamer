// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BulkRenamingRuleFactory.h"
#include "RemoveRule.h"
#include "RemoveRuleFactory.generated.h"

UCLASS()
class URemoveRuleFactory : public UBulkRenamingRuleFactory
{
	GENERATED_BODY()
public:
	inline FText GetDisplayName() override
	{
		return FText::FromString("Remove");
	}

	inline UBulkRenamingRule* Create() override
	{
		return NewObject<URemoveRule>();
	}
};