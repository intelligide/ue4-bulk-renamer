// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BulkRenamingRule.h"
#include "ReplaceRule.generated.h"

UCLASS()
class UReplaceRule : public UBulkRenamingRule
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString TextToBeReplaced;

	UPROPERTY(EditAnywhere)
	FString ReplaceWith;

	UPROPERTY(EditAnywhere)
	bool CaseSensitive = false;
	
	FString Apply(const FString&) const override;
};
