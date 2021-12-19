// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"
#include "BulkRenamingRuleFactory.generated.h"

class UBulkRenamingRule;

UCLASS(Abstract)
class UBulkRenamingRuleFactory : public UObject
{
	GENERATED_BODY()
public:
	virtual FText GetDisplayName()
	{
		return FText::GetEmpty();
	}

	virtual UBulkRenamingRule* Create()
	{
		return nullptr;
	}
};
