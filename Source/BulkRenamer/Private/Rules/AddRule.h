// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BulkRenamingRule.h"
#include "AddRule.generated.h"

UCLASS()
class UAddRule : public UBulkRenamingRule
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString Str;
	
	UPROPERTY(EditAnywhere)
	uint32 Index = 0;

	UPROPERTY(EditAnywhere)
	bool Backward = false;
	
	FString Apply(const FString&) const override;
};
