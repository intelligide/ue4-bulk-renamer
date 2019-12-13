// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BulkRenamingRuleFactory.h"
#include "MoveRule.h"
#include "MoveRuleFactory.generated.h"

UCLASS()
class UMoveRuleFactory : public UBulkRenamingRuleFactory
{
	GENERATED_BODY()
public:
	inline FText GetDisplayName() override
	{
		return FText::FromString("Move");
	}

	inline UBulkRenamingRule* Create() override
	{
		return NewObject<UMoveRule>();
	}
};