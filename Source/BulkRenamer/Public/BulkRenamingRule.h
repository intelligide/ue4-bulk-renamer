// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"
#include "BulkRenamingRule.generated.h"

class UBulkRenamingRule;

UCLASS(Abstract)
class UBulkRenamingRule: public UObject
{
	GENERATED_BODY()
public:
	virtual FString Apply(const FString&) const
	{
		return "";
	}
};